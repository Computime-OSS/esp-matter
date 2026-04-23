/*
 *
 *    Copyright (c) 2024 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */


#include "helpers.h"

#include "nvs_flash.h"

#include <esp_sntp.h>
#include <lib/support/CHIPMemString.h>
#include <lib/support/logging/CHIPLogging.h>
#include <lib/support/TimeUtils.h>

#include "TimeSync.h"

static constexpr time_t kMinValidTimeStampEpoch = 1704067200; // 1 Jan 2019
static constexpr uint32_t kMilliSecondsInADay   = 24 * 60 * 60 * 1000;

namespace {
const uint8_t kMaxNtpServerStringSize = 128;
char sSntpServerName[kMaxNtpServerStringSize + 1];

CHIP_ERROR GetLocalTimeString(char * buf, size_t buf_len)
{
    VerifyOrReturnError(buf_len > 0, CHIP_ERROR_INVALID_ARGUMENT);
    struct tm timeinfo;
    char strftime_buf[64];
    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);
    if (strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%dT%H:%M:%S%z", &timeinfo) == 0)
    {
        PRINTF_DEBUG("Buffer too small");
        return CHIP_ERROR_BUFFER_TOO_SMALL;
    }
    size_t print_size = snprintf(buf, buf_len, "%s, DST: %s", strftime_buf, timeinfo.tm_isdst ? "Yes" : "No");
    if (print_size >= (buf_len - 1))
    {
        PRINTF_DEBUG("Buffer size %d insufficient for localtime string. Required size: %d", static_cast<int>(buf_len),
                     static_cast<int>(print_size));
        return CHIP_ERROR_BUFFER_TOO_SMALL;
    }
    return CHIP_NO_ERROR;
}

bool ValidateTime()
{
    time_t now;
    time(&now);
    return (now > kMinValidTimeStampEpoch);
}

CHIP_ERROR PrintCurrentTime()
{
    char local_time[64] = { 0 };
    ReturnErrorOnFailure(GetLocalTimeString(local_time, sizeof(local_time)));
    if (!ValidateTime())
    {
        PRINTF_DEBUG("Time not synchronised yet.");
        return CHIP_ERROR_INCORRECT_STATE;
    }
    PRINTF_DEBUG("The current time is: %s.", local_time);
    return CHIP_NO_ERROR;
}

void TimeSyncCallback(struct timeval * tv)
{
    if (PrintCurrentTime() != CHIP_NO_ERROR)
    {
        DEBUG_CHECKPOINT("SNTP time is not synchronised.");
        return;
    }

    nvs_handle_t my_handle;
    esp_err_t err;

    // 1. Open NVS namespace "storage"
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        PRINTF_DEBUG("Error opening NVS handle: %s", esp_err_to_name(err));
        return;
    }

    // 2. Save the seconds and microseconds (persistent time)
    // Using int64_t for seconds ensures future-proofing (Year 2038)
    nvs_set_i64(my_handle, "sync_sec", (int64_t)tv->tv_sec);
    nvs_set_i32(my_handle, "sync_usec", (int32_t)tv->tv_usec);

    // 3. Commit the changes to flash
    err = nvs_commit(my_handle);
    if (err == ESP_OK) {
        PRINTF_DEBUG("Time synced and saved to NVS: %lld", (int64_t)tv->tv_sec);
    }

    // 4. Close the handle
    nvs_close(my_handle);

    DEBUG_CHECKPOINT("SNTP Time Synchronized!");
}

} // anonymous namespace

namespace chip {
namespace Esp32TimeSync {
void Init(const char * aSntpServerName, const uint16_t aSyncSntpIntervalDay)
{
    if (!aSyncSntpIntervalDay)
    {
        PRINTF_DEBUG("Invalid SNTP synchronization time interval.");
        return;
    }
    chip::Platform::CopyString(sSntpServerName, aSntpServerName);
    if (esp_sntp_enabled())
    {
        PRINTF_DEBUG("SNTP already initialized.");
    }
    PRINTF_DEBUG("Initializing SNTP. Using the SNTP server: %s", sSntpServerName);
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, sSntpServerName);

    esp_sntp_setservername(1, "time.salusconnect.io");
	esp_sntp_setservername(2, "time1.salusconnect.io");

    // esp_sntp_set_sync_interval(kMilliSecondsInADay * aSyncSntpIntervalDay);
    sntp_set_time_sync_notification_cb(TimeSyncCallback);
    esp_sntp_init();
}
} // namespace Esp32TimeSync
} // namespace chip

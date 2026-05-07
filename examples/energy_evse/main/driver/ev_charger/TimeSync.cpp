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
#include <lib/support/logging/CHIPLogging.h>
#include <lib/support/TimeUtils.h>
#include <array>
#include <string>

#include "TimeSync.h"

static constexpr time_t kMinValidTimeStampEpoch = 1704067200; // 1 Jan 2019
static constexpr uint32_t kMilliSecondsInADay   = 24 * 60 * 60 * 1000;

namespace {

constexpr uint8_t kMaxNtpServerStringSize = 128;

/** SNTP retains the hostname pointer; lifetime must span the process — static local avoids a mutable TU-scope global. */
std::string & SntpServerNameStorage()
{
    static std::string sName;
    return sName;
}

CHIP_ERROR GetLocalTimeString(std::string & localTime)
{
    struct tm timeinfo;
    std::array<char, 64> strftimeBuf = {};
    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);
    if (strftime(strftimeBuf.data(), strftimeBuf.size(), "%Y-%m-%dT%H:%M:%S%z", &timeinfo) == 0)
    {
        PRINTF_DEBUG("Buffer too small");
        return CHIP_ERROR_BUFFER_TOO_SMALL;
    }
    localTime.assign(strftimeBuf.data());
    localTime += ", DST: ";
    localTime += timeinfo.tm_isdst ? "Yes" : "No";
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
    std::string localTime;
    ReturnErrorOnFailure(GetLocalTimeString(localTime));
    if (!ValidateTime())
    {
        PRINTF_DEBUG("Time not synchronised yet.");
        return CHIP_ERROR_INCORRECT_STATE;
    }
    PRINTF_DEBUG("The current time is: %s.", localTime.c_str());
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
    auto & sntpServerName = SntpServerNameStorage();
    sntpServerName        = (aSntpServerName != nullptr) ? aSntpServerName : "";
    if (sntpServerName.size() > kMaxNtpServerStringSize)
    {
        sntpServerName.resize(kMaxNtpServerStringSize);
    }
    if (esp_sntp_enabled())
    {
        PRINTF_DEBUG("SNTP already initialized.");
    }
    PRINTF_DEBUG("Initializing SNTP. Using the SNTP server: %s", sntpServerName.c_str());
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, sntpServerName.c_str());

    esp_sntp_setservername(1, "time.salusconnect.io");
	esp_sntp_setservername(2, "time1.salusconnect.io");

    sntp_set_time_sync_notification_cb(TimeSyncCallback);
    esp_sntp_init();
}
} // namespace Esp32TimeSync
} // namespace chip

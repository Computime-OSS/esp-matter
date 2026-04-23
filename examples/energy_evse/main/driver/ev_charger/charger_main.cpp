#include <esp_check.h>
#include <esp_log.h>
#include <helpers.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "chargerManager.h"
#include "charger_main.h"
#include "hardwareControlInterface.h"

void GetReadableTime(uint32_t matterEpoch_s, char * outStr, size_t outSize)
{
    if (matterEpoch_s == 0)
    {
        snprintf(outStr, outSize, "NOT SET");
        return;
    }

    // Convert Matter Epoch (starts 2000) to Unix Epoch (starts 1970)
    // 946684800 is the offset defined in the Matter Specification
    time_t unixTime = static_cast<time_t>(matterEpoch_s + chip::kChipEpochSecondsSinceUnixEpoch);
    
    struct tm timeInfo;
    localtime_r(&unixTime, &timeInfo);
    
    // Format: MM/DD HH:MM
    strftime(outStr, outSize, "%d/%m/%Y %H:%M", &timeInfo);
}

namespace CT {
namespace Charger {

void init()
{
    (void)HardwareControlInterface::Instance().init();
    DEBUG_CHECKPOINT("Charger HW init done.");
}

void start()
{
    //this instance exec the default init
    (void)ChargerManager::Controller();
    DEBUG_CHECKPOINT("Charger SW init done.");
}

} // namespace Charger
} // namespace CT
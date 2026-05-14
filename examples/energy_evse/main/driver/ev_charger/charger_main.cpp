#include <esp_check.h>
#include <esp_log.h>
#include <helpers.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "chargerManager.h"
#include "charger_main.h"
#include "hardwareControlInterface.h"

#include "get_readable_time.h"

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
/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "esp_matter.h"
#include "esp_err.h"

#include <device.h>
#include <protocols/Protocols.h>

#include "helpers.h"
#include "chargerManager.h"
#include "matterManager.h"

#include "charger_main.h"

namespace esp_matter {
namespace Charger {

namespace core {
esp_err_t app_driver_init()
{
    PRINTF_DEBUG("Initializing Main Charger ...");
    CT::Charger::init();
    CT::Charger::start();
    
    CT::Charger::MatterManager::GetInstance().Init();

    return ESP_OK;
}
} // namespace core

} // namespace Charger
} // namespace esp_matter


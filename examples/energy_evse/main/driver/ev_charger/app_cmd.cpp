#include <cinttypes>
#include <cstdint>
#include <cstdlib>

#include <esp_check.h>
#include <esp_log.h>

#include <nvs_flash.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_matter_console.h>
#include <esp_matter_core.h>

#include <esp_wifi.h>
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "esp_vfs_usb_serial_jtag.h"
#include "argtable3/argtable3.h"
#include "driver/uart.h"
#include "driver/usb_serial_jtag.h"
#include "hal/uart_types.h"
#include "linenoise/linenoise.h"

#include "helpers.h"
#include "hardwareControlInterface.h"

#include "chargerManager.h"

#include "app_cmd.h"

#define PROMPT_STR "EVC"

namespace esp_matter {
namespace Charger {
namespace console {

static const char *TAG = "charger general command";
static int hw_cmd_handler(int argc, char **argv)
{
    if (argc > 1) {
		using namespace CT::Charger;
        if (!strcasecmp(argv[1], "cable")) {
			if (argc > 2) {
				bool connected = strtol(argv[2], nullptr, 10) != 0;
				HardwareControlInterface::Instance().setCableStatus(connected ? HwCableStatus_t::CONNECTED : HwCableStatus_t::NOT_CONNECTED);
			}
        } else if (!strcasecmp(argv[1], "limit")) {
            if (argc > 2) {
				int64_t limit = strtoll(argv[2], nullptr, 10);
				ChargerManager::Controller().setChargingSessionCurrentLimit(static_cast<int>(limit));
				DEBUG_CHECKPOINT("emulator: charge limit set to %" PRId64 " mA", static_cast<int64_t>(limit));
			}
        } else if (!strcasecmp(argv[1], "ev")) {
			if (argc > 2) {
				bool isDrawing = strtol(argv[2], nullptr, 10) != 0;
				HardwareControlInterface::Instance().setEVDrawing(isDrawing);
				DEBUG_CHECKPOINT("emulator: set EV drawing to %s", isDrawing ? "true" : "false");
			}
		} else if (!strcasecmp(argv[1], "fault")) {
			if (argc > 2) {
				uint8_t faultCode = static_cast<uint8_t>(strtoul(argv[2], nullptr, 10));
				HardwareControlInterface::Instance().setFaultCode(faultCode);
				DEBUG_CHECKPOINT("emulator: set fault code to %" PRIu8, faultCode);
			}
		}
    }

    return 0;
}

static esp_err_t wifi_connect_handler(int argc, char **argv)
{
    ESP_RETURN_ON_FALSE(argc >= 4, ESP_ERR_INVALID_ARG, TAG, "Usage: sw wifi <ssid> <password>");
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_RETURN_ON_ERROR(esp_wifi_init(&cfg), TAG, "Failed to initialize WiFi");
    ESP_RETURN_ON_ERROR(esp_wifi_stop(), TAG, "Failed to stop WiFi");
    ESP_RETURN_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_STA), TAG, "Failed to set WiFi mode");
    wifi_config_t wifi_cfg = {0};
    snprintf((char *)wifi_cfg.sta.ssid, sizeof(wifi_cfg.sta.ssid), "%s", argv[2]);
    snprintf((char *)wifi_cfg.sta.password, sizeof(wifi_cfg.sta.password), "%s", argv[3]);
    ESP_RETURN_ON_ERROR(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg), TAG, "Failed to set WiFi configuration");
    ESP_RETURN_ON_ERROR(esp_wifi_start(), TAG, "Failed to start WiFi");
    ESP_RETURN_ON_ERROR(esp_wifi_connect(), TAG, "Failed to connect WiFi");
    return ESP_OK;
}

static void check_nvs_health() {
    // 1. Initialize NVS with Error Handling (The "Safe" Way)
    esp_err_t ret = nvs_flash_init();
    
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        PRINTF_DEBUG("NVS corruption/truncation detected (0x%X). Erasing partition...", ret);
        // If partition is damaged, we MUST erase it to fix the crash
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 2. Get and Print Partition Statistics
    nvs_stats_t nvs_stats;
    // Use "nvs" for the default partition name from your table
    ret = nvs_get_stats("nvs", &nvs_stats);
    
    if (ret == ESP_OK) {
       PRINTF_DEBUG("===== NVS Partition Stats =====");
       PRINTF_DEBUG("Total Entries:      %d", nvs_stats.total_entries);
       PRINTF_DEBUG("Used Entries:       %d", nvs_stats.used_entries);
       PRINTF_DEBUG("Free Entries:       %d", nvs_stats.free_entries);
       PRINTF_DEBUG("Namespace Count:    %d", nvs_stats.namespace_count);
       PRINTF_DEBUG("===============================");
    } else {
        PRINTF_DEBUG("Failed to get NVS stats (0x%X)", ret);
    }

    // 3. List All Keys (Optional - useful for debugging what's inside)
    nvs_iterator_t it = NULL;
    esp_err_t res = nvs_entry_find(NVS_DEFAULT_PART_NAME, NULL, NVS_TYPE_ANY, &it);
    while (res == ESP_OK) {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        PRINTF_DEBUG("Key found: '%s' in namespace '%s', Type: %d", 
                 info.key, info.namespace_name, info.type);
        res = nvs_entry_next(&it);
    }
    nvs_release_iterator(it);
}

static int sw_cmd_handler(int argc, char **argv)
{
    if (argc > 1) 
	{
        if (!strcasecmp(argv[1], "factoryreset")) 
		{
			(void)esp_matter::factory_reset();
		}
        else if (!strcasecmp(argv[1], "nvs"))
		{
			if(argc > 2){
				if (!strcasecmp(argv[2], "erase"))
				{
					esp_err_t err = nvs_flash_erase();
					PRINTF_DEBUG("NVS erase %s", err==ESP_OK?"Done":"Failed");
				}
				else if (!strcasecmp(argv[2], "info"))
				{
					check_nvs_health();
				}
			}
		}
        else if (!strcasecmp(argv[1], "reboot"))
		{
			esp_restart();
		}
        else if (!strcasecmp(argv[1], "wifi"))
		{
			if (argc > 3)
			{
				wifi_connect_handler(argc, argv);
			}
		}
        else if (!strcasecmp(argv[1], "device")) 
		{
			CT::Charger::ChargerManager::Controller().showChargerDetails();
        }
		else if (!strcasecmp(argv[1], "start"))
		{
			CT::Charger::ChargerManager::Controller().startChargingSession_WithCard("04482B6A116280");
		}
		else if (!strcasecmp(argv[1], "stop"))
		{
			CT::Charger::ChargerManager::Controller().stopChargingSession_WithCard("04482B6A116280");
		}
		else if (!strcasecmp(argv[1], "info"))
		{
			CT::Charger::ChargerManager::Controller().showChargingSessionInfo();
		}
		else if (!strcasecmp(argv[1], "current"))
		{
			uint32_t data1 = 0;
			if ((argc > 2) && (sscanf(argv[2], "%d", &data1) > 0))
			{
				PRINTF_DEBUG("set charging current limit to %d mA", data1);
				CT::Charger::ChargerManager::Controller().setChargingSessionCurrentLimit(data1);
			}
		}
    }
    
    return 0;
}

static int nfc_cmd_handler(int argc, char **argv)
{
	if (argc > 1)
	{
		if (!strcmp(argv[1], "set"))
		{
			CT::Charger::ChargerManager::Controller().processDetectedCard("04482B6A116280");
		}
	}

	return 0;
}

static void charger_commands_register()
{
    const esp_console_cmd_t cmd_list[] = {
        {.command = "hw",
         .help = "emulator: hw cable <0|1> | limit <mA> | ev <0|1> | fault <0-255>",
         .func = hw_cmd_handler},
        {.command = "sw", .help = "control sw command", .func = sw_cmd_handler},
        {.command = "nfc", .help = "nfc commands", .func = nfc_cmd_handler},
    };
	
    // Loop to register each command
    for (size_t i = 0; i < sizeof(cmd_list) / sizeof(esp_console_cmd_t); i++) 
	{
        esp_console_cmd_register(&cmd_list[i]);
    }
}

void init()
{
    charger_commands_register();
}

} // namespace console
} // namespace Charger
} // namespace esp_matter

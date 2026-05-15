#include <cinttypes>
#include <cstdint>
#include <cstdlib>
#include <iterator>

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

namespace esp_matter {
namespace Charger {
namespace console {

namespace {
[[maybe_unused]] constexpr const char kPromptStr[] = "EVC";
} // namespace

const char * const TAG = "charger general command";

void hw_exec_cable(int argc, char ** argv)
{
    if (argc <= 2)
    {
        return;
    }
    auto const connected = strtol(argv[2], nullptr, 10) != 0;
    CT::Charger::HardwareControlInterface::Instance().setCableStatus(
        connected ? CT::Charger::HwCableStatus_t::CONNECTED : CT::Charger::HwCableStatus_t::NOT_CONNECTED);
}

void hw_exec_limit(int argc, char ** argv)
{
    if (argc <= 2)
    {
        return;
    }
    auto const limit = strtoll(argv[2], nullptr, 10);
    CT::Charger::ChargerManager::Controller().setChargingSessionCurrentLimit(static_cast<int>(limit));
    DEBUG_CHECKPOINT("emulator: charge limit set to %" PRId64 " mA", static_cast<int64_t>(limit));
}

void hw_exec_ev(int argc, char ** argv)
{
    if (argc <= 2)
    {
        return;
    }
    auto const isDrawing = strtol(argv[2], nullptr, 10) != 0;
    CT::Charger::HardwareControlInterface::Instance().setEVDrawing(isDrawing);
    DEBUG_CHECKPOINT("emulator: set EV drawing to %s", isDrawing ? "true" : "false");
}

void hw_exec_fault(int argc, char ** argv)
{
    if (argc <= 2)
    {
        return;
    }
    auto const faultCode = static_cast<uint8_t>(strtoul(argv[2], nullptr, 10));
    CT::Charger::HardwareControlInterface::Instance().setFaultCode(faultCode);
    DEBUG_CHECKPOINT("emulator: set fault code to %" PRIu8, faultCode);
}

int hw_cmd_handler(int argc, char ** argv)
{
    if (argc <= 1)
    {
        return 0;
    }
    if (!strcasecmp(argv[1], "cable"))
    {
        hw_exec_cable(argc, argv);
        return 0;
    }
    if (!strcasecmp(argv[1], "limit"))
    {
        hw_exec_limit(argc, argv);
        return 0;
    }
    if (!strcasecmp(argv[1], "ev"))
    {
        hw_exec_ev(argc, argv);
        return 0;
    }
    if (!strcasecmp(argv[1], "fault"))
    {
        hw_exec_fault(argc, argv);
        return 0;
    }
    return 0;
}

void sw_exec_current(int argc, char ** argv)
{
    if (argc <= 2)
    {
        return;
    }
    uint32_t data1 = 0;
    if (sscanf(argv[2], "%" SCNu32, &data1) <= 0)
    {
        return;
    }
    PRINTF_DEBUG("set charging current limit to %" PRIu32 " mA", data1);
    CT::Charger::ChargerManager::Controller().setChargingSessionCurrentLimit(data1);
}

int sw_cmd_handler(int argc, char ** argv)
{
    if (argc <= 1)
    {
        return 0;
    }
    if (!strcasecmp(argv[1], "factoryreset"))
    {
        (void) esp_matter::factory_reset();
        return 0;
    }
    if (!strcasecmp(argv[1], "nvs"))
    {
        sw_exec_nvs(argc, argv);
        return 0;
    }
    if (!strcasecmp(argv[1], "reboot"))
    {
        esp_restart();
        return 0;
    }
    if (!strcasecmp(argv[1], "device"))
    {
        CT::Charger::ChargerManager::Controller().showChargerDetails();
        return 0;
    }
    if (!strcasecmp(argv[1], "start"))
    {
        CT::Charger::ChargerManager::Controller().startChargingSession_WithCard("04482B6A116280");
        return 0;
    }
    if (!strcasecmp(argv[1], "stop"))
    {
        CT::Charger::ChargerManager::Controller().stopChargingSession_WithCard("04482B6A116280");
        return 0;
    }
    if (!strcasecmp(argv[1], "info"))
    {
        CT::Charger::ChargerManager::Controller().showChargingSessionInfo();
        return 0;
    }
    if (!strcasecmp(argv[1], "current"))
    {
        sw_exec_current(argc, argv);
        return 0;
    }
    return 0;
}

int nfc_cmd_handler(int argc, char **argv)
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

void charger_commands_register()
{
    const esp_console_cmd_t cmd_list[] = {
        {.command = "hw",
         .help = "emulator: hw cable <0|1> | limit <mA> | ev <0|1> | fault <0-255>",
         .func = hw_cmd_handler},
        {.command = "sw", .help = "control sw command", .func = sw_cmd_handler},
        {.command = "nfc", .help = "nfc commands", .func = nfc_cmd_handler},
    };
	
    // Loop to register each command
    for (size_t i = 0; i < std::size(cmd_list); i++)
	{
        esp_console_cmd_register(&cmd_list[i]);
    }
}

} // namespace console
} // namespace Charger
} // namespace esp_matter

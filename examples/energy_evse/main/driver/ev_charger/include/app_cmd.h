#pragma once

namespace esp_matter {
namespace Charger {

namespace console {
    void charger_commands_register();
    void hw_exec_cable(int argc, char ** argv);
    void hw_exec_limit(int argc, char ** argv);
    void hw_exec_ev(int argc, char ** argv);
    void hw_exec_fault(int argc, char ** argv);
    int hw_cmd_handler(int argc, char ** argv);
    void sw_exec_nvs(int argc, char ** argv);
    void sw_exec_current(int argc, char ** argv);
    int sw_cmd_handler(int argc, char ** argv);
    int nfc_cmd_handler(int argc, char **argv);
} // namespace console

} // namespace Charger
} // namespace esp_matter

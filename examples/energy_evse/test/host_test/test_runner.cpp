#include "unity.h"

#include "test_charger_declarations.h"

void setUp(void) {}

void tearDown(void) {}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    UNITY_BEGIN();

    run_test_charger_app_cmd_tests();
    run_test_charger_uiux_handler_tests();
    run_test_charger_manager_tests();
    run_test_charger_hardware_control_tests();
    run_test_charger_unit_other_tests();
    run_test_charger_session_math_tests();

    return UNITY_END();
}

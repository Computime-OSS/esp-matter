#pragma once

/// Call between `UNITY_BEGIN()` and `UNITY_END()`; registers all tests for `app_cmd.cpp`.
void run_test_charger_app_cmd_tests(void);

/// Registers tests for `charger_session_math.cpp`.
void run_test_charger_session_math_tests(void);

/// Registers tests for `calculate_energy.cpp` and `get_readable_time.cpp`.
void run_test_charger_unit_other_tests(void);

/// Registers tests for `charger_uiux_handler.cpp`.
void run_test_charger_uiux_handler_tests(void);

/// Registers tests for `chargerManager.cpp`.
void run_test_charger_manager_tests(void);

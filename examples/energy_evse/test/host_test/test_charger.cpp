#include "unity.h"

#include "app_cmd.h"

#include "calculate_energy.h"
#include "charger_session_math.h"
#include "get_readable_time.h"

void setUp(void) {
    // Run before every test
}

void tearDown(void) {
    // Run after every test
}

static void test_esp_charger_commands_register_noop(void) {
    esp_charger_commands_register();
}

static void test_hw_exec_cable_early_return_low_argc(void) {
    char a0[] = "hw";
    char *argv[] = { a0 };
    hw_exec_cable(1, argv);
}

static void test_hw_exec_cable_parses_zero(void) {
    char a0[] = "hw";
    char a1[] = "cable";
    char a2[] = "0";
    char *argv[] = { a0, a1, a2 };
    hw_exec_cable(3, argv);
}

static void test_hw_exec_cable_parses_nonzero(void) {
    char a0[] = "hw";
    char a1[] = "cable";
    char a2[] = "1";
    char *argv[] = { a0, a1, a2 };
    hw_exec_cable(3, argv);
}

static void test_hw_exec_limit_early_return(void) {
    char a0[] = "hw";
    char *argv[] = { a0 };
    hw_exec_limit(1, argv);
}

static void test_hw_exec_limit_parses_value(void) {
    char a0[] = "hw";
    char a1[] = "limit";
    char a2[] = "32000";
    char *argv[] = { a0, a1, a2 };
    hw_exec_limit(3, argv);
}

static void test_hw_exec_ev_early_return(void) {
    char a0[] = "hw";
    char *argv[] = { a0 };
    hw_exec_ev(1, argv);
}

static void test_hw_exec_ev_parses_drawing(void) {
    char a0[] = "hw";
    char a1[] = "ev";
    char a2[] = "1";
    char *argv[] = { a0, a1, a2 };
    hw_exec_ev(3, argv);
}

static void test_hw_exec_fault_early_return(void) {
    char a0[] = "hw";
    char *argv[] = { a0 };
    hw_exec_fault(1, argv);
}

static void test_hw_exec_fault_parses_code(void) {
    char a0[] = "hw";
    char a1[] = "fault";
    char a2[] = "42";
    char *argv[] = { a0, a1, a2 };
    hw_exec_fault(3, argv);
}

static void test_hw_cmd_handler_argc_le_one_returns_zero(void) {
    char a0[] = "hw";
    char *argv[] = { a0 };
    TEST_ASSERT_EQUAL_INT(0, hw_cmd_handler(1, argv));
}

static void test_hw_cmd_handler_cable_branch_case_insensitive(void) {
    char a0[] = "hw";
    char a1[] = "CaBlE";
    char a2[] = "0";
    char *argv[] = { a0, a1, a2 };
    TEST_ASSERT_EQUAL_INT(0, hw_cmd_handler(3, argv));
}

static void test_hw_cmd_handler_limit_branch(void) {
    char a0[] = "hw";
    char a1[] = "LIMIT";
    char a2[] = "16000";
    char *argv[] = { a0, a1, a2 };
    TEST_ASSERT_EQUAL_INT(0, hw_cmd_handler(3, argv));
}

static void test_hw_cmd_handler_ev_branch(void) {
    char a0[] = "hw";
    char a1[] = "ev";
    char a2[] = "0";
    char *argv[] = { a0, a1, a2 };
    TEST_ASSERT_EQUAL_INT(0, hw_cmd_handler(3, argv));
}

static void test_hw_cmd_handler_fault_branch(void) {
    char a0[] = "hw";
    char a1[] = "fault";
    char a2[] = "255";
    char *argv[] = { a0, a1, a2 };
    TEST_ASSERT_EQUAL_INT(0, hw_cmd_handler(3, argv));
}

static void test_hw_cmd_handler_unknown_subcommand_returns_zero(void) {
    char a0[] = "hw";
    char a1[] = "unknown";
    char *argv[] = { a0, a1 };
    TEST_ASSERT_EQUAL_INT(0, hw_cmd_handler(2, argv));
}

static void test_sw_exec_nvs_noop(void) {
    char a0[] = "sw";
    char *argv[] = { a0 };
    sw_exec_nvs(1, argv);
}

static void test_sw_exec_current_early_return_low_argc(void) {
    char a0[] = "sw";
    char *argv[] = { a0 };
    sw_exec_current(1, argv);
}

static void test_sw_exec_current_invalid_scanf_returns_early(void) {
    char a0[] = "sw";
    char a1[] = "current";
    char a2[] = "not_a_number";
    char *argv[] = { a0, a1, a2 };
    sw_exec_current(3, argv);
}

static void test_sw_exec_current_valid_uint32(void) {
    char a0[] = "sw";
    char a1[] = "current";
    char a2[] = "16000";
    char *argv[] = { a0, a1, a2 };
    sw_exec_current(3, argv);
}

static void test_sw_cmd_handler_argc_le_one(void) {
    char a0[] = "sw";
    char *argv[] = { a0 };
    TEST_ASSERT_EQUAL_INT(0, sw_cmd_handler(1, argv));
}

static void test_sw_cmd_handler_factoryreset(void) {
    char a0[] = "sw";
    char a1[] = "FACTORYRESET";
    char *argv[] = { a0, a1 };
    TEST_ASSERT_EQUAL_INT(0, sw_cmd_handler(2, argv));
}

static void test_sw_cmd_handler_reboot(void) {
    char a0[] = "sw";
    char a1[] = "Reboot";
    char *argv[] = { a0, a1 };
    TEST_ASSERT_EQUAL_INT(0, sw_cmd_handler(2, argv));
}

static void test_sw_cmd_handler_device(void) {
    char a0[] = "sw";
    char a1[] = "device";
    char *argv[] = { a0, a1 };
    TEST_ASSERT_EQUAL_INT(0, sw_cmd_handler(2, argv));
}

static void test_sw_cmd_handler_start(void) {
    char a0[] = "sw";
    char a1[] = "start";
    char *argv[] = { a0, a1 };
    TEST_ASSERT_EQUAL_INT(0, sw_cmd_handler(2, argv));
}

static void test_sw_cmd_handler_stop(void) {
    char a0[] = "sw";
    char a1[] = "stop";
    char *argv[] = { a0, a1 };
    TEST_ASSERT_EQUAL_INT(0, sw_cmd_handler(2, argv));
}

static void test_sw_cmd_handler_info(void) {
    char a0[] = "sw";
    char a1[] = "INFO";
    char *argv[] = { a0, a1 };
    TEST_ASSERT_EQUAL_INT(0, sw_cmd_handler(2, argv));
}

static void test_sw_cmd_handler_current_delegates(void) {
    char a0[] = "sw";
    char a1[] = "current";
    char a2[] = "8000";
    char *argv[] = { a0, a1, a2 };
    TEST_ASSERT_EQUAL_INT(0, sw_cmd_handler(3, argv));
}

static void test_sw_cmd_handler_unknown_returns_zero(void) {
    char a0[] = "sw";
    char a1[] = "nosuch";
    char *argv[] = { a0, a1 };
    TEST_ASSERT_EQUAL_INT(0, sw_cmd_handler(2, argv));
}

static void test_nfc_cmd_handler_argc_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, nfc_cmd_handler(0, nullptr));
}

static void test_nfc_cmd_handler_argc_one_no_crash(void) {
    char a0[] = "nfc";
    char *argv[] = { a0 };
    TEST_ASSERT_EQUAL_INT(0, nfc_cmd_handler(1, argv));
}

static void test_nfc_cmd_handler_set_branch(void) {
    char a0[] = "nfc";
    char a1[] = "set";
    char *argv[] = { a0, a1 };
    TEST_ASSERT_EQUAL_INT(0, nfc_cmd_handler(2, argv));
}

static void test_nfc_cmd_handler_non_set_branch(void) {
    char a0[] = "nfc";
    char a1[] = "clear";
    char *argv[] = { a0, a1 };
    TEST_ASSERT_EQUAL_INT(0, nfc_cmd_handler(2, argv));
}

static void test_nfc_cmd_handler_set_case_sensitive(void) {
    char a0[] = "nfc";
    char a1[] = "SET";
    char *argv[] = { a0, a1 };
    TEST_ASSERT_EQUAL_INT(0, nfc_cmd_handler(2, argv));
}

void test_calculate_energy_standard_value(void) {
    // Test 10A at 230V for 1 hour = 2.3kWh
    float result = calculate_energy(10.0f, 230.0f, 1.0f);
    TEST_ASSERT_EQUAL_FLOAT(2.3f, result);
}

void test_calculate_energy_zero_amps(void) {
    // Test 0A should result in 0kWh
    float result = calculate_energy(0.0f, 230.0f, 1.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, result);
}

void test_epoch_zero(void) {
    char buf[128];

    GetReadableTime(0, buf, sizeof buf);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("NOT SET", buf, "Expected " "NOT SET");
}

void test_epoch_86400(void) {
    char buf[128];

    GetReadableTime(86400u, buf, sizeof buf);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("02/01/2000 00:00", buf, "Expected " "02/01/2000 00:00");
}

void test_session_energy_delivered_mWh(void) {
    uint32_t delivered =
        CT::Charger::session_energy_delivered_mWh(12500, static_cast<int64_t>(2500));
    TEST_ASSERT_EQUAL_UINT32(10000u, delivered);
}

void test_session_time_elapsed_sec_from_monotonic_us(void) {
    int64_t elapsed = CT::Charger::session_time_elapsed_sec_from_monotonic_us(10500000LL, 2500000LL);
    TEST_ASSERT_EQUAL_INT64(8LL, elapsed);
}

void test_charger_thread_tick_aligns_interval(void) {
    TEST_ASSERT_TRUE(CT::Charger::charger_thread_tick_aligns_interval(0u, 1u, 200u));
    TEST_ASSERT_FALSE(CT::Charger::charger_thread_tick_aligns_interval(4u, 1u, 200u));
    TEST_ASSERT_TRUE(CT::Charger::charger_thread_tick_aligns_interval(5u, 1u, 200u));
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    UNITY_BEGIN();

    RUN_TEST(test_esp_charger_commands_register_noop);
    RUN_TEST(test_hw_exec_cable_early_return_low_argc);
    RUN_TEST(test_hw_exec_cable_parses_zero);
    RUN_TEST(test_hw_exec_cable_parses_nonzero);
    RUN_TEST(test_hw_exec_limit_early_return);
    RUN_TEST(test_hw_exec_limit_parses_value);
    RUN_TEST(test_hw_exec_ev_early_return);
    RUN_TEST(test_hw_exec_ev_parses_drawing);
    RUN_TEST(test_hw_exec_fault_early_return);
    RUN_TEST(test_hw_exec_fault_parses_code);
    RUN_TEST(test_hw_cmd_handler_argc_le_one_returns_zero);
    RUN_TEST(test_hw_cmd_handler_cable_branch_case_insensitive);
    RUN_TEST(test_hw_cmd_handler_limit_branch);
    RUN_TEST(test_hw_cmd_handler_ev_branch);
    RUN_TEST(test_hw_cmd_handler_fault_branch);
    RUN_TEST(test_hw_cmd_handler_unknown_subcommand_returns_zero);
    RUN_TEST(test_sw_exec_nvs_noop);
    RUN_TEST(test_sw_exec_current_early_return_low_argc);
    RUN_TEST(test_sw_exec_current_invalid_scanf_returns_early);
    RUN_TEST(test_sw_exec_current_valid_uint32);
    RUN_TEST(test_sw_cmd_handler_argc_le_one);
    RUN_TEST(test_sw_cmd_handler_factoryreset);
    RUN_TEST(test_sw_cmd_handler_reboot);
    RUN_TEST(test_sw_cmd_handler_device);
    RUN_TEST(test_sw_cmd_handler_start);
    RUN_TEST(test_sw_cmd_handler_stop);
    RUN_TEST(test_sw_cmd_handler_info);
    RUN_TEST(test_sw_cmd_handler_current_delegates);
    RUN_TEST(test_sw_cmd_handler_unknown_returns_zero);
    RUN_TEST(test_nfc_cmd_handler_argc_zero);
    RUN_TEST(test_nfc_cmd_handler_argc_one_no_crash);
    RUN_TEST(test_nfc_cmd_handler_set_branch);
    RUN_TEST(test_nfc_cmd_handler_non_set_branch);
    RUN_TEST(test_nfc_cmd_handler_set_case_sensitive);

    RUN_TEST(test_calculate_energy_standard_value);
    RUN_TEST(test_calculate_energy_zero_amps);
    RUN_TEST(test_epoch_zero);
    RUN_TEST(test_epoch_86400);
    RUN_TEST(test_session_energy_delivered_mWh);
    RUN_TEST(test_session_time_elapsed_sec_from_monotonic_us);
    RUN_TEST(test_charger_thread_tick_aligns_interval);

    return UNITY_END();
}
#include "unity.h"

#include "calculate_energy.h"
#include "charger_session_math.h"
#include "get_readable_time.h"

void setUp(void) {
    // Run before every test
}

void tearDown(void) {
    // Run after every test
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
    UNITY_BEGIN();

    RUN_TEST(test_calculate_energy_standard_value);
    RUN_TEST(test_calculate_energy_zero_amps);
    RUN_TEST(test_epoch_zero);
    RUN_TEST(test_epoch_86400);
    RUN_TEST(test_session_energy_delivered_mWh);
    RUN_TEST(test_session_time_elapsed_sec_from_monotonic_us);
    RUN_TEST(test_charger_thread_tick_aligns_interval);

    return UNITY_END();
}
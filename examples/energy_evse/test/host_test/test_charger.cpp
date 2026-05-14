#include "unity.h"
#include "calculate_energy.h"
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
    TEST_ASSERT_EQUAL_STRING_MESSAGE("NOT SET", buf, "Expected NOT SET for epoch 0");
}

int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_calculate_energy_standard_value);
    RUN_TEST(test_calculate_energy_zero_amps);
    RUN_TEST(test_epoch_zero);


    return UNITY_END();
}
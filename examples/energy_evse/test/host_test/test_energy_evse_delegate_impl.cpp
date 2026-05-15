#include "unit_test_exports.h"

#include "unity.h"

static void test_target_skipped_as_past(void)
{
    TEST_ASSERT_TRUE(EnergyEvse_TargetSkippedAsPast(100, 200, false));
    TEST_ASSERT_FALSE(EnergyEvse_TargetSkippedAsPast(300, 200, false));
    TEST_ASSERT_FALSE(EnergyEvse_TargetSkippedAsPast(100, 200, true));
}

void run_test_energy_evse_delegate_impl_tests(void)
{
    RUN_TEST(test_target_skipped_as_past);
}

#include "EnergyEvseDelegateImpl_host.h"

#include "unity.h"

static void test_all_target_days_mask(void) { TEST_ASSERT_EQUAL_UINT8(0x7f, kAllTargetDaysMask); }

void run_test_energy_evse_delegate_impl_header_tests(void)
{
    RUN_TEST(test_all_target_days_mask);
}

#include "matterManager_host_constants.h"

#include "unity.h"

static void test_support_discharging_v2x_false(void)
{
    TEST_ASSERT_FALSE(CT::Charger::kSupportDischargingV2x);
}

void run_test_matter_manager_header_tests(void)
{
    RUN_TEST(test_support_discharging_v2x_false);
}

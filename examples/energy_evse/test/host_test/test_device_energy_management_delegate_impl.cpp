#include "unit_test_exports.h"

#include "unity.h"

using chip::app::Clusters::DeviceEnergyManagement::MergedOptOutState;
using chip::app::Clusters::DeviceEnergyManagement::OptOutStateEnum;

static void test_merged_opt_out_state_combines(void)
{
    const auto merged = MergedOptOutState(OptOutStateEnum::kGridOptOut, OptOutStateEnum::kLocalOptOut);
    TEST_ASSERT_EQUAL(static_cast<int>(OptOutStateEnum::kOptOut), static_cast<int>(merged));
}

static void test_merged_opt_out_state_replaces(void)
{
    const auto merged = MergedOptOutState(OptOutStateEnum::kNoOptOut, OptOutStateEnum::kLocalOptOut);
    TEST_ASSERT_EQUAL(static_cast<int>(OptOutStateEnum::kLocalOptOut), static_cast<int>(merged));
}

void run_test_device_energy_management_delegate_impl_tests(void)
{
    RUN_TEST(test_merged_opt_out_state_combines);
    RUN_TEST(test_merged_opt_out_state_replaces);
}

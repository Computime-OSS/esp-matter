#include "ChargingTargetsMemMgr_host.h"

#include "unity.h"

using namespace chip;
using namespace chip::app::Clusters::EnergyEvse;

static void test_add_and_alloc_copy(void)
{
    ChargingTargetsMemMgr mgr;
    mgr.PrepareDaySchedule(0);
    Structs::ChargingTargetStruct::Type target{};
    target.targetTimeMinutesPastMidnight = 90;
    mgr.AddChargingTarget(target);
    TEST_ASSERT_EQUAL(CHIP_NO_ERROR, mgr.AllocAndCopy());
    TEST_ASSERT_EQUAL(1, mgr.GetNumDailyChargingTargets());
    TEST_ASSERT_NOT_NULL(mgr.GetChargingTargets());
    TEST_ASSERT_EQUAL_UINT16(90, mgr.GetChargingTargets()[0].targetTimeMinutesPastMidnight);
}

static void test_move_transfers_ownership(void)
{
    ChargingTargetsMemMgr mgr;
    mgr.PrepareDaySchedule(0);
    Structs::ChargingTargetStruct::Type target{};
    target.targetTimeMinutesPastMidnight = 120;
    mgr.AddChargingTarget(target);
    TEST_ASSERT_EQUAL(CHIP_NO_ERROR, mgr.AllocAndCopy());

    ChargingTargetsMemMgr moved(std::move(mgr));
    TEST_ASSERT_EQUAL(1, moved.GetNumDailyChargingTargets());
    TEST_ASSERT_NULL(mgr.GetChargingTargets());
}

void run_test_charging_targets_mem_mgr_tests(void)
{
    RUN_TEST(test_add_and_alloc_copy);
    RUN_TEST(test_move_transfers_ownership);
}

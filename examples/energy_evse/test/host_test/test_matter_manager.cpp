#include "matterManager_unit_stub.h"

#include "unity.h"

static void test_matter_manager_init_and_flags(void)
{
    auto &mgr = CT::Charger::MatterManager::GetInstance();
    mgr.resetForTest();
    mgr.Init();
    TEST_ASSERT_TRUE(mgr.GetChargingEnabled());
    TEST_ASSERT_TRUE(mgr.IsChargingAllowedByTargets());
    mgr.SetChargingEnabledForTest(false);
    TEST_ASSERT_FALSE(mgr.GetChargingEnabled());
}

void run_test_matter_manager_tests(void)
{
    RUN_TEST(test_matter_manager_init_and_flags);
}

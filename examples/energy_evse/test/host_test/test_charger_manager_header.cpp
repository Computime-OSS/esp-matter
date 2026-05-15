#include "chargerManager.h"

#include "unity.h"

static void test_charger_manager_thread_ticks(void)
{
    TEST_ASSERT_EQUAL(200, CHARGER_MGR_THREAD_TICKS);
}

static void test_charger_status_enum_values(void)
{
    TEST_ASSERT_TRUE(static_cast<int>(CT::Charger::ChargerStatus_t::AVAILABLE) >= 0);
    TEST_ASSERT_TRUE(static_cast<int>(CT::Charger::ChargerStatus_t::FAULTED) >= 0);
}

void run_test_charger_manager_header_tests(void)
{
    RUN_TEST(test_charger_manager_thread_ticks);
    RUN_TEST(test_charger_status_enum_values);
}

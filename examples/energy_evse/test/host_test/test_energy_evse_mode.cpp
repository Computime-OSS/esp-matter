#include "energy_evse_mode_host.h"

#include "unity.h"

using namespace chip;
using namespace chip::app::Clusters::EnergyEvseMode;

static void test_get_mode_value_by_index(void)
{
    EnergyEvseModeDelegate delegate;
    uint8_t mode = 0xFF;
    TEST_ASSERT_EQUAL(CHIP_NO_ERROR, delegate.GetModeValueByIndex(0, mode));
    TEST_ASSERT_EQUAL_UINT8(kModeManual, mode);
}

static void test_get_mode_value_exhausted(void)
{
    EnergyEvseModeDelegate delegate;
    uint8_t mode = 0;
    TEST_ASSERT_EQUAL(CHIP_ERROR_PROVIDER_LIST_EXHAUSTED, delegate.GetModeValueByIndex(99, mode));
}

static void test_handle_change_to_mode_success(void)
{
    EnergyEvseModeDelegate delegate;
    ModeBase::Commands::ChangeToModeResponse::Type response{};
    delegate.HandleChangeToMode(kModeSolarCharging, response);
    TEST_ASSERT_EQUAL_UINT8(0, response.status);
}

void run_test_energy_evse_mode_tests(void)
{
    RUN_TEST(test_get_mode_value_by_index);
    RUN_TEST(test_get_mode_value_exhausted);
    RUN_TEST(test_handle_change_to_mode_success);
}

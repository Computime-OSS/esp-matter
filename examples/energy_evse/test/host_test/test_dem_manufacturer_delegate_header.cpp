#include "DEMManufacturerDelegate_host.h"

#include "unity.h"

namespace {
class TestDemManufacturerDelegate : public chip::app::Clusters::DeviceEnergyManagement::DEMManufacturerDelegate {
public:
    int64_t GetApproxEnergyDuringSession() override { return 1000; }
};
} // namespace

static void test_default_handlers_return_ok(void)
{
    TestDemManufacturerDelegate delegate;
    TEST_ASSERT_EQUAL(1000, delegate.GetApproxEnergyDuringSession());
    TEST_ASSERT_EQUAL(chip::CHIP_NO_ERROR, delegate.HandleDeviceEnergyManagementCancelRequest());
}

void run_test_dem_manufacturer_delegate_header_tests(void)
{
    RUN_TEST(test_default_handlers_return_ok);
}

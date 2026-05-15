#include "ElectricalPowerMeasurementDelegate_host.h"

#include "unity.h"

using namespace chip::app::Clusters::ElectricalPowerMeasurement;

static void test_measurement_type_count(void)
{
    ElectricalPowerMeasurementDelegate delegate;
    TEST_ASSERT_EQUAL_UINT8(1, delegate.GetNumberOfMeasurementTypes());
}

void run_test_electrical_power_measurement_delegate_tests(void)
{
    RUN_TEST(test_measurement_type_count);
}

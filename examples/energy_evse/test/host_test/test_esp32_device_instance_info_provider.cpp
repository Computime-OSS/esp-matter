#include "ESP32DeviceInstanceInfoProvider.h"

#include "unity.h"

using namespace chip::DeviceLayer;

static void test_vendor_and_product_names(void)
{
    CTLEVDeviceInstanceInfoProvider provider;
    char buf[64] = {};
    TEST_ASSERT_EQUAL(chip::CHIP_NO_ERROR, provider.GetVendorName(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL_STRING("Computime Limited", buf);
    TEST_ASSERT_EQUAL(chip::CHIP_NO_ERROR, provider.GetProductName(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL_STRING("Matter EVSE", buf);
}

static void test_serial_number_from_mac(void)
{
    CTLEVDeviceInstanceInfoProvider provider;
    char buf[32] = {};
    TEST_ASSERT_EQUAL(chip::CHIP_NO_ERROR, provider.GetSerialNumber(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL_STRING("CTLMEV-112233", buf);
}

static void test_manufacturing_date(void)
{
    CTLEVDeviceInstanceInfoProvider provider;
    uint16_t year = 0;
    uint8_t month = 0;
    uint8_t day = 0;
    TEST_ASSERT_EQUAL(chip::CHIP_NO_ERROR, provider.GetManufacturingDate(year, month, day));
    TEST_ASSERT_EQUAL_UINT16(2026, year);
    TEST_ASSERT_EQUAL_UINT8(1, month);
    TEST_ASSERT_EQUAL_UINT8(23, day);
}

void run_test_esp32_device_instance_info_provider_tests(void)
{
    RUN_TEST(test_vendor_and_product_names);
    RUN_TEST(test_serial_number_from_mac);
    RUN_TEST(test_manufacturing_date);
}

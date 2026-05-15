#include "ESP32DeviceInstanceInfoProvider.h"
#include "chip_device_layer.h"
#include "esp_matter_core.h"
#include "esp_mac.h"

#include "unity.h"

using namespace chip;
using namespace chip::DeviceLayer;

namespace {

void reset_harness(void)
{
    unit_test_esp_read_mac_fail() = false;
    esp_matter::ResetFactoryResetCallCountForTest();
    ConfigurationMgr().SetCountryCodeForTest("");
}

} // namespace

static void test_vendor_and_product_names(void)
{
    reset_harness();
    CTLEVDeviceInstanceInfoProvider provider;
    char buf[64] = {};
    TEST_ASSERT_EQUAL(CHIP_NO_ERROR, provider.GetVendorName(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL_STRING("Computime Limited", buf);
    TEST_ASSERT_EQUAL(CHIP_NO_ERROR, provider.GetProductName(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL_STRING("Matter EVSE", buf);
}

static void test_vendor_and_product_ids(void)
{
    reset_harness();
    CTLEVDeviceInstanceInfoProvider provider;
    uint16_t vendorId  = 0;
    uint16_t productId = 0;

    TEST_ASSERT_EQUAL(CHIP_NO_ERROR, provider.GetVendorId(vendorId));
    TEST_ASSERT_EQUAL(CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID, vendorId);
    TEST_ASSERT_EQUAL(CHIP_NO_ERROR, provider.GetProductId(productId));
    TEST_ASSERT_EQUAL(CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_ID, productId);
}

static void test_vendor_and_product_name_buffer_too_small(void)
{
    reset_harness();
    CTLEVDeviceInstanceInfoProvider provider;
    char buf[8] = {};

    TEST_ASSERT_EQUAL(CHIP_ERROR_BUFFER_TOO_SMALL, provider.GetVendorName(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL(CHIP_ERROR_BUFFER_TOO_SMALL, provider.GetProductName(buf, sizeof(buf)));
}

static void test_hardware_version_string(void)
{
    reset_harness();
    CTLEVDeviceInstanceInfoProvider provider;
    char buf[32] = {};

    TEST_ASSERT_EQUAL(CHIP_NO_ERROR, provider.GetHardwareVersionString(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL_STRING("CTLEV02G01R01", buf);
    TEST_ASSERT_EQUAL(CHIP_ERROR_BUFFER_TOO_SMALL, provider.GetHardwareVersionString(buf, 8));
}

static void test_unsupported_optional_attributes(void)
{
    reset_harness();
    CTLEVDeviceInstanceInfoProvider provider;
    char buf[16] = {};
    uint16_t hwVersion = 0;

    TEST_ASSERT_EQUAL(CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE, provider.GetPartNumber(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL(CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE, provider.GetProductURL(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL(CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE, provider.GetProductLabel(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL(CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE, provider.GetHardwareVersion(hwVersion));
}

static void test_part_number_fr_triggers_factory_reset(void)
{
    reset_harness();
    ConfigurationMgr().SetCountryCodeForTest("FR");

    CTLEVDeviceInstanceInfoProvider provider;
    char buf[16] = {};
    TEST_ASSERT_EQUAL(CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE, provider.GetPartNumber(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL(1, esp_matter::FactoryResetCallCountForTest());
}

static void test_part_number_non_fr_no_factory_reset(void)
{
    reset_harness();
    ConfigurationMgr().SetCountryCodeForTest("US");

    CTLEVDeviceInstanceInfoProvider provider;
    char buf[16] = {};
    TEST_ASSERT_EQUAL(CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE, provider.GetPartNumber(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL(0, esp_matter::FactoryResetCallCountForTest());
}

static void test_serial_number_from_mac(void)
{
    reset_harness();
    CTLEVDeviceInstanceInfoProvider provider;
    char buf[32] = {};
    TEST_ASSERT_EQUAL(CHIP_NO_ERROR, provider.GetSerialNumber(buf, sizeof(buf)));
    TEST_ASSERT_EQUAL_STRING("CTLMEV-112233", buf);
}

static void test_serial_number_mac_read_failure(void)
{
    reset_harness();
    unit_test_esp_read_mac_fail() = true;

    CTLEVDeviceInstanceInfoProvider provider;
    char buf[32] = {};
    TEST_ASSERT_EQUAL(CHIP_ERROR_INTERNAL, provider.GetSerialNumber(buf, sizeof(buf)));
}

static void test_serial_number_buffer_too_small(void)
{
    reset_harness();
    CTLEVDeviceInstanceInfoProvider provider;
    char buf[8] = {};
    TEST_ASSERT_EQUAL(CHIP_ERROR_BUFFER_TOO_SMALL, provider.GetSerialNumber(buf, sizeof(buf)));
}

static void test_manufacturing_date(void)
{
    reset_harness();
    CTLEVDeviceInstanceInfoProvider provider;
    uint16_t year  = 0;
    uint8_t month  = 0;
    uint8_t day    = 0;
    TEST_ASSERT_EQUAL(CHIP_NO_ERROR, provider.GetManufacturingDate(year, month, day));
    TEST_ASSERT_EQUAL_UINT16(2026, year);
    TEST_ASSERT_EQUAL_UINT8(1, month);
    TEST_ASSERT_EQUAL_UINT8(23, day);
}

static void test_rotating_device_id_wrong_key_type(void)
{
    reset_harness();
    CTLEVDeviceInstanceInfoProvider provider;
    uint8_t data[8] = {};
    MutableByteSpan span;
    span.data   = data;
    span.length = sizeof(data);
    TEST_ASSERT_EQUAL(CHIP_ERROR_WRONG_KEY_TYPE, provider.GetRotatingDeviceIdUniqueId(span));
}

void run_test_esp32_device_instance_info_provider_tests(void)
{
    RUN_TEST(test_vendor_and_product_names);
    RUN_TEST(test_vendor_and_product_ids);
    RUN_TEST(test_vendor_and_product_name_buffer_too_small);
    RUN_TEST(test_hardware_version_string);
    RUN_TEST(test_unsupported_optional_attributes);
    RUN_TEST(test_part_number_fr_triggers_factory_reset);
    RUN_TEST(test_part_number_non_fr_no_factory_reset);
    RUN_TEST(test_serial_number_from_mac);
    RUN_TEST(test_serial_number_mac_read_failure);
    RUN_TEST(test_serial_number_buffer_too_small);
    RUN_TEST(test_manufacturing_date);
    RUN_TEST(test_rotating_device_id_wrong_key_type);
}

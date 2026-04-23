#include "helpers.h"

#include <esp_matter_core.h>

#include <platform/internal/GenericDeviceInstanceInfoProvider.h>

#include "ESP32DeviceInstanceInfoProvider.h"

namespace chip {
namespace DeviceLayer {


#undef CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME
#undef CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME
#undef CHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION_STRING
#undef CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING
#undef CHIP_CONFIG_SOFTWARE_VERSION_NUMBER
#undef CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER
#undef CHIP_DEVICE_CONFIG_DEVICE_NAME

#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME                       "Computime Limited"
#define CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME                      "Matter EVSE"
#define CHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION_STRING   "CTLEV02G01R01"
#define CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING           "0.1.0.8C"
#define CHIP_CONFIG_SOFTWARE_VERSION_NUMBER                         2
#define CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER                       "20260123CTLMEV00001"
#define CHIP_DEVICE_CONFIG_DEVICE_NAME                              "Test EVSE"

CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetVendorName(char * buf, size_t bufSize)
{
    ReturnErrorCodeIf(bufSize < sizeof(CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME), CHIP_ERROR_BUFFER_TOO_SMALL);
    strcpy(buf, CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME);
    return CHIP_NO_ERROR;
}

CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetVendorId(uint16_t & vendorId)
{
    vendorId = static_cast<uint16_t>(CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID);
    return CHIP_NO_ERROR;
}

CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetProductName(char * buf, size_t bufSize)
{
    ReturnErrorCodeIf(bufSize < sizeof(CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME), CHIP_ERROR_BUFFER_TOO_SMALL);
    strcpy(buf, CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME);

    return CHIP_NO_ERROR;
}

CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetProductId(uint16_t & productId)
{
    productId = static_cast<uint16_t>(CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_ID);
    return CHIP_NO_ERROR;
}

CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetPartNumber(char * buf, size_t bufSize)
{
#if 1
    constexpr size_t kMaxLen   = DeviceLayer::ConfigurationManager::kMaxLocationLength;
    char location[kMaxLen + 1] = { 0 };
    size_t codeLen             = 0;

    CHIP_ERROR err = ConfigurationMgr().GetCountryCode(location, sizeof(location), codeLen);

    if(err == CHIP_NO_ERROR)
    {
        if(strcmp(location, "FR") == 0)
        {
            //this is a special hardcoded string FR = Factory Reset
            //if the location is set to FR and read this part number attribute
            //we will do a factory reset as a quick work arround for demo purposes
            PRINTF_DEBUG("Factory Reset triggered via PartNumber attribute read");
            esp_matter::factory_reset();
        }
    }
#endif
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetProductURL(char * buf, size_t bufSize)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetProductLabel(char * buf, size_t bufSize)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

#include "esp_mac.h"
#include <esp_mac.h>
#include <lib/support/CHIPMemString.h>
#include <platform/CHIPDeviceLayer.h>
CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetSerialNumber(char * buf, size_t bufSize)
{
    uint8_t mac[6];
    // Use the Base MAC (or WIFI_STA) as the unique identifier
    esp_err_t err = esp_read_mac(mac, ESP_MAC_WIFI_STA);
    if (err != ESP_OK) {
        ChipLogError(DeviceLayer, "Failed to read MAC address: %d", err);
        return CHIP_ERROR_INTERNAL;
    }

    // Format the string: CTLMEV-XXXXXX (6 hex chars from last 3 bytes of MAC)
    // snprintf returns the number of characters that would have been written
    int written = snprintf(buf, bufSize, "CTLMEV-%02X%02X%02X", mac[3], mac[4], mac[5]);

    // Check if the buffer was large enough
    if (written < 0 || static_cast<size_t>(written) >= bufSize) {
        ChipLogError(DeviceLayer, "Serial number buffer too small (needed %d, got %u)", written + 1, (unsigned int)bufSize);
        return CHIP_ERROR_BUFFER_TOO_SMALL;
    }

    PRINTF_DEBUG("Device Serial Number set to: %s", buf);
    
    return CHIP_NO_ERROR;
}

CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetManufacturingDate(uint16_t & year, uint8_t & month, uint8_t & day)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    enum
    {
        kDateStringLength = 10 // YYYY-MM-DD
    };
    char dateStr[kDateStringLength + 1] = "2026-01-23";
    char * parseEnd;

    // Cast does not lose information, because we then check that we only parsed
    // 4 digits, so our number can't be bigger than 9999.
    year = static_cast<uint16_t>(strtoul(dateStr, &parseEnd, 10));

    // Cast does not lose information, because we then check that we only parsed
    // 2 digits, so our number can't be bigger than 99.
    month = static_cast<uint8_t>(strtoul(dateStr + 5, &parseEnd, 10));

    // Cast does not lose information, because we then check that we only parsed
    // 2 digits, so our number can't be bigger than 99.
    day = static_cast<uint8_t>(strtoul(dateStr + 8, &parseEnd, 10));

    return CHIP_NO_ERROR;
}

CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetHardwareVersion(uint16_t & hardwareVersion)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetHardwareVersionString(char * buf, size_t bufSize)
{
    ReturnErrorCodeIf(bufSize < sizeof(CHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION_STRING), CHIP_ERROR_BUFFER_TOO_SMALL);
    strcpy(buf, CHIP_DEVICE_CONFIG_DEFAULT_DEVICE_HARDWARE_VERSION_STRING);
    return CHIP_NO_ERROR;
}

CHIP_ERROR CTLEVDeviceInstanceInfoProvider::GetRotatingDeviceIdUniqueId(MutableByteSpan & uniqueIdSpan)
{
    return CHIP_ERROR_WRONG_KEY_TYPE;
}



} // namespace DeviceLayer
} // namespace chip
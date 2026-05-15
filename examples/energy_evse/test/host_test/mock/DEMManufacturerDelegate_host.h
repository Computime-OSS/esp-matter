#pragma once

#include "chip_support.h"

namespace chip {
namespace app {
namespace Clusters {
namespace DeviceEnergyManagement {

class DEMManufacturerDelegate {
public:
    DEMManufacturerDelegate() = default;
    virtual ~DEMManufacturerDelegate() = default;

    virtual int64_t GetApproxEnergyDuringSession() = 0;

    virtual CHIP_ERROR HandleDeviceEnergyManagementPowerAdjustRequest(int64_t, uint32_t, int) { return CHIP_NO_ERROR; }
    virtual CHIP_ERROR HandleDeviceEnergyManagementCancelRequest() { return CHIP_NO_ERROR; }
};

} // namespace DeviceEnergyManagement
} // namespace Clusters
} // namespace app
} // namespace chip

#pragma once

#include "chip_support.h"

namespace chip {
namespace app {
namespace Clusters {
namespace ElectricalPowerMeasurement {

enum class PowerModeEnum : uint8_t { kAc = 0 };

class ElectricalPowerMeasurementDelegate {
public:
    virtual ~ElectricalPowerMeasurementDelegate() = default;
    PowerModeEnum GetPowerMode() { return mPowerMode; }
    uint8_t GetNumberOfMeasurementTypes() { return 1; }

private:
    PowerModeEnum mPowerMode = PowerModeEnum::kAc;
};

} // namespace ElectricalPowerMeasurement
} // namespace Clusters
} // namespace app
} // namespace chip

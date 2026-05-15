#pragma once

#include <cstdint>

bool EnergyEvse_TargetSkippedAsPast(uint16_t targetMinutesPastMidnight, uint16_t minutesPastMidnightNow_m,
                                    bool allowTargetsInPast);

namespace chip {
namespace app {
namespace Clusters {
namespace DeviceEnergyManagement {

enum class OptOutStateEnum : uint8_t {
    kNoOptOut = 0,
    kLocalOptOut = 1,
    kGridOptOut = 2,
    kOptOut = 3,
};

OptOutStateEnum MergedOptOutState(OptOutStateEnum oldValue, OptOutStateEnum newValue);

} // namespace DeviceEnergyManagement
} // namespace Clusters
} // namespace app
} // namespace chip

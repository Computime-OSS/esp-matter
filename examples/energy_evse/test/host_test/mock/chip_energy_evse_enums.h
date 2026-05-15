#pragma once

#include "chip_support.h"

namespace chip {
namespace app {
namespace Clusters {
namespace EnergyEvse {

enum class TargetDayOfWeekBitmap : uint8_t {
    kSunday = 0x01,
    kMonday = 0x02,
    kTuesday = 0x04,
    kWednesday = 0x08,
    kThursday = 0x10,
    kFriday = 0x20,
    kSaturday = 0x40,
};

} // namespace EnergyEvse
} // namespace Clusters
} // namespace app
} // namespace chip

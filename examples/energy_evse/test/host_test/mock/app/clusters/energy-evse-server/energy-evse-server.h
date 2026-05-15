#pragma once

#include <cstdint>

namespace chip {
namespace app {
namespace Clusters {
namespace EnergyEvse {

constexpr uint8_t kEvseTargetsMaxNumberOfDays  = 7;
constexpr uint8_t kEvseTargetsMaxTargetsPerDay = 10;

} // namespace EnergyEvse
} // namespace Clusters
} // namespace app
} // namespace chip

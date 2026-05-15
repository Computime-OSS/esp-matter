#include "unit_test_exports.h"

namespace chip {
namespace app {
namespace Clusters {
namespace DeviceEnergyManagement {

OptOutStateEnum MergedOptOutState(OptOutStateEnum oldValue, OptOutStateEnum newValue)
{
    if ((oldValue == OptOutStateEnum::kGridOptOut && newValue == OptOutStateEnum::kLocalOptOut) ||
        (oldValue == OptOutStateEnum::kLocalOptOut && newValue == OptOutStateEnum::kGridOptOut))
    {
        return OptOutStateEnum::kOptOut;
    }
    return newValue;
}

} // namespace DeviceEnergyManagement
} // namespace Clusters
} // namespace app
} // namespace chip

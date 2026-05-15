#pragma once

#include "esp_matter_dem.h"

namespace esp_matter {
namespace cluster {
namespace energy_evse {
namespace attribute {
inline void create_user_maximum_charge_current(cluster_t *, int64_t) {}
} // namespace attribute

namespace feature {
namespace charging_preferences {
inline void add(cluster_t *) {}
} // namespace charging_preferences

namespace rfid {
inline void add(cluster_t *) {}
} // namespace rfid

namespace soc_reporting {
inline void add(cluster_t *) {}
} // namespace soc_reporting

namespace v2x {
inline void add(cluster_t *) {}
} // namespace v2x

} // namespace feature
} // namespace energy_evse
} // namespace cluster
} // namespace esp_matter

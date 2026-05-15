#pragma once

#include "esp_matter_dem.h"

namespace esp_matter {
namespace cluster {
namespace electrical_power_measurement {
namespace attribute {
inline void create_frequency(cluster_t *, int64_t) {}
inline void create_active_current(cluster_t *, int64_t) {}
inline void create_voltage(cluster_t *, int64_t) {}
} // namespace attribute
} // namespace electrical_power_measurement
} // namespace cluster
} // namespace esp_matter

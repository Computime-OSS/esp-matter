#pragma once

#include "chip_support.h"

#include <cstdint>

namespace esp_matter {

using cluster_t = void *;
using endpoint_t = uint16_t;

namespace cluster {
inline cluster_t * get(endpoint_t, uint32_t) { return nullptr; }
} // namespace cluster

namespace cluster::device_energy_management::feature::power_adjustment {
inline void add(cluster_t *) {}
} // namespace cluster::device_energy_management::feature::power_adjustment

} // namespace esp_matter

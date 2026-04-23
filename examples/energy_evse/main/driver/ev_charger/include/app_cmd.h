#pragma once
#include <esp_err.h>

namespace esp_matter {
namespace Charger {

namespace core {
esp_err_t app_driver_init();
} // namespace core

namespace console {
void init();
} // namespace console

} // namespace Charger
} // namespace esp_matter

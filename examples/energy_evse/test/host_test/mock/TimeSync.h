#pragma once

namespace chip {
namespace Esp32TimeSync {
void Init(const char * aSntpServerName, uint16_t aSyncSntpIntervalDay);
} // namespace Esp32TimeSync
} // namespace chip

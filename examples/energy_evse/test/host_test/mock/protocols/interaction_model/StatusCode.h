#pragma once

#include <cstdint>

namespace chip {
namespace Protocols {
namespace InteractionModel {

enum class Status : uint8_t {
    Success = 0,
    Failure = 1,
};

} // namespace InteractionModel
} // namespace Protocols
} // namespace chip

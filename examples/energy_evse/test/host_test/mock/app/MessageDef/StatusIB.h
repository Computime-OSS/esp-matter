#pragma once

#include "chip_support.h"
#include "protocols/interaction_model/StatusCode.h"

namespace chip {
namespace app {

struct StatusIB
{
    Protocols::InteractionModel::Status mStatus = Protocols::InteractionModel::Status::Failure;

    StatusIB() = default;

    explicit StatusIB(Protocols::InteractionModel::Status imStatus) : mStatus(imStatus) {}

    explicit StatusIB(CHIP_ERROR error)
    {
        if (error == CHIP_NO_ERROR) {
            mStatus = Protocols::InteractionModel::Status::Success;
        } else {
            mStatus = Protocols::InteractionModel::Status::Failure;
        }
    }
};

} // namespace app
} // namespace chip

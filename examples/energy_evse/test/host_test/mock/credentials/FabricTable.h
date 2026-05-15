#pragma once

#include "chip_fabric_table.h"

#include <cstdint>
#include <vector>

namespace chip {

using FabricIndex = uint8_t;

class FabricTable {
public:
    class Delegate {
    public:
        virtual ~Delegate() = default;
        virtual void OnFabricRemoved(const FabricTable & fabricTable, FabricIndex fabricIndex) = 0;
    };

    CHIP_ERROR AddFabricDelegate(Delegate * delegate)
    {
        if (delegate != nullptr) {
            mDelegates.push_back(delegate);
        }
        return CHIP_NO_ERROR;
    }

    void RemoveFabricDelegate(Delegate * delegate)
    {
        for (auto it = mDelegates.begin(); it != mDelegates.end(); ++it) {
            if (*it == delegate) {
                mDelegates.erase(it);
                break;
            }
        }
    }

private:
    std::vector<Delegate *> mDelegates;
};

} // namespace chip

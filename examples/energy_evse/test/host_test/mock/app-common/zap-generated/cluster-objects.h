#pragma once

#include "chip_support.h"

#include <cstdint>
#include <initializer_list>
#include <vector>

namespace chip {
namespace app {
namespace Clusters {
namespace EnergyEvse {
namespace Structs {

struct ChargingTargetStruct {
    struct Type {
        uint16_t targetTimeMinutesPastMidnight = 0;
        uint8_t dayOfWeek = 0;
    };

    struct DecodableType {
        uint16_t targetTimeMinutesPastMidnight = 0;
        uint8_t dayOfWeek = 0;

        operator Type() const
        {
            Type out;
            out.targetTimeMinutesPastMidnight = targetTimeMinutesPastMidnight;
            out.dayOfWeek                     = dayOfWeek;
            return out;
        }
    };
};

} // namespace Structs
} // namespace EnergyEvse
} // namespace Clusters

namespace DataModel {

template <typename T>
struct List {
    const T * data  = nullptr;
    size_t count  = 0;

    List() = default;
    List(const T * d, size_t c) : data(d), count(c) {}

    size_t size() const { return count; }

    const T * begin() const { return data; }
    const T * end() const { return data + count; }
};

template <typename T>
class DecodableList {
public:
    class Iterator {
    public:
        explicit Iterator(const DecodableList * parent) : mParent(parent) {}

        bool Next()
        {
            if (mParent == nullptr || mIndex >= mParent->mItems.size()) {
                return false;
            }
            ++mIndex;
            return mIndex <= mParent->mItems.size();
        }

        T & GetValue()
        {
            auto * parent = const_cast<DecodableList *>(mParent);
            return parent->mItems[mIndex > 0 ? mIndex - 1 : 0];
        }

    private:
        const DecodableList * mParent;
        size_t mIndex = 0;
    };

    DecodableList() = default;

    explicit DecodableList(std::initializer_list<T> items) : mItems(items) {}

    CHIP_ERROR ComputeSize(size_t * out) const
    {
        if (out == nullptr) {
            return CHIP_ERROR_INVALID_ARGUMENT;
        }
        if (mComputeSizeError != CHIP_NO_ERROR) {
            return mComputeSizeError;
        }
        *out = mItems.size();
        return CHIP_NO_ERROR;
    }

    Iterator begin() const { return Iterator(this); }

    void SetItemsForTest(std::initializer_list<T> items) { mItems.assign(items); }

    void SetComputeSizeErrorForTest(CHIP_ERROR err) { mComputeSizeError = err; }

    CHIP_ERROR GetComputeSizeErrorForTest() const { return mComputeSizeError; }

private:
    std::vector<T> mItems;
    CHIP_ERROR mComputeSizeError = CHIP_NO_ERROR;
};

} // namespace DataModel
} // namespace app
} // namespace chip

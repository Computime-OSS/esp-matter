#pragma once

#include "chip_support.h"

#include <cstring>

namespace chip {
namespace app {
namespace Clusters {
namespace EnergyEvse {

constexpr uint8_t kEvseTargetsMaxNumberOfDays = 7;
constexpr uint8_t kEvseTargetsMaxTargetsPerDay = 10;

namespace Structs {
struct ChargingTargetStruct {
    struct Type {
        uint16_t targetTimeMinutesPastMidnight = 0;
        uint8_t dayOfWeek = 0;
    };
};
} // namespace Structs

class ChargingTargetsMemMgr {
public:
    ChargingTargetsMemMgr();
    ~ChargingTargetsMemMgr();

    ChargingTargetsMemMgr(const ChargingTargetsMemMgr &) = delete;
    ChargingTargetsMemMgr & operator=(const ChargingTargetsMemMgr &) = delete;

    ChargingTargetsMemMgr(ChargingTargetsMemMgr && other) noexcept;
    ChargingTargetsMemMgr & operator=(ChargingTargetsMemMgr && other) noexcept;

    void PrepareDaySchedule(uint16_t chargingTargetSchedulesIdx);
    void AddChargingTarget(const Structs::ChargingTargetStruct::Type & chargingTarget);
    CHIP_ERROR AllocAndCopy();

    Structs::ChargingTargetStruct::Type * GetChargingTargets() const;
    uint16_t GetNumDailyChargingTargets() const;

private:
    Structs::ChargingTargetStruct::Type * mpListOfDays[kEvseTargetsMaxNumberOfDays];
    Structs::ChargingTargetStruct::Type mDailyChargingTargets[kEvseTargetsMaxTargetsPerDay];
    uint16_t mChargingTargetSchedulesIdx = 0;
    uint16_t mNumDailyChargingTargets = 0;
};

} // namespace EnergyEvse
} // namespace Clusters
} // namespace app
} // namespace chip

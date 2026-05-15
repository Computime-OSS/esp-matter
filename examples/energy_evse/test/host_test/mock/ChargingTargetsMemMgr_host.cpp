#include "ChargingTargetsMemMgr_host.h"

#include <new>

namespace chip {
namespace app {
namespace Clusters {
namespace EnergyEvse {

ChargingTargetsMemMgr::ChargingTargetsMemMgr()
{
    std::memset(mpListOfDays, 0, sizeof(mpListOfDays));
}

ChargingTargetsMemMgr::~ChargingTargetsMemMgr()
{
    for (uint16_t idx = 0; idx < kEvseTargetsMaxNumberOfDays; idx++) {
        delete[] mpListOfDays[idx];
        mpListOfDays[idx] = nullptr;
    }
}

ChargingTargetsMemMgr::ChargingTargetsMemMgr(ChargingTargetsMemMgr && other) noexcept
    : mChargingTargetSchedulesIdx(other.mChargingTargetSchedulesIdx), mNumDailyChargingTargets(other.mNumDailyChargingTargets)
{
    std::memcpy(mpListOfDays, other.mpListOfDays, sizeof(mpListOfDays));
    std::memcpy(mDailyChargingTargets, other.mDailyChargingTargets, sizeof(mDailyChargingTargets));
    std::memset(other.mpListOfDays, 0, sizeof(other.mpListOfDays));
    other.mChargingTargetSchedulesIdx = 0;
    other.mNumDailyChargingTargets = 0;
}

ChargingTargetsMemMgr & ChargingTargetsMemMgr::operator=(ChargingTargetsMemMgr && other) noexcept
{
    if (this != &other) {
        for (uint16_t idx = 0; idx < kEvseTargetsMaxNumberOfDays; idx++) {
            delete[] mpListOfDays[idx];
        }
        std::memset(mpListOfDays, 0, sizeof(mpListOfDays));
        std::memcpy(mpListOfDays, other.mpListOfDays, sizeof(mpListOfDays));
        std::memcpy(mDailyChargingTargets, other.mDailyChargingTargets, sizeof(mDailyChargingTargets));
        mChargingTargetSchedulesIdx = other.mChargingTargetSchedulesIdx;
        mNumDailyChargingTargets = other.mNumDailyChargingTargets;
        std::memset(other.mpListOfDays, 0, sizeof(other.mpListOfDays));
        other.mChargingTargetSchedulesIdx = 0;
        other.mNumDailyChargingTargets = 0;
    }
    return *this;
}

void ChargingTargetsMemMgr::PrepareDaySchedule(uint16_t chargingTargetSchedulesIdx)
{
    mNumDailyChargingTargets = 0;
    if (chargingTargetSchedulesIdx >= kEvseTargetsMaxNumberOfDays) {
        return;
    }
    mChargingTargetSchedulesIdx = chargingTargetSchedulesIdx;
    delete[] mpListOfDays[mChargingTargetSchedulesIdx];
    mpListOfDays[mChargingTargetSchedulesIdx] = nullptr;
}

void ChargingTargetsMemMgr::AddChargingTarget(const Structs::ChargingTargetStruct::Type & chargingTarget)
{
    if (mNumDailyChargingTargets < kEvseTargetsMaxTargetsPerDay) {
        mDailyChargingTargets[mNumDailyChargingTargets++] = chargingTarget;
    }
}

CHIP_ERROR ChargingTargetsMemMgr::AllocAndCopy()
{
    if (mpListOfDays[mChargingTargetSchedulesIdx] != nullptr) {
        return CHIP_ERROR_INCORRECT_STATE;
    }
    if (mNumDailyChargingTargets == 0) {
        return CHIP_NO_ERROR;
    }
    mpListOfDays[mChargingTargetSchedulesIdx] =
        new Structs::ChargingTargetStruct::Type[mNumDailyChargingTargets];
    if (mpListOfDays[mChargingTargetSchedulesIdx] == nullptr) {
        return CHIP_ERROR_NO_MEMORY;
    }
    for (uint16_t idx = 0; idx < mNumDailyChargingTargets; idx++) {
        mpListOfDays[mChargingTargetSchedulesIdx][idx] = mDailyChargingTargets[idx];
    }
    return CHIP_NO_ERROR;
}

Structs::ChargingTargetStruct::Type * ChargingTargetsMemMgr::GetChargingTargets() const
{
    return mpListOfDays[mChargingTargetSchedulesIdx];
}

uint16_t ChargingTargetsMemMgr::GetNumDailyChargingTargets() const { return mNumDailyChargingTargets; }

} // namespace EnergyEvse
} // namespace Clusters
} // namespace app
} // namespace chip

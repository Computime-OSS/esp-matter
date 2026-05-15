#include "chip_support.h"

namespace {

bool TargetSkippedAsPast(uint16_t targetMinutesPastMidnight, uint16_t minutesPastMidnightNow_m, bool allowTargetsInPast)
{
    return !allowTargetsInPast && targetMinutesPastMidnight < minutesPastMidnightNow_m;
}

} // namespace

bool EnergyEvse_TargetSkippedAsPast(uint16_t targetMinutesPastMidnight, uint16_t minutesPastMidnightNow_m,
                                    bool allowTargetsInPast)
{
    return TargetSkippedAsPast(targetMinutesPastMidnight, minutesPastMidnightNow_m, allowTargetsInPast);
}

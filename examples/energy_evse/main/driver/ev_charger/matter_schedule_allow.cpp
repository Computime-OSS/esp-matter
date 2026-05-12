#include "matter_schedule_allow.h"

bool MatterScheduleIsChargingAllowedAtUnix(
    std::time_t now_unix,
    std::time_t schedule_start_unix,
    std::time_t schedule_target_unix)
{
    if (now_unix < schedule_start_unix)
    {
        return false;
    }
    if (now_unix >= schedule_target_unix)
    {
        return false;
    }
    return true;
}

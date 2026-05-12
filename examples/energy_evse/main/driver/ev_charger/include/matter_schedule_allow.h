#pragma once

#include <ctime>

/**
 * Pure schedule gate extracted from MatterManager::IsChargingAllowedByTargets:
 * charging is allowed only when now_unix is in [schedule_start_unix, schedule_target_unix).
 *
 * @param now_unix Current time (Unix epoch seconds).
 * @param schedule_start_unix Start of allowed window (Unix epoch).
 * @param schedule_target_unix End of window (exclusive); same semantics as Matter code (now >= target disallows).
 */
bool MatterScheduleIsChargingAllowedAtUnix(
    std::time_t now_unix,
    std::time_t schedule_start_unix,
    std::time_t schedule_target_unix);

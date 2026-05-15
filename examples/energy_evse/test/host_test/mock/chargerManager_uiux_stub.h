#pragma once

#include <cstdint>

#include "helpers.h"

namespace CT {
namespace Charger {

/// Minimal `ChargerManager` stand-in for host tests (`UNIT_TEST`) compiling
/// `charger_uiux_handler.cpp` without Matter, FreeRTOS, or hardware headers.
class ChargerManager {
public:
    static ChargerManager &Controller() {
        static ChargerManager instance;
        return instance;
    }

    void resetForTest() { *this = ChargerManager(); }

    struct {
        bool isAuthorized_ = false;
        bool isActive_ = false;
        int64_t timeElapsed = 0;
        struct {
            uint32_t energyDelivered_mWh = 0;
            uint32_t energyOffsetmWh = 0;
            uint32_t current = 0;
            uint32_t voltage = 0;
        } chargingData;
    } session;

    struct {
        uint32_t itemIdx = 0;
    } uiux;

    ChargerStatus_t status = ChargerStatus_t::INIT;

    bool on_time_equal_1 = false;
    bool on_time_equal_5 = false;

    bool onTimeEqual_Second(uint32_t sec) {
        if (sec == 1U) {
            return on_time_equal_1;
        }
        if (sec == 5U) {
            return on_time_equal_5;
        }
        return false;
    }
};

} // namespace Charger
} // namespace CT

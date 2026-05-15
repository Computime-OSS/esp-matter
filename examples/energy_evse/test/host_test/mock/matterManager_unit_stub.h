#pragma once

#include <cstdint>

namespace CT {
namespace Charger {

class MatterManager {
public:
    static MatterManager &GetInstance() {
        static MatterManager inst;
        return inst;
    }

    MatterManager(MatterManager const &) = delete;
    void operator=(MatterManager const &) = delete;

    void resetForTest() {
        charging_enabled_ = true;
        targets_allow_ = true;
        isConnected = false;
    }

    void StartSession(int64_t) {}
    void StopSession(int64_t) {}
    void UpdateState() {}
    void UpdateFaultState(uint8_t) {}
    void UpdateSession(int64_t) {}

    int SendReadings(int64_t, int64_t, int64_t) {
        return 0;
    }

    int SendCumulativeEnergyReading(int64_t, int64_t) {
        return 0;
    }

    bool GetChargingEnabled() {
        return charging_enabled_;
    }

    void SetChargingEnabledForTest(bool v) {
        charging_enabled_ = v;
    }

    bool IsChargingAllowedByTargets() {
        return targets_allow_;
    }

    void SetTargetsAllowForTest(bool v) {
        targets_allow_ = v;
    }

    bool isConnected = false;

private:
    MatterManager() = default;

    bool charging_enabled_ = true;
    bool targets_allow_ = true;
};

} // namespace Charger
} // namespace CT

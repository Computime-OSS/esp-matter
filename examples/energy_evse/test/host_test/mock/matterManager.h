#pragma once

#include "app/clusters/device-energy-management-server/device-energy-management-server.h"

namespace CT {
namespace Charger {

inline constexpr bool kSupportDischargingV2x = false;

class MatterManager {
public:
    static MatterManager & GetInstance()
    {
        static MatterManager inst;
        return inst;
    }

    static void ReportAttributeChangeToMatter(chip::EndpointId endpoint, chip::ClusterId clusterId, chip::AttributeId attributeId);

    static int & ReportAttributeCallCount();

    bool isConnected = false;

    void Init() {}

    void resetForTest()
    {
        isConnected        = false;
        charging_enabled_  = true;
        targets_allow_     = true;
    }

    void StartSession(int64_t) {}
    void StopSession(int64_t) {}
    void UpdateState() {}
    void UpdateFaultState(uint8_t) {}
    void UpdateSession(int64_t) {}

    int SendReadings(int64_t, int64_t, int64_t) { return 0; }
    int SendCumulativeEnergyReading(int64_t, int64_t) { return 0; }

    bool GetChargingEnabled() const { return charging_enabled_; }
    void SetChargingEnabledForTest(bool v) { charging_enabled_ = v; }

    bool IsChargingAllowedByTargets() const { return targets_allow_; }
    void SetTargetsAllowForTest(bool v) { targets_allow_ = v; }

private:
    MatterManager() = default;

    bool charging_enabled_ = true;
    bool targets_allow_    = true;
};

} // namespace Charger
} // namespace CT

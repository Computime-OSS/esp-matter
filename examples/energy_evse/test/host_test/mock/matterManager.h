#pragma once

#include "app/clusters/device-energy-management-server/device-energy-management-server.h"

namespace CT {
namespace Charger {

class MatterManager {
public:
    static MatterManager & GetInstance()
    {
        static MatterManager inst;
        return inst;
    }

    static void ReportAttributeChangeToMatter(chip::EndpointId endpoint, chip::ClusterId clusterId, chip::AttributeId attributeId);

    static int & ReportAttributeCallCount();

    void Init() {}

private:
    MatterManager() = default;
};

} // namespace Charger
} // namespace CT

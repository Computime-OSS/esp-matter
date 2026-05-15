#include "matterManager.h"

namespace CT {
namespace Charger {

namespace {
int g_report_count = 0;
} // namespace

void MatterManager::ReportAttributeChangeToMatter(chip::EndpointId endpoint, chip::ClusterId clusterId,
                                                  chip::AttributeId attributeId)
{
    (void) endpoint;
    (void) clusterId;
    (void) attributeId;
    ++g_report_count;
}

int & MatterManager::ReportAttributeCallCount() { return g_report_count; }

} // namespace Charger
} // namespace CT

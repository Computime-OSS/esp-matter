#pragma once
#include <stdlib.h>
#include <string.h>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_matter.h"

#include <device.h>
#include <protocols/Protocols.h>

#include "helpers.h"

#include "ESP32DeviceInstanceInfoProvider.h"

#include "energy-evse-modes.h"
#include "DeviceEnergyManagementDelegateImpl.h"
#include "EnergyEvseDelegateImpl.h"
#include "PowerTopologyDelegate.h"
#include "ElectricalPowerMeasurementDelegate.h"

#define SUPPORT_DISCHARGING_V2X 0

using chip::Protocols::InteractionModel::Status;

namespace chip {
namespace app {
namespace Clusters {
namespace PowerSource {

// using namespace esp_matter::cluster::power_source;
class PowerSourceDelegate
{
private:
    /* data */
public:
    PowerSourceDelegate(){};

    void SetupDelegate(EndpointId id);
    void AddCustomAttributes();
    void AddCustomFeatures(Feature aFeature);
    void LateSetupAfterMatter();

    EndpointId mEndpointId = 0;
    BitMask<Feature> mFeature;

    using config_t = esp_matter::cluster::power_source::config_t;

    config_t config;
};
} // namespace PowerSource
} // namespace Clusters
} // namespace app
} // namespace chip

namespace CT {
namespace Charger {

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::EnergyEvse;
using namespace chip::app::Clusters::EnergyEvseMode;
using namespace chip::app::Clusters::DeviceEnergyManagement;
using namespace chip::app::Clusters::PowerSource;
using namespace chip::app::Clusters::PowerTopology;
using namespace chip::app::Clusters::ElectricalPowerMeasurement;

using namespace esp_matter;

class MatterManager
{
public:
    static MatterManager &GetInstance()
    {
        static MatterManager instance;
        return instance;
    }

    MatterManager(MatterManager const &) = delete;
    void operator=(MatterManager const &) = delete;

    void Init();
    void InitializeDeviceDelegates();
    void InitializeMatterDeviceNode();
    void StartMatterStack();

    static void Wrapper_MatterEventCb(const ChipDeviceEvent *event, intptr_t arg);
    void HandleMatterEventCb(const ChipDeviceEvent *event);

    void HandleMatterAttributeUpdate(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *val);

    bool isConnected;
    node_t *device_node;

    void UpdateState();
    void UpdateFaultState(uint8_t faultCode);

    void StartSession(int64_t currentEnergy);
    void StopSession(int64_t currentEnergy);
    void UpdateSession(int64_t currentEnergy);
    
    bool IsChargingAllowedByTargets(void);

    CHIP_ERROR InitializePowerMeasurementCluster();
    CHIP_ERROR InitializePowerSourceCluster();
    CHIP_ERROR SendReadings(int64_t aActivePower_mW, int64_t aVoltage_mV, int64_t aActiveCurrent_mA);
    CHIP_ERROR SendCumulativeEnergyReading(int64_t aCumulativeEnergyImported, int64_t aCumulativeEnergyExported);

    static void ReportAttributeChangeToMatter(EndpointId endpoint, ClusterId clusterId, AttributeId attributeId);

    // static void SendEventToMatter(EndpointId endpoint, uint32_t eventId, void* data);

    /* Delegates external APIs*/
    bool GetChargingEnabled();

    chip::DeviceLayer::CTLEVDeviceInstanceInfoProvider     DIIProvider;

    EnergyEvseDelegate                  EE_dg;
    EnergyEvseModeDelegate              EEM_dg;
    DeviceEnergyManagementDelegate      DEM_dg;

    PowerSourceDelegate                 PS_dg;
    PowerTopologyDelegate               PT_dg;
    ElectricalPowerMeasurementDelegate  EPM_dg;
private:
    MatterManager();
};

} // namespace Charger
} // namespace CT

/*
 *
 *    Copyright (c) 2024 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "esp_matter.h"

#include "helpers.h"

#include <PowerTopologyDelegate.h>

using namespace chip;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::PowerTopology;

using namespace esp_matter;

CHIP_ERROR PowerTopologyDelegate::GetAvailableEndpointAtIndex(size_t index, EndpointId & endpointId)
{
    PRINTF_DEBUG("GetAvailableEndpointAtIndex:%d", index);
    if (index >= ArraySize(mAvailableEps)) return CHIP_ERROR_PROVIDER_LIST_EXHAUSTED;
    
    if(index == 0){
        endpointId = mAvailableEps[index];
        PRINTF_DEBUG("GetAvailableEndpointAtIndex:%d = %d", index, endpointId);
        return CHIP_NO_ERROR;
    }

    return CHIP_ERROR_NOT_FOUND;
}

CHIP_ERROR PowerTopologyDelegate::GetActiveEndpointAtIndex(size_t index, EndpointId & endpointId)
{
    PRINTF_DEBUG("GetActiveEndpointAtIndex:%d", index);
    return CHIP_ERROR_PROVIDER_LIST_EXHAUSTED;
}

void PowerTopologyDelegate::SetupDelegate(EndpointId id)
{
    mEndpointId = id;

    AddCustomAttributes();

    // AddCustomFeatures(
    //     BitMask<PowerTopology::Feature, uint32_t>(
    //         PowerTopology::Feature::kSetTopology
    //     )
    // );
}

void PowerTopologyDelegate::SetAvailableEndpointIds(EndpointId id, size_t index)
{
    if (index >= ArraySize(mAvailableEps)) return;

    mAvailableEps[index] = id;

    PRINTF_DEBUG("Set Available Endpoints Idx:%d=%d", index, id);
}

void PowerTopologyDelegate::AddCustomAttributes()
{

}

void PowerTopologyDelegate::AddCustomFeatures(Feature aFeature)
{
    mFeature.Set(aFeature);

    //this is already set when create the power_topology cluster in esp_matter_endpoint.cpp
    if (mFeature.Has(Feature::kSetTopology))
    {
        using namespace esp_matter::cluster::power_topology;
        if (mFeature.Has(Feature::kDynamicPowerFlow))
        {
            cluster_t *cluster = cluster::get(mEndpointId, Clusters::PowerTopology::Id);
            feature::dynamic_power_flow::add(cluster);
        }
    }
}

void PowerTopologyDelegate::LateSetupAfterMatter()
{
    PRINTF_DEBUG();
    esp_matter_attr_val_t val = esp_matter_array((uint8_t *)mAvailableEps, ArraySize(mAvailableEps), ArraySize(mAvailableEps));

    esp_err_t err = esp_matter::attribute::report(mEndpointId, PowerTopology::Id, PowerTopology::Attributes::AvailableEndpoints::Id, &val);

    PRINTF_DEBUG("Power Source ActiveEndpoints: (%s)", err==ESP_OK?"OK":"Failed");
}

CHIP_ERROR PowerTopologyInstance::Init()
{
    return Instance::Init();
}

void PowerTopologyInstance::Shutdown()
{
    Instance::Shutdown();
}

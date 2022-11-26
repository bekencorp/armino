/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
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

/**
 *    @file
 *          Provides the implementation of the Device Layer ConfigurationManager object
 *          for the Beken.
 */
/* this file behaves like a config.h, comes first */
#include <platform/Beken/ConfigurationManagerImpl.h>
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <platform/Beken/BekenConfig.h>
#include <platform/ConfigurationManager.h>
#include <platform/DiagnosticDataProvider.h>
#include <platform/internal/GenericConfigurationManagerImpl.ipp>
#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>

#include "wlan_ui_pub.h"

namespace chip {
namespace DeviceLayer {

using namespace chip::DeviceLayer::Internal;
using namespace chip::DeviceLayer;

ConfigurationManagerImpl & ConfigurationManagerImpl::GetDefaultInstance()
{
    static ConfigurationManagerImpl sInstance;
    return sInstance;
}

CHIP_ERROR ConfigurationManagerImpl::Init()
{
    CHIP_ERROR err;
    uint32_t rebootCount;

    ChipLogProgress(DeviceLayer, "ConfigurationManagerImpl::Init");
    // Force initialization of NVS namespaces if they doesn't already exist.
    err = BekenConfig::EnsureNamespace(BekenConfig::kConfigNamespace_ChipFactory);
    SuccessOrExit(err);
    err = BekenConfig::EnsureNamespace(BekenConfig::kConfigNamespace_ChipConfig);
    SuccessOrExit(err);
    err = BekenConfig::EnsureNamespace(BekenConfig::kConfigNamespace_ChipCounters);
    SuccessOrExit(err);

    if (BekenConfig::ConfigValueExists(BekenConfig::kCounterKey_RebootCount))
    {
        err = GetRebootCount(rebootCount);
        SuccessOrExit(err);

        err = StoreRebootCount(rebootCount + 1);
        SuccessOrExit(err);
    }
    else
    {
        // The first boot after factory reset of the Node.
        err = StoreRebootCount(1);
        SuccessOrExit(err);
    }

    if (!BekenConfig::ConfigValueExists(BekenConfig::kCounterKey_TotalOperationalHours))
    {
        err = StoreTotalOperationalHours(0);
        SuccessOrExit(err);
    }

    if (!BekenConfig::ConfigValueExists(BekenConfig::kCounterKey_BootReason))
    {
        err = StoreBootReason(to_underlying(BootReasonType::kUnspecified));
        SuccessOrExit(err);
    }

    if (!BekenConfig::ConfigValueExists(BekenConfig::kConfigKey_SoftwareVersion))
    {
        WriteConfigValue(BekenConfig::kConfigKey_SoftwareVersion, (uint32_t)0);
        SuccessOrExit(err);
    }

    if (!BekenConfig::ConfigValueExists(BekenConfig::kConfigKey_SoftwareVersionString))
    {
        WriteConfigValueStr(BekenConfig::kConfigKey_SoftwareVersionString, "0.0");
        SuccessOrExit(err);
    }


    // Initialize the generic implementation base class.
    err = Internal::GenericConfigurationManagerImpl<BekenConfig>::Init();
    SuccessOrExit(err);

    err = CHIP_NO_ERROR;
exit:
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::GetRebootCount(uint32_t & rebootCount)
{
    return ReadConfigValue(BekenConfig::kCounterKey_RebootCount, rebootCount);
}

CHIP_ERROR ConfigurationManagerImpl::StoreRebootCount(uint32_t rebootCount)
{
    return WriteConfigValue(BekenConfig::kCounterKey_RebootCount, rebootCount);
}

CHIP_ERROR ConfigurationManagerImpl::GetTotalOperationalHours(uint32_t & totalOperationalHours)
{
    return ReadConfigValue(BekenConfig::kCounterKey_TotalOperationalHours, totalOperationalHours);
}

CHIP_ERROR ConfigurationManagerImpl::StoreTotalOperationalHours(uint32_t totalOperationalHours)
{
    return WriteConfigValue(BekenConfig::kCounterKey_TotalOperationalHours, totalOperationalHours);
}

CHIP_ERROR ConfigurationManagerImpl::GetSoftwareVersionString(char * buf, size_t bufSize)
{
    char temp[kMaxSoftwareVersionStringLength + 1] ={0};
    size_t outLen = 0;
    #if 0
    memset(buf, 0, bufSize);
    
    BekenConfig::ReadConfigValueStr(BekenConfig::kConfigKey_SoftwareVersionString, temp,  sizeof(temp) - 1 , outLen);
    ChipLogError(DeviceLayer," %s %d GetSoftwareVersionString %s\r\n",__FUNCTION__,__LINE__,temp);
    ReturnErrorCodeIf(bufSize < strlen(temp), CHIP_ERROR_BUFFER_TOO_SMALL);
    ReturnErrorCodeIf(strlen(temp) > ConfigurationManager::kMaxSoftwareVersionStringLength, CHIP_ERROR_INTERNAL);
    strcpy(buf, temp);
    #endif
    #if 1
    uint32_t software_version = 0;
    memset(buf, 0, bufSize);
    BekenConfig::ReadConfigValue(BekenConfig::kConfigKey_SoftwareVersion, software_version);
    sprintf(temp, "%ld.0", software_version);
    ReturnErrorCodeIf(bufSize < strlen(temp), CHIP_ERROR_BUFFER_TOO_SMALL);
    strcpy(buf, temp);
    #endif
    return CHIP_NO_ERROR;
}

CHIP_ERROR ConfigurationManagerImpl::GetSoftwareVersion(uint32_t & softwareVer)
{
    #if 0
    softwareVer = CHIP_CONFIG_SOFTWARE_VERSION_NUMBER;
    #endif
    BekenConfig::ReadConfigValue(BekenConfig::kConfigKey_SoftwareVersion,softwareVer);

    ChipLogError(DeviceLayer,"%s %d mSoftwareVersion %lu \r\n",__FUNCTION__,__LINE__,softwareVer);
    return CHIP_NO_ERROR;
}

CHIP_ERROR ConfigurationManagerImpl::GetBootReason(uint32_t & bootReason)
{
    return ReadConfigValue(BekenConfig::kCounterKey_BootReason, bootReason);
}

CHIP_ERROR ConfigurationManagerImpl::StoreBootReason(uint32_t bootReason)
{
    return WriteConfigValue(BekenConfig::kCounterKey_BootReason, bootReason);
}

CHIP_ERROR ConfigurationManagerImpl::GetPrimaryWiFiMACAddress(uint8_t * buf)
{
    bk_wifi_get_station_mac_address((char*)buf);
    return CHIP_NO_ERROR;
}

bool ConfigurationManagerImpl::CanFactoryReset()
{
    // TODO: query the application to determine if factory reset is allowed.
    return true;
}

void ConfigurationManagerImpl::InitiateFactoryReset()
{
    PlatformMgr().ScheduleWork(DoFactoryReset);
}

CHIP_ERROR ConfigurationManagerImpl::ReadPersistedStorageValue(::chip::Platform::PersistedStorage::Key key, uint32_t & value)
{
    BekenConfig::Key configKey{ BekenConfig::kConfigNamespace_ChipCounters, key };

    CHIP_ERROR err = ReadConfigValue(configKey, value);
    if (err == CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::WritePersistedStorageValue(::chip::Platform::PersistedStorage::Key key, uint32_t value)
{
    BekenConfig::Key configKey{ BekenConfig::kConfigNamespace_ChipCounters, key };
    return WriteConfigValue(configKey, value);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, bool & val)
{
    return BekenConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, uint32_t & val)
{
    return BekenConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, uint64_t & val)
{
    return BekenConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen)
{
    return BekenConfig::ReadConfigValueStr(key, buf, bufSize, outLen);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    return BekenConfig::ReadConfigValueBin(key, buf, bufSize, outLen);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, bool val)
{
    return BekenConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, uint32_t val)
{
    return BekenConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, uint64_t val)
{
    return BekenConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueStr(Key key, const char * str)
{
    return BekenConfig::WriteConfigValueStr(key, str);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueStr(Key key, const char * str, size_t strLen)
{
    return BekenConfig::WriteConfigValueStr(key, str, strLen);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen)
{
    return BekenConfig::WriteConfigValueBin(key, data, dataLen);
}

void ConfigurationManagerImpl::RunConfigUnitTest(void)
{
    BekenConfig::RunConfigUnitTest();
}

void ConfigurationManagerImpl::DoFactoryReset(intptr_t arg)
{
    CHIP_ERROR err;

    ChipLogProgress(DeviceLayer, "Performing factory reset");

    // Erase all values in the chip-config NVS namespace.
    err = BekenConfig::ClearNamespace(BekenConfig::kConfigNamespace_ChipConfig);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "ClearNamespace(ChipConfig) failed: %s", chip::ErrorStr(err));
    }

    // Restart the system.
    ChipLogProgress(DeviceLayer, "System restarting");
    bk_reboot();
}

ConfigurationManager & ConfigurationMgrImpl()
{
    return ConfigurationManagerImpl::GetDefaultInstance();
}

} // namespace DeviceLayer
} // namespace chip

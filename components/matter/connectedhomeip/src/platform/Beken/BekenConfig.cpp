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

/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <lib/core/CHIPEncoding.h>
#include <platform/Beken/BekenConfig.h>
#include <lib/support/CHIPMem.h>
#include <lib/support/CHIPMemString.h>
#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>

#include "matter_pal.h"

enum
{
    kPrefsTypeBoolean = 1,
    kPrefsTypeInteger = 2,
    kPrefsTypeString  = 3,
    kPrefsTypeBuffer  = 4,
    kPrefsTypeBinary  = 5
};

namespace chip {
namespace DeviceLayer {
namespace Internal {

// *** CAUTION ***: Changing the names or namespaces of these values will *break* existing devices.

// NVS namespaces used to store device configuration information.
const char BekenConfig::kConfigNamespace_ChipFactory[]  = "chip-factory";
const char BekenConfig::kConfigNamespace_ChipConfig[]   = "chip-config";
const char BekenConfig::kConfigNamespace_ChipCounters[] = "chip-counters";

// Keys stored in the chip-factory namespace
const BekenConfig::Key BekenConfig::kConfigKey_SerialNum             = { kConfigNamespace_ChipFactory, "serial-num" };
const BekenConfig::Key BekenConfig::kConfigKey_MfrDeviceId           = { kConfigNamespace_ChipFactory, "device-id" };
const BekenConfig::Key BekenConfig::kConfigKey_MfrDeviceCert         = { kConfigNamespace_ChipFactory, "device-cert" };
const BekenConfig::Key BekenConfig::kConfigKey_MfrDeviceICACerts     = { kConfigNamespace_ChipFactory, "device-ca-certs" };
const BekenConfig::Key BekenConfig::kConfigKey_MfrDevicePrivateKey   = { kConfigNamespace_ChipFactory, "device-key" };
const BekenConfig::Key BekenConfig::kConfigKey_HardwareVersion       = { kConfigNamespace_ChipFactory, "hardware-ver" };
const BekenConfig::Key BekenConfig::kConfigKey_HardwareVersionString = { kConfigNamespace_ChipFactory, "hw-ver-str" };
const BekenConfig::Key BekenConfig::kConfigKey_ManufacturingDate     = { kConfigNamespace_ChipFactory, "mfg-date" };
const BekenConfig::Key BekenConfig::kConfigKey_SetupPinCode          = { kConfigNamespace_ChipFactory, "pin-code" };
const BekenConfig::Key BekenConfig::kConfigKey_SetupDiscriminator    = { kConfigNamespace_ChipFactory, "discriminator" };
const BekenConfig::Key BekenConfig::kConfigKey_Spake2pIterationCount = { kConfigNamespace_ChipFactory, "iteration-count" };
const BekenConfig::Key BekenConfig::kConfigKey_Spake2pSalt           = { kConfigNamespace_ChipFactory, "salt" };
const BekenConfig::Key BekenConfig::kConfigKey_Spake2pVerifier       = { kConfigNamespace_ChipFactory, "verifier" };
const BekenConfig::Key BekenConfig::kConfigKey_UniqueId              = { kConfigNamespace_ChipFactory, "uniqueId" };
const BekenConfig::Key BekenConfig::kConfigKey_DACCert               = { kConfigNamespace_ChipFactory, "dac-cert" };
const BekenConfig::Key BekenConfig::kConfigKey_DACPrivateKey         = { kConfigNamespace_ChipFactory, "dac-key" };
const BekenConfig::Key BekenConfig::kConfigKey_DACPublicKey          = { kConfigNamespace_ChipFactory, "dac-pub-key" };
const BekenConfig::Key BekenConfig::kConfigKey_PAICert               = { kConfigNamespace_ChipFactory, "pai-cert" };
const BekenConfig::Key BekenConfig::kConfigKey_CertDeclaration       = { kConfigNamespace_ChipFactory, "cert-dclrn" };
const BekenConfig::Key BekenConfig::kConfigKey_VendorId              = { kConfigNamespace_ChipFactory, "vendor-id" };
const BekenConfig::Key BekenConfig::kConfigKey_VendorName            = { kConfigNamespace_ChipFactory, "vendor-name" };
const BekenConfig::Key BekenConfig::kConfigKey_ProductId             = { kConfigNamespace_ChipFactory, "product-id" };
const BekenConfig::Key BekenConfig::kConfigKey_ProductName           = { kConfigNamespace_ChipFactory, "product-name" };
const BekenConfig::Key BekenConfig::kConfigKey_ProductLabel          = { kConfigNamespace_ChipFactory, "product-label" };
const BekenConfig::Key BekenConfig::kConfigKey_ProductURL            = { kConfigNamespace_ChipFactory, "product-url" };
const BekenConfig::Key BekenConfig::kConfigKey_SupportedCalTypes     = { kConfigNamespace_ChipFactory, "cal-types" };
const BekenConfig::Key BekenConfig::kConfigKey_SupportedLocaleSize   = { kConfigNamespace_ChipFactory, "locale-sz" };
const BekenConfig::Key BekenConfig::kConfigKey_RotatingDevIdUniqueId = { kConfigNamespace_ChipFactory, "rd-id-uid" };

// Keys stored in the chip-config namespace
const BekenConfig::Key BekenConfig::kConfigKey_FabricId                    = { kConfigNamespace_ChipConfig, "fabric-id" };
const BekenConfig::Key BekenConfig::kConfigKey_ServiceConfig               = { kConfigNamespace_ChipConfig, "service-config" };
const BekenConfig::Key BekenConfig::kConfigKey_PairedAccountId             = { kConfigNamespace_ChipConfig, "account-id" };
const BekenConfig::Key BekenConfig::kConfigKey_ServiceId                   = { kConfigNamespace_ChipConfig, "service-id" };
const BekenConfig::Key BekenConfig::kConfigKey_GroupKeyIndex               = { kConfigNamespace_ChipConfig, "group-key-index" };
const BekenConfig::Key BekenConfig::kConfigKey_LastUsedEpochKeyId          = { kConfigNamespace_ChipConfig, "last-ek-id" };
const BekenConfig::Key BekenConfig::kConfigKey_FailSafeArmed               = { kConfigNamespace_ChipConfig, "fail-safe-armed" };
const BekenConfig::Key BekenConfig::kConfigKey_WiFiStationSecType          = { kConfigNamespace_ChipConfig, "sta-sec-type" };
const BekenConfig::Key BekenConfig::kConfigKey_OperationalDeviceId         = { kConfigNamespace_ChipConfig, "op-device-id" };
const BekenConfig::Key BekenConfig::kConfigKey_OperationalDeviceCert       = { kConfigNamespace_ChipConfig, "op-device-cert" };
const BekenConfig::Key BekenConfig::kConfigKey_OperationalDeviceICACerts   = { kConfigNamespace_ChipConfig, "op-device-ca-certs" };
const BekenConfig::Key BekenConfig::kConfigKey_OperationalDevicePrivateKey = { kConfigNamespace_ChipConfig, "op-device-key" };
const BekenConfig::Key BekenConfig::kConfigKey_RegulatoryLocation          = { kConfigNamespace_ChipConfig, "regulatory-location" };
const BekenConfig::Key BekenConfig::kConfigKey_CountryCode                 = { kConfigNamespace_ChipConfig, "country-code" };
const BekenConfig::Key BekenConfig::kConfigKey_Breadcrumb                  = { kConfigNamespace_ChipConfig, "breadcrumb" };
const BekenConfig::Key BekenConfig::kConfigKey_SoftwareVersion             = { kConfigNamespace_ChipConfig, "software-version" };
const BekenConfig::Key BekenConfig::kConfigKey_SoftwareVersionString       = { kConfigNamespace_ChipConfig, "software-version-str" };

// Keys stored in the Chip-counters namespace
const BekenConfig::Key BekenConfig::kCounterKey_RebootCount           = { kConfigNamespace_ChipCounters, "reboot-count" };
const BekenConfig::Key BekenConfig::kCounterKey_UpTime                = { kConfigNamespace_ChipCounters, "up-time" };
const BekenConfig::Key BekenConfig::kCounterKey_TotalOperationalHours = { kConfigNamespace_ChipCounters, "total-hours" };
const BekenConfig::Key BekenConfig::kCounterKey_BootReason            = { kConfigNamespace_ChipCounters, "boot-reason" };

CHIP_ERROR BekenConfig::ReadConfigValue(Key key, bool & val)
{
    uint32_t success    = 0;
    uint32_t out_length = 0;
    uint8_t intval      = 0;
    success             = bk_read_data(key.Namespace, key.Name, (char *) &intval, 1, &out_length);

    if (kNoErr != success)
        ChipLogProgress(DeviceLayer, "bk_read_data: %s  %s failed\n", (char *) key.Namespace, (char *) key.Name);

    val = (intval != 0);
    if (kNoErr == success)
        return CHIP_NO_ERROR;
    else
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
}

CHIP_ERROR BekenConfig::ReadConfigValue(Key key, uint16_t & val)
{
    uint32_t success    = 0;
    uint32_t out_length = 0;
    uint32_t temp_data  = 0;

    success = bk_read_data(key.Namespace, key.Name, (char *) &temp_data, sizeof(uint16_t), &out_length);

    if (kNoErr != success)
        ChipLogProgress(DeviceLayer, "bk_read_data: %s  %s failed\n", (char *) key.Namespace, (char *) key.Name);
    val = temp_data;

    if (kNoErr == success)
        return CHIP_NO_ERROR;
    else
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
}

CHIP_ERROR BekenConfig::ReadConfigValue(Key key, uint32_t & val)
{
    uint32_t success    = 0;
    uint32_t out_length = 0;
    uint32_t temp_data  = 0;

    success = bk_read_data(key.Namespace, key.Name, (char *) &temp_data, sizeof(uint32_t), &out_length);

    if (kNoErr != success)
        ChipLogProgress(DeviceLayer, "bk_read_data: %s  %s failed\n", (char *) key.Namespace, (char *) key.Name);
    val = temp_data;

    if (kNoErr == success)
        return CHIP_NO_ERROR;
    else
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
}

CHIP_ERROR BekenConfig::ReadConfigValue(Key key, uint64_t & val)
{
    uint32_t success    = 0;
    uint32_t out_length = 0;
    uint64_t temp_data  = 0;

    success = bk_read_data(key.Namespace, key.Name, (char *) &temp_data, sizeof(uint64_t), &out_length);

    if (kNoErr != success)
        ChipLogProgress(DeviceLayer, "bk_read_data: %s  %s failed\n", (char *) key.Namespace, (char *) key.Name);
    val = temp_data;

    if (kNoErr == success)
        return CHIP_NO_ERROR;
    else
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
}

CHIP_ERROR BekenConfig::ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen)
{
    uint32_t success    = 0;
    uint32_t out_length = 0;

    success = bk_read_data(key.Namespace, key.Name, buf, bufSize, &out_length);
    outLen  = out_length;

    if (kNoErr != success)
        ChipLogProgress(DeviceLayer, "bk_read_data: %s  %s failed\n", (char *) key.Namespace, (char *) key.Name);

    if (kNoErr == success)
    {
        return CHIP_NO_ERROR;
    }
    else
    {
        outLen = 0;
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
}

CHIP_ERROR BekenConfig::ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    uint32_t success    = 0;
    uint32_t out_length = 0;

    success = bk_read_data(key.Namespace, key.Name, (char *) buf, bufSize, &out_length);
    outLen  = out_length;

    if (kNoErr != success)
        ChipLogProgress(DeviceLayer, "bk_read_data: %s  %s failed\n", (char *) key.Namespace, (char *) key.Name);

    if (kNoErr == success)
    {
        return CHIP_NO_ERROR;
    }
    else
    {
        outLen = 0;
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
}

CHIP_ERROR BekenConfig::WriteConfigValue(Key key, bool val)
{
    uint32_t success = 0;
    uint8_t value    = 0;

    if (val == 1)
    {
        value = 1;
    }
    else
    {
        value = 0;
    }

    success = bk_write_data(key.Namespace, key.Name, (char *) &value, 1);
    if (kNoErr != success)
        ChipLogError(DeviceLayer, "bk_write_data: %s  %s  %s failed\n", (char *) key.Namespace, (char *) key.Name,
                     value ? "true" : "false");

    return CHIP_NO_ERROR;
}

CHIP_ERROR BekenConfig::WriteConfigValue(Key key, uint16_t val)
{
    uint32_t success = 0;

    success = bk_write_data(key.Namespace, key.Name, (char *) &val, sizeof(val));
    if (kNoErr != success)
        ChipLogError(DeviceLayer, "bk_write_data: %s  %s = %lu(0x%lx) failed\n", (char *) key.Namespace, (char *) key.Name, val,
                     val);

    return CHIP_NO_ERROR;
}

CHIP_ERROR BekenConfig::WriteConfigValue(Key key, uint32_t val)
{
    uint32_t success = 0;

    success = bk_write_data(key.Namespace, key.Name, (char *) &val, sizeof(val));
    if (kNoErr != success)
        ChipLogError(DeviceLayer, "bk_write_data: %s  %s = %lu(0x%lx) failed\n", (char *) key.Namespace, (char *) key.Name, val,
                     val);

    return CHIP_NO_ERROR;
}

CHIP_ERROR BekenConfig::WriteConfigValue(Key key, uint64_t val)
{
    uint32_t success = 0;

    success = bk_write_data(key.Namespace, key.Name, (char *) &val, sizeof(val));
    if (kNoErr != success)
        ChipLogError(DeviceLayer, "bk_write_data: %s  %s = %llu(0x%llx) failed\n", (char *) key.Namespace, (char *) key.Name, val,
                     val);

    return CHIP_NO_ERROR;
}

CHIP_ERROR BekenConfig::WriteConfigValueStr(Key key, const char * str)
{
    uint32_t success = 0;

    success = bk_write_data(key.Namespace, key.Name, (char *) str, strlen(str));
    if (kNoErr != success)
        ChipLogError(DeviceLayer, "bk_write_data: %s %s %s failed\n", (char *) key.Namespace, (char *) key.Name, (char *) str);

    return CHIP_NO_ERROR;
}

CHIP_ERROR BekenConfig::WriteConfigValueStr(Key key, const char * str, size_t strLen)
{
    uint32_t success = 0;
    strLen           = (strLen > strlen(str)) ? strlen(str) : strLen;

    success = bk_write_data(key.Namespace, key.Name, (char *) str, strLen);
    if (kNoErr != success)
        ChipLogError(DeviceLayer, "bk_write_data: %s %s %s failed\n", (char *) key.Namespace, (char *) key.Name, (char *) str);

    return CHIP_NO_ERROR;
}

CHIP_ERROR BekenConfig::WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen)
{
    uint32_t success = 0;

    success = bk_write_data(key.Namespace, key.Name, (char *) data, dataLen);
    if (kNoErr != success)
        ChipLogError(DeviceLayer, "bk_write_data: %s  %s failed \r\n", key.Namespace, key.Name);

    return CHIP_NO_ERROR;
}

CHIP_ERROR BekenConfig::ClearConfigValue(Key key)
{
    uint32_t success = 0;

    success = bk_clean_data(key.Namespace, key.Name);
    if (kNoErr != success)
        ChipLogProgress(DeviceLayer, "%s : %s  %s failed\n", __FUNCTION__, key.Namespace, key.Name);
    return CHIP_NO_ERROR;
}

bool BekenConfig::ConfigValueExists(Key key)
{
    if (kNoErr == bk_ensure_name_data(key.Namespace, key.Name))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

CHIP_ERROR BekenConfig::EnsureNamespace(const char * ns)
{
    uint32_t success = 0;

    success = bk_ensure_namespace(ns);
    if (kNoErr != success)
    {
        ChipLogError(DeviceLayer, "dct_register_module failed\n");
    }
    return CHIP_NO_ERROR;
}

CHIP_ERROR BekenConfig::ClearNamespace(const char * ns)
{
    uint32_t success = 0;

    success = bK_clear_namespace(ns);
    if (success != 0)
    {
        ChipLogError(DeviceLayer, "ClearNamespace failed\n");
    }
    return CHIP_NO_ERROR;
}

void BekenConfig::RunConfigUnitTest() {}

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip

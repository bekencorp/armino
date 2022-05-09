/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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

#include <lib/support/CodeUtils.h>
#include <lib/support/SafeInt.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/Beken/NetworkCommissioningDriver.h>

#include <limits>
#include <string>

#include "wlan_ui_pub.h"

using namespace ::chip;
#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
namespace chip {
namespace DeviceLayer {
namespace NetworkCommissioning {

namespace {
constexpr char kWiFiSSIDKeyName[]        = "wifi-ssid";
constexpr char kWiFiCredentialsKeyName[] = "wifi-pass";
static uint8_t WiFiSSIDStr[DeviceLayer::Internal::kMaxWiFiSSIDLength];
} // namespace

CHIP_ERROR BekenWiFiDriver::Init()
{
    CHIP_ERROR err;
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::Init\r\n");

    err = GetSavedNetWorkConfig(&mSavedNetwork);

    mStagingNetwork   = mSavedNetwork;
    mpScanCallback    = nullptr;
    mpConnectCallback = nullptr;
    return err;
}

CHIP_ERROR BekenWiFiDriver::GetSavedNetWorkConfig(WiFiNetwork *WifiNetconf)
{
    CHIP_ERROR err;
    size_t ssidLen        = 0;
    size_t credentialsLen = 0;

    memset(WifiNetconf, 0x0, sizeof(WiFiNetwork));
    err = PersistedStorage::KeyValueStoreMgr().Get(kWiFiCredentialsKeyName, WifiNetconf->credentials,
                                                   sizeof(WifiNetconf->credentials), &credentialsLen);
    if (err == CHIP_ERROR_NOT_FOUND)
    {
        return CHIP_NO_ERROR;
    }

    err = PersistedStorage::KeyValueStoreMgr().Get(kWiFiSSIDKeyName, WifiNetconf->ssid, sizeof(WifiNetconf->ssid), &ssidLen);
    if (err == CHIP_ERROR_NOT_FOUND)
    {
        return CHIP_NO_ERROR;
    }
    WifiNetconf->credentialsLen = credentialsLen;
    WifiNetconf->ssidLen        = ssidLen;
    return err;
}

CHIP_ERROR BekenWiFiDriver::Shutdown()
{
    return CHIP_NO_ERROR;
}

CHIP_ERROR BekenWiFiDriver::CommitConfiguration()
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::CommitConfiguration\r\n");
    ReturnErrorOnFailure(PersistedStorage::KeyValueStoreMgr().Put(kWiFiSSIDKeyName, mStagingNetwork.ssid, mStagingNetwork.ssidLen));
    ReturnErrorOnFailure(PersistedStorage::KeyValueStoreMgr().Put(kWiFiCredentialsKeyName, mStagingNetwork.credentials,
                                                                  mStagingNetwork.credentialsLen));
    mSavedNetwork = mStagingNetwork;
    return CHIP_NO_ERROR;
}

CHIP_ERROR BekenWiFiDriver::RevertConfiguration()
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::RevertConfiguration\r\n");
    mStagingNetwork = mSavedNetwork;
    return CHIP_NO_ERROR;
}

bool BekenWiFiDriver::NetworkMatch(const WiFiNetwork & network, ByteSpan networkId)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::NetworkMatch\r\n");
    return networkId.size() == network.ssidLen && memcmp(networkId.data(), network.ssid, network.ssidLen) == 0;
}

Status BekenWiFiDriver::AddOrUpdateNetwork(ByteSpan ssid, ByteSpan credentials)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::AddOrUpdateNetwork\r\n");
    VerifyOrReturnError(mStagingNetwork.ssidLen == 0 || NetworkMatch(mStagingNetwork, ssid), Status::kBoundsExceeded);
    VerifyOrReturnError(credentials.size() <= sizeof(mStagingNetwork.credentials), Status::kOutOfRange);
    VerifyOrReturnError(ssid.size() <= sizeof(mStagingNetwork.ssid), Status::kOutOfRange);

    memcpy(mStagingNetwork.credentials, credentials.data(), credentials.size());
    mStagingNetwork.credentialsLen = static_cast<decltype(mStagingNetwork.credentialsLen)>(credentials.size());

    memcpy(mStagingNetwork.ssid, ssid.data(), ssid.size());
    mStagingNetwork.ssidLen = static_cast<decltype(mStagingNetwork.ssidLen)>(ssid.size());

    return Status::kSuccess;
}

Status BekenWiFiDriver::RemoveNetwork(ByteSpan networkId)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::RemoveNetwork\r\n");
    VerifyOrReturnError(NetworkMatch(mStagingNetwork, networkId), Status::kNetworkIDNotFound);

    // Use empty ssid for representing invalid network
    mStagingNetwork.ssidLen = 0;
    return Status::kSuccess;
}

Status BekenWiFiDriver::ReorderNetwork(ByteSpan networkId, uint8_t index)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::ReorderNetwork\r\n");
    // Only one network is supported now
    VerifyOrReturnError(index == 0, Status::kOutOfRange);
    VerifyOrReturnError(NetworkMatch(mStagingNetwork, networkId), Status::kNetworkIDNotFound);
    return Status::kSuccess;
}

CHIP_ERROR BekenWiFiDriver::ConnectWiFiNetwork(const char * ssid, uint8_t ssidLen, const char * key, uint8_t keyLen)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::ConnectWiFiNetwork....ssid:%s", ssid);
    ReturnErrorOnFailure(ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Disabled));

    wifi_sta_config_t sta_config;

    memset(&sta_config, 0x0, sizeof(sta_config));
    sta_config.security = WIFI_SECURITY_AUTO;//can't use WIFI_DEFAULT_STA_CONFIG because of C99 designator error
    strncpy(sta_config.ssid, ssid, ssidLen);
    strncpy(sta_config.password, key, keyLen);

    BK_LOG_ON_ERR(bk_wifi_sta_set_config(&sta_config));
    BK_LOG_ON_ERR(bk_wifi_sta_start());
    BK_LOG_ON_ERR(bk_wifi_sta_connect());

    return ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Enabled);
}

void BekenWiFiDriver::OnConnectWiFiNetwork()
{
     ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::OnConnectWiFiNetwork\r\n");
     if (mpConnectCallback)
    {
        mpConnectCallback->OnResult(Status::kSuccess, CharSpan(), 0);
        mpConnectCallback = nullptr;
    }
}

void BekenWiFiDriver::ConnectNetwork(ByteSpan networkId, ConnectCallback * callback)
{
    CHIP_ERROR err          = CHIP_NO_ERROR;
    Status networkingStatus = Status::kSuccess;

    VerifyOrExit(NetworkMatch(mStagingNetwork, networkId), networkingStatus = Status::kNetworkIDNotFound);
    VerifyOrExit(mpConnectCallback == nullptr, networkingStatus = Status::kUnknownError);

    err               = ConnectWiFiNetwork(reinterpret_cast<const char *>(mStagingNetwork.ssid), mStagingNetwork.ssidLen,
                             reinterpret_cast<const char *>(mStagingNetwork.credentials), mStagingNetwork.credentialsLen);
    mpConnectCallback = callback;
exit:
    if (err != CHIP_NO_ERROR)
    {
        networkingStatus = Status::kUnknownError;
    }
    if (networkingStatus != Status::kSuccess)
    {
        ChipLogError(NetworkProvisioning, "Failed to connect to WiFi network:%s", chip::ErrorStr(err));
        mpConnectCallback = nullptr;
        callback->OnResult(networkingStatus, CharSpan(), 0);
    }
}

CHIP_ERROR BekenWiFiDriver::StartScanWiFiNetworks(ByteSpan ssid)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::StartScanWiFiNetworks\r\n");
    CHIP_ERROR err          = CHIP_NO_ERROR;
    if (ssid.data())//directed scanning
    {
        uint8_t **ssid_array;
        uint8_t * oob_ssid;
	memcpy(oob_ssid,ssid.data(),ssid.size());
        ssid_array = &oob_ssid;       
	ChipLogProgress(NetworkProvisioning, "StartScanWiFiNetworks... ssid:%s\r\n", ssid.data());
	bk_wlan_start_assign_scan(ssid_array, 1);
    }
    else//non-directed scanning
    {
        bk_wlan_start_scan();
    }
	
    return CHIP_NO_ERROR;
}

void BekenWiFiDriver::OnScanWiFiNetworkDone()
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::OnScanWiFiNetworkDone\r\n");
    char scan_rst_ap_num = 0;     
    scan_rst_ap_num = bk_wlan_get_scan_ap_result_numbers();
    if(scan_rst_ap_num == 0)
    {
        os_printf("NULL AP\r\n");
        return;
    }
    sta_scan_res *scan_rst_table = ( sta_scan_res *)pvPortMalloc(sizeof(sta_scan_res) * scan_rst_ap_num);
    if(scan_rst_table == NULL)
    {
        os_printf("scan_rst_table malloc failed!\r\n");
        return;
    }
    bk_wlan_get_scan_ap_result(scan_rst_table, scan_rst_ap_num);
	
}

void BekenWiFiDriver::ScanNetworks(ByteSpan ssid, WiFiDriver::ScanCallback * callback)
{
    if (callback != nullptr)
    {
        mpScanCallback = callback;
        if (StartScanWiFiNetworks(ssid) != CHIP_NO_ERROR)
        {
            mpScanCallback = nullptr;
            callback->OnFinished(Status::kUnknownError, CharSpan(), nullptr);
        }
    }
}

CHIP_ERROR GetConnectedNetwork(Network & network)
{
     ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver  GetConnectedNetwork\r\n");
#ifdef FANG
    wifi_ap_record_t ap_info;
    esp_err_t err;
    err = esp_wifi_sta_get_ap_info(&ap_info);
    if (err != ESP_OK)
    {
        return chip::DeviceLayer::Internal::ESP32Utils::MapError(err);
    }
    uint8_t length = strnlen(reinterpret_cast<const char *>(ap_info.ssid), DeviceLayer::Internal::kMaxWiFiSSIDLength);
    if (length > sizeof(network.networkID))
    {
        return CHIP_ERROR_INTERNAL;
    }
    memcpy(network.networkID, ap_info.ssid, length);
    network.networkIDLen = length;
#endif
    return CHIP_NO_ERROR;
}

size_t BekenWiFiDriver::WiFiNetworkIterator::Count()
{
     ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::WiFiNetworkIterator::Count\r\n");
    return mDriver->mStagingNetwork.ssidLen == 0 ? 0 : 1;
}

bool BekenWiFiDriver::WiFiNetworkIterator::Next(Network & item)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::WiFiNetworkIterator::Next\r\n");
    if (mExhausted || mDriver->mStagingNetwork.ssidLen == 0)
    {
        return false;
    }
    memcpy(item.networkID, mDriver->mStagingNetwork.ssid, mDriver->mStagingNetwork.ssidLen);
    item.networkIDLen = mDriver->mStagingNetwork.ssidLen;
    item.connected    = false;
    mExhausted        = true;

    Network connectedNetwork;
    CHIP_ERROR err = GetConnectedNetwork(connectedNetwork);
    if (err == CHIP_NO_ERROR)
    {
        if (connectedNetwork.networkIDLen == item.networkIDLen &&
            memcmp(connectedNetwork.networkID, item.networkID, item.networkIDLen) == 0)
        {
            item.connected = true;
        }
    }
    return true;
}

} // namespace NetworkCommissioning
} // namespace DeviceLayer
} // namespace chip
#endif // CHIP_DEVICE_CONFIG_ENABLE_WIFI

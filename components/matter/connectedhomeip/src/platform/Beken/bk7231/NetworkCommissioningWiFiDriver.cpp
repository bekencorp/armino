/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
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
#include <platform/Beken/NetworkCommissioningDriver.h>
#include <platform/CHIPDeviceLayer.h>

#include <limits>
#include <string>

using namespace ::chip;
#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
namespace chip {
namespace DeviceLayer {
namespace NetworkCommissioning {

namespace {
constexpr char kWiFiSSIDKeyName[]        = "wifi-ssid";
constexpr char kWiFiCredentialsKeyName[] = "wifi-pass";

} // namespace

bool BKScanResponseIterator::Next(WiFiScanResponse & item)
{
    ScanResult_adv * apList = (ScanResult_adv *) mpScanResults;

    if (mIternum >= mSize)
    {
        return false;
    }
    uint8_t ssidlenth = strlen(apList->ApList[mIternum].ssid);
    item.security.SetRaw(NC_SECURITYCONVERT(apList->ApList[mIternum].security));
    item.ssidLen = ssidlenth;
    item.channel  = apList->ApList[mIternum].channel;
    item.wiFiBand = chip::DeviceLayer::NetworkCommissioning::WiFiBand::k2g4;
    item.rssi     = apList->ApList[mIternum].ApPower;
    memcpy(item.ssid, apList->ApList[mIternum].ssid, ssidlenth);
    memcpy(item.bssid, apList->ApList[mIternum].bssid, 6);

    mIternum++;
    return true;
}


CHIP_ERROR BekenWiFiDriver::Init(NetworkStatusChangeCallback * networkStatusChangeCallback)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::Init\r\n");
    CHIP_ERROR err;
    size_t ssidLen        = 0;
    size_t credentialsLen = 0;

    err = PersistedStorage::KeyValueStoreMgr().Get(kWiFiCredentialsKeyName, mSavedNetwork.credentials,
                                                   sizeof(mSavedNetwork.credentials), &credentialsLen);
    if (err == CHIP_ERROR_NOT_FOUND)
    {
        return CHIP_NO_ERROR;
    }

    err = PersistedStorage::KeyValueStoreMgr().Get(kWiFiSSIDKeyName, mSavedNetwork.ssid, sizeof(mSavedNetwork.ssid), &ssidLen);
    if (err == CHIP_ERROR_NOT_FOUND)
    {
        return CHIP_NO_ERROR;
    }
    mSavedNetwork.credentialsLen = credentialsLen;
    mSavedNetwork.ssidLen        = ssidLen;

    mStagingNetwork              = mSavedNetwork;
    mpScanCallback               = nullptr;
    mpConnectCallback            = nullptr;
    mpStatusChangeCallback       = networkStatusChangeCallback;
    return err;
}

void BekenWiFiDriver::Shutdown()
{
    mpStatusChangeCallback = nullptr;
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

Status BekenWiFiDriver::AddOrUpdateNetwork(ByteSpan ssid, ByteSpan credentials, MutableCharSpan & outDebugText,
                                           uint8_t & outNetworkIndex)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::AddOrUpdateNetwork\r\n");

    outDebugText.reduce_size(0);
    outNetworkIndex = 0;
    VerifyOrReturnError(mStagingNetwork.ssidLen == 0 || NetworkMatch(mStagingNetwork, ssid), Status::kBoundsExceeded);
    VerifyOrReturnError(credentials.size() <= sizeof(mStagingNetwork.credentials), Status::kOutOfRange);
    VerifyOrReturnError(ssid.size() <= sizeof(mStagingNetwork.ssid), Status::kOutOfRange);

    memcpy(mStagingNetwork.credentials, credentials.data(), credentials.size());
    mStagingNetwork.credentialsLen = static_cast<decltype(mStagingNetwork.credentialsLen)>(credentials.size());

    memcpy(mStagingNetwork.ssid, ssid.data(), ssid.size());
    mStagingNetwork.ssidLen = static_cast<decltype(mStagingNetwork.ssidLen)>(ssid.size());

    return Status::kSuccess;
}

Status BekenWiFiDriver::RemoveNetwork(ByteSpan networkId, MutableCharSpan & outDebugText, uint8_t & outNetworkIndex)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::RemoveNetwork\r\n");

    outDebugText.reduce_size(0);
    outNetworkIndex = 0;
    VerifyOrReturnError(NetworkMatch(mStagingNetwork, networkId), Status::kNetworkIDNotFound);

    // Use empty ssid for representing invalid network
    mStagingNetwork.ssidLen = 0;
    return Status::kSuccess;
}

Status BekenWiFiDriver::ReorderNetwork(ByteSpan networkId, uint8_t index, MutableCharSpan & outDebugText)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::ReorderNetwork\r\n");
    // Only one network is supported now
    outDebugText.reduce_size(0);
    VerifyOrReturnError(index == 0, Status::kOutOfRange);
    VerifyOrReturnError(NetworkMatch(mStagingNetwork, networkId), Status::kNetworkIDNotFound);
    return Status::kSuccess;
}

CHIP_ERROR BekenWiFiDriver::ConnectWiFiNetwork(const char * ssid, uint8_t ssidLen, const char * key, uint8_t keyLen)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::ConnectWiFiNetwork....ssid:%s", ssid);
    ReturnErrorOnFailure(ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Disabled));

    network_InitTypeDef_st network_cfg;

    memset(&network_cfg, 0, sizeof(network_InitTypeDef_st));

    network_cfg.wifi_mode = BK_STATION;
    memcpy(network_cfg.wifi_ssid, ssid, ssidLen);
    memcpy(network_cfg.wifi_key, key, keyLen);
    network_cfg.dhcp_mode = DHCP_CLIENT;
    bk_wlan_start(&network_cfg);
	
    return ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Enabled);
}

void BekenWiFiDriver::OnConnectWiFiNetwork()
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::OnConnectWiFiNetwork\r\n");
    if (mpConnectCallback)
    {
        chip::DeviceLayer::PlatformMgr().LockChipStack();
        mpConnectCallback->OnResult(Status::kSuccess, CharSpan(), 0);
        chip::DeviceLayer::PlatformMgr().UnlockChipStack();
        mpConnectCallback = nullptr;
    }
}

void BekenWiFiDriver::ConnectNetwork(ByteSpan networkId, ConnectCallback * callback)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::ConnectNetwork\r\n");
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
        //chip::DeviceLayer::PlatformMgr().LockChipStack();
        callback->OnResult(networkingStatus, CharSpan(), 0);
        //chip::DeviceLayer::PlatformMgr().UnlockChipStack();
    }
}

CHIP_ERROR GetConnectedNetwork(Network & network)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::GetConnectedNetwork\r\n");
    LinkStatusTypeDef wifi_setting={0};
    bk_wlan_get_link_status(&wifi_setting);
    uint8_t length = strnlen(reinterpret_cast<const char *>(wifi_setting.ssid), DeviceLayer::Internal::kMaxWiFiSSIDLength);
    memcpy(network.networkID, wifi_setting.ssid, length);
    ChipLogProgress(NetworkProvisioning, "networkID:[%s][%d]\r\n",network.networkID,length);
    network.networkIDLen = length;//yellan sizeof(network.networkID);
    return CHIP_NO_ERROR;
}

void BekenWiFiDriver::OnNetworkStatusChange()
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::OnNetworkStatusChange\r\n");
    Network configuredNetwork = { 0 };

    VerifyOrReturn(mpStatusChangeCallback != nullptr);
    GetConnectedNetwork(configuredNetwork);

    if (configuredNetwork.networkID[0])
    {
        mpStatusChangeCallback->OnNetworkingStatusChange(
            Status::kSuccess, MakeOptional(ByteSpan(configuredNetwork.networkID, configuredNetwork.networkIDLen)), NullOptional);
        return;
    }
    mpStatusChangeCallback->OnNetworkingStatusChange(
        Status::kUnknownError, MakeOptional(ByteSpan(configuredNetwork.networkID, configuredNetwork.networkIDLen)),
        MakeOptional(GetLastDisconnectReason()));
}

CHIP_ERROR BekenWiFiDriver::SetLastDisconnectReason(const ChipDeviceEvent * event)
{
    //VerifyOrReturnError(event->Type == DeviceEventType::kRtkWiFiStationDisconnectedEvent, CHIP_ERROR_INVALID_ARGUMENT);
    //mLastDisconnectedReason = wifi_get_last_error();
    //TODO
    return CHIP_NO_ERROR;
}

int32_t BekenWiFiDriver::GetLastDisconnectReason()
{
    return mLastDisconnectedReason;
}

CHIP_ERROR BekenWiFiDriver::StartScanWiFiNetworks(ByteSpan ssid)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::StartScanWiFiNetworks\r\n");
    CHIP_ERROR err          = CHIP_NO_ERROR;
    if (ssid.data())//directed scanning
    {
        uint8_t **ssid_array;
        uint8_t *oob_ssid = (uint8_t*)malloc(ssid.size()*sizeof(uint8_t));
        memcpy(oob_ssid,ssid.data(),ssid.size());
        ChipLogProgress(NetworkProvisioning, "directed scanning... ssid:%s ; %d \r\n",oob_ssid,ssid.size());
        ssid_array = &oob_ssid;
	    bk_wlan_start_assign_scan(ssid_array, 1);
    }
    else//non-directed scanning
    {
        ChipLogProgress(NetworkProvisioning, "non-directed scanning...\r\n" );
        bk_wlan_start_scan();
    }
    return CHIP_NO_ERROR;
}

void BekenWiFiDriver::OnScanWiFiNetworkDone()
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::OnScanWiFiNetworkDone\r\n");
    ScanResult_adv apList={0};//yellan 20220919
    int scan_rst_ap_num = 0;
    if (wlan_sta_scan_result(&apList) == 0) {
        scan_rst_ap_num = apList.ApNum;
		ChipLogProgress(NetworkProvisioning,"Got ap count: %d\r\n", apList.ApNum);
    }
    if(scan_rst_ap_num == 0)
    {
        os_printf("NULL AP\r\n");
        GetInstance().mpScanCallback->OnFinished(Status::kSuccess, CharSpan(), nullptr);
        GetInstance().mpScanCallback = nullptr;
        return;
    }
    BKScanResponseIterator iter(scan_rst_ap_num, &apList);
    if (GetInstance().mpScanCallback)
    {
        GetInstance().mpScanCallback->OnFinished(Status::kSuccess, CharSpan(), &iter);
        GetInstance().mpScanCallback = nullptr;
    }
    else
    {
        ChipLogProgress(NetworkProvisioning, "can't find the ScanCallback function\r\n");
    }
}

void scan_ap_cb(void *ctxt, uint8_t param)
{
    DeviceLayer::SystemLayer().ScheduleLambda([]() {
        NetworkCommissioning::BekenWiFiDriver::GetInstance().OnScanWiFiNetworkDone();
    });
}

void BekenWiFiDriver::ScanNetworks(ByteSpan ssid, WiFiDriver::ScanCallback * callback)
{
    ChipLogProgress(NetworkProvisioning, "BekenWiFiDriver::ScanNetworks\r\n");
    if (callback != nullptr)
    {
        mpScanCallback = callback;
        bk_wlan_scan_ap_reg_cb(scan_ap_cb);
        if (StartScanWiFiNetworks(ssid) != CHIP_NO_ERROR)
        {
            mpScanCallback = nullptr;
            callback->OnFinished(Status::kUnknownError, CharSpan(), nullptr);
        }
    }
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
    Network connectedNetwork={0};
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

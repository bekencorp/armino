/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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

#include "CHIPDeviceManager.h"
#include "DeviceCallbacks.h"
#include "Server.h"

#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include <platform/CHIPDeviceLayer.h>
#include <support/CHIPMem.h>

#include <app/clusters/identify-server/identify-server.h>
#include <app/clusters/network-commissioning/network-commissioning.h>
#include <app/server/OnboardingCodesUtil.h>
#include <lib/support/ErrorStr.h>
#include <platform/Beken/BekenConfig.h>
#include <platform/Beken/NetworkCommissioningDriver.h>
#include <setup_payload/ManualSetupPayloadGenerator.h>
#include <setup_payload/QRCodeSetupPayloadGenerator.h>

//#if CONFIG_ENABLE_OTA_REQUESTOR
#include "app/clusters/ota-requestor/BDXDownloader.h"
#include "app/clusters/ota-requestor/OTARequestor.h"
#include "platform/Beken/OTAImageProcessorImpl.h"
#include "platform/GenericOTARequestorDriver.h"
//#endif

using chip::OTAImageProcessorImpl;
using chip::BDXDownloader;
using chip::ByteSpan;
using chip::EndpointId;
using chip::FabricIndex;
using chip::GetRequestorInstance;
using chip::NodeId;
using chip::OnDeviceConnected;
using chip::OnDeviceConnectionFailure;
using chip::OTADownloader;
using chip::OTAImageProcessorParams;
using chip::OTARequestor;
using chip::VendorId;
using chip::Callback::Callback;
using chip::System::Layer;
using chip::Transport::PeerAddress;
using namespace chip::Messaging;
using namespace chip::app::Clusters::OtaSoftwareUpdateProvider::Commands;


using namespace ::chip;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceManager;
using namespace ::chip::DeviceLayer;
using namespace ::chip::System;

namespace {

static DeviceCallbacks EchoCallbacks;
OTARequestor gRequestorCore;
GenericOTARequestorDriver gRequestorUser;
BDXDownloader gDownloader;
OTAImageProcessorImpl gImageProcessor;

app::Clusters::NetworkCommissioning::Instance
    sWiFiNetworkCommissioningInstance(0 /* Endpoint Id */, &(NetworkCommissioning::BekenWiFiDriver::GetInstance()));
} // namespace

// need to check CONFIG_RENDEZVOUS_MODE
bool isRendezvousBLE()
{
    RendezvousInformationFlags flags = RendezvousInformationFlags(CONFIG_RENDEZVOUS_MODE);
    return flags.Has(RendezvousInformationFlag::kBLE);
}

//#if CONFIG_ENABLE_OTA_REQUESTOR
extern "C" void QueryImageCmdHandler()
{
    ChipLogProgress(DeviceLayer, "Calling QueryImageCmdHandler");
    static_cast<OTARequestor *>(GetRequestorInstance())->TriggerImmediateQuery();
}

extern "C" void ApplyUpdateCmdHandler()
{
    ChipLogProgress(DeviceLayer, "Calling ApplyUpdateCmdHandler");

    static_cast<OTARequestor *>(GetRequestorInstance())->ApplyUpdate();
}

extern "C" void NotifyUpdateAppliedHandler(uint32_t version)
{
    ChipLogProgress(DeviceLayer, "NotifyUpdateApplied");
    
    static_cast<OTARequestor *>(GetRequestorInstance())->NotifyUpdateApplied(version);
}

/*********************************************************************
 * Funtion Name:BkQueryImageCmdHandler
 *
 * Funtion Discription:trigger ota requestor queries image from ota provider
 *
 * 
 * Date:2022-02-22
 *******************************************************************/
extern "C" void BkQueryImageCmdHandler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv )
{
#if 0
    uint32_t dwLoop = 0;
    uint32_t nodeId = 0;
    uint32_t fabricId = 0;

    char cmd0 = 0;
    char cmd1 = 0;

    for(dwLoop = 0; dwLoop < argc; dwLoop++)
    {
        ChipLogProgress(DeviceLayer, "QueryImageArgument %d = %s\r\n", dwLoop + 1, argv[dwLoop]);
    }

    if(argc == 3)
    {
        cmd0 = argv[1][0] - 0x30;
        cmd1 = argv[1][1] - 0x30;
        nodeId = (uint32_t)(cmd0 * 10 + cmd1);
        
        cmd0 = argv[2][0] - 0x30;
        cmd1 = argv[2][1] - 0x30;
        fabricId = (uint32_t)(cmd0 * 10 + cmd1);
        ChipLogProgress(DeviceLayer, "nodeId %lu,fabricId %lu\r\n", nodeId,fabricId);
    }
    else
    {
        ChipLogProgress(DeviceLayer,"cmd param error ");
        return ;
    }
#endif
    QueryImageCmdHandler();
    ChipLogProgress(DeviceLayer,"QueryImageCmdHandler begin");

    return ;
}

/*********************************************************************
 * Funtion Name:BkApplyUpdateCmdHandler
 *
 * Funtion Discription:trigger ota requestor queries image from ota provider
 *
 * 
 * Date:2022-02-22
 *******************************************************************/
extern "C" void BkApplyUpdateCmdHandler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv )
{
    ApplyUpdateCmdHandler();
    ChipLogProgress(DeviceLayer,"ApplyUpdateCmdHandler send request");

    return ;
}

/*********************************************************************
 * Funtion Name:BkNotifyUpdateApplied
 *
 * Funtion Discription:trigger ota requestor notify update applied to ota provider
 *
 * 
 * Date:2022-03-10
 *******************************************************************/
extern "C" void BkNotifyUpdateApplied(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv )
{
    uint32_t dwLoop = 0;
    uint32_t version = 0;

    char cmd0 = 0;
    char cmd1 = 0;

    for(dwLoop = 0; dwLoop < argc; dwLoop++)
    {
        ChipLogProgress(DeviceLayer, "NotifyUpdateApplied %d = %s\r\n", dwLoop + 1, argv[dwLoop]);
    }

    if(argc == 2)
    {
        cmd0 = argv[1][0] - 0x30;
        cmd1 = argv[1][1] - 0x30;
        version = (uint32_t)(cmd0 * 10 + cmd1);
        
        ChipLogProgress(DeviceLayer, "version %lu \r\n", version);
    }
    else
    {
        ChipLogProgress(DeviceLayer,"cmd param error ");
        return ;
    }

    NotifyUpdateAppliedHandler( version);
    ChipLogProgress(DeviceLayer,"NotifyUpdateApplied send request");

    return ;
}

static void InitOTARequestor(void)
{
    // Initialize and interconnect the Requestor and Image Processor objects -- START
    SetRequestorInstance(&gRequestorCore);
    ChipLogProgress(DeviceLayer,"InitOTARequestor gRequestorCore init");

    // Set server instance used for session establishment
    /*  - Set server instance used to get access to the system resources necessary to open CASE sessions and drive
    *   - BDX transfers
    *   - Set the OTA requestor driver instance used to communicate download progress and errors
    *   - Set the BDX downloader instance used for initiating BDX downloads
    */
    gRequestorCore.Init(&(chip::Server::GetInstance()), &gRequestorUser, &gDownloader);

    // WARNING: this is probably not realistic to know such details of the image or to even have an OTADownloader instantiated at
    // the beginning of program execution. We're using hardcoded values here for now since this is a reference application.
    // TODO: instatiate and initialize these values when QueryImageResponse tells us an image is available
    // TODO: add API for OTARequestor to pass QueryImageResponse info to the application to use for OTADownloader init
    OTAImageProcessorParams ipParams;
    gImageProcessor.SetOTAImageProcessorParams(ipParams);
    gImageProcessor.SetOTADownloader(&gDownloader);

    // Connect the Downloader and Image Processor objects
    gDownloader.SetImageProcessorDelegate(&gImageProcessor);
    gRequestorUser.Init(&gRequestorCore, &gImageProcessor);

    // Initialize and interconnect the Requestor and Image Processor objects -- END
}
//#endif // CONFIG_ENABLE_OTA_REQUESTOR

void OnIdentifyStart(Identify *)
{
    ChipLogProgress(Zcl, "OnIdentifyStart");
}

void OnIdentifyStop(Identify *)
{
    ChipLogProgress(Zcl, "OnIdentifyStop");
}

void OnTriggerEffect(Identify * identify)
{
    switch (identify->mCurrentEffectIdentifier)
    {
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_BLINK:
        ChipLogProgress(Zcl, "EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_BLINK");
        break;
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_BREATHE:
        ChipLogProgress(Zcl, "EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_BREATHE");
        break;
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_OKAY:
        ChipLogProgress(Zcl, "EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_OKAY");
        break;
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_CHANNEL_CHANGE:
        ChipLogProgress(Zcl, "EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_CHANNEL_CHANGE");
        break;
    default:
        ChipLogProgress(Zcl, "No identifier effect");
        return;
    }
}

static Identify gIdentify1 = {
    chip::EndpointId{ 1 }, OnIdentifyStart, OnIdentifyStop, EMBER_ZCL_IDENTIFY_IDENTIFY_TYPE_VISIBLE_LED, OnTriggerEffect,
};

//Warkaround for ld error:undefined reference to '__sync_synchronize'
//refer to https://stackoverflow.com/questions/64658430/gnu-arm-embedded-toolchain-undefined-reference-to-sync-synchronize
extern "C" void __sync_synchronize() {}

extern "C" unsigned int __atomic_fetch_add_4(volatile void * ptr, unsigned int val, int memorder)
{
    return (*(unsigned int*)ptr + val);
}

extern "C" bool __atomic_compare_exchange_4(volatile void* pulDestination, void* ulComparand, unsigned int desired, bool weak, int success_memorder, int failure_memorder)
{
    bool ulReturnValue;
    if( *(unsigned int*)pulDestination == *(unsigned int*)ulComparand )
    {
        *(unsigned int*)pulDestination = desired;
         ulReturnValue = true;
    }
    else
    {
        *(unsigned int*)ulComparand = *(unsigned int*)pulDestination;
        ulReturnValue = false;
    }
    return ulReturnValue;
}

extern "C" unsigned int __atomic_fetch_sub_4(volatile void * ptr, unsigned int val, int memorder)
{
    return (*(unsigned int*)ptr + val);
}
extern "C" bool __atomic_compare_exchange_1(volatile void* pulDestination, void* ulComparand, unsigned char desired, bool weak, int success_memorder, int failure_memorder)
{
    bool ulReturnValue;
    if( *(unsigned char*)pulDestination == *(unsigned char*)ulComparand )
    {
        *(unsigned char*)pulDestination = desired;
         ulReturnValue = true;
    }
    else
    {
        *(unsigned char*)ulComparand = *(unsigned char*)pulDestination;
        ulReturnValue = false;
    }
    return ulReturnValue;
}

extern "C" unsigned int __atomic_fetch_and_4(volatile void *pulDestination, unsigned int ulValue, int memorder)
{
    unsigned int ulCurrent;

    ulCurrent = *(unsigned int*)pulDestination;
    *(unsigned int*)pulDestination &= ulValue;
    return ulCurrent;
}

extern "C" bool __sync_bool_compare_and_swap_4(volatile void* ptr, unsigned int oldval, unsigned int newval)
{
    if(*(unsigned int*)ptr == oldval)
    {
        *(unsigned int*)ptr = newval;
        return true;
    }
    else
    {
        return false;
    }
}
/* stub for __libc_init_array */
extern "C" void _fini(void) { }
extern "C" void _init(void) {;}

static void InitServer(intptr_t context)
{
    // Init ZCL Data Model and CHIP App Server
    chip::Server::GetInstance().Init();

    // Initialize device attestation config
    SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
    sWiFiNetworkCommissioningInstance.Init();
    PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));
}

extern "C" void ChipTest(void)
{
    ChipLogProgress(Zcl, "ChipTest");
    CHIP_ERROR err = CHIP_NO_ERROR;
    //initPref();

    CHIPDeviceManager & deviceMgr = CHIPDeviceManager::GetInstance();
    err                           = deviceMgr.Init(&EchoCallbacks); //start the CHIP task
    if (err != CHIP_NO_ERROR)
    {
        ChipLogProgress(Zcl, "DeviceManagerInit() - ERROR!");
    }
    else
    {
        ChipLogProgress(Zcl, "DeviceManagerInit() - OK");
    }
    chip::DeviceLayer::PlatformMgr().ScheduleWork(InitServer, reinterpret_cast<intptr_t>(nullptr));
    InitOTARequestor();
    while (true)
        vTaskDelay(pdMS_TO_TICKS(50)); //Just server the application event handler
    ChipLogProgress(SoftwareUpdate, "Exited");
	return;
}

bool lowPowerClusterSleep()
{
    return true;
}

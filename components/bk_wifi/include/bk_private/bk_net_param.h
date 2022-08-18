// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "wlan_defs_pub.h"

#define INFO_TLV_HEADER           (0x00564c54)   // ASCII TLV

typedef enum{
#if CONFIG_WIFI_FAST_CONNECT
    FAST_CONNECT_ITEM	= 0x00001111,
#endif
    AUTO_CONNECT_ITEM	= 0x11111111,
    WIFI_MODE_ITEM	= 0x22222222,
    DHCP_MODE_ITEM	= 0x33333333,
    WIFI_MAC_ITEM	= 0x44444444,
    SSID_KEY_ITEM	= 0x55555555,
    IP_CONFIG_ITEM	= 0x66666666,
    RF_CFG_TSSI_ITEM    = 0x77777777,
    RF_CFG_DIST_ITEM    = 0x88888888,
    RF_CFG_MODE_ITEM    = 0x99999999,
    CHARGE_CONFIG_ITEM  = 0xaaaaaaaa,
    RF_CFG_TSSI_B_ITEM  = 0xbbbbbbbb,
    USER_COMM1_ITEM	= 0x11112222,	
    DID_STR_ITEM	= 0xcccccccc,
    USER_PSW_ITEM	= 0xdddddddd,
    CAMERA_PARA_ITEM	= 0xeeeeeeee,
    REBOOT_FLAG_ITEM    = 0xffffffff
}NET_INFO_ITEM;

typedef struct info_item_st
{
    UINT32 type;
    UINT32 len;
}INFO_ITEM_ST,TLV_HEADER_ST,*INFO_ITEM_ST_PTR;

typedef struct item_common_st
{
    INFO_ITEM_ST head;
    UINT32 value;
}ITEM_COMM_ST,*ITEM_COMM_ST_PTR;

typedef struct item_mac_addr_st
{
    INFO_ITEM_ST head;
    char mac[6];
    char reserved[2];// 4bytes boundary
}ITEM_MAC_ADDR_ST,*ITEM_MAC_ADDR_ST_PTR;

typedef struct item_charge_st
{
    INFO_ITEM_ST head;
#if (CONFIG_SOC_BK7251)
    char chrg[4];
#else
    char chrg[3];
    char reserved[1];
#endif
}ITEM_CHARGE_ST,*ITEM_CHARGE_ST_PTR;

#if CONFIG_WIFI_FAST_CONNECT
typedef struct item_fastconnect_st
{
    INFO_ITEM_ST head;
	uint8_t ssid[33];
	uint8_t bssid[6];
	uint8_t security;
	uint8_t channel;
	uint8_t psk[65];
	uint8_t pwd[65];
	uint8_t ip_addr[4];
	uint8_t netmask[4];
	uint8_t gw[4];
	uint8_t dns1[4];
}ITEM_FASTCONNECT_ST,*ITEM_FASTCONNECT_ST_PTR;
#endif

typedef struct item_ssidkey_st
{
    INFO_ITEM_ST head;
    char wifi_ssid[32];
    char wifi_key[64]; 
}ITEM_SSIDKEY_ST,*ITEM_SSIDKEY_ST_PTR;

typedef struct item_ip_config_st
{
    INFO_ITEM_ST head;
    char local_ip_addr[16];
    char net_mask[16];
    char gateway_ip_addr[16];    
}ITEM_IP_CONFIG_ST,*ITEM_IP_CONFIG_ST_PTR;

UINT32 test_get_whole_tbl(UINT8 *ptr);
UINT32 save_info_item(NET_INFO_ITEM item,UINT8 *ptr0,UINT8*ptr1,UINT8 *ptr2);
UINT32 get_info_item(NET_INFO_ITEM item,UINT8 *ptr0,UINT8 *ptr1, UINT8 *ptr2);

#ifdef __cplusplus
}
#endif

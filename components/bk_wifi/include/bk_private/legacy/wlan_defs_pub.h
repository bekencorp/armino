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
#include <common/bk_include.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief WiFi APIs Version 1 (Old WiFi API)
 * @addtogroup bk_api_wifi_v1 Old WiFi API group
 * @{
 */

/**
 * @brief WLAN encryption type
 * @defgroup bk_api_wifi_v1_enum enum
 * @ingroup bk_api_wifi_v1
 * @{
 */
enum {
	WLAN_ENC_OPEN,  /**< No encryption */
	WLAN_ENC_WEP,   /**< WiFi encryption algorithmn is WEP */
	WLAN_ENC_CCMP,  /**< WiFi encryption algorithmn is CCMP */
	WLAN_ENC_TKIP,  /**< WiFi encryption algorithmn is TKIP */
};

#define WiFi_Interface  wlanInterfaceTypedef /**< WiFi interface */

#define DHCP_DISABLE  (0)   /**< Disable DHCP service. */
#define DHCP_CLIENT   (1)   /**< Enable DHCP client which get IP address from DHCP server automatically */
#define DHCP_SERVER   (2)   /**< Enable DHCP server, needs assign a static address as local address. */

typedef enum _wifi_dis_reason
{
	UNSUPPORT_ENCRYPT = 0, /**< Unsupport encrypt algorithmn */
	WRONG_PASSPHRASE,      /**< WiFi disconnect because configured wrong password */
	DEAUTHENTICATION,      /**< WiFi disconnect because of receiving deauth frame */
	DISASSOCIATION,        /**< WiFi disconnect because of receiving disassociation frame */
	AP_MISSING,            /**< WiFi disconnect because AP is missing, eg. AP is powered off etc */
	WLAN_DISCONNECT,       /**< WiFi is disconnected by application */
} WIFI_DISCONN_REASON;         /**< WiFi reason code */

typedef enum
{
	BK_SOFT_AP,  /**< Act as an access point, and other station can connect, 4 stations Max*/
	BK_STATION,   /**< Act as a station which can connect to an access point*/
	BK_P2P
} wlanInterfaceTypedef;

enum bk_wlan_sec_type_e
{
	BK_SECURITY_TYPE_NONE,        /**< Open system. */
	BK_SECURITY_TYPE_WEP,         /**< Wired Equivalent Privacy. WEP security. */
	BK_SECURITY_TYPE_WPA_TKIP,    /**< WPA /w TKIP */
	BK_SECURITY_TYPE_WPA_AES,     /**< WPA /w AES */
	BK_SECURITY_TYPE_WPA2_TKIP,   /**< WPA2 /w TKIP */
	BK_SECURITY_TYPE_WPA2_AES,    /**< WPA2 /w AES */
	BK_SECURITY_TYPE_WPA2_MIXED,  /**< WPA2 /w AES or TKIP */
	BK_SECURITY_TYPE_WPA3_SAE,    /**< WPA3 SAE */
	BK_SECURITY_TYPE_WPA3_WPA2_MIXED, /**< WPA3 SAE or WPA2 AES */
	BK_SECURITY_TYPE_EAP,         /**< EAP */
	BK_SECURITY_TYPE_OWE,         /**< OWE */
	BK_SECURITY_TYPE_AUTO,        /**< It is used when calling @ref bk_wlan_start_sta_adv, _BK_ read security type from scan result. */
};

enum
{
	WLAN_RX_BEACON,    /**< receive beacon packet */
	WLAN_RX_PROBE_REQ, /**< receive probe request packet */
	WLAN_RX_PROBE_RES, /**< receive probe response packet */
	WLAN_RX_ACTION,    /**< receive action packet */
	WLAN_RX_MANAGEMENT,/**< receive ALL management packet */
	WLAN_RX_DATA,      /**< receive ALL data packet */
	WLAN_RX_MCAST_DATA,/**< receive ALL multicast and broadcast packet */
	WLAN_RX_ALL,       /**< receive ALL 802.11 packet */
};

typedef uint8_t wlan_sec_type_t;  /**< WiFi security type */

#define MONITOR_FILTER_MUL_BRD_CAST     (1U << 0) /**< Multicast frame filter bit */
#define MONITOR_FILTER_DUPL_FRM         (1U << 1) /**< Duplicated frame filter bit */

/**
 * @}
 */

/**
 * @brief WLAN encryption type
 * @defgroup bk_api_wifi_v1_typedef struct
 * @ingroup bk_api_wifi_v1 struct
 * @{
 */

typedef struct _wifi_link_info
{
	uint8_t bssid[6];           /**< BSSID of link */
	int16_t rssi;               /**< RSSI of link */
	uint32_t data_rate;         /**< Data rate TODO */
	WIFI_DISCONN_REASON reason; /**< Disconnect reason TODO */
} WIFI_LINK_INFO_T;

/**
 *  @brief  wlan local IP information structure definition.
 */
typedef struct
{
	uint8_t dhcp;       /**< DHCP mode: @ref DHCP_DISABLE, @ref DHCP_CLIENT, @ref DHCP_SERVER.*/
	char    ip[16];     /**< Local IP address on the target wlan interface: @ref wlanInterfaceTypedef.*/
	char    gate[16];   /**< Router IP address on the target wlan interface: @ref wlanInterfaceTypedef.*/
	char    mask[16];   /**< Netmask on the target wlan interface: @ref wlanInterfaceTypedef.*/
	char    dns[16];    /**< DNS server IP address.*/
	char    mac[16];    /**< MAC address, example: "C89346112233".*/
	char    broadcastip[16]; /**< Broadcast IP address */
} IPStatusTypedef;

/**
 *  @brief  Scan result using normal scan.
 */
typedef  struct  _ScanResult
{
	char ApNum;       /**< The number of access points found in scanning. */
	struct _ApList
	{
		char ssid[33];  /**< The SSID of an access point. */
		char ApPower;   /**< Signal strength, min:0, max:100. */
	} *ApList;
} ScanResult;

/**
 *      @brief  Scan result using advanced scan.
 */
typedef  struct  _ScanResult_adv
{
	char ApNum;       /**< The number of access points found in scanning.*/
	struct ApListStruct
	{
		char ssid[33];    /**< The SSID of an access point.*/
		char ApPower;     /**< Signal strength, min:0, max:100*/
		uint8_t bssid[6]; /**< The BSSID of an access point.*/
		char channel;     /**< The RF frequency, 1-13*/
		wlan_sec_type_t security;       /**< Security type, see wlan_sec_type_t */
	} *ApList;  /**< AP list found by scan */
} ScanResult_adv;

/**
 * @brief  The info of the APs found by scan.
 *
 * The WiFi driver scans the APs by parsing the received beacon or probe response of
 * the AP and all following info are came from the beacon or probe response.
 */
typedef struct sta_scan_res
{
	UINT8 bssid[6];    /**< The BSSID of AP */
	char ssid[32];     /**< The SSID of AP */
	char on_channel;   /**< Indicate whether the channel in DS IE is valid 2.4G channel */
	char channel;      /**< The channel of AP */
	UINT16 beacon_int; /**< Beacon interval of AP */
	UINT16 caps;       /**< Capability IE of AP */
	int level;         /**< RSSI of the received frame */
	int security;      /**< Encryption algorithm of AP */
	UINT8 tsf[8];      /**< TSF value in beacon or probe response */
	UINT32 ie_len;     /**< IE length of beacon or probe response */
	/* Followed by ie_len of IE data */
} SCAN_RST_ITEM_T, *SCAN_RST_ITEM_PTR;

/**
 *  @brief  Input network paras, used in bk_wlan_start function.
 */
typedef struct _network_InitTypeDef_st
{
	char wifi_mode;               /**< WiFi mode, BK_SOFT_AP or BK_STATION */
	char wifi_ssid[33];           /**< For station, it's SSID of the AP to be connected, for AP, it's the SSID of our AP */
	char wifi_key[65];            /**< Security key of the wlan needs to be connected, ignored in an open system.*/
	char local_ip_addr[16];       /**< Static IP configuration, Local IP address. */
	char net_mask[16];            /**< Static IP configuration, Netmask. */
	char gateway_ip_addr[16];     /**< Static IP configuration, Router IP address. */
	char dns_server_ip_addr[16];  /**< Static IP configuration, DNS server IP address. */
	char dhcp_mode;               /**< DHCP mode, @ref DHCP_DISABLE, @ref DHCP_CLIENT and @ref DHCP_SERVER. */
	char wifi_bssid[6];           /**< For station, it's the BSSID of AP to be connected, for AP, it's BSSID of our AP */
	char reserved[26];            /**< Reserved */
	int  wifi_retry_interval;     /**< Retry interval if an error is occured when connecting an access point,
						time unit is millisecond. */
} network_InitTypeDef_st;

/**
 *  @brief  Advanced precise wlan parameters, used in @ref network_InitTypeDef_adv_st.
 */
typedef struct
{
	char    ssid[32];    /**< SSID of the wlan that needs to be connected. Example: "SSID String". */
	char    bssid[6];    /**< BSSID of the wlan needs to be connected. Example: {0xC8 0x93 0x46 0x11 0x22 0x33}. */
	uint8_t channel;     /**< Wlan's RF frequency, channel 0-13. 1-13 means a fixed channel
                            that can speed up a connection procedure, 0 is not a fixed input
                            means all channels are possible*/
	wlan_sec_type_t security; /**< Security type of AP */
} apinfo_adv_t;

/**
 *  @brief  Input network precise paras in bk_wlan_start_sta_adv function.
 */
typedef struct _network_InitTypeDef_adv_st
{
	apinfo_adv_t ap_info;         /**< @ref apinfo_adv_t. */
	char  key[64];                /**< Security key or PMK of the wlan. */
	int   key_len;                /**< The length of the key. */
	char  local_ip_addr[16];      /**< Static IP configuration, Local IP address. */
	char  net_mask[16];           /**< Static IP configuration, Netmask. */
	char  gateway_ip_addr[16];    /**< Static IP configuration, Router IP address. */
	char  dns_server_ip_addr[16]; /**< Static IP configuration, DNS server IP address. */
	char  dhcp_mode;              /**< DHCP mode, @ref DHCP_DISABLE, @ref DHCP_CLIENT and @ref DHCP_SERVER. */
	char  reserved[32];           /**< reserved */
	int   wifi_retry_interval;    /**< Retry interval if an error is occured when connecting an access point,
					time unit is millisecond. */
} network_InitTypeDef_adv_st;

typedef struct _network_InitTypeDef_ap_st
{
	char wifi_ssid[32];           /**< SSID of AP */
	char wifi_key[64];            /**< key of AP */
	uint8_t channel;              /**< Channel of AP */
	wlan_sec_type_t security;     /**< Security type of AP */
	uint8_t ssid_hidden;          /**< Whether the AP is hidden */
	uint8_t max_con;              /**< Max number of stations allowed to connect in, TODO? */
	char local_ip_addr[16];       /**< IP of AP */
	char net_mask[16];            /**< Network mask of AP */
	char gateway_ip_addr[16];     /**< Gateway IP of AP */
	char dns_server_ip_addr[16];  /**< DNS server IP of AP */
	char dhcp_mode;               /**< DHCP mode */
	char reserved[32];            /**< Reserved */
	int  wifi_retry_interval;     /**< For STA only */
} network_InitTypeDef_ap_st;

/**
 *  @brief  Current link status in station mode.
 */
typedef struct _linkStatus_t
{
	int conn_state;         /**< The link to wlan is established or not, 0: disconnected, 1: connected. */
	int wifi_strength;      /**< Signal strength of the current connected AP */
	uint8_t  ssid[32];      /**< SSID of the current connected wlan */
	uint8_t  bssid[6];      /**< BSSID of the current connected wlan */
	int      channel;       /**< Channel of the current connected wlan */
	wlan_sec_type_t security; /**< Security type of WiFi link */
} LinkStatusTypeDef;

typedef struct
{
        int8_t rssi; /**< RSSI of WiFi link, generally it's the RSSI of the received WiFi frame */
} wifi_link_info_t;

/*same with RL_BSSID_INFO_T{}*/
struct wlan_fast_connect_info
{
	uint8_t ssid[33]; /**< SSID of AP */
	uint8_t bssid[6]; /**< BSSID of AP */
	uint8_t security; /**< Security of AP */
	uint8_t channel;  /**< Channel of AP */
	uint8_t psk[65];  /**< PSK of AP */
	uint8_t pwd[65];  /**< password of AP */
	uint8_t ip_addr[4];
	uint8_t netmask[4];
	uint8_t gw[4];
	uint8_t dns1[4];
};

typedef struct vif_addcfg_st {
	char *ssid;   /**< SSID of VIF */
	char *key;    /**< Key of VIF */
	char *name;   /**< Name of VIF, can be "sta" or "softap" */
	u8 wlan_role; /**< Role of VIF, can be BK_SOFT_AP or BK_STATION */
	u8 adv;       /**< Only for station role, indicate whether connect the AP with advance method */
} VIF_ADDCFG_ST, *VIF_ADDCFG_PTR;


/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

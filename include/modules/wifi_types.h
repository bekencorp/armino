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

#include <common/sys_config.h>
#include <stdint.h>
#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief WiFi APIs Version 2 (New WiFi API)
 * @addtogroup bk_api_wifi_v2 New WiFi API group
 * @{
 */

/**
 * @brief WiFi enum
 * @defgroup bk_api_wifi_v2_enum enum
 * @ingroup bk_api_wifi_v2
 * @{
 */

#define BK_ERR_WIFI_NOT_INIT            (BK_ERR_WIFI_BASE - 1) /**< WiFi is not initialized, call bk_wifi_init() to init the WiFi */
#define BK_ERR_WIFI_STA_NOT_STARTED     (BK_ERR_WIFI_BASE - 2) /**< STA is not started, call bk_wifi_sta_start() to start the STA */
#define BK_ERR_WIFI_AP_NOT_STARTED      (BK_ERR_WIFI_BASE - 3) /**< AP is not initialized, call bk_wifi_ap_start() to start the AP */
#define BK_ERR_WIFI_CHAN_RANGE          (BK_ERR_WIFI_BASE - 4) /**< Invalid channel range */
#define BK_ERR_WIFI_COUNTRY_POLICY      (BK_ERR_WIFI_BASE - 5) /**< Invalid country policy */
#define BK_ERR_WIFI_RESERVED_FIELD      (BK_ERR_WIFI_BASE - 6) /**< Reserved fields not 0 */
#define BK_ERR_WIFI_MONITOR_IP          (BK_ERR_WIFI_BASE - 7) /**< Monitor is in progress */
#define BK_ERR_WIFI_STA_NOT_CONFIG      (BK_ERR_WIFI_BASE - 8) /**< STA is not configured, call bk_wifi_sta_config() to configure it */
#define BK_ERR_WIFI_AP_NOT_CONFIG       (BK_ERR_WIFI_BASE - 9) /**< AP is not configured, call bk_wifi_ap_config() to configure it */
#define BK_ERR_WIFI_DRIVER              (BK_ERR_WIFI_BASE - 10) /**< Internal WiFi driver error */
#define BK_ERR_WIFI_MONITOR_ENTER       (BK_ERR_WIFI_BASE - 11) /**< WiFi failed to enter monitor mode */
#define BK_ERR_WIFI_DRIVER_DEL_VIF      (BK_ERR_WIFI_BASE - 12) /**< WiFi driver failed to delete WiFi virtual interface */
#define BK_ERR_WIFI_DRIVER_AP_START     (BK_ERR_WIFI_BASE - 13) /**< WiFi driver failed to start BK AP */
#define BK_ERR_WIFI_CHAN_NUMBER         (BK_ERR_WIFI_BASE - 14) /**< Invalid channel number */

#define WIFI_MIN_CHAN_NUM    1      /**< Minimum supported channel number */
#define WIFI_MAX_CHAN_NUM    14     /**< Maximum supported channel number */

#define WIFI_SSID_STR_LEN    (32+1) /**< Maximum **NULL-terminated** WiFi SSID length */
#define WIFI_BSSID_LEN       6      /**< Length of BSSID */
#define WIFI_MAC_LEN         6      /**< Length of MAC */

#define WIFI_PASSWORD_LEN    (64+1) /**< Maximum **NULL-terminated** WiFi password length */

/**
 * @brief default init configuration
 * */
#define WIFI_DEFAULT_INIT_CONFIG() {\
	.features = 0,\
	}

/**
 * @brief default STA configuration
 * */
#define WIFI_DEFAULT_STA_CONFIG() {\
	.ssid = "sta_default_ssid",\
	.bssid = {0},\
	.channel = 0,\
	.security = WIFI_SECURITY_AUTO,\
	.password = "12345678",\
	.reserved = {0},\
	}

/**
 * @brief default AP configuration
 * */
#define WIFI_DEFAULT_AP_CONFIG() {\
	.ssid = "ap_default_ssid",\
	.password = "",\
	.channel = 0,\
	.security = WIFI_SECURITY_WPA2_MIXED,\
	.hidden = 0,\
	.max_con = 0,\
	.reserved = {0},\
	}

/**
 * @brief default Scan configuration
 */
#define WIFI_DEFAULT_SCAN_CONFIG() {\
	.ssid = "scan_default_ssid",\
	.reserved = {0},\
}

/**
 * @brief default Filter configuration
 */
#define WIFI_DEFAULT_FILTER_CONFIG() {\
	.rx_all_default_mgmt = 1,\
	.rx_probe_req = 0,\
	.rx_probe_rsp = 0,\
	.rx_all_beacon = 0,\
	.rx_action = 0,\
	.reserved = 0,\
}

/**
 * @brief WiFi public event type
 */
typedef enum {
	EVENT_WIFI_SCAN_DONE = 0,      /**< WiFi scan done event */

	EVENT_WIFI_STA_CONNECTED,      /**< The BK STA is connected */
	EVENT_WIFI_STA_DISCONNECTED,   /**< The BK STA is disconnected */

	EVENT_WIFI_AP_CONNECTED,       /**< A STA is connected to the BK AP */
	EVENT_WIFI_AP_DISCONNECTED,    /**< A STA is disconnected from the BK AP */
	EVENT_WIFI_COUNT,              /**< WiFi event count */
} wifi_event_t;

typedef enum {
	WIFI_REASON_UNSPECIFIED = 1,
	WIFI_REASON_PREV_AUTH_NOT_VALID = 2,
	WIFI_REASON_DEAUTH_LEAVING = 3,
	WIFI_REASON_DISASSOC_DUE_TO_INACTIVITY = 4,
	WIFI_REASON_DISASSOC_AP_BUSY = 5,
	WIFI_REASON_CLASS2_FRAME_FROM_NONAUTH_STA = 6,
	WIFI_REASON_CLASS3_FRAME_FROM_NONASSOC_STA = 7,
	WIFI_REASON_DISASSOC_STA_HAS_LEFT = 8,
	WIFI_REASON_STA_REQ_ASSOC_WITHOUT_AUTH = 9,
	WIFI_REASON_PWR_CAPABILITY_NOT_VALID = 10,
	WIFI_REASON_SUPPORTED_CHANNEL_NOT_VALID = 11,
	WIFI_REASON_BSS_TRANSITION_DISASSOC = 12,
	WIFI_REASON_INVALID_IE = 13,
	WIFI_REASON_MICHAEL_MIC_FAILURE = 14,
	WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT = 15,
	WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,
	WIFI_REASON_IE_IN_4WAY_DIFFERS = 17,
	WIFI_REASON_GROUP_CIPHER_NOT_VALID = 18,
	WIFI_REASON_PAIRWISE_CIPHER_NOT_VALID = 19,
	WIFI_REASON_AKMP_NOT_VALID = 20,
	WIFI_REASON_UNSUPPORTED_RSN_IE_VERSION = 21,
	WIFI_REASON_INVALID_RSN_IE_CAPAB = 22,
	WIFI_REASON_IEEE_802_1X_AUTH_FAILED = 23,
	WIFI_REASON_CIPHER_SUITE_REJECTED = 24,
	WIFI_REASON_TDLS_TEARDOWN_UNREACHABLE = 25,
	WIFI_REASON_TDLS_TEARDOWN_UNSPECIFIED = 26,
	WIFI_REASON_SSP_REQUESTED_DISASSOC = 27,
	WIFI_REASON_NO_SSP_ROAMING_AGREEMENT = 28,
	WIFI_REASON_BAD_CIPHER_OR_AKM = 29,
	WIFI_REASON_NOT_AUTHORIZED_THIS_LOCATION = 30,
	WIFI_REASON_SERVICE_CHANGE_PRECLUDES_TS = 31,
	WIFI_REASON_UNSPECIFIED_QOS_REASON = 32,
	WIFI_REASON_NOT_ENOUGH_BANDWIDTH = 33,
	WIFI_REASON_DISASSOC_LOW_ACK = 34,
	WIFI_REASON_EXCEEDED_TXOP = 35,
	WIFI_REASON_STA_LEAVING = 36,
	WIFI_REASON_END_TS_BA_DLS = 37,
	WIFI_REASON_UNKNOWN_TS_BA = 38,
	WIFI_REASON_TIMEOUT = 39,
	WIFI_REASON_PEERKEY_MISMATCH = 45,
	WIFI_REASON_AUTHORIZED_ACCESS_LIMIT_REACHED = 46,
	WIFI_REASON_EXTERNAL_SERVICE_REQUIREMENTS = 47,
	WIFI_REASON_INVALID_FT_ACTION_FRAME_COUNT = 48,
	WIFI_REASON_INVALID_PMKID = 49,
	WIFI_REASON_INVALID_MDE = 50,
	WIFI_REASON_INVALID_FTE = 51,
	WIFI_REASON_MESH_PEERING_CANCELLED = 52,
	WIFI_REASON_MESH_MAX_PEERS = 53,
	WIFI_REASON_MESH_CONFIG_POLICY_VIOLATION = 54,
	WIFI_REASON_MESH_CLOSE_RCVD = 55,
	WIFI_REASON_MESH_MAX_RETRIES = 56,
	WIFI_REASON_MESH_CONFIRM_TIMEOUT = 57,
	WIFI_REASON_MESH_INVALID_GTK = 58,
	WIFI_REASON_MESH_INCONSISTENT_PARAMS = 59,
	WIFI_REASON_MESH_INVALID_SECURITY_CAP = 60,
	WIFI_REASON_MESH_PATH_ERROR_NO_PROXY_INFO = 61,
	WIFI_REASON_MESH_PATH_ERROR_NO_FORWARDING_INFO = 62,
	WIFI_REASON_MESH_PATH_ERROR_DEST_UNREACHABLE = 63,
	WIFI_REASON_MAC_ADDRESS_ALREADY_EXISTS_IN_MBSS = 64,
	WIFI_REASON_MESH_CHANNEL_SWITCH_REGULATORY_REQ = 65,
	WIFI_REASON_MESH_CHANNEL_SWITCH_UNSPECIFIED = 66,

	WIFI_REASON_BEACON_LOST = 256,       /**< The BK STA can't detect the beacon of the connected AP */
	WIFI_REASON_NO_AP_FOUND = 257,       /**< Can't find the target AP */
	WIFI_REASON_WRONG_PASSWORD = 258,    /**< The password is wrong */
	WIFI_REASON_DISCONNECT_BY_APP = 259, /**< The BK STA disconnected by application */
	WIFI_REASON_DHCP_TIMEOUT = 260,      /**<The BK STA dhcp timeout, 20s**/
	WIFI_REASON_MAX,                     /**<The BK STA connect success*/
} wifi_err_reason_t;

typedef enum {
	WIFI_SECURITY_NONE,            /**< Open system. */
	WIFI_SECURITY_WEP,             /**< WEP security, **it's unsafe security, please don't use it** */
	WIFI_SECURITY_WPA_TKIP,        /**< WPA TKIP */
	WIFI_SECURITY_WPA_AES,         /**< WPA AES */
	WIFI_SECURITY_WPA2_TKIP,       /**< WPA2 TKIP */
	WIFI_SECURITY_WPA2_AES,        /**< WPA2 AES */
	WIFI_SECURITY_WPA2_MIXED,      /**< WPA2 AES or TKIP */
	WIFI_SECURITY_WPA3_SAE,        /**< WPA3 SAE */
	WIFI_SECURITY_WPA3_WPA2_MIXED, /**< WPA3 SAE or WPA2 AES */
	WIFI_SECURITY_EAP,             /**< EAP */
	WIFI_SECURITY_OWE,             /**< OWE */
	WIFI_SECURITY_AUTO,            /**< WiFi automatically detect the security type */
} wifi_security_t;

typedef enum {
	/* for STA mode */
	WIFI_LINKSTATE_STA_IDLE = 0,      /**< sta mode is idle */
	WIFI_LINKSTATE_STA_CONNECTING,    /**< sta mode is connecting */
	WIFI_LINKSTATE_STA_DISCONNECTED,  /**< sta mode is disconnected */
	WIFI_LINKSTATE_STA_CONNECTED,     /**< sta mode is connected */
	WIFI_LINKSTATE_STA_GOT_IP,        /**< sta mode got ip */

	/* for AP mode */
	WIFI_LINKSTATE_AP_CONNECTED,      /**< softap mode, a client association success */
	WIFI_LINKSTATE_AP_DISCONNECTED,   /**< softap mode, a client disconnect */
	WIFI_LINKSTATE_AP_CONNECT_FAILED, /**< softap mode, a client association failed */
	WIFI_LINKSTATE_MAX,               /**< reserved */
	//TODO maybe we can provide more precise link status
} wifi_link_state_t;

typedef enum {
	WIFI_COUNTRY_POLICY_AUTO,   /**< Country policy is auto, use the country info of AP to which the station is connected */
	WIFI_COUNTRY_POLICY_MANUAL, /**< Country policy is manual, always use the configured country info */
} wifi_country_policy_t;            /**< WiFi country policy */

typedef enum {
	WIFI_SECOND_CHANNEL_NONE = 0,/**< HT20 mode, no secondary channel */
	WIFI_SECOND_CHANNEL_ABOVE,   /**< Second channel is above the primary channel */
	WIFI_SECOND_CHANNEL_BELOW,   /**< Second channel is below the primary channel */
} wifi_second_channel_t;

/**
 * @}
 */

/**
 * @brief WLAN struct  type
 * @defgroup bk_api_wifi_v2_typedef struct
 * @ingroup bk_api_wifi_v2 struct
 * @{
 */

typedef struct {
	uint64_t features;          /**< WiFi feature bitmaps */
	uint8_t reserved[16];       /**< Reserved */
} wifi_init_config_t;

typedef struct {
	uint8_t primary;                 /**< Primary channel */
	wifi_second_channel_t second;    /**< Secondary cahnnel */
} wifi_channel_t;

/* wpa_vendor_elem_frame */
enum wifi_sta_vsie {
	WIFI_VENDOR_ELEM_ASSOC_REQ = 0,
	WIFI_VENDOR_ELEM_PROBE_REQ = 1,
	NUM_WIFI_VENDOR_ELEM_FRAMES
};

typedef struct {
	char ssid[WIFI_SSID_STR_LEN];      /**< SSID of AP to be connected */
	uint8_t bssid[WIFI_BSSID_LEN];     /**< BSSID of AP to be connected, fast connect only */
	uint8_t channel;                   /**< Primary channel of AP to be connected, fast connect only */
	wifi_security_t security;          /**< Security of AP to be connected */
	char  password[WIFI_PASSWORD_LEN]; /**< Security key or PMK of the wlan. */
#if CONFIG_INCLUDE_STA_VSIE
	struct {
		uint8_t len;
		uint8_t buf[255];
	} vsies[NUM_WIFI_VENDOR_ELEM_FRAMES];
#endif
	uint8_t reserved[32];              /**< reserved, **must set to 0** */

#if CONFIG_INCLUDE_WPA2_ENTERPRISE
	/* starts of WPA2-Enterprise/WPA3-Enterprise configuration */
	char eap[16];                      /**< phase1 authType: TLS/TTLS/SIM */
	char identity[32];                 /**< user identity */
	char ca[32];                       /**< CA certificate filename */
	char client_cert[32];              /**< client's Certification filename in PEM,DER format */
	char private_key[32];              /**< client's private key filename in PEM,DER format */
	char private_key_passwd[32];       /**< client's private key password */
	char phase1[32];                   /**< client's phase1 parameters */
#endif

	/* auto reconnect configuration */
	int auto_reconnect_count;          /**< auto reconnect max count, 0 for always reconnect */
	int auto_reconnect_timeout;        /**< auto reconnect timeout in secs, 0 for no timeout */
	bool disable_auto_reconnect_after_disconnect;  /**< disable auto reconnect if deauth/disassoc by AP when in connected state */
} wifi_sta_config_t;

typedef struct {
	wifi_link_state_t state;           /**<Wifi linkstate*/
	wifi_err_reason_t reason_code;     /**<Wifi disconnect reason code, success will be WIFI_REASON_MAX*/
} wifi_linkstate_reason_t;

typedef struct {
	wifi_link_state_t state;           /**< The WiFi connection status */
	int aid;                           /**< STA AID */
	int rssi;                          /**< The RSSI of AP the BK STA is connected */
	char ssid[WIFI_SSID_STR_LEN];      /**< SSID of AP the BK STA is connected */
	uint8_t bssid[WIFI_BSSID_LEN];     /**< BSSID of AP the BK STA is connected */
	uint8_t channel;                   /**< Primary channel of AP the BK STA is connected */
	wifi_security_t security;          /**< Security of AP the BK STA is connected */
	char  password[WIFI_PASSWORD_LEN]; /**< Passord of AP the BK STA is connected */
} wifi_link_status_t;

typedef struct {
	char ssid[WIFI_SSID_STR_LEN];     /**< SSID to be scaned */
	uint8_t reserved[16];             /**< Reserved fields, **must be zero** */
} wifi_scan_config_t;

typedef struct {
	char ssid[WIFI_SSID_STR_LEN];      /**< SSID of AP found by scan */
	uint8_t bssid[WIFI_BSSID_LEN];     /**< BSSID of AP found by scan */
	int  rssi;                         /**< RSSI of AP found by scan */
	uint8_t channel;                   /**< The channel of the AP found by scan */
	wifi_security_t security;          /**< The security type of the AP found by scan */
	uint8_t reserved[16];              /**< Reserved, **must be zero** */
} wifi_scan_ap_info_t;

typedef struct {
	int ap_num;                        /**< The number of AP found by scan */
	wifi_scan_ap_info_t *aps;          /**< The APs found by scan */
} wifi_scan_result_t;

typedef struct {
	char ssid[WIFI_SSID_STR_LEN];      /**< The SSID of BK AP */
	char password[WIFI_PASSWORD_LEN];  /**< Password of BK AP, ignored in an open system.*/
	uint8_t channel;                   /**< The channel of BK AP, 0 indicates default TODO */
	wifi_security_t security;          /**< Security type of BK AP, default value TODO */
	uint8_t hidden: 1;                 /**< Whether the BK AP is hidden */
	uint8_t acs: 1;                    /**< Whether Auto Channel Selection is enabled */
	uint8_t vsie_len;                  /**< Beacon/ProbeResp Vendor Specific IE len */
	uint8_t vsie[255];                 /**< Beacon/ProbeResp Vendor Specific IE */
	uint8_t max_con;                   /**< Max number of stations allowed to connect to BK AP, TODO default value? */
	uint8_t reserved[32];              /**< Reserved, **must be zero** */
} wifi_ap_config_t;

/**
 * @brief Wlan station information definition
 */
typedef struct wlan_ap_sta {
	uint8_t addr[6];
	uint32_t ipaddr;
	int8_t rssi;
} wlan_ap_sta_t;

/**
 * @brief Wlan connected stations information definition
 */
typedef struct wlan_ap_stas {
	wlan_ap_sta_t *sta;
	int num;
} wlan_ap_stas_t;

/**
 * @brief softap beacon/probe response's vendor IEs
 * Dynamic change softap's vendor specific IEs.
 */
typedef struct wlan_ap_vsie {
	uint8_t len;
	uint8_t vsie[255];
} wlan_ap_vsie_t;

/**
 * @brief STA vendor IEs for (re)assoc, scan, p2p, p2p go, etc.
 * Dynamic change STA's vendor specific IEs.
 */
typedef struct wlan_sta_vsie {
	int frame;
	uint8_t len;
	uint8_t vsie[255];
} wlan_sta_vsie_t;

typedef struct {
	uint32_t rx_mcast_frame: 1;        /**< Set this bit to enable callback to receive multicast/broadcast*/
	uint32_t rx_dup_frame: 1;          /**< Set this bit to enable callback to receive the duplicated*/
	uint32_t reserved: 30;             /**< Reserved bits, **must set to 0** */
} wifi_monitor_config_t;

typedef struct {
	int rssi;                         /**< RSSI of the received frame in monitor mode */
} wifi_frame_info_t;

/**
 * @brief Monitor callback to receive frames relayed by WiFi driver.
 *
 * @attention 1. The memory of frame and frame_info will be freed immediately when the
 *               callback is returned. If we want to use the frame or frame_info in other
 *               task, make a copy of them and handle them accordingly.
 * @attention 2. Don't do too much work in the callback because the callback is called in
 *               context of WiFi core thread, too much work may impact the performance of
 *               WiFi.
 **/
typedef bk_err_t (*wifi_monitor_cb_t)(const uint8_t *frame, uint32_t len, const wifi_frame_info_t *frame_info);

typedef struct {
	uint32_t rx_all_default_mgmt: 1;  /**< Set the bit to enable the callback to receive all management frame recived by WiFi driver */
	uint32_t rx_probe_req: 1;         /**< Set the bit to enable the callback to receive probe request */
	uint32_t rx_probe_rsp: 1;         /**< Set the bit to enable the callback to receive probe response */
	uint32_t rx_all_beacon: 1;        /**< Set the bit to enable the callback to receive all beacon */
	uint32_t rx_action: 1;            /**< Set the bit to enable the callback to receive action frame */
	uint32_t reserved: 27;            /**< Reserved bits, **must set to 0** */
} wifi_filter_config_t;

/**
 * @brief Filter callback to receive frames relayed by WiFi driver.
 *
 * @attention 1. The memory of frame and frame_info will be freed immediately when the
 *               callback is returned. If we want to use the frame or frame_info in other
 *               task, make a copy of them and handle them accordingly.
 * @attention 2. Don't do too much work in the callback because the callback is called in
 *               context of WiFi core thread, too much work may impact the performance of
 *               WiFi.
 **/
typedef bk_err_t (*wifi_filter_cb_t)(const uint8_t *frame, uint32_t len, const wifi_frame_info_t *frame_info);

typedef struct {
	char                  cc[3];          /**< country code string */
	uint8_t               schan;          /**< start channel */
	uint8_t               nchan;          /**< total channel number */
	int8_t                max_tx_power;   /**< maximum tx power */
	wifi_country_policy_t policy;         /**< country policy */
} wifi_country_t;

typedef struct {
	uint32_t scan_id; /**< Scan ID */
} wifi_event_scan_done_t;

typedef struct {
	char    ssid[WIFI_SSID_STR_LEN];      /**< SSID of connected AP */
	uint8_t bssid[WIFI_BSSID_LEN];        /**< BSSID of connected AP*/
} wifi_event_sta_connected_t;

typedef struct {
	int disconnect_reason;                /**< Disconnect reason of BK STA */
	bool local_generated;                 /**< if disconnect is request by local */
} wifi_event_sta_disconnected_t;

typedef struct {
	uint8_t mac[WIFI_MAC_LEN];            /**< MAC of the STA connected to the BK AP */
} wifi_event_ap_connected_t;

typedef struct {
	uint8_t mac[WIFI_MAC_LEN];            /**< MAC of the STA disconnected from the BK AP */
} wifi_event_ap_disconnected_t;

/**
 * @brief Wi-Fi Statistic info.
 */
typedef struct {
	uint32_t tx_success_count;	/**< Number of TX successes, 0 if unavailable */
	uint32_t tx_retry_count;	/**< Number of TX retries, 0 if unavailable */
	uint32_t tx_fail_count; 	/**< Number of TX failures, 0 if unavailable */
	uint32_t rx_success_count;	/**< Number of RX successes, 0 if unavailable */
	uint32_t rx_crcerror_count; /**< Number of RX FCS errors, 0 if unavailable */
	uint32_t mic_error_count;	/**< Number of MIC errors, 0 if unavailable */
	int8_t noise;				/**< Average noise level in dBm, -128 if unavailable */
	uint16_t phyrate; 		/**< Maximum used PHY rate, 0 if unavailable */
	uint16_t txrate;			/**< Average used TX rate, 0 if unavailable */
	uint16_t rxrate;			/**< Average used RX rate, 0 if unavailable */
	int8_t rssi;				/**< Average beacon RSSI in dBm, -128 if unavailable */
	uint8_t bandwidth;		/**< Average used bandwidth, 0 if unavailable */
	uint8_t idle_time_per;		/**< Percent of idle time, 0 if unavailable */
} wifi_statistics_info_t;

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

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
#include <common/bk_err.h>
#include "wlan_defs_pub.h"
#include <modules/wifi.h>
#include "bk_rw_msg.h"
#include "bk_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief WiFi APIs Version 1.0
 * @defgroup bk_api_wifi_v1 Old WiFi API group
 * @{
 */

/* TODO add general description about legacy WiFi API
 *
 */

/**
 * @brief     configure country info
 *
 * @attention 1. The default country is {.cc="CN", .schan=1, .nchan=13, policy=WIFI_COUNTRY_POLICY_AUTO}
 * @attention 2. When the country policy is WIFI_COUNTRY_POLICY_AUTO, the country info of the AP to which
 *               the station is connected is used. E.g. if the configured country info is {.cc="USA", .schan=1, .nchan=11}
 *               and the country info of the AP to which the station is connected is {.cc="JP", .schan=1, .nchan=14}
 *               then the country info that will be used is {.cc="JP", .schan=1, .nchan=14}. If the station disconnected
 *               from the AP the country info is set back back to the country info of the station automatically,
 *               {.cc="USA", .schan=1, .nchan=11} in the example.
 * @attention 3. When the country policy is WIFI_COUNTRY_POLICY_MANUAL, always use the configured country info.
 * @attention 4. When the country info is changed because of configuration or because the station connects to a different
 *               external AP, the country IE in probe response/beacon of the soft-AP is changed also.
 * @attention 5. The country configuration is not stored into flash
 * @attention 6. This API doesn't validate the per-country rules, it's up to the user to fill in all fields according to
 *               local regulations.
 *
 * @param     country   the configured country info
 *
 * @return
 *    - kNoErr: succeed
 *    - kNotInitializedErr: WiFi is not initialized
 *    - kParamErr: invalid argument
 */
__bk_deprecated
bk_err_t bk_wlan_set_country(const wifi_country_t *country);

/**
  * @brief     get the current country info
  *
  * @param     country  country info
  *
  * @return
  *    - kNoErr: succeed
  *    - kNotInitializedErr: WiFi is not initialized
  *    - kParamErr: invalid argument
  */
__bk_deprecated
bk_err_t bk_wlan_get_country(wifi_country_t *country);

/** @brief  Connect or establish a Wi-Fi network in normal mode (station or soft ap mode).
 *
 *  This function can establish a Wi-Fi connection as a station or create
 *  a soft AP that other staions can connect (4 stations Max). In station mode,
 *  _BK_ first scan all of the supported Wi-Fi channels to find a wlan that
 *  matchs the input SSID, and read the security mode. Then try to connect
 *  to the target wlan. If any error occurs in the connection procedure or
 *  disconnected after a successful connection, _BK_ start the reconnection
 *  procedure in backgound after a time interval defined in inNetworkInitPara.
 *  Call this function twice when setup coexistence mode (staion + soft ap).
 *  This function retruns immediately in station mode, and the connection will
 *  be executed in background.
 *
 *  @param  inNetworkInitPara: Specifies wlan parameters.
 *
 *  @return In station mode, allways retrurn kWlanNoErr.
 *          In soft ap mode, return kWlanXXXErr
 */
__bk_deprecated
bk_err_t bk_wlan_start(network_InitTypeDef_st* inNetworkInitPara);

/**
  * @brief     stop STA or AP
  *
  * @param     mode mode can be BK_SOFT_AP or BK_STATION
  *
  * @return
  *    - kNoErr: succeed
  *    - otherwise: fail
  */
__bk_deprecated
int bk_wlan_stop(char mode);

/** @brief  Connect to a Wi-Fi network with advantage settings (station mode only)
 *
 *  This function can connect to an access point with precise settings,
 *  that greatly speed up the connection if the input settings are correct
 *  and fixed. If this fast connection is failed for some reason, _BK_
 *  change back to normal: scan + connect mode refer to @ref bk_wlan_start.
 *  This function returns after the fast connection try.
 *
 *  @note   This function cannot establish a soft ap, use StartNetwork() for this
 *          purpose.
 *          If input SSID length is 0, _BK_ use BSSID to connect the target wlan.
 *          If both SSID and BSSID are all wrong, the connection will be failed.
 *
 *  @param  inNetworkInitParaAdv: Specifies the precise wlan parameters.
 *
 *  @return Allways return kWlanNoErr although error occurs in first fast try
 *          kWlanTimeoutErr: DHCP client timeout
 */
__bk_deprecated
bk_err_t bk_wlan_start_sta_adv(network_InitTypeDef_adv_st* inNetworkInitParaAdv);

/** @brief  Read current wireless link status on station interface.
 *
 *  @param  outStatus: Point to the buffer to store the link status.
 *
 *  @return
 *      - kNoErr        : on success.
 *      - kGeneralErr   : fail
 */
__bk_deprecated
bk_err_t bk_wlan_get_link_status(LinkStatusTypeDef *outStatus);

/**
 * @brief  Start scan to scan all APs in all channels
 *
 * When scan is completed, the register scan callback is called and the application
 * can handle the scan result in the callback.
 *
 * TODO add more about how to use scan, backgroud scan, passive or active scan etc?
 */
__bk_deprecated
void bk_wlan_start_scan(void);

/**
 * @brief  Start scan to scan specified SSIDs in all channels
 *
 *  - When scan is completed, the register scan callback is called and the application
 * can handle the scan result in the callback.
 *  - Compare to /ref bk_wlan_start_scan, this API only scan specified SSIDs.
 *
 * TODO add more about how to use assigned scan
 *
 * @param ssid_array SSIDs to be scaned
 * @param ssid_num SSID number in ssid_ary
 */
__bk_deprecated
void bk_wlan_start_assign_scan(UINT8 **ssid_array, UINT8 ssid_num);

/**
 * @brief  Register scan completion callback
 *
 * When scan is completed, the register scan callback will be called, the application
 * can handle the result in the callback.
 *
 * @param ind_cb scan callback
 */
__bk_deprecated
void bk_wlan_scan_ap_reg_cb(FUNC_2PARAM_PTR ind_cb);

/**
 * @brief  Get the number of found APs
 *
 * In scan callback, the application can get the number of found APs by calling this API.
 *
 * @return return the number of found APs
 */
__bk_deprecated
unsigned char bk_wlan_get_scan_ap_result_numbers(void);

/**
 * @brief  Get all APs found by this scan
 *
 * In scan callback, the application can get all found by calling this API.
 *
 * @param scan_result_table store the found APs
 * @param ap_num the maxiumu AP number that scan_result_table can store
 *
 * @return return the actual number of returned APs
 */
__bk_deprecated
int bk_wlan_get_scan_ap_result(SCAN_RST_ITEM_PTR scan_result_table,unsigned char ap_num);

/**
 * @brief Send raw 802.11 frame
 *
 * @attention 1. This API can be used in WiFi station, softap, or monitor mode.
 * @attention 2. Only support to send non-QoS frame.
 * @attention 3. The frame sequence will be overwritten by WiFi driver.
 * @attention 4. The API doesn't check the correctness of the raw frame, the
 *               caller need to guarantee the correctness of the frame.
 *
 * @param buffer raw 802.11 frame
 * @param len the length of the 802.11 frame
 *
 * @return
 *    - kNoErr: success
 *    - otherwise: fail
 */
__bk_deprecated
int bk_wlan_send_80211_raw_frame(uint8_t *buffer, int len);

/**
 * @brief set filter bitmap
 *
 *  - If bit MONITOR_FILTER_MUL_BRD_CAST is set, the multicast data frame is not
 * filtered to the monitor callback;
 *  - If bit MONITOR_FILTER_DUPL_FRM is set, the duplicated data frame is not
 * filtered to the monitor callback.
 *
 * @param filter filter bitmap
 *
 *  @return
 *      - kNoErr        : on success.
 *      - kGeneralErr   : fail
 */
__bk_deprecated
void bk_wlan_set_monitor_filter(unsigned char filter);

/**
 * @brief check whether the filter_type is set or not
 *
 * This function is used internally by SDK, it's used to check whether the
 * specified filter_type is set or not.
 *
 * @param filter_type filter type
 *
 *  @return
 *      - true : if filter_type is set
 *      - false : if filter_type is not set
 */
__bk_deprecated
int bk_wlan_monitor_filter(unsigned char filter_type);

/** @brief  Start wifi monitor mode
 *
 *  @attention This function disconnects wifi station and softAP before entering monitor mode.
 *
 *  @return
 *      - kNoErr        : on success.
 *      - kGeneralErr   : fail
 */
__bk_deprecated
int bk_wlan_start_monitor(void);

/** @brief  Stop wifi monitor mode
 *
 *  @return
 *      - kNoErr        : on success.
 *      - kGeneralErr   : fail
 */
__bk_deprecated
int bk_wlan_stop_monitor(void);

/** @brief  Set the monitor channel synchronous
 *
 *  This function is for monitor mode only, it can change the channel dynamically
 *  without restarting the monitor mode.
 *
 *  @param channel the monitor channel, range is from 1 to 13
 *
 *  @return
 *      - kNoErr        : on success.
 *      - kGeneralErr   : fail
 */
__bk_deprecated
int bk_wlan_set_channel_sync(int channel);

/**
 * @brief  Set the monitor channel asynchronous
 *
 * @param channel the monitor channel, range is from 1 to 13
 *
 * @return
 *      - kNoErr        : on success.
 *      - kGeneralErr   : fail
 */
__bk_deprecated
int bk_wlan_set_channel(int channel);

/**
 * @brief  start the station
 *
 * This function disconnect the station first and the connect the station
 * with the new configuration.
 *
 * @param  inNetworkInitPara the configuration of the station
 *
 * @return
 *      - kNoErr        : on success.
 *      - kGeneralErr   : fail
 */
__bk_deprecated
bk_err_t bk_wlan_start_sta(network_InitTypeDef_st *inNetworkInitPara);

/**
 * @brief  start the AP
 *
 * This function stop the AP first and then start the AP with new configuration.
 *
 * @param  inNetworkInitPara the configuration of the AP
 *
 * @return
 *      - kNoErr        : on success.
 *      - kGeneralErr   : fail
 */
__bk_deprecated
bk_err_t bk_wlan_start_ap(network_InitTypeDef_st *inNetworkInitPara);

/**
 * @brief  start the AP
 *
 * This function stop the AP first and then start the AP with new configuration.
 * It can set more configurations than /ref bk_wlan_start_ap.
 *
 * @param  inNetworkInitParaAP the configuration of the AP
 *
 * @return
 *      - kNoErr        : on success.
 *      - kGeneralErr   : fail
 */
__bk_deprecated
bk_err_t bk_wlan_start_ap_adv(network_InitTypeDef_ap_st *inNetworkInitParaAP);

/**
 * @brief  set AP default channel
 *
 * TODO -  precondition to use this API?
 *
 * @param  channel AP channel
 */
__bk_deprecated
void bk_wlan_ap_set_default_channel(uint8_t channel);

/**
 * @brief  The callback function of 802.11 frame receiving
 *
 * When WiFi driver receives 802.11 frame that should be filtered to application,
 * the registered callback function will be called. For the filter condition,
 * refer to specific monitor register API, such as bk_wlan_register_monitor_cb etc.
 *
 * @param  data the 802.11 frame received by WiFi driver
 * @param  len the length of data
 * @param  info link info, such as rssi etc
 *
 */
typedef void (*monitor_cb_t)(uint8_t *data, int len, wifi_link_info_t *info);

/**
 * @brief  Register the monitor callback function
 *
 *  On receiving a 802.11 packet, the WiFi driver calls the registered function to handle the packets.
 *  TODO - add more description which frame should be filtered!
 *
 *  @param  fn callback function to receive the WiFi packets
 *
 * @return
 *      - kNoErr        : on success.
 *      - kGeneralErr   : fail
 */
__bk_deprecated
void bk_wlan_register_monitor_cb(monitor_cb_t fn);


#if CONFIG_DEEP_PS

/**
 * @brief  Request deep sleep, and wakeup by gpio.
 *
 * TODO - add detail?
 * TODO - why this API is so complex? can we simplify it?
 *
 * @param  gpio_index_map The gpio bitmap which set 1 enable wakeup deep sleep.
 *                         gpio_index_map is hex and every bits is map to gpio0-gpio31.
 *          gpio_edge_map The gpio edge bitmap for wakeup gpios,
 *          gpio_edge_map is hex and every bits is map to gpio0-gpio31.
 *              0:rising,1:falling.
 */
void bk_enter_deep_sleep(UINT32 gpio_index_map,
		UINT32 gpio_edge_map,
		UINT32 gpio_last_index_map,
		UINT32 gpio_last_edge_map,
		UINT32 sleep_time,
		UINT32 wake_up_way,
		UINT32 gpio_stay_lo_map,
		UINT32 gpio_stay_hi_map);

#endif

/**
 * @brief  Enable DTIM power save
 *
 * DTIM power save will enable/disable WiFi RF dynamically according to
 * WiFi power management protocol.
 *
 * TODO add more detail about DTIM power save, such as:
 *  - precondition to use it
 *  - how about AP/STA coexist?
 *  - how about WIFI/BLE coexist?
 *  - how about Monitor mode?
 *
 * @return
 *      - kNoErr   : on success.
 *      - otherwise: fail
 */
__bk_deprecated
int bk_wlan_ps_enable(void );

/**
 * @brief  Disable DTIM power save
 *
 * @return
 *      - kNoErr   : on success.
 *      - otherwise: fail
 */
__bk_deprecated
int bk_wlan_ps_disable(void);

/**
 * @brief  Enable DTIM power save timer
 *
 * TODO why we need this API? for keepalive?
 *
 * @return
 *      - kNoErr  : on success.
 *      - oterwise: fail
 */
__bk_deprecated
int bk_wlan_ps_timer_start(void);

/**
 * @brief  Disable DTIM power save timer
 *
 * TODO add details
 *
 * @return
 *      - kNoErr   : on success.
 *      - otherwise: fail
 */
__bk_deprecated
int bk_wlan_ps_timer_pause(void);

/**
 * @brief  Enable MCU power save
 *
 * When MCU power save is enabled, the power management model low the MCU
 * frequency to save power if it detects there is no activity; and resume
 * the MCU if specific activities happen, such as interrupt, timer etc.
 *
 * TODO add details.
 *
 * @return
 *      - kNoErr   : on success.
 *      - otherwise: fail
 */
__bk_deprecated
int bk_wlan_mcu_ps_mode_enable(void);

/**
 * @brief  Disable MCU power save
 *
 * TODO add details.
 *
 * @return
 *      - kNoErr   : on success.
 *      - otherwise: fail
 */
__bk_deprecated
int bk_wlan_mcu_ps_mode_disable(void);

/**
 * @brief  Register WiFi status change callback
 *
 * If WiFi status is changed, the callback is called by WiFi driver.
 *
 * TODO add details.
 *
 * @param cb status change callback function
 */
__bk_deprecated
void bk_wlan_status_register_cb(FUNC_1PARAM_PTR cb);

/**
 * @brief  Get WiFi status change callback
 *
 * @return return status callback
 */
__bk_deprecated
FUNC_1PARAM_PTR bk_wlan_get_status_cb(void);

/**
 * @brief  AP no password callback
 */
typedef void (*bk_ap_no_password_cb_t)(void);

/**
 * @brief  Register AP no password callback
 *
 * @attention Current this API has no effect at all, in other words the
 * WiFi driver never calls the callback.
 *
 * @return return status callback
 */
__bk_deprecated
void bk_ap_no_password_connected_register_cb(bk_ap_no_password_cb_t func);

/**
 * @brief  get the system MAC
 *
 * @param mac store the system MAC
 */
__bk_deprecated
void bk_wifi_get_mac_address(char *mac);

/**
 * @brief  Get maximum power level
 *
 * TODO add more description about the power level
 *
 */
__bk_deprecated
uint32_t bk_wlan_max_power_level_get(void);

/**
 * @brief  Get AP configurations
 *
 * @param ap store ap basic configurations
 * @param key store the pointer of the global key of AP
 * @param key_len store the key length
 *
 * @return
 *    - kNoErr: success
 *    - others: otherwise
 */
__bk_deprecated
bk_err_t bk_wlan_get_bssid_info(apinfo_adv_t *ap, uint8_t **key, int *key_len);

#ifdef CONFIG_AOS_MESH

/**
 * @brief  Register mesh monitor callback
 *
 * If mesh monitor callback is registered, the WiFi driver filters all received
 * 802.11 data frame to the callback.
 *
 * @param fn mesh monitor callback
 */
void wlan_register_mesh_monitor_cb(monitor_cb_t fn);

/**
 * @brief  Set mesh BSSID
 *
 * @param bssid the BSSID of mesh
 *
 * @return
 *     - kNoErr: success
 *     - others: otherwise
 */
int wlan_set_mesh_bssid(uint8_t *bssid);

/**
 * @brief  Get mesh BSSID
 *
 * This function return the global mesh BSSID
 */
uint8_t *wlan_get_mesh_bssid(void);
#endif

/**
 * @brief  Set Wi-Fi state
 *
 * This function set Wi-Fi state to stop active scan, to ensure
 *	softap video_transfer
 */
void bk_wifi_set_video_transfer_state(bool enable);

/**
 * @brief  Prepare Deepsleep
 *
 * This function set Wi-Fi prepare deep sleep
 *
 */
void bk_wifi_prepare_deepsleep(void);

typedef struct {
	uint32_t sta_start_tick;		/**< sta start connect tick */
	uint32_t sta_assoc_tick;		/**< sta complete assoc tick */
	uint32_t sta_eapol_tick;		/**< sta complete eapol tick */
	uint32_t sta_ip_tick;		/**< sta got ip tick */
}wifi_connect_tick_t;
extern wifi_connect_tick_t sta_tick;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

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

#include <modules/wifi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief WiFi APIs Version 2.0
 * @defgroup bk_api_wifi_v2 New WiFi API group
 * @{
 */

/**
 * @brief     This API initializes the WiFi driver, e.g. creating task for supplicant, allocating
 * resource for low level WiFi driver etc.
 *
 * Usage example:
 *
 *      wifi_init_config_t init_config = WIFI_DEFAULT_INIT_CONFIG;
 *      BK_LOG_ON_ERR(bk_wifi_init(&init_config);
 *
 * @attention 1. This API is the 1st API that should be called before any other WiFi API can be called.
 * @attention 2. Make sure the reserved field in config is zero, otherwise you may encounter
 *               compatibility issue in future if more config fields are added.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_wifi_init(const wifi_init_config_t *init_config);

/**
 * @brief     Deinit the BK WiFi driver
 *
 * This API free all resources related to WiFi.
 *
 * @attention 1. This API is not ready yet, will support in future release.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NOT_SUPPORT: The API is not supported yet.
 *    - others: other errors.
 */
bk_err_t bk_wifi_deinit(void);

/**
 * @brief     Start the BK STA
 *
 * This API init the resoure specific to BK STA, e.g. init STA specific globals, init
 * the supplicant and STA specific WiFi driver etc.
 *
 * @attention 1. Make sure the bk_wifi_sta_set_config() are succeedful before calling this API.
 * @attention 2. This API connect the BK STA to the configured AP automatically.
 * @attention 3. If the BK STA is already started, this API ignores the request and returns BK_OK.
 *               if you want to restart the BK STA, call bk_wifi_sta_stop() first and then call
 *               this API.
 * @attention 4. TODO description about fast connection
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_WIFI_STA_NOT_CONFIG: the BK STA is not configured.
 *    - BK_ERR_WIFI_MONITOR_IP: the BK STA can't be started because monitor is started.
 *    - others: other errors.
 */
bk_err_t bk_wifi_sta_start(void);

/**
 * @brief     Stop the BK STA
 *
 * @attention This API causes the BK STA disconnect from the AP, it similar as
 *            bk_wifi_sta_disconnect(), but this API may free resource.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_FAIL: fail
 */
bk_err_t bk_wifi_sta_stop(void);

/**
 * @brief     Stop the pure scan operation
 *
 * @attention This API causes the BK STA disconnect from the AP, it similar as
 *            bk_wifi_sta_disconnect(), but this API may free resource.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_FAIL: fail
 */
bk_err_t bk_wifi_scan_stop(void);

/**
 * @brief     Config the BK STA
 *
 * This API configures the basic configurations of the BK STA, it should be called
 * before we call bk_wifi_sta_start() to start the BK STA. It can also be used to
 * update the configuration after the BK STA is started.
 *
 * Usage example:
 *
 *     wifi_sta_config_t sta_config = WIFI_DEFAULT_STA_CONFIG();
 *
 *     os_strncpy(sta_config.ssid, "ssid", WIFI_SSID_STR_LEN);
 *     os_strncpy(sta_config.password, "password");
 *     //more initialization here
 *     BK_LOG_ON_ERR(bk_wifi_sta_set_config(&sta_config));
 *
 * @attention 1. Don't call this API when the monitor is in-progress
 * @attention 2. If STA is already connected to AP, this API cases BK STA reconnects the AP.
 * @attention 3. Make sure the reserved fields in sta_config is zero, otherwise you may
 *               encounter compatibility issue in future if more config fields are added.
 *
 * @param sta_config the STA configuration
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_STA_NOT_STARTED: the STA is not started, call bk_wifi_sta_start() first.
 *    - BK_ERR_NULL_PARAM: parameter config is NULL.
 *    - BK_ERR_WIFI_RESERVED_FIELD: the reserved field of config is not 0.
 *    - others: other errors
 */
bk_err_t bk_wifi_sta_set_config(const wifi_sta_config_t *sta_config);

/**
 * @brief     Get the configuration of the BK STA.
 *
 * @param sta_config store the BK STA configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: the parameter config is NULL.
 *    - BK_ERR_WIFI_STA_NOT_CONFIG: STA is not configured yet.
 *    - others: other errors
 */
bk_err_t bk_wifi_sta_get_config(wifi_sta_config_t *sta_config);

/**
 * @brief     Get the WiFi station state of the BK STA.
 *
 * @attention the difference between this API and bk_wifi_sta_get_link_status() is
 *     - This API gets the actual WiFi STA link status and reason code
	- If sta is idle or connected, reason code will be  WIFI_REASON_MAX
 *
 * @param state store the WiFi station state of the BK STA
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: the parameter config is NULL.
 *    - others: other errors
 */
bk_err_t bk_wifi_sta_get_linkstate_with_reason(wifi_linkstate_reason_t *info);

/**
 * @brief     Get the WiFi link info of the BK STA.
 *
 * Get the actual WiFi link status of the BK STA.
 *
 * @attention the difference between this API and bk_wifi_sta_get_config() is
 *     - This API gets the actual STA link info while the later gets the configured value
 *     - This API can get more info of the link, such as RSSI, WiFi link status, AID etc.
 *     - The AID is only valid if @link_status->state is WIFI_LINK_CONNECTED.
 *
 * @param link_status store the WiFi link status of the BK STA
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: the parameter link_info is NULL.
 *    - BK_ERR_WIFI_DRIVER: WiFi low level driver has failure.
 *    - BK_ERR_WIFI_NO_MEM: Not enough memory
 *    - others: other errors
 */
bk_err_t bk_wifi_sta_get_link_status(wifi_link_status_t *link_status);

/**
 * @brief     Connect the BK STA to the AP.
 *
 * @attention 1. Make sure STA is started by bk_wifi_sta_start() before calling this API.
 * @attention 2. Don't call this API when the monitor is in-progress
 * @attention 3. If STA is already connected to AP, this API reconnects the BK STA.
 * @attention 4. TODO - multiple same SSID connect???
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_WIFI_STA_NOT_STARTED: the STA is not started, call bk_wifi_sta_start() first.
 *    - BK_ERR_WIFI_MONITOR_IP: the API is not allowed because monitor is in-progress.
 *    - others: other failures.
 */
bk_err_t bk_wifi_sta_connect(void);

/**
 * @brief     Disconnect the WiFi connection of the BK STA
 *
 * @attention TODO - add description about disconnect event!
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other failures.
 */
bk_err_t bk_wifi_sta_disconnect(void);

/**
 * @brief     Start a scan
 *
 * This API notifies the WiFi driver to start a scan, the event EVENT_WIFI_SCAN_DONE will
 * be raised if the scan is completed. General steps to use this API:
 *  - prepare the scan done event callback, the callback can call bk_wifi_scan_get_result()
 *    to get the scan result and then call bk_wifi_scan_free_result() to free the resource.
 *  - call bk_event_register_cb(EVENT_MOD_WIFI, EVENT_WIFI_SCAN_DONE, ...) to register
 *    scan done event callback.
 *  - call this API to trigger this the scan.
 *
 * Usage example:
 *
 *     //Define your scan done handler
 *     int scan_done_handler(void *arg, event_module_t event_module, int event_id, void *event_data)
 *     {
 *         wifi_scan_result_t scan_result = {0};
 *
 *         BK_LOG_ON_ERR(bk_wifi_scan_get_result(&scan_result));
 *         bk_wifi_scan_dump_result(&scan_result);
 *         bk_wifi_scan_free_result(&scan_result);
 *
 *         return BK_OK;
 *     }
 *
 *     //Start the scan
 *     wifi_scan_config_t config = {0};
 *
 *     BK_LOG_ON_ERR(bk_event_register_cb(EVENT_MOD_WIFI, EVENT_WIFI_SCAN_DONE, scan_done_handler, NULL));
 *     BK_LOG_ON_ERR(bk_wifi_scan_start(&scan_config));
 *
 * @attention 1. This API triggers an active scan on all channels (TODO double check)
 * @attention 2. Pass NULL scan_config to scan all APs, otherwise scan the SSID specified in scan_config.ssid
 * @attention 3. Make sure the reserved fields in scan_config is zero, otherwise you may
 *               encounter compatibility issue in future if more config fields are added.
 * @attention 4. TODO scan result limits???
 *
 * @param scan_config the configuration of scan
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_WIFI_STA_NOT_STARTED: the STA is not started, call bk_wifi_sta_start() first.
 *    - BK_ERR_WIFI_MONITOR_IP: the API is not allowed because monitor is in-progress.
 *    - others: other failures.
 */
bk_err_t bk_wifi_scan_start(const wifi_scan_config_t *scan_config);

/**
 * @brief     Get the scan result
 *
 * Usage example:
 *
 *     wifi_scan_result_t scan_result = {0};
 *
 *     BK_LOG_ON_ERR(bk_wifi_scan_get_result(&scan_result));
 *     bk_wifi_scan_free_result(&scan_result);
 *
 * @attention 1. The caller don't need to allocate memory for scan_result->aps, this API will
 *               allocate the memory for scan_result->aps according to the actual found AP number.
 * @attention 2. Don't forget to call bk_wifi_scan_free_result(&scan_result) to free the
 *            the resource allocated in this API, otherwise memory leak.
 *
 * @param scan_result store the scan result
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: NULL scan_result
 *    - BK_ERR_NO_MEM: out of memory
 *    - others: other failures
 */
bk_err_t bk_wifi_scan_get_result(wifi_scan_result_t *scan_result);

/**
 * @brief     Dump the scan result
 *
 * @param scan_result The scan result to be dumped.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_PARAM: invalid scan result
 */
bk_err_t bk_wifi_scan_dump_result(const wifi_scan_result_t *scan_result);

/**
 * @brief     Free the scan result
 *
 * @attention This API free scan_result->aps which allocated in bk_wifi_scan_get_result(),
 *            it doesn't free scan_result. So if scan_result is allocated by the caller,
 *            the caller need to free it.
 *
 * @param scan_result The scan result to be freed.
 *
 * @return
 *    - BK_OK: always succeed
 */
void bk_wifi_scan_free_result(wifi_scan_result_t *scan_result);


/**
 * @brief     Add/Update/Del STA's Vendor Specific IE **before** connect to AP.
 *
 * @attention If you want to add vsie when sta starts, just initialize wifi_sta_config_t
 *            when call bk_wifi_sta_set_config().
 *
 * @param     buf   vsie buf
 * @param     len   vsie buf len
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_sta_add_vendor_ie(uint8_t frame, uint8_t *vsie, uint8_t len);
bk_err_t bk_wifi_sta_update_vendor_ie(uint8_t frame, uint8_t *vsie, uint8_t len);
bk_err_t bk_wifi_sta_del_vendor_ie(uint8_t frame);

/**
 * @brief     Start the BK AP
 *
 * This API init the resoure specific to BK AP, e.g. init BK AP specific globals, init
 * the hostapd and AP specific WiFi driver etc.
 *
 * If the BK AP is already started, this API ignores the request and returns BK_OK, we can
 * call bk_wifi_ap_stop() to stop BK AP first and then call bk_wifi_ap_start() to restart it.
 *
 * **Restart** AP Usage example:
 *
 *     BK_LOG_ON_ERR(bk_wifi_ap_stop());
 *     BK_LOG_ON_ERR(bk_wifi_ap_start());
 *
 * @attention 1. Don't call this API when the monitor is in-progress
 * @attention 2. If bk_wifi_ap_set_config() is not called, this API start the AP with
 *               default value, normally you should configure the AP first before calling
 *               this API.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_WIFI_AP_NOT_CONFIGURED: the WiFi AP is not configured, call bk_wifi_ap_set_config() first.
 *    - others: other errors
 */
bk_err_t bk_wifi_ap_start(void);

/**
 * @brief     Stop the BK AP
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_WIFI_NOT_INIT: the WiFi is not initialized, call bk_wifi_init() first.
 *    - others: other errors
 */
bk_err_t bk_wifi_ap_stop(void);

/**
 * @brief     Config the BK AP
 *
 * Usage example:
 *
 *     wifi_ap_config_t ap_config = WIFI_DEFAULT_AP_CONFIG();
 *
 *     os_strncpy(ap_config.ssid, "ssid", WIFI_SSID_STR_LEN);
 *     os_strncpy(ap_config.password, "password", WIFI_PASSWORD_LEN);
 *     BK_LOG_ON_ERR(bk_wifi_ap_set_config(&ap_config));
 *
 * @attention 1. Make sure the reserved field in config is zero, otherwise you may
 *               encounter compatibility issue in future if more config fields are added.
 *
 * @param ap_config the AP configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: the ap_config is NULL.
 *    - others: other errors
 */
bk_err_t bk_wifi_ap_set_config(const wifi_ap_config_t *ap_config);

/**
 * @brief     Get the configuration of the BK AP.
 *
 * @param     ap_config store the configuration of BK AP
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NULL_PARAM: the parameter config is NULL.
 *    - others: other errors
 */
bk_err_t bk_wifi_ap_get_config(wifi_ap_config_t *ap_config);


/**
 * @brief     Get the STAs connected to BK AP.
 *
 * @attention Free stas->sta after usage.
 *
 * @param     stas store the STA list in BK AP
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_ap_get_sta_list(wlan_ap_stas_t *stas);

/**
 * @brief     Add softap's Vendor Specific IE when softap is running.
 *
 * @attention If you want to add vsie when softap starts, just initialize wifi_ap_config_t
 *            when call bk_wifi_ap_start().
 *
 * @param     buf   vsie buf
 * @param     len   vsie buf len
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_ap_add_vendor_ie(void *buf, uint8_t len);

/**
 * @brief     Delete softap's Vendor Specific IE when softap is running.
 *
 * @attention If you want to add vsie when softap starts, just initialize wifi_ap_config_t
 *            when call bk_wifi_ap_start().
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_ap_del_vendor_ie(void);

/**
 * @brief     Start the monitor
 *
 * @attention 1. When monitor mode is enabled, the scan will not work and
 *               bk_wifi_scan_start() returns BK_ERR_WIFI_MONITOR_IP.
 * @attention 2. When monitor mode is enabled, the STA connect will not work
 *               and bk_wifi_sta_connect() returns BK_ERR_WIFI_MONITOR_IP.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_monitor_start(void);


/**
 * @brief     Stop the monitor
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_monitor_stop(void);

/**
 * @brief     Configure the monitor
 *
 * @param     monitor_config the configuration of monitor
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_monitor_set_config(const wifi_monitor_config_t *monitor_config);

/**
 * @brief     Get the configuration of the monitor
 *
 * @param monitor_config store the configuration of monitor
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_monitor_get_config(wifi_monitor_config_t *monitor_config);

/**
 * @brief     Register the user callback of monitor
 *
 * @param monitor_cb monitor callback
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_monitor_register_cb(const wifi_monitor_cb_t monitor_cb);

/**
 * @brief     Set monitor channel
 *
 * @attention 1. Currently we only support HT20, chan->second should always be 0.
 * @attention 2. This API is only used for settting monitor channel, NOT for other purpose.
 * @attention 3. This API directly change the hardware channel, it may impacts BK STA/AP if
 *               the STA or AP interface is started. TODO describe details of the impact.
 * @attention 4. The channel range can be set is from channel 1 to channel 14,
 *
 * @param chan channel of monitor
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_monitor_set_channel(const wifi_channel_t *chan);

/**
 * @brief     twt set up
 * @param     setup_type   suggest/demand
 * @param     mantissa   wake_int_mantissa
 * @param     min_twt   min_twt_wake_dur
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_twt_setup(uint8_t setup_type, uint16_t mantissa, uint8_t min_twt);

/**
 * @brief    twt tear down
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_twt_teardown(void);

/**
 * @brief     Set the filter configuration
 *
 * The filter is used to configure what kind of management packets can be received
 * by the filter callback registered by bk_wifi_filter_register_cb().
 *
 * Per the WiFi receiving data path, the management frames are firstly received by
 * WiFi hardware, then the hardware relays them to WiFi software (WiFi driver). Normally
 * the hardware receives all management frames in the air, however it don't
 * relay all of them to WiFi driver (WiFi software), it only relays management frames
 * necessary to STA or AP or scan or monitor, depends on which interface or function are
 * enabled. In other word, WiFi driver only receives management frames necessary to them
 * from hardware.
 *
 * When no filter callback is registered, all management frame processing is ended
 * in WiFi driver unless the filter callback is registered.
 *
 * If filter callback is register and if filter_config.rx_all_default_mgmt is set to 1,
 * then all the management frames received by WiFi driver are relayed to the filter
 * callback. **Notified that the default management frame types can be received depends
 * on whether STA or AP or scan or monitor are enabled or not, e.g. when only STA is
 * enabled, the WiFi driver needs to handle PROBE response, but don't need to handle PROBE
 * request, so the WiFi driver only receive PROBE response from WiFi hardware**.
 *
 * Here is summary about default management frame received by WiFi:
 *  Condition      | Default management frame
 * ----------------|-------------------------------------------------------------------
 *  STA connected  | Management frames sent by the connected AP, such as beacon etc.
 *  AP started     | Management frames sent by connected STA and probe request etc.
 *  Scan starting  | Probe response and beacon frames of all APs.
 *  Monitor started| All kind of management frames
 *
 * How about if the filter callback wants to receive more management frame types,
 * e.g. want to receive PROBE request when only STA is enabled. We can set
 * filter_config->rx_probe_req to receive PROBE response no matter whether AP is enabled
 * or NOT.
 *
 * Usage example:
 *
 *     // Only receive management frame default received by WiFi driver
 *     // Equal to the case when bk_wifi_filter_set_config() is not called
 *     wifi_filter_config_t filter_config = WIFI_DEFAULT_FILTER_CONFIG();
 *     BK_LOG_ON_ERR(bk_wifi_filter_set_config(&filter_config);
 *
 *     // Receive all beacon and management frames default received by WiFi driver
 *     filter_config.rx_all_beacon = 1;
 *     BK_LOG_ON_ERR(bk_wifi_filter_set_config(&filter_config);
 *
 *     // Only receive all beacon
 *     filter_config.rx_all_beacon = 1;
 *     filter_config.rx_all_default_mgmt = 0;
 *     BK_LOG_ON_ERR(bk_wifi_filter_set_config(&filter_config);
 *
 * @attention 1. If this API is not called, the default filter is to relay all
 *               mamangement frames received by WiFi driver to the filter callback.
 * @attention 2. If STA is enabled, the WiFi driver only receives beacon from the
 *               connected AP, if scan is enabled or filter_config->rx_all_beacon
 *               is set, the WiFi driver receives beacons of all APs.
 *
 * @param filter_config configuration of filter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_filter_set_config(const wifi_filter_config_t *filter_config);

/**
 * @brief     Get the filter configuration
 *
 * @param filter_config store configuration of filter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_filter_get_config(wifi_filter_config_t *filter_config);

/**
 * @brief     Register the filter user callback
 *
 * @attention 1. If the monitor is enabled, the management packets received by filter
 *               callback can also be received by monitor callback. If this is not what
 *               we want, we can disable filter when enabling monitor, or disable monitor
 *               when enabling filter.
 * @attention 2. During WiFi scan, the filter also works.
 * @attention 3. Pass NULL callback or set all fields of filter_config to 0 to disable the
 *               filter.
 * @attention 4. The memory used to hold the frame buffer and frame_info will be freed
 *               immediately when the callback is returned, so don't relay them to other
 *               task. Make a copy of them and then relay to other task if necessary.
 * @attention 5. Don't do too much work in the filter callback because it's called in
 *               WiFi driver core thread, otherwise it may impact the WiFi performance.
 *
 * @param filter_cb the filter user callback
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_filter_register_cb(const wifi_filter_cb_t filter_cb);

/**
 * @brief     Get the MAC of BK STA
 *
 * @attention 1. The AP's MAC is derived from the base MAC of the system.
 * @attention 2. If you want to change the MAC of AP, call bk_set_mac() to set
 *               the base MAC of the system.
 *
 * @param mac store the MAC of BK STA
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */

bk_err_t bk_wifi_sta_get_mac(uint8_t *mac);

/**
 * @brief     Get the MAC of BK AP
 *
 * @attention 1. The AP's MAC is derived from the base MAC of the system.
 * @attention 2. If you want to change the MAC of AP, call bk_set_mac() to set
 *               the base MAC of the system.
 *
 * @param mac store the MAC of BK AP
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_wifi_ap_get_mac(uint8_t *mac);

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
 *    - kNoErr: succeed
 *    - otherwise: fail
 */
bk_err_t bk_wifi_send_raw(uint8_t *buffer, int len);

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
 *    - BK_OK: succeed
 *    - BK_ERR_WIFI_NOT_INIT: WiFi is not initialized
 *    - BK_ERR_PARAM: invalid argument
 */
bk_err_t bk_wifi_set_country(const wifi_country_t *country);

/**
  * @brief     get the current country info
  *
  * @param     country  country info
  *
  * @return
  *    - BK_OK: succeed
  *    - BK_ERR_WIFI_NOT_INIT: WiFi is not initialized
  *    - BK_ERR_PARAM: invalid argument
  */
bk_err_t bk_wifi_get_country(wifi_country_t *country);

/**
 * @brief  enable wifi sta power management
 *
 * @return
 *      - BK_OK: on succeed
 *      - others: real error, used for future.
 */
bk_err_t bk_wifi_sta_pm_enable(void);

/**
 * @brief  disable wifi sta power management
 *
 * @return
 *      - BK_OK: on succeed
 *      - others: real error, used for future.
 */
bk_err_t bk_wifi_sta_pm_disable(void);

/**
 * @brief  get current channel ID
 *         
 * @param  basically range from 1~13
 *
 * @return
 *      - BK_OK: on succeed
 *      - others: real error, used for future.
 */
bk_err_t bk_wifi_get_channel(void);

/**
 * @brief  Set Wi-Fi MAC addresses.
 *
 * The given MAC address will become the station MAC address.The AP MAC address
 * (i.e BSSID) will be the same MAC address but with the local bit set.
 *
 * @param mac system mac

 * @usage example:
 *		mac c8478cxxyyzz
 *		txevm  -e 2  //if needed
 *
 * @return

 *		- BK_OK: on succeed
 *      - others: real error, used for future.
 *	
 * @note on some platforms the change of MAC address can only take effect after reboot.
 */
bk_err_t bk_wifi_set_mac_address(char *mac);

/**
 * @brief  Get Wi-Fi Calibration status.
 *
 * @return
 *      - BK_OK: on succeed
 *      - others: real error, used for future.
 */
bk_err_t bk_wifi_manual_cal_rfcali_status(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

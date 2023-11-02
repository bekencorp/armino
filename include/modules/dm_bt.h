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

#ifndef INCLUDE_MODULES_BK_DM_BT_API_H_
#define INCLUDE_MODULES_BK_DM_BT_API_H_


#include <modules/dm_bt_types.h>

#ifdef __cplusplus
extern"C" {
#endif

/**
 * @brief           sets BT event callback function
 *
 * @param[in]       func : callback
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS : Succeed
 *                  - BK_ERR_BT_FAIL: others
 */
bt_err_t bk_bt_gap_set_event_callback(bt_event_cb_t func);



/**
 * @brief           send linkkey when peer need authentication
 *
 * @param[in]       is_exist : if linkkey exist. If no exist, peer may trigger pair procedure
 * @param[in]       key : linkkey,
 *
 * @attention       key->addr must be valued
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS : Succeed
 *                  - BK_ERR_BT_FAIL: others
 */
bt_err_t bk_bt_gap_linkkey_reply(uint8_t is_exist, bk_bt_linkkey_storage_t *key);


/**
 * @brief           sets the Bluetooth connectable, discoverable
 *
 * @param[in]       conn_mode : one of the enums of bk_bt_conn_mode_t
 *
 * @param[in]       disc_mode : one of the enums of bk_bt_disc_mode_t
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS : Succeed
 *                  - BK_ERR_BT_FAIL: others
 */
bt_err_t bk_bt_gap_set_visibility(bk_bt_conn_mode_t conn_mode, bk_bt_disc_mode_t disc_mode);


/**
 * @brief set the class of device
 *
 * @param[in]       cod: Class of Device for the device(@see enum common_cod_t)
 *
 * @return
 *                 - BK_ERR_BT_SUCCESS: succeed
 *                 - others: fail
 */
bt_err_t bk_bt_gap_set_device_class(uint32_t cod);

/**
 * @brief set the local name
 *
 * @param[in]       name: pointer of name
 * @param[in]       len: the length of name
 *
 * @return
 *                 - BK_ERR_BT_SUCCESS: succeed
 *                 - others: fail
 */
bt_err_t bk_bt_gap_set_local_name(uint8_t *name, uint8_t len);

/**
 * @brief           This function starts Inquiry and Name Discovery. This function should be called after esp_bluedroid_enable() completes successfully.
 *                  When Inquiry is halted and cached results do not contain device name, then Name Discovery will connect to the peer target to get the device name.
 *                  bk_bt_gap_cb_t will be called with BK_DM_BT_GAP_DISC_STATE_CHANGED_EVT when Inquiry is started or Name Discovery is completed.
 *                  bk_bt_gap_cb_t will be called with BK_DM_BT_GAP_DISC_RES_EVT each time the two types of discovery results are got.
 *
 * @param[in]       mode - Inquiry mode
 *
 * @param[in]       inq_len - Inquiry duration in 1.28 sec units, ranging from 0x01 to 0x30. This parameter only specifies the total duration of the Inquiry process,
 *                          - when this time expires, Inquiry will be halted.
 *
 * @param[in]       num_rsps - Number of responses that can be received before the Inquiry is halted, value 0 indicates an unlimited number of responses.
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS : Succeed
 *                  - others: fail
 */
bk_err_t bk_bt_gap_start_discovery(bk_bt_inq_mode_t mode, uint8_t inq_len, uint8_t num_rsps);



/**
 * @brief           Cancel Inquiry and Name Discovery. This function should be called after esp_bluedroid_enable() completes successfully.
 *                  bt_event_cb_t will be called with BK_DM_BT_GAP_DISC_STATE_CHANGED_EVT if Inquiry or Name Discovery is cancelled by
 *                  calling this function.
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS : Succeed
 *                  - others: fail
 */
bt_err_t bk_bt_gap_cancel_discovery(void);


/**
 *
 * @brief     Initialize the bluetooth A2DP sink module.
 *
 * @param[in]   aac_supported: 1 means aac is supported, 0 means aac is not supported.
 *
 * @attention   sink/source must init only one.
 *
 * @return
 *            - BK_ERR_BT_SUCCESS: the initialization request is successfully
 *            -  others: fail
 */
bt_err_t bk_bt_a2dp_sink_init(uint8_t aac_supported);


/**
 * @brief           Register application callback function to A2DP module. used by both A2DP source
 *                  and sink.
 *
 * @param[in]       callback: A2DP event callback function
 *
 * @attention       this function must call after sink or source init
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  -  others: fail
 */
bt_err_t bk_bt_a2dp_register_callback(bk_bt_a2dp_cb_t callback);

/**
 * @brief           Register A2DP sink data output function;
 *
 * @param[in]       callback: A2DP sink data callback function
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  -  others: fail
 */
bt_err_t bk_bt_a2dp_sink_register_data_callback(bk_bt_sink_data_cb_t callback);

/**
 *
 * @brief           Connect to remote bluetooth A2DP source device. This API must be called after
 *                  bk_bt_a2dp_sink_init().
 *
 * @param[in]       remote_bda: remote bluetooth device address
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  -  others: fail
 *
 */
bt_err_t bk_bt_a2dp_sink_connect(uint8_t *remote_bda);

/**
 *
 * @brief           Disconnect from the remote A2DP source device. This API must be called after
 *                  bk_bt_a2dp_sink_init().
 *
 * @param[in]       remote_bda: remote bluetooth device address
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  -  others: fail
 *
 */
bt_err_t bk_bt_a2dp_sink_disconnect(uint8_t *remote_bda);

/**
 *
 * @brief           Initialize the bluetooth AVRCP controller module
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 *
 */
bt_err_t bk_bt_avrcp_ct_init(void);



/**
 * @brief           Register application callbacks to AVRCP module.
 *
 * @param[in]       callback: AVRCP controller callback function
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 *
 */
bt_err_t bk_bt_avrcp_ct_register_callback(bk_avrcp_ct_cb_t callback);

/**
 * @brief           Send GetCapabilities PDU to AVRCP target to retrieve remote device's supported
 *                  notification event_ids. This function should be called after
 *                  BK_AVRCP_CT_CONNECTION_STATE_EVT is received and AVRCP connection is established.
 *
 * @param[in]       remote_bda: remote bluetooth device address
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 */
bt_err_t bk_bt_avrcp_ct_send_get_rn_capabilities_cmd(uint8_t *remote_bda);

/**
 * @brief           Send register notification command to AVRCP target. This function should be called after
 *                  BK_AVRCP_CT_CONNECTION_STATE_EVT is received and AVRCP connection is established.
 *
 * @param[in]       remote_bda: remote bluetooth device address
 *
 * @param[in]       event_id : id of events, e.g. BK_AVRCP_RN_PLAY_STATUS_CHANGE, BK_AVRCP_RN_TRACK_CHANGE, etc.
 *
 * @param[in]       event_parameter : playback interval for BK_AVRCP_RN_PLAY_POS_CHANGED;
 *                                    For other events , value of this parameter is ignored.
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 */
bt_err_t bk_bt_ct_send_register_notification_cmd(uint8_t *remote_bda, uint8_t event_id, uint32_t event_parameter);

/**
 * @brief           Send passthrough command to AVRCP target. This function should be called after
 *                  BK_AVRCP_CT_CONNECTION_STATE_EVT is received and AVRCP connection is established.
 *
 * @param[in]       remote_bda: remote bluetooth device address.
 *
 * @param[in]       key_code : passthrough command code, e.g. BK_AVRCP_PT_CMD_PLAY, BK_AVRCP_PT_CMD_STOP, etc.
 *
 * @param[in]       key_state : passthrough command key state, BK_AVRCP_PT_CMD_STATE_PRESSED or
 *                  BK_AVRCP_PT_CMD_STATE_RELEASED
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 */
bt_err_t bk_bt_avrcp_ct_send_passthrough_cmd(uint8_t *remote_bda, uint8_t key_code, uint8_t key_state);



/**
 *
 * @brief           Initialize the bluetooth AVRCP target module
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 *
 */
bt_err_t bk_bt_avrcp_tg_init(void);


/**
 * @brief           Register application callbacks to AVRCP target module.
 *
 * @param[in]       callback: AVRCP target callback function
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 *
 */
bt_err_t bk_bt_avrcp_tg_register_callback(bk_avrcp_tg_cb_t callback);



/**
 *
 * @brief           Get the requested event notification capabilies on local AVRC target. The capability is returned
 *                  in a bit mask representation in evt_set. This function should be called after bk_avrcp_tg_init().
 *
 *                  For capability type "BK_AVRCP_RN_CAP_API_METHOD_ALLOWED, the retrieved event set is constant and
 *                  it covers all of the notification events that can possibly be supported with current
 *                  implementation.
 *
 *                  For capability type BK_AVRCP_RN_CAP_API_METHOD_CURRENT_ENABLE, the event set covers the notification
 *                  events selected to be supported under current configuration, The configuration can be
 *                  changed using bk_avrcp_tg_set_rn_evt_cap().
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 */
bt_err_t bk_bt_avrcp_tg_get_rn_evt_cap(bk_avrcp_rn_cap_api_method_t cap, bk_avrcp_rn_evt_cap_mask_t *evt_set);

/**
 *
 * @brief           Set the event notification capabilities on local AVRCP target. The capability is given in a
 *                  bit mask representation in evt_set and must be a subset of allowed event IDs with current
 *                  implementation. This function should be called after bk_avrcp_tg_init().
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 *
 */
bt_err_t bk_bt_avrcp_tg_set_rn_evt_cap(const bk_avrcp_rn_evt_cap_mask_t *evt_set);

/**
 *
 * @brief           Send RegisterNotification Response to remote AVRCP controller. Local event notification
 *                  capability can be set using bk_avrcp_tg_set_rn_evt_cap(), in a bit mask representation
 *                  in evt_set. This function should be called after bk_avrcp_tg_init().
 *
 * @param[in]       addr: peer addr
 *
 * @param[in]       event_id: notification event ID that remote AVRCP CT registers
 *
 * @param[in]       rsp: notification response code
 *
 * @param[in]       param: parameters included in the specific notification
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 *
 */
bt_err_t bk_bt_avrcp_tg_send_rn_rsp(uint8_t *addr, bk_avrcp_rn_event_ids_t event_id, bk_avrcp_rn_rsp_t rsp,
                                    bk_avrcp_rn_param_t *param);

/**
 *
 * @brief           Initialize the bluetooth A2DP source module. A2DP can work independently.
 *
 * @attention       sink/source must init only one.
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: if the initialization request is sent to lower layer successfully
 *                  - others: fail
 *
 */
bt_err_t bk_bt_a2dp_source_init(void);

/**
 * @brief           Register A2DP source data input function. For now, the input should be PCM data stream.

 *
 * @param[in]       callback: A2DP source data callback function
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 *
 */
bk_err_t bk_a2dp_source_register_data_callback(bk_a2dp_source_data_cb_t callback);

/**
 * @brief           set a2dp source data format
 *
 * @param[in]       sample_rate: pcm data sample rate, such as 8000 16000 32000 44100 48000.
 *
 * @param[in]       bit_depth : pcm data bit depth, now support 16 bits only.
 *
 * @param[in]       channel_count : pcm data channel count, support 1 or 2 now.
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: success
 *                  - others: fail
 */
bk_err_t bk_a2dp_source_set_pcm_data_format(uint32_t sample_rate, uint8_t bit_depth, uint8_t channel_count);


/**
 *
 * @brief           Connect to remote A2DP sink device. This API must be called
 *                  after bk_bt_a2dp_source_init()
 *
 * @param[in]       addr: remote bluetooth device address
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: connect request is sent to lower layer successfully
 *                  - others: fail
 *
 */
bt_err_t bk_bt_a2dp_source_connect(uint8_t *addr);

/**
 *
 * @brief           Disconnect from the remote A2DP sink device. This API must be called
 *                  after bk_bt_a2dp_source_init()
 *
 * @param[in]       addr: remote bluetooth device address
 * @return
 *
 *                  - BK_ERR_BT_SUCCESS: connect request is sent to lower layer successfully
 *                  - others: fail
 *
 */
bt_err_t bk_bt_a2dp_source_disconnect(uint8_t *addr);

/**
 *
 * @brief           Media control commands. This API can be used for both A2DP sink
 *                  and must be called after bk_bt_a2dp_source_init()
 *
 * @param[in]       ctrl: control commands for A2DP action
 *
 * @attention       this function only used for a2dp source
 *
 * @return
 *                  - BK_ERR_BT_SUCCESS: control command is sent to lower layer successfully
 *                  - others: fail
 *
 */
bt_err_t bk_a2dp_media_ctrl(bk_a2dp_media_ctrl_t ctrl);

#ifdef __cplusplus
}
#endif

#endif

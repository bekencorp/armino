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
 *
 * @brief     Initialize the bluetooth A2DP sink module.
 *
 * @param[in]   aac_supported: 1 means aac is supported, 0 means aac is not supported.
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

#ifdef __cplusplus
}
#endif

#endif

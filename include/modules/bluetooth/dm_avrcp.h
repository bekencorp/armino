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

#include "modules/bluetooth/dm_bt_types.h"
#include "modules/bluetooth/dm_avrcp_types.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup dm_avrcp AVRCP API
 * @{
 */

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

///@}

#ifdef __cplusplus
}
#endif



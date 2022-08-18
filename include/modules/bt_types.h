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

#ifndef INCLUDE_MODULES_BK_BT_TYPES_H_
#define INCLUDE_MODULES_BK_BT_TYPES_H_

#include <common/bk_typedef.h>

/// BD address length
#define BK_BT_ADDR_LEN       (6)

typedef enum
{
    BK_BT_CONTROLLER_STACK_TYPE_BTDM_5_2,
    BK_BT_CONTROLLER_STACK_NOT_SUPPORT,
}BK_BT_CONTROLLER_STACK_TYPE;

typedef enum
{
    BK_BT_HOST_STACK_TYPE_ETHERMIND,
    BK_BT_HOST_STACK_TYPE_NOT_SUPPORT,
}BK_BT_HOST_STACK_TYPE;


/**
 * @brief bt api return enum
 */
typedef bk_err_t bt_err_t;

#define BK_ERR_BT_SUCCESS               BK_OK                      /**< success */
#define BK_ERR_BT_FAIL                  BK_FAIL                    /**< fail        */
#define BK_ERR_BT_NO_MEM                BK_ERR_NO_MEM              /**<  no mem       */
#define BK_ERR_BT_PROFILE               (BK_ERR_BLE_BASE - 40)      /**<  profile err       */
#define BK_ERR_BT_CREATE_DB             (BK_ERR_BLE_BASE - 41)      /**< bk_ble_create_db err        */
#define BK_ERR_BT_CMD_NOT_SUPPORT       (BK_ERR_BLE_BASE - 42)      /**< unknow cmd err        */
#define BK_ERR_BT_UNKNOW_IDX            (BK_ERR_BLE_BASE - 43)      /**< index err, suchas conn_ind        */
#define BK_ERR_BT_BT_STATUS             (BK_ERR_BLE_BASE - 44)      /**<  ble status not match       */
#define BK_ERR_BT_ADV_DATA              (BK_ERR_BLE_BASE - 45)      /**<  adv data err, such as too long       */
#define BK_ERR_BT_CMD_RUN               (BK_ERR_BLE_BASE - 46)      /**< cmd run err        */
#define BK_ERR_BT_INIT_CREATE           (BK_ERR_BLE_BASE - 47)      /**< create init err, such as bk_ble_create_init        */
#define BK_ERR_BT_INIT_STATE            (BK_ERR_BLE_BASE - 48)      /**<  current init status not match       */


/*
 * @brief for bt api async call result in bt_at_cmd_cb's cmd
 */
typedef enum
{
    BT_CMD_INQUIRY,
    BT_CMD_CREATE_CONNECT,
    BT_CMD_DISCONNECT,
    BT_CMD_SDP,
    BT_CMD_MAX,
} bt_cmd_t;

typedef enum{
    BT_EVENT_INQUIRY_RESULT,
    BT_EVENT_DISCONNECT,
}bt_event_t;
/**
 * @defgroup bk_bt_api_v1_typedef struct
 * @brief bt struct type
 * @ingroup bk_bt_api_v1_typedef struct
 * @{
 */
typedef struct
{
    uint8_t cmd_idx;      /**< actv_idx */
    bt_err_t status;     /**< The status for this command */
    void *param;
} bt_cmd_param_t;


/**
 * @brief for sync bt api call return
 *
 * most bt api have bt_cmd_cb_t param, you must wait is callback.
 *
 * @param
 * - cmd: cmd id.
 * - param: param
 *
**/
typedef void (*bt_cmd_cb_t)(bt_cmd_t cmd, bt_cmd_param_t *param);
typedef void (*bt_event_cb_t)(bt_event_t event, void *param);

#endif
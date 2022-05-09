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
#include "modules/ble.h"

// no used
#define BK_BLE_MAX_SCAN_NUM               (15)
#define BK_BLE_ABIT(n) (1 << n)

typedef enum
/// Stop notification/indication
{
    PRF_STOP_NTFIND = 0x0000,
    /// Start notification
    PRF_START_NTF,
    /// Start indication
    PRF_START_IND,
} prf_conf;



#if 0//old ext api 2 private api

#define ERR_SUCCESS              BK_ERR_BLE_SUCCESS
#define ERR_FAIL                 BK_ERR_BLE_FAIL
#define ERR_NO_MEM               BK_ERR_BLE_NO_MEM
#define ERR_PROFILE              BK_ERR_BLE_PROFILE
#define ERR_CREATE_DB            BK_ERR_BLE_CREATE_DB
#define ERR_CMD_NOT_SUPPORT      BK_ERR_BLE_CMD_NOT_SUPPORT
#define ERR_UNKNOW_IDX           BK_ERR_BLE_UNKNOW_IDX
#define ERR_BLE_STATUS           BK_ERR_BLE_BLE_STATUS
#define ERR_ADV_DATA             BK_ERR_BLE_ADV_DATA
#define ERR_CMD_RUN              BK_ERR_BLE_CMD_RUN
#define ERR_INIT_CREATE          BK_ERR_BLE_INIT_CREATE
#define ERR_INIT_STATE           BK_ERR_BLE_INIT_STATE
#define ERR_ATTC_WRITE           BK_ERR_BLE_ATTC_WRITE
#define ERR_ATTC_WRITE_UNREGISTER                BK_ERR_BLE_ATTC_WRITE_UNREGISTER

#define MAX_ADV_DATA_LEN          BK_BLE_MAX_ADV_DATA_LEN
#define MAX_SCAN_NUM              BK_BLE_MAX_SCAN_NUM
#define GAP_BD_ADDR_LEN       BK_BLE_GAP_BD_ADDR_LEN
#define APP_DEVICE_NAME_MAX_LEN      BK_BLE_APP_DEVICE_NAME_MAX_LEN
#define ABIT BK_BLE_ABIT
#define BK_PERM_SET BK_BLE_PERM_SET
#define BK_PERM_GET BK_BLE_PERM_GET

typedef ble_att_info_req_t att_info_req_t;
typedef ble_write_req_t write_req_t;
typedef ble_read_req_t read_req_t;
typedef ble_recv_adv_t recv_adv_t;
typedef ble_mtu_change_t mtu_change_t;
typedef ble_conn_ind_t conn_ind_t;
typedef ble_discon_ind_t discon_ind_t;
typedef ble_create_db_t create_db_t;
typedef ble_attm_desc_t bk_attm_desc_t;
typedef ble_init_config_t init_config_t;
typedef ble_read_phy_t le_read_phy_t;
typedef ble_set_phy_t le_set_phy_t;
typedef ble_adv_param_t le_adv_param_t;
typedef ble_scan_param_t le_scan_param_t;
typedef ble_conn_param_t le_conn_param_t;


enum
{
    RP_MASK =                                    BK_BLE_RP_MASK,
    RP_POS =                                     BK_BLE_RP_POS,
    WP_MASK =                                    BK_BLE_WP_MASK,
    WP_POS =                                     BK_BLE_WP_POS,
    IP_MASK =                                    BK_BLE_IP_MASK,
    IP_POS =                                     BK_BLE_IP_POS,
    NP_MASK =                                    BK_BLE_NP_MASK,
    NP_POS =                                     BK_BLE_NP_POS,
    BROADCAST_MASK =                             BK_BLE_BROADCAST_MASK,
    BROADCAST_POS =                              BK_BLE_BROADCAST_POS,
    RD_MASK =                                    BK_BLE_RD_MASK,
    RD_POS =                                     BK_BLE_RD_POS,
    WRITE_COMMAND_MASK =                         BK_BLE_WRITE_COMMAND_MASK,
    WRITE_COMMAND_POS =                          BK_BLE_WRITE_COMMAND_POS,
    WRITE_REQ_MASK =                             BK_BLE_WRITE_REQ_MASK,
    WRITE_REQ_POS =                              BK_BLE_WRITE_REQ_POS,
    NTF_MASK =                                   BK_BLE_NTF_MASK,
    NTF_POS =                                    BK_BLE_NTF_POS,
    IND_MASK =                                   BK_BLE_IND_MASK,
    IND_POS =                                    BK_BLE_IND_POS,
    WRITE_SIGNED_MASK =                          BK_BLE_WRITE_SIGNED_MASK,
    WRITE_SIGNED_POS =                           BK_BLE_WRITE_SIGNED_POS,
    EXT_MASK =                                   BK_BLE_EXT_MASK,
    EXT_POS =                                    BK_BLE_EXT_POS,
};


enum
{
    EKS_MASK         = BK_BLE_EKS_MASK,
    EKS_POS          = BK_BLE_EKS_POS,
    UUID_LEN_MASK    = BK_BLE_UUID_LEN_MASK,
    UUID_LEN_POS     = BK_BLE_UUID_LEN_POS,
    RI_MASK          = BK_BLE_RI_MASK,
    RI_POS           = BK_BLE_RI_POS,
};


enum
{
    SVC_MI_MASK        = BK_BLE_SVC_MI_MASK,
    SVC_MI_POS         = BK_BLE_SVC_MI_POS,
    SVC_EKS_MASK       = BK_BLE_SVC_EKS_MASK,
    SVC_EKS_POS        = BK_BLE_SVC_EKS_POS,
    SVC_AUTH_MASK      = BK_BLE_SVC_AUTH_MASK,
    SVC_AUTH_POS       = BK_BLE_SVC_AUTH_POS,
    SVC_DIS_MASK       = BK_BLE_SVC_DIS_MASK,
    SVC_DIS_POS        = BK_BLE_SVC_DIS_POS,
    SVC_UUID_LEN_MASK  = BK_BLE_SVC_UUID_LEN_MASK,
    SVC_UUID_LEN_POS   = BK_BLE_SVC_UUID_LEN_POS,
    SVC_SECONDARY_MASK = BK_BLE_SVC_SECONDARY_MASK,
    SVC_SECONDARY_POS  = BK_BLE_SVC_SECONDARY_POS,
};

#endif

struct adv_param {
    uint8_t  advData[BK_BLE_MAX_ADV_DATA_LEN];   /**< Advertising data */
    uint8_t  advDataLen;                  /**< The length of advertising data */
    uint8_t  respData[BK_BLE_MAX_ADV_DATA_LEN];  /**< Scan response data */
    uint8_t  respDataLen;                 /**< The length of scan response data */
    uint8_t  channel_map;                 /**< Advertising channel map */
    uint16_t interval_min;                /**< Minimum advertising interval */
    uint16_t interval_max;                /**< Maximum advertising interval */
    uint16_t duration;                    /**< Advertising duration */
};

struct scan_param {
    uint8_t  filter_en;                   /**< The control of filter */
    uint8_t  channel_map;                 /**< Channel mapping */
    uint16_t interval;                    /**< The scanning interval */
    uint16_t window;                      /**< The scanning window */
};


/**
 * @brief     Start a ble advertising
 *
 * @param
 *    - actv_idx: the index of activity
 *    - adv: the adv parameter
 *    - callback: register a callback for this action
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_adv_start(uint8_t actv_idx, struct adv_param *adv, ble_cmd_cb_t callback);

/**
 * @brief     Stop the advertising that has been started
 *
 * @param
 *    - actv_idx: the index of activity
 *    - callback: register a callback for this action
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_adv_stop(uint8_t actv_idx, ble_cmd_cb_t callback);

/**
 * @brief     Start a ble scan
 *
 * @param
 *    - actv_idx: the index of activity
 *    - scan: the scan parameter
 *    - callback: register a callback for this action
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_scan_start(uint8_t actv_idx, struct scan_param *scan, ble_cmd_cb_t callback);

/**
 * @brief     Stop the scan that has been started
 *
 * @param
 *    - actv_idx: the index of activity
 *    - callback: register a callback for this action
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_scan_stop(uint8_t actv_idx, ble_cmd_cb_t callback);//todo delete




/**
 * @brief     Find the specific activity by activity's state
 *
 * @param
 *    - state: the activity's state, see actv_state_t
 *
 * @return
 *    - xx: the index of the activity in this state
 */
uint8_t bk_ble_find_actv_state_idx_handle(uint8_t state);//todo: too much, del


/**
 * @brief     Find the specific initiating activity by activity's state
 *
 * @param
 *    - state: the initiating activity's state, see app_init_state
 *
 * @return
 *    - xx: the index of the initiating activity in this state
 */
uint8_t bk_ble_find_master_state_idx_handle(uint8_t state);

/**
 * @brief     Init the BLE driver
 *
 * This API should be called before any other BLE APIs.
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_init(void);

/**
 * @brief     Deinit the BLE driver
 *
 * This API free all resources related to BLE.
 *
 * @attention 1. This API is not ready yet, will support in future release.
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_deinit(void);




/**
 * @brief     Enable ble power save
 *
 * @return
 *    - void
 */
extern void ble_ps_enable_set(void);

/**
 * @brief     Disable ble power save
 *
 * @return
 *    - void
 */
extern void ble_ps_enable_clear(void);

/**
 * @brief     Get ble power save state
 *
 * @return
 *    - 1: power save is enabled
 *    - 0: power save is disabled
 */
extern UINT32 ble_ps_enabled(void );

/**
 * @brief     get ble connection tx/rx phy info
 *
  * @param
 *    - conn_idx: the connection idx
 *    - callback: register a callback for this action, ble_cmd_t: BLE_CONN_READ_PHY
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_read_phy(uint8_t conn_idx, ble_cmd_cb_t callback);

/**
 * @brief     set ble connection tx/rx phy info
 *
  * @param
 *    - conn_idx: the connection idx
 *    - phy_info: the set phy_info
 *    - callback: register a callback for this action, ble_cmd_t: BLE_CONN_SET_PHY
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_set_phy(uint8_t conn_idx, ble_set_phy_t * phy_info, ble_cmd_cb_t callback);




/**
 * @brief for hci raw data callback.
 *
 * stack report type to upper
 *
 * @param
 * - buf: payload, see BK_BLE_HCI_TYPE as buf[0]
 * - len: buf's len
 *
 * @return
 * - BK_ERR_BLE_SUCCESS: succeed
**/
typedef ble_err_t (*ble_hci_raw_to_host_cb)(uint8_t *buf, uint16_t len);


/**
 * @brief     reg hci recv raw data callback
 *
 * @param
 *    - cb: callback function
 *
 * @attention 1. you must call this after recv BLE_5_STACK_OK evt !
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_reg_hci_raw_recv_callback(ble_hci_raw_to_host_cb cb);


/**
 * @brief send hci raw data to controller.
 *
 *
 * @param
 * - buf: payload, see buf[0] as BK_BLE_HCI_TYPE
 * - len: buf's len
 *
 * @attention 1. you must call this after bk_ble_reg_hci_recv_callback !
 *
 * @return
 * - BK_ERR_BLE_SUCCESS: succeed
**/
ble_err_t bk_ble_hci_raw_to_controller(uint8_t *buf, uint16_t len);

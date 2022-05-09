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

#ifndef INCLUDE_MODULES_BK_BLE_API_H_
#define INCLUDE_MODULES_BK_BLE_API_H_


#include <modules/ble_types.h>

#ifdef __cplusplus
extern"C" {
#endif

/**
 * @brief ble APIs Version 1.0
 * @defgroup bk_ble_api_v1 New ble api group
 * @{
 */

/**
 * @brief     Register a gatt service
 * @param
 *     - ble_db_cfg: service param
 *
 * User example:
 *     First we must build test_service_db
 *     test_service_db is a database for att, which used in ble discovery. reading writing and other operation is used on a att database.
 *
 *
 * @code
 *     enum {
 *         TEST_IDX_SVC,
 *         TEST_IDX_CHAR_DECL,
 *         TEST_IDX_CHAR_VALUE,
 *         TEST_IDX_CHAR_DESC,
 *
 *         TEST_IDX_CHAR_DATALEN_DECL,
 *         TEST_IDX_CHAR_DATALEN_VALUE,
 *
 *         TEST_IDX_CHAR_INTER_DECL,
 *         TEST_IDX_CHAR_INTER_VALUE,
 *
 *         TEST_IDX_NB,
 *     };
 *
 *     //att records database.
 *
 *     ble_attm_desc_t test_service_db[TEST_IDX_NB] = {
 *        //  Service Declaration
 *        [TEST_IDX_SVC]              = {DECL_PRIMARY_SERVICE_128, BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
 *        // Characteristic declare
 *        [TEST_IDX_CHAR_DECL]    = {DECL_CHARACTERISTIC_128,  BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
 *        // Characteristic Value
 *        [TEST_IDX_CHAR_VALUE]   = {{0x34, 0x12, 0},     BK_BLE_PERM_SET(NTF, ENABLE), BK_BLE_PERM_SET(RI, ENABLE) | BK_BLE_PERM_SET(UUID_LEN, UUID_16), 128},
 *        //Client Characteristic Configuration Descriptor
 *        [TEST_IDX_CHAR_DESC] = {DESC_CLIENT_CHAR_CFG_128, BK_BLE_PERM_SET(RD, ENABLE) | BK_BLE_PERM_SET(WRITE_REQ, ENABLE), 0, 0},
 *     };
 * @endcode
 * TEST_IDX_SVC is nessecery, is declare a primary att service. The macro define is:
 *
 * @code
 *     #define DECL_PRIMARY_SERVICE_128     {0x00,0x28,0}
 * @endcode
 *
 * which is an UUID say it is a "primary service"
 * BK_BLE_PERM_SET(RD, ENABLE) means it can be read, and must be read, so it can be discove by peer master.
 *
 * TEST_IDX_CHAR_DECL declare a characteristic as a element in service, it must be BK_BLE_PERM_SET(RD, ENABLE)
 *
 * @code
 * #define DECL_CHARACTERISTIC_128      {0x03,0x28,0}
 * @endcode
 * show it's a "characteristic"
 *
 * BK_BLE_PERM_SET(RD, ENABLE) means it can be read, and must be read, so it can be discove by peer master.
 *
 *
 * TEST_IDX_CHAR_VALUE is the real value of TEST_IDX_CHAR_DECL,
 * {0x34, 0x12, 0} means it's type is 0x1234, you can determine by you self
 * BK_BLE_PERM_SET(NTF, ENABLE) means it cant notify peer, such as value change. For exzample, BLE mouse report pos by "notify" peer.
 * BK_BLE_PERM_SET(RI, ENABLE) means if peer read this att record, it will enable notification.
 * BK_BLE_PERM_SET(UUID_LEN, UUID_16) means the first elem's max len of TEST_IDX_CHAR_VALUE.
 *
 * TEST_IDX_CHAR_DESC is a Client Characteristic Configuration Descriptor for TEST_IDX_CHAR_VALUE, it used by peer master as know as a client.
 * As common usage, it config TEST_IDX_CHAR_VALUE indication or notification. Peer can write this att handle to triggle it.
 * Must be BK_BLE_PERM_SET(RD, ENABLE) | BK_BLE_PERM_SET(WRITE_REQ, ENABLE)
 *
 * Now, you have a basic database for peer, in this case, peer write TEST_IDX_CHAR_DESC or read TEST_IDX_CHAR_VALUE to enable notification, and then we notify peer by TEST_IDX_CHAR_VALUE
 *
 *
 * Secondlly, we build ble_db_cfg
 * @code
 *     struct bk_ble_db_cfg ble_db_cfg;
 *
 *     ble_db_cfg.att_db = (ble_attm_desc_t *)test_service_db;
 *     ble_db_cfg.att_db_nb = TEST_IDX_NB;
 *     ble_db_cfg.prf_task_id = g_test_prf_task_id;
 *     ble_db_cfg.start_hdl = 0;
 *     ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16);
 * @endcode
 * prf_task_id is app handle. If you have multi att service, used prf_task_id to distinguish it.
 * svc_perm show TEST_IDX_SVC UUID type's len.
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_create_db (struct bk_ble_db_cfg* ble_db_cfg);

/**
 * @brief     Register ble event notification callback
 *
 * @param
 *    - func: event callback
 *
 * @attention 1. you must regist it, otherwise you cant get any event !
 * @attention 2. you must regist it before bk_ble_create_db, otherwise you cant get BLE_5_CREATE_DB event
 *
 * User example:
 * @code
 * void ble_at_notice_cb(ble_notice_t notice, void *param)
{
    switch (notice) {

    case BLE_5_WRITE_EVENT: {

        if (w_req->prf_id == g_test_prf_task_id)
        {
            switch(w_req->att_idx)
            {
            case TEST_IDX_CHAR_DECL:
                break;
            case TEST_IDX_CHAR_VALUE:
                break;
            case TEST_IDX_CHAR_DESC:
                //when peer enable notification, we create time and notify peer, such as
                //write_buffer = (uint8_t *)os_malloc(s_test_data_len);
                //bk_ble_send_noti_value(s_test_data_len, write_buffer, g_test_prf_task_id, TEST_IDX_CHAR_VALUE);
                break;

            default:
                break;
            }
        }
        break;
    }
    case BLE_5_CREATE_DB:
    //bk_ble_create_db success here
    break;
    }
}

bk_ble_set_notice_cb(ble_at_notice_cb);
 * @endcode
 * @return
 *    - void
 */
void bk_ble_set_notice_cb(ble_notice_cb_t func);

/**
 * @brief     Get device name
 *
 * @param
 *    - name: store the device name
 *    - buf_len: the length of buf to store the device name
 *
 * @return
 *    - length: the length of device name
 */
uint8_t bk_ble_appm_get_dev_name(uint8_t* name, uint32_t buf_len);

/**
 * @brief     Set device name
 *
 * @param
 *    - len: the length of device name
 *    - name: the device name to be set
 *
 * @return
 *    - length: the length of device name
 */
uint8_t bk_ble_appm_set_dev_name(uint8_t len, uint8_t* name);

/**
 * @brief     Create a ble advertising activity
 *
 * @param
 *    - actv_idx: the index of activity
 *    - adv_param: the advertising parameter
 *    - callback: register a callback for this action, ble_cmd_t: BLE_CREATE_ADV
 * @attention 1.you must wait callback status, 0 mean success.
 *
 * User example:
 * @code
 *     ble_adv_param_t adv_param;
 *
 *     adv_param.own_addr_type = 0;//BLE_STATIC_ADDR
 *     adv_param.adv_type = 0; //ADV_IND
 *     adv_param.chnl_map = 7;
 *     adv_param.adv_prop = 3;
 *     adv_param.adv_intv_min = 0x120; //min
 *     adv_param.adv_intv_max = 0x160; //max
 *     adv_param.prim_phy = 1;// 1M
 *     adv_param.second_phy = 1;// 1M
 *     actv_idx = bk_ble_get_idle_actv_idx_handle();
 *     if (actv_idx != UNKNOW_ACT_IDX) {
 *         bk_ble_create_advertising(actv_idx, &adv_param, ble_at_cmd_cb);
 *     }
 * @endcode
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_create_advertising(uint8_t actv_idx, ble_adv_param_t *adv_param, ble_cmd_cb_t callback);

/**
 * @brief     Start a ble advertising
 *
 * @param
 *    - actv_idx: the index of activity
 *    - duration: Advertising duration (in unit of 10ms). 0 means that advertising continues
 *    - callback: register a callback for this action, ble_cmd_t: BLE_START_ADV
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_advertising
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_start_advertising(uint8_t actv_idx, uint16 duration, ble_cmd_cb_t callback);

/**
 * @brief     Stop the advertising that has been started
 *
 * @param
 *    - actv_idx: the index of activity
 *    - callback: register a callback for this action, ble_cmd_t: BLE_STOP_ADV
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_start_advertising
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_stop_advertising(uint8_t actv_idx, ble_cmd_cb_t callback);

/**
 * @brief     Delete the advertising that has been created
 *
 * @param
 *    - actv_idx: the index of activity
 *    - callback: register a callback for this action, ble_cmd_t: BLE_DELETE_ADV
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_advertising
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_delete_advertising(uint8_t actv_idx, ble_cmd_cb_t callback);

/**
 * @brief     Set the advertising data
 *
 * @param
 *    - actv_idx: the index of activity
 *    - adv_buff: advertising data
 *    - adv_len: the length of advertising data
 *    - callback: register a callback for this action, ble_cmd_t: BLE_SET_ADV_DATA
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_advertising
 *
 *
 * User example:
 * @code
 *     const uint8_t adv_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
 *     bk_ble_set_adv_data(actv_idx, adv_data, sizeof(adv_data), ble_at_cmd_cb);
 * @endcode
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_set_adv_data(uint8_t actv_idx, uint8_t* adv_buff, uint8_t adv_len, ble_cmd_cb_t callback);

/**
 * @brief     Set the scan response data
 *
 * @param
 *    - actv_idx: the index of activity
 *    - scan_buff: scan response data
 *    - scan_len: the length of scan response data
 *    - callback: register a callback for this action, ble_cmd_t: BLE_SET_RSP_DATA
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.scan rsp data similaly to adv data
 * @attention 3.must used after bk_ble_create_advertising
 *
 *
 * User example:
 * @code
 *     const uint8_t scan_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
 *     bk_ble_set_scan_rsp_data(actv_idx, scan_data, sizeof(scan_data), ble_at_cmd_cb);
 * @endcode
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_set_scan_rsp_data(uint8_t actv_idx, uint8_t* scan_buff, uint8_t scan_len, ble_cmd_cb_t callback);

/**
 * @brief     Set the periodic advertising data
 *
 * @param
 *    - actv_idx: the index of activity
 *    - per_adv_buff: periodic advertising data
 *    - per_adv_len: the length of periodic advertising data
 *    - callback: register a callback for this action, ble_cmd_t: BLE_SET_ADV_DATA????
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_advertising
 *
 * User example:
 * @code
 *     const uint8_t adv_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
 *     bk_ble_set_per_adv_data(actv_idx, adv_data, sizeof(adv_data), ble_at_cmd_cb);
 * @endcode
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_set_per_adv_data(uint8_t actv_idx, uint8_t* per_adv_buff, uint8_t per_adv_len, ble_cmd_cb_t callback);

/**
 * @brief     Read the phy of connection device
 *
 * @param
 *    - conn_idx: the index of connection device
 *    - callback: register a callback for this action, ble_cmd_t: BLE_CONN_READ_PHY
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after after connected
 *
 * User example:
 * @code
 *     bk_ble_read_phy(conn_idx, ble_at_cmd_cb);
 * @endcode
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_read_phy(uint8_t conn_idx, ble_cmd_cb_t callback);


/**
 * @brief     Set the phy of connection device
 *
 * @param
 *    - conn_idx: the index of connection device
 *    - phy_info: phy parameters
 *    - callback: register a callback for this action, ble_cmd_t: BLE_CONN_SET_PHY
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after after connected
 *
 * User example:
 * @code
 *     ble_set_phy_t * phy = {0x04, 0x01, 0x01};
 *     //set tx phy to s2 coded phy, and set rx phy to 1M phy
 *     bk_ble_set_phy(1, phy, ble_at_cmd_cb);
 * @endcode
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_set_phy(uint8_t conn_idx, ble_set_phy_t * phy_info, ble_cmd_cb_t callback);

/**
 * @brief     Update connection parameters
 *
 * @param
 *    - conn_idx: the index of connection
 *    - conn_param: connection parameters
 *    - callback: register a callback for this action, ble_cmd_t: BLE_CONN_UPDATE_PARAM
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after connected
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_update_param(uint8_t conn_idx, ble_conn_param_t *conn_param, ble_cmd_cb_t callback);

/**
 * @brief     Disconnect a ble connection
 *
 * @param
 *    - conn_idx: the index of connection
 *    - callback: register a callback for this action, ble_cmd_t: BLE_CONN_DIS_CONN
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after connected
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_disconnect(uint8_t conn_idx, ble_cmd_cb_t callback);

/**
 * @brief     Exchange MTU
 *
 * @param
 *    - conn_idx: the index of connection
 *    - callback: register a callback for this action, ble_cmd_t: BLE_CONN_UPDATE_MTU
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after connected
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_gatt_mtu_change(uint8_t conn_idx, ble_cmd_cb_t callback);

/**
 * @brief     Create a ble scan activity
 *
 * @param
 *    - actv_idx: the index of activity
 *    - scan_param: the scan parameter
 *    - callback: register a callback for this action, ble_cmd_t: BLE_CREATE_SCAN
 *
 * @attention 1.you must wait callback status, 0 mean success.
 *
 * User exzample:
 * @code
    ble_scan_param_t scan_param;

    scan_param.own_addr_type = 0;//BLE_STATIC_ADDR
    scan_param.scan_phy = 5;
    scan_param.scan_intv = 0x64; //interval
    scan_param.scan_wd = 0x1e; //windows
    bk_ble_create_scaning(actv_idx, &, ble_at_cmd);
 *
 * @endcode
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_create_scaning(uint8_t actv_idx, ble_scan_param_t *scan_param, ble_cmd_cb_t callback);

/**
 * @brief     Start a ble scan
 *
 * @param
 *    - actv_idx: the index of activity
 *    - callback: register a callback for this action, ble_cmd_t: BLE_START_SCAN
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_scaning
 * @attention 3.adv will report in ble_notice_cb_t as BLE_5_REPORT_ADV
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_start_scaning(uint8_t actv_idx, ble_cmd_cb_t callback);

/**
 * @brief     Stop the scan that has been started
 *
 * @param
 *    - actv_idx: the index of activity
 *    - callback: register a callback for this action, ble_cmd_t: BLE_STOP_SCAN
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_start_scaning
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_stop_scaning(uint8_t actv_idx, ble_cmd_cb_t callback);

/**
 * @brief     Delete the scan that has been created
 *
 * @param
 *    - actv_idx: the index of activity
 *    - callback: register a callback for this action, ble_cmd_t: BLE_DELETE_SCAN
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_scaning
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_delete_scaning(uint8_t actv_idx, ble_cmd_cb_t callback);

/**
 * @brief     Create a activity for initiating a connection
 *
 * @param
 *    - con_idx: the index of connection
 *    - conn_param: the connection parameter
 *    - callback: register a callback for this action, ble_cmd_t: BLE_INIT_CREATE
 *
 * @attention 1.you must wait callback status, 0 mean success.
 *
 * User example:
 * @code
 *
 *  ble_conn_param_t conn_param;
    conn_param.intv_min = 0x40; //interval
    conn_param.intv_max = 0x40; //interval
    conn_param.con_latency = 0;
    conn_param.sup_to = 0x200;//supervision timeout
    conn_param.init_phys = 1;// 1M
    bk_ble_create_init(con_idx, &conn_param, ble_at_cmd);
 * @endcode
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_create_init(uint8_t con_idx, ble_conn_param_t *conn_param, ble_cmd_cb_t callback);

/**
 * @brief     Initiate a connection
 *
 * @param
 *    - con_idx: the index of connection
 *    - callback: register a callback for this action, ble_cmd_t: BLE_INIT_START_CONN
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_init
 * @attention 3.when connect result, will recv BLE_5_INIT_CONNECT_EVENT in ble_notice_cb_t
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_init_start_conn(uint8_t con_idx, ble_cmd_cb_t callback);

/**
 * @brief     Stop a connection
 *
 * @param
 *    - con_idx: the index of connection
 *    - callback: register a callback for this action, ble_cmd_t: BLE_INIT_STOP_CONN
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_init_start_conn
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_init_stop_conn(uint8_t con_idx,ble_cmd_cb_t callback);//todo: upper feature

/**
 * @brief     Set the address of the device to be connected
 *
 * @param
 *    - connidx: the index of connection
 *    - bdaddr: the address of the device to be connected
 *    - addr_type: the address type of the device to be connected, 1: public 0: random
 *
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_init
 * @attention 3.addr_type must right, if wrong, cant connect
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_init_set_connect_dev_addr(uint8_t connidx, bd_addr_t *bdaddr, uint8_t addr_type);

/**
 * @brief     Get an idle activity
 *
 * @return
 *    - xx: the idle activity's index
 */
uint8_t bk_ble_get_idle_actv_idx_handle(void);

/**
 * @brief     Get the maximum count of activities
 *
 * @return
 *    - xx: the maximum count of activities
 */
uint8_t bk_ble_get_max_actv_idx_count(void);

/**
 * @brief     Get the maximum count of supported connections
 *
 * @return
 *    - xx: the maximum count of supported connections
 */
uint8_t bk_ble_get_max_conn_idx_count(void);


/**
 * @brief     Get an idle connection activity
 *
 * @return
 *    - xx: the idle connection activity's index
 */
uint8_t bk_ble_get_idle_conn_idx_handle(void);


/**
 * @brief     Find the specific connection activity by address
 *
 * @param
 *    - connt_addr: the address of the connected device
 *
 * @return
 *    - xx: the index of the connection activity meeting the address
 */
uint8_t bk_ble_find_conn_idx_from_addr(bd_addr_t *connt_addr);


/**
 * @brief     Get the connection state of the specific device
 *
 * @param
 *    - connt_addr: the device's address
 *
 * @return
 *    - 1: this device is connected
 *    - 0: this device is disconnected
 */
uint8_t bk_ble_get_connect_state(bd_addr_t * connt_addr);

/**
 * @brief     get ble mac addr
 *
 * @attention 1. return mac is 6 bytes.
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_get_mac(uint8_t *mac);

/**
 * @brief     set ble mac addr
 *
 * @param
 *    - actv_idx: actv_idx: the index of activity
 *    - mac: the device's address
 *    - callback: register a callback for this action
 *
 * @attention 1. This API is not ready yet.
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_set_mac(uint8_t actv_idx, uint8_t *mac, ble_cmd_cb_t callback);

/**
 * @brief As slaver, send a notification of an attribute's value
 *
 * @param
 *    - len: the length of attribute's value
 *    - buf: attribute's value
 *    - prf_id: The id of the profile
 *    - att_idx: The index of the attribute
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_send_noti_value(uint32_t len, uint8_t *buf, uint16_t prf_id, uint16_t att_idx);

/**
 * @brief     reg hci recv callback
 *
 * @param
 *    - evt_cb: evt callback function
 *    - acl_cb: acl callback function
 *
 * @attention 1. you must call this after recv BLE_5_STACK_OK evt !
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ble_err_t bk_ble_reg_hci_recv_callback(ble_hci_to_host_cb evt_cb, ble_hci_to_host_cb acl_cb);

/**
 * @brief send hci to controller.
 *
 *
 * @param
 * - type: see BK_BLE_HCI_TYPE.
 * - buf: payload
 * - len: buf's len
 *
 * @attention 1. you must call this after bk_ble_reg_hci_recv_callback !
 *
 * @return
 * - BK_ERR_BLE_SUCCESS: succeed
**/
ble_err_t bk_ble_hci_to_controller(uint8_t type, uint8_t *buf, uint16_t len);



/**
 * @brief send hci cmd to controller.
 *
 *
 * @param
 * - buf: payload
 * - len: buf's len
 *
 * @attention 1. you must call this after bk_ble_reg_hci_recv_callback !
 *
 * @return
 * - BK_ERR_BLE_SUCCESS: succeed
**/
ble_err_t bk_ble_hci_cmd_to_controller(uint8_t *buf, uint16_t len);

/**
 * @brief send hci acl to controller.
 *
 *
 * @param
 * - buf: payload
 * - len: buf's len
 *
 * @attention 1. you must call this after bk_ble_reg_hci_recv_callback !
 *
 * @return
 * - BK_ERR_BLE_SUCCESS: succeed
**/
ble_err_t bk_ble_hci_acl_to_controller(uint8_t *buf, uint16_t len);



/*
 * @brief get if stack support central and link count
 *
 * @param
 * - count: if return true, show how many central link can be support, otherwise not used.
 *
 * @return
 *    - 1: support
 *    - 0: not support.
 */
uint8_t bk_ble_if_support_central(uint8_t *count);

/*
 * @brief get controller stack type
 *
 * @return
 *    enum BK_BLE_CONTROLLER_STACK_TYPE
 */

BK_BLE_CONTROLLER_STACK_TYPE bk_ble_get_controller_stack_type(void);

/*
 * @}
 */
#ifdef __cplusplus
}
#endif

//#endif

#endif

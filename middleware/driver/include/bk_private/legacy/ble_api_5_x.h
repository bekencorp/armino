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
#include "modules/dm_ble_types.h"

// no used
//#define BK_BLE_MAX_SCAN_NUM               (15)
//#define BK_BLE_ABIT(n) (1 << n)



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
    @brief use in @ref BLE_5_DISCOVERY_PRIMARY_SERVICE_EVENT
 */
typedef struct
{
    API_RESULT event_result;
    //ATT_HANDLE att_handle;
    uint8_t conn_handle;
    GATT_SERVICE_PARAM *service;
    uint16_t count;
} ble_discovery_primary_service_t;


/**
    @brief use in @ref BLE_5_DISCOVERY_CHAR_EVENT
 */
typedef struct
{
    API_RESULT event_result;
    //    ATT_HANDLE att_handle;
    uint8_t conn_handle;
    //ble_gatt_characteristic_param_t *character;
    GATT_CHARACTERISTIC_PARAM *character;
    uint16_t count;
} ble_discovery_char_t;



/**
    @brief use in ble connection
 */
typedef struct
{
    uint8_t    peer_address_type;
    bd_addr_t  peer_address;
    uint8_t    initiating_phys;

    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
} ble_conn_param_normal_t;

/**
    @brief use in ble connection
 */
typedef struct
{
    uint8_t    initiator_filter_policy;
    uint8_t    own_address_type;
    uint8_t    peer_address_type;
    bd_addr_t  peer_address;
    uint8_t    initiating_phys;
    uint16_t *scan_interval;
    uint16_t *scan_window;
    uint16_t *conn_interval_min;
    uint16_t *conn_interval_max;
    uint16_t *conn_latency;
    uint16_t *supervision_timeout;
    uint16_t *minimum_ce_length;
    uint16_t *maximum_ce_length;
} ble_conn_param_ex_t;

/**
    @brief use in ble connection
 */
typedef struct
{
    uint8_t    peer_address_type;
    bd_addr_t  peer_address;
    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t minimum_ce_length;
    uint16_t maximum_ce_length;
} ble_update_conn_param_t;

/**
    @brief use in BLE_5_REPORT_ADV
 */
typedef struct
{
    uint16_t evt_type;
    uint8_t    peer_address_type;
    bd_addr_t  peer_address;
    uint8_t rssi;
    uint8_t *data;
    uint8_t data_len;
    uint8_t *name;
} ble_adv_report_t;

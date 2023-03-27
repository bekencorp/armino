#pragma once

#include "modules/dm_ble_types.h"
#include "modules/dm_ble.h"

#ifdef __cplusplus
extern"C" {
#endif

//legacy
ble_err_t bk_ble_init_ext(void *func);//ble_notice_cb_t func
uint8_t bk_ble_find_actv_state_idx_handle(uint8_t state);
uint8_t bk_ble_find_master_state_idx_handle(uint8_t state);

//ethermind
ble_err_t bk_ble_set_tasks_stack_size(uint16_t size, uint8_t index);
uint16_t bk_ble_get_task_stack_size(uint8_t index);
ble_err_t bk_ble_get_tasks_name(char *name[], uint8_t max_name_len, uint8_t *count);
const char *bk_ble_get_task_name(uint8_t index);
uint8_t bk_ble_get_task_count(void);

ble_err_t bk_ble_create_connection(ble_conn_param_normal_t *conn_param, ble_cmd_cb_t callback);
ble_err_t bk_ble_create_connection_ex(ble_conn_param_ex_t *conn_param, ble_cmd_cb_t callback);



ble_err_t bk_ble_set_advertising_params_extended(
    uint8_t   adv_handle,
    uint16_t  adv_event_properties,
    uint32_t  primary_advertising_interval_min,
    uint32_t  primary_advertising_interval_max,
    uint8_t   primary_advertising_channel_map,
    uint8_t   own_address_type,
    uint8_t   peer_address_type,
    uint8_t *peer_address,
    uint8_t   advertising_filter_policy,
    int8_t    advertising_tx_power,
    uint8_t   primary_advertising_phy,
    uint8_t   secondary_adv_max_skip,
    uint8_t   secondary_advertising_phy,
    uint8_t   advertising_set_id,
    uint8_t   scan_req_nfy_enable,
    ble_cmd_cb_t callback);

ble_err_t bk_ble_set_advertising_data_extended(
    uint8_t   advertising_handle,
    uint8_t   operation,
    uint8_t   frag_pref,
    uint8_t *adv_buff,
    uint8_t   adv_len,
    ble_cmd_cb_t callback);

ble_err_t bk_ble_set_advertising_enable_extended(
    uint8_t    enable,
    uint8_t    number_of_sets,
    uint8_t   *advertising_handle,
    uint16_t *duration,
    uint8_t   *max_extd_adv_evts,
    ble_cmd_cb_t callback);

ble_err_t bk_ble_att_write(uint8_t conn_handle, ATT_ATTR_HANDLE hdl, uint8_t *value, uint16_t length);


ble_err_t bk_ble_set_scan_response_data_extended(
    uint8_t   advertising_handle,
    uint8_t   operation,
    uint8_t   fragment_pref,
    uint8_t   scan_response_data_length,
    uint8_t *scan_response_data,
    ble_cmd_cb_t callback);



ble_err_t bk_ble_set_scan_parameters_extended(
    uint8_t    own_address_type,
    uint8_t    scanning_filter_policy,
    uint8_t    scanning_phy,
    uint16_t *scan_interval,
    uint16_t *scan_window,
    ble_cmd_cb_t callback);

ble_err_t bk_ble_set_scan_parameters(
    uint8_t    own_address_type,
    uint8_t    scanning_filter_policy,
    uint8_t    scanning_phy,
    uint16_t   scan_interval,
    uint16_t   scan_window,
    ble_cmd_cb_t callback);

ble_err_t bk_ble_set_scan_enable_extended(
    uint8_t   enable,
    uint8_t   filter_duplicates,
    uint16_t  duration,
    uint16_t  period,
    ble_cmd_cb_t callback);


/**
  @brief     send notify when recv enable notify

  @param conn_handle: connection handle
  @param adv DEVICE_HANDLE:

  @attention 1.you must wait callback status, 0 mean success.
  @return
     - BK_ERR_BLE_SUCCESS: succeed
     - others: other errors.
 */
ble_err_t bk_ble_get_conn_handle_from_device_handle(uint8 *conn_handle, DEVICE_HANDLE *device_handle);//ble_err_t bk_ble_get_att_handle_from_device_handle(ATT_CON_ID *att_handle, DEVICE_HANDLE *device_handle);


ble_err_t bk_ble_hci_read_phy(
    bd_addr_t *peer_addr,
    uint8_t peer_addr_type,
    ble_cmd_cb_t callback);

ble_err_t bk_ble_hci_set_phy(
    bd_addr_t *peer_addr,
    uint8_t peer_addr_type,
    ble_set_phy_t *le_set_phy,
    ble_cmd_cb_t callback);

ble_err_t bk_ble_set_local_name(     uint8_t *name, uint8_t name_len,  ble_cmd_cb_t callback);
ble_err_t bk_ble_get_local_name(     ble_cmd_cb_t callback);
ble_err_t bk_ble_read_local_addr(ble_cmd_cb_t callback);


void bk_ble_bt_gatt_db_get_char_val_hndl(GATT_DB_HANDLE *gdbh, ATT_ATTR_HANDLE *attr_handle);

ble_err_t bk_ble_cancel_connect(ble_cmd_cb_t callback);
ble_err_t bk_ble_update_connection_params(ble_update_conn_param_t *conn_param, ble_cmd_cb_t callback);
ble_err_t bk_ble_set_gatt_mtu(uint8_t conn_handle, uint16_t mtu);

ATT_ATTR_HANDLE bk_ble_get_current_gatt_db_attr_handle(void);


#ifdef __cplusplus
}
#endif


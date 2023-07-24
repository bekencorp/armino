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
#include "modules/bluetooth/dm_gatt_types.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup dm_gatts_v1 GATT SERVER API
 * @{
 */

/**
 * @defgroup dm_gatts_types_v1 Defines
 * @{
 */

/// GATT Server callback function events
typedef enum
{
    BK_GATTS_REG_EVT                 = 0,       /*!< When register application id, the event comes */
    BK_GATTS_READ_EVT                = 1,       /*!< When gatt client request read operation, the event comes */
    BK_GATTS_WRITE_EVT               = 2,       /*!< When gatt client request write operation, the event comes */
    BK_GATTS_EXEC_WRITE_EVT          = 3,       /*!< When gatt client request execute write, the event comes */
    BK_GATTS_MTU_EVT                 = 4,       /*!< When set mtu complete, the event comes */
    BK_GATTS_CONF_EVT                = 5,       /*!< When receive confirm, the event comes */
    BK_GATTS_UNREG_EVT               = 6,       /*!< When unregister gatt_if, the event comes */
    BK_GATTS_CREATE_EVT              = 7,       /*!< When create service complete, the event comes */

    BK_GATTS_ADD_CHAR_EVT            = 9,       /*!< When add characteristic complete, the event comes */
    BK_GATTS_ADD_CHAR_DESCR_EVT      = 10,      /*!< When add descriptor complete, the event comes */

    BK_GATTS_START_EVT               = 12,      /*!< When start service complete, the event comes */

    BK_GATTS_CONNECT_EVT             = 14,      /*!< When gatt client connect, the event comes */
    BK_GATTS_DISCONNECT_EVT          = 15,      /*!< When gatt client disconnect, the event comes */

    /* following is extra event */
    BK_GATTS_RESPONSE_EVT            = 21,      /*!< When gatt send response complete, the event comes */
    BK_GATTS_CREAT_ATTR_TAB_EVT      = 22,      /*!< When gatt create table complete, the event comes */
} bk_gatts_cb_event_t;

/**
 * @brief Gatt server callback parameters union
 */
typedef union
{
    /**
     * @brief BK_GATTS_REG_EVT
     */
    struct gatts_reg_evt_param
    {
        bk_gatt_status_t status;       /*!< Operation status */
        uint16_t app_id;                /*!< Application id which input in register API, not used now */
        bk_gatt_if_t gatt_if;           /// gatt_if, only one will return now !!!
    } reg;                              /*!< Gatt server callback param of BK_GATTS_REG_EVT */

    /**
     * @brief BK_GATTS_READ_EVT
     */
    struct gatts_read_evt_param
    {
        uint16_t conn_id;               /*!< Connection id */
        uint32_t trans_id;              /*!< Transaction id */
        bk_bd_addr_t bda;              /*!< The bluetooth device address which been read */
        uint16_t handle;                /*!< The attribute handle */
        uint16_t offset;                /*!< Offset of the value, if the value is too long */
        bool is_long;                   /*!< The value is too long or not */
        bool need_rsp;                  /*!< The read operation need to do response */
    } read;                             /*!< Gatt server callback param of BK_GATTS_READ_EVT */


    /**
     * @brief BK_GATTS_WRITE_EVT
     */
    struct gatts_write_evt_param
    {
        uint16_t conn_id;               /*!< Connection id */
        uint32_t trans_id;              /*!< Transaction id */
        bk_bd_addr_t bda;              /*!< The bluetooth device address which been written */
        uint16_t handle;                /*!< The attribute handle */
        uint16_t offset;                /*!< Offset of the value, if the value is too long */
        bool need_rsp;                  /*!< The write operation need to do response */
        bool is_prep;                   /*!< This write operation is prepare write */
        uint16_t len;                   /*!< The write attribute value length */
        uint8_t *value;                 /*!< The write attribute value */
    } write;                            /*!< Gatt server callback param of BK_GATTS_WRITE_EVT */

    /**
     * @brief BK_GATTS_EXEC_WRITE_EVT
     */
    struct gatts_exec_write_evt_param
    {
        uint16_t conn_id;               /*!< Connection id */
        uint32_t trans_id;              /*!< Transaction id */
        bk_bd_addr_t bda;              /*!< The bluetooth device address which been written */
#define BK_GATT_PREP_WRITE_CANCEL 0x00 /*!< Prepare write flag to indicate cancel prepare write */
#define BK_GATT_PREP_WRITE_EXEC   0x01 /*!< Prepare write flag to indicate execute prepare write */
        uint8_t exec_write_flag;        /*!< Execute write flag */
        bool need_rsp;                  /// The write operation need to do response
    } exec_write;                       /*!< Gatt server callback param of BK_GATTS_EXEC_WRITE_EVT */

    /**
     * @brief BK_GATTS_MTU_EVT
     */
    struct gatts_mtu_evt_param
    {
        uint16_t conn_id;               /*!< Connection id */
        uint16_t mtu;                   /*!< MTU size */
    } mtu;                              /*!< Gatt server callback param of BK_GATTS_MTU_EVT */

    /**
     * @brief BK_GATTS_CONF_EVT
     */
    struct gatts_conf_evt_param
    {
        bk_gatt_status_t status;       /*!< Operation status */
        uint16_t conn_id;               /*!< Connection id */
        uint16_t handle;                /*!< attribute handle. Not used when indicate */
        uint16_t len;                   /*!< The indication or notification value length, len is valid when send notification or indication failed. Not used when indicate */
        uint8_t *value;                 /*!< The indication or notification value , value is valid when send notification or indication failed. Not used when indicate */
    } conf;                             /*!< Gatt server callback param of BK_GATTS_CONF_EVT (confirm) */

    /**
     * @brief BK_GATTS_UNREG_EVT
     */

    /**
     * @brief BK_GATTS_CREATE_EVT
     */
    struct gatts_create_evt_param
    {
        bk_gatt_status_t status;       /*!< Operation status */
        uint16_t service_handle;        /*!< Service attribute handle */
        bk_gatt_srvc_id_t service_id;  /*!< Service id, include service uuid and other information */
    } create;                           /*!< Gatt server callback param of BK_GATTS_CREATE_EVT */


    /**
     * @brief BK_GATTS_ADD_CHAR_EVT
     */
    struct gatts_add_char_evt_param
    {
        bk_gatt_status_t status;       /*!< Operation status */
        uint16_t attr_handle;           /*!< Characteristic attribute handle */
        uint16_t service_handle;        /*!< Service attribute handle */
        bk_bt_uuid_t char_uuid;        /*!< Characteristic uuid */
    } add_char;                         /*!< Gatt server callback param of BK_GATTS_ADD_CHAR_EVT */

    /**
     * @brief BK_GATTS_ADD_CHAR_DESCR_EVT
     */
    struct gatts_add_char_descr_evt_param
    {
        bk_gatt_status_t status;       /*!< Operation status */
        uint16_t attr_handle;           /*!< Descriptor attribute handle */
        uint16_t service_handle;        /*!< Service attribute handle */
        bk_bt_uuid_t descr_uuid;       /*!< Characteristic descriptor uuid */
    } add_char_descr;                   /*!< Gatt server callback param of BK_GATTS_ADD_CHAR_DESCR_EVT */

    /**
     * @brief BK_GATTS_START_EVT
     */
    struct gatts_start_evt_param
    {
        bk_gatt_status_t status;       /*!< Operation status */
        uint16_t service_handle;        /*!< Service attribute handle */
    } start;                            /*!< Gatt server callback param of BK_GATTS_START_EVT */

    /**
     * @brief BK_GATTS_CONNECT_EVT
     */
    struct gatts_connect_evt_param
    {
        uint16_t conn_id;               /*!< Connection id */
        uint8_t link_role;              /*!< Link role : master role = 0  ; slave role = 1*/
        bk_bd_addr_t remote_bda;       /*!< Remote bluetooth device address */
        bk_gatt_conn_params_t conn_params; /*!< current Connection parameters. Not used now */
    } connect;                          /*!< Gatt server callback param of BK_GATTS_CONNECT_EVT */

    /**
     * @brief BK_GATTS_DISCONNECT_EVT
     */
    struct gatts_disconnect_evt_param
    {
        uint16_t conn_id;               /*!< Connection id */
        bk_bd_addr_t remote_bda;       /*!< Remote bluetooth device address */
        bk_gatt_conn_reason_t reason;  /*!< Indicate the reason of disconnection. Not used now */
    } disconnect;                       /*!< Gatt server callback param of BK_GATTS_DISCONNECT_EVT */


    /**
     * @brief BK_GATTS_RESPONSE_EVT
     */
    struct gatts_rsp_evt_param
    {
        bk_gatt_status_t status;       /*!< Operation status */
        uint16_t handle;                /*!< Attribute handle which send response */
    } rsp;                              /*!< Gatt server callback param of BK_GATTS_RESPONSE_EVT */

    /**
     * @brief BK_GATTS_CREAT_ATTR_TAB_EVT
     */
    struct gatts_add_attr_tab_evt_param
    {
        bk_gatt_status_t status;       /*!< Operation status */
        bk_bt_uuid_t svc_uuid;         /*!< Service uuid type */
        uint16_t num_handle;            /*!< The number of the attribute handle to be added to the gatts database */
        uint16_t *handles;              /*!< The number to the handles */
    } add_attr_tab;                     /*!< Gatt server callback param of BK_GATTS_CREAT_ATTR_TAB_EVT */

} bk_ble_gatts_cb_param_t;

/**
 * @brief GATT Server callback function type
 * @param event : Event type
 * @param gatts_if : GATT server access interface, normally
 *                   different gatts_if correspond to different profile
 * @param param : Point to callback parameter, currently is union type
 */
typedef int32_t (* bk_gatts_cb_t)(bk_gatts_cb_event_t event, bk_gatt_if_t gatts_if, bk_ble_gatts_cb_param_t *param);


///@}


/**
 * @defgroup dm_gatts_functions_v1 Functions
 * @{
 */



/**
 * @brief           This function is called to register application callbacks
 *                  with BTA GATTS module.
 * @param             callback : see bk_gatts_cb_t
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_register_callback(bk_gatts_cb_t callback);

/**
 * @brief           This function is called to register application identifier
 * @param             app_id : app id, not used now
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_app_register(uint16_t app_id);



/**
 * @brief           unregister with GATT Server.
 *
 * @param[in]       gatts_if: GATT server access interface
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_app_unregister(bk_gatt_if_t gatts_if);


/**
 * @brief           Create a service. When service creation is done, a callback
 *                  event BK_GATTS_CREATE_EVT is called to report status
 *                  and service ID to the profile. The service ID obtained in
 *                  the callback function needs to be used when adding included
 *                  service and characteristics/descriptors into the service.
 *
 * @param[in]       gatts_if: GATT server access interface
 * @param[in]       service_id: service ID.
 * @param[in]       num_handle: number of handle requested for this service.
 *
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_create_service(bk_gatt_if_t gatts_if,
                                       bk_gatt_srvc_id_t *service_id, uint16_t num_handle);


/**
 * @brief               Create a service attribute tab.
 * @param[in]       gatts_attr_db: the pointer to the service attr tab
 * @param[in]       gatts_if: GATT server access interface
 * @param[in]       max_nb_attr: the number of attribute to be added to the service database.
 *
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_create_attr_tab(const bk_gatts_attr_db_t *gatts_attr_db,
                                        bk_gatt_if_t gatts_if,
                                        uint16_t max_nb_attr);


/**
 * @brief           This function is called to add a characteristic into a service.
 *
 * @param[in]       service_handle: service handle to which this included service is to
 *                  be added.
 * @param[in]       char_uuid : Characteristic UUID.
 * @param[in]       perm      : Characteristic value declaration attribute permission.
 * @param[in]       property  : Characteristic Properties
 * @param[in]       char_val    : Characteristic value
 * @param[in]       control : attribute response control byte
 *
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_add_char(uint16_t service_handle,  bk_bt_uuid_t *char_uuid,
                                 bk_gatt_perm_t perm, bk_gatt_char_prop_t property, bk_attr_value_t *char_val,
                                 bk_attr_control_t *control);


/**
 * @brief           This function is called to add characteristic descriptor. When
 *                  it's done, a callback event BK_GATTS_ADD_DESCR_EVT is called
 *                  to report the status and an ID number for this descriptor.
 *
 * @param[in]       service_handle: service handle to which this characteristic descriptor is to be added.
 * @param[in]       char_handle: which characteristic descriptor is to be added.
 * @param[in]       char_handle: which characteristic descriptor is to be added.
 * @param[in]       perm: descriptor access permission.
 * @param[in]       descr_uuid: descriptor UUID.
 * @param[in]       char_descr_val  : Characteristic descriptor value
 * @param[in]       control : attribute response control byte
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_add_char_descr (uint16_t service_handle,
                                        uint16_t char_handle,
                                        bk_bt_uuid_t   *descr_uuid,
                                        bk_gatt_perm_t perm, bk_attr_value_t *char_descr_val,
                                        bk_attr_control_t *control);


/**
 * @brief           This function is called to start a service.
 *
 * @param[in]       service_handle: the service handle to be started.
 *
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_start_service(uint16_t service_handle);


/**
 * @brief           Send indicate or notify to GATT client.
 *                  Set param need_confirm as false will send notification, otherwise indication.
 *
 * @param[in]       gatts_if: GATT server access interface
 * @param[in]       conn_id - connection handle.
 * @param[in]       attr_handle - attribute handle to indicate.
 * @param[in]       value_len - indicate value length.
 * @param[in]       value: value to indicate.
 * @param[in]       need_confirm - Whether a confirmation is required.
 *                  false sends a GATT notification, true sends a GATT indication.
 *
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_send_indicate(bk_gatt_if_t gatts_if, uint16_t conn_id, uint16_t attr_handle,
                                     uint16_t value_len, uint8_t *value, bool need_confirm);


/**
 * @brief           This function is called to send a response to a request.
 *
 * @param[in]       gatts_if: GATT server access interface
 * @param[in]       conn_id - connection handle.
 * @param[in]       trans_id - Transaction id
 * @param[in]       status - response status
 * @param[in]       rsp - response data.
 *
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_send_response(bk_gatt_if_t gatts_if, uint16_t conn_id, uint32_t trans_id,
                                      bk_gatt_status_t status, bk_gatt_rsp_t *rsp);


/**
 * @brief           This function is called to set the attribute value by the application
 *
 * @param[in]       attr_handle: the attribute handle which to be set
 * @param[in]       length: the value length
 * @param[in]       value: the pointer to the attribute value
 *
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_set_attr_value(uint16_t attr_handle, uint16_t length, const uint8_t *value);

/**
 * @brief       Retrieve attribute value
 *
 * @param[in]   attr_handle: Attribute handle.
 * @param[out]  length: pointer to the attribute value length
 * @param[out]  value:  Pointer to attribute value payload, the value cannot be modified by user
 *
 * @return
 *                  - BK_ERR_BLE_SUCCESS : success
 *                  - other  : failed
 *
 */
ble_err_t bk_ble_gatts_get_attr_value(uint16_t attr_handle, uint16_t *length, uint8_t **value);



///@}

///@}

#ifdef __cplusplus
}
#endif



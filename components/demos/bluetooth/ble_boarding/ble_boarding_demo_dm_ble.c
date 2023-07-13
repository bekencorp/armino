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


#include <stdio.h>
#include <stdlib.h>
#include "at_common.h"
#if CONFIG_BLE//(CONFIG_BLE_5_X || CONFIG_BTDM_5_2)
#include "at_ble_common.h"
#include "bk_wifi_wrapper.h"
#include "legacy_include/bluetooth_legacy_include.h"
#include "ble_boarding_demo.h"


static beken_semaphore_t ble_boarding_sema = NULL;
static ble_err_t s_at_cmd_status = BK_ERR_BLE_SUCCESS;
static uint8_t s_boarding_ssid[64];
static uint8_t s_boarding_password[32];
static uint16_t s_boarding_ssid_len = 0;
static uint16_t s_boarding_password_len = 0;
static uint8_t s_boarding_notify[1] = {0};
static uint8_t s_conn_ind = ~0;

#define GATT_BOARDING_SERVICE_UUID              0xFFFFU
#define GATT_BOARDING_NOTIFY_CHARACTERISTIC     0x1234U
#define GATT_BOARDING_SSID_CHARACTERISTIC       0x9ABCU
#define GATT_BOARDING_PASSWORD_CHARACTERISTIC   0xDEF0U

typedef struct
{
    uint16_t pri_service_handle;
    uint16_t chara_notify_handle;
    uint16_t chara_ssid_handle;
    uint16_t chara_pass_handle;
} boarding_env_s;

static boarding_env_s boarding_env;


static bk_err_t gatt_db_boarding_gatt_char_handler(uint8_t conn_handle, GATT_DB_HANDLE *handle, GATT_DB_PARAMS *params);




static void ble_at_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
    s_at_cmd_status = param->status;

    switch (cmd)
    {
    case BLE_CREATE_ADV:
    case BLE_SET_ADV_DATA:
    case BLE_SET_RSP_DATA:
    case BLE_START_ADV:
    case BLE_STOP_ADV:
    case BLE_CREATE_SCAN:
    case BLE_START_SCAN:
    case BLE_STOP_SCAN:
    case BLE_INIT_CREATE:
    case BLE_INIT_START_CONN:
    case BLE_INIT_STOP_CONN:
    case BLE_CONN_DIS_CONN:
    case BLE_CONN_UPDATE_PARAM:
    case BLE_DELETE_ADV:
    case BLE_DELETE_SCAN:
    case BLE_CONN_READ_PHY:
    case BLE_CONN_SET_PHY:
    case BLE_CONN_UPDATE_MTU:
    case BLE_SET_RANDOM_ADDR:
        if (ble_boarding_sema != NULL)
        {
            rtos_set_semaphore( &ble_boarding_sema );
        }

        break;

    default:
        break;
    }

}


static uint32_t dm_ble_event_cb(ble_event_enum_t notice, void *param)
{
    switch (notice)
    {
    case BK_DM_BLE_EVENT_STACK_OK:
        os_printf("ble stack ok\n");
        break;


    case BK_DM_BLE_EVENT_MTU_CHANGE:
    {
        ble_mtu_change_t *m_ind = (ble_mtu_change_t *)param;
        os_printf("%s m_ind:conn_idx:%d, mtu_size:%d\r\n", __func__, m_ind->conn_idx, m_ind->mtu_size);
        break;
    }

    case BK_DM_BLE_EVENT_CONNECT:
    {
        ble_conn_att_t *ind = (typeof(ind))param;
        s_conn_ind = ind->conn_handle;
        break;
    }

    case BK_DM_BLE_EVENT_DISCONNECT:
    {
        ble_conn_att_t *d_ind = (typeof(d_ind))param;
        os_printf("disconnect :conn_idx:%d,reason:%d\r\n", d_ind->conn_handle, d_ind->event_result);
        s_conn_ind = ~0;
        break;
    }

    case BK_DM_BLE_EVENT_CREATE_DB:
    {
        os_printf("BK_DM_BLE_EVENT_CREATE_DB OK\n");

        break;
    }

    case BK_DM_BLE_EVENT_TX_DONE:
        break;

    case BK_DM_BLE_EVENT_CONN_UPDATA:
    {
        ble_conn_update_param_compl_ind_t *updata_param = (typeof(updata_param))param;
        os_printf("BK_DM_BLE_EVENT_CONN_UPDATA:conn_interval:0x%04x, con_latency:0x%04x, sup_to:0x%04x\n",
                  updata_param->conn_interval, updata_param->conn_latency, updata_param->supervision_timeout);
        break;
    }

    default:
        break;
    }
    return 0;
}

int dm_ble_boarding_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int retval = kNoErr;
    char *msg = NULL;

    if (bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        retval = kParamErr;
        goto error;
    }

    if (argc != 2)
    {
        os_printf("\nThe number of param is wrong!\n");
        retval = kParamErr;
        goto error;
    }

    uint8_t adv_data[31] = {0};
    uint8_t adv_len = 0;
    ble_adv_parameter_t tmp_param;

    GATT_DB_SERVICE_INFO service_info;
    uint16_t num_attr_handles;
    uint16_t service_handle;
    GATT_DB_UUID_TYPE char_uuid;
    uint16_t perm;
    uint16_t property;
    ATT_VALUE char_value;
    UINT16 char_handle;


    retval = rtos_init_semaphore(&ble_boarding_sema, 1);

    if (retval != kNoErr)
    {
        goto error;
    }

    bk_ble_set_event_callback(dm_ble_event_cb);

    service_info.is_primary = 1;
    service_info.uuid.uuid_format = ATT_16_BIT_UUID_FORMAT;
    service_info.uuid.uuid.uuid_16 = GATT_BOARDING_SERVICE_UUID;
    service_info.link_req = GATT_DB_SER_SUPPORT_ANY_LINK_TYPE;
    service_info.sec_req = GATT_DB_SER_NO_SECURITY_PROPERTY;
    num_attr_handles = 30U;
    retval = bk_ble_gatt_db_add_service
             (
                 &service_info,
                 num_attr_handles,
                 &service_handle
             );

    if (0 != retval)
    {
        os_printf("%s: BT_gatt_db_add_service() failed. Result: 0x%04X\n", __func__, retval);
        goto error;
    }
    else
    {
        boarding_env.pri_service_handle = service_handle;
    }

    char_uuid.uuid_format = ATT_16_BIT_UUID_FORMAT;
    char_uuid.uuid.uuid_16 = GATT_BOARDING_NOTIFY_CHARACTERISTIC;
    perm = GATT_DB_PERM_READ;
    property = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY);
    char_value.val = s_boarding_notify;
    char_value.len = sizeof(s_boarding_notify);
    char_value.actual_len = char_value.len;
    retval = bk_ble_gatt_db_add_characteristic
             (
                 service_handle,
                 &char_uuid,
                 perm,
                 property,
                 &char_value,
                 &char_handle
             );

    if (0 != retval)
    {
        os_printf("%s: bk_ble_gatt_db_add_characteristic() failed. Result: 0x%04X\n", __func__, retval);
        goto error;
    }
    else
    {
        boarding_env.chara_notify_handle = char_handle;
    }

    GATT_DB_UUID_TYPE    desc_uuid;
    ATT_VALUE            desc_value;

    uint8_t cccd_value[2U]    = { 0x00U, 0x00U };

    desc_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    desc_uuid.uuid.uuid_16 = GATT_CLIENT_CONFIG;

    perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);

    desc_value.val         = cccd_value;
    desc_value.len         = 2U;
    desc_value.actual_len  = desc_value.len;

    /* Add descriptor CCCD */
    retval = bk_ble_gatt_db_add_characteristic_descriptor
             (
                 service_handle,
                 boarding_env.chara_notify_handle,
                 &desc_uuid,
                 perm,
                 &desc_value
             );

    if (0 != retval)
    {
        os_printf("%s: bk_ble_gatt_db_add_characteristic_descriptor() failed. Result: 0x%04X\n", __func__, retval);
        goto error;
    }

    char_uuid.uuid_format = ATT_16_BIT_UUID_FORMAT;
    char_uuid.uuid.uuid_16 = GATT_BOARDING_SSID_CHARACTERISTIC;
    perm = GATT_DB_PERM_READ | GATT_DB_PERM_WRITE;
    property = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY);
    char_value.val = s_boarding_ssid;
    char_value.len = sizeof(s_boarding_ssid);
    char_value.actual_len = char_value.len;
    retval = bk_ble_gatt_db_add_characteristic
             (
                 service_handle,
                 &char_uuid,
                 perm,
                 property,
                 &char_value,
                 &char_handle
             );

    if (0 != retval)
    {
        os_printf("%s: bk_ble_gatt_db_add_characteristic() failed. Result: 0x%04X\n", __func__, retval);
        goto error;
    }
    else
    {
        boarding_env.chara_ssid_handle = char_handle;
    }

    char_uuid.uuid_format = ATT_16_BIT_UUID_FORMAT;
    char_uuid.uuid.uuid_16 = GATT_BOARDING_PASSWORD_CHARACTERISTIC;
    perm = GATT_DB_PERM_READ | GATT_DB_PERM_WRITE;
    property = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY);
    char_value.val = s_boarding_password;
    char_value.len = sizeof(s_boarding_password);
    char_value.actual_len = char_value.len;
    retval = bk_ble_gatt_db_add_characteristic
             (
                 service_handle,
                 &char_uuid,
                 perm,
                 property,
                 &char_value,
                 &char_handle
             );

    if (0 != retval)
    {
        os_printf("%s: bk_ble_gatt_db_add_characteristic() failed. Result: 0x%04X\n", __func__, retval);
        goto error;
    }
    else
    {
        boarding_env.chara_pass_handle = char_handle;
    }

    retval = bk_ble_gatt_db_add_completed();

    if (retval != 0)
    {
        os_printf("%s GATT Database Registration err: 0x%04X\n", __func__, retval);
        goto error;
    }

    retval = bk_ble_gatt_db_set_callback(gatt_db_boarding_gatt_char_handler);

    if (retval != 0)
    {
        os_printf("%s bk_ble_gatt_db_set_callback err: 0x%04X\n", __func__, retval);
        goto error;
    }



    memset(&tmp_param, 0, sizeof(tmp_param));

    tmp_param.adv_intv_max = 160;
    tmp_param.adv_intv_min = 120;
    tmp_param.adv_type = ADV_LEGACY_TYPE_ADV_IND;
    tmp_param.chnl_map = ADV_ALL_CHNLS;
    tmp_param.filter_policy = ADV_FILTER_POLICY_ALLOW_SCAN_ANY_CONNECT_ANY;
    tmp_param.own_addr_type = 0;
    tmp_param.peer_addr_type = 0;
    //tmp_param.peer_addr;

    //    retval = bk_ble_set_advertising_params(adv_param.adv_intv_min, adv_param.adv_intv_max, adv_param.chnl_map,
    //            adv_param.own_addr_type,adv_param.prim_phy, adv_param.second_phy, ble_at_cmd_cb);

    retval = bk_ble_set_advertising_params(&tmp_param, ble_at_cmd_cb);

    if (retval != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }

    if (ble_boarding_sema != NULL)
    {
        retval = rtos_get_semaphore(&ble_boarding_sema, AT_SYNC_CMD_TIMEOUT_MS);

        if (retval != kNoErr)
        {
            goto error;
        }
    }

    bd_addr_t random_addr;
    bk_get_mac((uint8_t *)random_addr.addr, MAC_TYPE_BLUETOOTH);

    for (int i = 0; i < sizeof(random_addr.addr) / 2; i++)
    {
        uint8_t tmp_addr = random_addr.addr[i];
        random_addr.addr[i] = random_addr.addr[sizeof(random_addr.addr) - 1 - i];
        random_addr.addr[sizeof(random_addr.addr) - 1 - i] = tmp_addr;
    }

    random_addr.addr[0]++;

    retval = bk_ble_set_random_addr((bd_addr_t *)&random_addr, ble_at_cmd_cb);

    if (retval != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }

    if (ble_boarding_sema != NULL)
    {
        retval = rtos_get_semaphore(&ble_boarding_sema, AT_SYNC_CMD_TIMEOUT_MS);

        if (retval != kNoErr)
        {
            goto error;
        }
    }

    adv_len = os_strtoul(argv[1], NULL, 16) & 0xFF;

    if (adv_len > 31 || adv_len != os_strlen(argv[0]) / 2)
    {
        os_printf("input adv len over limited\n");
        retval = kParamErr;
        goto error;
    }

    at_set_data_handle(adv_data, argv[0],  os_strlen(argv[0]));
    retval = bk_ble_set_advertising_data(adv_len, adv_data, ble_at_cmd_cb);

    if (retval != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }

    if (ble_boarding_sema != NULL)
    {
        retval = rtos_get_semaphore(&ble_boarding_sema, AT_SYNC_CMD_TIMEOUT_MS);

        if (retval != kNoErr)
        {
            goto error;
        }
    }

    retval = bk_ble_set_advertising_enable(1, ble_at_cmd_cb);

    if (retval != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }

    if (ble_boarding_sema != NULL)
    {
        retval = rtos_get_semaphore(&ble_boarding_sema, AT_SYNC_CMD_TIMEOUT_MS);

        if (retval != kNoErr)
        {
            goto error;
        }
    }

    retval = kNoErr;
    msg = AT_CMD_RSP_SUCCEED;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return retval;

error:
    os_printf("%s failed. \n", __func__);
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));

    if (ble_boarding_sema != NULL)
    {
        rtos_deinit_semaphore(&ble_boarding_sema);
    }

    return retval;

}

static bk_err_t gatt_db_boarding_gatt_char_handler(uint8_t conn_handle, GATT_DB_HANDLE *handle, GATT_DB_PARAMS *params)
{
    UINT16 config;

    if (handle->service_id == boarding_env.pri_service_handle)
    {
        switch (params->db_op)
        {
        case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
        {
            if (handle->char_id == boarding_env.chara_notify_handle)
            {
                config = (((UINT16)(params->value.val[1])) << 8) | params->value.val[0];

                if (GATT_CLI_CNFG_NOTIFICATION == config)
                {
                    os_printf("client notify config open\r\n");
                }
                else if (GATT_CLI_CNFG_DEFAULT == config)
                {
                    os_printf("client notify config close\r\n");
                }
                else
                {
                    //nothing to do
                }
            }
        }
        break;

        case GATT_DB_CHAR_PEER_READ_REQ:
        {
            if (handle->char_id == boarding_env.chara_pass_handle)
            {
                ATT_VALUE param;
                bk_ble_gatt_get_char_val(handle, &param);
                os_printf("Borading read PASS:%s, %d \r\n", param.val, param.actual_len);
            }
            else if (handle->char_id == boarding_env.chara_ssid_handle)
            {
                ATT_VALUE param;
                bk_ble_gatt_get_char_val(handle, &param);
                os_printf("Borading read SSID:%s, %d \r\n", param.val, param.actual_len);
            }
            else
            {
                //nothing to do
            }
        }
        break;

        case GATT_DB_CHAR_PEER_WRITE_REQ:
        {
            if (handle->char_id == boarding_env.chara_pass_handle)
            {
                s_boarding_password_len = 0;
                os_memset((uint8_t *)s_boarding_password, 0, sizeof(s_boarding_password) / sizeof(s_boarding_password[0]));
                os_memcpy((uint8_t *)s_boarding_password, params->value.val, params->value.len);
                s_boarding_password_len = params->value.len;
                os_printf("Boarding write PASS:%s, %d, %d\r\n", s_boarding_password, s_boarding_password_len, params->value.actual_len);
#if CONFIG_WIFI_ENABLE
                demo_sta_app_init((char *)s_boarding_ssid, (char *)s_boarding_password);
#endif
            }
            else if (handle->char_id == boarding_env.chara_ssid_handle)
            {
                s_boarding_ssid_len = 0;
                os_memset((uint8_t *)s_boarding_ssid, 0, sizeof(s_boarding_ssid) / sizeof(s_boarding_ssid[0]));
                os_memcpy((uint8_t *)s_boarding_ssid, params->value.val, params->value.len);
                s_boarding_ssid_len = params->value.len;
                os_printf("Boarding write SSID:%s, %d, %d\r\n", s_boarding_ssid, s_boarding_ssid_len, params->value.actual_len);
            }
            else
            {
                //nothing to do
            }
        }
        break;

        default:
            //                os_printf(
            //                "No Specific Application Handling Required for Operation 0x%02X\n",
            //                params->db_op);
            break;
        }
    }

    return 0;
}


#endif


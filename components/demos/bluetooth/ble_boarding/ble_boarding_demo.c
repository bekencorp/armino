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

static beken_semaphore_t ble_boarding_sema = NULL;
ble_err_t s_at_cmd_status = BK_ERR_BLE_SUCCESS;
static uint8_t s_boarding_ssid[64];
static uint8_t s_boarding_password[32];
static uint16_t s_boarding_ssid_len = 0;
static uint16_t s_boarding_password_len = 0;
static uint8_t s_boarding_notify[1] = {0};

#define GATT_BOARDING_SERVICE_UUID              0xFFFFU
#define GATT_BOARDING_NOTIFY_CHARACTERISTIC     0x1234U
#define GATT_BOARDING_SSID_CHARACTERISTIC       0x9ABCU
#define GATT_BOARDING_PASSWORD_CHARACTERISTIC   0xDEF0U

typedef struct{
    uint16_t pri_service_handle;
    uint16_t chara_notify_handle;
    uint16_t chara_ssid_handle;
    uint16_t chara_pass_handle;
}boarding_env_s;

static boarding_env_s boarding_env;

static int ble_boarding_legacy_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int ble_boarding_ethermind_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static API_RESULT gatt_db_boarding_gatt_char_handler(GATT_DB_HANDLE    * handle,GATT_DB_PARAMS    * params);

uint8_t s_conn_ind = ~0;
//extern void register_app_sdp_charac_callback(void (*)(uint32_t type,uint8 conidx,uint16_t hdl,uint16_t len,uint8 *data));

enum {
    BOARDING_IDX_SVC,
    BOARDING_IDX_CHAR_DECL,
    BOARDING_IDX_CHAR_VALUE,

	BOARDING_IDX_CHAR_SSID_DECL,
	BOARDING_IDX_CHAR_SSID_VALUE,

	BOARDING_IDX_CHAR_PASSWORD_DECL,
	BOARDING_IDX_CHAR_PASSWORD_VALUE,

	BOARDING_IDX_NB,
};

#define DECL_PRIMARY_SERVICE_128     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define DECL_CHARACTERISTIC_128      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define DESC_CLIENT_CHAR_CFG_128     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

ble_attm_desc_t boarding_service_db[BOARDING_IDX_NB] = {
    //  Service Declaration
    [BOARDING_IDX_SVC]        = {DECL_PRIMARY_SERVICE_128, BK_BLE_PERM_SET(RD, ENABLE), 0, 0},

    [BOARDING_IDX_CHAR_DECL]  = {DECL_CHARACTERISTIC_128,  BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
    // Characteristic Value
    [BOARDING_IDX_CHAR_VALUE] = {{0x34, 0x12, 0}, BK_BLE_PERM_SET(NTF, ENABLE), BK_BLE_PERM_SET(RI, ENABLE) | BK_BLE_PERM_SET(UUID_LEN, UUID_16), 128},

    //ssid
    [BOARDING_IDX_CHAR_SSID_DECL]  = {DECL_CHARACTERISTIC_128, BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
    [BOARDING_IDX_CHAR_SSID_VALUE] = {{0xbc, 0x9a, 0}, BK_BLE_PERM_SET(WRITE_REQ, ENABLE) | BK_BLE_PERM_SET(RD, ENABLE), BK_BLE_PERM_SET(RI, ENABLE) | BK_BLE_PERM_SET(UUID_LEN, UUID_16), 128},

    //password
    [BOARDING_IDX_CHAR_PASSWORD_DECL]    = {DECL_CHARACTERISTIC_128, BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
    [BOARDING_IDX_CHAR_PASSWORD_VALUE]   = {{0xf0, 0xde, 0}, BK_BLE_PERM_SET(WRITE_REQ, ENABLE) | BK_BLE_PERM_SET(RD, ENABLE), BK_BLE_PERM_SET(RI, ENABLE) | BK_BLE_PERM_SET(UUID_LEN, UUID_16), 128},

};

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
            if (ble_boarding_sema != NULL)
                rtos_set_semaphore( &ble_boarding_sema );
            break;
        default:
            break;
    }

}

static void ble_at_notice_cb(ble_notice_t notice, void *param)
{
    switch (notice) {
    case BLE_5_STACK_OK:
        os_printf("ble stack ok");
        break;
    case BLE_5_WRITE_EVENT: {
        ble_write_req_t *w_req = (ble_write_req_t *)param;
        os_printf("write_cb:conn_idx:%d, prf_id:%d, att_idx:%d, len:%d, data[0]:0x%02x\r\n",
                w_req->conn_idx, w_req->prf_id, w_req->att_idx, w_req->len, w_req->value[0]);
//#if (CONFIG_BTDM_5_2)

		if (bk_ble_get_controller_stack_type() == BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2
            && w_req->prf_id == PRF_TASK_ID_BOARDING) {
			switch(w_req->att_idx)
            {
            case BOARDING_IDX_CHAR_DECL:
                break;
            case BOARDING_IDX_CHAR_VALUE:
                break;

            case BOARDING_IDX_CHAR_SSID_DECL:
                break;
            case BOARDING_IDX_CHAR_SSID_VALUE:
				s_boarding_ssid_len = 0;
				os_memset((uint8_t *)s_boarding_ssid, 0, sizeof(s_boarding_ssid)/sizeof(s_boarding_ssid[0]));
                os_memcpy((uint8_t *)s_boarding_ssid, w_req->value, w_req->len);
				s_boarding_ssid_len = w_req->len;
                break;

            case BOARDING_IDX_CHAR_PASSWORD_DECL:
                break;
            case BOARDING_IDX_CHAR_PASSWORD_VALUE:
				s_boarding_password_len = 0;
				os_memset((uint8_t *)s_boarding_password, 0, sizeof(s_boarding_password)/sizeof(s_boarding_password[0]));
                os_memcpy((uint8_t *)s_boarding_password, w_req->value, w_req->len);
				s_boarding_password_len = w_req->len;
				demo_sta_app_init((char *)s_boarding_ssid, (char *)s_boarding_password);
				break;

            default:
                break;
            }
        }
//#endif
        break;
    }
    case BLE_5_READ_EVENT: {
        ble_read_req_t *r_req = (ble_read_req_t *)param;
        os_printf("read_cb:conn_idx:%d, prf_id:%d, att_idx:%d\r\n",
                r_req->conn_idx, r_req->prf_id, r_req->att_idx);

        if (r_req->prf_id == PRF_TASK_ID_BOARDING) {
            switch(r_req->att_idx)
            {
                case BOARDING_IDX_CHAR_DECL:
                    break;
                case BOARDING_IDX_CHAR_VALUE:
                    break;

                case BOARDING_IDX_CHAR_SSID_DECL:
                    break;
                case BOARDING_IDX_CHAR_SSID_VALUE:
                    bk_ble_read_response_value(s_boarding_ssid_len, s_boarding_ssid, r_req->prf_id, r_req->att_idx);
                    os_printf("len:%d, data[0]:0x%02x, data[1]:0x%02x, data[2]:0x%02x\r\n", s_boarding_ssid_len, s_boarding_ssid[0], s_boarding_ssid[1], s_boarding_ssid[2]);
                    break;

                case BOARDING_IDX_CHAR_PASSWORD_DECL:
                    break;
                case BOARDING_IDX_CHAR_PASSWORD_VALUE:
                    bk_ble_read_response_value(s_boarding_password_len, s_boarding_password, r_req->prf_id, r_req->att_idx);
                    os_printf("len:%d, data[0]:0x%02x, data[1]:0x%02x, data[2]:0x%02x\r\n", s_boarding_password_len, s_boarding_password[0], s_boarding_password[1], s_boarding_password[2]);
                    break;

                default:
                    break;
            }
        }
        break;
    }
    case BLE_5_REPORT_ADV: {
        ble_recv_adv_t *r_ind = (ble_recv_adv_t *)param;
        uint8_t adv_type = r_ind->evt_type & 0x03;
        os_printf("r_ind:actv_idx:%d,", r_ind->actv_idx);
        switch (adv_type)
        {
            case 0:
                os_printf("evt_type:EXT_ADV,");
                break;
            case 1:
                os_printf("evt_type:LEG_ADV,");
                break;
            case 4:
                os_printf("evt_type:PER_ADV,");
                break;
            default:
                os_printf("evt_type:ERR_ADV,");
                break;
        }
        os_printf(" adv_addr_type:%d, adv_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                r_ind->adv_addr_type, r_ind->adv_addr[0], r_ind->adv_addr[1], r_ind->adv_addr[2],
                r_ind->adv_addr[3], r_ind->adv_addr[4], r_ind->adv_addr[5]);
        break;
    }
    case BLE_5_MTU_CHANGE: {
        ble_mtu_change_t *m_ind = (ble_mtu_change_t *)param;
        os_printf("%s m_ind:conn_idx:%d, mtu_size:%d\r\n", __func__, m_ind->conn_idx, m_ind->mtu_size);
        break;
    }
    case BLE_5_CONNECT_EVENT: {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        os_printf("c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
        s_conn_ind = c_ind->conn_idx;
        break;
    }
    case BLE_5_DISCONNECT_EVENT: {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        os_printf("d_ind:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx, d_ind->reason);
        s_conn_ind = ~0;
        break;
    }
    case BLE_5_ATT_INFO_REQ: {
        ble_att_info_req_t *a_ind = (ble_att_info_req_t *)param;
        os_printf("a_ind:conn_idx:%d\r\n", a_ind->conn_idx);
        a_ind->length = 128;
        a_ind->status = BK_ERR_BLE_SUCCESS;
        break;
    }
    case BLE_5_CREATE_DB: {
        ble_create_db_t *cd_ind = (ble_create_db_t *)param;
        os_printf("cd_ind:prf_id:%d, status:%d\r\n", cd_ind->prf_id, cd_ind->status);
//#if (CONFIG_BTDM_5_2)
        if(bk_ble_get_controller_stack_type() == BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
        {
            s_at_cmd_status = cd_ind->status;
            if (ble_boarding_sema != NULL)
                rtos_set_semaphore( &ble_boarding_sema );
        }
//#endif
        break;
    }
    case BLE_5_INIT_CONNECT_EVENT: {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        os_printf("BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
        break;
    }
    case BLE_5_INIT_DISCONNECT_EVENT: {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        os_printf("BLE_5_INIT_DISCONNECT_EVENT:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx, d_ind->reason);
        break;
    }
    case BLE_5_SDP_REGISTER_FAILED:
        os_printf("BLE_5_SDP_REGISTER_FAILED\r\n");
        break;
    case BLE_5_READ_PHY_EVENT: {
        ble_read_phy_t *phy_param = (ble_read_phy_t *)param;
        os_printf("BLE_5_READ_PHY_EVENT:tx_phy:0x%02x, rx_phy:0x%02x\r\n",phy_param->tx_phy, phy_param->rx_phy);
        break;
    }
    case BLE_5_TX_DONE:
        break;
    case BLE_5_CONN_UPDATA_EVENT: {
        ble_conn_param_t *updata_param = (ble_conn_param_t *)param;
        os_printf("BLE_5_CONN_UPDATA_EVENT:conn_interval:0x%04x, con_latency:0x%04x, sup_to:0x%04x\r\n", updata_param->intv_max,
        updata_param->con_latency, updata_param->sup_to);
        break;
    }
    case BLE_5_PAIRING_REQ:
        bk_printf("BLE_5_PAIRING_REQ\r\n");
        ble_smp_ind_t *s_ind = (ble_smp_ind_t *)param;
        bk_ble_sec_send_auth_mode(s_ind->conn_idx, GAP_AUTH_REQ_NO_MITM_BOND, GAP_IO_CAP_NO_INPUT_NO_OUTPUT,
            GAP_SEC1_NOAUTH_PAIR_ENC, GAP_OOB_AUTH_DATA_NOT_PRESENT);
        break;

    case BLE_5_PARING_PASSKEY_REQ:
        bk_printf("BLE_5_PARING_PASSKEY_REQ\r\n");
        break;

    case BLE_5_ENCRYPT_EVENT:
        bk_printf("BLE_5_ENCRYPT_EVENT\r\n");
        break;

    case BLE_5_PAIRING_SUCCEED:
        bk_printf("BLE_5_PAIRING_SUCCEED\r\n");
        break;
    default:
        break;
    }
}

int ble_boarding_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        return ble_boarding_legacy_handle(pcWriteBuffer, xWriteBufferLen, argc, argv);
    }else
    {
        return ble_boarding_ethermind_handle(pcWriteBuffer, xWriteBufferLen, argc, argv);
    }
}


int ble_boarding_legacy_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = kNoErr;
	char *msg = NULL;
	uint16 service_uuid = 0xFFFF;
	uint16 char_uuid = 0x1234;
	struct bk_ble_db_cfg ble_db_cfg;

	int actv_idx = 0;
	uint8_t adv_data[31];
	uint8_t adv_len = 0;

	ble_adv_param_t adv_param;

	if (argc != 2) {
		os_printf("\nThe number of param is wrong!\n");
		err = kParamErr;
		goto error;
	}

	err = rtos_init_semaphore(&ble_boarding_sema, 1);
    if(err != kNoErr){
        goto error;
    }

	os_memcpy(&(boarding_service_db[BOARDING_IDX_CHAR_VALUE].uuid[0]), &char_uuid, 2);
	os_memset(&ble_db_cfg, 0, sizeof(ble_db_cfg));

	ble_db_cfg.att_db = (ble_attm_desc_t *)boarding_service_db;
	ble_db_cfg.att_db_nb = BOARDING_IDX_NB;
	ble_db_cfg.prf_task_id = PRF_TASK_ID_BOARDING;
	ble_db_cfg.start_hdl = 0;
	ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16);
	os_memcpy(&(ble_db_cfg.uuid[0]), &service_uuid, 2);

	bk_ble_set_notice_cb(ble_at_notice_cb);
//	register_app_sdp_charac_callback(ble_sdp_charac_callback);
	err = bk_ble_create_db(&ble_db_cfg);
	if (err != BK_ERR_BLE_SUCCESS) {
		goto error;
	} else {
        err = rtos_get_semaphore(&ble_boarding_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if (err != kNoErr){
            goto error;
        } else {
            if (s_at_cmd_status == BK_ERR_BLE_SUCCESS) {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
            } else {
                err = s_at_cmd_status;
                s_at_cmd_status = BK_ERR_BLE_SUCCESS;
                goto error;
            }
        }
    }
	os_printf("creat boarding service successfully!\r\n");

	os_memset(&adv_param, 0, sizeof(ble_adv_param_t));
	adv_param.chnl_map = 7;
	adv_param.adv_intv_min = 120;
	adv_param.adv_intv_max = 160;
	adv_param.own_addr_type = BLE_STATIC_ADDR;
	adv_param.adv_type = 0;
	adv_param.adv_prop = 3;
	adv_param.prim_phy = 1;
	adv_param.second_phy = 1;

	err = bk_ble_create_advertising(actv_idx, &adv_param, ble_at_cmd_cb);
	if (err != BK_ERR_BLE_SUCCESS) {
		goto error;
	} else {
        err = rtos_get_semaphore(&ble_boarding_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if (err != kNoErr){
            goto error;
        } else {
            if (s_at_cmd_status == BK_ERR_BLE_SUCCESS) {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
            } else {
                err = s_at_cmd_status;
                s_at_cmd_status = BK_ERR_BLE_SUCCESS;
                goto error;
            }
        }
    }
	os_printf("set adv param and creat adv successfully!\r\n");

	adv_len = os_strtoul(argv[1], NULL, 16) & 0xFF;
	if (adv_len > 31 || adv_len != os_strlen(argv[0]) / 2)
	{
		os_printf("input adv len over limited\n");
		err = kParamErr;
		goto error;
	}
	at_set_data_handle(adv_data, argv[0],  os_strlen(argv[0]));

	err = bk_ble_set_adv_data(actv_idx, adv_data, adv_len, ble_at_cmd_cb);
	if (err != BK_ERR_BLE_SUCCESS) {
	   goto error;
	} else {
        err = rtos_get_semaphore(&ble_boarding_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if (err != kNoErr){
            goto error;
        } else {
            if (s_at_cmd_status == BK_ERR_BLE_SUCCESS) {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
            } else {
                err = s_at_cmd_status;
                s_at_cmd_status = BK_ERR_BLE_SUCCESS;
                goto error;
            }
        }
    }
	os_printf("set adv data successfully!\r\n");

	err = bk_ble_start_advertising(actv_idx, 0, ble_at_cmd_cb);
	if (err != BK_ERR_BLE_SUCCESS) {
	   goto error;
	} else {
		if (s_at_cmd_status == BK_ERR_BLE_SUCCESS)
		{
			msg = AT_CMD_RSP_SUCCEED;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			rtos_deinit_semaphore(&ble_boarding_sema);
			os_printf("start adv successfully!\r\n");
			return err;
		}
		else
		{
			err = s_at_cmd_status;
			s_at_cmd_status = BK_ERR_BLE_SUCCESS;
			goto error;
		}
	}


error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	if (ble_boarding_sema != NULL)
		rtos_deinit_semaphore(&ble_boarding_sema);
	return err;
}

static int ble_boarding_ethermind_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int retval = kNoErr;
    char *msg = NULL;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        retval = kParamErr;
        goto error;
    }

    if (argc != 2) {
        os_printf("\nThe number of param is wrong!\n");
        retval = kParamErr;
        goto error;
    }

    uint8_t adv_data[31];
    uint8_t adv_len = 0;

    ble_adv_param_t adv_param;

    GATT_DB_SERVICE_INFO service_info;
    uint16_t num_attr_handles;
    uint16_t service_handle;
    GATT_DB_UUID_TYPE char_uuid;
    uint16_t perm;
    uint16_t property;
    ATT_VALUE char_value;
    UINT16 char_handle;

    retval = rtos_init_semaphore(&ble_boarding_sema, 1);
    if(retval != kNoErr){
        goto error;
    }

    bk_ble_set_notice_cb(ble_at_notice_cb);

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
    }else
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
    }else
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
    }else
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
    }else
    {
        boarding_env.chara_pass_handle = char_handle;
    }

    retval = bk_ble_gatt_db_dyn_register();

    if (retval != 0)
    {
        os_printf("%s GATT Database Registration err: 0x%04X\n", __func__, retval);
        goto error;
    }

    retval = bk_ble_gatt_db_init_pl(gatt_db_boarding_gatt_char_handler);

    if (retval != 0)
    {
        os_printf("%s bk_ble_gatt_db_init_pl err: 0x%04X\n", __func__, retval);
        goto error;
    }

    os_memset(&adv_param, 0, sizeof(ble_adv_param_t));
	adv_param.chnl_map = 7;
	adv_param.adv_intv_min = 120;
	adv_param.adv_intv_max = 160;
	adv_param.own_addr_type = BLE_STATIC_ADDR;
	adv_param.adv_type = 0;
	adv_param.adv_prop = 3;
	adv_param.prim_phy = 1;
	adv_param.second_phy = 1;

    retval = bk_ble_set_advertising_params(adv_param.adv_intv_min, adv_param.adv_intv_max, adv_param.chnl_map,
            adv_param.own_addr_type,adv_param.prim_phy, adv_param.second_phy, ble_at_cmd_cb);

    if (retval != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }

    if (ble_boarding_sema != NULL)
    {
        retval = rtos_get_semaphore(&ble_boarding_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if (retval != kNoErr)
            goto error;
    }

    adv_len = os_strtoul(argv[1], NULL, 16) & 0xFF;
	if (adv_len > 31 || adv_len != os_strlen(argv[0]) / 2)
	{
		os_printf("input adv len over limited\n");
		retval = kParamErr;
		goto error;
	}
	at_set_data_handle(adv_data, argv[0],  os_strlen(argv[0]));
    retval = bk_ble_set_advertising_data(adv_data, adv_len, ble_at_cmd_cb);
    if (retval != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }

    if(ble_boarding_sema != NULL)
    {
        retval = rtos_get_semaphore(&ble_boarding_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(retval != kNoErr)
            goto error;
    }

    retval = bk_ble_set_advertising_enable(1, ble_at_cmd_cb);
    if (retval != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }

    if(ble_boarding_sema != NULL)
    {
        retval = rtos_get_semaphore(&ble_boarding_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(retval != kNoErr)
            goto error;
    }

    retval = kNoErr;
    msg = AT_CMD_RSP_SUCCEED;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return retval;

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_boarding_sema != NULL)
        rtos_deinit_semaphore(&ble_boarding_sema);
    return retval;

}

static API_RESULT gatt_db_boarding_gatt_char_handler(GATT_DB_HANDLE    * handle,GATT_DB_PARAMS    * params)
{
    UINT16 config;
    if(handle->service_id == boarding_env.pri_service_handle)
    {
        switch (params->db_op)
        {
            case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
            {
                if(handle->char_id == boarding_env.chara_notify_handle)
                {
                    config = (((UINT16)(params->value.val[1]))<<8) | params->value.val[0];
                    if (GATT_CLI_CNFG_NOTIFICATION == config)
                    {
                        os_printf("client notify config open\r\n");
                    }else if(GATT_CLI_CNFG_DEFAULT == config)
                    {
                        os_printf("client notify config close\r\n");
                    }else
                    {
                        //nothing to do
                    }
                }
            }
            break;

            case GATT_DB_CHAR_PEER_READ_REQ:
            {
                if(handle->char_id == boarding_env.chara_pass_handle)
                {
                    ATT_VALUE param;
                    bk_ble_gatt_get_char_val(handle, &param);
                    os_printf("Borading read PASS:%s, %d \r\n",param.val, param.actual_len);
                }else if(handle->char_id == boarding_env.chara_ssid_handle)
                {
                    ATT_VALUE param;
                    bk_ble_gatt_get_char_val(handle, &param);
                    os_printf("Borading read SSID:%s, %d \r\n",param.val, param.actual_len);
                }else
                {
                    //nothing to do
                }
            }
            break;

            case GATT_DB_CHAR_PEER_WRITE_REQ:
            {
                if(handle->char_id == boarding_env.chara_pass_handle)
                {
                    s_boarding_password_len = 0;
                    os_memset((uint8_t *)s_boarding_password, 0, sizeof(s_boarding_password)/sizeof(s_boarding_password[0]));
                    os_memcpy((uint8_t *)s_boarding_password, params->value.val, params->value.len);
                    s_boarding_password_len = params->value.len;
                    os_printf("Boarding write PASS:%s, %d, %d\r\n",s_boarding_password, s_boarding_password_len, params->value.actual_len);
                    demo_sta_app_init((char *)s_boarding_ssid, (char *)s_boarding_password);
                }else if(handle->char_id == boarding_env.chara_ssid_handle)
                {
                    s_boarding_ssid_len = 0;
                    os_memset((uint8_t *)s_boarding_ssid, 0, sizeof(s_boarding_ssid)/sizeof(s_boarding_ssid[0]));
                    os_memcpy((uint8_t *)s_boarding_ssid, params->value.val, params->value.len);
                    s_boarding_ssid_len = params->value.len;
                    os_printf("Boarding write SSID:%s, %d, %d\r\n",s_boarding_ssid, s_boarding_ssid_len, params->value.actual_len);
                }else
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


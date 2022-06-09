#include <stdio.h>
#include <stdlib.h>
#include "at_common.h"


#if CONFIG_BLE//(CONFIG_BLE_5_X || CONFIG_BTDM_5_2)
#include "at_ble_common.h"

enum {
    TEST_IDX_SVC,
    TEST_IDX_CHAR_DECL,
    TEST_IDX_CHAR_VALUE,
    TEST_IDX_CHAR_DESC,

    TEST_IDX_CHAR_DATALEN_DECL,
    TEST_IDX_CHAR_DATALEN_VALUE,

    TEST_IDX_CHAR_INTER_DECL,
    TEST_IDX_CHAR_INTER_VALUE,

    TEST_IDX_NB,
};


static beken_semaphore_t ble_at_cmd_sema = NULL;
ble_err_t at_cmd_status = BK_ERR_BLE_SUCCESS;
uint8 g_test_prf_task_id = 0;
uint8 g_test_noti_att_id = 0;

//#if CONFIG_BTDM_5_2
static beken_timer_t ble_noti_tmr;
static beken_timer_t ble_performance_tx_statistics_tmr;
static beken_timer_t ble_performance_rx_statistics_tmr;
static uint32 s_test_noti_count = 0;
//#endif
static uint16_t s_test_data_len = 20;
static uint32_t s_test_send_inter = 500;

static uint8_t s_service_type = 0;


uint8_t s_test_conn_ind = ~0;

static uint32_t s_performance_tx_bytes = 0;
static uint32_t s_performance_rx_bytes = 0;
static uint8_t s_performance_tx_enable = 0;

int set_ble_name_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int get_ble_name_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_set_adv_param_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_set_adv_data_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_set_per_adv_data_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_set_scan_rsp_data_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_set_adv_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_set_scan_param_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_set_scan_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_create_connect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_cancel_create_connect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_disconnect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_update_conn_param_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_get_conn_state_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_get_local_addr_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_set_local_addr_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);




static int ble_register_service_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_register_noti_service_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_register_performance_service_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_enable_performance_statistic_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_tx_test_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_tx_test_param_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_update_mtu_2_max_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void ble_test_service_write_handle(uint8 val);
int ble_read_phy_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_set_phy_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_set_max_mtu_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
//#endif
int ble_delete_adv_activity_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_delete_scan_activity_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern int ble_boarding_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void ble_test_noti_hdl(void *param);
void ble_performance_tx_timer_hdl(void *param);
void ble_performance_rx_timer_hdl(void *param);
int ble_connect_by_name_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_disconnect_by_name_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_create_periodic_sync_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_start_periodic_sync_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_stop_periodic_sync_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

const at_command_t ble_at_cmd_table[] = {
    {0, "SETBLENAME", 0, "return ble name", set_ble_name_handle},
    {1, "GETBLENAME", 0, "return ble name", get_ble_name_handle},
    {2, "SETADVPARAM", 1, "help", ble_set_adv_param_handle},
    {3, "SETADVDATA", 1, "set adv data", ble_set_adv_data_handle},
    {4, "SETPERADVDATA", 1, "set perodic adv data", ble_set_per_adv_data_handle},
    {5, "SETSCANRSPDATA", 1, "set scan response data", ble_set_scan_rsp_data_handle},
    {6, "SETADVENABLE", 1, "set adv enable(1)/disable(0)", ble_set_adv_enable_handle},
    {7, "SETSCANPARAM", 1, "set scan param", ble_set_scan_param_handle},
    {8, "SETSCANENABLE", 1, "set scan enable(1)/disable(0)", ble_set_scan_enable_handle},

    {9, "CREATECONNECT", 1, "create connection", ble_create_connect_handle},
    {10, "CANCELCONNECT", 1, "cancel create connection", ble_cancel_create_connect_handle},
    {11, "DISCONNECT", 1, "disconnect current connection", ble_disconnect_handle},
    {12, "UPDATECONNPARAM", 1, "update connection param", ble_update_conn_param_handle},

    {13, "GETCONNECTSTATE", 0, "get connection state", ble_get_conn_state_handle},
    {14, "GETLOCALADDR", 0, "get ble local address", ble_get_local_addr_handle},
    {15, "SETLOCALADDR", 0, "set ble local address", ble_set_local_addr_handle},

    {16, "REGISTERSERVICE", 1, "register a service", ble_register_noti_service_handle},
    {17, "READPHY", 1, "read current tx/rx phy", ble_read_phy_handle},
    {18, "SETPHY", 1, "set current tx/rx phy", ble_set_phy_handle},

    {19, "DELETEADV", 1, "delete adv activity", ble_delete_adv_activity_handle},
    {20, "DELETESCAN", 1, "delete scan activity", ble_delete_scan_activity_handle},

    {21, "REGPERFORMANCESERVICE", 1, "register performance service: <task id> <service uuid> <char uuid> [data_len uuid] [interv uuid]",
        ble_register_performance_service_handle},
    {22, "SETTXTESTPARAM", 0, "set tx test param: <data_len> <interv>", ble_tx_test_param_handle},
    {23, "SETTXTESTENABLE", 0, "enable tx test: <1|0>", ble_tx_test_enable_handle},
    {24, "UPDATEMTU2MAX", 1, "update mtu 2 max: <addr>", ble_update_mtu_2_max_handle},
    {25, "BOARDING", 0, "boarding", ble_boarding_handle},
    {26, "ENABLEPERFORMANCESTATISTIC", 1, "enable performance statistic, <tx|rx> <1|0>",
        ble_enable_performance_statistic_handle},
    {27, "SETMAXMTU", 0, "set att maximal mtu", ble_set_max_mtu_handle},
    {28, "CONNECTBYNAME", 1, "create connection by name", ble_connect_by_name_handle},
    {29, "DISCONNECTBYNAME", 1, "disconnect current connection by name", ble_disconnect_by_name_handle},

    {30, "CREATEPERIODICSYNC", 1, "create periodic sync", ble_create_periodic_sync_handle},
    {31, "STARTPERIODICSYNC", 1, "set periodic sync start", ble_start_periodic_sync_handle},
    {32, "STOPPERIODICSYNC", 1, "set periodic sync stop", ble_stop_periodic_sync_handle},

//#endif
};

int ble_at_cmd_cnt(void)
{
    return sizeof(ble_at_cmd_table) / sizeof(ble_at_cmd_table[0]);
}

void ble_at_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
    at_cmd_status = param->status;
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
        case BLE_CREATE_PERIODIC:
        case BLE_START_PERIODIC:
        case BLE_STOP_PERIODIC:
        case BLE_DELETE_PERIODIC:
            if (ble_at_cmd_sema != NULL)
                rtos_set_semaphore( &ble_at_cmd_sema );
            break;
        default:
            break;
    }

}

enum {
    STATE_IDLE,
    STATE_DISCOVERING,
    STATE_DISCOVERED,
    STATE_CONNECTINIG,
    STATE_CONNECTED,
    STATE_DISCONNECTINIG,
};

typedef struct
{
    uint8_t len;
    uint8_t name[50];
} ble_device_name_t;

typedef struct
{
    uint8_t state;
    uint8_t addr_type;
    bd_addr_t bdaddr;
    ble_device_name_t dev;
} ble_device_info_t;

ble_device_info_t g_peer_dev;

static uint8_t ble_check_device_name(uint8_t* p_buf, uint8_t data_len, ble_device_name_t* dev_name)
{
    uint8_t* p_data = p_buf;
    uint8_t* p_data_end = p_buf + data_len;
    uint8_t name_len = 0;

    uint8_t is_found = 0;

    while (p_data < p_data_end)
    {
        if (*(p_data + 1) == 0x09)//GAP_AD_TYPE_COMPLETE_NAME
        {
            name_len = *p_data - 1;

            if ((name_len == dev_name->len) && (!os_memcmp(dev_name->name, p_data + 2, name_len)))
            {
                is_found = 1;
            }
            break;
        }

        // Go to next advertising info
        p_data += (*p_data + 1);
    }

    return (is_found);
}

void ble_at_notice_cb(ble_notice_t notice, void *param)
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
            && w_req->prf_id == g_test_prf_task_id) {
            switch(w_req->att_idx)
            {
            case TEST_IDX_CHAR_DECL:
                break;
            case TEST_IDX_CHAR_VALUE:
                break;
            case TEST_IDX_CHAR_DESC:
                g_test_noti_att_id = TEST_IDX_CHAR_VALUE;
                ble_test_service_write_handle(w_req->value[0]);
                break;

            case TEST_IDX_CHAR_DATALEN_DECL:
                break;
            case TEST_IDX_CHAR_DATALEN_VALUE:
                os_memcpy(&s_test_data_len, w_req->value, sizeof(s_test_data_len));
                break;

            case TEST_IDX_CHAR_INTER_DECL:
                break;
            case TEST_IDX_CHAR_INTER_VALUE:
                os_memcpy(&s_test_send_inter, w_req->value, sizeof(s_test_send_inter));
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

		if (r_req->prf_id == g_test_prf_task_id) {
		    switch(r_req->att_idx)
		    {
			    case TEST_IDX_CHAR_DECL:
			        break;
			    case TEST_IDX_CHAR_VALUE:
			        break;
			    case TEST_IDX_CHAR_DESC:
			        r_req->value[0] = 0x11;
			        r_req->value[1] = 0x22;
			        r_req->value[2] = 0x33;
			        r_req->length = 3;
			        break;

			    case TEST_IDX_CHAR_DATALEN_DECL:

			        break;
			    case TEST_IDX_CHAR_DATALEN_VALUE:
			        r_req->length = sizeof(s_test_data_len);
			        os_memcpy(r_req->value, &s_test_data_len, sizeof(s_test_data_len));
			        break;

			    case TEST_IDX_CHAR_INTER_DECL:

			        break;
			    case TEST_IDX_CHAR_INTER_VALUE:
			        r_req->length = sizeof(s_test_send_inter);
			        os_memcpy(r_req->value, &s_test_send_inter, sizeof(s_test_send_inter));
			        break;

			    default:
			        break;
		    }
		}
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

        if ((STATE_DISCOVERING == g_peer_dev.state) && (ble_check_device_name(r_ind->data , r_ind->data_len, &(g_peer_dev.dev))))
        {
            os_printf("dev : %s is discovered\r\n",g_peer_dev.dev.name);
            os_memcpy(g_peer_dev.bdaddr.addr, r_ind->adv_addr, BK_BLE_GAP_BD_ADDR_LEN);
            g_peer_dev.addr_type = r_ind->adv_addr_type;
            g_peer_dev.state = STATE_DISCOVERED;
            if (ble_at_cmd_sema != NULL)
                rtos_set_semaphore( &ble_at_cmd_sema );
        }

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
        s_test_conn_ind = c_ind->conn_idx;
        break;
    }
    case BLE_5_DISCONNECT_EVENT: {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        os_printf("d_ind:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx, d_ind->reason);
        s_test_conn_ind = ~0;
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
            at_cmd_status = cd_ind->status;
            if (ble_at_cmd_sema != NULL)
                rtos_set_semaphore( &ble_at_cmd_sema );
        }
//#endif
        break;
    }
    case BLE_5_INIT_CONNECT_EVENT: {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        os_printf("BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);

        if ((STATE_CONNECTINIG == g_peer_dev.state) && (!os_memcmp(g_peer_dev.bdaddr.addr, c_ind->peer_addr, BK_BLE_GAP_BD_ADDR_LEN)))
        {
            g_peer_dev.state = STATE_CONNECTED;
        }
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
        if(s_service_type && s_performance_tx_enable == 1)
        {
            s_performance_tx_bytes += s_test_data_len;
            ble_test_noti_hdl(NULL);
        }
        break;
    case BLE_5_CONN_UPDATA_EVENT: {
        ble_conn_param_t *updata_param = (ble_conn_param_t *)param;
        os_printf("BLE_5_CONN_UPDATA_EVENT:conn_interval:0x%04x, con_latency:0x%04x, sup_to:0x%04x\r\n", updata_param->intv_max,
        updata_param->con_latency, updata_param->sup_to);
        break;
    }
    case BLE_5_PERIODIC_SYNC_CMPL_EVENT:
        os_printf("BLE_5_PERIODIC_SYNC_CMPL_EVENT \n");
        break;
    default:
        break;
    }
}

int set_ble_name_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    //uint8_t name[] = "BK_BLE_7231n";
    uint8_t name_len = 0;/*os_strlen((const char *)name);*/
    int err = kNoErr;
    if (argc != 1)
    {
        err = kParamErr;
        goto error;
    }

    name_len = bk_ble_appm_set_dev_name(os_strlen(argv[0]), (uint8_t *)argv[0]);
    if (name_len == 0)
    {
        os_printf("\nname is empty!!!\n");
        goto error;
    }

    msg = AT_CMD_RSP_SUCCEED;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return kNoErr;

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return err;
}


int get_ble_name_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint8_t name[BK_BLE_APP_DEVICE_NAME_MAX_LEN] = {0};
    uint8_t name_len = 0;
    int err = kNoErr;
    if (argc != 0)
    {
        err = kParamErr;
        goto error;
    }

    name_len = bk_ble_appm_get_dev_name(name, BK_BLE_APP_DEVICE_NAME_MAX_LEN);
    if (name_len == 0)
    {
        os_printf("\nname is empty!!!\n");
        goto error;
    }

    sprintf(pcWriteBuffer, "%s%s", AT_CMDRSP_HEAD, name);
    return kNoErr;

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return err;
}

//AT+BLECMD=SETADVPARAM, map, min_intval, max_intval, local_addr_type, adv_type, adv_properties, prim_phy, second_phy
int ble_set_adv_param_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;
    int actv_idx = 0;
    ble_adv_param_t adv_param;

    if (argc != 8)
    {
        os_printf("\nThe count of param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    os_memset(&adv_param, 0, sizeof(ble_adv_param_t));
    adv_param.chnl_map = os_strtoul(argv[0], NULL, 16);
    if (adv_param.chnl_map > 7)
    {
        os_printf("\nThe first(channel_map) param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    adv_param.adv_intv_min = os_strtoul(argv[1], NULL, 16) & 0xFFFFFF;
    adv_param.adv_intv_max = os_strtoul(argv[2], NULL, 16) & 0xFFFFFF;
    if ((adv_param.adv_intv_min > ADV_INTERVAL_MAX || adv_param.adv_intv_min < ADV_INTERVAL_MIN)
        || (adv_param.adv_intv_max > ADV_INTERVAL_MAX || adv_param.adv_intv_max < ADV_INTERVAL_MIN)
        || (adv_param.adv_intv_min > adv_param.adv_intv_max))
    {
        os_printf("input param interval is error\n");
        err = kParamErr;
        goto error;
    }

    adv_param.own_addr_type = os_strtoul(argv[3], NULL, 16) & 0xFF;
    switch (adv_param.own_addr_type)
    {
        case 0:
        case 1:
            adv_param.own_addr_type = BLE_STATIC_ADDR;
            break;
        case 2:
            adv_param.own_addr_type = BLE_GEN_RSLV_ADDR;
            break;
        case 3:
            adv_param.own_addr_type = BLE_GEN_NON_RSLV_ADDR;
            break;
        default:
            os_printf("\nThe third(own_addr_type) param is wrong!\n");
            err = kParamErr;
            break;
    }

    if (err != kNoErr)
        goto error;

    adv_param.adv_type = os_strtoul(argv[4], NULL, 16) & 0xFF;
    if (adv_param.adv_type > 2)
    {
        os_printf("\nThe forth(adv_type) param is wrong!\n");
        err = kParamErr;
        goto error;
    }
    adv_param.adv_prop = os_strtoul(argv[5], NULL, 16) & 0xFFFF;
    adv_param.prim_phy = os_strtoul(argv[6], NULL, 16) & 0xFF;
    if(!(adv_param.prim_phy == 1 || adv_param.prim_phy == 3))
    {
        os_printf("input param prim_phy is error\n");
        err = kParamErr;
        goto error;
    }

    adv_param.second_phy = os_strtoul(argv[7], NULL, 16) & 0xFF;
    if(adv_param.second_phy < 1 || adv_param.second_phy > 3)
    {
        os_printf("input param second_phy is error\n");
        err = kParamErr;
        goto error;
    }

    if (ble_at_cmd_table[2].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_ADV_CREATED);
    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        actv_idx = bk_ble_get_idle_actv_idx_handle();
        if (actv_idx == UNKNOW_ACT_IDX)
        {
            err = kNoResourcesErr;
            goto error;
        }
    }

    err = bk_ble_create_advertising(actv_idx, &adv_param, ble_at_cmd_cb);
    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

//AT+BLECMD=SETADVDATA,data,length
int ble_set_adv_data_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint8_t adv_data[31];
    uint8_t adv_len = 0;
    int actv_idx = -1;
    int err = kNoErr;

    if (argc != 2)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    adv_len = os_strtoul(argv[1], NULL, 16) & 0xFF;
    if (adv_len > 31 || adv_len != os_strlen(argv[0]) / 2)
    {
        os_printf("input adv len over limited\n");
        err = kParamErr;
        goto error;
    }

    at_set_data_handle(adv_data, argv[0],  os_strlen(argv[0]));

    if (ble_at_cmd_table[3].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_ADV_CREATED);

    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_ADV_STARTED);
        if (actv_idx == AT_BLE_MAX_ACTV)
        {
            os_printf("ble adv not set params before\n");
            err = kNoResourcesErr;
            goto error;
        }
    }

    err = bk_ble_set_adv_data(actv_idx, adv_data, adv_len, ble_at_cmd_cb);
    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

//AT+BLECMD=SETPERADVDATA,data,length
int ble_set_per_adv_data_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint8_t adv_data[31];
    uint8_t adv_len = 0;
    int actv_idx = -1;
    int err = kNoErr;

    if (argc != 2)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    adv_len = os_strtoul(argv[1], NULL, 16) & 0xFF;
    if (adv_len > 31 || adv_len != os_strlen(argv[0]) / 2)
    {
        os_printf("input adv len over limited\n");
        err = kParamErr;
        goto error;
    }

    at_set_data_handle(adv_data, argv[0],  os_strlen(argv[0]));

    if (ble_at_cmd_table[4].is_sync_cmd)
    {
    err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_ADV_CREATED);
    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_ADV_STARTED);
        if (actv_idx == AT_BLE_MAX_ACTV)
        {
            os_printf("ble adv not set params before\n");
            err = kNoResourcesErr;
            goto error;
        }
    }

    err = bk_ble_set_per_adv_data(actv_idx, adv_data, adv_len, ble_at_cmd_cb);
    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_set_scan_rsp_data_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint8_t scan_rsp_data[31];
    uint8_t data_len = 0;
    int actv_idx = -1;
    int err = kNoErr;

    if (argc != 2)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    data_len = os_strtoul(argv[1], NULL, 16) & 0xFF;
    if (data_len > 31 || data_len != os_strlen(argv[0]) / 2)
    {
        os_printf("input adv len over limited\n");
        err = kParamErr;
        goto error;
    }

    at_set_data_handle(scan_rsp_data, argv[0], os_strlen(argv[0]));

    if (ble_at_cmd_table[5].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_ADV_CREATED);
    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_ADV_STARTED);
        if (actv_idx == AT_BLE_MAX_ACTV)
        {
            os_printf("ble adv not set params before\n");
            err = kNoResourcesErr;
            goto error;
        }
    }

    err = bk_ble_set_scan_rsp_data(actv_idx, scan_rsp_data, data_len, ble_at_cmd_cb);
    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;

}

int ble_set_adv_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int actv_idx = -1;
    int enable = 0;
    int err = kNoErr;

    if (argc != 1)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    if (os_strcmp(argv[0], "1") == 0)
    {
        enable = 1;
    }
    else if (os_strcmp(argv[0], "0") == 0)
    {
        enable = 0;
    }
    else
    {
        os_printf("the input param is error\n");
        err = kParamErr;
        goto error;
    }

    if (ble_at_cmd_table[6].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    if (enable == 1)
    {
        actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_ADV_CREATED);
    }
    else
    {
        actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_ADV_STARTED);
    }

    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        os_printf("ble adv not set params before\n");
        err = kNoResourcesErr;
        goto error;
    }

    if (enable == 1)
    {
        err = bk_ble_start_advertising(actv_idx, 0, ble_at_cmd_cb);
    }
    else
    {
        err = bk_ble_stop_advertising(actv_idx, ble_at_cmd_cb);
    }

    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_set_scan_param_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;
    uint8_t actv_idx = 0;
    ble_scan_param_t scan_param;

    if (argc != 4)
    {
        os_printf("\nThe number of param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    os_memset(&scan_param, 0, sizeof(ble_scan_param_t));
    scan_param.own_addr_type = os_strtoul(argv[0], NULL, 16) & 0xFF;
    switch (scan_param.own_addr_type)
    {
        case 0:
        case 1:
            scan_param.own_addr_type = BLE_STATIC_ADDR;
            break;
        case 2:
            scan_param.own_addr_type = BLE_GEN_RSLV_ADDR;
            break;
        case 3:
            scan_param.own_addr_type = BLE_GEN_NON_RSLV_ADDR;
            break;
        default:
            os_printf("\nThe fourth param is wrong!\n");
            err = kParamErr;
            break;
    }

    if (err != kNoErr)
        goto error;

    scan_param.scan_phy = os_strtoul(argv[1], NULL, 16) & 0xFF;
    if (!(scan_param.scan_phy & (PHY_1MBPS_BIT | PHY_CODED_BIT)))
    {
        os_printf("\nThe scan phy param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    scan_param.scan_intv = os_strtoul(argv[2], NULL, 16) & 0xFFFF;
    scan_param.scan_wd = os_strtoul(argv[3], NULL, 16) & 0xFFFF;
    if (scan_param.scan_intv < SCAN_INTERVAL_MIN || scan_param.scan_intv > SCAN_INTERVAL_MAX ||
        scan_param.scan_wd < SCAN_WINDOW_MIN || scan_param.scan_wd > SCAN_WINDOW_MAX ||
        scan_param.scan_intv < scan_param.scan_wd)
    {
        os_printf("\nThe second/third param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    if (ble_at_cmd_table[7].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    bk_ble_set_notice_cb(ble_at_notice_cb);
    actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_SCAN_CREATED);
    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        actv_idx = bk_ble_get_idle_actv_idx_handle();
        if (actv_idx == UNKNOW_ACT_IDX)
        {
            err = kNoResourcesErr;
            goto error;
        }
    }

    err = bk_ble_create_scaning(actv_idx, &scan_param, ble_at_cmd_cb);

    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return err;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_set_scan_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint8_t actv_idx = 0;
    int enable = 0;
    int err = kNoErr;

    uint8_t filt_duplicate = 0;
    uint16_t duration = 0;
    uint16_t period = 10;

    if (argc < 1)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    if (os_strcmp(argv[0], "1") == 0)
    {
        enable = 1;
    }
    else if (os_strcmp(argv[0], "0") == 0)
    {
        enable = 0;
    }
    else
    {
        os_printf("the input param is error\n");
        err = kParamErr;
        goto error;
    }

    if(enable)
    {
        if(argc == 1)
        {

        }
        else if (argc >= 1 + 3)
        {
            filt_duplicate = os_strtoul(argv[1], NULL, 16) & 0xFF;
            duration = os_strtoul(argv[2], NULL, 16) & 0xFFFF;
            period = os_strtoul(argv[3], NULL, 16) & 0xFFFF;
        }
        else
        {
            os_printf("the input param is error\n");
            err = kParamErr;
            goto error;
        }
    }


    if (ble_at_cmd_table[8].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }
    bk_ble_set_notice_cb(ble_at_notice_cb);

    if (enable == 1)
    {
        actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_SCAN_CREATED);
    }
    else
    {
        actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_SCAN_STARTED);
    }

    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        os_printf("scan actv not start before\n");
        err = kNoResourcesErr;
        goto error;
    }

    if (enable == 1)
    {
        err = bk_ble_start_scaning_ex(actv_idx, filt_duplicate, duration, period, ble_at_cmd_cb);
    }
    else
    {
        err = bk_ble_stop_scaning(actv_idx, ble_at_cmd_cb);
    }

    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_start_connect_handle(uint8_t actv_idx, uint8_t peer_addr_type, bd_addr_t *bdaddr, ble_cmd_cb_t cb)
{
    int err = kNoErr;
    err = bk_ble_init_set_connect_dev_addr(actv_idx, bdaddr, peer_addr_type);
    if (err != 0)
    {
        return err;
    }
    err = bk_ble_init_start_conn(actv_idx, cb);
    if (err != 0)
    {
        return err;
    }

    if(ble_at_cmd_sema != NULL)
    {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err == kNoErr)
        {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                return kNoErr;
            }
            else
            {
                err = at_cmd_status;
            }
        }
    }
    else
        err = kNotFoundErr;

    return err;
}

int ble_create_connect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint8_t actv_idx = 0;
    ble_conn_param_t conn_param;
    uint8_t peer_addr_type = 0;
    bd_addr_t bdaddr;
    int err = kNoErr;
    uint8_t central_count = 0;

    if(!bk_ble_if_support_central(&central_count) || central_count == 0)
    {
        os_printf("not support central\n");
        err = kParamErr;
        goto error;
    }


    if (argc != 6)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    conn_param.intv_min = os_strtoul(argv[0], NULL, 16) & 0xFFFF;
    conn_param.con_latency = os_strtoul(argv[1], NULL, 16) & 0xFFFF;
    if (conn_param.intv_min < CON_INTERVAL_MIN || conn_param.intv_min > CON_INTERVAL_MAX
        || conn_param.con_latency > CON_LATENCY_MAX)
    {
        err = kParamErr;
        goto error;
    }

    conn_param.sup_to = os_strtoul(argv[2], NULL, 16) & 0xFFFF;
    if (conn_param.sup_to < CON_SUP_TO_MIN || conn_param.sup_to > CON_SUP_TO_MAX)
    {
        err = kParamErr;
        goto error;
    }

    conn_param.init_phys = os_strtoul(argv[3], NULL, 16) & 0xFF;
    if (conn_param.init_phys > (PHY_1MBPS_BIT | PHY_2MBPS_BIT | PHY_CODED_BIT))
    {
        err = kParamErr;
        goto error;
    }

    peer_addr_type = os_strtoul(argv[4], NULL, 16) & 0xFF;
    if (peer_addr_type > 0x03)
    {
        err = kParamErr;
        goto error;
    }

    err = get_addr_from_param(&bdaddr, argv[5]);
    if (err != kNoErr)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    if ((10 * conn_param.sup_to) < (((1 + conn_param.con_latency) * conn_param.intv_min * 5 + 1) >> 1))
    {
        os_printf("input param not suitable, maybe you can set con_latency to 0\n");
        err = kParamErr;
        goto error;
    }

    if (ble_at_cmd_table[9].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    bk_ble_set_notice_cb(ble_at_notice_cb);

    /*actv_idx = bk_ble_find_master_state_idx_handle(AT_INIT_STATE_CREATED);
    if (actv_idx == AT_BLE_MAX_CONN)*/
    {
        /// Do not create actv
        actv_idx = bk_ble_get_idle_conn_idx_handle();
        if (actv_idx == UNKNOW_ACT_IDX)
        {
            err = kNoResourcesErr;
            goto error;
        }

        err = bk_ble_create_init(actv_idx, &conn_param, ble_at_cmd_cb);
        if (err != BK_ERR_BLE_SUCCESS)
            goto error;
        if(ble_at_cmd_sema != NULL) {
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(err != kNoErr) {
                goto error;
            } else {
                if (at_cmd_status == BK_ERR_BLE_SUCCESS)
                {
                    err = ble_start_connect_handle(actv_idx, peer_addr_type, &bdaddr, ble_at_cmd_cb);
                    if (err != kNoErr)
                        goto error;
                    msg = AT_CMD_RSP_SUCCEED;
                    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                    rtos_deinit_semaphore(&ble_at_cmd_sema);
                    return err;
                }
                else
                {
                    err = at_cmd_status;
                    goto error;
                }
            }
        }
    }
    /*else
    {
        /// have created actv, this happend in which connection have been disconnected
        err = ble_start_connect_handle(actv_idx, peer_addr_type, &bdaddr, ble_at_cmd_cb);
        if (err != kNoErr)
            goto error;
        msg = AT_CMD_RSP_SUCCEED;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        rtos_deinit_semaphore(&ble_at_cmd_sema);
        return err;
    }*/

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_cancel_create_connect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint8_t actv_idx = 0;
    int err = kNoErr;
    uint8_t central_count = 0;

    if(!bk_ble_if_support_central(&central_count) || central_count == 0)
    {
        os_printf("not support central\n");
        err = kParamErr;
        goto error;
    }

    if (argc != 0)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    if (ble_at_cmd_table[10].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    actv_idx = bk_ble_find_master_state_idx_handle(AT_INIT_STATE_CONECTTING);
    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        os_printf("ble adv not set params before\n");
        err = kNoResourcesErr;
        goto error;
    }

    err = bk_ble_init_stop_conn(actv_idx, ble_at_cmd_cb);
    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_disconnect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;
    bd_addr_t connect_addr;
    uint8_t conn_idx;
    uint8_t central_count = 0;

    if(!bk_ble_if_support_central(&central_count) || central_count == 0)
    {
        os_printf("not support central\n");
        err = kParamErr;
        goto error;
    }

    if (argc != 1)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    err = get_addr_from_param(&connect_addr, argv[0]);
    if (err != kNoErr)
    {
        os_printf("input addr param error\n");
        err = kParamErr;
        goto error;
    }

    if (ble_at_cmd_table[11].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    /// get connect_idx from connect_addr
    conn_idx = bk_ble_find_conn_idx_from_addr(&connect_addr);
    if (conn_idx == AT_BLE_MAX_CONN)
    {
        os_printf("ble not connection\n");
        err = kNoResourcesErr;
        goto error;
    }

    err = bk_ble_disconnect(conn_idx, ble_at_cmd_cb);
    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_update_conn_param_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;
    bd_addr_t connect_addr;
    ble_conn_param_t conn_param;
    uint8_t conn_idx;
    uint8_t central_count = 0;

    if(!bk_ble_if_support_central(&central_count) || central_count == 0)
    {
        os_printf("not support central\n");
        err = kParamErr;
        goto error;
    }

    if (argc != 5)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    err = get_addr_from_param(&connect_addr, argv[0]);
    if (err != kNoErr)
    {
        os_printf("input addr param error\n");
        err = kParamErr;
        goto error;
    }

    conn_param.intv_min = os_strtoul(argv[1], NULL, 16) & 0xFFFF;
    conn_param.intv_max = os_strtoul(argv[2], NULL, 16) & 0xFFFF;
    conn_param.con_latency = os_strtoul(argv[3], NULL, 16) & 0xFFFF;
    conn_param.sup_to = os_strtoul(argv[4], NULL, 16) & 0xFFFF;

    if ((conn_param.intv_min < CON_INTERVAL_MIN || conn_param.intv_min > CON_INTERVAL_MAX) ||
        (conn_param.intv_max < CON_INTERVAL_MIN || conn_param.intv_max > CON_INTERVAL_MAX) ||
        (conn_param.intv_min > conn_param.intv_max) || (conn_param.con_latency > CON_LATENCY_MAX) ||
        (conn_param.sup_to < CON_SUP_TO_MIN || conn_param.sup_to > CON_SUP_TO_MAX))
    {
        os_printf("input update param not suitable\n");
        err = kParamErr;
        goto error;
    }

    if ((10 * conn_param.sup_to) < (((1 + conn_param.con_latency) * conn_param.intv_max * 5 + 1) >> 1))
    {
        os_printf("input param not suitable, maybe you can set con_latency to 0\n");
        err = kParamErr;
        goto error;
    }

    if (ble_at_cmd_table[12].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    /// get connect_idx from connect_addr
    conn_idx = bk_ble_find_conn_idx_from_addr(&connect_addr);
    if (conn_idx == AT_BLE_MAX_CONN)
    {
        os_printf("ble not connection\n");
        err = kNoResourcesErr;
        goto error;
    }

    err = bk_ble_update_param(conn_idx, &conn_param, ble_at_cmd_cb);
    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return err;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_get_conn_state_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;
    uint8_t conn_state = 0;
    bd_addr_t peer_addr;

    if (argc != 1)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    err = get_addr_from_param(&peer_addr, argv[0]);
    if (err != kNoErr)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    conn_state = bk_ble_get_connect_state(&peer_addr);

    if (conn_state == 1)
    {
        sprintf(pcWriteBuffer, "%s%s\r\n", AT_CMDRSP_HEAD, "BLE_CONNECT");
    }
    else
    {
        sprintf(pcWriteBuffer, "%s:%s\r\n", AT_CMDRSP_HEAD, "BLE_DISCONNECT");
    }

    return err;

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return err;
}

int ble_get_local_addr_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	uint8_t local_addr[6];

	if (argc != 0) {
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}

	err = bk_ble_get_mac(local_addr);
	if (err != kNoErr) {
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}

	sprintf(pcWriteBuffer, "%s%s%02x:%02x:%02x:%02x:%02x:%02x\r\n", AT_CMDRSP_HEAD, "BLE_ADDR:", local_addr[5],
	local_addr[4], local_addr[3], local_addr[2], local_addr[1], local_addr[0]);
	return err;
error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}

int ble_set_local_addr_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	// not defined
	return 0;
}

//#if (CONFIG_BTDM_5_2)


#define DECL_PRIMARY_SERVICE_128     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define DECL_CHARACTERISTIC_128      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define DESC_CLIENT_CHAR_CFG_128     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}



ble_attm_desc_t test_service_db[TEST_IDX_NB] = {
    //  Service Declaration
    [TEST_IDX_SVC]              = {DECL_PRIMARY_SERVICE_128, BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
    [TEST_IDX_CHAR_DECL]    = {DECL_CHARACTERISTIC_128,  BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
    // Characteristic Value
    [TEST_IDX_CHAR_VALUE]   = {{0x34, 0x12, 0},     BK_BLE_PERM_SET(NTF, ENABLE), BK_BLE_PERM_SET(RI, ENABLE) | BK_BLE_PERM_SET(UUID_LEN, UUID_16), 128},
    //Client Characteristic Configuration Descriptor
    [TEST_IDX_CHAR_DESC] = {DESC_CLIENT_CHAR_CFG_128, BK_BLE_PERM_SET(RD, ENABLE) | BK_BLE_PERM_SET(WRITE_REQ, ENABLE), 0, 0},

    //data len
    [TEST_IDX_CHAR_DATALEN_DECL]    = {DECL_CHARACTERISTIC_128,  BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
    [TEST_IDX_CHAR_DATALEN_VALUE]   = {{0xbc, 0x9a, 0}, BK_BLE_PERM_SET(WRITE_REQ, ENABLE) | BK_BLE_PERM_SET(RD, ENABLE), BK_BLE_PERM_SET(RI, ENABLE) | BK_BLE_PERM_SET(UUID_LEN, UUID_16), 128},

    //send inter
    [TEST_IDX_CHAR_INTER_DECL]    = {DECL_CHARACTERISTIC_128,  BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
    [TEST_IDX_CHAR_INTER_VALUE]   = {{0xf0, 0xde, 0}, BK_BLE_PERM_SET(WRITE_REQ, ENABLE) | BK_BLE_PERM_SET(RD, ENABLE), BK_BLE_PERM_SET(RI, ENABLE) | BK_BLE_PERM_SET(UUID_LEN, UUID_16), 128},

};

int ble_update_mtu_2_max_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int err = kNoErr;
    char *msg = NULL;
    uint8 conn_idx = 0;
    bd_addr_t connect_addr;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

    if (argc < 1)
    {
        os_printf("\nThe number of param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    err = get_addr_from_param(&connect_addr, argv[0]);
    if (err != kNoErr)
    {
        os_printf("input addr param error\r\n");
        err = kParamErr;
        goto error;
    }

    conn_idx = bk_ble_find_conn_idx_from_addr(&connect_addr);
    if (conn_idx == AT_BLE_MAX_CONN)
    {
        os_printf("ble not connection\r\n");
        err = kNoResourcesErr;
        goto error;
    }


    if (1)//ble_at_cmd_table[22].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if (err != kNoErr)
        {
            goto error;
        }
    }

    bk_ble_set_notice_cb(ble_at_notice_cb);

    err = bk_ble_gatt_mtu_change(conn_idx, ble_at_cmd_cb);

    if (err != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }
    else
    {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if (err != kNoErr)
        {
            goto error;
        }
        else
        {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return err;
            }
            else
            {
                err = at_cmd_status;
                at_cmd_status = BK_ERR_BLE_SUCCESS;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    g_test_prf_task_id = 0;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_tx_test_param_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int err = kNoErr;
    char *msg = NULL;
    uint16_t len = 0;
    uint32_t inter = 0;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

    if (argc < 2)
    {
        os_printf("\nThe number of param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    if(g_test_prf_task_id == 0)
    {
        os_printf("%s not reg profile, exit\n", __func__);
        err = kParamErr;
        goto error;
    }

    len = os_strtoul(argv[0], NULL, 10) & 0xFFFF;
    inter = os_strtoul(argv[1], NULL, 10) & 0xFFFFFFFF;


    if(inter == 0 || len == 0)
    {
        os_printf("%s param err\n", __func__);
        err = kParamErr;
        goto error;
    }
    s_test_data_len = len;
    s_test_send_inter = inter;
    os_printf("%s len %d inter %d\n", __func__, s_test_data_len, s_test_send_inter);
    if (err != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }
    else
    {
        {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                return err;
            }
            else
            {
                err = at_cmd_status;
                at_cmd_status = BK_ERR_BLE_SUCCESS;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_tx_test_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int err = kNoErr;
    char *msg = NULL;
    uint8_t enable = 0;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

    if (argc < 1)
    {
        os_printf("\nThe number of param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    if(g_test_prf_task_id == 0)
    {
        os_printf("%s not reg profile, exit\n", __func__);
        err = kParamErr;
        goto error;
    }

    enable = os_strtoul(argv[0], NULL, 10) & 0xFF;
    os_printf("%s enable %d\n", __func__, enable);
    ble_test_service_write_handle(enable);

    if (err != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }
    else
    {
        {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                return err;
            }
            else
            {
                err = at_cmd_status;
                at_cmd_status = BK_ERR_BLE_SUCCESS;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}


static void ble_sdp_charac_callback(uint32_t type, uint8 conidx, uint16_t hdl, uint16_t len, uint8 *data)
{
    //os_printf("%s recv type %d len %d data[0] 0x%02X\n", __func__, type, len, data[0]);
    s_performance_rx_bytes += len;
}

int ble_register_noti_service_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    s_service_type = 0;
    return ble_register_service_handle(pcWriteBuffer, xWriteBufferLen, argc, argv);
}

int ble_register_performance_service_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    s_service_type = 1;

    return ble_register_service_handle(pcWriteBuffer, xWriteBufferLen, argc, argv);
}

#define PERFORMANCE_STATISTIC_TX "tx"
#define PERFORMANCE_STATISTIC_RX "rx"

int ble_enable_performance_statistic_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int err = kNoErr;
    char *msg = NULL;

    uint8_t enable = 0;
    uint8_t type = 0;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

    if (argc < 2)
    {
        os_printf("\nThe number of param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    if(g_test_prf_task_id == 0)
    {
        os_printf("%s not reg profile, exit\n", __func__);
        err = kParamErr;
        goto error;
    }

    if(strlen(argv[0]) > 3)
    {
        os_printf("%s param err, exit\n", __func__);
        err = kParamErr;
        goto error;
    }

    if(!strncmp(PERFORMANCE_STATISTIC_TX, argv[0], strlen(PERFORMANCE_STATISTIC_TX)))
    {
        type = 1;
    }
    else if(!strncmp(PERFORMANCE_STATISTIC_RX, argv[0], strlen(PERFORMANCE_STATISTIC_RX)))
    {
        type = 2;
    }
    else
    {
        os_printf("%s param err, exit\n", __func__);
        err = kParamErr;
        goto error;
    }


    enable = os_strtoul(argv[1], NULL, 10) & 0xFFFFFFFF;


    if(type == 1)//tx
    {
        if(enable)
        {
            if(!rtos_is_timer_init(&ble_performance_tx_statistics_tmr))
            {
                s_performance_tx_bytes = 0;

                rtos_init_timer(&ble_performance_tx_statistics_tmr, 1000, ble_performance_tx_timer_hdl, (void *)0);
                rtos_start_timer(&ble_performance_tx_statistics_tmr);
            }
        }
        else
        {
            if (rtos_is_timer_init(&ble_performance_tx_statistics_tmr))
            {
                s_performance_tx_bytes = 0;
                if (rtos_is_timer_running(&ble_performance_tx_statistics_tmr))
                    rtos_stop_timer(&ble_performance_tx_statistics_tmr);
                rtos_deinit_timer(&ble_performance_tx_statistics_tmr);
            }
        }
    }
    else if(type == 2)//rx
    {
        if(enable)
        {
            if(!rtos_is_timer_init(&ble_performance_rx_statistics_tmr))
            {
                s_performance_rx_bytes = 0;

                rtos_init_timer(&ble_performance_rx_statistics_tmr, 1000, ble_performance_rx_timer_hdl, (void *)0);
                rtos_start_timer(&ble_performance_rx_statistics_tmr);
            }
        }
        else
        {
            if (rtos_is_timer_init(&ble_performance_rx_statistics_tmr))
            {
                s_performance_rx_bytes = 0;

                if (rtos_is_timer_running(&ble_performance_rx_statistics_tmr))
                    rtos_stop_timer(&ble_performance_rx_statistics_tmr);
                rtos_deinit_timer(&ble_performance_rx_statistics_tmr);
            }
        }
    }


    if (err != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }
    else
    {
        {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                return err;
            }
            else
            {
                err = at_cmd_status;
                at_cmd_status = BK_ERR_BLE_SUCCESS;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}


static int ble_register_service_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int err = kNoErr;
    char *msg = NULL;
    uint16 service_uuid = 0;
    uint16 char_uuid = 0x1234;
    struct bk_ble_db_cfg ble_db_cfg;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

    if (argc < 2)
    {
        os_printf("\nThe number of param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    g_test_prf_task_id = os_strtoul(argv[0], NULL, 16) & 0xFF;
    service_uuid = os_strtoul(argv[1], NULL, 16) & 0xFFFF;
    if(argc >= 3)
    {
        char_uuid = os_strtoul(argv[2], NULL, 16) & 0xFFFF;
    }

    if(argc >= 5)
    {
        s_test_data_len = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
        s_test_send_inter = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
    }

    os_memcpy(&(test_service_db[TEST_IDX_CHAR_VALUE].uuid[0]), &char_uuid, 2);

    os_memset(&ble_db_cfg, 0, sizeof(ble_db_cfg));

    ble_db_cfg.att_db = (ble_attm_desc_t *)test_service_db;
    ble_db_cfg.att_db_nb = TEST_IDX_NB;
    ble_db_cfg.prf_task_id = g_test_prf_task_id;
    ble_db_cfg.start_hdl = 0;
    ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16);
    os_memcpy(&(ble_db_cfg.uuid[0]), &service_uuid, 2);

    if (1)//ble_at_cmd_table[19].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if (err != kNoErr)
        {
            goto error;
        }
    }
    bk_ble_set_notice_cb(ble_at_notice_cb);
    extern void register_app_sdp_charac_callback(void (*)(uint32_t type,uint8 conidx,uint16_t hdl,uint16_t len,uint8 *data));
    register_app_sdp_charac_callback(ble_sdp_charac_callback);
    err = bk_ble_create_db(&ble_db_cfg);

    if (err != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }
    else
    {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if (err != kNoErr)
        {
            goto error;
        }
        else
        {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return err;
            }
            else
            {
                err = at_cmd_status;
                at_cmd_status = BK_ERR_BLE_SUCCESS;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    g_test_prf_task_id = 0;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}


void ble_test_noti_hdl(void *param)
{
    uint8 *write_buffer;

    ble_err_t ret = BK_ERR_BLE_SUCCESS;
    write_buffer = (uint8_t *)os_malloc(s_test_data_len);

    if(!write_buffer)
    {
        os_printf("%s alloc err\n", __func__);
        return;
    }

    os_memset(write_buffer, 0, s_test_data_len);
    os_memcpy(write_buffer, &s_test_noti_count, sizeof(s_test_noti_count));
    s_test_noti_count++;

    ret = bk_ble_send_noti_value(s_test_data_len, write_buffer, g_test_prf_task_id, TEST_IDX_CHAR_VALUE);
    if(ret != BK_ERR_BLE_SUCCESS)
    {
        os_printf("%s ret err %d\n", __func__, ret);
    }

    os_free(write_buffer);
}

void ble_performance_tx_timer_hdl(void *param)
{
    uint32_t tmp = s_performance_tx_bytes;
    s_performance_tx_bytes = 0;

    os_printf("%s current tx %d bytes/sec\n", __func__, tmp);
}

void ble_performance_rx_timer_hdl(void *param)
{
    uint32_t tmp = s_performance_rx_bytes;
    s_performance_rx_bytes = 0;

    os_printf("%s current rx %d bytes/sec\n", __func__, tmp);
}

void ble_test_service_write_handle(uint8 val)
{
    if (val)
    {
        if(s_service_type)
        {
            if(!s_performance_tx_enable)
            {
                s_performance_tx_enable = 1;
                ble_test_noti_hdl(NULL);
            }

        }
        else if (!rtos_is_timer_init(&ble_noti_tmr))
        {
            s_test_noti_count = 0;
            rtos_init_timer(&ble_noti_tmr, s_test_send_inter, ble_test_noti_hdl, (void *)0);
            rtos_start_timer(&ble_noti_tmr);
        }
    }
    else
    {
        s_performance_tx_enable = 0;
        if (rtos_is_timer_init(&ble_noti_tmr))
        {
            if (rtos_is_timer_running(&ble_noti_tmr))
                rtos_stop_timer(&ble_noti_tmr);
            rtos_deinit_timer(&ble_noti_tmr);
        }
    }
}

int ble_read_phy_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	bd_addr_t connect_addr;
	uint8_t conn_idx = 0;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

	if (argc != 1) {
		os_printf("input param error\r\n");
		err = kParamErr;
		goto error;
	}

	bk_ble_set_notice_cb(ble_at_notice_cb);
	err = get_addr_from_param(&connect_addr, argv[0]);
	if (err != kNoErr)
	{
		os_printf("input addr param error\r\n");
		err = kParamErr;
		goto error;
	}

	if (ble_at_cmd_table[17].is_sync_cmd)
	{
		err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
		if(err != kNoErr){
			goto error;
		}
	}

	/// get connect_idx from connect_addr
	conn_idx = bk_ble_find_conn_idx_from_addr(&connect_addr);
	if (conn_idx == AT_BLE_MAX_CONN)
	{
		os_printf("ble not connection\r\n");
		err = kNoResourcesErr;
		goto error;
	}

	err = bk_ble_read_phy(conn_idx, ble_at_cmd_cb);
	if (err != kNoErr) {
		os_printf("read connect tx/rx phy failed\r\n");
		goto error;
	}

	if(ble_at_cmd_sema != NULL) {
		err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
		if(err != kNoErr) {
			os_printf("get sema fail\r\n");
			goto error;
		} else {
			if (at_cmd_status == BK_ERR_BLE_SUCCESS)
			{
				msg = AT_CMD_RSP_SUCCEED;
				os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
				rtos_deinit_semaphore(&ble_at_cmd_sema);
				return err;
			} else {
				err = at_cmd_status;
				goto error;
			}
		}
	}
error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	if (ble_at_cmd_sema != NULL)
		rtos_deinit_semaphore(&ble_at_cmd_sema);
	return err;
}

int ble_set_phy_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	bd_addr_t connect_addr;
	uint8_t conn_idx = 0;
	ble_set_phy_t le_set_phy;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

	if (argc != 4) {
		os_printf("input param error\r\n");
		err = kParamErr;
		goto error;
	}

	if (ble_at_cmd_table[18].is_sync_cmd)
	{
		err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
		if(err != kNoErr){
			goto error;
		}
	}

	bk_ble_set_notice_cb(ble_at_notice_cb);
	err = get_addr_from_param(&connect_addr, argv[0]);
	if (err != kNoErr)
	{
		os_printf("input addr param error\r\n");
		err = kParamErr;
		goto error;
	}

	le_set_phy.tx_phy = os_strtoul(argv[1], NULL, 10) & 0xFF;
	le_set_phy.rx_phy = os_strtoul(argv[2], NULL, 10) & 0xFF;
	le_set_phy.phy_opt = os_strtoul(argv[3], NULL, 10) & 0xFF;
	switch (le_set_phy.phy_opt) {
		case 0:
			le_set_phy.phy_opt = CODED_NO_PREFEER;
			break;
		case 1:
			le_set_phy.phy_opt = CODED_S2_PREFEER;
			break;
		case 2:
			le_set_phy.phy_opt = CODED_S8_PREFEER;
			break;
		default:
			os_printf("input phy_opt param error\r\n");
			err = kParamErr;
			goto error;
	}

	/// get connect_idx from connect_addr
	conn_idx = bk_ble_find_conn_idx_from_addr(&connect_addr);
	if (conn_idx == AT_BLE_MAX_CONN)
	{
		os_printf("ble not connection\r\n");
		err = kNoResourcesErr;
		goto error;
	}

	err = bk_ble_set_phy(conn_idx, &le_set_phy, ble_at_cmd_cb);
	if (err != kNoErr) {
		os_printf("set connect tx/rx phy failed\r\n");
		goto error;
	}

	if(ble_at_cmd_sema != NULL) {
		err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
		if(err != kNoErr) {
			os_printf("get sema fail\r\n");
			goto error;
		} else {
			if (at_cmd_status == BK_ERR_BLE_SUCCESS)
			{
				msg = AT_CMD_RSP_SUCCEED;
				os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
				rtos_deinit_semaphore(&ble_at_cmd_sema);
				return err;
			} else {
				err = at_cmd_status;
				goto error;
			}
		}
	}

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	if (ble_at_cmd_sema != NULL)
		rtos_deinit_semaphore(&ble_at_cmd_sema);
	return err;
}
//#endif

int ble_set_max_mtu_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	uint16_t att_max_mtu = 0;

	if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
	{
		err = kParamErr;
		goto error;
	}

	if (argc != 1) {
		os_printf("input param error\r\n");
		err = kParamErr;
		goto error;
	}

	if (ble_at_cmd_table[26].is_sync_cmd)
	{
		err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
		if(err != kNoErr){
			goto error;
		}
	}

	bk_ble_set_notice_cb(ble_at_notice_cb);
	att_max_mtu = os_strtoul(argv[0], NULL, 10) & 0xFF;

	err = bk_ble_set_max_mtu(att_max_mtu);
	if (err != kNoErr) {
		os_printf("set att maximal MTU failed\r\n");
		goto error;
	}

	if(ble_at_cmd_sema != NULL) {
		err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
		if(err != kNoErr) {
			os_printf("get sema fail\r\n");
			goto error;
		} else {
			if (at_cmd_status == BK_ERR_BLE_SUCCESS)
			{
				msg = AT_CMD_RSP_SUCCEED;
				os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
				rtos_deinit_semaphore(&ble_at_cmd_sema);
				return err;
			} else {
				err = at_cmd_status;
				goto error;
			}
		}
	}

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	if (ble_at_cmd_sema != NULL)
		rtos_deinit_semaphore(&ble_at_cmd_sema);
	return err;
}


int ble_delete_adv_activity_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;
    int actv_idx = 0;

    if (ble_at_cmd_table[19].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_ADV_CREATED);
    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        os_printf("\n adv activity has not been created!\n");
        err = kNotFoundErr;
        goto error;
    }

    err = bk_ble_delete_advertising(actv_idx, ble_at_cmd_cb);
    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_delete_scan_activity_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;
    int actv_idx = 0;

    if (ble_at_cmd_table[20].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_SCAN_CREATED);
    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        os_printf("\n scan activity has not been created!\n");
        err = kNotFoundErr;
        goto error;
    }

    err = bk_ble_delete_scaning(actv_idx, ble_at_cmd_cb);
    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_connect_by_name_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint8_t actv_idx = 0;
    ble_conn_param_t conn_param;
    int err = kNoErr;
    uint8_t central_count = 0;

    if(!bk_ble_if_support_central(&central_count) || central_count == 0)
    {
        os_printf("not support central\n");
        err = kParamErr;
        goto error;
    }


    if (argc != 5)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    conn_param.intv_min = os_strtoul(argv[0], NULL, 16) & 0xFFFF;
    conn_param.con_latency = os_strtoul(argv[1], NULL, 16) & 0xFFFF;
    if (conn_param.intv_min < CON_INTERVAL_MIN || conn_param.intv_min > CON_INTERVAL_MAX
        || conn_param.con_latency > CON_LATENCY_MAX)
    {
        err = kParamErr;
        goto error;
    }

    conn_param.sup_to = os_strtoul(argv[2], NULL, 16) & 0xFFFF;
    if (conn_param.sup_to < CON_SUP_TO_MIN || conn_param.sup_to > CON_SUP_TO_MAX)
    {
        err = kParamErr;
        goto error;
    }

    conn_param.init_phys = os_strtoul(argv[3], NULL, 16) & 0xFF;
    if (conn_param.init_phys > (PHY_1MBPS_BIT | PHY_2MBPS_BIT | PHY_CODED_BIT))
    {
        err = kParamErr;
        goto error;
    }

    os_memset(&g_peer_dev, 0, sizeof(g_peer_dev));

    g_peer_dev.dev.len = os_strlen(argv[4]);
    os_memcpy(g_peer_dev.dev.name, argv[4], g_peer_dev.dev.len);
    g_peer_dev.state = STATE_DISCOVERING;

    if (err != kNoErr)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    if ((10 * conn_param.sup_to) < (((1 + conn_param.con_latency) * conn_param.intv_min * 5 + 1) >> 1))
    {
        os_printf("input param not suitable, maybe you can set con_latency to 0\n");
        err = kParamErr;
        goto error;
    }

    if (ble_at_cmd_table[28].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    bk_ble_set_notice_cb(ble_at_notice_cb);

    if (ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, 10000);
        if(err != kNoErr) {
            goto error;
        } else {
            if (g_peer_dev.state == STATE_DISCOVERED)
            {
                g_peer_dev.state = STATE_CONNECTINIG;
            }
            else
            {
                err = BK_ERR_BLE_FAIL;
                goto error;
            }
        }
    }

    /*actv_idx = bk_ble_find_master_state_idx_handle(AT_INIT_STATE_CREATED);
    if (actv_idx == AT_BLE_MAX_CONN)*/
    {
        /// Do not create actv
        actv_idx = bk_ble_get_idle_conn_idx_handle();
        if (actv_idx == UNKNOW_ACT_IDX)
        {
            err = kNoResourcesErr;
            goto error;
        }

        err = bk_ble_create_init(actv_idx, &conn_param, ble_at_cmd_cb);
        if (err != BK_ERR_BLE_SUCCESS)
            goto error;
        if(ble_at_cmd_sema != NULL) {
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(err != kNoErr) {
                goto error;
            } else {
                if (at_cmd_status == BK_ERR_BLE_SUCCESS)
                {
                    err = ble_start_connect_handle(actv_idx, g_peer_dev.addr_type, &(g_peer_dev.bdaddr), ble_at_cmd_cb);
                    if (err != kNoErr)
                        goto error;
                    msg = AT_CMD_RSP_SUCCEED;
                    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                    rtos_deinit_semaphore(&ble_at_cmd_sema);
                    return err;
                }
                else
                {
                    err = at_cmd_status;
                    goto error;
                }
            }
        }
    }
/*    else
    {
        /// have created actv, this happend in which connection have been disconnected
        err = ble_start_connect_handle(actv_idx, g_peer_dev.addr_type, &(g_peer_dev.bdaddr), ble_at_cmd_cb);
        if (err != kNoErr)
            goto error;
        msg = AT_CMD_RSP_SUCCEED;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        rtos_deinit_semaphore(&ble_at_cmd_sema);
        return err;
    }
*/
error:
    msg = AT_CMD_RSP_ERROR;
    g_peer_dev.state = STATE_IDLE;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_disconnect_by_name_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;
    uint8_t conn_idx;
    uint8_t central_count = 0;
    ble_device_name_t dev;

    if(!bk_ble_if_support_central(&central_count) || central_count == 0)
    {
        os_printf("not support central\n");
        err = kParamErr;
        goto error;
    }

    if (argc != 1)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    os_memset(&dev, 0, sizeof(dev));

    dev.len = os_strlen(argv[0]);
    os_memcpy(dev.name, argv[0], dev.len);

    if (!((STATE_CONNECTED == g_peer_dev.state) && (dev.len == g_peer_dev.dev.len) && (!os_memcmp(g_peer_dev.dev.name, dev.name, dev.len))))
    {
        err = kParamErr;
        goto error;
    }

    if (ble_at_cmd_table[29].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    /// get connect_idx from connect_addr
    conn_idx = bk_ble_find_conn_idx_from_addr(&g_peer_dev.bdaddr);
    if (conn_idx == AT_BLE_MAX_CONN)
    {
        os_printf("ble not connection\n");
        err = kNoResourcesErr;
        goto error;
    }

    g_peer_dev.state = STATE_DISCONNECTINIG;

    err = bk_ble_disconnect(conn_idx, ble_at_cmd_cb);
    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}



int ble_create_periodic_sync_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;
    uint8_t actv_idx = 0;

    if (ble_at_cmd_table[30].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    bk_ble_set_notice_cb(ble_at_notice_cb);
    actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_PER_SYNC_CREATED);
    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        actv_idx = bk_ble_get_idle_actv_idx_handle();
        if (actv_idx == UNKNOW_ACT_IDX)
        {
            err = kNoResourcesErr;
            goto error;
        }
    }

    err = bk_ble_create_periodic_sync(actv_idx, ble_at_cmd_cb);

    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return err;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

int ble_start_periodic_sync_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint8_t actv_idx = 0;

    int err = kNoErr;

    ble_periodic_param_t periodic_param;

    if (argc < 7)
    {
        os_printf("\nThe number of param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    os_memset(&periodic_param, 0, sizeof(periodic_param));

    periodic_param.report_disable = os_strtoul(argv[0], NULL, 16) & 0xFF;
    periodic_param.adv_sid = os_strtoul(argv[1], NULL, 16) & 0xFF;

    err = get_addr_from_param(&periodic_param.adv_addr, argv[2]);
    if (err != kNoErr)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    periodic_param.adv_addr_type = os_strtoul(argv[3], NULL, 16) & 0xFF;
    periodic_param.skip = os_strtoul(argv[4], NULL, 16) & 0xFF;
    periodic_param.sync_to = os_strtoul(argv[5], NULL, 16) & 0xFF;
    periodic_param.cte_type = os_strtoul(argv[6], NULL, 16) & 0xFF;



    if (1)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }
    bk_ble_set_notice_cb(ble_at_notice_cb);


    actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_PER_SYNC_CREATED);


    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        os_printf("periodic not create before\n");
        err = kNoResourcesErr;
        goto error;
    }

    err = bk_ble_start_periodic_sync(actv_idx, &periodic_param, ble_at_cmd_cb);

    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}


int ble_stop_periodic_sync_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    uint8_t actv_idx = 0;

    int err = kNoErr;

    bd_addr_t addr;

    if (argc < 1)
    {
        os_printf("\nThe number of param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    err = get_addr_from_param(&addr, argv[0]);
    if (err != kNoErr)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    if (1)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    bk_ble_set_notice_cb(ble_at_notice_cb);

    actv_idx = bk_ble_find_actv_state_idx_handle(AT_ACTV_PER_SYNC_STARTED);

    if (actv_idx == AT_BLE_MAX_ACTV)
    {
        os_printf("periodic not start before\n");
        err = kNoResourcesErr;
        goto error;
    }

    err = bk_ble_stop_periodic_sync(actv_idx, ble_at_cmd_cb);

    if (err != BK_ERR_BLE_SUCCESS)
        goto error;
    if(ble_at_cmd_sema != NULL) {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr) {
            goto error;
        } else {
            if (at_cmd_status == BK_ERR_BLE_SUCCESS)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                rtos_deinit_semaphore(&ble_at_cmd_sema);
                return 0;
            }
            else
            {
                err = at_cmd_status;
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}


#endif

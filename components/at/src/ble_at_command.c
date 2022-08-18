#include <stdio.h>
#include <stdlib.h>
#include "at_common.h"
#include "modules/wifi.h"
#include <driver/gpio.h>

#if CONFIG_BLE//(CONFIG_BLE_5_X || CONFIG_BTDM_5_2)
#include "at_ble_common.h"
#include "legacy_include/bluetooth_legacy_include.h"

#ifdef CONFIG_ALI_MQTT
#include "iot_import.h"
#include "iot_export.h"
#include "iot_export_mqtt.h"
#endif


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

uint8_t s_test_conn_ind = ~0;


static uint8_t s_service_type = 0;

static uint32_t s_performance_tx_bytes = 0;
static uint32_t s_performance_rx_bytes = 0;
static uint8_t s_performance_tx_enable = 0;


//for ethermind
static beken_timer_t s_ethermind_ble_send_test_timer;
static uint16_t s_ethermind_send_test_service_handle = 0;;
static uint16_t s_ethermind_send_char_test_handle = 0;
static uint16_t s_ethermind_send_size_handle = 0;
static uint16_t s_ethermind_send_intv_handle = 0;


static uint8_t s_ethermind_send_test_value = 0xa;
static uint16_t s_ethermind_send_size_value = 20;
static uint16_t s_ethermind_send_intv_value = 1000;

static uint8_t s_ethermind_att_handle; //ATT_CON_ID


static uint8_t s_ethermind_service_type = 0;

static uint32_t s_ethermind_performance_tx_bytes = 0;
static uint32_t s_ethermind_performance_rx_bytes = 0;
static uint8_t s_ethermind_auto_tx_enable = 0;

static beken_timer_t ble_ethermind_performance_tx_statistics_tmr;
static beken_timer_t ble_ethermind_performance_rx_statistics_tmr;


static ATT_ATTR_HANDLE s_ethermind_nordic_write_attr_handle[8];
static ATT_ATTR_HANDLE s_ethermind_nordic_write_notify_handle[8];
//static ATT_HANDLE s_ethermind_nordic_att_info[8];
static uint8_t s_ethermind_nordic_att_info[8];
static uint8_t s_ethermind_nordic_used[8] = {0};

const uint8_t nus_tx_uuid[] = {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x03,0x00,0x40,0x6e};
const uint8_t nus_rx_uuid[] = {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x02,0x00,0x40,0x6e};

static uint16_t plc_ccc_handle[10] = {0};
static uint16_t plc_rx_handle[10] = {0};

static uint8_t send_value[32] = {0};
//static ATT_HANDLE att_handle;
static ATT_ATTR_HANDLE s_read_tmp_attr_handle = 0;


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
void ble_test_service_write_handle(uint8 val, uint8 con_idx);
int ble_read_phy_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_set_phy_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_set_max_mtu_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
//#endif
int ble_delete_adv_activity_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_delete_scan_activity_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern int ble_boarding_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void ble_test_noti_hdl(void *param);
static void ble_performance_tx_timer_hdl(void *param);
static void ble_performance_rx_timer_hdl(void *param);
int ble_connect_by_name_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_disconnect_by_name_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int ble_create_periodic_sync_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_start_periodic_sync_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_stop_periodic_sync_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int ble_att_write_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

static void ble_tx_test_active_timer_callback(void *param);
static void ble_ethermind_performance_tx_timer_hdl(void *param);
static void ble_ethermind_performance_rx_timer_hdl(void *param);
int ble_enable_packet_loss_ratio_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#ifdef CONFIG_ALI_MQTT
int ble_mqtt_loop_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif
int ble_power_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

static int ble_att_read_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#ifdef CONFIG_ALI_MQTT
static void ble_send_data_2_mqtt(uint8 con_idx, uint16_t len, uint8 *data);
uint8_t loop_type = 0;

enum {
    LT_NONE,
    LT_WIFI_ONLY,
    LT_COEX,
};
#endif

const at_command_t ble_at_cmd_table[] = {
#ifdef CONFIG_BT
    {0, "SETBLENAME", 1, "return ble name", set_ble_name_handle},
    {1, "GETBLENAME", 1, "return ble name", get_ble_name_handle},
#else
    {0, "SETBLENAME", 0, "return ble name", set_ble_name_handle},
    {1, "GETBLENAME", 0, "return ble name", get_ble_name_handle},
#endif
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
#ifdef CONFIG_BT
    {14, "GETLOCALADDR", 1, "get ble local address", ble_get_local_addr_handle},
#else
    {14, "GETLOCALADDR", 0, "get ble local address", ble_get_local_addr_handle},
#endif
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
    {33, "ATTWRITE", 1, "att write <ATT_CON_ID> <ATT_ATTR_HANDLE> <value>", ble_att_write_handle},
    {34, "ENABLEPLRTEST", 0, "enable packet loss ratio test", ble_enable_packet_loss_ratio_test_handle},
#ifdef CONFIG_ALI_MQTT
    {35, "MQTTLOOPBACK", 0, "enable mqtt loopback test", ble_mqtt_loop_handle},
#endif
    {36, "POWER", 0, "power on/off", ble_power_handle},
    {37, "ATTREAD", 1, "att read <DEVICE_HANDLE> <ATT_CON_ID> <ATT_ATTR_HANDLE>", ble_att_read_handle},
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
        case BLE_SET_LOCAL_NAME:
        case BLE_GET_LOCAL_NAME:
        case BLE_READ_LOCAL_ADDR:
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

static uint8_t ble_get_device_name_from_adv(uint8_t* p_buf, uint8_t data_len, uint8_t* dev_name, uint8_t *in_out_len)
{
    uint8_t* p_data = p_buf;
    uint8_t* p_data_end = p_buf + data_len;

    uint8_t is_found = 0;

    while (p_data < p_data_end)
    {
        if (*(p_data + 1) == 0x09)//GAP_AD_TYPE_COMPLETE_NAME
        {
            is_found = 1;

            os_memcpy(dev_name, p_data + 2, ((p_data[0] - 1 > *in_out_len) ? *in_out_len : p_data[0] - 1));
            break;
        }

        // Go to next advertising info
        p_data += (*p_data + 1);
    }

    return (is_found);
}

static int8_t ethermind_find_idle_info_index(uint8_t *index)
{
    for (uint8_t i = 0; i < sizeof(s_ethermind_nordic_used) / sizeof(s_ethermind_nordic_used[0]); ++i)
    {
        if(s_ethermind_nordic_used[i] == 0)
        {
            *index = i;
            return 0;
        }
    }

    return -1;
}

static int8_t ethermind_clean_info_by_index(uint8_t index)
{
    s_ethermind_nordic_used[index] = 0;
    os_memset(s_ethermind_nordic_write_attr_handle + index, 0, sizeof(*s_ethermind_nordic_write_attr_handle));
    os_memset(s_ethermind_nordic_att_info + index, 0, sizeof(*s_ethermind_nordic_att_info));

    return 0;
}

static int8_t ethermind_find_index_by_info(uint8_t *index, uint8_t att_con_id)
{

    for (uint8_t i = 0; i < sizeof(s_ethermind_nordic_used) / sizeof(s_ethermind_nordic_used[0]); ++i)
    {
        if(s_ethermind_nordic_used[i] &&
            s_ethermind_nordic_att_info[i] == att_con_id)
        {
            *index = i;
            return 0;
        }
    }

    return -1;
}

int8_t ethermind_find_index_by_att_con_id(uint8_t *index, ATT_CON_ID att_id)
{

    for (uint8_t i = 0; i < sizeof(s_ethermind_nordic_used) / sizeof(s_ethermind_nordic_used[0]); ++i)
    {
        if(s_ethermind_nordic_used[i] &&
            s_ethermind_nordic_att_info[i] == att_id)
        {
            *index = i;
            return 0;
        }
    }

    return -1;
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
                ble_test_service_write_handle(w_req->value[0], w_req->conn_idx);
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
                {
                    uint8_t value[3] = {0};
                    uint32_t length = 3;
                    value[0] = 0x11;
                    value[1] = 0x22;
                    value[2] = 0x33;
                    length = 3;
                    bk_ble_read_response_value(length, value, r_req->prf_id, r_req->att_idx);
                    break;
                }
                case TEST_IDX_CHAR_DATALEN_DECL:

                    break;
                case TEST_IDX_CHAR_DATALEN_VALUE:
                {
                    uint8_t *value = (uint8_t *)&s_test_data_len;
                    bk_ble_read_response_value(sizeof(s_test_data_len), value, r_req->prf_id, r_req->att_idx);
                    break;
                }

                case TEST_IDX_CHAR_INTER_DECL:

                    break;

                case TEST_IDX_CHAR_INTER_VALUE:
                {
                    uint8_t *value = (uint8_t *)&s_test_send_inter;
                    bk_ble_read_response_value(sizeof(s_test_send_inter), value, r_req->prf_id, r_req->att_idx);
                    break;
                }

                default:
                    break;
            }
        }
        break;
    }
    case BLE_5_REPORT_ADV:
    {
        if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
        {
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
        }
        else
        {
            ble_adv_report_t *r_ind = (typeof(r_ind))param;
            char dname[64] = {0};
            uint8_t dname_len = sizeof(dname);

            memset(&dname, 0, sizeof(dname));

            ble_get_device_name_from_adv(r_ind->data, r_ind->data_len, (uint8_t *)dname, &dname_len);
            dname[dname_len - 1] = 0;

            if(r_ind->evt_type & (1 << 4))
            {
                os_printf("evt_type:LEG_ADV,");
            }
            else
            {
                os_printf("evt_type:EXT_ADV,");
            }

            os_printf(" adv_addr_type:%d, adv_addr:%02x:%02x:%02x:%02x:%02x:%02x name %s\r\n",
                    r_ind->peer_address_type,
                    r_ind->peer_address.addr[5],
                    r_ind->peer_address.addr[4],
                    r_ind->peer_address.addr[3],
                    r_ind->peer_address.addr[2],
                    r_ind->peer_address.addr[1],
                    r_ind->peer_address.addr[0],
                    dname);

            if ((STATE_DISCOVERING == g_peer_dev.state) && (ble_check_device_name(r_ind->data , r_ind->data_len, &(g_peer_dev.dev))))
            {
                os_printf("dev : %s is discovered\r\n",g_peer_dev.dev.name);
                os_memcpy(g_peer_dev.bdaddr.addr, r_ind->peer_address.addr, BK_BLE_GAP_BD_ADDR_LEN);
                g_peer_dev.addr_type = r_ind->peer_address_type;
                g_peer_dev.state = STATE_DISCOVERED;

                if (ble_at_cmd_sema != NULL)
                {
                    rtos_set_semaphore( &ble_at_cmd_sema );
                }
            }
        }
    }
    break;
    case BLE_5_MTU_CHANGE: {
        ble_mtu_change_t *m_ind = (ble_mtu_change_t *)param;
        os_printf("%s m_ind:conn_idx:%d, mtu_size:%d\r\n", __func__, m_ind->conn_idx, m_ind->mtu_size);
        break;
    }
    case BLE_5_CONNECT_EVENT: {
        if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
        {
            ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
            os_printf("c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                    c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                    c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
            s_test_conn_ind = c_ind->conn_idx;
        }
        else
        {
            ble_conn_att_t *conn_att = (typeof(conn_att))param;
            os_printf("%s ethermind connected, ATT_CON_ID %d atype %d addr 0x%02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
                      conn_att->conn_handle, conn_att->peer_addr_type,
                      conn_att->peer_addr[5],
                      conn_att->peer_addr[4],
                      conn_att->peer_addr[3],
                      conn_att->peer_addr[2],
                      conn_att->peer_addr[1],
                      conn_att->peer_addr[0]
                      );
            s_ethermind_att_handle = conn_att->conn_handle;
            if ((STATE_CONNECTINIG == g_peer_dev.state) && (!os_memcmp(g_peer_dev.bdaddr.addr, conn_att->peer_addr, BK_BLE_GAP_BD_ADDR_LEN)))
            {
                g_peer_dev.state = STATE_CONNECTED;
            }
        }
        break;
    }
    case BLE_5_DISCONNECT_EVENT: {
        if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
        {
            ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
            os_printf("d_ind:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx, d_ind->reason);
            s_test_conn_ind = ~0;
        }
        else
        {
            ble_conn_att_t *att_handle = (typeof(att_handle))param;
            uint8_t index = 0;
            os_printf("%s ethermind disconnect reason %d attid %d atype %d addr 0x%02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
                      att_handle->event_result,
                      att_handle->conn_handle, att_handle->peer_addr_type,
                      att_handle->peer_addr[5],
                      att_handle->peer_addr[4],
                      att_handle->peer_addr[3],
                      att_handle->peer_addr[2],
                      att_handle->peer_addr[1],
                      att_handle->peer_addr[0]
                      );

            memset(&s_ethermind_att_handle, 0, sizeof(s_ethermind_att_handle));

            if(0 == ethermind_find_index_by_info(&index, att_handle->conn_handle))
            {
                ethermind_clean_info_by_index(index);
            }
        }
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

//#if (CONFIG_BTDM_5_2)
        if(bk_ble_get_controller_stack_type() == BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
        {
            ble_create_db_t *cd_ind = (ble_create_db_t *)param;
            os_printf("cd_ind:prf_id:%d, status:%d\r\n", cd_ind->prf_id, cd_ind->status);

            at_cmd_status = cd_ind->status;
            if (ble_at_cmd_sema != NULL)
                rtos_set_semaphore( &ble_at_cmd_sema );
        }
//#endif
        else
        {
            os_printf("%s BLE_5_CREATE_DB ok\n", __func__);
        }
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
        if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
        {
            uint8_t con_idx = *(uint8_t *)param;
            if(s_service_type && s_performance_tx_enable == 1)
            {
                s_performance_tx_bytes += s_test_data_len;
                ble_test_noti_hdl((void *)((uint32)con_idx));
            }
        }
        else
        {
            ble_att_tx_compl_t *tx_comp = (typeof(tx_comp))param;
            os_printf("%s BLE_5_TX_DONE res %d ATT_CON_ID %d ATT_ATTR_HANDLE %d\n", __func__,
                      tx_comp->event_result,
                      tx_comp->conn_handle, tx_comp->attr_handle);

            if(s_ethermind_service_type == 1 && s_ethermind_auto_tx_enable)
            {
//                for (uint8_t i = 0; i < a_wr_c->num_compl; ++i)
//                {
//                    ble_tx_test_active_timer_callback(NULL);
//                }
            }
        }
        break;

    case BLE_5_CONN_UPDATA_EVENT:
    {
        if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
        {
            ble_conn_param_t *updata_param = (ble_conn_param_t *)param;
            os_printf("BLE_5_CONN_UPDATA_EVENT:conn_interval:0x%04x, con_latency:0x%04x, sup_to:0x%04x\r\n", updata_param->intv_max,
            updata_param->con_latency, updata_param->sup_to);
        }
        else
        {
            ble_conn_update_param_compl_ind_t *ind = (typeof(ind))param;
            os_printf("%s BLE_5_CONN_UPDATA_EVENT mac 0x%02X:%02X:%02X:%02X:%02X:%02X status 0x%X interval 0x%X lantency 0x%X tout 0x%X\n", __func__,
                      ind->peer_address.addr[5],
                      ind->peer_address.addr[4],
                      ind->peer_address.addr[3],
                      ind->peer_address.addr[2],
                      ind->peer_address.addr[1],
                      ind->peer_address.addr[0],
                      ind->status, ind->conn_interval, ind->conn_latency, ind->supervision_timeout);
        }
        break;
    }

    case BLE_5_PERIODIC_SYNC_CMPL_EVENT:
        os_printf("BLE_5_PERIODIC_SYNC_CMPL_EVENT \n");
        break;

    case BLE_5_RECV_NOTIFY_EVENT:
    {
        //ethermind

        ble_att_notify_t *tmp = (typeof(tmp))param;


        s_ethermind_performance_rx_bytes += tmp->len;

#ifdef CONFIG_ALI_MQTT
        if (loop_type == LT_COEX)
        {
            os_printf("ATT_CON_ID %d data %s len %d sendto mqtt\n",
                      tmp->conn_handle,
                      tmp->data, tmp->len);

            ble_send_data_2_mqtt(tmp->conn_handle, tmp->len, tmp->data);
        }
        else
#endif
        {
            uint8_t index = 0;
            if(0 == ethermind_find_index_by_info(&index, tmp->conn_handle))
            {
                os_printf("%s ATT_CON_ID %d, send data to nordic len %d %s\n", __func__,
                          tmp->conn_handle, tmp->len, tmp->data);

                bk_ble_att_write(tmp->conn_handle, s_ethermind_nordic_write_attr_handle[index], tmp->data, tmp->len);
            }
            else
            {
                os_printf("%s cant find info ATT_CON_ID %d\n", __func__, tmp->conn_handle);
            }
        }
    }
    break;

    case BLE_5_DISCOVERY_PRIMARY_SERVICE_EVENT:
    {
        ble_discovery_primary_service_t *tmp = (typeof(tmp))param;
        os_printf("%s BLE_5_DISCOVERY_PRIMARY_SERVICE_EVENT count %d\n", __func__, tmp->count);
    }
        break;

    case BLE_5_DISCOVERY_CHAR_EVENT:
    {
        ble_discovery_char_t *tmp = (typeof(tmp))param;
        uint8_t index = 0;
        uint8_t found = 0;
        uint8_t i = 0;

        os_printf("%s BLE_5_DISCOVERY_CHAR_EVENT count %d ATT_CON_ID %d\n", __func__, tmp->count,
                  tmp->conn_handle);

        //

        for (i = 0; i < tmp->count; ++i)
        {
            //os_printf("%s type %d %p\n", __func__, tmp->character[7].uuid_type, &tmp->character[7].uuid_type);

            switch(tmp->character[i].uuid_type)
            {
            case ATT_16_BIT_UUID_FORMAT:
                os_printf("%s char 16bit uuid 0x%04X, %d\n", __func__, tmp->character[i].uuid.uuid_16, tmp->character[i].value_handle);
                break;

            case ATT_128_BIT_UUID_FORMAT:
            {
                //os_printf("%s char 128bit uuid 0x%08X\n", __func__, (uint32_t *)(tmp->character[i].uuid.uuid_128.value + 11));

                if(!os_memcmp(&tmp->character[i].uuid.uuid_128.value, nus_rx_uuid, sizeof(nus_rx_uuid)))
                {
                    found = 1;
                    i = tmp->count - 1;
                    break;
                }
            }
                break;
            }
        }

        if(!found)
        {
            break;
        }


        if(0 == ethermind_find_index_by_info(&index, tmp->conn_handle))
        {
            os_printf("%s nordic info already exist ! att_id %d\n", __func__, tmp->conn_handle);
        }
        else if(0 == ethermind_find_idle_info_index(&index))
        {
        }
        else
        {
            os_printf("%s nordic info list is full !\n", __func__);
            break;
        }


        for (i = 0; i < tmp->count; ++i)
        {
            //os_printf("%s type %d %p\n", __func__, tmp->character[7].uuid_type, &tmp->character[7].uuid_type);

            switch(tmp->character[i].uuid_type)
            {
            case ATT_16_BIT_UUID_FORMAT:
                break;

            case ATT_128_BIT_UUID_FORMAT:
            {
                os_printf("%s char 128bit uuid 0x%08X attr_hdl:d%d\n", __func__, (uint32_t *)(tmp->character[i].uuid.uuid_128.value + 11), tmp->character[i].value_handle);

                if(!os_memcmp(&tmp->character[i].uuid.uuid_128.value, nus_rx_uuid, sizeof(nus_rx_uuid)))
                {
                    os_printf("%s add to nordic info list %d\n", __func__, index);
                    s_ethermind_nordic_used[index] = 1;
                    s_ethermind_nordic_write_attr_handle[index] = tmp->character[i].value_handle;
                    s_ethermind_nordic_att_info[index] = tmp->conn_handle;
                }
                else if(!os_memcmp(&tmp->character[i].uuid.uuid_128.value, nus_tx_uuid, sizeof(nus_tx_uuid)))
                {
                    if(tmp->character[i].desc_index)
                    {
                        for (uint8_t j = 0; j < tmp->character[i].desc_index; ++j)
                        {
                            if (ATT_16_BIT_UUID_FORMAT == tmp->character[i].descriptor[j].uuid_type && GATT_CLIENT_CONFIG == tmp->character[i].descriptor[j].uuid.uuid_16)
                            {
                                s_ethermind_nordic_write_notify_handle[index] = tmp->character[i].descriptor[j].handle;
//                                const uint16_t noti_enable = 0x0001;
//                                os_printf("%s write nordic enable notify\n", __func__);
//                                bk_ble_att_write(&tmp->att_handle, tmp->character[i].descriptor[j].handle, (uint8_t *)&noti_enable, sizeof(noti_enable));
                            }
                        }
                    }
                }
            }
                break;

            default:
                os_printf("%s unknow uuid type %d %d %p\n", __func__, tmp->character[i].uuid_type, i, &tmp->character[i].uuid_type);
                break;
            }
        }


    }
        break;
    case BLE_5_CONN_UPD_PAR_ASK:
    {
        if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
        {
            ble_conn_update_para_ind_t *tmp = (typeof(tmp))param;
            os_printf("%s BLE_5_CONN_UPD_PAR_ASK accept\n", __func__);
            tmp->is_agree = 1;
        }
        else
        {
            ble_conn_update_param_ind_t *ind = (typeof(ind))param;
            os_printf("%s BLE_5_CONN_UPD_PAR_ASK 0x%02X:%02X:%02X:%02X:%02X:%02X imin 0x%X imax 0x%x latency 0x%X tout 0x%X\n", __func__,
                      ind->peer_address.addr[5],
                      ind->peer_address.addr[4],
                      ind->peer_address.addr[3],
                      ind->peer_address.addr[2],
                      ind->peer_address.addr[1],
                      ind->peer_address.addr[0],
                      ind->conn_interval_min, ind->conn_interval_max, ind->conn_latency, ind->supervision_timeout);

//            ind->conn_interval_min++;
//            ind->conn_interval_max++;
            ind->is_agree = 1;
        }
    }
        break;
    case BLE_5_ATT_READ_RESPONSE:
    {
        ble_att_rw_t *tmp = (typeof(tmp))param;
        if(!tmp->event_result)
        {
            os_printf("%s att_id:%d, attr_hand:0x%x, result:%d\r\n",__func__,
                      tmp->conn_handle,
                      s_read_tmp_attr_handle,
                      tmp->event_result
                      );
            os_printf("data-> ");
            for(int i=0;i<tmp->len;i++)
            {
                os_printf("0x%x ", *tmp->data);
            }
            os_printf("\r\n");
            if (ble_at_cmd_sema != NULL)
               rtos_set_semaphore( &ble_at_cmd_sema );
        }else
        {
            os_printf("%s: Recevied Error Response!!! att_id:%d, attr_hand:%d, resp code:%d",__func__,
                      tmp->conn_handle,
                      ((uint8_t *)(&(tmp->event_result)))[1],
                      ((uint8_t *)(&(tmp->event_result)))[0]
                      );
        }
    }
    break;

    case BLE_5_PAIRING_REQ:
    {
        bk_printf("BLE_5_PAIRING_REQ\r\n");
        ble_smp_ind_t *s_ind = (ble_smp_ind_t *)param;
        bk_ble_sec_send_auth_mode(s_ind->conn_idx, GAP_AUTH_REQ_NO_MITM_BOND, GAP_IO_CAP_NO_INPUT_NO_OUTPUT,
            GAP_SEC1_NOAUTH_PAIR_ENC, GAP_OOB_AUTH_DATA_NOT_PRESENT);
        break;
    }

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

int set_ble_name_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    //uint8_t name[] = "BK_BLE_7231n";
    uint8_t name_len = 0;/*os_strlen((const char *)name);*/
#ifdef CONFIG_BT
    uint8_t name[BK_BLE_APP_DEVICE_NAME_MAX_LEN] = {0};
#endif
    int err = kNoErr;
    if (argc != 1)
    {
        err = kParamErr;
        goto error;
    }
#ifdef CONFIG_BT
    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
#endif
        name_len = bk_ble_appm_set_dev_name(os_strlen(argv[0]), (uint8_t *)argv[0]);
        if (name_len == 0)
        {
            os_printf("\nname is empty!!!\n");
            goto error;
        }
        msg = AT_CMD_RSP_SUCCEED;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        return kNoErr;
#ifdef CONFIG_BT
    }
    else
    {
        if (ble_at_cmd_table[0].is_sync_cmd)
        {
            err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
            if(err != kNoErr){
                goto error;
            }
        }

        if(os_strlen(argv[0]) < BK_BLE_APP_DEVICE_NAME_MAX_LEN)
        {
            os_memcpy(name, (uint8_t *)argv[0], os_strlen(argv[0]));
            name_len = os_strlen(argv[0]);
        }
        else
        {
            os_memcpy(name, (uint8_t *)argv[0], BK_BLE_APP_DEVICE_NAME_MAX_LEN);
            name_len = BK_BLE_APP_DEVICE_NAME_MAX_LEN;
        }
        bk_ble_set_local_name(name, name_len, ble_at_cmd_cb);

        if (ble_at_cmd_sema != NULL)
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
                    return 0;
                }
                else
                {
                    err = at_cmd_status;
                    goto error;
                }
            }
        }
    }
#endif

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
#ifdef CONFIG_BT
    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
#endif
        name_len = bk_ble_appm_get_dev_name(name, BK_BLE_APP_DEVICE_NAME_MAX_LEN);
        if (name_len == 0)
        {
            os_printf("\nname is empty!!!\n");
            goto error;
        }

        sprintf(pcWriteBuffer, "%s%s", AT_CMDRSP_HEAD, name);
        return kNoErr;
#ifdef CONFIG_BT
    }
    else
    {
        if (ble_at_cmd_table[0].is_sync_cmd)
        {
            err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
            if(err != kNoErr){
                goto error;
            }
        }

        bk_ble_get_local_name(ble_at_cmd_cb);

        if (ble_at_cmd_sema != NULL)
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
                    return 0;
                }
                else
                {
                    err = at_cmd_status;
                    goto error;
                }
            }
        }
    }
#endif

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

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {

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
    }
    else
    {
        err = bk_ble_set_advertising_params(adv_param.adv_intv_min, adv_param.adv_intv_max, adv_param.chnl_map,
            adv_param.own_addr_type,adv_param.prim_phy, adv_param.second_phy, ble_at_cmd_cb);

        if (err != BK_ERR_BLE_SUCCESS)
        {
            goto error;
        }

        if (ble_at_cmd_sema != NULL)
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
                    return 0;
                }
                else
                {
                    err = at_cmd_status;
                    goto error;
                }
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

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
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
    }
    else
    {
        err = bk_ble_set_advertising_data(adv_data, adv_len, ble_at_cmd_cb);
        if (err != BK_ERR_BLE_SUCCESS)
        {
            goto error;
        }

        if(ble_at_cmd_sema != NULL)
        {
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(err != kNoErr)
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
                    return 0;
                }
                else
                {
                    err = at_cmd_status;
                    goto error;
                }
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

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
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
    }
    else
    {
        err = bk_ble_set_scan_response_data(data_len, scan_rsp_data, ble_at_cmd_cb);
        if (err != BK_ERR_BLE_SUCCESS)
        {
            goto error;
        }
        if(ble_at_cmd_sema != NULL)
        {
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(err != kNoErr)
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
                    return 0;
                }
                else
                {
                    err = at_cmd_status;
                    goto error;
                }
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


    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {

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
    }
    else
    {
        err = bk_ble_set_advertising_enable(enable, ble_at_cmd_cb);
        if (err != BK_ERR_BLE_SUCCESS)
        {
            goto error;
        }

        if(ble_at_cmd_sema != NULL)
        {
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(err != kNoErr)
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
                    return 0;
                }
                else
                {
                    err = at_cmd_status;
                    goto error;
                }
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

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
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
    }
    else
    {

        err = bk_ble_set_scan_parameters(scan_param.own_addr_type, 0x00, scan_param.scan_phy, scan_param.scan_intv, scan_param.scan_wd, ble_at_cmd_cb);

        if (err != BK_ERR_BLE_SUCCESS)
        {
            goto error;
        }

        if(ble_at_cmd_sema != NULL)
        {
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(err != kNoErr)
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
                    goto error;
                }
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

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {

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
    }
    else
    {
        err = bk_ble_set_scan_enable_extended(enable, filt_duplicate, duration, period, ble_at_cmd_cb);

        if (err != BK_ERR_BLE_SUCCESS)
        {
            goto error;
        }
        if(ble_at_cmd_sema != NULL)
        {
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(err != kNoErr)
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
                    return 0;
                }
                else
                {
                    err = at_cmd_status;
                    goto error;
                }
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

    conn_param.intv_max = conn_param.intv_min = os_strtoul(argv[0], NULL, 16) & 0xFFFF;
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
    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
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
        if(ble_at_cmd_sema != NULL)
        {
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(err != kNoErr)
            {
                goto error;
            }
            else
            {
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

    else
    {
        ble_conn_param_normal_t tmp;

        tmp.conn_interval_min = conn_param.intv_min;
        tmp.conn_interval_max = conn_param.intv_max;
        tmp.conn_latency = conn_param.con_latency;
        tmp.supervision_timeout = conn_param.sup_to;
        tmp.initiating_phys = conn_param.init_phys;

        tmp.peer_address_type = peer_addr_type;
        memcpy(tmp.peer_address.addr, bdaddr.addr, sizeof(bdaddr.addr));

        err = bk_ble_create_connection(&tmp, ble_at_cmd_cb);

        if (err != BK_ERR_BLE_SUCCESS)
        {
            goto error;
        }

        if(ble_at_cmd_sema != NULL)
        {
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(err != kNoErr)
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
                    goto error;
                }
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

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        actv_idx = bk_ble_find_master_state_idx_handle(AT_INIT_STATE_CONNECTTING);
        if (actv_idx == AT_BLE_MAX_ACTV)
        {
            os_printf("ble adv not set params before\n");
            err = kNoResourcesErr;
            goto error;
        }

        err = bk_ble_init_stop_conn(actv_idx, ble_at_cmd_cb);
    }
    else
    {
        err = bk_ble_cancel_connect(ble_at_cmd_cb);
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

int ble_disconnect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;
    bd_addr_t connect_addr;
    uint8_t addr_type = 1;

    uint8_t conn_idx;
    uint8_t central_count = 0;

    if(!bk_ble_if_support_central(&central_count) || central_count == 0)
    {
        os_printf("not support central\n");
        err = kParamErr;
        goto error;
    }

    if (argc > 2)
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

    if (argc < 2)
    {
        os_printf("%s warning, you need input addr type\n", __func__);
    }
    else
    {
        addr_type = os_strtoul(argv[1], NULL, 10) & 0xFF;
    }

    if (ble_at_cmd_table[11].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    /// get connect_idx from connect_addr
    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        conn_idx = bk_ble_find_conn_idx_from_addr(&connect_addr);
        if (conn_idx == AT_BLE_MAX_CONN)
        {
            os_printf("ble not connection\n");
            err = kNoResourcesErr;
            goto error;
        }

        err = bk_ble_disconnect(conn_idx, ble_at_cmd_cb);
    }
    else
    {
        err = bk_ble_disconnect_connection(&connect_addr, addr_type, ble_at_cmd_cb);
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

int ble_update_conn_param_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;
    bd_addr_t connect_addr;
    ble_conn_param_t conn_param;
    uint8_t conn_idx;
    uint8_t central_count = 0;
    uint8_t addr_type = 1;

    if(!bk_ble_if_support_central(&central_count) || central_count == 0)
    {
        os_printf("not support central\n");
        err = kParamErr;
        goto error;
    }

    if (argc > 6)
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

    if (argc < 6)
    {
        os_printf("%s warning, you need input addr type\n", __func__);
    }
    else
    {
        addr_type = os_strtoul(argv[5], NULL, 10) & 0xFF;
    }

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
        if(err != kNoErr)
        {
            os_printf("rtos_init_semaphore error\n");
            goto error;
        }
    }

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        /// get connect_idx from connect_addr
        conn_idx = bk_ble_find_conn_idx_from_addr(&connect_addr);
        if (conn_idx == AT_BLE_MAX_CONN)
        {
            os_printf("ble not connection\n");
            err = kNoResourcesErr;
            goto error;
        }
        err = bk_ble_update_param(conn_idx, &conn_param, ble_at_cmd_cb);
    }
    else
    {
        ble_update_conn_param_t tmp;
        tmp.peer_address_type = addr_type;

        tmp.conn_interval_min = conn_param.intv_min;
        tmp.conn_interval_max = conn_param.intv_max;
        tmp.conn_latency = conn_param.con_latency;
        tmp.supervision_timeout = conn_param.sup_to;

        memcpy(tmp.peer_address.addr, connect_addr.addr, sizeof(connect_addr.addr));

        err = bk_ble_update_connection_params(&tmp, ble_at_cmd_cb);
    }

    if (err != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }

    if(ble_at_cmd_sema != NULL)
    {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr)
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
                goto error;
            }
        }
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
    {
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    }
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
    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        if (conn_state == 1)
        {
            sprintf(pcWriteBuffer, "%s%s\r\n", AT_CMDRSP_HEAD, "BLE_CONNECT");
        }
        else
        {
            sprintf(pcWriteBuffer, "%s:%s\r\n", AT_CMDRSP_HEAD, "BLE_DISCONNECT");
        }

    }
    else
    {
        if (conn_state == AT_INIT_STATE_CONNECTTED)
        {
            sprintf(pcWriteBuffer, "%s%s\r\n", AT_CMDRSP_HEAD, "BLE_CONNECTED");
        }
        else if (conn_state == AT_INIT_STATE_IDLE)
        {
            sprintf(pcWriteBuffer, "%s:%s\r\n", AT_CMDRSP_HEAD, "BLE_DISCONNECT");
        }
        else if (conn_state == AT_INIT_STATE_CONNECTTING)
        {
            sprintf(pcWriteBuffer, "%s:%s\r\n", AT_CMDRSP_HEAD, "BLE_CONNECTTING");
        }
        else if (conn_state == AT_INIT_STATE_STOPPING)
        {
            sprintf(pcWriteBuffer, "%s:%s\r\n", AT_CMDRSP_HEAD, "BLE_DISCONNECTTING");
        }
        else
        {
            sprintf(pcWriteBuffer, "%s:%s\r\n", AT_CMDRSP_HEAD, "BLE_DISCONNECT");
        }
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

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        err = bk_ble_get_mac(local_addr);
        if (err != kNoErr) {
            os_printf("bk_ble_get_mac error\n");
            err = kParamErr;
            goto error;
        }
        sprintf(pcWriteBuffer, "%s%s%02x:%02x:%02x:%02x:%02x:%02x\r\n", AT_CMDRSP_HEAD, "BLE_ADDR:", local_addr[5],
            local_addr[4], local_addr[3], local_addr[2], local_addr[1], local_addr[0]);
        return err;
    }
    else
    {
        if (ble_at_cmd_table[14].is_sync_cmd)
        {
            err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
            if(err != kNoErr){
                goto error;
            }
        }

        err = bk_ble_read_local_addr(ble_at_cmd_cb);

        if (err != BK_ERR_BLE_SUCCESS)
        {
            goto error;
        }
        if(ble_at_cmd_sema != NULL)
        {
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(err != kNoErr)
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
                    goto error;
                }
            }
        }
    }

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

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        uint8 conn_idx = 0;
        bd_addr_t connect_addr;

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
    }
    else
    {
        uint8_t conn_handle;

        conn_handle = os_strtoul(argv[0], NULL, 10) & 0xFF;
        err = bk_ble_set_gatt_mtu(conn_handle, 517);

        if (err == 0)
        {
            msg = AT_CMD_RSP_SUCCEED;
            os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
            return err;
        }
        else
        {
            os_printf("%s set mtu err %d\n", __func__, err);
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
    s_ethermind_send_size_value = s_test_data_len = len;
    s_ethermind_send_intv_value = s_test_send_inter = inter;
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
    uint8 con_idx = 0;

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
    con_idx = os_strtoul(argv[1], NULL, 10) & 0xFF;
    os_printf("%s enable %d, con_idx %d\n", __func__, enable, con_idx);

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        ble_test_service_write_handle(enable, con_idx);
    }
    else
    {
        s_ethermind_auto_tx_enable = enable;

        if(enable)
        {
            if(s_ethermind_service_type == 1)
            {
                ble_tx_test_active_timer_callback(NULL);
            }
            else
            {
                if (!rtos_is_timer_init(&s_ethermind_ble_send_test_timer))
                {
                    rtos_init_timer(&s_ethermind_ble_send_test_timer, 1000, ble_tx_test_active_timer_callback, NULL);
                    rtos_start_timer(&s_ethermind_ble_send_test_timer);
                }
            }
        }
        else
        {
            if (rtos_is_timer_init(&s_ethermind_ble_send_test_timer))
            {
                if (rtos_is_timer_running(&s_ethermind_ble_send_test_timer))
                {
                    rtos_stop_timer(&s_ethermind_ble_send_test_timer);
                }

                rtos_deinit_timer(&s_ethermind_ble_send_test_timer);
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


static void ble_sdp_charac_callback(uint32_t type, uint8 conidx, uint16_t hdl, uint16_t len, uint8 *data)
{
    //os_printf("%s recv type %d len %d data[0] 0x%02X\n", __func__, type, len, data[0]);
    s_performance_rx_bytes += len;
}

int ble_register_noti_service_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    s_ethermind_service_type = s_service_type = 0;

    return ble_register_service_handle(pcWriteBuffer, xWriteBufferLen, argc, argv);
}

int ble_register_performance_service_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    s_ethermind_service_type = s_service_type = 1;

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

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        if(g_test_prf_task_id == 0)
        {
            os_printf("%s not reg profile, exit\n", __func__);
            err = kParamErr;
            goto error;
        }

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
    }
    else
    {
        if(type == 1)//tx
        {
            if(enable)
            {
                if(!rtos_is_timer_init(&ble_ethermind_performance_tx_statistics_tmr))
                {
                    s_ethermind_performance_tx_bytes = 0;

                    rtos_init_timer(&ble_ethermind_performance_tx_statistics_tmr, 1000, ble_ethermind_performance_tx_timer_hdl, (void *)0);
                    rtos_start_timer(&ble_ethermind_performance_tx_statistics_tmr);
                }
            }
            else
            {
                if (rtos_is_timer_init(&ble_ethermind_performance_tx_statistics_tmr))
                {
                    s_performance_tx_bytes = 0;
                    if (rtos_is_timer_running(&ble_ethermind_performance_tx_statistics_tmr))
                        rtos_stop_timer(&ble_ethermind_performance_tx_statistics_tmr);
                    rtos_deinit_timer(&ble_ethermind_performance_tx_statistics_tmr);
                }
            }
        }
        else if(type == 2)//rx
        {
            if(enable)
            {
                if(!rtos_is_timer_init(&ble_ethermind_performance_rx_statistics_tmr))
                {
                    s_ethermind_performance_rx_bytes = 0;

                    rtos_init_timer(&ble_ethermind_performance_rx_statistics_tmr, 1000, ble_ethermind_performance_rx_timer_hdl, (void *)0);
                    rtos_start_timer(&ble_ethermind_performance_rx_statistics_tmr);
                }
            }
            else
            {
                if (rtos_is_timer_init(&ble_ethermind_performance_rx_statistics_tmr))
                {
                    s_ethermind_performance_rx_bytes = 0;

                    if (rtos_is_timer_running(&ble_ethermind_performance_rx_statistics_tmr))
                    {
                        rtos_stop_timer(&ble_ethermind_performance_rx_statistics_tmr);
                    }

                    rtos_deinit_timer(&ble_ethermind_performance_rx_statistics_tmr);
                }
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



static void ble_tx_test_active_timer_callback(void *param)
{
    API_RESULT retval = 0;
    uint8_t *tmp_buff = NULL;

    tmp_buff = os_malloc(s_ethermind_send_size_value);

    if (!tmp_buff)
    {
        os_printf("%s alloc send failed\n", __func__);
        return;
    }

    os_memset(tmp_buff, 0, s_ethermind_send_size_value);
    tmp_buff[0] = s_ethermind_send_test_value++;


    retval = bk_ble_send_notify(s_ethermind_att_handle, s_ethermind_send_test_service_handle,
                       s_ethermind_send_char_test_handle, tmp_buff, s_ethermind_send_size_value);

    os_free(tmp_buff);

    if (retval != 0)
    {
        os_printf("%s notify err %d\n", __func__, retval);
    }
    else
    {
        s_ethermind_performance_tx_bytes += s_ethermind_send_size_value;
    }
}

static void ble_tx_test_passive_timer_callback(void *param)
{
    API_RESULT retval = 0;
    uint8_t *tmp_buff = NULL;
    GATT_DB_HANDLE *gdbh = (GATT_DB_HANDLE *)param;
    uint8_t conn_handle;

    tmp_buff = os_malloc(s_ethermind_send_size_value);

    if (!tmp_buff)
    {
        os_printf("%s alloc send failed\n", __func__);
        return;
    }

    os_memset(tmp_buff, 0, s_ethermind_send_size_value);
    tmp_buff[0] = s_ethermind_send_test_value++;

    retval = bk_ble_get_conn_handle_from_device_handle(&conn_handle, &(gdbh->device_id));

    if (retval != 0)
    {
        return;
    }

    retval = bk_ble_send_notify(conn_handle, gdbh->service_id, gdbh->char_id,
                       tmp_buff, s_ethermind_send_size_value);

    os_free(tmp_buff);

    if (retval != 0)
    {
        os_printf("%s notify err %d\n", __func__, retval);
    }
    else
    {
        s_ethermind_performance_tx_bytes += s_ethermind_send_size_value;
    }

}

static API_RESULT ethermind_test_gatt_char_handler
(
    GATT_DB_HANDLE     *handle,
    GATT_DB_PARAMS     *params
)
{
    API_RESULT retval = 0;
    uint16_t value = 0;

    static GATT_DB_HANDLE *gdbh = NULL;

    os_printf("%s device_id %d service %d char %d op 0x%02X\n", __func__,
             handle->device_id, handle->service_id, handle->char_id, params->db_op);

    if (handle->service_id == s_ethermind_send_test_service_handle)
    {
        switch (params->db_op)
        {
        case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
        {
            //BT_UNPACK_LE_2_BYTE (&value, params->value.val);
            memcpy(&value, params->value.val, 2);
            if (handle->char_id == s_ethermind_send_char_test_handle)
            {
                if (GATT_CLI_CNFG_NOTIFICATION == value)
                {
                    os_printf("%s enable notify\n", __func__);

                    if (!gdbh)
                    {
                        gdbh = (GATT_DB_HANDLE *)os_malloc(sizeof(*handle));

                        if (!gdbh)
                        {
                            os_printf("%s cant alloc GATT_DB_HANDLE\n", __func__);
                            retval = BK_FAIL;
                            break;
                        }
                    }

                    memcpy(gdbh, handle, sizeof(*handle));

                    if(s_ethermind_service_type == 1)
                    {
                        s_ethermind_auto_tx_enable = 1;
                        ble_tx_test_passive_timer_callback((void *)(size_t)gdbh);

                    }
                    else if (!rtos_is_timer_init(&s_ethermind_ble_send_test_timer))
                    {
                        rtos_init_timer(&s_ethermind_ble_send_test_timer, 1000, ble_tx_test_passive_timer_callback, (void *)(size_t)gdbh);
                        rtos_start_timer(&s_ethermind_ble_send_test_timer);
                    }

                }
                else
                {
                    os_printf("%s disable notify\n", __func__);
                    s_ethermind_auto_tx_enable = 0;

                    if (rtos_is_timer_init(&s_ethermind_ble_send_test_timer))
                    {
                        if (rtos_is_timer_running(&s_ethermind_ble_send_test_timer))
                        {
                            rtos_stop_timer(&s_ethermind_ble_send_test_timer);
                        }

                        rtos_deinit_timer(&s_ethermind_ble_send_test_timer);
                    }

                    if (gdbh)
                    {
                        os_free(gdbh);
                        gdbh = NULL;
                    }
                }
            }
            else
            {
                retval = BK_FAIL;
            }
        }
        break;

        case GATT_DB_CHAR_PEER_WRITE_REQ:
        {
            if (handle->char_id == s_ethermind_send_size_handle)
            {
                //BT_UNPACK_LE_2_BYTE(&value, k);
                memcpy(&value, params->value.val, 2);
                os_printf("%s write s_ethermind_send_size_handle %d\n", __func__, value);
            }
            else if (handle->char_id == s_ethermind_send_intv_handle)
            {
                //BT_UNPACK_LE_2_BYTE(&value, params->value.val);
                memcpy(&value, params->value.val, 2);
                os_printf("%s write s_ethermind_send_intv_handle %d\n", __func__, value);
            }
            else
            {
                retval = BK_FAIL;
            }
        }
        break;
        }
    }
    else
    {
//        retval = BK_FAIL;
    }

    return retval;
}



static int ble_register_service_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int err = kNoErr;
    char *msg = NULL;
    uint16 my_service_uuid = 0;
    uint16 my_char_uuid = 0x1234;
    struct bk_ble_db_cfg ble_db_cfg;
    uint8 cur_test_prf_task_id = 0;

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

    cur_test_prf_task_id = g_test_prf_task_id;
    g_test_prf_task_id = os_strtoul(argv[0], NULL, 16) & 0xFF;
    my_service_uuid = os_strtoul(argv[1], NULL, 16) & 0xFFFF;
    if(argc >= 3)
    {
        my_char_uuid = os_strtoul(argv[2], NULL, 16) & 0xFFFF;
    }

    if(argc >= 5)
    {
        s_test_data_len = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
        s_test_send_inter = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
    }

    os_memcpy(&(test_service_db[TEST_IDX_CHAR_VALUE].uuid[0]), &my_char_uuid, 2);

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {

        os_memset(&ble_db_cfg, 0, sizeof(ble_db_cfg));

        ble_db_cfg.att_db = (ble_attm_desc_t *)test_service_db;
        ble_db_cfg.att_db_nb = TEST_IDX_NB;
        ble_db_cfg.prf_task_id = g_test_prf_task_id;
        ble_db_cfg.start_hdl = 0;
        ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16);
        os_memcpy(&(ble_db_cfg.uuid[0]), &my_service_uuid, 2);

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
    }
    else
    {
        API_RESULT           retval;
        GATT_DB_SERVICE_INFO service_info;
        uint16_t               num_attr_handles;
        uint16_t               service_handle;
        GATT_DB_UUID_TYPE    char_uuid;
        uint16_t               perm;
        uint16_t               property;
        ATT_VALUE            char_value;

        if (ble_at_cmd_table[16].is_sync_cmd)
        {
            err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
            if (err != kNoErr)
            {
                goto error;
            }
        }

        bk_ble_set_notice_cb(ble_at_notice_cb);

        service_info.is_primary        = 1;
        service_info.uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        service_info.uuid.uuid.uuid_16 = my_service_uuid;
        service_info.link_req          = GATT_DB_SER_SUPPORT_ANY_LINK_TYPE;
        service_info.sec_req           = GATT_DB_SER_NO_SECURITY_PROPERTY;

        num_attr_handles = 10;

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
            /* Save GATT Service Instance for future reference */
            s_ethermind_send_test_service_handle  = service_handle;

            char_uuid.uuid_format   = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16  = my_char_uuid;

            perm                    = GATT_DB_PERM_READ | GATT_DB_PERM_WRITE;
            property                = GATT_DB_CHAR_NOTIFY_PROPERTY | GATT_DB_CHAR_READ_PROPERTY |
                                        GATT_DB_CHAR_WRITE_PROPERTY;

            char_value.val          = (uint8_t *)"";//(UCHAR *)s_ethermind_send_test_value;
            char_value.len          = 0;//strlen((char *)char_value.val);
            char_value.actual_len   = char_value.len;


            retval = bk_ble_gatt_db_add_characteristic
                       (
                           service_handle,
                           &char_uuid,
                           perm,
                           property,
                           &char_value,
                           &s_ethermind_send_char_test_handle
                       );

            if (0 != retval)
            {
                os_printf("%s: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n", __func__, retval);
                goto error;
            }
            else
            {
                /* Add CCCD */
                GATT_DB_UUID_TYPE    desc_uuid;
                UINT16               perm;
                ATT_VALUE            desc_value;

                uint8_t cccd_value[2U]    = { 0x00U, 0x00U };

                desc_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
                desc_uuid.uuid.uuid_16 = GATT_CLIENT_CONFIG;
                perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
                desc_value.val         = cccd_value;
                desc_value.len         = sizeof(cccd_value);
                desc_value.actual_len  = desc_value.len;

                /* Add descriptor CCCD */
                retval = bk_ble_gatt_db_add_characteristic_descriptor
                       (
                           service_handle,
                           s_ethermind_send_char_test_handle,
                           &desc_uuid,
                           perm,
                           &desc_value
                       );
            }
        }

        if (0 != retval)
        {
            os_printf("%s: add send desc failed. Result: 0x%04X\n", __func__, retval);
            goto error;
        }
        else
        {
            char_uuid.uuid_format   = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16  = 0x9abc;

            perm                    = GATT_DB_PERM_READ | GATT_DB_PERM_WRITE;
            property                = GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY;

            char_value.val          = (uint8_t *)&s_ethermind_send_size_value;
            char_value.len          = sizeof(s_ethermind_send_size_value);
            char_value.actual_len   = char_value.len;

            retval = bk_ble_gatt_db_add_characteristic
                   (
                       service_handle,
                       &char_uuid,
                       perm,
                       property,
                       &char_value,
                       &s_ethermind_send_size_handle
                   );
        }

        if (0 != retval)
        {
            os_printf("%s: add send size char failed. Result: 0x%04X\n", __func__, retval);
            goto error;
        }
        else
        {
            char_uuid.uuid_format   = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16  = 0xdef0;

            perm                    = GATT_DB_PERM_READ | GATT_DB_PERM_WRITE;
            property                = GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY;

            char_value.val          = (uint8_t *)&s_ethermind_send_intv_value;
            char_value.len          = sizeof(s_ethermind_send_intv_value);
            char_value.actual_len   = char_value.len;

            retval = bk_ble_gatt_db_add_characteristic
                   (
                       service_handle,
                       &char_uuid,
                       perm,
                       property,
                       &char_value,
                       &s_ethermind_send_intv_handle
                   );
        }

        if (0 != retval)
        {
            os_printf("%s: add send inter char failed. Result: 0x%04X\n", __func__, retval);
            goto error;
        }
        else
        {
            retval = bk_ble_gatt_db_dyn_register();

            if (retval != 0)
            {
                os_printf("%s GATT Database Registration err: 0x%04X\n", __func__, retval);
                goto error;
            }
        }


        retval = bk_ble_gatt_db_init_pl(ethermind_test_gatt_char_handler);

        if (retval != 0)
        {
            os_printf("%s bk_ble_gatt_db_init_pl err: 0x%04X\n", __func__, retval);
            goto error;
        }

        err = 0;

        msg = AT_CMD_RSP_SUCCEED;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        return err;
    }




error:
    msg = AT_CMD_RSP_ERROR;
    g_test_prf_task_id = cur_test_prf_task_id;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}


void ble_test_noti_hdl(void *param)
{
    uint8 *write_buffer;
    uint8 con_idx = (uint32)param;

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

    ret = bk_ble_send_noti_value(con_idx, s_test_data_len, write_buffer, g_test_prf_task_id, TEST_IDX_CHAR_VALUE);
    if(ret != BK_ERR_BLE_SUCCESS)
    {
        os_printf("%s ret err %d\n", __func__, ret);
    }

    os_free(write_buffer);
}

static void ble_performance_tx_timer_hdl(void *param)
{
    uint32_t tmp = s_performance_tx_bytes;
    s_performance_tx_bytes = 0;

    os_printf("%s current tx %d bytes/sec\n", __func__, tmp);
}

static void ble_performance_rx_timer_hdl(void *param)
{
    uint32_t tmp = s_performance_rx_bytes;
    s_performance_rx_bytes = 0;

    os_printf("%s current rx %d bytes/sec\n", __func__, tmp);
}

static void ble_ethermind_performance_tx_timer_hdl(void *param)
{
    uint32_t tmp = s_ethermind_performance_tx_bytes;
    s_ethermind_performance_tx_bytes = 0;

    os_printf("%s current tx %d bytes/sec\n", __func__, tmp);
}

static void ble_ethermind_performance_rx_timer_hdl(void *param)
{
    uint32_t tmp = s_ethermind_performance_rx_bytes;
    s_ethermind_performance_rx_bytes = 0;

    os_printf("%s current rx %d bytes/sec\n", __func__, tmp);
}


void ble_test_service_write_handle(uint8 val, uint8 con_idx)
{
    if (val)
    {
        if(s_service_type)
        {
            if(!s_performance_tx_enable)
            {
                s_performance_tx_enable = 1;
                ble_test_noti_hdl((void *)((uint32)con_idx));
            }

        }
        else if (!rtos_is_timer_init(&ble_noti_tmr))
        {
            s_test_noti_count = 0;
            rtos_init_timer(&ble_noti_tmr, s_test_send_inter, ble_test_noti_hdl, (void *)((uint32)con_idx));
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
    uint8_t addr_type = 1;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

	if (argc > 2) {
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

    if (argc < 2)
    {
        os_printf("%s warning, you need input addr type\n", __func__);
    }
    else
    {
        addr_type = os_strtoul(argv[1], NULL, 10) & 0xFF;
    }

	if (ble_at_cmd_table[17].is_sync_cmd)
	{
		err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
		if(err != kNoErr){
			goto error;
		}
	}

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        /// get connect_idx from connect_addr
        conn_idx = bk_ble_find_conn_idx_from_addr(&connect_addr);
        if (conn_idx == AT_BLE_MAX_CONN)
        {
            os_printf("ble not connection\r\n");
            err = kNoResourcesErr;
            goto error;
        }

        err = bk_ble_read_phy(conn_idx, ble_at_cmd_cb);
    }
    else
    {
        err = bk_ble_hci_read_phy(&connect_addr, addr_type, ble_at_cmd_cb);
    }

    if (err != kNoErr)
    {
        os_printf("read connect tx/rx phy failed\r\n");
        goto error;
    }

    if(ble_at_cmd_sema != NULL)
    {
        err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(err != kNoErr)
        {
            os_printf("get sema fail\r\n");
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
    uint8_t addr_type = 1;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

	if (argc > 5) {
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

    if (argc < 5)
    {
        os_printf("%s warning, you need input addr type\n", __func__);
    }
    else
    {
        addr_type = os_strtoul(argv[4], NULL, 10) & 0xFF;
    }

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        /// get connect_idx from connect_addr
        conn_idx = bk_ble_find_conn_idx_from_addr(&connect_addr);
        if (conn_idx == AT_BLE_MAX_CONN)
        {
            os_printf("ble not connection\r\n");
            err = kNoResourcesErr;
            goto error;
        }

        err = bk_ble_set_phy(conn_idx, &le_set_phy, ble_at_cmd_cb);
    }
    else
    {
        err = bk_ble_hci_set_phy(&connect_addr, addr_type, &le_set_phy, ble_at_cmd_cb);
    }
    if (err != kNoErr)
    {
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
    conn_param.intv_max = conn_param.intv_min;

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
    os_printf("%s %s\r\n", __func__, g_peer_dev.dev.name);
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

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        if (ble_at_cmd_sema != NULL)
        {
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
    }else
    {
        ble_scan_param_t scan_param;
        scan_param.own_addr_type = BLE_STATIC_ADDR;
        scan_param.scan_phy = PHY_1MBPS_BIT;
        scan_param.scan_intv = 0x64;
        scan_param.scan_wd = 0x1e;

        uint8_t filt_duplicate = 0;
        uint16_t duration = 0;
        uint16_t period = 10;


        if(ble_at_cmd_sema != NULL)
        {
            err = bk_ble_set_scan_parameters(scan_param.own_addr_type, 0x00, scan_param.scan_phy, scan_param.scan_intv, scan_param.scan_wd, ble_at_cmd_cb);
            os_printf("%s set scan param\r\n", __func__);
            if (err != BK_ERR_BLE_SUCCESS)
            {
                goto error;
            }
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(err != kNoErr)
            {
                goto error;
            }
            err = bk_ble_set_scan_enable_extended(1, filt_duplicate, duration, period, ble_at_cmd_cb);
            if (err != BK_ERR_BLE_SUCCESS)
            {
                goto error;
            }
            err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);

            if(err != kNoErr)
            {
                goto error;
            }
            os_printf("%s start scan\r\n", __func__);

            rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);

            if(g_peer_dev.state == STATE_DISCOVERED)
            {
                bk_ble_set_scan_enable_extended(0, filt_duplicate, duration, period, ble_at_cmd_cb);
                err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
                if(err != kNoErr)
                {
                    goto error;
                }
                os_printf("%s stop scan\r\n", __func__);

                ble_conn_param_normal_t tmp;

                tmp.conn_interval_min = conn_param.intv_min;
                tmp.conn_interval_max = conn_param.intv_max;
                tmp.conn_latency = conn_param.con_latency;
                tmp.supervision_timeout = conn_param.sup_to;
                tmp.initiating_phys = conn_param.init_phys;

                tmp.peer_address_type = g_peer_dev.addr_type;
                os_memcpy(tmp.peer_address.addr, &(g_peer_dev.bdaddr.addr[0]), BK_BLE_GAP_BD_ADDR_LEN);
                os_printf("%s start conn, peer addr: %x %x %x %x %x %x\r\n", __func__, tmp.peer_address.addr[0], tmp.peer_address.addr[1],
                            tmp.peer_address.addr[2],tmp.peer_address.addr[3],tmp.peer_address.addr[4],tmp.peer_address.addr[5]);
                g_peer_dev.state = STATE_CONNECTINIG;

                err = bk_ble_create_connection(&tmp, ble_at_cmd_cb);

                if (err != BK_ERR_BLE_SUCCESS)
                {
                    goto error;
                }

                err = rtos_get_semaphore(&ble_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
                if(err != kNoErr)
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
                        goto error;
                    }
                    }
            }else
            {
                os_printf("%s SCAN Timeout %d\r\n", __func__, g_peer_dev.state);
                bk_ble_set_scan_enable_extended(0, filt_duplicate, duration, period, ble_at_cmd_cb);
                goto error;
            }
        }

    }

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
    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
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
    }else
    {
        g_peer_dev.state = STATE_DISCONNECTINIG;
        err = bk_ble_disconnect_connection(&g_peer_dev.bdaddr, g_peer_dev.addr_type, ble_at_cmd_cb);
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


#if 0
void s_ble_update_param_callback(MASTER_COMMON_TYPE type, uint8 conidx, void *param)
{
    switch(type)
    {
    case MST_TYPE_UPP_ASK:
    {
        struct mst_comm_updata_para *tmp = (typeof(tmp))param;
        os_printf("%s MST_TYPE_UPP_ASK accept\n", __func__);
        tmp->is_agree = 0;
    }
        break;

    default:
        break;
    }
}

int ble_update_param_reply_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = AT_CMD_RSP_SUCCEED;

    int err = kNoErr;
#if 0
    uint8_t con_idx = 0;
    uint8_t accept = 0;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

    if (argc != 2) {
        os_printf("input param error\r\n");
        err = kParamErr;
        goto error;
    }

    bk_ble_set_notice_cb(ble_at_notice_cb);

    bk_ble_register_app_sdp_common_callback(s_ble_update_param_callback);

    con_idx = os_strtoul(argv[0], NULL, 10) & 0xFF;
    accept = os_strtoul(argv[0], NULL, 10) & 0xFF;

    //err = ble_param_update_reply(con_idx, accept);
    if (err != kNoErr) {
        os_printf("set update param reply failed\r\n");
        msg = AT_CMD_RSP_ERROR;
        goto error;
    }



error:
#endif
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}
#endif

int ble_att_write_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int err = kNoErr;

    char tmp_buf[3] = {0};
   // int scanf_ret = 0;
    uint8_t data_count = 0;
    int8_t i = 0;

    uint8_t conn_handle = 0;
    ATT_ATTR_HANDLE attr_handle = 0;

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        err = kParamErr;
        goto error;
    }

    if (argc < 3)
    {
        os_printf("\nThe number of param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    //att_handle.device_id = os_strtoul(argv[0], NULL, 10) & 0xFF;
    conn_handle = os_strtoul(argv[0], NULL, 10) & 0xFF;
    attr_handle = os_strtoul(argv[1], NULL, 10) & 0xFFFF;

    if(strlen(argv[2]) > sizeof(send_value) * 2)
    {
        os_printf("\nThe number of data is wrong!\n");
        err = kParamErr;
        goto error;
    }

    for (i = strlen(argv[2]) - 2, data_count = 0; i >= 0; i -= 2, data_count++)
    {
        os_memset(tmp_buf, 0, sizeof(tmp_buf));
        os_memcpy(tmp_buf, argv[2] + i, 2);

        send_value[data_count] = os_strtoul(tmp_buf, NULL, 10) & 0xFF;
    }
    os_printf("%s %d %d %d\n", __func__, conn_handle, attr_handle, data_count);

    err = bk_ble_att_write(conn_handle, attr_handle, send_value, data_count);

    if (err != BK_ERR_BLE_SUCCESS)
    {
        goto error;
    }
    else
    {
        msg = AT_CMD_RSP_SUCCEED;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        return err;
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (ble_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&ble_at_cmd_sema);
    return err;
}

static int ble_att_read_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int err = kNoErr;
    char *msg = NULL;
    ATT_HANDLE att_handle;

    if(bk_ble_get_host_stack_type() != BK_BLE_HOST_STACK_TYPE_ETHERMIND)
    {
        err = kParamErr;
        goto error;
    }

    if (argc < 3)
    {
        os_printf("\nThe number of param is wrong!\n");
        err = kParamErr;
        goto error;
    }

    //uint8_t conn_handle = 0;

    att_handle.device_id = os_strtoul(argv[0], NULL, 10) & 0xFF;
    att_handle.att_id = os_strtoul(argv[1], NULL, 10) & 0xFF;
    s_read_tmp_attr_handle = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
//    os_printf("%s %d %d %d\n", __func__, att_handle.device_id, att_handle.att_id, attr_handle);

    if (ble_at_cmd_table[37].is_sync_cmd)
    {
        err = rtos_init_semaphore(&ble_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    bk_ble_set_notice_cb(ble_at_notice_cb);

    bk_ble_att_read(att_handle.att_id, s_read_tmp_attr_handle);

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

static void ble_plr_sdp_comm_callback(MASTER_COMMON_TYPE type,uint8 conidx,void *param)
{
    if (MST_TYPE_ATT_UUID == type)
    {
        struct ble_sdp_char_inf *char_inf = (struct ble_sdp_char_inf*)param;

        if ((!os_memcmp(char_inf->uuid, nus_tx_uuid, 16)) && (conidx < AT_BLE_MAX_CONN))
        {
            plc_ccc_handle[conidx] = char_inf->val_hdl + 1;
        }

        if ((!os_memcmp(char_inf->uuid, nus_rx_uuid, 16)) && (conidx < AT_BLE_MAX_CONN))
        {
            plc_rx_handle[conidx] = char_inf->val_hdl;
        }
    }

    if (MST_TYPE_SDP_END == type)
    {
        bk_ble_gatt_mtu_change(conidx, NULL);
    }

    if(type == MST_TYPE_UPP_ASK)
    {
        struct mst_comm_updata_para *tmp = (typeof(tmp))param;
        os_printf("%s MST_TYPE_UPP_ASK accept\n", __func__);
        tmp->is_agree = 1;
    }

}

static void ble_plr_sdp_charac_callback(CHAR_TYPE type,uint8 conidx,uint16_t hdl,uint16_t len,uint8 *data)
{
    if (CHARAC_NOTIFY == type)
    {
        //os_printf("con %d , len : %d, recv data %s\n", conidx, len, data);
        //bk_ble_gatt_write_value(conidx, plc_rx_handle[conidx], len, data);
#ifdef CONFIG_ALI_MQTT
        if (loop_type == LT_COEX)
        {
            ble_send_data_2_mqtt(conidx, len, data);
        }
        else
        {
            bk_ble_gatt_write_value(conidx, plc_rx_handle[conidx], len, data);
        }
#else
        bk_ble_gatt_write_value(conidx, plc_rx_handle[conidx], len, data);
#endif

    }
    else if (CHARAC_WRITE_DONE == type)
    {
        //os_printf("con %d\n", conidx);
    }
}

int ble_enable_packet_loss_ratio_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int err = kNoErr;
    char *msg = NULL;
    uint8_t cmd = 0;

    if(bk_ble_get_controller_stack_type() != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        err = kParamErr;
        goto error;
    }

    if (argc < 1)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    cmd = os_strtoul(argv[0], NULL, 10) & 0xFF;

    if (cmd == 0)
    {
        if(BK_BLE_HOST_STACK_TYPE_ETHERMIND != bk_ble_get_host_stack_type())
        {
            for (uint8_t j = 0; j < AT_BLE_MAX_CONN; j++)
            {
                if (plc_ccc_handle[j])
                {
                    bk_ble_gatt_write_ccc(j, plc_ccc_handle[j], 0x00);
                }
                else
                {
                    os_printf("the %dth conn is not connected\n", j);
                }
            }
        }
        else
        {
            const uint16_t noti_enable = 0;
            for(uint8_t i = 0; i < sizeof(s_ethermind_nordic_att_info) / sizeof(s_ethermind_nordic_att_info[0]); i++)
            {
                if(s_ethermind_nordic_used[i])
                {
                    bk_ble_att_write(s_ethermind_nordic_att_info[i], s_ethermind_nordic_write_notify_handle[i], (uint8_t *)&noti_enable, sizeof(noti_enable));
                }

            }
        }
    }
    else if (cmd == 1)
    {
        if(BK_BLE_HOST_STACK_TYPE_ETHERMIND != bk_ble_get_host_stack_type())
        {
            os_memset(plc_ccc_handle, 0, sizeof(plc_ccc_handle));
            os_memset(plc_rx_handle, 0, sizeof(plc_ccc_handle));
            bk_ble_register_app_sdp_common_callback(ble_plr_sdp_comm_callback);
            bk_ble_register_app_sdp_charac_callback(ble_plr_sdp_charac_callback);
        }
    }
    else if(cmd == 2)
    {
        if (argc < 4)
        {
            os_printf("input param error\n");
            err = kParamErr;
            goto error;
        }

        uint16_t data_cnt = os_strtoul(argv[1], NULL, 10) & 0xFFFF;
        uint32_t interval = os_strtoul(argv[2], NULL, 10);
        uint16_t data_len = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
        uint8_t cmd[10] = {0x5a,0xa5,0,0};
        os_memcpy(&cmd[2], &data_cnt, sizeof(data_cnt));
        os_memcpy(&cmd[4], &interval, sizeof(interval));
        os_memcpy(&cmd[8], &data_len, sizeof(data_len));
        if(BK_BLE_HOST_STACK_TYPE_ETHERMIND != bk_ble_get_host_stack_type())
        {
            for (uint8_t j =0;j<AT_BLE_MAX_CONN;j++)
            {
                if (plc_rx_handle[j])
                {
                    bk_ble_gatt_write_value(j, plc_rx_handle[j], sizeof(cmd), cmd);
                }
                else
                {
                    os_printf("the %dth conn is not connected\n",j);
                }
            }
        }
        else
        {
            for(uint8_t i = 0; i < sizeof(s_ethermind_nordic_att_info) / sizeof(s_ethermind_nordic_att_info[0]); i++)
            {
                if(s_ethermind_nordic_used[i])
                {
                    os_printf("%s ATT_CON_ID %d set notify param\n", __func__, s_ethermind_nordic_att_info[i]);
                    bk_ble_att_write(s_ethermind_nordic_att_info[i], s_ethermind_nordic_write_attr_handle[i], (uint8_t *)&cmd, sizeof(cmd));
                }
            }
        }
    }
    else if(cmd == 3)
    {
        if(BK_BLE_HOST_STACK_TYPE_ETHERMIND != bk_ble_get_host_stack_type())
        {
            for (uint8_t j =0;j < AT_BLE_MAX_CONN;j++)
            {
                if (plc_ccc_handle[j])
                {
                    bk_ble_gatt_write_ccc(j, plc_ccc_handle[j], 0x01);
                }
                else
                {
                    os_printf("the %dth conn is not connected\n",j);
                }
            }
        }
        else
        {
            const uint16_t noti_enable = 0x0001;
            for(uint8_t i = 0; i < sizeof(s_ethermind_nordic_att_info) / sizeof(s_ethermind_nordic_att_info[0]); i++)
            {
                if(s_ethermind_nordic_used[i])
                {
                    os_printf("%s ATT_CON_ID %d enable notify\n", __func__, s_ethermind_nordic_att_info[i]);
                    bk_ble_att_write(s_ethermind_nordic_att_info[i], s_ethermind_nordic_write_notify_handle[i], (uint8_t *)&noti_enable, sizeof(noti_enable));
                }

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
    return err;
}


#ifdef CONFIG_ALI_MQTT
/*
* AT+BLE=MQTTLOOPBACK, connect, 222.71.10.2, aclsemi, xxxxxxxxx
* AT+BLE=MQTTLOOPBACK, wifi, /ble/lpsr/xxx, /ble/lpci/xxx, 285, 7000
*/
static beken_timer_t *mqtt_loop_timer = NULL;
uint32_t loop_count = 0;
uint32_t loop_interval = 0;
char *loop_topic_ci = NULL;
char *loop_topic_sr = NULL;
uint32_t loop_ci_index = 0;
uint32_t loop_sr_index = 0;

char *ble_mqtt_loop_tx_buffer = NULL;
char *ble_mqtt_loop_rx_buffer = NULL;
void *ble_mqtt_loop_client = NULL;


#define PRODUCT_KEY             "aclsemi"
#define DEVICE_NAME             "bk7256"
#define DEVICE_SECRET           "nMwWRZrjupURGSByK7qu3uCwzEYUHORu"


static void mqtt_loop_timer_callback(void *param)
{
    if (loop_type == LT_WIFI_ONLY)
    {
        iotx_mqtt_topic_info_t topic_msg;
        char msg_pub[350] = {0};
        int rc = -1;
        char rev_data[250] = {0};

        /* Initialize topic information */
        memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));

        topic_msg.qos = IOTX_MQTT_QOS0;
        topic_msg.retain = 0;
        topic_msg.dup = 0;

        /* Generate topic message */
        int msg_len = snprintf(msg_pub, sizeof(msg_pub), "{\"link_id\":\"%d\", \"rev_len\":\"%d\", \"rev_data\":\"", 0, 250);

        if (msg_len < 0) {
            os_printf("Error occur! Exit program.\n");
        }

        sprintf(rev_data, "%d-%d", loop_ci_index++, loop_sr_index);
        os_memcpy(&msg_pub[msg_len], rev_data, 250);
        msg_len += 250;

        msg_pub[msg_len++] = '"';
        msg_pub[msg_len++] = '}';

        topic_msg.payload = (void *)msg_pub;
        topic_msg.payload_len = msg_len;

        rc = IOT_MQTT_Publish(ble_mqtt_loop_client, loop_topic_ci, &topic_msg);
        if (rc < 0) {
            os_printf("error occur when publish.\n");
        }
        os_printf("packet-id=%u, publish topic msg=%s.\n", (uint32_t)rc, msg_pub);
    }


    if (loop_ci_index == loop_count)
    {
        rtos_stop_timer(mqtt_loop_timer);
        os_free(mqtt_loop_timer);
        mqtt_loop_timer = NULL;
        os_printf("stop\n");
    }

}

void ble_mqtt_loop_event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    uintptr_t packet_id = (uintptr_t)msg->msg;

    switch (msg->event_type) {
        case IOTX_MQTT_EVENT_UNDEF:
            os_printf("undefined event occur.\n");
            break;

        case IOTX_MQTT_EVENT_DISCONNECT:
            os_printf("MQTT disconnect.\n");
            break;

        case IOTX_MQTT_EVENT_RECONNECT:
            os_printf("MQTT reconnect.\n");
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_SUCCESS:
            os_printf("subscribe success, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_TIMEOUT:
            os_printf("subscribe wait ack timeout, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_NACK:
            os_printf("subscribe nack, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_SUCCESS:
            os_printf("unsubscribe success, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
            os_printf("unsubscribe timeout, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_NACK:
            os_printf("unsubscribe nack, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_SUCCESS:
            os_printf("publish success, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_TIMEOUT:
            os_printf("publish timeout, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_NACK:
            os_printf("publish nack, packet-id=%u.\n", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_RECVEIVED:
#if 0
			iotx_mqtt_topic_info_pt topic_info = (iotx_mqtt_topic_info_pt)msg->msg;
            os_printf("topic message arrived but without any related handle:\n");
            os_printf("    topic=%.*s,\n",
                          topic_info->topic_len,
                          topic_info->ptopic);
            os_printf("    topic_msg=%.*s.\n",
                          topic_info->payload_len,
                          topic_info->payload);
#endif
            break;

        default:
            os_printf("Should NOT arrive here.\n");
            break;
    }
}

#define REQ_TIMEOUT_MS (5000)
#define TRX_BUF_SIZE (7*1024)

void *ble_mqtt_open(const char *host_name, const char *username,
                        const char *password)
{
    iotx_conn_info_pt pconn_info;
    iotx_mqtt_param_t mqtt_params;

	if (ble_mqtt_loop_tx_buffer == NULL)
	{
		ble_mqtt_loop_tx_buffer = (char *)os_malloc(TRX_BUF_SIZE);

		if (ble_mqtt_loop_tx_buffer == NULL)
		{
			os_printf("not enough memory for ble_mqtt_loop_tx_buffer\n");
			goto error;
		}
	}

	if (ble_mqtt_loop_rx_buffer == NULL)
	{
		ble_mqtt_loop_rx_buffer = (char *)os_malloc(TRX_BUF_SIZE);

		if (ble_mqtt_loop_rx_buffer == NULL)
		{
			os_printf("not enough memory for ble_mqtt_loop_rx_buffer\n");
			goto error;
		}
	}

    /* Device AUTH */
    if (0 != IOT_SetupConnInfo(PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, (void **)&pconn_info)) {
        os_printf("AUTH request failed!\n");
		goto error;
    }

    os_snprintf(pconn_info->host_name, sizeof(pconn_info->host_name)-1, "%s", host_name);
    os_snprintf(pconn_info->username, sizeof(pconn_info->username)-1, "%s", username);
    os_snprintf(pconn_info->password, sizeof(pconn_info->password)-1, "%s", password);

    /* Initialize MQTT parameter */
    memset(&mqtt_params, 0x0, sizeof(mqtt_params));

    mqtt_params.port = pconn_info->port;
    mqtt_params.host = pconn_info->host_name;
    mqtt_params.client_id = pconn_info->client_id;
    mqtt_params.username = pconn_info->username;
    mqtt_params.password = pconn_info->password;
    mqtt_params.pub_key = pconn_info->pub_key;

    mqtt_params.request_timeout_ms = REQ_TIMEOUT_MS;
    mqtt_params.clean_session = 0;
    mqtt_params.keepalive_interval_ms = 60000;
    mqtt_params.pread_buf = ble_mqtt_loop_rx_buffer;
    mqtt_params.read_buf_size = TRX_BUF_SIZE;
    mqtt_params.pwrite_buf = ble_mqtt_loop_tx_buffer;
    mqtt_params.write_buf_size = TRX_BUF_SIZE;

    mqtt_params.handle_event.h_fp = ble_mqtt_loop_event_handle;
    mqtt_params.handle_event.pcontext = NULL;


    /* Construct a MQTT client with specify parameter */
    return IOT_MQTT_Construct(&mqtt_params);


error:

	if (ble_mqtt_loop_rx_buffer)
	{
		os_free(ble_mqtt_loop_rx_buffer);
		ble_mqtt_loop_rx_buffer = NULL;
	}

	if (ble_mqtt_loop_tx_buffer)
	{
		os_free(ble_mqtt_loop_tx_buffer);
		ble_mqtt_loop_tx_buffer = NULL;
	}

	return NULL;
}

enum
{
    COEX_DEMO_MSG_NULL = 0,
    COEX_DEMO_MSG_TX = 0,
};

typedef struct
{
    uint8_t type;
    uint16_t len;
    char *data;
} coex_demo_msg_t;

beken_queue_t coex_demo_msg_que = NULL;
beken_thread_t coex_demo_thread_handle = NULL;
#define COEX_DEMO_MSG_COUNT          (30)

static void ble_send_data_2_mqtt(uint8 con_idx, uint16_t len, uint8 *data)
{
    coex_demo_msg_t demo_msg;
    char msg_pub[350] = {0};
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(coex_demo_msg_t));
    //os_memset(msg_pub, 0, sizeof(msg_pub));
    if (coex_demo_msg_que == NULL)
        return;

    /* Generate topic message */
    int msg_len = snprintf(msg_pub, sizeof(msg_pub), "{\"link_id\":\"%d\", \"rev_len\":\"%d\", \"rev_data\":\"", con_idx, len);
    if (msg_len < 0) {
        os_printf("Error occur! Exit program.\n");
    }

    //os_printf("%s len %d msg_len1 %d %p %s\n", __func__, len, msg_len, &msg_len, msg_pub);
    os_memcpy(&msg_pub[msg_len], data, len);
    msg_len += len;

    msg_pub[msg_len++] = '"';
    msg_pub[msg_len++] = '}';

    //os_printf("total msg_len = %d\n",msg_len);

    demo_msg.data = (char *) os_malloc(msg_len);
    if (demo_msg.data == NULL)
    {
        os_printf("%s, malloc failed\r\n", __func__);
        return;
    }

    os_memcpy(demo_msg.data, msg_pub, msg_len);
    demo_msg.type = COEX_DEMO_MSG_TX;
    demo_msg.len = msg_len;

    rc = rtos_push_to_queue(&coex_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);
    if(kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n",__func__);
        if (demo_msg.data)
        {
            os_free(demo_msg.data);
        }
    }
}

static void ble_mqtt_loop_recv_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    //iotx_mqtt_topic_info_pt ptopic_info = (iotx_mqtt_topic_info_pt) msg->msg;
    if (loop_type == LT_WIFI_ONLY)
    {
        os_printf("ble mqtt loop recv %d\n", loop_sr_index++);
    }
    else if (loop_type == LT_COEX)
    {
        iotx_mqtt_topic_info_pt ptopic_info = (iotx_mqtt_topic_info_pt) msg->msg;
        uint16_t vaild_len = 0;
        uint8_t con_idx = 0;
        uint8_t *data = NULL;
        char *pos = NULL;

        pos = os_strstr(ptopic_info->payload, "link_id");
        if (pos)
        {
            con_idx = atoi(pos + os_strlen("link_id") + 3);
        }

        pos = os_strstr(ptopic_info->payload, "rev_len");
        if (pos)
        {
            vaild_len = atoi(pos + os_strlen("rev_len") + 3);
        }

        //os_printf("ble mqtt recv payload_len %d, vaild_len %d, con_idx %d\n", ptopic_info->payload_len, vaild_len, con_idx);

        if (ptopic_info->payload_len > vaild_len)
        {
            pos = os_strstr(ptopic_info->payload, "rev_data");
            if (pos)
            {
                data = (uint8_t *)pos + os_strlen("rev_data") + 3;
                //os_printf("recv data[0] %c\n", data[0]);
                if(BK_BLE_HOST_STACK_TYPE_ETHERMIND != bk_ble_get_host_stack_type())
                {
                    bk_ble_gatt_write_value(con_idx, plc_rx_handle[con_idx], vaild_len, data);
                }
                else
                {
                    uint8_t index = 0;
                    if(0 == ethermind_find_index_by_att_con_id(&index, con_idx))
                    {
                        os_printf("%s ATT_CON_ID %d, send data to nordic %s\n", __func__,
                                  s_ethermind_nordic_att_info[index],
                                  data);


                        bk_ble_att_write(s_ethermind_nordic_att_info[index], s_ethermind_nordic_write_attr_handle[index], data, vaild_len);
                    }
                    else
                    {
                        os_printf("%s cant find att_id %d\n", __func__, con_idx);
                    }
                }

            }
        }

    }
}

#if CONFIG_ARCH_RISCV
extern u64 riscv_get_mtimer(void);
#endif
void coex_demo_main(void *arg)
{
#if CONFIG_ARCH_RISCV
    uint64_t current_delay_max = 0;
    uint8_t packet_sent_count = 0;
#endif
    while (1) {
        bk_err_t err;
        coex_demo_msg_t msg;

        err = rtos_pop_from_queue(&coex_demo_msg_que, &msg, BEKEN_WAIT_FOREVER);
        if (kNoErr == err)
        {
            switch (msg.type) {
                case COEX_DEMO_MSG_TX:
                    {
                        int rc = -1;
                        iotx_mqtt_topic_info_t topic_msg;
#if CONFIG_ARCH_RISCV
                        uint64_t current_time_begin,current_time_end, current_delay = 0;
#endif
                        /* Initialize topic information */
                        os_memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));

                        topic_msg.qos = IOTX_MQTT_QOS0;
                        topic_msg.retain = 0;
                        topic_msg.dup = 0;
                        topic_msg.payload = (void *)msg.data;
                        topic_msg.payload_len = msg.len;
#if CONFIG_ARCH_RISCV
                        //bk_gpio_pull_up(GPIO_2);
                        current_time_begin = riscv_get_mtimer();
#endif
                        rc = IOT_MQTT_Publish(ble_mqtt_loop_client, loop_topic_ci, &topic_msg);
                        if (rc < 0)
                        {
                            os_printf("error occur when publish.\n");
                        }
                        else
                        {
                            //os_printf("packet-id=%u, publish topic msg=%s.\n", (uint32_t)rc, msg.data);
                        }
                        os_free(msg.data);
#if CONFIG_ARCH_RISCV
                        current_time_end = riscv_get_mtimer();
                        current_delay = current_time_end - current_time_begin;

                        if (current_delay > current_delay_max)
                        {
                            current_delay_max = current_delay;
                        }
                        packet_sent_count++;
                        if (packet_sent_count >= 10)
                        {
                            os_printf("mqtt pulish max delay %ld us.\n",(u32)(current_delay_max/26));
                            packet_sent_count = 0;
                            current_delay_max = 0;
                        }
#endif
                        //bk_gpio_pull_down(GPIO_2);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    rtos_deinit_queue(&coex_demo_msg_que);
    coex_demo_msg_que = NULL;
    coex_demo_thread_handle = NULL;
    rtos_delete_thread(NULL);
}

int coex_demo_task_init(void)
{
    bk_err_t ret = BK_OK;
    if ((!coex_demo_thread_handle) && (!coex_demo_msg_que))
    {
        ret = rtos_init_queue(&coex_demo_msg_que,
                              "coex_demo_msg_que",
                              sizeof(coex_demo_msg_t),
                              COEX_DEMO_MSG_COUNT);
        if (ret != kNoErr) {
            os_printf("coex demo msg queue failed \r\n");
            return BK_FAIL;
        }

        ret = rtos_create_thread(&coex_demo_thread_handle,
                             BEKEN_DEFAULT_WORKER_PRIORITY,
                             "coex_demo",
                             (beken_thread_function_t)coex_demo_main,
                             4096,
                             (beken_thread_arg_t)0);
        if (ret != kNoErr) {
            os_printf("coex demo task fail \r\n");
            rtos_deinit_queue(&coex_demo_msg_que);
            coex_demo_msg_que = NULL;
            coex_demo_thread_handle = NULL;
        }

        return kNoErr;
    }
    else
    {
        return kInProgressErr;
    }
}

int ble_mqtt_loop_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int err = kNoErr, i;
    char *msg = NULL;

	for (i = 0; i < argc; i++)
	{
		os_printf("arg[%d]: %s\n", i, argv[i]);
	}

	if (loop_topic_ci == NULL)
	{
		loop_topic_ci = os_malloc(25);
		os_memset(loop_topic_ci, 0, 25);
	}

	if (loop_topic_sr == NULL)
	{
		loop_topic_sr = os_malloc(25);
		os_memset(loop_topic_sr, 0, 25);
	}


	if (!strcmp(argv[0], "connect"))
	{
		ble_mqtt_loop_client = ble_mqtt_open(argv[1], argv[2], argv[3]);

		if (ble_mqtt_loop_client == NULL)
		{
			os_printf("MQTT connect failed\n");
			goto error;
		}
		else
		{
			os_printf("MQTT connect success\n");
		}
	}

	if (!strcmp(argv[0], "wifi"))
	{
		if (ble_mqtt_loop_client == NULL)
		{
			os_printf("MQTT client was not init\n");
			goto error;
		}

		if (mqtt_loop_timer == NULL)
		{
			mqtt_loop_timer = (beken_timer_t*)os_malloc(sizeof(beken_timer_t));
		}

		uint8_t sta_mac[6];
		bk_wifi_sta_get_mac((uint8_t *)sta_mac);
        sta_mac[5] += 1;

		snprintf(loop_topic_sr, 25, "/ble/lpsr/%02x%02x%02x", sta_mac[3], sta_mac[4], sta_mac[5]);
		snprintf(loop_topic_ci, 25, "/ble/lpci/%02x%02x%02x", sta_mac[3], sta_mac[4], sta_mac[5]);

		loop_interval = os_strtoul(argv[1], NULL, 10) & 0xFFFF;
		loop_count = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		loop_type = LT_WIFI_ONLY;

		loop_sr_index = 0;
		loop_ci_index = 0;

		os_printf("WIFI only test, interval: %u, count: %u\n", loop_interval, loop_count);
		os_printf("topic: %s:%s\n", loop_topic_sr, loop_topic_ci);

		if (IOT_MQTT_Subscribe(ble_mqtt_loop_client, loop_topic_sr, IOTX_MQTT_QOS0, ble_mqtt_loop_recv_handle, NULL) < 0) {

			os_printf("IOT_MQTT_Subscribe() TOPIC_CMD failed\n");
		}


		if (!rtos_is_timer_init(mqtt_loop_timer))
		{
			rtos_init_timer(mqtt_loop_timer, loop_interval, mqtt_loop_timer_callback, NULL);
			rtos_start_timer(mqtt_loop_timer);
		}
	}

	if (!strcmp(argv[0], "stop"))
	{
		if (loop_topic_sr != NULL && ble_mqtt_loop_client != NULL)
		{
			IOT_MQTT_Unsubscribe(ble_mqtt_loop_client, loop_topic_sr);
			os_printf("UnSubscribe\n");
		}

		if (mqtt_loop_timer != NULL)
		{
			rtos_stop_timer(mqtt_loop_timer);
			os_free(mqtt_loop_timer);
			mqtt_loop_timer = NULL;
		}

	}

	if (!os_strcmp(argv[0], "coex"))
	{
		if (ble_mqtt_loop_client == NULL)
		{
			os_printf("MQTT client was not init\n");
			goto error;
		}

		loop_type = LT_COEX;

		uint8_t sta_mac[6];
		bk_wifi_sta_get_mac((uint8_t *)sta_mac);
        sta_mac[5] += 1;

		snprintf(loop_topic_sr, 25, "/ble/lpsr/%02x%02x%02x", sta_mac[3], sta_mac[4], sta_mac[5]);
		snprintf(loop_topic_ci, 25, "/ble/lpci/%02x%02x%02x", sta_mac[3], sta_mac[4], sta_mac[5]);

		loop_sr_index = 0;
		loop_ci_index = 0;

		os_printf("topic: %s:%s", loop_topic_sr, loop_topic_ci);

		if (IOT_MQTT_Subscribe(ble_mqtt_loop_client, loop_topic_sr, IOTX_MQTT_QOS0, ble_mqtt_loop_recv_handle, NULL) < 0)
		{
			os_printf("IOT_MQTT_Subscribe() TOPIC_CMD failed\n");
		}

		coex_demo_task_init();
	}

	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}
#endif

int ble_power_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int err = kNoErr;
    char *msg = NULL;

    if (argc < 1)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    if (os_strcmp(argv[0], "1") == 0)
    {
        bk_ble_init();
    }
    else if (os_strcmp(argv[0], "0") == 0)
    {
        bk_ble_deinit();
    }

    msg = AT_CMD_RSP_SUCCEED;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return err;

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return err;
}

#endif

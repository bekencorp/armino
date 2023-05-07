#include <stdio.h>
#include <stdlib.h>
#include "at_common.h"
#include <driver/gpio.h>
#include "modules/bt.h"
#include "modules/bt_types.h"
#include "ethermind_export/bt_defines.h"
#if CONFIG_BT
#include "bt_include.h"
#include "bt_sbc_common.h"
#include "bt_sbc_encoder.h"
#endif

#if CONFIG_BT && CONFIG_BLE
#include "modules/dm_bt.h"
#endif

#define LOCAL_NAME "soundbar"

#define INQUIRY_LAP 0x9e8B33U
#define INQUIRY_LEN 0x0A

#define CONNECTION_PACKET_TYPE 0xcc18
#define CONNECTION_PAGE_SCAN_REPETITIOIN_MODE 0x01
#define CONNECTION_CLOCK_OFFSET 0x00

#define DISCONNECT_REASON_REMOTE_USER_TERMINATE 0x13
//controller will check reason, now support disconnect reason:
//uint8_t ValidReason[] = {0x05,0x13,0x14,0x15,0x1A,0x29};
#define DISCONNECT_REASON DISCONNECT_REASON_REMOTE_USER_TERMINATE

#define SPP_HANDLE_INVALID                   0xFFU
#define PRINT_FUNC os_printf("%s \n", __func__)
#define SPP_ENABLE_DATA_CNF_LOG
#define SPP_TX_BUFF_SIZE  4096
#define SPP_TX_BUFF_LIST_NUM 2
#define BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER\
       "ADDR: %02X:%02X:%02X:%02X:%02X:%02X"
#define BT_DEVICE_ADDR_ONLY_SPACED_FRMT_SPECIFIER\
       "ADDR: %02X %02X %02X %02X %02X %02X"
#define BT_DEVICE_ADDR_ONLY_PRINT_STR(ref)\
        (ref)[0U],(ref)[1U],(ref)[2U],(ref)[3U],(ref)[4U],(ref)[5U]
#define BT_DEVICE_ADDR_FRMT_SPECIFIER\
        "ADDR: %02X:%02X:%02X:%02X:%02X:%02X, TYPE: %02X"
#define BT_DEVICE_ADDR_SPACED_FRMT_SPECIFIER\
        "ADDR: %02X %02X %02X %02X %02X %02X, TYPE: %02X"
#define BT_IGNORE_UNUSED_PARAM(v) (void)(v)

typedef enum{
    STATE_DISCONNECT,
    STATE_CONNECTED,
    STATE_PROFILE_DISCONNECT,
    STATE_PROFILE_CONNECTED_AS_CLIENT,
    STATE_PROFILE_CONNECTED_AS_SERVER,
}connect_state_s;

typedef struct{
    uint16_t conn_handle;
    uint8_t conn_state;
    bd_addr_t peer_addr;
    uint8_t spp_init;
    SPP_HANDLE client_spp_handle;
    SPP_HANDLE server_spp_handle;
    uint32_t spp_record_handle;
    uint8_t local_server_channel;
    uint8_t peer_server_channel;
    uint8_t tx_confirm;
    uint32_t tx_throught_len;
    uint32_t tx_throught_total_len;
    uint32_t crc;
    uint64_t tx_time;
    uint8_t rx_through;
    uint32_t rx_through_len;
    uint64_t rx_time;
    float speed;
}spp_env_s;

static bt_err_t at_cmd_status = BK_ERR_BT_SUCCESS;

static beken_semaphore_t bt_at_cmd_sema = NULL;

static uint16_t conn_handle = 0xff;
static bk_bt_linkkey_storage_t s_bt_linkkey;
static spp_env_s spp_env;


static char *tx_through_cmd_start = "spp tx throught start";
static char *tx_through_cmd_end = "spp tx throught end";
static API_RESULT bt_spp_event_notify_cb
                   (
                       /* IN */  SPP_HANDLE    spp_handle,
                       /* IN */  SPP_EVENTS    spp_event,
                       /* IN */  API_RESULT    status,
                       /* IN */  void *        data,
                       /* IN */  UINT16        data_length
                   );
static int bt_start_inquiry_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_create_connection_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_disconnect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_spp_init_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_spp_connect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_spp_tx_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_write_scan_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_read_scan_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_a2dp_sink_connect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_a2dp_sink_disconnect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#ifdef CONFIG_AUDIO
static int bt_enable_a2dp_sink_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_enable_hfp_unit_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif
static int bt_enable_opp_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_spp_through_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static void bt_spp_through_poll(char *tx_data);


static uint32_t crc_table(uint32_t index);
static uint32_t crc32(uint32_t crc, const uint8_t *buf, int len);
static unsigned int make_crc32_table(void);
static uint32_t calc_crc32(uint32_t crc, const uint8_t *buf, int len);
static void bt_spp_clear();

#if CONFIG_ARCH_RISCV
extern u64 riscv_get_mtimer(void);
#endif
extern int bk_rand(void);
static int sema_value;

static void bt_at_sema_set()
{
    if (bt_at_cmd_sema != NULL)
    {
        sema_value++;
//        os_printf("-->sem set%d\r\n", sema_value);
        rtos_set_semaphore(&bt_at_cmd_sema);
    }
}

static bk_err_t bt_at_sema_get(uint32_t timeout_ms)
{
    bk_err_t re = 1;
    if (bt_at_cmd_sema != NULL)
    {
        re = rtos_get_semaphore(&bt_at_cmd_sema, timeout_ms);
        sema_value--;
    }
//    os_printf("-->sem get%d\r\n", sema_value);
    return re;
}

static uint32_t bt_at_event_cb(bt_event_enum_t event, void *param)
{
    switch (event)
    {
        case BK_DM_BT_EVENT_INQUIRY_RESULT:
            {
                uint8_t *addr = (uint8_t *)param;
                os_printf("BT Inquiryed addr: %x %x %x %x %x %x \r\n",*(addr+5), *(addr+4), *(addr+3), *(addr+2),*(addr+1),*(addr));
            }
            break;
        case BK_DM_BT_EVENT_DISCONNECT:
            {
                uint8_t *addr = (uint8_t *)param;
                os_printf("Disconnected from %x %x %x %x %x %x \r\n",addr[5],addr[4],addr[3],addr[2],addr[1],addr[0]);
                if (!os_memcmp(addr, spp_env.peer_addr.addr, 6))
                {
                    bt_spp_clear();
                }
            }
            break;
#if 0
        case BK_DM_BT_EVENT_CMD_COMPLETE:
            {
                os_printf("BT Event Complete!!! \r\n");
            }
            break;
#endif
        case BK_DM_BT_EVENT_CONNECTION_COMPLETE:
            {
                uint8_t *addr = (uint8_t *)param;
                os_printf("Connected to %02x:%02x:%02x:%02x:%02x:%02x\n",addr[5],addr[4],addr[3],addr[2],addr[1],addr[0]);
            }
            break;
        case BK_DM_BT_EVENT_LINKKEY_NOTIFY:
        {
            bk_bt_linkkey_storage_t *linkkey = (typeof(linkkey))param;
            os_printf("%s recv linkkey %02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
                    linkkey->addr[5],
                    linkkey->addr[4],
                    linkkey->addr[3],
                    linkkey->addr[2],
                    linkkey->addr[1],
                    linkkey->addr[0]);
            memcpy(&s_bt_linkkey, linkkey, sizeof(s_bt_linkkey));
        }
            break;
        case BK_DM_BT_EVENT_LINKKEY_REQ:
        {
            uint8_t *addr = (typeof(addr))param;
            if(!memcmp(addr, s_bt_linkkey.addr, sizeof(s_bt_linkkey.addr)))
            {
                os_printf("%s found linkkey %02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
                        addr[5],
                        addr[4],
                        addr[3],
                        addr[2],
                        addr[1],
                        addr[0]);
                bk_bt_gap_linkkey_reply(1, &s_bt_linkkey);
            }
            else
            {
                bk_bt_linkkey_storage_t tmp;
                os_printf("%s notfound linkkey %02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
                         addr[5],
                         addr[4],
                         addr[3],
                         addr[2],
                         addr[1],
                         addr[0]);
                memset(&tmp, 0, sizeof(tmp));
                memcpy(tmp.addr, addr, sizeof(tmp.addr));
                bk_bt_gap_linkkey_reply(0, &tmp);
            }
        }
            break;
        default:
            break;
    }
    return 0;
}

static void bt_at_cmd_cb(bt_cmd_t cmd, bt_cmd_param_t *param)
{
    at_cmd_status = param->status;
//    os_printf("%s %d \r\n", __func__, cmd);
    switch (cmd)
    {
        case BT_CMD_CREATE_CONNECT:
            if(param->status == BK_ERR_BT_SUCCESS)
            {
                conn_handle = *((uint16_t *)(param->param));
            }
            break;
        case BT_CMD_DISCONNECT:
            if(conn_handle == spp_env.conn_handle)
            {
              bt_spp_clear();
            }
            conn_handle = 0xff;
            break;
        case BT_CMD_SDP:
            if(at_cmd_status)
            {
                spp_env.peer_server_channel = *((uint8_t *)param->param);
            }
            break;
        case BT_CMD_READ_SCAN_ENABLE:
            {
                uint8_t scan_enable = 0xFF;
                if(param->status == BK_ERR_BT_SUCCESS)
                {
                    scan_enable = *(uint8_t *)(param->param+4);
                }
                os_printf("Read scan enable, status:%d, Scan Enable: 0x%x \r\n", param->status, scan_enable);
            }
            break;
        case BT_CMD_WRITE_SCAN_ENABLE:
            os_printf("Write scan enable, status:%d \r\n", param->status);
            break;
        default:
            break;
    }   
    bt_at_sema_set();
}

const at_command_t bt_at_cmd_table[] = {
    {0, "INQUIRY", 0, "start inquiry", bt_start_inquiry_handle},
    {1, "CONNECT", 0, "create connection", bt_create_connection_handle},
    {2, "DISCONNECT", 1, "disconnect",  bt_disconnect_handle},

    {3, "SPP_CONNECT", 1, "spp connect", bt_spp_connect_handle},

    {4, "WRITE_SCAN_ENABLE", 1, "write_scan_enable", bt_write_scan_enable_handle},
    {5, "READ_SCAN_ENABLE", 1, "read_scan_enable", bt_read_scan_enable_handle},
#ifdef CONFIG_AUDIO
    {6, "A2DP_SINK_TEST", 0, "enable a2dp sink test", bt_enable_a2dp_sink_test_handle},
    {7, "HFP_UNIT_TEST", 0, "enable hfp unit test", bt_enable_hfp_unit_test_handle},
#endif
    {8, "OPP_TEST", 0, "enable opp test", bt_enable_opp_test_handle},

    {9, "SPP_TX", 1, "spp_tx", bt_spp_tx_handle},
    {10, "SPP_INIT", 0, "spp_init", bt_spp_init_handle},
    {11, "SPP_THROUGH_TEST", 1, "spp_through_test", bt_spp_through_test_handle},
    {12, "A2DP_SINK_CONNECT", 1, "a2dp sink connect", bt_a2dp_sink_connect_handle},
    {13, "A2DP_SINK_DISCONNECT", 1, "a2dp sink disconnect", bt_a2dp_sink_disconnect_handle},
};

static void bt_spp_clear()
{
    spp_env.conn_handle = 0xFF;
    spp_env.conn_state = STATE_DISCONNECT;
    spp_env.crc = 0xFFFFFFFF;
    os_memset(&spp_env.peer_addr, 0, 6);
    spp_env.peer_server_channel = 0xFF;
    spp_env.rx_through = 0;
    spp_env.rx_through_len = 0;
    spp_env.rx_time = 0;
    spp_env.speed = 0;
    spp_env.server_spp_handle = SPP_HANDLE_INVALID;
    spp_env.tx_confirm = 0;
    spp_env.tx_throught_len = 0;
    spp_env.tx_throught_total_len = 0;
    spp_env.tx_time = 0;
}

API_RESULT bt_spp_event_notify_cb
           (
               /* IN */  SPP_HANDLE spp_handle,
               /* IN */  SPP_EVENTS spp_event,
               /* IN */  API_RESULT      status,
               /* IN */  void          * data,
               /* IN */  UINT16          data_length
           )
{
    UINT32 index;
    UCHAR * l_data;
    l_data = (UCHAR*)( data );
#if 0
    os_printf("\n"\
           "SPP HANDLE : %u\n"\
           "EVENT      : %d\n"\
           "RESULT     : 0x%04X\n",
            (unsigned int) spp_handle, spp_event, status);
    if (API_SUCCESS != status)
    {
        os_printf("\nSPP Command failure\n");
        return API_FAILURE;
    }
#endif /* 0 */
    switch(spp_event)
    {
    case SPP_CONNECT_CNF:
        os_printf("SPP_CONNECT_CNF -> 0x%04X\n", status);
        os_printf("SPP Instance Connected : %u\n",(unsigned int) spp_handle);
        os_printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));
        if (0x00 == status)
        {
            spp_env.conn_state = STATE_PROFILE_CONNECTED_AS_CLIENT;
            bt_at_sema_set();
        }
        break;
    case SPP_CONNECT_IND:
        os_printf("SPP_CONNECT_IND -> 0x%04X\n", status);
        os_printf("SPP Instance Connected : %u\n",(unsigned int) spp_handle);
        os_printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

        /* Set the global handle */
        /* g_spp_handle = spp_handle; */
        if(spp_env.conn_state != STATE_PROFILE_CONNECTED_AS_CLIENT || spp_env.conn_state != STATE_PROFILE_CONNECTED_AS_SERVER)
        {
            spp_env.conn_state = STATE_PROFILE_CONNECTED_AS_SERVER;
            uint16_t conn_handle = bk_bt_get_conn_handle(l_data);
            spp_env.conn_handle = conn_handle;
            spp_env.server_spp_handle = spp_handle;
            os_memcpy(&spp_env.peer_addr.addr[0], l_data, 6);
            os_printf("Conn Handle: 0x%02x\n, Server spp handle: 0x%02x\n", conn_handle, spp_env.server_spp_handle);
        }
        break;
    case SPP_DISCONNECT_CNF:
        os_printf("SPP_DISCONNECT_CNF -> Disconnection Successful\n");
        os_printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));
        break;
    case SPP_DISCONNECT_IND:
        os_printf("SPP_DISCONNECT_IND -> Disconnection Successful\n");
        os_printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));
        if(spp_env.conn_state == STATE_PROFILE_CONNECTED_AS_CLIENT || spp_env.conn_state == STATE_PROFILE_CONNECTED_AS_SERVER)
        {
            spp_env.conn_state = STATE_PROFILE_DISCONNECT;
        }
        break;
    case SPP_STOP_CNF:
        os_printf("SPP_STOP_CNF -> Stop Successful\n");
        break;
    case SPP_SEND_CNF:
    {
        #if 0
        UCHAR * buffer;
        API_RESULT retval;
        #endif
        os_printf("Received spp send cnf\n");
        os_printf("   spp handle = %d\n", spp_handle);
//            os_printf("    Buffer = %p\n", l_data);
        os_printf("    Actual Data Length = %d\n", data_length);
        if (0x00 != status)
        {
            os_printf ("status: *** 0x%04X\n", status);
            break;
        }

        if(spp_env.tx_confirm)
        {
            os_printf("\n----------------CHAR DUMP-----------------------\n");
            for (index = 0U; index < data_length; index++)
            {
                os_printf("%c ", l_data[index]);
            }
            os_printf("\n------------------------------------------------\n");

            spp_env.tx_confirm = 0;
//            os_printf("free buff->%p \r\n", data);
            os_free(data);
            bt_at_sema_set();
            break;
        }

        if(spp_env.tx_throught_total_len > 0)
        {
            spp_env.tx_throught_len += data_length;
            uint64_t current_time = riscv_get_mtimer();
            float spend_time = (float)(current_time - spp_env.tx_time)/26/1000000;
            os_printf("---->spend time: %f s\r\n", spend_time);
            float speed = (float)spp_env.tx_throught_len/1024/spend_time;
            spp_env.crc = calc_crc32(spp_env.crc, (uint8_t *)l_data, data_length);
            spp_env.speed = speed;
            os_printf("Spp tx length: %d, speed: %.3fKB/s \r\n", spp_env.tx_throught_len, speed);
            if(spp_env.tx_throught_len < spp_env.tx_throught_total_len)
            {
                bt_spp_through_poll(data);
            }else
            {
                bt_at_sema_set();
            }
        }
    }
    break;
    case SPP_RECVD_DATA_IND:
        {
            do{
                if(!spp_env.rx_through)
                {
                    os_printf("SPP_RECVD_DATA_IND -> Data received successfully\n");
                    os_printf("\n----------------CHAR DUMP-----------------------\n");
                    for (index = 0U; index < data_length; index++)
                    {
                        os_printf("%c ", l_data[index]);
                    }
                    os_printf("\n------------------------------------------------\n");
                    #if 0
                    os_printf("\n----------------HEX DUMP------------------------\n");
                    for (index = 0U; index < data_length; index++)
                    {
                        os_printf("%02X ", l_data[index]);
                    }
                    os_printf("\n------------------------------------------------\n");
                    #endif
                }else
                {
                    if(data_length == os_strlen(tx_through_cmd_end) && !os_strncmp((char *)l_data, tx_through_cmd_end, data_length))
                    {
                        os_printf("\n----------------CHAR DUMP-----------------------\n");
                        for (index = 0U; index < data_length; index++)
                        {
                            os_printf("%c ", l_data[index]);
                        }
                        os_printf("\n------------------------------------------------\n");
                        break;
                    }
//                    for (index = 0U; index < data_length; index++)
//                    {
    //                    os_printf("0x%02x ", l_data[index]);
//                    }
                    spp_env.rx_through_len+=data_length;
                    spp_env.crc = calc_crc32(spp_env.crc, l_data, data_length);
                    uint64_t current_time = riscv_get_mtimer();
                    float spend_time = (float)(current_time-spp_env.rx_time)/26/1000000;
                    os_printf("-----> spend time: %f s\r\n", spend_time);
                    float speed = (float)spp_env.rx_through_len/ 1024 / spend_time;
                    spp_env.speed = speed;
                    os_printf("Spp received data len: %d, speed :%.4fKB/s\r\n", spp_env.rx_through_len, speed);
                }
            }while(0);
            if(!os_strncmp((char *)l_data, tx_through_cmd_start, data_length))
            {
                os_printf("Spp received spp tx through start cmd \r\n");
                spp_env.crc = 0xffffffff;
                make_crc32_table();
                spp_env.rx_through = 1;
                spp_env.rx_time = riscv_get_mtimer();
            }
            if(!os_strncmp((char *)l_data, tx_through_cmd_end, data_length))
            {
                os_printf("Spp received spp tx through end cmd \r\n");
                os_printf("Spp tx through test finish, rx total len: %d, crc value: 0x%x, speed: %.4fKB/s \r\n", spp_env.rx_through_len, spp_env.crc, spp_env.speed);
                spp_env.rx_through = 0;
                spp_env.crc = 0xffffffff;
                spp_env.rx_through_len = 0;
                spp_env.rx_time = 0;
                spp_env.speed = 0;
            }
        }
        break;
    default:
        os_printf("\nUnknown command type\n");
        break;
    } /* switch */
    BT_IGNORE_UNUSED_PARAM(spp_handle);
    return 0x00;
}

int bt_at_cmd_cnt(void)
{
    return sizeof(bt_at_cmd_table) / sizeof(bt_at_cmd_table[0]);
}

static int bt_start_inquiry_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;
    if (bt_at_cmd_table[0].is_sync_cmd)
    {
        err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    if(bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        bk_bt_gap_set_event_callback(bt_at_event_cb);
        err = bk_bt_inquiry(INQUIRY_LAP, INQUIRY_LEN, 0, bt_at_cmd_cb);
        if(!err)
        {
            msg = AT_CMD_RSP_SUCCEED;
            os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
            if (bt_at_cmd_sema != NULL)
                rtos_deinit_semaphore(&bt_at_cmd_sema);
            return err;
        }else
        {
            goto error;
        }
    }
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

static int bt_create_connection_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;
    if (bt_at_cmd_table[1].is_sync_cmd)
    {
        err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    if(bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        bk_bt_gap_set_event_callback(bt_at_event_cb);
        bd_addr_t addr;
        uint8_t allow_role_switch = 0;
        err = get_addr_from_param(&addr, argv[0]);
        if(err) goto error;

        allow_role_switch = os_strtoul(argv[1], NULL, 16) & 0xFF;
        err = bk_bt_connect(&(addr.addr[0]),
                              CONNECTION_PACKET_TYPE,
                              CONNECTION_PAGE_SCAN_REPETITIOIN_MODE,
                              0,
                              CONNECTION_CLOCK_OFFSET,
                              allow_role_switch,
                              bt_at_cmd_cb);
        if(!err)
        {
            msg = AT_CMD_RSP_SUCCEED;
            os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
            if (bt_at_cmd_sema != NULL)
                rtos_deinit_semaphore(&bt_at_cmd_sema);
            return err;
        }else
        {
            goto error;
        }
    }
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

static int bt_disconnect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;
    if (bt_at_cmd_table[2].is_sync_cmd)
    {
        err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    if(bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        bk_bt_gap_set_event_callback(bt_at_event_cb);
        bd_addr_t addr;
        err = get_addr_from_param(&addr, argv[0]);
        if(err) goto error;

        err = bk_bt_disconnect(&(addr.addr[0]), DISCONNECT_REASON, bt_at_cmd_cb);
        if(bt_at_cmd_sema == NULL) goto error;
        err = bt_at_sema_get(5*1000);
        if(!err)
        {
            msg = AT_CMD_RSP_SUCCEED;
            os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
            if (bt_at_cmd_sema != NULL)
                rtos_deinit_semaphore(&bt_at_cmd_sema);
            return err;
        }else
        {
            goto error;
        }
    }
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

static int bt_spp_connect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;
    if (bt_at_cmd_table[3].is_sync_cmd)
    {
        err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }
    if(bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        if(spp_env.conn_state == STATE_PROFILE_CONNECTED_AS_CLIENT || spp_env.conn_state == STATE_PROFILE_CONNECTED_AS_SERVER || spp_env.conn_state == STATE_PROFILE_DISCONNECT)
        {
            os_printf("Spp exisit one connection now, please disconnect first!!\r\n");
            goto error;
        }
        bk_bt_gap_set_event_callback(bt_at_event_cb);
        err = get_addr_from_param(&spp_env.peer_addr, argv[0]);
        if(err) goto error;
        if(!spp_env.spp_init)
        {
            bk_bt_spp_init(bt_spp_event_notify_cb);
            spp_env.client_spp_handle = SPP_HANDLE_INVALID;
            bk_bt_spp_start((uint32_t *)&spp_env.client_spp_handle, &spp_env.local_server_channel, &spp_env.spp_record_handle);
            spp_env.spp_init = 1;
            os_printf("Spp init, spp handle: 0x%02x , record handle: 0x%02x  \r\n", spp_env.client_spp_handle, spp_env.spp_record_handle);
        }
        if(spp_env.conn_state != STATE_DISCONNECT)
        {
            os_printf("With remote device is connected, please disconnect first \r\n");
            goto error;
        }
        bk_bt_connect(&(spp_env.peer_addr.addr[0]), 
                        CONNECTION_PACKET_TYPE,
                        CONNECTION_PAGE_SCAN_REPETITIOIN_MODE,
                        0,
                        CONNECTION_CLOCK_OFFSET,
                        1,
                        bt_at_cmd_cb);
        if(bt_at_cmd_sema == NULL) goto error;
        err = bt_at_sema_get(10*1000);
        if(err != kNoErr) goto error;
        if(at_cmd_status == 0x00) 
        {
            spp_env.conn_state = STATE_CONNECTED;
            spp_env.conn_handle = conn_handle;
        }else
        {
            os_printf("BT Connect fail !!!!!!\r\n");
            goto error;
        }
        bk_bt_sdp(spp_env.conn_handle, &(spp_env.peer_addr.addr[0]), bt_at_cmd_cb);
        err = bt_at_sema_get(10*1000);
        if(err != kNoErr) goto error;
        if(!at_cmd_status) goto error;
        
        bk_bt_spp_connect(&(spp_env.peer_addr.addr[0]), spp_env.peer_server_channel, (uint32_t)spp_env.client_spp_handle, bt_at_cmd_cb);
        err = bt_at_sema_get(10*1000);
        if(!err)
        {
            msg = AT_CMD_RSP_SUCCEED;
            os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
            if (bt_at_cmd_sema != NULL)
                rtos_deinit_semaphore(&bt_at_cmd_sema);
//            os_printf("------------>spp connect cnf1 %d\r\n", spp_env.conn_state);
            return err;
        }else
        {
            goto error;
        }
    }
error:
    if(spp_env.conn_state == STATE_CONNECTED)
    {
        os_printf("Spp connect fail, disconnect acl link \r\n!!");
        bk_bt_disconnect(&spp_env.peer_addr.addr[0], DISCONNECT_REASON, bt_at_cmd_cb);
    }
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    os_memset(&spp_env, 0, sizeof(spp_env_s));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

static int bt_spp_tx_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;
    if (bt_at_cmd_table[9].is_sync_cmd)
    {
        err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }
    if(bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        bk_bt_gap_set_event_callback(bt_at_event_cb);
        if(spp_env.conn_state != STATE_PROFILE_CONNECTED_AS_CLIENT && spp_env.conn_state != STATE_PROFILE_CONNECTED_AS_SERVER)
        {
            os_printf("Please connet spp first !! \r\n");
            goto error;
        }
        uint16_t tx_len = os_strlen((char *)argv[0]);
        char * tx_data = (char *)os_malloc(tx_len);
//        os_printf("alloc buffer->%p\r\n", tx_data);
        os_memcpy(tx_data, argv[0], tx_len);
        os_printf("spp tx data %d: %s \r\n", tx_len, tx_data);
        if(tx_len)
        {
            spp_env.tx_confirm = 1;
            uint32_t spp_hanle  = ((spp_env.conn_state == STATE_PROFILE_CONNECTED_AS_CLIENT) ? spp_env.client_spp_handle : spp_env.server_spp_handle);
            bk_bt_spp_tx(spp_hanle, tx_data, tx_len, NULL);
            err = rtos_get_semaphore(&bt_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
            if(!err)
            {
                return err;
            }else
            {
                if(tx_data)
                {
                    os_printf("spp tx fail, over time !!\r\n");
                    os_free(tx_data);
                    tx_data = NULL;
                }
                goto error;
            }
        }else
        {
            os_printf("Data is null !!\r\n");
            return err;
        }
    }
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}
extern API_RESULT BT_spp_send
           (
               /* IN */ SPP_HANDLE spp_handle,
               /* IN */ UCHAR * data,
               /* IN */ UINT16 data_length
           );

static void bt_spp_through_poll(char *tx_data)
{
    if(spp_env.tx_throught_len < spp_env.tx_throught_total_len)
    {
        uint32_t remain_len = spp_env.tx_throught_total_len - spp_env.tx_throught_len;
        uint32_t tx_len = (remain_len>=SPP_TX_BUFF_SIZE) ? SPP_TX_BUFF_SIZE:remain_len;
        uint8_t rand = bk_rand() & 0xff;
        os_memset(tx_data, rand, tx_len);
        uint32_t spp_hanle  = ((spp_env.conn_state == STATE_PROFILE_CONNECTED_AS_CLIENT) ? spp_env.client_spp_handle : spp_env.server_spp_handle);
        bk_bt_spp_tx(spp_hanle, tx_data, tx_len, NULL);
    }
}

static int bt_spp_through_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;
    if (bt_at_cmd_table[11].is_sync_cmd)
    {
        err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }
    if(bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        if(spp_env.conn_state != STATE_PROFILE_CONNECTED_AS_CLIENT && spp_env.conn_state != STATE_PROFILE_CONNECTED_AS_SERVER)
        {
            os_printf("Please connet spp first !! \r\n");
            goto error;
        }
        uint32_t tx_data_length = os_strtoul(argv[0], NULL, 16) & 0xFFFFFFFF;
        os_printf("spp tx throught test, data len:%d \r\n", tx_data_length);
        if(tx_data_length == 0)
        {
            tx_data_length = 0xffff;
            os_printf("data len is 0, set to default, data len:%d \r\n", tx_data_length);
        }
        char *tx_buff = (char *)os_malloc(os_strlen(tx_through_cmd_start));
        os_memcpy(tx_buff, tx_through_cmd_start, os_strlen(tx_through_cmd_start));
//        os_printf("alloc buffer->%p\r\n", tx_buff);
        spp_env.tx_confirm = 1;
        uint32_t spp_hanle  = ((spp_env.conn_state == STATE_PROFILE_CONNECTED_AS_CLIENT) ? spp_env.client_spp_handle : spp_env.server_spp_handle);
        bk_bt_spp_tx(spp_hanle, tx_buff, os_strlen(tx_through_cmd_start), NULL);
        err = bt_at_sema_get(AT_SYNC_CMD_TIMEOUT_MS);
        if(err)
        {
            if(tx_buff)
            {
                os_printf("spp tx through init fail, tx cmd over time !!\r\n");
                os_free(tx_buff);
                tx_buff = NULL;
            }
            goto error;
        }

        spp_env.tx_throught_total_len = tx_data_length;
        spp_env.crc = 0xFFFFFFFF;
        make_crc32_table();
        spp_env.tx_time = riscv_get_mtimer();
        char *tx_data = (char *)os_malloc(SPP_TX_BUFF_SIZE);
        bt_spp_through_poll(tx_data);
        err = bt_at_sema_get(AT_SYNC_CMD_TIMEOUT_MS*tx_data_length);
        os_free(tx_data);
        if(err)
        {
            os_printf("spp tx through tx fail, tx cmd over time !!\r\n");
            goto error;
        }

        tx_buff = (char *)os_malloc(os_strlen(tx_through_cmd_end));
        os_memcpy(tx_buff, tx_through_cmd_end, os_strlen(tx_through_cmd_end));
//        os_printf("alloc buffer->%p\r\n", tx_buff);
        spp_env.tx_confirm = 1;
        bk_bt_spp_tx(spp_hanle, tx_buff, os_strlen(tx_through_cmd_end), NULL);
        err = bt_at_sema_get(AT_SYNC_CMD_TIMEOUT_MS);
        if(err)
        {
            if(tx_buff)
            {
                os_printf("spp tx through end fail, tx cmd over time !!\r\n");
                os_free(tx_buff);
                tx_buff = NULL;
            }
            goto error;
        }
        os_printf("Spp tx thtrouth finish    \n");
        os_printf("TX throuth CRC value: 0x%x , speed: %.3fKB/s \r\n", spp_env.crc, spp_env.speed);
        spp_env.speed = 0;
        spp_env.tx_confirm = 0;
        spp_env.tx_throught_len = 0;
        spp_env.tx_throught_total_len = 0;
        spp_env.tx_time = 0;
        spp_env.crc = 0xffffffff;
        return err;
    }
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

static int bt_spp_init_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    int err = kNoErr;
    if(bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        bk_bt_gap_set_visibility(BK_BT_CONNECTABLE, BK_BT_DISCOVERABLE);
        if(!spp_env.spp_init)
        {
            bk_bt_gap_set_event_callback(bt_at_event_cb);
            //bk_bt_write_scan_enable(3, bt_at_cmd_cb);
            bk_bt_spp_init(bt_spp_event_notify_cb);
            spp_env.client_spp_handle = SPP_HANDLE_INVALID;
            bk_bt_spp_start((uint32_t *)&spp_env.client_spp_handle, &spp_env.local_server_channel, &spp_env.spp_record_handle);
            spp_env.spp_init = 1;
        }
        os_printf("SPP INIT, SPP__HANDL:0x%x\r\n", spp_env.client_spp_handle);
    }
    return err;
}

static int bt_write_scan_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;
    if (bt_at_cmd_table[4].is_sync_cmd)
    {
        err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    if(bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        bk_bt_gap_set_event_callback(bt_at_event_cb);
        uint8_t scan_enable = os_strtoul(argv[0], NULL, 10) & 0xFFFFFFFF;
        if(scan_enable > 0x03)
        {
            os_printf("%s para error, scan_enable:%d \r\n", scan_enable);
            goto error;
        }
//        os_printf("%s, %d \r\n", __func__, scan_enable);
        //serr = bk_bt_write_scan_enable(scan_enable, bt_at_cmd_cb);
        //if(err) goto error;

        //err = rtos_get_semaphore(&bt_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        err = bk_bt_gap_set_visibility(BK_BT_CONNECTABLE, BK_BT_DISCOVERABLE);
        if(!err)
        {
            msg = AT_CMD_RSP_SUCCEED;
            os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
            if (bt_at_cmd_sema != NULL)
                rtos_deinit_semaphore(&bt_at_cmd_sema);
            return err;
        }else
        {
            goto error;
        }
    }
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

static int bt_read_scan_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;
    if (bt_at_cmd_table[4].is_sync_cmd)
    {
        err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    if(bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        bk_bt_gap_set_event_callback(bt_at_event_cb);

        err = bk_bt_read_scan_enable(bt_at_cmd_cb);
        if(err) goto error;

        err = rtos_get_semaphore(&bt_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if(!err)
        {
            msg = AT_CMD_RSP_SUCCEED;
            os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
            if (bt_at_cmd_sema != NULL)
                rtos_deinit_semaphore(&bt_at_cmd_sema);
            return err;
        }else
        {
            goto error;
        }
    }
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

#ifdef CONFIG_AUDIO
static int bt_enable_a2dp_sink_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;

#if CONFIG_A2DP_SINK_DEMO
    uint8_t aac_supported = 0;

    if (argc < 1)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    if (os_strcmp(argv[0], "1") == 0)
    {
        aac_supported = 1;
    }
    else if (os_strcmp(argv[0], "0") == 0)
    {
        aac_supported = 0;
    }
    else
    {
        os_printf("the input param is error\n");
        err = kParamErr;
        goto error;
    }

    extern int a2dp_sink_demo_init(uint8_t aac_supported);
    err = a2dp_sink_demo_init(aac_supported);
#else
    err = BK_ERR_NOT_SUPPORT;
#endif

    if (!err)
    {
        msg = AT_CMD_RSP_SUCCEED;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        return err;
    }
    else
    {
        goto error;
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return err;
}

static int bt_enable_hfp_unit_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;

#if CONFIG_HFP_HF_DEMO
    uint8_t msbc_supported = 0;

    if (argc < 1)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    if (os_strcmp(argv[0], "1") == 0)
    {
        msbc_supported = 1;
    }
    else if (os_strcmp(argv[0], "0") == 0)
    {
        msbc_supported = 0;
    }
    else
    {
        os_printf("the input param is error\n");
        err = kParamErr;
        goto error;
    }

    extern int hfp_hf_demo_init(uint8_t msbc_supported);
    err = hfp_hf_demo_init(msbc_supported);
#else
    err = BK_ERR_NOT_SUPPORT;
#endif

    if (!err)
    {
        msg = AT_CMD_RSP_SUCCEED;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        return err;
    }
    else
    {
        goto error;
    }

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}
#endif

static int bt_a2dp_sink_connect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;
    bd_addr_t remote_bdaddr;

    err = get_addr_from_param(&remote_bdaddr, argv[0]);
    if (err) goto error;

    err = bk_bt_a2dp_sink_connect(remote_bdaddr.addr);

    if (err)
    {
        bk_printf("%s a2dp sink connect err %d\n", __func__, err);
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
    return err;
}

static int bt_a2dp_sink_disconnect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;
    bd_addr_t remote_bdaddr;

    err = get_addr_from_param(&remote_bdaddr, argv[0]);

    if (err) goto error;

    err = bk_bt_a2dp_sink_disconnect(remote_bdaddr.addr);

    if(err)
    {
        bk_printf("%s a2dp sink disconnect err %d\n", __func__, err);
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
    return err;
}

#if CONFIG_FATFS
#include "ff.h"
#endif

bt_opp_server_cb_t bt_opp_server_cb;
#if CONFIG_FATFS
static FIL opp_file;
static char opp_file_name[100] = {0};
static uint8_t opp_file_is_open = 0;
#endif

void bt_opp_server_push_started(uint8_t *obj_name, uint16_t len)
{
    os_printf("OPP Push Object Name: %s, len %d\r\n", obj_name, len);

#if CONFIG_FATFS
    os_memset(opp_file_name, 0, sizeof(opp_file_name));

    sprintf(opp_file_name, "1:/%s", obj_name);
    FRESULT fr = f_open(&opp_file, opp_file_name, FA_CREATE_ALWAYS | FA_WRITE);
    if (fr != FR_OK)
    {
        os_printf("open %s fail, error = %d.\r\n", opp_file_name, fr);
    }
    else
    {
        opp_file_is_open = 1;
    }
#endif
}

void bt_opp_server_push_finished(void)
{
    os_printf("OPP Push Object Finish!\r\n");

#if CONFIG_FATFS
    if (opp_file_is_open)
    {
        FRESULT fr = f_close(&opp_file);
        if (fr != FR_OK)
        {
            os_printf("close %s fail, error = %d.\r\n", opp_file_name, fr);
        }

        opp_file_is_open = 0;
    }
#endif
}

void bt_opp_server_push_data_ind(uint8_t *data, uint16_t data_len)
{
#if CONFIG_FATFS
    if (opp_file_is_open)
    {
        uint32 bw = 0;
        /* write data to file */
        FRESULT fr = f_write(&opp_file, (void *)data, data_len, &bw);
        if (fr != FR_OK)
        {
            os_printf("write %s fail, error = %d.\r\n", opp_file_name, fr);
        }
    }
#endif
}

static int bt_enable_opp_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;

    uint8_t role = 0;

    if (argc < 1)
    {
        os_printf("input param error\n");
        err = kParamErr;
        goto error;
    }

    if (os_strcmp(argv[0], "1") == 0)
    {
        role = 1;//server
    }
    else if (os_strcmp(argv[0], "0") == 0)
    {
        role = 0;//client
    }
    else
    {
        os_printf("the input param is error\n");
        err = kParamErr;
        goto error;
    }

    err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
    if (err != kNoErr){
        goto error;
    }

    if (bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
       err = bk_bt_gap_set_visibility(BK_BT_CONNECTABLE, BK_BT_DISCOVERABLE);

        if (!err)
        {
            if (role)
            {
                bt_opp_server_cb.push_started = bt_opp_server_push_started;
                bt_opp_server_cb.push_finished = bt_opp_server_push_finished;
                bt_opp_server_cb.push_data_ind = bt_opp_server_push_data_ind;
                err = bk_bt_opp_server_init(&bt_opp_server_cb);
            }
            else
            {
                //client todo
            }

            if (!err)
            {
                msg = AT_CMD_RSP_SUCCEED;
                os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
                if (bt_at_cmd_sema != NULL)
                    rtos_deinit_semaphore(&bt_at_cmd_sema);
                return err;
            }
            else
            {
                goto error;
            }
        }
        else
        {
            goto error;
        }

    }
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

#define DO1(buf) crc = crc_table(((int)crc ^ (*buf++)) & 0xff) ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);

static uint32_t crc_table(uint32_t index)
{
	uint32_t c = index;
	uint32_t poly = 0xedb88320L;
	int k;

	for (k = 0; k < 8; k++)
		c = c & 1 ? poly ^ (c >> 1) : c >> 1;

	return c;
}

uint32_t crc32(uint32_t crc, const uint8_t *buf, int len)
{
    if (buf == NULL) return 0L;

  //  crc = crc ^ 0xffffffffL;
    while (len >= 8)
    {
      DO8(buf);
      len -= 8;
    }
    if (len) do {
      DO1(buf);
    } while (--len);
    return crc;// ^ 0xffffffffL;
}


static unsigned int crc32_table[256];

unsigned int make_crc32_table(void)
{
	uint32_t c;
	int i = 0;
	int bit = 0;

	for(i = 0;i < 256;i++)
	{
		c = (unsigned int)i;

		for(bit = 0;bit < 8;bit++)
		{
			if(c&1)
			{
				c = (c>>1)^(0xEDB88320);
			}
			else
			{
				c = c >> 1;
			}
		}

		crc32_table[i] = c;
//		os_printf("crc32_table[%d] = %08x\r\n",i,crc32_table[i]);
	}

	return 0;
}

uint32_t calc_crc32(uint32_t crc, const uint8_t *buf, int len)
{
	while(len--)
	{
		crc = (crc >> 8)^(crc32_table[(crc^*buf++)&0xff]);
	}

	return crc;
}



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

#define A2DP_SOURCE_SBC_FRAME_COUNT 5

typedef enum{
    STATE_DISCONNECT = 0,
    STATE_CONNECTING,
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


typedef struct
{
    uint8_t inited;
    uint16_t conn_handle;
    bd_addr_t peer_addr;
    uint8_t conn_state;
    uint8_t start_status;
    uint32_t mtu;
}a2dp_env_s;


static bt_err_t at_cmd_status = BK_ERR_BT_SUCCESS;

static beken_semaphore_t bt_at_cmd_sema = NULL;

static uint16_t conn_handle = 0xff;

static spp_env_s spp_env;
static a2dp_env_s a2dp_env;


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
#ifdef CONFIG_AUDIO
static int bt_enable_a2dp_sink_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_enable_hfp_unit_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_enable_a2dp_source_connect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_enable_a2dp_source_disconnect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

static int bt_enable_a2dp_source_start_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_enable_a2dp_source_suspend_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

static int bt_enable_a2dp_source_write_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

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



static void bt_at_event_cb(bt_event_t event, void *param)
{
    switch (event)
    {
        case BT_EVENT_INQUIRY_RESULT:
            {
                uint8_t *addr = (uint8_t *)param;
                os_printf("BT Inquiryed addr: %x %x %x %x %x %x \r\n",*(addr+5), *(addr+4), *(addr+3), *(addr+2),*(addr+1),*(addr));
            }
            break;
        case BT_EVENT_DISCONNECT:
            os_printf("BT Disconnected, conn_handle:0x%02x \r\n", (*(uint16_t *)param));
            if(conn_handle == (*(uint16_t *)param))
            {
                conn_handle = 0xFF;
            }
            if(spp_env.conn_handle == (*(uint16_t *)param))
            {
                bt_spp_clear();
            }
            break;
        case BT_EVENT_COMPLETE:
            {
                os_printf("BT Event Complete!!! \r\n");
            }
            break;
        case BT_EVENT_CONNECTION_COMPLETE:
            {
                os_printf("BT Event CONNECTION Complete, conn_handle:0x%02x \r\n", (*(uint16_t *)param));
            }
            break;
        default:
            break;
    }
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

#if CONFIG_AUDIO
    {9, "A2DP_SOURCE_CONNECT", 0, "enable a2dp source connect", bt_enable_a2dp_source_connect_handle},
    {10, "A2DP_SOURCE_DISCONNECT", 0, "enable a2dp source disconnect", bt_enable_a2dp_source_disconnect_handle},
    {11, "A2DP_SOURCE_START", 0, "enable a2dp source disconnect", bt_enable_a2dp_source_start_handle},
    {12, "A2DP_SOURCE_SUSPEND", 0, "enable a2dp source disconnect", bt_enable_a2dp_source_suspend_handle},
	{13, "A2DP_SOURCE_WRITE_TEST", 0, "enable a2dp source write test", bt_enable_a2dp_source_write_test_handle},
#endif
    {14, "SPP_TX", 1, "spp_tx", bt_spp_tx_handle},
    {15, "SPP_INIT", 0, "spp_init", bt_spp_init_handle},
    {16, "SPP_THROUGH_TEST", 1, "spp_through_test", bt_spp_through_test_handle},
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

static void user_a2dp_connection_change(uint8_t status, uint8_t reason)
{

    if (a2dp_env.conn_state != status)
    {
        a2dp_env.conn_state = status;

        if(status == A2DP_CONNECTION_STATUS_CONNECTED || status == A2DP_CONNECTION_STATUS_IDLE)
        {
            if (bt_at_cmd_sema != NULL)
            {
                rtos_set_semaphore( &bt_at_cmd_sema );
            }
        }

    }

    bk_printf("%s %d %d\n", __func__, status, reason);
}

static void user_a2dp_start_cnf(uint8_t result, uint8_t reason, uint32_t mtu)
{
    bk_printf("%s %d %d\n", __func__, result, reason);

    if(result == 0 && a2dp_env.start_status == 0 )
    {
        a2dp_env.start_status = 1;
        a2dp_env.mtu = mtu;

        if (bt_at_cmd_sema != NULL)
        {
            rtos_set_semaphore( &bt_at_cmd_sema );
        }
    }
}

static void user_a2dp_suspend_cnf(uint8_t result, uint8_t reason)
{
    bk_printf("%s %d %d\n", __func__, result, reason);

    if(result == 0 && a2dp_env.start_status == 1 )
    {
        a2dp_env.start_status = 0;

        if (bt_at_cmd_sema != NULL)
        {
            rtos_set_semaphore( &bt_at_cmd_sema );
        }
    }
}

const static bt_a2dp_source_cb_t bt_a2dp_source_cb =
{
    .a2dp_connection_change = user_a2dp_connection_change,
    .a2dp_start_cnf = user_a2dp_start_cnf,
    .a2dp_suspend_cnf = user_a2dp_suspend_cnf,
};

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
        bk_bt_set_event_callback(bt_at_event_cb);
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
        bk_bt_set_event_callback(bt_at_event_cb);
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
        bk_bt_set_event_callback(bt_at_event_cb);
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
        bk_bt_set_event_callback(bt_at_event_cb);
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
    if (bt_at_cmd_table[13].is_sync_cmd)
    {
        err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }
    if(bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        bk_bt_set_event_callback(bt_at_event_cb);
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
    if (bt_at_cmd_table[15].is_sync_cmd)
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
        bk_bt_write_scan_enable(3, bt_at_cmd_cb);
        if(!spp_env.spp_init)
        {
            bk_bt_set_event_callback(bt_at_event_cb);
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
        bk_bt_set_event_callback(bt_at_event_cb);
        uint8_t scan_enable = os_strtoul(argv[0], NULL, 10) & 0xFFFFFFFF;
        if(scan_enable > 0x03)
        {
            os_printf("%s para error, scan_enable:%d \r\n", scan_enable);
            goto error;
        }
//        os_printf("%s, %d \r\n", __func__, scan_enable);
        err = bk_bt_write_scan_enable(scan_enable, bt_at_cmd_cb);
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
        bk_bt_set_event_callback(bt_at_event_cb);

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

#include <driver/sbc_types.h>
#include <driver/aud_types.h>
#include <driver/aud.h>
#include <components/aud_intf.h>
#include <driver/sbc.h>
#include "modules/aac_decoder.h"


typedef struct
{
    uint8_t type;
    uint16_t len;
    char *data;
} bt_audio_demo_msg_t;

enum
{
    BT_AUDIO_MSG_NULL = 0,
    BT_AUDIO_D2DP_START_MSG = 1,
    BT_AUDIO_D2DP_STOP_MSG = 2,
    BT_AUDIO_D2DP_DATA_IND_MSG = 3,
    BT_AUDIO_VOICE_START_MSG = 4,
    BT_AUDIO_VOICE_STOP_MSG = 5,
    BT_AUDIO_VOICE_IND_MSG = 6,
};

#define CODEC_AUDIO_SBC                      0x00U
#define CODEC_AUDIO_AAC                      0x02U

#define CODEC_VOICE_CVSD                     0x00U
#define CODEC_VOICE_MSBC                     0x01U

bt_a2dp_sink_cb_t bt_audio_a2dp_sink_cb;
uint8_t bt_audio_a2dp_codec = CODEC_AUDIO_SBC;
bt_hfp_unit_cb_t bt_audio_hfp_unit_cb;
uint8_t bt_audio_hfp_codec = CODEC_VOICE_CVSD;

sbcdecodercontext_t bt_audio_sbc_decoder;
SbcEncoderContext bt_audio_sbc_encoder;
beken_queue_t bt_audio_demo_msg_que = NULL;
beken_thread_t bt_audio_demo_thread_handle = NULL;

static beken_timer_t bt_a2dp_source_write_timer;
static SbcEncoderContext s_sbc_software_encoder_ctx;

#define BT_AUDIO_DEMO_MSG_COUNT          (30)
#define SCO_MSBC_SAMPLES_PER_FRAME      120
#define SCO_CVSD_SAMPLES_PER_FRAME      60
//#define MSBC_FRAME_BUFFER_MAX_SIZE      60
static void *aac_decoder = NULL;

uint8_t mic_sco_data[1000] = {0};
uint16_t mic_data_count = 0;

static int bt_audio_mic_data_handler(uint8_t *data, unsigned int len)
{
    uint8_t sent_threshold = ((CODEC_VOICE_MSBC == bt_audio_hfp_codec) ? SCO_MSBC_SAMPLES_PER_FRAME*2 : SCO_CVSD_SAMPLES_PER_FRAME*2);

    //os_printf("[send_mic_data_to_air]  %d \r\n",len);
    os_memcpy(&mic_sco_data[mic_data_count], data, len);
    mic_data_count += len;

    while (mic_data_count >= sent_threshold)
    {
        if (CODEC_VOICE_MSBC == bt_audio_hfp_codec)
        {
            int32_t produced = sbc_encoder_encode(&bt_audio_sbc_encoder, (int16_t*)(&mic_sco_data));
            //os_printf("[send_mic_data_to_air_msbc]  %d \r\n",produced);
            bk_bt_voice_out_write((uint8_t*)&bt_audio_sbc_encoder.stream[-2], produced+2);
            //bk_bt_voice_out_write(mic_sco_data, MSBC_FRAME_BUFFER_MAX_SIZE);
        }
        else
        {
            bk_bt_voice_out_write(mic_sco_data, sent_threshold);
        }

        mic_data_count -= sent_threshold;
        os_memmove(mic_sco_data, &mic_sco_data[sent_threshold], mic_data_count);
    }

    return len;
}

uint8_t bt_audio_a2dp_convert_to_dac_sample_rate(uint8_t codec_type, uint32_t sample_rate)
{
    uint8_t dac_sample_rate = AUD_DAC_SAMP_RATE_44_1K;

    if (CODEC_AUDIO_SBC == codec_type)
    {
        switch(sample_rate)
        {
            case 16000:
                dac_sample_rate = AUD_DAC_SAMP_RATE_16K;
                break;

            case 32000:
                dac_sample_rate = AUD_DAC_SAMP_RATE_32K;
                break;

            case 44100:
                dac_sample_rate = AUD_DAC_SAMP_RATE_44_1K;
                break;

            case 48000:
                dac_sample_rate = AUD_DAC_SAMP_RATE_48K;
                break;

            default:
                os_printf("Unsupported sbc sample rate %d \r\n", sample_rate);
                break;
        }
    }
    else if (CODEC_AUDIO_AAC == codec_type)
    {
        switch(sample_rate)
        {
            case 44100:
                dac_sample_rate = AUD_DAC_SAMP_RATE_44_1K;
                break;

            case 48000:
                dac_sample_rate = AUD_DAC_SAMP_RATE_48K;
                break;

            default:
                os_printf("Unsupported aac sample rate %d \r\n", sample_rate);
                break;
        }
    }
    else
    {
        os_printf("%s, Unsupported codec %d \r\n", __func__, codec_type);
    }

    return dac_sample_rate;
}

void bt_audio_demo_main(void *arg)
{
    while (1) {
        bk_err_t err;
        bt_audio_demo_msg_t msg;

        err = rtos_pop_from_queue(&bt_audio_demo_msg_que, &msg, BEKEN_WAIT_FOREVER);
        if (kNoErr == err)
        {
            switch (msg.type) {
                case BT_AUDIO_D2DP_START_MSG:
                    {
                        bk_err_t ret = BK_OK;
                        union codec_info *p_codec_info = (union codec_info *)msg.data;
                        uint8_t dac_sample_rate = 0;
                        uint16_t frame_size = 0;

                        os_printf("BT_AUDIO_D2DP_START_MSG \r\n");

                        if (CODEC_AUDIO_SBC == p_codec_info->codec_type)
                        {
                            dac_sample_rate = bt_audio_a2dp_convert_to_dac_sample_rate(p_codec_info->codec_type, p_codec_info->sbc_codec.sample_rate);
                            frame_size = p_codec_info->sbc_codec.sample_rate * 20 / 1000 *2 *2;
                            bk_sbc_decoder_init(&bt_audio_sbc_decoder);
                        }
                        else if (CODEC_AUDIO_AAC == p_codec_info->codec_type)
                        {
                            dac_sample_rate = bt_audio_a2dp_convert_to_dac_sample_rate(p_codec_info->codec_type, p_codec_info->aac_codec.sample_rate);
                            frame_size = 4096;
                            aac_decoder = (void*)(os_malloc(aac_decoder_get_ram_size_without_in_buffer()));
                            if (!aac_decoder)
                            {
                                os_printf("%s, malloc aac_decoder failed!!!\r\n", __func__);
                            }
                            else
                            {
                                bt_a2dp_aac_decoder_init(aac_decoder, p_codec_info->aac_codec.sample_rate, p_codec_info->aac_codec.channels);
                            }
                        }
                        else
                        {
                            os_printf("%s, Unsupported codec %d \r\n", __func__, p_codec_info->codec_type);
                        }

                        os_printf("dac_sample_rate %d \r\n", dac_sample_rate);

                        aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
                        aud_intf_spk_setup_t aud_intf_spk_setup = DEFAULT_AUD_INTF_SPK_SETUP_CONFIG();
                        aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;

                        aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
                        aud_intf_drv_setup.task_config.priority = 3;
                        aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
                        aud_intf_drv_setup.aud_intf_tx_mic_data = NULL;
                        ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);
                        if (ret != BK_ERR_AUD_INTF_OK)
                        {
                            os_printf("bk_aud_intf_drv_init fail, ret:%d \r\n", ret);
                        }
                        else
                        {
                            os_printf("bk_aud_intf_drv_init complete \r\n");
                        }

                        aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
                        ret = bk_aud_intf_set_mode(aud_work_mode);
                        if (ret != BK_ERR_AUD_INTF_OK)
                        {
                            os_printf("bk_aud_intf_set_mode fail, ret:%d \r\n", ret);
                        }
                        else
                        {
                            os_printf("bk_aud_intf_set_mode complete \r\n");
                        }

                        aud_intf_spk_setup.spk_chl = AUD_INTF_SPK_CHL_DUAL;
                        aud_intf_spk_setup.samp_rate = dac_sample_rate;
                        aud_intf_spk_setup.frame_size = frame_size;
                        aud_intf_spk_setup.spk_gain = 0x2d;
                        aud_intf_spk_setup.work_mode = AUD_DAC_WORK_MODE_DIFFEN;
                        ret = bk_aud_intf_spk_init(&aud_intf_spk_setup);
                        if (ret != BK_ERR_AUD_INTF_OK)
                        {
                            os_printf("bk_aud_intf_spk_init fail, ret:%d \r\n", ret);
                        }
                        else
                        {
                            os_printf("bk_aud_intf_spk_init complete \r\n");
                        }

                        ret = bk_aud_intf_spk_start();
                        if (ret != BK_ERR_AUD_INTF_OK)
                        {
                            os_printf("bk_aud_intf_spk_start fail, ret:%d \r\n", ret);
                        }
                        else
                        {
                            os_printf("bk_aud_intf_spk_start complete \r\n");
                        }

                        os_free(msg.data);
                    }
                    break;

                case BT_AUDIO_D2DP_DATA_IND_MSG:
                    {
                        bk_err_t ret = BK_OK;
                        uint8 *fb = (uint8_t*)msg.data;;

                        if (CODEC_AUDIO_SBC == bt_audio_a2dp_codec)
                        {
                            fb++;  //skip Number of Frames
                            uint16_t inlen = msg.len - 1;
                            uint32 fp = 0;

                            while(fp < inlen)
                            {
                                int32_t res = bk_sbc_decoder_frame_decode(&bt_audio_sbc_decoder, fb, inlen);
                                if (res >= 0) {
                                    fb += res;
                                    fp += res;
                                }

                                ret = bk_aud_intf_write_spk_data((uint8_t*)bt_audio_sbc_decoder.pcm_sample, bt_audio_sbc_decoder.pcm_length*4);
                                if (ret != BK_OK)
                                {
                                    os_printf("write spk data fail \r\n");
                                }
                            }
                        }
                        else if (CODEC_AUDIO_AAC == bt_audio_a2dp_codec)
                        {
                            uint8_t *outbuf = NULL;
                            uint32_t outlen;
                            uint8_t* inbuf = &fb[9];
                            uint32_t inlen = 0;
                            uint8_t  len   = 255;

                            do
                            {
                                inlen += len = *inbuf++;
                            }
                            while(len == 255);

                            if (aac_decoder && (0 == bt_a2dp_aac_decoder_decode(aac_decoder, inbuf, inlen, &outbuf, &outlen)))
                            {
                                ret = bk_aud_intf_write_spk_data(outbuf, outlen);
                                if (ret != BK_OK)
                                {
                                    os_printf("write spk data fail \r\n");
                                }
                            }
                            else
                            {
                                os_printf("bt_a2dp_aac_decoder_decode failed!\r\n");
                            }
                        }
                        else
                        {
                            os_printf("%s, cannot decode data due to unsupported a2dp codec %d \r\n", __func__, bt_audio_a2dp_codec);
                        }

                        os_free(msg.data);
                    }
                    break;

                case BT_AUDIO_D2DP_STOP_MSG:
                    {
                        os_printf("BT_AUDIO_D2DP_STOP_MSG \r\n");
                        bk_aud_intf_spk_deinit();
                        bk_aud_intf_drv_deinit();
                        if (aac_decoder)
                        {
                            os_free(aac_decoder);
                            aac_decoder = NULL;
                        }
                    }
                    break;

                case BT_AUDIO_VOICE_START_MSG:
                    {
                        bk_err_t ret = BK_OK;

                        os_printf("BT_AUDIO_VOICE_START_MSG \r\n");

                        aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
                        aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
                        aud_intf_voc_setup_t aud_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();

                        if (CODEC_VOICE_MSBC == bt_audio_hfp_codec)
                        {
                            bk_sbc_decoder_init(&bt_audio_sbc_decoder);
                            sbc_encoder_init(&bt_audio_sbc_encoder, 16000, 1);
                            sbc_encoder_ctrl(&bt_audio_sbc_encoder, SBC_ENCODER_CTRL_CMD_SET_MSBC_ENCODE_MODE, (uint32_t)NULL);
                        }

                        aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
                        aud_intf_drv_setup.task_config.priority = 3;
                        aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
                        aud_intf_drv_setup.aud_intf_tx_mic_data = bt_audio_mic_data_handler;
                        ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);
                        if (ret != BK_ERR_AUD_INTF_OK) {
                            os_printf("bk_aud_intf_drv_init fail, ret:%d \r\n", ret);
                        } else {
                            os_printf("bk_aud_intf_drv_init complete \r\n");
                        }

                        aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
                        ret = bk_aud_intf_set_mode(aud_work_mode);
                        if (ret != BK_ERR_AUD_INTF_OK) {
                            os_printf("bk_aud_intf_set_mode fail, ret:%d \r\n", ret);
                        } else {
                            os_printf("bk_aud_intf_set_mode complete \r\n");
                        }

                        aud_voc_setup.aec_enable = true;
                        aud_voc_setup.samp_rate = ((CODEC_VOICE_MSBC == bt_audio_hfp_codec) ? AUD_INTF_VOC_SAMP_RATE_16K : AUD_INTF_VOC_SAMP_RATE_8K);
                        aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_PCM;
                        aud_voc_setup.spk_mode = AUD_DAC_WORK_MODE_SIGNAL_END;//AUD_DAC_WORK_MODE_DIFFEN;

                        aud_voc_setup.mic_gain = 0x2d;
                        aud_voc_setup.spk_gain = 0x2d;
                        aud_voc_setup.aec_cfg.ec_depth = 20;
                        aud_voc_setup.aec_cfg.TxRxThr = 30;
                        aud_voc_setup.aec_cfg.TxRxFlr = 6;
                        aud_voc_setup.aec_cfg.ns_level = 2;
                        aud_voc_setup.aec_cfg.ns_para = 1;

                        ret = bk_aud_intf_voc_init(aud_voc_setup);
                        if (ret != BK_ERR_AUD_INTF_OK)
                        {
                            os_printf("bk_aud_intf_voc_init fail, ret:%d\n", ret);
                            break;
                        }

                        ret = bk_aud_intf_voc_start();
                        if (ret != BK_ERR_AUD_INTF_OK)
                        {
                            os_printf("bk_aud_intf_voc_start fail, ret:%d\n", ret);
                            break;
                        }

                        os_printf("hfp audio init ok\r\n");
                    }
                    break;

                case BT_AUDIO_VOICE_STOP_MSG:
                    {
                        os_printf("BT_AUDIO_VOICE_STOP_MSG \r\n");
                        bk_aud_intf_voc_deinit();
                        bk_aud_intf_drv_deinit();
                    }
                    break;

                case BT_AUDIO_VOICE_IND_MSG:
                    {
                        bk_err_t ret = BK_OK;
                        uint8 *fb = (uint8_t*)msg.data;

                        //os_printf("len %d, %x %x %x\r\n",msg.len,fb[0], fb[1], fb[2]);

                        if (CODEC_VOICE_MSBC == bt_audio_hfp_codec)
                        {
                            fb += 2; //Skip Synchronization Header
                            ret = bk_sbc_decoder_frame_decode(&bt_audio_sbc_decoder, fb, msg.len -2);
                            if (ret < 0)
                            {
                                os_printf("msbc decode fail, ret:%d\n", ret);
                            }
                            else
                            {
                                ret = bk_aud_intf_write_spk_data((uint8_t*)bt_audio_sbc_decoder.pcm_sample, SCO_MSBC_SAMPLES_PER_FRAME*2);
                                if (ret != BK_OK) {
                                    os_printf("write spk data fail \r\n");
                                }
                            }
                        }
                        else
                        {
                            ret = bk_aud_intf_write_spk_data((uint8_t*)fb, SCO_CVSD_SAMPLES_PER_FRAME*2);
                            if (ret != BK_OK) {
                                os_printf("write spk data fail \r\n");
                            }
                        }

                        os_free(msg.data);
                    }
                    break;

                default:
                    break;
            }
        }
    }

    rtos_deinit_queue(&bt_audio_demo_msg_que);
    bt_audio_demo_msg_que = NULL;
    bt_audio_demo_thread_handle = NULL;
    rtos_delete_thread(NULL);
}

int bt_audio_demo_task_init(void)
{
    bk_err_t ret = BK_OK;
    if ((!bt_audio_demo_thread_handle) && (!bt_audio_demo_msg_que))
    {
        ret = rtos_init_queue(&bt_audio_demo_msg_que,
                              "bt_audio_demo_msg_que",
                              sizeof(bt_audio_demo_msg_t),
                              BT_AUDIO_DEMO_MSG_COUNT);
        if (ret != kNoErr) {
            os_printf("bt_audio demo msg queue failed \r\n");
            return BK_FAIL;
        }

        ret = rtos_create_thread(&bt_audio_demo_thread_handle,
                             BEKEN_DEFAULT_WORKER_PRIORITY,
                             "bt_audio_demo",
                             (beken_thread_function_t)bt_audio_demo_main,
                             4096,
                             (beken_thread_arg_t)0);
        if (ret != kNoErr) {
            os_printf("bt_audio demo task fail \r\n");
            rtos_deinit_queue(&bt_audio_demo_msg_que);
            bt_audio_demo_msg_que = NULL;
            bt_audio_demo_thread_handle = NULL;
        }

        return kNoErr;
    }
    else
    {
        return kInProgressErr;
    }
}

void bt_audio_media_data_ind(uint8_t *data, uint16_t data_len)
{
    bt_audio_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_demo_msg_t));
    if (bt_audio_demo_msg_que == NULL)
        return;

    demo_msg.data = (char *) os_malloc(data_len);
    if (demo_msg.data == NULL)
    {
        os_printf("%s, malloc failed\r\n", __func__);
        return;
    }

    os_memcpy(demo_msg.data, data, data_len);
    demo_msg.type = BT_AUDIO_D2DP_DATA_IND_MSG;
    demo_msg.len = data_len;

    rc = rtos_push_to_queue(&bt_audio_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);
    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n",__func__);
        if (demo_msg.data)
        {
            os_free(demo_msg.data);
        }
    }
}

void bt_audio_a2dp_suspend_ind(void)
{
    bt_audio_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_demo_msg_t));
    if (bt_audio_demo_msg_que == NULL)
        return;

    demo_msg.type = BT_AUDIO_D2DP_STOP_MSG;
    demo_msg.len = 0;

    rc = rtos_push_to_queue(&bt_audio_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);
    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n",__func__);
    }
}

void bt_audio_a2dp_start_ind(union codec_info *codec)
{
    bt_audio_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_demo_msg_t));
    if (bt_audio_demo_msg_que == NULL)
        return;

    demo_msg.data = (char *) os_malloc(sizeof(union codec_info));
    if (demo_msg.data == NULL)
    {
        os_printf("%s, malloc failed\r\n", __func__);
        return;
    }

    bt_audio_a2dp_codec = codec->codec_type;
    os_printf("%s, codec_id %d \r\n", __func__, bt_audio_a2dp_codec);

    os_memcpy(demo_msg.data, codec, sizeof(union codec_info));
    demo_msg.type = BT_AUDIO_D2DP_START_MSG;
    demo_msg.len = sizeof(union codec_info);

    rc = rtos_push_to_queue(&bt_audio_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);
    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n",__func__);
    }
}

static int bt_enable_a2dp_sink_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;

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

    err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
    if (err != kNoErr){
        goto error;
    }

    if (bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        err = bk_bt_write_scan_enable(0x3, bt_at_cmd_cb);
        if (err) goto error;

        err = rtos_get_semaphore(&bt_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if (!err)
        {
            bt_audio_demo_task_init();

            bt_audio_a2dp_sink_cb.a2dp_start_ind = bt_audio_a2dp_start_ind;
            bt_audio_a2dp_sink_cb.a2dp_suspend_ind = bt_audio_a2dp_suspend_ind;
            bt_audio_a2dp_sink_cb.media_data_ind = bt_audio_media_data_ind;
            err = bk_bt_a2dp_sink_init(aac_supported, &bt_audio_a2dp_sink_cb);
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




void bt_audio_hfp_unit_voice_data_ind(uint8_t *data, uint16_t data_len)
{
    bt_audio_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_demo_msg_t));
    if (bt_audio_demo_msg_que == NULL)
        return;

    demo_msg.data = (char *) os_malloc(data_len);
    if (demo_msg.data == NULL)
    {
        os_printf("%s, malloc failed\r\n", __func__);
        return;
    }

    os_memcpy(demo_msg.data, data, data_len);
    demo_msg.type = BT_AUDIO_VOICE_IND_MSG;
    demo_msg.len = data_len;

    rc = rtos_push_to_queue(&bt_audio_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);
    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n",__func__);
        if (demo_msg.data)
        {
            os_free(demo_msg.data);
        }
    }
}

void bt_audio_sco_connected(void)
{
    bt_audio_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_demo_msg_t));
    if (bt_audio_demo_msg_que == NULL)
        return;

    demo_msg.type = BT_AUDIO_VOICE_START_MSG;
    demo_msg.len = 0;

    rc = rtos_push_to_queue(&bt_audio_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);
    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n",__func__);
    }
}

void bt_audio_sco_disconnected(void)
{
    bt_audio_demo_msg_t demo_msg;
    int rc = -1;

    os_memset(&demo_msg, 0x0, sizeof(bt_audio_demo_msg_t));
    if (bt_audio_demo_msg_que == NULL)
        return;

    demo_msg.type = BT_AUDIO_VOICE_STOP_MSG;
    demo_msg.len = 0;

    rc = rtos_push_to_queue(&bt_audio_demo_msg_que, &demo_msg, BEKEN_NO_WAIT);
    if (kNoErr != rc)
    {
        os_printf("%s, send queue failed\r\n",__func__);
    }
}

void bt_audio_hfp_unit_codec_ind(uint8_t codec_id)
{
    os_printf("%s, codec_id %d \r\n", __func__, codec_id);
    bt_audio_hfp_codec = codec_id;
}

static int bt_enable_hfp_unit_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;

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

    err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
    if (err != kNoErr){
        goto error;
    }

    if (bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        err = bk_bt_write_scan_enable(0x3, bt_at_cmd_cb);
        if (err) goto error;

        err = rtos_get_semaphore(&bt_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
        if (!err)
        {
            bt_audio_demo_task_init();

            bt_audio_hfp_unit_cb.sco_connected = bt_audio_sco_connected;
            bt_audio_hfp_unit_cb.sco_disconnected = bt_audio_sco_disconnected;
            bt_audio_hfp_unit_cb.codec_ind = bt_audio_hfp_unit_codec_ind;
            bt_audio_hfp_unit_cb.voice_data_ind = bt_audio_hfp_unit_voice_data_ind;
            err = bk_bt_hfp_unit_init(msbc_supported, &bt_audio_hfp_unit_cb);
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


#endif

#if CONFIG_FATFS
#include "ff.h"
#include "test_fatfs.h"
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
        err = bk_bt_write_scan_enable(0x3, bt_at_cmd_cb);
        if (err) goto error;

        err = rtos_get_semaphore(&bt_at_cmd_sema, AT_SYNC_CMD_TIMEOUT_MS);
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

#if CONFIG_AUDIO

#define A2DP_SOURCE_WRITE_AUTO_TIMER_MS 30
static beken_time_t s_last_get_sample_time = 0;
static uint32_t s_remain_send_sample_count = 0;
static uint32_t s_remain_send_sample_miss_num = 0;


static int bt_enable_a2dp_source_connect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
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
        bk_bt_set_event_callback(bt_at_event_cb);

        err = get_addr_from_param(&a2dp_env.peer_addr, argv[0]);

        if(err) goto error;
        if(!a2dp_env.inited)
        {
            bk_bt_a2dp_source_init((void *)&bt_a2dp_source_cb);
            a2dp_env.inited = 1;
        }

        if(a2dp_env.conn_state != A2DP_CONNECTION_STATUS_IDLE)
        {
            os_printf("With remote device is not idle, please disconnect first\n");
            goto error;
        }

//        a2dp_env.conn_state = STATE_CONNECTING;

        bk_bt_connect(&(a2dp_env.peer_addr.addr[0]),
                        CONNECTION_PACKET_TYPE,
                        CONNECTION_PAGE_SCAN_REPETITIOIN_MODE,
                        0,
                        CONNECTION_CLOCK_OFFSET,
                        1,
                        bt_at_cmd_cb);

        if(bt_at_cmd_sema == NULL) goto error;

        err = rtos_get_semaphore(&bt_at_cmd_sema, 12 * 1000);

        if(err != kNoErr) goto error;

        if(at_cmd_status == 0x00)
        {
//            a2dp_env.conn_state = STATE_CONNECTED;
            a2dp_env.conn_handle = conn_handle;
        }

//        bk_bt_sdp(a2dp_env.conn_handle, &(a2dp_env.peer_addr.addr[0]), bt_at_cmd_cb);
//
//        err = rtos_get_semaphore(&bt_at_cmd_sema, 60 * 1000);
//
//        if(err != kNoErr) goto error;
//
//        if(!at_cmd_status) goto error;

        err = bk_bt_a2dp_source_connect(&(a2dp_env.peer_addr));

        if(err)
        {
            bk_printf("%s connect a2dp err %d\n", __func__, err);
            goto error;
        }

        err = rtos_get_semaphore(&bt_at_cmd_sema, 60 * 1000);

        if(!err)
        {
//            spp_env.conn_state = STATE_PROFILE_CONNECTED;
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
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
//    os_memset(&spp_env, 0, sizeof(spp_env_s));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

static int bt_enable_a2dp_source_disconnect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
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
        bk_bt_set_event_callback(bt_at_event_cb);

        err = get_addr_from_param(&a2dp_env.peer_addr, argv[0]);

        if(err) goto error;

        if(a2dp_env.conn_state != A2DP_CONNECTION_STATUS_CONNECTED)
        {
            os_printf("With remote device is not connected\n");
            goto error;
        }

        err = bk_bt_a2dp_source_disconnect(&(a2dp_env.peer_addr));

        if(err)
        {
            bk_printf("%s disconnect a2dp err %d\n", __func__, err);
            goto error;
        }

        err = rtos_get_semaphore(&bt_at_cmd_sema, 60 * 1000);

        if(err)
        {
            bk_printf("%s disconnect a2dp timeout %d\n", __func__, err);
            goto error;
        }

        err = bk_bt_disconnect(a2dp_env.peer_addr.addr, DISCONNECT_REASON_REMOTE_USER_TERMINATE, bt_at_cmd_cb);

        if(err)
        {
            bk_printf("%s disconnect link err %d\n", __func__, err);
            goto error;
        }

        err = rtos_get_semaphore(&bt_at_cmd_sema, 60 * 1000);

        if(!err)
        {
//            spp_env.conn_state = STATE_PROFILE_CONNECTED;
            msg = AT_CMD_RSP_SUCCEED;
            os_memcpy(pcWriteBuffer, msg, os_strlen(msg));

            os_memset(&a2dp_env, 0, sizeof(a2dp_env));

            if (bt_at_cmd_sema != NULL)
                rtos_deinit_semaphore(&bt_at_cmd_sema);
            return err;
        }
        else
        {
            goto error;
        }
    }
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
//    os_memset(&spp_env, 0, sizeof(spp_env_s));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

static int bt_a2dp_source_mic_data_handler(uint8_t *data, unsigned int len)//640
{
//    uint8_t sent_threshold = ((CODEC_VOICE_MSBC == bt_audio_hfp_codec) ? SCO_MSBC_SAMPLES_PER_FRAME * 2 : SCO_CVSD_SAMPLES_PER_FRAME * 2);
    uint32_t sent_threshold = s_sbc_software_encoder_ctx.pcm_length * 2; //pcm_length == 128
    uint32_t index = 0;
    int32_t encode_len = 0;
    bt_err_t ret = 0;

    static uint32_t last_len = 0;
    //os_printf("[send_mic_data_to_air]  %d \r\n",len);
    if(((uint32_t)data) % 2)
    {
        os_printf("%s err data is not 2 bytes aligned !!!\n", __func__);
    }

    if(last_len != len)
    {
        os_printf("%s len %d\n", __func__, len);
        last_len = len;
    }

    if(len > sizeof(mic_sco_data))
    {
        os_printf("%s too long %d !\n", __func__, len);
        return 0;
    }

    os_memcpy(&mic_sco_data[mic_data_count], data, len);
    mic_data_count += len;

    for (uint32_t i = 0; i < mic_data_count / sent_threshold; ++i)
    {
        encode_len = sbc_encoder_encode(&s_sbc_software_encoder_ctx, (const int16_t *)(mic_sco_data + i * sent_threshold));
        if(encode_len < 0)
        {
            os_printf("%s encode err %d\n", __func__, encode_len);
            mic_data_count = 0;
            return 0;
        }

        ret = bk_bt_a2dp_source_write(&(a2dp_env.peer_addr), 1, s_sbc_software_encoder_ctx.stream, encode_len);

        if(ret)
        {
            os_printf("%s bk_bt_a2dp_source_write err %d\n", __func__, ret);
        }

    }

    os_memmove(mic_sco_data, &mic_sco_data[(mic_data_count / sent_threshold) * sent_threshold], mic_data_count % sent_threshold);

    mic_data_count = mic_data_count % sent_threshold;

    return 0;

    while (mic_data_count >= sent_threshold)
    {
        encode_len = sbc_encoder_encode(&s_sbc_software_encoder_ctx, (const int16_t *)(mic_sco_data + index));
        if(encode_len < 0)
        {
            os_printf("%s encode err %d\n", __func__, encode_len);
            mic_data_count = 0;
            return 0;
        }

        mic_data_count -= sent_threshold;
        index += sent_threshold;


        ret = bk_bt_a2dp_source_write(&(a2dp_env.peer_addr), 1, s_sbc_software_encoder_ctx.stream, encode_len);

        if(ret)
        {
            os_printf("%s bk_bt_a2dp_source_write err %d\n", __func__, ret);
        }

    }


    os_memmove(mic_sco_data, &mic_sco_data[sent_threshold], mic_data_count);

    return len;
}

static int32_t bt_a2dp_source_prepare_sbc_encoder(void)
{
    bk_err_t ret = BK_OK;

    os_printf("%s\n", __func__);

    memset(&s_sbc_software_encoder_ctx, 0, sizeof(s_sbc_software_encoder_ctx));
    sbc_encoder_init(&s_sbc_software_encoder_ctx, 44100, 1);

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_ALLOCATION_METHOD, 0);
    if(ret != SBC_ENCODER_ERROR_OK)
    {
        os_printf("%s SBC_ENCODER_CTRL_CMD_SET_ALLOCATION_METHOD err %d\n", __func__, ret);
        return ret;
    }

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_BITPOOL, 53);
    if(ret != SBC_ENCODER_ERROR_OK)
    {
        os_printf("%s SBC_ENCODER_CTRL_CMD_SET_BITPOOL err %d\n", __func__, ret);
        return ret;
    }

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_BLOCK_MODE, 3);
    if(ret != SBC_ENCODER_ERROR_OK)
    {
        os_printf("%s SBC_ENCODER_CTRL_CMD_SET_BLOCK_MODE err %d\n", __func__, ret);
        return ret;
    }

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_CHANNEL_MODE, 3); //0:MONO, 1:DUAL, 2:STEREO, 3:JOINT STEREO
    if(ret != SBC_ENCODER_ERROR_OK)
    {
        os_printf("%s SBC_ENCODER_CTRL_CMD_SET_CHANNEL_MODE err %d\n", __func__, ret);
        return ret;
    }

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_SAMPLE_RATE_INDEX, 2); //0:16000, 1:32000, 2:44100, 3:48000
    if(ret != SBC_ENCODER_ERROR_OK)
    {
        os_printf("%s SBC_ENCODER_CTRL_CMD_SET_SAMPLE_RATE_INDEX err %d\n", __func__, ret);
        return ret;
    }

    ret = sbc_encoder_ctrl(&s_sbc_software_encoder_ctx, SBC_ENCODER_CTRL_CMD_SET_SUBBAND_MODE, 1);
    if(ret != SBC_ENCODER_ERROR_OK)
    {
        os_printf("%s SBC_ENCODER_CTRL_CMD_SET_SUBBAND_MODE err %d\n", __func__, ret);
        return ret;
    }


    os_printf("%s sbc encode count %d\n", __func__, s_sbc_software_encoder_ctx.pcm_length);

    return 0;
}


static int32_t bt_a2dp_source_start_voc(void)
{
    bk_err_t ret = BK_OK;

    os_printf("%s\n", __func__);

    os_printf("%s TODO: most sound box a2dp support neither mono channel nor 16 kHZ !!!\n", __func__);
    aud_intf_drv_setup_t aud_intf_drv_setup;
    aud_intf_work_mode_t aud_work_mode = 0;
    aud_intf_voc_setup_t aud_voc_setup;

    memset(&aud_intf_drv_setup, 0, sizeof(aud_intf_drv_setup));
    memset(&aud_voc_setup, 0, sizeof(aud_voc_setup));

    aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
    aud_intf_drv_setup.task_config.priority = 3;
    aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
    aud_intf_drv_setup.aud_intf_tx_mic_data = bt_a2dp_source_mic_data_handler;

    ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);

    if (ret != BK_ERR_AUD_INTF_OK)
    {
        os_printf("%s bk_aud_intf_drv_init fail, ret:%d\n", __func__, ret);
        return ret;
    }
    else
    {
        os_printf("%s bk_aud_intf_drv_init complete\n", __func__);
    }

    aud_work_mode = AUD_INTF_WORK_MODE_VOICE;

    ret = bk_aud_intf_set_mode(aud_work_mode);

    if (ret != BK_ERR_AUD_INTF_OK)
    {
        os_printf("%s bk_aud_intf_set_mode fail, ret:%d\n", __func__, ret);
        return ret;
    }
    else
    {
        os_printf("%s bk_aud_intf_set_mode complete\n", __func__);
    }

    aud_voc_setup.aec_enable = true;
    aud_voc_setup.samp_rate = AUD_INTF_VOC_SAMP_RATE_16K;
    aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_PCM;
    aud_voc_setup.spk_mode = AUD_DAC_WORK_MODE_SIGNAL_END; //AUD_DAC_WORK_MODE_DIFFEN;
    aud_voc_setup.mic_type = AUD_INTF_MIC_TYPE_BOARD;
    aud_voc_setup.mic_gain = 0x2d;
    aud_voc_setup.spk_gain = 0x2d;
    aud_voc_setup.aec_cfg.ec_depth = 20;
    aud_voc_setup.aec_cfg.TxRxThr = 30;
    aud_voc_setup.aec_cfg.TxRxFlr = 6;
    aud_voc_setup.aec_cfg.ns_level = 2;
    aud_voc_setup.aec_cfg.ns_para = 1;

    ret = bk_aud_intf_voc_init(aud_voc_setup);
    if (ret != BK_ERR_AUD_INTF_OK)
    {
        os_printf("%s bk_aud_intf_voc_init fail, ret:%d\n", __func__, ret);
        return ret;
    }

    ret = bk_aud_intf_voc_start();
    if (ret != BK_ERR_AUD_INTF_OK)
    {
        os_printf("%s bk_aud_intf_voc_start fail, ret:%d\n", __func__, ret);
        return ret;
    }

    os_printf("%s ok\n", __func__);

    return 0;
}


static int bt_enable_a2dp_source_start_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
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
        err = get_addr_from_param(&a2dp_env.peer_addr, argv[0]);

        if(err) goto error;

        if(a2dp_env.conn_state != A2DP_CONNECTION_STATUS_CONNECTED)
        {
            os_printf("With remote device is not connected\n");
            goto error;
        }

        if(a2dp_env.start_status != 0)
        {
            os_printf("is already start\n");
            goto error;
        }

        err = bk_bt_a2dp_source_start(&(a2dp_env.peer_addr));

        if(err)
        {
            bk_printf("%s start err %d\n", __func__, err);
            goto error;
        }

        err = rtos_get_semaphore(&bt_at_cmd_sema, 60 * 1000);
        if(!err)
        {
//            spp_env.conn_state = STATE_PROFILE_CONNECTED;
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
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
//    os_memset(&spp_env, 0, sizeof(spp_env_s));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}


static int bt_enable_a2dp_source_suspend_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
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
        err = get_addr_from_param(&a2dp_env.peer_addr, argv[0]);

        if(err) goto error;

        if(a2dp_env.conn_state != A2DP_CONNECTION_STATUS_CONNECTED)
        {
            os_printf("With remote device is not connected\n");
            goto error;
        }

        if(a2dp_env.start_status == 0)
        {
            os_printf("is already suspend\n");
            goto error;
        }

        err = bk_bt_a2dp_source_suspend(&(a2dp_env.peer_addr));

        if(err)
        {
            bk_printf("%s suspend err %d\n", __func__, err);
            goto error;
        }

        err = rtos_get_semaphore(&bt_at_cmd_sema, 60 * 1000);
        if(!err)
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
error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

static void bt_a2dp_source_write_from_file_timer_hdl(void *param)
{
    FRESULT fr;
    static uint32_t offset = 0;
    unsigned int read_len = 0;
    FIL *pcm_file_fd = (typeof(pcm_file_fd))param;
    int32_t encode_len = 0;
    bt_err_t ret = 0;
    uint8_t *tmp_buf = NULL;
    uint32_t encode_index = 0;

    uint32_t sent_threshold = s_sbc_software_encoder_ctx.pcm_length * 2 *
            s_sbc_software_encoder_ctx.num_channels; //pcm_length == 128 when 16 block 8 subband 1 channel
    //s_sbc_software_encoder_ctx.frame.blocks * s_sbc_software_encoder_ctx.frame.subbands == s_sbc_software_encoder_ctx.pcm_length;

    tmp_buf = os_malloc(A2DP_SOURCE_SBC_FRAME_COUNT * 128); //max 128

    if(!tmp_buf)
    {
        os_printf("%s malloc err\n", __func__);
        return;
    }

    for (uint32_t i = 0; i < A2DP_SOURCE_SBC_FRAME_COUNT; ++i)
    {
        fr = f_read(pcm_file_fd, (void *)(mic_sco_data + offset), sent_threshold, &read_len);
        if (fr != FR_OK)
        {
            os_printf("%s read fail %d\n", __func__, fr);
            os_free(tmp_buf);
            return;
        }

        if(read_len < sent_threshold)
        {
            os_printf("%s read len %d < %d !!\n", __func__, read_len, sent_threshold);

            if(read_len == 0)
            {
                os_printf("%s read file end !!\n", __func__, read_len, sent_threshold);
                os_free(tmp_buf);
                rtos_stop_timer(&bt_a2dp_source_write_timer);
                return;
            }

            memset(mic_sco_data + read_len, 0, sent_threshold - read_len);
        }

        encode_len = sbc_encoder_encode(&s_sbc_software_encoder_ctx, (const int16_t *)(mic_sco_data));
        if(encode_len < 0)
        {
           os_printf("%s encode err %d\n", __func__, encode_len);
           os_free(tmp_buf);
           return;
        }

        if(encode_index + encode_len > A2DP_SOURCE_SBC_FRAME_COUNT * 128)
        {
            os_printf("%s encode data large than malloc !! %d\n", __func__, encode_len);
            os_free(tmp_buf);
            return;

        }
        memcpy(tmp_buf + encode_index, s_sbc_software_encoder_ctx.stream, encode_len);
        encode_index += encode_len;

    }

    ret = bk_bt_a2dp_source_write(&(a2dp_env.peer_addr), A2DP_SOURCE_SBC_FRAME_COUNT, tmp_buf, encode_index);

    if(ret)
    {
        os_printf("%s bk_bt_a2dp_source_write err %d\n", __func__, ret);
    }

    os_free(tmp_buf);
}


static void bt_a2dp_set_get_sample_clean(void)
{
    s_last_get_sample_time = 0;
    s_remain_send_sample_count = 0;
    s_remain_send_sample_miss_num = 0;
}

static void bt_a2dp_source_write_from_file_timer_auto_hdl(void *param)
{
    unsigned int read_len = 0;
    static uint32_t read_len_all = 0;
    FIL *pcm_file_fd = (FIL *)param;
    FRESULT fr;
    int32_t encode_len = 0;
    bt_err_t ret = 0;
    uint8_t *tmp_buf = NULL;
    uint32_t encode_index = 0;


    //    beken_time_t last_time = s_last_get_sample_time;
    beken_time_t cur_time = rtos_get_time();
    beken_time_t will_send_period_time = 0;


    uint32_t will_send_frame_count = 0;
//    uint32_t sample_count = 0, sample_miss_num = 0;


    //s_sbc_software_encoder_ctx.frame.blocks * s_sbc_software_encoder_ctx.frame.subbands == s_sbc_software_encoder_ctx.pcm_length;
    //pcm_length == 128 when 16 block 8 subband
    uint32_t one_pcm_frame_encode_sample_count_per_channel = s_sbc_software_encoder_ctx.pcm_length;
    uint32_t one_pcm_frame_encode_sample_bytes = one_pcm_frame_encode_sample_count_per_channel * 2 * s_sbc_software_encoder_ctx.num_channels;
    const uint32_t send_sbc_frame_count = 2;// A2DP_SOURCE_SBC_FRAME_COUNT;
    const uint32_t ignore_max_mtu = 0;

    uint32_t i = 0, j = 0;

    if (s_last_get_sample_time == 0)
    {
        will_send_period_time = A2DP_SOURCE_WRITE_AUTO_TIMER_MS;//first
    }
    else
    {
        will_send_period_time = cur_time - s_last_get_sample_time;
    }

    s_remain_send_sample_count += will_send_period_time * s_sbc_software_encoder_ctx.sample_rate / 1000;
    s_remain_send_sample_miss_num += will_send_period_time * s_sbc_software_encoder_ctx.sample_rate % 1000;

    s_remain_send_sample_count += s_remain_send_sample_miss_num / 1000;
    s_remain_send_sample_miss_num = s_remain_send_sample_miss_num % 1000;

    s_last_get_sample_time = cur_time;

    if(!s_remain_send_sample_count)
    {
        return;
    }
    else if (s_remain_send_sample_count < one_pcm_frame_encode_sample_count_per_channel)
    {
        if (will_send_period_time * s_sbc_software_encoder_ctx.sample_rate / 1000 <
                send_sbc_frame_count * one_pcm_frame_encode_sample_count_per_channel)
        {
            return;
        }
    }
    else if (s_remain_send_sample_count >= one_pcm_frame_encode_sample_count_per_channel &&
            s_remain_send_sample_count < one_pcm_frame_encode_sample_count_per_channel * send_sbc_frame_count)
    {
        if (0)//will_send_period_time * s_sbc_software_encoder_ctx.sample_rate / 1000 < send_sbc_frame_count * one_pcm_frame_encode_sample_count_per_channel)
        {
            return;
        }
    }


    will_send_frame_count = ((s_remain_send_sample_count / one_pcm_frame_encode_sample_count_per_channel > 1) ?
            (s_remain_send_sample_count / one_pcm_frame_encode_sample_count_per_channel) : 1);

//    if(will_send_frame_count > A2DP_SOURCE_SBC_FRAME_COUNT)
//    {
//        will_send_frame_count = A2DP_SOURCE_SBC_FRAME_COUNT;
//    }

    if(will_send_frame_count > 100)
    {
        os_printf("%s\n", __func__);
    }

    if(!will_send_frame_count)
    {
        os_printf("%s 0\n", __func__);
    }

    //printf("%s will_send_frame_count %d \n", __func__, will_send_frame_count);

    tmp_buf = os_malloc(will_send_frame_count * 128); //max 128

    do
    {
//        for (; i < will_send_frame_count; ++i)
        for(i = 0; ( ignore_max_mtu || encode_index + encode_len <= a2dp_env.mtu) && i + j < will_send_frame_count; ++i)
        {
            fr = f_read(pcm_file_fd, (void *)(mic_sco_data), one_pcm_frame_encode_sample_bytes, &read_len);
//            read_len = fread((mic_sco_data), 1, one_pcm_frame_encode_sample_bytes, pcm_file_fd);

            if (fr != FR_OK)
            {
                os_printf("%s read fail %d\n", __func__, fr);
                os_free(tmp_buf);
                return;
            }

            read_len_all += read_len;

            if (read_len < one_pcm_frame_encode_sample_bytes)
            {
                os_printf("%s read len %d < %d!!\n", __func__, read_len, one_pcm_frame_encode_sample_bytes);


                if (read_len == 0)
                {
                    os_printf("%s read file end %d %d !!\n", __func__, read_len, one_pcm_frame_encode_sample_bytes);
                    os_free(tmp_buf);
                    f_close(pcm_file_fd);
//                    fclose(pcm_file_fd);
//                    *(FILE **)param = NULL;
                    rtos_stop_timer(&bt_a2dp_source_write_timer);
                    bt_a2dp_set_get_sample_clean();
                    return;
                }

                memset(mic_sco_data + read_len, 0, one_pcm_frame_encode_sample_bytes - read_len);
            }

            encode_len = sbc_encoder_encode(&s_sbc_software_encoder_ctx, (const int16_t *)(mic_sco_data));

            if (encode_len < 0)
            {
                os_printf("%s encode err %d\n", __func__, encode_len);
                os_free(tmp_buf);
                return;
            }

            if (encode_index + encode_len > will_send_frame_count * 128)
            {
                os_printf("%s encode data large than malloc !! %d\n", __func__, encode_len);
                os_free(tmp_buf);
                return;
            }

            memcpy(tmp_buf + encode_index, s_sbc_software_encoder_ctx.stream, encode_len);
            encode_index += encode_len;
        }
//        while(encode_index + encode_len <= a2dp_env.mtu && i < will_send_frame_count);

        ret = bk_bt_a2dp_source_write(&(a2dp_env.peer_addr), i, tmp_buf, encode_index);


        if (encode_index > 10000 || i > 5)
        {
            os_printf("%s encode_index %d i %d\n", __func__, encode_index, i);
        }

        if (ret)
        {
            os_printf("%s bk_bt_a2dp_source_write err %d\n", __func__, ret);
        }

        encode_index = 0;
        j += i;
    }
    while(j < will_send_frame_count);

    if(s_remain_send_sample_count < j * one_pcm_frame_encode_sample_count_per_channel)
    {
        s_remain_send_sample_count = 0;
    }
    else
    {
        s_remain_send_sample_count -= j * one_pcm_frame_encode_sample_count_per_channel;
    }

    os_free(tmp_buf);
}

static int32_t bt_a2dp_source_encode_from_file(uint8_t * path)
{
    static FIL pcm_file_fd;
    FRESULT fr;
    uint32_t inter = (uint32_t)((640 * 1000.0) / (float)s_sbc_software_encoder_ctx.sample_rate);

    char full_path[64] = {0};

    test_mount(1);

    //need s16le dual pcm
//    sprintf((char *)full_path, "%d:/%s", mount_label, path);
    sprintf((char *)full_path, "%s", path);


    os_printf("%s send interval %d\n", __func__, inter);

    if (rtos_is_timer_init(&bt_a2dp_source_write_timer))
    {
        if (rtos_is_timer_running(&bt_a2dp_source_write_timer))
        {
            rtos_stop_timer(&bt_a2dp_source_write_timer);
            f_close(&pcm_file_fd);
        }

        rtos_deinit_timer(&bt_a2dp_source_write_timer);
    }

    if (!rtos_is_timer_init(&bt_a2dp_source_write_timer))
    {
//        rtos_init_timer(&bt_a2dp_source_write_timer, inter, bt_a2dp_source_write_from_file_timer_hdl, (void *)&pcm_file_fd);
        rtos_init_timer(&bt_a2dp_source_write_timer, A2DP_SOURCE_WRITE_AUTO_TIMER_MS, bt_a2dp_source_write_from_file_timer_auto_hdl, (void *)&pcm_file_fd);

    }


    rtos_change_period(&bt_a2dp_source_write_timer, inter);

    memset(&pcm_file_fd, 0, sizeof(pcm_file_fd));
    fr = f_open(&pcm_file_fd, (const char *)full_path, FA_OPEN_EXISTING | FA_READ);
    if (fr != FR_OK)
    {
        os_printf("open %s fail.\n", full_path);
        return -1;
    }

    rtos_start_timer(&bt_a2dp_source_write_timer);
    return 0;
}


static int bt_enable_a2dp_source_write_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    PRINT_FUNC;
    char *msg;
    int err = kNoErr;
    uint8_t choise = 0;//0 mic 1 file 2 static code

    if (0)//bt_at_cmd_table[3].is_sync_cmd)
    {
        err = rtos_init_semaphore(&bt_at_cmd_sema, 1);
        if(err != kNoErr){
            goto error;
        }
    }

    if(bk_bt_get_host_stack_type() == BK_BT_HOST_STACK_TYPE_ETHERMIND)
    {
        err = get_addr_from_param(&a2dp_env.peer_addr, argv[0]);
        if(err) goto error;

        if(argc >= 2)
        {
            if(strcasecmp(argv[1], "mic") == 0)
            {
                choise = 0;
            }
            else if(strcasecmp(argv[1], "file") == 0)
            {
                choise = 1;

                if(argc < 3)
                {
                    os_printf("%s please input file path\n", __func__);
                    goto error;
                }
            }
            else
            {
                os_printf("%s unknow case %S\n", __func__, argv[1]);
                goto error;
            }

        }

        err = bt_a2dp_source_prepare_sbc_encoder();
        if(err)
        {
            goto error;
        }

#if 1
        if(a2dp_env.conn_state != A2DP_CONNECTION_STATUS_CONNECTED)
        {
            os_printf("With remote device is not connected\n");
            goto error;
        }

        if(a2dp_env.start_status == 0)
        {
            os_printf("is suspend\n");
            goto error;
        }
#endif

        switch(choise)
        {
        case 0:
            err = bt_a2dp_source_start_voc();
            break;

        case 1:
            err = bt_a2dp_source_encode_from_file((uint8_t *)argv[2]);
            break;

        default:
            break;
        }

//        err = rtos_get_semaphore(&bt_at_cmd_sema, 60 * 1000);
        if(!err)
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

error:
    msg = AT_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
    return err;
}

#endif

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

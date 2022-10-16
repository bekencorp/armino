#include <stdio.h>
#include <stdlib.h>
#include "at_common.h"
#include <driver/gpio.h>
#include "modules/bt.h"
#include "modules/bt_types.h"
#include "ethermind_export/bt_defines.h"

#if CONFIG_BT
#include "bt_include.h"
#endif

#define INQUIRY_LAP 0x9e8B33U
#define INQUIRY_LEN 0x0A

#define CONNECTION_PACKET_TYPE 0xcc18
#define CONNECTION_PAGE_SCAN_REPETITIOIN_MODE 0x01
#define CONNECTION_CLOCK_OFFSET 0x00

#define DISCONNECT_REASON 0x16

#define SPP_HANDLE_INVALID                   0xFFU
#define PRINT_FUNC os_printf("%s \n", __func__)
#define SPP_ENABLE_DATA_CNF_LOG

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
    STATE_PROFILE_CONNECTED,
}connect_state_s;


typedef struct{
    uint16_t conn_handle;
    uint8_t conn_state;
    bd_addr_t peer_addr;
    uint8_t spp_init;
    SPP_HANDLE  spp_handle;
    uint32_t spp_record_handle;
    uint8_t local_server_channel;
    uint8_t peer_server_channel;
}spp_env_s;

static bt_err_t at_cmd_status = BK_ERR_BT_SUCCESS;

static beken_semaphore_t bt_at_cmd_sema = NULL;

static uint16_t conn_handle = 0xff;
static spp_env_s spp_env;
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
static int bt_spp_connect_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_write_scan_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_read_scan_enable_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#ifdef CONFIG_AUDIO
static int bt_enable_a2dp_sink_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static int bt_enable_hfp_unit_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif
static int bt_enable_opp_test_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

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
            spp_env.conn_state = STATE_DISCONNECT;
            os_printf("BT Disconnected!!! \r\n");
            break;
        case BT_EVENT_COMPLETE:
            {
                os_printf("BT Event Complete!!! \r\n");
            }
            break;
        default:
            break;
    }
}

static void bt_at_cmd_cb(bt_cmd_t cmd, bt_cmd_param_t *param)
{
    at_cmd_status = param->status;
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
                os_memset(&spp_env, 0, sizeof(spp_env_s));
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
    if (bt_at_cmd_sema != NULL)
        rtos_set_semaphore( &bt_at_cmd_sema );
}

const at_command_t bt_at_cmd_table[] = {
    {0, "INQUIRY", 0, "start inquiry", bt_start_inquiry_handle},
    {1, "CONNECT", 0, "create connection", bt_create_connection_handle},
    {2, "DISCONNECT", 0, "disconnect",  bt_disconnect_handle},

    {3, "SPP_CONNECT", 1, "spp connect", bt_spp_connect_handle},

    {4, "WRITE_SCAN_ENABLE", 1, "write_scan_enable", bt_write_scan_enable_handle},
    {5, "READ_SCAN_ENABLE", 1, "read_scan_enable", bt_read_scan_enable_handle},
#ifdef CONFIG_AUDIO
    {6, "A2DP_SINK_TEST", 0, "enable a2dp sink test", bt_enable_a2dp_sink_test_handle},
    {7, "HFP_UNIT_TEST", 0, "enable hfp unit test", bt_enable_hfp_unit_test_handle},
#endif
    {8, "OPP_TEST", 0, "enable opp test", bt_enable_opp_test_handle},
};
API_RESULT bt_spp_event_notify_cb
           (
               /* IN */  SPP_HANDLE spp_handle,
               /* IN */  SPP_EVENTS spp_event,
               /* IN */  API_RESULT      status,
               /* IN */  void          * data,
               /* IN */  UINT16          data_length
           )
{
#ifdef SPP_ENABLE_DATA_CNF_LOG
    UINT32 index;
#endif /* SPP_ENABLE_DATA_CNF_LOG */
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
#ifdef SPP_VCOM_INTERFACE
            appl_vcom_handle = vcom_connect_pl(spp_port, spp_baud, spp_flow, spp_read_cb);
#endif /* SPP_VCOM_INTERFACE */
            if (bt_at_cmd_sema != NULL)
                rtos_set_semaphore( &bt_at_cmd_sema );
        }
        break;
    case SPP_CONNECT_IND:
        os_printf("SPP_CONNECT_IND -> 0x%04X\n", status);
        os_printf("SPP Instance Connected : %u\n",(unsigned int) spp_handle);
        os_printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

        /* Set the global handle */
        /* g_spp_handle = spp_handle; */

#ifdef SPP_VCOM_INTERFACE
        appl_vcom_handle = vcom_connect_pl(spp_port, spp_baud, spp_flow, spp_read_cb);
#endif /* SPP_VCOM_INTERFACE */
        break;
    case SPP_DISCONNECT_CNF:
        os_printf("SPP_DISCONNECT_CNF -> Disconnection Successful\n");
        os_printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));
#ifdef SPP_VCOM_INTERFACE
        if (0x00 == status)
        {
            vcom_disconnect_pl(appl_vcom_handle);
        }
        spp_env.conn_state = STATE_PROFILE_DISCONNECT;
#endif /* SPP_VCOM_INTERFACE */
        break;
    case SPP_DISCONNECT_IND:
        os_printf("SPP_DISCONNECT_IND -> Disconnection Successful\n");
        os_printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));
#ifdef SPP_VCOM_INTERFACE
        vcom_disconnect_pl(appl_vcom_handle);
#endif /* SPP_VCOM_INTERFACE */
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
#ifdef SPP_ENABLE_DATA_CNF_LOG
        os_printf("Received spp send cnf\n");
        os_printf("   spp handle = %d\n", spp_handle);
        os_printf("    Buffer = %p\n", l_data);
        os_printf("    Actual Data Length = %d\n", data_length);
#else /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
        os_printf(". ");
#endif /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
        if (0x00 != status)
        {
            os_printf ("*** 0x%04X\n", status);
        }
        os_free(data);
        #if 0
        appl_count++;
        if (appl_count == appl_loop)
        {
            os_printf("\n*** SPP Write for %d times has completed!!!\n", appl_loop);
            break;
        }
        buffer = (UCHAR *)BT_alloc_mem(appl_nbytes);
        if (NULL == buffer)
        {
            os_printf("*** Buffer Allocation FAILED. !!\n");
            break;
        }
        if (2U > appl_nbytes)
        {
            BT_mem_set(buffer, appl_count, appl_nbytes);
        }
        else
        {
            BT_PACK_BE_2_BYTE_VAL(buffer, appl_count);
            BT_mem_set((&buffer[2U]), appl_count, (appl_nbytes - 2U));
        }
#ifdef SPP_ENABLE_DATA_CNF_LOG
        os_printf("Allocated Buffer = %p\n", buffer);
        os_printf("Initiating [%d]th spp  Write ... ", appl_count);
#endif /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
        retval = BT_spp_send
                 (
                     g_spp_handle,
                     buffer,
                     appl_nbytes
                 );
        if (API_SUCCESS != retval)
        {
            os_printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
            BT_free_mem(buffer);
        }
        else
        {
#ifdef SPP_ENABLE_DATA_CNF_LOG
            os_printf("Succeeded !!\n");
            os_printf("Wait for Write Complete.\n");
#endif /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
        }
        #endif
    }
    break;
    case SPP_RECVD_DATA_IND:
#ifdef SPP_FILE_OPERATION
        if (0U != spp_recv_file_operation)
        {
            UINT16 written;
            if (NULL != spp_recv_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(l_data, data_length, spp_recv_fp, &written);
            }
            os_printf(". ");
        }
        else
#endif /* SPP_FILE_OPERATION */
        {
#ifdef SPP_ENABLE_DATA_CNF_LOG
            os_printf("SPP_RECVD_DATA_IND -> Data received successfully\n");
            os_printf("\n----------------CHAR DUMP-----------------------\n");
            for (index = 0U; index < data_length; index++)
            {
                os_printf("%c ", l_data[index]);
            }
            os_printf("\n------------------------------------------------\n");
            os_printf("\n----------------HEX DUMP------------------------\n");
            for (index = 0U; index < data_length; index++)
            {
                os_printf("%02X ", l_data[index]);
            }
            os_printf("\n------------------------------------------------\n");
#else /* SPP_ENABLE_DATA_CNF_LOG */
            os_printf(".");
#endif /* SPP_ENABLE_DATA_CNF_LOG */
        }
#ifdef SPP_VCOM_INTERFACE
        vcom_write_pl(appl_vcom_handle, l_data, data_length);
#endif /* SPP_VCOM_INTERFACE */
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
        bk_bt_set_event_callback(bt_at_event_cb);
        err = get_addr_from_param(&spp_env.peer_addr, argv[0]);
        if(err) goto error;
        if(!spp_env.spp_init)
        {
            bk_bt_spp_init(bt_spp_event_notify_cb);
            spp_env.spp_handle = SPP_HANDLE_INVALID;
            bk_bt_spp_start((uint32_t *)&spp_env.spp_handle, &spp_env.local_server_channel, &spp_env.spp_record_handle);
            spp_env.spp_init = 1;
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
        err = rtos_get_semaphore(&bt_at_cmd_sema, 12*1000);
        if(err != kNoErr) goto error;
        if(at_cmd_status == 0x00) 
        {
            spp_env.conn_state = STATE_CONNECTED;
            spp_env.conn_handle = conn_handle;
        }
        bk_bt_sdp(spp_env.conn_handle, &(spp_env.peer_addr.addr[0]), bt_at_cmd_cb);
        err = rtos_get_semaphore(&bt_at_cmd_sema, 60*1000);
        if(err != kNoErr) goto error;
        if(!at_cmd_status) goto error;
        
        bk_bt_spp_connect(&(spp_env.peer_addr.addr[0]), spp_env.peer_server_channel, (uint32_t)spp_env.spp_handle, bt_at_cmd_cb);
        err = rtos_get_semaphore(&bt_at_cmd_sema, 60*1000);
        if(!err)
        {
            spp_env.conn_state = STATE_PROFILE_CONNECTED;
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
    os_memset(&spp_env, 0, sizeof(spp_env_s));
    if (bt_at_cmd_sema != NULL)
        rtos_deinit_semaphore(&bt_at_cmd_sema);
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
beken_queue_t bt_audio_demo_msg_que = NULL;
beken_thread_t bt_audio_demo_thread_handle = NULL;
#define BT_AUDIO_DEMO_MSG_COUNT          (30)
#define SCO_MSBC_SAMPLES_PER_FRAME      120
#define SCO_CVSD_SAMPLES_PER_FRAME      60
//#define MSBC_FRAME_BUFFER_MAX_SIZE      60
static void *aac_decoder = NULL;

uint8_t mic_sco_data[1000]={0};
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
            //msbc encode todo
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

                        aud_intf_drv_setup_t aud_intf_drv_setup;
                        aud_intf_spk_setup_t aud_intf_spk_setup;
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

                        aud_intf_drv_setup_t aud_intf_drv_setup;
                        aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
                        aud_intf_voc_setup_t aud_voc_setup;

                        if (CODEC_VOICE_MSBC == bt_audio_hfp_codec)
                        {
                            bk_sbc_decoder_init(&bt_audio_sbc_decoder);
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
#endif
#include <stdio.h>

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



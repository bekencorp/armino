#include "light_server_app.h"
#include <common/bk_include.h>

#if LIGHT_SERVER_APPLICATION
#include "Arch.h"
#include <os/mem.h>
#include "uart_pub.h"
#include "stdarg.h"
#include <common/bk_include.h>
#include <os/os.h>
#include "Error.h"
#include "portmacro.h"
#include "light_socket.h"
#include "string.h"
#include "sockets.h"
#include "light_commun_protocol.h"
#include "gpio_pub.h"

typedef enum
{
    LIGHT_SERVER_NULL = 0,
    LIGHT_SERVER_RX_MEG,
    LIGHT_SERVER_TX_MEG
} LIGHT_MSG_TYPE;

typedef struct
{
    LIGHT_MSG_TYPE msgtype;
    int len;
    unsigned char *msg;
    int fd;
} light_msg_T;

beken_queue_t light_sck_msg_q = NULL;

void light_gpio4_led_intial(void)
{
    GPIO_INDEX gpio_idx;
    UINT32 mode;
    u32 param;

    gpio_idx = GPIO4;
    //mode = GMODE_OUTPUT_PULLUP;
    mode = GMODE_OUTPUT;
    param =  GPIO_CFG_PARAM(gpio_idx, mode);
    /* config GPIO:output mode. */
    sddev_control(GPIO_DEV_NAME, CMD_GPIO_CFG, &param);

    gpio_idx = GPIO4;
    param =  GPIO_CFG_PARAM(gpio_idx, 1);
    sddev_control(GPIO_DEV_NAME, CMD_GPIO_OUTPUT, &param);
}

void light_sck_cs_txdata_sender(int fd, unsigned char *buf, int len)
{
    bk_err_t ret;
    light_msg_T txmsg;
    char *txbuf = NULL;

    if(len)
    {
        txbuf = (char *) os_malloc( len );
    }

    if((txbuf) && (len))
    {
        memcpy(txbuf, buf, len);

        txmsg.msgtype = LIGHT_SERVER_TX_MEG;
        txmsg.len = len;
        txmsg.msg = txbuf;
        txmsg.fd = fd & (~SERVER_SOCKET_MSG);

        if(light_sck_msg_q)
        {
            ret = rtos_push_to_queue(&light_sck_msg_q, &txmsg, BEKEN_NO_WAIT);
            if(kNoErr != ret)
            {
                os_printf("light_sck_msg_q tx failed\r\n");
                os_free(txbuf);
            }
        }
    }
    else if(txbuf)
    {
        os_free(txbuf);
    }
}

void light_sck_cs_txdata_handler(light_msg_T *TxMsg)
{
    int fd = -1;
    LIST_HEADER_T *pos, *n;
    light_socket_handler_meg_T *sckHdl;
    int len = 0;

    if((TxMsg->fd < 0x00FF) && (TxMsg->msg))
    {
        fd = TxMsg->fd;

        if(light_tcp_fd_find_connect(fd) == fd)
        {
            len = send( fd, TxMsg->msg, TxMsg->len, 0 );
        }
    }
    else if( TxMsg->msg )
    {
        list_for_each_safe(pos, n, &light_tcp_link_node_list)
        {
            sckHdl = list_entry(pos, light_socket_handler_meg_T, list);
            fd = sckHdl->fd;
            len = send( fd, TxMsg->msg, TxMsg->len, 0 );
        }
    }

    if(TxMsg->msg)
    {
        os_free(TxMsg->msg);
        TxMsg->msg = NULL;
    }
}

int light_sck_cs_rxdat_sender(int fd, char *buf, int len)
{
    light_msg_T RxMsg;
    bk_err_t ret;
    char *rxbuf = NULL;
    unsigned long queue_len;

    GLOBAL_INT_DECLARATION();
    if(light_sck_msg_q == NULL)
    {
        return kGeneralErr;
    }

    GLOBAL_INT_DISABLE();
    queue_len = rtos_get_sema_count(&light_sck_msg_q);
    if(queue_len >= (LIGHT_SOCKET_MSG_QUEUE_LENGTH / 2))
    {
        GLOBAL_INT_RESTORE();
        os_printf("rx discard!\r\n");
        return kGeneralErr;
    }
    GLOBAL_INT_RESTORE();

    if(len)
    {
        rxbuf = (char *) os_malloc( len );
    }

    if((rxbuf) && (len))
    {
        memcpy(rxbuf, buf, len);

        RxMsg.len = len;
        RxMsg.msg = rxbuf;
        RxMsg.msgtype = LIGHT_SERVER_RX_MEG;
        RxMsg.fd = fd | SERVER_SOCKET_MSG;

        ret = rtos_push_to_queue(&light_sck_msg_q, &RxMsg, BEKEN_NO_WAIT);
        if(kNoErr != ret)
        {
            os_printf("light_sck_msg_q rx failed\r\n");
        }

        return kNoErr;
    }
    else if(rxbuf)
    {
        os_free(rxbuf);
    }

    return kGeneralErr;
}

void light_sck_cs_rxdata_handler(light_msg_T *RxMsg)
{
    int sta;

    sta = light_cmmun_ptcl_msg_input(RxMsg->msg, RxMsg->len, RxMsg->fd);
    if((sta) || (get_message_bady_node_number()))
    {
        protocol_message_body_handler();
    }

    if(RxMsg->msg)
    {
        os_free(RxMsg->msg);
        RxMsg->msg = NULL;
    }
}

void light_app_main ( beken_thread_arg_t arg )
{
    rtos_delay_milliseconds( 1000 );

    bk_err_t ret;
    light_msg_T msg;

    while(1)
    {
        ret = rtos_pop_from_queue(&light_sck_msg_q, &msg, BEKEN_WAIT_FOREVER);
        if(kNoErr != ret)
        {
            goto exit;
        }

        switch(msg.msgtype)
        {
        case LIGHT_SERVER_RX_MEG:
            light_sck_cs_rxdata_handler(&msg);
            break;

        case LIGHT_SERVER_TX_MEG:
            light_sck_cs_txdata_handler(&msg);
            break;

        default:
            os_printf("Error msg.msgtype:%d\r\n", msg.msgtype);
            break;
        }
    }

exit:
    if(light_sck_msg_q)
    {
        rtos_deinit_queue(&light_sck_msg_q);
    }
    rtos_delete_thread( NULL );
}

int demo_start( void )
{
    bk_err_t err = kNoErr;

    light_gpio4_led_intial();

    err  = rtos_init_queue(&light_sck_msg_q,
                           "Lightskt_queue",
                           sizeof(light_msg_T),
                           LIGHT_SOCKET_MSG_QUEUE_LENGTH);
    if (kNoErr != err )
    {
        os_printf("Create Lightskt_queue failed\r\n");
        goto exit;
    }

    err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "light",
                              (beken_thread_function_t)light_app_main,
                              0x800,
                              (beken_thread_arg_t)0 );
    if (kNoErr != err )
    {
        os_printf("Create light failed\r\n");
        goto exit;
    }

    err = light_socket_application_start();
    if(err != kNoErr)
    {
        goto exit;
    }

    return err;
exit:
    if(light_sck_msg_q)
    {
        rtos_deinit_queue(&light_sck_msg_q);
    }
    return err;
}

#endif /** LIGHT_SERVER_APPLICATION **/


#include "light_client_app_demo.h"
#include <common/bk_include.h>

#if LIGHT_CLIENT_APP_DEMO

#include "Arch.h"
#include <os/mem.h>
#include "uart_pub.h"
#include "stdarg.h"
#include <common/bk_include.h>
#include <os/os.h>
#include "Error.h"
#include "portmacro.h"
#include "string.h"
#include "sockets.h"
#include "gpio_pub.h"
#include "light_client_sockt.h"
#include "light_commun_protocol.h"

typedef enum
{
    LINGHT_NULL_MSG = 0,
    LINGHT_SOCKET_RX_MSG,
    LINGHT_SOCKET_TX_MSG,
} LIGHT_CLIENT_MSG_TYPE;


typedef struct
{
    LIGHT_CLIENT_MSG_TYPE type;
    int len;
    unsigned char *msg;
    int from_to;
} light_client_msg_T;

beken_queue_t light_client_msg_q = NULL;


void light_client_gpio4_led_intial(void)
{
    GPIO_INDEX gpio_idx;
    UINT32 mode;
    u32 param;

    gpio_idx = GPIO4;
    //mode = GMODE_OUTPUT_PULLUP;
    mode = GMODE_OUTPUT;
    param =  GPIO_CFG_PARAM(gpio_idx, mode);

    sddev_control(GPIO_DEV_NAME, CMD_GPIO_CFG, &param);

    gpio_idx = GPIO4;
    param =  GPIO_CFG_PARAM(gpio_idx, 1);
    sddev_control(GPIO_DEV_NAME, CMD_GPIO_OUTPUT, &param);
}


int light_client_input_msg_sender(int from, char *msg, unsigned int len)
{
    int ret;
    light_client_msg_T rxmsg;
    char *rxbuf = NULL;
    unsigned long queue_len;

    GLOBAL_INT_DECLARATION();
    if(light_client_msg_q == NULL)
        return t_error;

    GLOBAL_INT_DISABLE();
    queue_len = rtos_get_sema_count(&light_client_msg_q);
    if(queue_len > (LIGHT_CLIENT_SOCKET_MSG_QUEUE_LENGTH / 2))
    {
        GLOBAL_INT_RESTORE();
        os_printf("rx discard!\r\n");
        return kGeneralErr;
    }
    GLOBAL_INT_RESTORE();

    if(len)
    {
        rxbuf = (char *) os_malloc( len );
        if(rxbuf == NULL)
        {
            os_printf("malloc failed\r\n");
            return t_error;
        }

        memcpy(rxbuf, msg, len);
    }

    rxmsg.type = LINGHT_SOCKET_RX_MSG;
    rxmsg.len = len;
    rxmsg.msg = rxbuf;
    rxmsg.from_to = from;

    ret = rtos_push_to_queue(&light_client_msg_q, &rxmsg, BEKEN_NO_WAIT);
    if(kNoErr != ret)
    {
        os_printf("light rec queue failed\r\n");
        if(rxbuf)
        {
            os_free(rxbuf);
            rxbuf = NULL;
        }
        return t_error;
    }

    return t_ok;
}

void light_client_input_msg_handler(light_client_msg_T *msg)
{
    int status;

    switch(msg->from_to & 0xFF000000)
    {
    case CLIENT_SOCKET_MSG:
        status = light_cmmun_ptcl_msg_input(msg->msg, msg->len, msg->from_to);
        if((status == PoK) || (get_message_bady_node_number()))
        {
            protocol_message_body_handler();
        }
        break;

    default:
        os_printf("unknown input msg destination!\r\n");
        break;
    }

    if(msg->msg)
    {
        os_free(msg->msg);
        msg->msg = NULL;
    }
}

int light_client_output_msg_sender(int to, char *msg, unsigned int len)
{
    int ret;
    light_client_msg_T txmsg;
    char *txbuf = NULL;

    if(light_client_msg_q == NULL)
        return t_error;

    if(len)
    {
        txbuf = (char *) os_malloc( len );
        if(txbuf == NULL)
        {
            os_printf("malloc failed\r\n");
            return t_error;
        }

        memcpy(txbuf, msg, len);
    }

    txmsg.type = LINGHT_SOCKET_TX_MSG;
    txmsg.len = len;
    txmsg.msg = txbuf;
    txmsg.from_to = to;

    ret = rtos_push_to_queue(&light_client_msg_q, &txmsg, 100);
    if(kNoErr != ret)
    {
        os_printf("light send queue failed\r\n");
        if(txbuf)
        {
            os_free(txbuf);
            txbuf = NULL;
        }
        return t_error;
    }

    return t_ok;
}

void light_client_output_msg_handler(light_client_msg_T *msg)
{
    int to;

    switch(msg->from_to & 0xFF000000)
    {
    case CLIENT_SOCKET_MSG:
        to = msg->from_to & (~CLIENT_SOCKET_MSG);
        tcp_client_send_remote_server_data(to, msg->msg, msg->len);
        break;

    default:
        os_printf("unknown out msg destination!\r\n");
        break;
    }

    if(msg->msg)
    {
        os_free(msg->msg);
        msg->msg = NULL;
    }
}

int create_light_client_msg_queue(beken_queue_t *queue)
{
    int err = kNoErr;

    err  = rtos_init_queue( queue,
                            "Lightskt_queue",
                            sizeof(light_client_msg_T),
                            LIGHT_CLIENT_SOCKET_MSG_QUEUE_LENGTH);
    if (kNoErr != err )
    {
        os_printf("create queue failed\r\n");
        return t_error;
    }

    return t_ok;
}


void light_client_app_main ( beken_thread_arg_t arg )
{
    bk_err_t ret;
    light_client_msg_T msg;

    rtos_delay_milliseconds( 100 );

    ret = create_light_client_msg_queue(&light_client_msg_q);
    if(ret != t_ok)
    {
        os_printf("create light_client_msg_q error\r\n");
        goto exit;
    }

    light_net_connect_start();

    while(1)
    {
        ret = rtos_pop_from_queue(&light_client_msg_q, &msg, BEKEN_WAIT_FOREVER);
        switch(msg.type)
        {
        case LINGHT_NULL_MSG:

            break;
        case LINGHT_SOCKET_RX_MSG:
            light_client_input_msg_handler(&msg);
            break;

        case LINGHT_SOCKET_TX_MSG:
            light_client_output_msg_handler(&msg);
            break;

        default:
            os_printf("Error msgtype:%d\r\n", msg.type);
            break;
        }
    }

exit:
    rtos_delete_thread( NULL );
}

int demo_start( void )
{
    bk_err_t err = kNoErr;

    light_client_gpio4_led_intial();

    err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "light_clt_thd",
                              (beken_thread_function_t)light_client_app_main,
                              0x800,
                              (beken_thread_arg_t)0 );
    if (kNoErr != err )
    {
        os_printf("create light_Client thread failed\r\n");
    }

    return err;
}
#endif


/***
  Time:2017.10.19
  Beken

  the demo is function

  UART1      TCP
   Rx   ->    Send
   Tx   <-    Recv

***/

#include "uart1_tcp_server_demo.h"
#include <common/bk_include.h>

#if UART_TCP_SERVER_DEMO
#include <common/bk_include.h>
#include <os/os.h>
#include <common/bk_list.h>
#include <os/mem.h>
#include "sockets.h"
#include "Error.h"
#include "uart_pub.h"
#include <common/bk_err.h>
#include "lwip/sockets.h"
#include <os/str.h>
#include "string.h"
#include "Arch.h"
#include <driver/uart.h>
#include "bk_uart.h"

#define UART1_TCP_SERVER_PORT            12000

typedef struct
{
    LIST_HEADER_T list;
    int fd;
} socket_handler_fd_meg_T;

typedef enum
{
    NULL_MSG = 0,
    TCP_TX_MSG,
    TCP_RX_MSG,
    UART1_TX_MSG,
    UART1_RX_MSG,
} UART_TCP_MSG_TYPE;

typedef struct
{
    UART_TCP_MSG_TYPE type;
    int len;
    char *msg;
} uart1_tcp_msg_T;

beken_queue_t uart1_msg_q = NULL;
beken_queue_t tcp_skt_msg_q = NULL;
LIST_HEADER_T tcp_link_node_list = {&tcp_link_node_list, &tcp_link_node_list};

extern bk_err_t uart_write_byte(uart_id_t id, uint8_t data);
int tcp_sck_txdat_sender(int fd, char *rxbuf, int len);

void uart1_rxdata_sender(void)
{
    bk_err_t ret;
    int len = 0;
    uart1_tcp_msg_T u1Rxmsg;

    if(uart1_msg_q == NULL)
    {
        return;
    }

    len = uart_get_length_in_buffer(UART_ID_0);

    u1Rxmsg.type = UART1_RX_MSG;
    u1Rxmsg.len = len;
    u1Rxmsg.msg = (void *)0;

    ret = rtos_push_to_queue(&uart1_msg_q, &u1Rxmsg, BEKEN_WAIT_FOREVER);
    if(kNoErr != ret)
    {
        os_printf("uart1_msg_q rx sender failed\r\n");
    }
}

void uart1_txdata_sender(unsigned char *txbuf, int len)
{
    bk_err_t ret;
    uart1_tcp_msg_T u1txmsg;

    if(uart1_msg_q == NULL)
    {
        return;
    }

    u1txmsg.type = UART1_TX_MSG;
    u1txmsg.len = len;
    u1txmsg.msg = txbuf;

    ret = rtos_push_to_queue(&uart1_msg_q, &u1txmsg, BEKEN_NEVER_TIMEOUT);
    if(kNoErr != ret)
    {
        os_printf("uart1_msg_q tx sender failed\r\n");
    }
}

void uart1_txdata_handler(uart1_tcp_msg_T u1txmsg)
{
    int len = 0;
    int i = 0;
    char *buf = NULL;

    if(u1txmsg.msg)
    {
        buf = u1txmsg.msg;
        len = u1txmsg.len;

        for(i = 0; i < len; i++)
        {
            uart_write_byte(BK_UART_1, buf[i]);
        }

        os_free(buf);
    }

}

void uart1_rxdata_handler(uart1_tcp_msg_T u1rxmsg)
{
    char *Rxbuf = NULL;
    int len = 0;
    bk_err_t ret;

    len = u1rxmsg.len;

    if(len)
    {
        Rxbuf = (char *) os_malloc(len);

        if(Rxbuf)
        {
            if (bk_uart_read_bytes(UART_ID_0, Rxbuf, len, BEKEN_NO_WAIT) < BK_OK)
            {
                goto exit;
            }

            ret = tcp_sck_txdat_sender(0xFF, Rxbuf, len);
            if (kNoErr == ret)
            {
                Rxbuf = NULL;
            }
            else
            {
                os_printf("uart1 rxdata handler error\r\n");
            }
        }
    }

exit:
    if(Rxbuf)
    {
        os_free( Rxbuf );
    }
}

void uart1_tx_rx_handle_thread( beken_thread_arg_t arg )
{
    bk_err_t ret;
    uart1_tcp_msg_T u1msg;

    while(1)
    {
        ret = rtos_pop_from_queue(&uart1_msg_q, &u1msg, BEKEN_WAIT_FOREVER);

        switch(u1msg.type)
        {
        case UART1_RX_MSG:
            uart1_rxdata_handler(u1msg);
            break;
        case UART1_TX_MSG:
            uart1_txdata_handler(u1msg);
            break;
        default:
            os_printf("unknown msg\r\n");
            break;
        }
    }


exit:
    //    rtos_deinit_queue(&uart1_msg_q);
    rtos_delete_thread( NULL );
}

int tcp_connect_handle_register(int fd)
{
    socket_handler_fd_meg_T *sckHdl;

    sckHdl = (socket_handler_fd_meg_T *) os_malloc(sizeof(socket_handler_fd_meg_T));
    if ( sckHdl )
    {
        sckHdl->fd = fd;
        list_add_head(&sckHdl->list, &tcp_link_node_list);
    }
    else
    {
        os_printf( "tcp connect num add failed");
        return kGeneralErr;
    }

    return kNoErr;
}

int tcp_connect_handle_unregister(int fd)
{
    socket_handler_fd_meg_T *sckHdl;
    LIST_HEADER_T *pos, *n;

    list_for_each_safe(pos, n, &tcp_link_node_list)
    {
        sckHdl = list_entry(pos, socket_handler_fd_meg_T, list);
        if(sckHdl->fd == fd)
        {
            list_del(pos);
            os_free(sckHdl);
        }
    }

    return kNoErr;
}

int tcp_sck_txdat_sender(int fd, char *rxbuf, int len)
{
    uart1_tcp_msg_T tcpmsg;
    bk_err_t ret;

    if (( rxbuf ) && ( len ))
    {
        tcpmsg.len = len;
        tcpmsg.msg = rxbuf;
        tcpmsg.type = TCP_TX_MSG;

        ret = rtos_push_to_queue(&tcp_skt_msg_q, &tcpmsg, BEKEN_NEVER_TIMEOUT);
        if(kNoErr != ret)
        {
            os_printf("tcp sck txdat sender failed\r\n");
        }

        return kNoErr;
    }
    else if( rxbuf )
    {
        os_free( rxbuf );
    }

    return kGeneralErr;
}

int tcp_sck_txdat_handler(uart1_tcp_msg_T Sdmsg)
{
    int fd = 0;
    int len = 0;
    char *buf = NULL;
    socket_handler_fd_meg_T *sckHdl;
    LIST_HEADER_T *pos, *n;

    if(Sdmsg.msg)
    {
        buf = Sdmsg.msg;
        len = Sdmsg.len;

        list_for_each_safe(pos, n, &tcp_link_node_list)
        {
            sckHdl = list_entry(pos, socket_handler_fd_meg_T, list);
            fd = sckHdl->fd;
            len = send( fd, buf, len, 0 );
        }

        os_free( buf );
        buf = NULL;
        return kNoErr;
    }

    return kGeneralErr;
}

int tcp_sck_rxdat_sender(int fd, char *buf, int len)
{
    uart1_tcp_msg_T tcpmsg;
    bk_err_t ret;
    char *rxbuf = NULL;

    if( len )
    {
        rxbuf = (char *) os_malloc( len );
    }

    if(( rxbuf ) && ( len ))
    {
        memcpy(rxbuf, buf, len);
        tcpmsg.len = len;
        tcpmsg.msg = rxbuf;
        tcpmsg.type = TCP_RX_MSG;

        ret = rtos_push_to_queue(&tcp_skt_msg_q, &tcpmsg, BEKEN_NEVER_TIMEOUT);
        if(kNoErr != ret)
        {
            os_printf("tcp_sck_txdat_sender failed\r\n");
        }

        return kNoErr;
    }
    else if(rxbuf)
    {
        os_free( rxbuf );
    }

    return kGeneralErr;
}

int tcp_sck_rxdat_handler(uart1_tcp_msg_T Sdmsg)
{
    if(( Sdmsg.msg ) && ( Sdmsg.len ))
    {
        uart1_txdata_sender(Sdmsg.msg, Sdmsg.len);
        return kNoErr;
    }
    else if( Sdmsg.msg )
    {
        os_free( Sdmsg.msg );
    }

    return kGeneralErr;
}

void tcp_client_sender_thread( beken_thread_arg_t arg )
{
    bk_err_t err = kNoErr;
    int fd = (int) arg;
    int len = 0;
    char *buf = NULL;
    uart1_tcp_msg_T SdMsg;

    while ( 1 )
    {
        err = rtos_pop_from_queue(&tcp_skt_msg_q, &SdMsg, BEKEN_WAIT_FOREVER);
        if ( kNoErr == err )
        {
            switch( SdMsg.type )
            {
            case  TCP_TX_MSG:
                tcp_sck_txdat_handler( SdMsg );
                break;
            case  TCP_RX_MSG:
                tcp_sck_rxdat_handler( SdMsg );
                break;
            default:
                os_printf("unknown msg\r\n");
                break;
            }
        }
    }

exit:
    if ( err != kNoErr ) os_printf( "TCP client Sender thread exit with err: %d\r\n", err );
    if ( buf != NULL ) os_free( buf );

    rtos_delete_thread( NULL );
}


void tcp_client_connect_thread( beken_thread_arg_t arg )
{
    bk_err_t err = kNoErr;
    int fd = (int) arg;
    int len = 0;
    fd_set readfds;
    char *buf = NULL;
    struct timeval t;

    buf = (char *) os_malloc( 1024 );
    ASSERT(buf);

    t.tv_sec = 5;
    t.tv_usec = 0;

    while ( 1 )
    {
        FD_ZERO( &readfds );
        FD_SET( fd, &readfds );

        select( fd + 1, &readfds, NULL, NULL, &t);

        if ( FD_ISSET( fd, &readfds ) ) /*one client has data*/
        {
            len = recv( fd, buf, 1024, 0 );

            if ( len == 0 )
            {
                os_printf( "tcp(fd:%d) is disconnected\r\n", fd );
                goto exit;
            }

            tcp_sck_rxdat_sender(fd, buf, len);
        }
    }

exit:
    if ( err != kNoErr )
        os_printf( "TCP client thread exit(err: %d)\r\n", err );

    if ( buf != NULL )
        os_free( buf );

    close( fd );
    rtos_delete_thread( NULL );
}

void uart_tcp_server_listener_thread( beken_thread_arg_t arg )
{
    (void)( arg );
    bk_err_t err = kNoErr;
    char client_ip_str[16];
    int tcp_listen_fd = -1, client_fd = -1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sockaddr_t_size = sizeof(client_addr);
    fd_set readfds;

    tcp_listen_fd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons( UART1_TCP_SERVER_PORT );
    err = bind( tcp_listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr) );

    err = listen( tcp_listen_fd, 0 );

    while ( 1 )
    {
        FD_ZERO( &readfds );
        FD_SET( tcp_listen_fd, &readfds );

        select( tcp_listen_fd + 1, &readfds, NULL, NULL, NULL);

        if ( FD_ISSET( tcp_listen_fd, &readfds ) )
        {
            client_fd = accept( tcp_listen_fd, (struct sockaddr *) &client_addr, &sockaddr_t_size );
            if ( client_fd >= 0 )
            {
                os_strcpy( client_ip_str, inet_ntoa( client_addr.sin_addr ) );
                os_printf( "TCP Client %s:%d connected, fd: %d", client_ip_str, client_addr.sin_port, client_fd );
                if( kNoErr != rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                                                  "TCP Clients",
                                                  (beken_thread_function_t)tcp_client_connect_thread,
                                                  0x800,
                                                  (beken_thread_arg_t)client_fd ) )
                {
                    close( client_fd );
                    client_fd = -1;
                }
                else
                {
                    tcp_connect_handle_register(client_fd);
                }
            }
        }
    }

exit:
    if ( err != kNoErr )
    {
        os_printf( "server listerner thread exit,err: %d\r\n", err );
    }
    close( tcp_listen_fd );
    rtos_delete_thread( NULL );
}

void demo_start(void)
{
    bk_err_t ret;

    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();

    ret  = rtos_init_queue(&uart1_msg_q,
                           "Uart1msg_queue",
                           sizeof(uart1_tcp_msg_T),
                           6);
    if (kNoErr != ret )
    {
        os_printf("Create Uart1msg_queue failed\r\n");
        goto exit;
    }

    ret  = rtos_init_queue(&tcp_skt_msg_q,
                           "TCPsktUart_queue",
                           sizeof(uart1_tcp_msg_T),
                           6);
    if (kNoErr != ret )
    {
        os_printf("Create TCPsktUart_queue failed\r\n");
        goto exit;
    }

    ret = rtos_create_thread( NULL,
                              BEKEN_APPLICATION_PRIORITY,
                              "tcp_svr_lst",
                              (beken_thread_function_t)uart_tcp_server_listener_thread,
                              0x800,
                              (beken_thread_arg_t)0 );
    if (kNoErr != ret )
    {
        os_printf("Create UART1_TxRx_Handle failed\r\n");
        goto exit;
    }

    ret = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "UART1_TxRx_Handle",
                              uart1_tx_rx_handle_thread,
                              0x800,
                              0 );
    if (kNoErr != ret )
    {
        os_printf("Create UART1_TxRx_Handle failed\r\n");
        goto exit;
    }

    ret = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "client_Sender",
                              tcp_client_sender_thread,
                              0x800,
                              0 );
    if (kNoErr != ret )
    {
        os_printf("Create client_Sender failed\r\n");
        goto exit;
    }

    GLOBAL_INT_RESTORE();
    return;

exit:
    os_printf("Failed\r\n");
    GLOBAL_INT_RESTORE();
}

#endif /*UART_TCP_SERVER_DEMO*/

#include "light_socket.h"
#include <common/bk_include.h>

#if LIGHT_SERVER_APPLICATION
#include <os/mem.h>
#include "uart_pub.h"
#include "stdarg.h"
#include <common/bk_include.h>
#include <os/os.h>
#include "Error.h"
#include "portmacro.h"
#include "sockets.h"
#include <os/str.h>
#include "light_commun_protocol.h"

#ifdef TCP_SERVER_LIGHT

#define Light_SERVER_PORT            16000 /*set up a tcp server,port at 20000*/

LIST_HEADER_T light_tcp_link_node_list = {&light_tcp_link_node_list, &light_tcp_link_node_list};

int light_tcp_connect_handler_register(int fd)
{
    light_socket_handler_meg_T *LsckHdl;

    LsckHdl = (light_socket_handler_meg_T *) os_malloc(sizeof(light_socket_handler_meg_T));
    if(LsckHdl)
    {
        LsckHdl->fd = fd;
        list_add_head(&LsckHdl->list, &light_tcp_link_node_list);
    }
    else
    {
        os_printf( "Light_tcp_connect_num_add failed");
        return -1;
    }

    return kNoErr;
}

int light_tcp_connect_handler_unregister(int fd)
{
    light_socket_handler_meg_T *LsckHdl;
    LIST_HEADER_T *pos, *n;

    list_for_each_safe(pos, n, &light_tcp_link_node_list)
    {
        LsckHdl = list_entry(pos, light_socket_handler_meg_T, list);
        if(LsckHdl->fd == fd)
        {
            list_del(pos);
            os_free(LsckHdl);
        }
    }

    return kNoErr;
}

int light_tcp_fd_find_connect(int fd)
{
    light_socket_handler_meg_T *LsckHdl;
    LIST_HEADER_T *pos, *n;

    list_for_each_safe(pos, n, &light_tcp_link_node_list)
    {
        LsckHdl = list_entry(pos, light_socket_handler_meg_T, list);
        if(LsckHdl->fd == fd)
        {
            return fd;
        }
    }

    return -1;
}

void light_cp_client_connect_thread( beken_thread_arg_t arg )
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

    light_tcp_connect_handler_register(fd);
    protocol_serial_number_register(fd | SERVER_SOCKET_MSG, 0xFF, 0xFF);

    while ( 1 )
    {
        FD_ZERO( &readfds );
        FD_SET( fd, &readfds );

        select( fd + 1, &readfds, NULL, NULL, &t);

        if ( FD_ISSET( fd, &readfds ) ) /*one client has data*/
        {
            len = recv( fd, buf, 1024, 0 );

            if ( len <= 0 )
            {
                os_printf( "TCP Client is disconnected, fd: %d\r\n", fd );
                goto exit;
            }

            light_sck_cs_rxdat_sender(fd, buf, len);
        }
    }

exit:
    if ( err != kNoErr )
    {
        os_printf( "TCP client thread exit with err: %d\r\n", err );
    }

    if ( buf != NULL )
    {
        os_free( buf );
    }

    protocol_msg_unregister(fd | SERVER_SOCKET_MSG);
    light_tcp_connect_handler_unregister(fd);

    close( fd );
    rtos_delete_thread( NULL );
}

void light_tcp_server_listener_thread( beken_thread_arg_t arg )
{
    (void)( arg );
    bk_err_t err = kNoErr;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sockaddr_t_size = sizeof(client_addr);
    char client_ip_str[16];
    int tcp_listen_fd = -1, client_fd = -1;
    fd_set readfds;

    tcp_listen_fd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;/* Accept conenction request on all network interface */
    server_addr.sin_port = htons( Light_SERVER_PORT );/* Server listen on port: 54321 */
    err = bind( tcp_listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr) );

    err = listen( tcp_listen_fd, 0);

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
                os_printf( "TCP Client %s:%d connected, fd: %d\r\n", client_ip_str, client_addr.sin_port, client_fd );
                if ( kNoErr
                        != rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                                               "TCP Clients",
                                               (beken_thread_function_t)light_cp_client_connect_thread,
                                               0x800,
                                               (beken_thread_arg_t)client_fd ) )
                {
                    close( client_fd );
                    client_fd = -1;
                }
            }
        }
    }

    if ( err != kNoErr )
        os_printf( "Server listerner thread exit with err: %d", err );

    close( tcp_listen_fd );
    rtos_delete_thread( NULL );
}
#endif

int light_socket_application_start(void)
{
    bk_err_t err = kNoErr;

    /* Start a thread*/
    err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "light",
                              (beken_thread_function_t)light_tcp_server_listener_thread,
                              0x800,
                              (beken_thread_arg_t)0 );
    return err;
}


#endif


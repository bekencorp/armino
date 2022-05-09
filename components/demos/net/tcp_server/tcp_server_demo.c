/**
 ******************************************************************************
 * @file    tcp_server_demo.c
 * @author
 * @version V1.0.0
 * @date
 * @brief   tcp server Demo
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2017 Beken Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ******************************************************************************
 */
#include "tcp_server_demo.h"
#include <common/bk_include.h>

#if TCP_SERVER_DEMO
#include <common/bk_include.h>
#include <os/os.h>
#include <common/bk_err.h>
#include "lwip/sockets.h"
#include <os/mem.h>
#include <os/str.h>


#define NET_TCP_SERVER_PORT  22000

void net_tcp_client_thread( beken_thread_arg_t arg )
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

        if ( FD_ISSET( fd, &readfds ) ) /* one client has data */
        {
            len = recv( fd, buf, 1024, 0 );

            if ( len == 0 )
            {
                os_printf( "TCP Client is disconnected, fd: %d", fd );
                goto exit;
            }

            len = send( fd, buf, len, 0 );
        }
    }

exit:
    if ( err != kNoErr )
    {
        os_printf( "TCP client thread exit with err: %d", err );
    }

    if ( buf != NULL )
    {
        os_free( buf );
    }

    close( fd );
    rtos_delete_thread( NULL );
}

void net_tcp_server_thread( beken_thread_arg_t arg )
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
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons( NET_TCP_SERVER_PORT );
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
                os_printf( "tcp client %s:%d connected, fd: %d", client_ip_str, client_addr.sin_port, client_fd );
                if( kNoErr != rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                                                  "TCP Clients",
                                                  (beken_thread_function_t)net_tcp_client_thread,
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
    {
        os_printf( "Server listerner thread exit with err: %d", err );
    }

    close( tcp_listen_fd );
    rtos_delete_thread( NULL );
}

int demo_start( void )
{
    bk_err_t err = kNoErr;

    err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "net_tcp_server",
                              (beken_thread_function_t)net_tcp_server_thread,
                              0x800,
                              (beken_thread_arg_t)0 );

    return err;
}

#endif /*TCP_SERVER_DEMO*/


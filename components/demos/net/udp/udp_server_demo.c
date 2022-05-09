/**
 ******************************************************************************
 * @file    udp_server_demo.c
 * @author
 * @version V1.0.0
 * @date
 * @brief   udp server Demo
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
#include "udp_server_demo.h"
#include <common/bk_include.h>

#if UDP_SERVER_DEMO
#include <common/bk_include.h>
#include <os/os.h>
#include <common/bk_list.h>
#include <os/mem.h>
#include <os/str.h>
#include "lwip/sockets.h"
#include "Error.h"
#include "uart_pub.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/inet.h"

#define UDP_LISTENER_PORT 55000

void show_udp_recv(char *buf, unsigned int len)
{
    while(len--)
    {
        os_printf("%02x", *buf++);
    }
    os_printf("\r\n");
}

/* TCP server listener thread */
void udp_server_thread( beken_thread_arg_t arg )
{


    int udp_listen_fd = -1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bk_err_t err = kNoErr;
    char *buf = NULL;
    int len = 0;
    fd_set readfds;

    buf = (char *) os_malloc( 1024 );
    if(buf == NULL)
    {
        os_printf("buf == NULL\r\n");
        goto exit;
    }

    udp_listen_fd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );//Make UDP socket
    if(udp_listen_fd == -1)
    {
        os_printf("udp_listen_fd == -1\r\n");
        goto exit;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(UDP_LISTENER_PORT);

    err = bind( udp_listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr) );

    while ( 1 )
    {
        FD_ZERO( &readfds );
        FD_SET( udp_listen_fd, &readfds );

        select( udp_listen_fd + 1, &readfds, NULL, NULL, NULL);

        if ( FD_ISSET( udp_listen_fd, &readfds ) )
        {
            if ( FD_ISSET( udp_listen_fd, &readfds ) ) /*one client has data*/
            {
                len = recvfrom( udp_listen_fd, buf, 1024, 0 , (struct sockaddr *) &client_addr, &client_addr_len);
                show_udp_recv(buf, len);
                if(len)
                {
                    len = sendto( udp_listen_fd, buf, len, 0 , (struct sockaddr *) &client_addr, sizeof(client_addr));
                }
            }
        }
    }

exit:
    if ( err != kNoErr )
    {
        os_printf( "Server listener thread exit with err: %d", err );
    }

    if(udp_listen_fd)
    {
        close( udp_listen_fd );
    }

    if(buf)
    {
        os_free(buf);
    }

    rtos_delete_thread( NULL );
}


int demo_start( void )
{
    bk_err_t err = kNoErr;

    err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "udp_server",
                              (beken_thread_function_t)udp_server_thread,
                              0x800,
                              (beken_thread_arg_t)0 );
    return err;
}



#endif

// EOF


/**
 ******************************************************************************
 * @file    udp_client_demo.c
 * @author
 * @version V1.0.0
 * @date
 * @brief   udp client Demo
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
#include "udp_client_demo.h"
#include <common/bk_include.h>

#if UDP_CLIENT_DEMO
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
#include "string.h"
#include "Wlan_ui_pub.h"
#include "stdio.h"
#include "stdlib.h"

beken_timer_t *udp_client_connect_timer = NULL;

int check_connect_wifi(void)
{
    IPStatusTypedef ipStatus;
    bk_err_t ret = kNoErr;

    os_memset(&ipStatus, 0x0, sizeof(IPStatusTypedef));
    ret = bk_wlan_get_ip_status(&ipStatus, STATION);

    if(strcmp(ipStatus.ip, "0.0.0.0") != 0)
    {
        os_printf("ip:%s \r\n", ipStatus.ip);
        return 0;
    }
    return 1;
}

void udp_client_demo_thread( beken_thread_arg_t arg )
{
    bk_err_t err = kNoErr;
    struct sockaddr_in server_addr;
    socklen_t sockaddr_t_size = sizeof(server_addr);
    int SkFd = -1;
    unsigned int count = 0;
    char *buf = NULL;
    int len = 0;
    fd_set readfds;

    buf = (char *) os_malloc( 1024 );
    if(buf == NULL)
    {
        os_printf("buf == NULL\r\n");
        goto exit;
    }

    SkFd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );//Make UDP socket
    if(SkFd == -1)
    {
        os_printf("SkFd == -1\r\n");
        goto exit;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = IPADDR_BROADCAST;
    //server_addr.sin_addr.s_addr = inet_addr("192.168.5.103");
    server_addr.sin_port = htons(UDP_Client_Port);
    os_printf("UDP Client Run\r\n");

    while ( 1 )
    {
        sprintf(buf, "UDP Client Count:%d\r\n", count);
        count++;

        len = strlen(buf);
        len = sendto( SkFd, buf, len, 0 , (struct sockaddr *) &server_addr, sizeof(server_addr));

        rtos_delay_milliseconds( 500 );
    }

exit:
    if ( err != kNoErr )
    {
        os_printf( "udp client thread exit with err: %d", err );
    }

    if(SkFd)
    {
        close( SkFd );
    }

    if(buf)
    {
        os_free(buf);
    }

    rtos_start_timer(udp_client_connect_timer);
    rtos_delete_thread( NULL );
}

void udp_client_check_net_timer_alarm( void *arg )
{
    bk_err_t err = kNoErr;
    (void)arg;

    if(!check_connect_wifi())
    {
        rtos_stop_timer( udp_client_connect_timer );
        err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                                  "udp_client",
                                  (beken_thread_function_t)udp_client_demo_thread,
                                  0x800,
                                  (beken_thread_arg_t)0 );
        if(err != kNoErr)
        {
            rtos_start_timer(udp_client_connect_timer);
        }
    }
}

int create_udp_client_check_net_timer(void)
{
    bk_err_t err;

    if(udp_client_connect_timer == NULL)
    {
        udp_client_connect_timer = (beken_timer_t *) os_malloc( sizeof(beken_timer_t) );
    }

    if(udp_client_connect_timer == NULL)
    {
        os_printf("tcp_connect_timer error!\r\n");
        return kGeneralErr;
    }

    err = rtos_init_timer(udp_client_connect_timer, 500, udp_client_check_net_timer_alarm, 0);
    if(kNoErr != err)
    {
        if(udp_client_connect_timer)
        {
            os_free(udp_client_connect_timer);
            udp_client_connect_timer = NULL;
        }
        os_printf("tcp_connect_timer error!\r\n");
    }

    return err;
}

void udp_client_connect_timer_destroy( void )
{
    rtos_deinit_timer( udp_client_connect_timer );
    if(udp_client_connect_timer)
    {
        os_free(udp_client_connect_timer);
        udp_client_connect_timer = NULL;
    }
}

int demo_start( void )
{
    bk_err_t err;

    err = create_udp_client_check_net_timer();
    if(kNoErr != err)
    {
        return err;
    }

    err = rtos_start_timer(udp_client_connect_timer);
    if(kNoErr != err)
    {
        udp_client_connect_timer_destroy();
        os_printf("timer start failed!\r\n");

        return err;
    }

    return kNoErr;
}

#endif

// EOF


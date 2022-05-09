/**
 ******************************************************************************
 * @file    wifi_scan.c
 * @author
 * @version V1.0.0
 * @date
 * @brief   scan wifi hot spots demo
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
#include "wifi_softap.h"
#include <common/bk_include.h>

#if WIFI_SOFTAP_DEMO

#include <common/bk_include.h>
#include <os/os.h>
#include <common/bk_err.h>
#include "wlan_ui_pub.h"
#include "uart_pub.h"
#include <os/mem.h>
#include "rw_pub.h"
#include "param_config.h"
#include <os/str.h>

void wifi_softap_app_init(char *ap_ssid, char *ap_key)
{
    network_InitTypeDef_st wNetConfig;

    os_memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));

    os_strcpy((char *)wNetConfig.wifi_ssid, ap_ssid);
    os_strcpy((char *)wNetConfig.wifi_key, ap_key);

    wNetConfig.wifi_mode = SOFT_AP;
    wNetConfig.dhcp_mode = DHCP_Server;
    wNetConfig.wifi_retry_interval = 100;
    os_strcpy((char *)wNetConfig.local_ip_addr, "192.168.10.1");
    os_strcpy((char *)wNetConfig.net_mask, "255.255.255.0");
    os_strcpy((char *)wNetConfig.dnsServer_ip_addr, "192.168.10.1");

    bk_printf("SSID:%s , KEY:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);
    bk_wlan_start(&wNetConfig);
}

/* wifi scan thread */
void wifi_softap_thread( beken_thread_arg_t arg )
{
    (void) arg;
    char *ap_ssid = "Beken_7231";
    char *ap_key = "12345678";

    wifi_softap_app_init(ap_ssid, ap_key);

    rtos_delete_thread( NULL );
}

int demo_start( void )
{
    bk_err_t err = kNoErr;

    err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "wifi_softap",
                              (beken_thread_function_t)wifi_softap_thread,
                              0x800,
                              (beken_thread_arg_t)0 );

    return err;
}

#endif


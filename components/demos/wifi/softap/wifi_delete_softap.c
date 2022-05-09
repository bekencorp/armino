/**
 ******************************************************************************
 * @file    wifi_delete_softap.h
 * @author
 * @version V1.0.0
 * @date
 * @brief   delete a built sotfap demo
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
#include <common/bk_include.h>
#include "wifi_delete_softap.h"

#if WIFI_DELETE_SOFTAP_DEMO
#include <common/bk_include.h>
#include <os/os.h>
#include <common/bk_err.h>
#include "wlan_ui_pub.h"
#include "uart_pub.h"
#include <os/mem.h>
#include "rw_pub.h"
#include "param_config.h"
#include <os/str.h>

struct demo_AP_cfg
{
    Demo_WF_AP_AUTH_MODE_E SECURITY;
    char ssid[32 + 1];
    char s_len;
    char passwd[64 + 1];
    char p_len;
    char chan;
};

int target_wifi_ap_start(IN CONST struct demo_AP_cfg *cfg)
{
    int ret = 0;
    network_InitTypeDef_ap_st wNetConfig;

    os_memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_ap_st));

    switch ( cfg-> SECURITY)
    {
    case DWF_OPEN :
        wNetConfig.security = SECURITY_TYPE_NONE;
        break;
    case DWF_WEP :
        wNetConfig.security = SECURITY_TYPE_WEP;
        break;
    case DWF_WPA_PSK :
        wNetConfig.security = SECURITY_TYPE_WPA2_TKIP;
        break;
    case DWF_WPA2_PSK :
        wNetConfig.security = SECURITY_TYPE_WPA2_AES;
        break;
    case DWF_WPA_WPA2_PSK:
        wNetConfig.security = SECURITY_TYPE_WPA2_MIXED;
        break;
    default:
        wNetConfig.security = SECURITY_TYPE_NONE;
        break;
    }

    wNetConfig.channel = cfg->chan;
    wNetConfig.dhcp_mode = DHCP_Server;

    os_memcpy((char *)wNetConfig.wifi_ssid, cfg->ssid, cfg->s_len);
    os_memcpy((char *)wNetConfig.wifi_key, cfg->passwd, cfg->p_len);

    os_strcpy((char *)wNetConfig.local_ip_addr, "192.168.10.1");
    os_strcpy((char *)wNetConfig.net_mask, "255.255.255.0");
    os_strcpy((char *)wNetConfig.gateway_ip_addr, "192.168.10.1");
    os_strcpy((char *)wNetConfig.dnsServer_ip_addr, "192.168.10.1");

    os_printf("ssid:%s	key:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);
    bk_wlan_start_ap_adv(&wNetConfig);

    return ret;
}

void wifi_create_softap_app_init(char *ap_ssid, char *ap_key)
{
    struct demo_AP_cfg cfg;

    os_memset(&cfg, 0, sizeof(struct demo_AP_cfg));
    cfg.s_len = os_strlen(ap_ssid);
    cfg.p_len = os_strlen(ap_key);
    if((cfg.s_len <= 32) && (cfg.p_len <= 64))
    {
        os_strcpy((char *)cfg.ssid, ap_ssid);
        os_strcpy((char *)cfg.passwd, ap_key);
        cfg.chan = 5;
        cfg.SECURITY = DWF_WPA_WPA2_PSK;

        target_wifi_ap_start(&cfg);
    }
    else
    {
        os_printf("SSID Or Password error\r\n");
    }
}

int wifi_delete_ap_stop(void)
{
    bk_wlan_stop(SOFT_AP);
    return 0;
}

void wifi_delete_softap_thread( beken_thread_arg_t arg )
{
    (void) arg;
    unsigned int dly = 60000 * 1; //mS
    char *ap_ssid = "Beken_7231_dlt";
    char *ap_key = "12345678";

    wifi_create_softap_app_init(ap_ssid, ap_key);

    os_printf("delay %d mS kill AP!\r\n", dly);
    rtos_delay_milliseconds((TickType_t)dly);

    os_printf("stop AP!\r\n");
    wifi_delete_ap_stop();

    rtos_delete_thread( NULL );
}

int demo_start( void )
{
    bk_err_t err = kNoErr;

    err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "wifi_delap",
                              (beken_thread_function_t)wifi_delete_softap_thread,
                              0x800,
                              (beken_thread_arg_t)0 );

    return err;
}

#endif


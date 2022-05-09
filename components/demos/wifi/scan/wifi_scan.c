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
#include <common/bk_include.h>
#include "wifi_scan.h"

#if WIFI_SCAN_DEMO
#include <common/bk_include.h>
#include <os/os.h>
#include <common/bk_err.h>
#include "components/event.h"
#include "wlan_ui_pub.h"
#include "uart_pub.h"
#include <os/mem.h>
#include "rw_pub.h"
#include "param_config.h"

static beken_semaphore_t scan_handle = NULL;

static int scan_ap_cb(void *arg, event_module_t event_module,
					  int event_id, void *event_data)
{
    if(scan_handle)
    {
        rtos_set_semaphore( &scan_handle );
    }

	return BK_OK;
}

void show_scan_ap_result(void)
{
    struct sta_scan_res *scan_rst_table;
    char scan_rst_ap_num = 0;       /**< The number of access points found in scanning. */
    int i;

    scan_rst_ap_num = bk_wlan_get_scan_ap_result_numbers();
    if(scan_rst_ap_num == 0)
    {
        os_printf("NULL AP\r\n");
        return;
    }

    scan_rst_table = (struct sta_scan_res *)os_malloc(sizeof(struct sta_scan_res) * scan_rst_ap_num);
    if(scan_rst_table == NULL)
    {
        os_printf("scan_rst_table malloc failed!\r\n");
        return;
    }

    bk_wlan_get_scan_ap_result(scan_rst_table, scan_rst_ap_num);

    os_printf("\r\nscan ap count:%d\r\n", scan_rst_ap_num);
    for( i = 0; i < scan_rst_ap_num; i++ )
    {
        os_printf("%d: %s, ", i + 1, scan_rst_table[i].ssid);
        os_printf("Channal:%d, ", scan_rst_table[i].channel);
        switch(scan_rst_table[i].security)
        {
        case BK_SECURITY_TYPE_NONE:
            os_printf(" %s, ", "Open");
            break;
        case BK_SECURITY_TYPE_WEP:
            os_printf(" %s, ", "CIPHER_WEP");
            break;
        case BK_SECURITY_TYPE_WPA_TKIP:
            os_printf(" %s, ", "CIPHER_WPA_TKIP");
            break;
        case BK_SECURITY_TYPE_WPA_AES:
            os_printf(" %s, ", "CIPHER_WPA_AES");
            break;
        case BK_SECURITY_TYPE_WPA2_TKIP:
            os_printf(" %s, ", "CIPHER_WPA2_TKIP");
            break;
        case BK_SECURITY_TYPE_WPA2_AES:
            os_printf(" %s, ", "CIPHER_WPA2_AES");
            break;
        case BK_SECURITY_TYPE_WPA2_MIXED:
            os_printf(" %s, ", "CIPHER_WPA2_MIXED");
            break;
        case BK_SECURITY_TYPE_AUTO:
            os_printf(" %s, ", "CIPHER_AUTO");
            break;
        default:
            os_printf(" %s(%d), ", "unknown", scan_rst_table[i].security);
            break;
        }
        os_printf("RSSI=%d \r\n", scan_rst_table[i].level);
    }

    if( scan_rst_table != NULL )
    {
        os_free(scan_rst_table);
        scan_rst_table = NULL;
    }
}

void wifi_scan_thread( beken_thread_arg_t arg )
{
    (void) arg;
    bk_err_t err = kNoErr;

    os_printf("start scanning..............\r\n");

    err = rtos_init_semaphore( &scan_handle, 1 );
    if(err == kNoErr)
    {
		bk_event_register_cb(EVENT_MOD_WIFI, EVENT_WIFI_SCAN_DONE,
								   scan_ap_cb, NULL);
		bk_wifi_scan_start(NULL);

        err = rtos_get_semaphore(&scan_handle, BEKEN_WAIT_FOREVER);
        if(err == kNoErr)
        {
            show_scan_ap_result();
        }

        if(scan_handle)
        {
            rtos_deinit_semaphore(&scan_handle);
        }
    }
    else
    {
        os_printf("scan_handle init failed!\r\n");
    }

    rtos_delete_thread( NULL );
}

int demo_start( void )
{
    bk_err_t err = kNoErr;

    os_printf("\r\n\r\nwifi scan demo............\r\n\r\n" );
    err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "wifiscan",
                              (beken_thread_function_t)wifi_scan_thread,
                              0x800,
                              (beken_thread_arg_t)0 );

    return err;
}


#endif /**WIFI_SCAN_DEMO**/


#include <common/bk_include.h>
#include "video_demo_config.h"

#if (CONFIG_NET_WORK_VIDEO_TRANSFER == 1) && (APP_VIDEO_TRANSFER_MODE & VIDEO_TRANSFER_P2P_MODE)
#include <os/str.h>
#include "uart_pub.h"
#include <os/mem.h>

#include <os/os.h>
#include <common/bk_err.h>

#include "wlan_ui_pub.h"

#include <lwip/inet.h>
#include "BkDriverFlash.h"

#include "video_transfer_tcp.h"
#include "video_transfer_udp.h"
#include "video_demo_pub.h"
#include "video_upd_spd.h"
#ifdef CONFIG_COMPONENTS_P2P
//#include "rw_tx_buffering.h"
//#include "rw_ieee80211.h"
#include "net.h"
#include "sys.h"
#include "bk_wifi.h"
#endif

#define APP_DEMO_P2P_DEBUG        1
#if APP_DEMO_P2P_DEBUG
#define APP_DEMO_P2P_PRT      fatal_prf
#define APP_DEMO_P2P_WARN     fatal_prf
#define APP_DEMO_P2P_FATAL    fatal_prf
#else
#define APP_DEMO_P2P_PRT      null_prf
#define APP_DEMO_P2P_WARN     null_prf
#define APP_DEMO_P2P_FATAL    null_prf
#endif

#define APP_DEMO_P2P_QITEM_COUNT           4
#define APP_DEMO_P2P_TIMER_INTVAL          500  // ms

typedef enum
{
    APS_WIFI_DISCONECTED          = 0,
    APS_WIFI_CONECTED,
    APS_APP_DISCONECTED,
    APS_APP_CONECTED,
} DEMO_P2P_STATUS;

typedef struct app_demo_p2p_st
{
    beken_thread_t thread_hdl;
    beken_queue_t msg_que;
    u32 status;
    char *wifi_ssid;
    char *wifi_key;
} APP_DEMO_P2P_ST, *APP_DEMO_P2P_PTR;

typedef struct head_param
{
    u32 type;
    u32 len;
} head_param_t;

APP_DEMO_P2P_PTR g_demo_p2p = NULL;

extern void bk_wlan_status_register_cb(FUNC_1PARAM_PTR cb);

uint32 app_demo_p2p_alloc_buffer(void)
{
    if (g_demo_p2p == NULL)
    {
        g_demo_p2p = os_malloc(sizeof(APP_DEMO_P2P_ST));
        if (!g_demo_p2p)
        {
            APP_DEMO_P2P_FATAL("app_demo_sta_init malloc failed\r\n");
            return 0;
        }
        os_memset(g_demo_p2p, 0, sizeof(APP_DEMO_P2P_ST));
    }

    if (g_demo_p2p->wifi_ssid == NULL)
    {
        g_demo_p2p->wifi_ssid = os_malloc(sizeof(char) * 33);
        if (!g_demo_p2p->wifi_ssid)
        {
            APP_DEMO_P2P_FATAL("ssid fail\r\n");
            os_free(g_demo_p2p);
            g_demo_p2p = NULL;
            return 0;
        }
    }

    if (g_demo_p2p->wifi_key == NULL)
    {
        g_demo_p2p->wifi_key = os_malloc(sizeof(char) * 64);
        if (!g_demo_p2p->wifi_key)
        {
            APP_DEMO_P2P_FATAL("key fail\r\n");
            os_free(g_demo_p2p->wifi_ssid);
            g_demo_p2p->wifi_ssid = NULL;
            os_free(g_demo_p2p);
            g_demo_p2p = NULL;
            return 0;
        }
    }

    return 1;
}

void app_demo_p2p_free_buffer(void)
{
    if (g_demo_p2p)
    {
        if (g_demo_p2p->wifi_key)
        {
            os_free(g_demo_p2p->wifi_key);
            g_demo_p2p->wifi_key = NULL;
        }

        if (g_demo_p2p->wifi_ssid)
        {
            os_free(g_demo_p2p->wifi_ssid);
            g_demo_p2p->wifi_ssid = NULL;
        }

        os_free(g_demo_p2p);
        g_demo_p2p = NULL;

        APP_DEMO_P2P_FATAL("p2p_free_buffer done\r\n");
    }
}

void app_demo_p2p_send_msg(u32 new_msg, u32 new_data)
{
    int ret;
    DRONE_MSG_T msg;

    if (g_demo_p2p && g_demo_p2p->msg_que)
    {
        msg.dmsg = new_msg;
	 msg.data = new_data;

        ret = rtos_push_to_queue(&g_demo_p2p->msg_que, &msg, BEKEN_NO_WAIT);
        if (kNoErr != ret)
        {
            os_printf("app_demo_sta_send_msg failed\r\n");
        }
    }
}

static void app_demo_p2p_rw_event_func(void *new_evt)
{
    wifi_linkstate_reason_t info = *((wifi_linkstate_reason_t *)new_evt);

    if (info.state == WIFI_LINKSTATE_STA_CONNECTED)
    {
        APP_DEMO_P2P_PRT("WIFI_LINKSTATE_STA_CONNECTED\r\n");
        app_demo_p2p_send_msg(DAP_WIFI_CONECTED, 0);
    }
    else if (info.state == WIFI_LINKSTATE_STA_DISCONNECTED)
    {
        APP_DEMO_P2P_PRT("WIFI_LINKSTATE_STA_DISCONNECTED\r\n");
        app_demo_p2p_send_msg(DAP_WIFI_DISCONECTED, 0);
    }
	else if (evt_type == RW_EVT_AP_CONNECTED)
    {
        APP_DEMO_P2P_PRT("RW_EVT_AP_CONNECTED\r\n");
        app_demo_p2p_send_msg(DAP_WIFI_CONECTED, 1);
    }
    else if (evt_type == RW_EVT_AP_DISCONNECTED)
    {
        APP_DEMO_P2P_PRT("RW_EVT_AP_DISCONNECTED\r\n");
        app_demo_p2p_send_msg(DAP_WIFI_DISCONECTED, 1);
    }
}
extern void ip_address_set(int iface, int dhcp, char *ip, char *mask, char*gw, char*dns);
static int app_demo_p2p_setup(void)
{
    network_InitTypeDef_st wNetConfig;
    int len;
    os_memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));

    len = os_strlen(g_demo_p2p->wifi_ssid);
    if (32 < len)
    {
        APP_DEMO_P2P_PRT("ssid name more than 32 Bytes\r\n");
        return 0;
    }
    os_strcpy((char *)wNetConfig.wifi_ssid, g_demo_p2p->wifi_ssid);

    len = os_strlen(g_demo_p2p->wifi_key);
    if (64 < len)
    {
        APP_DEMO_P2P_PRT("key more than 64 Bytes\r\n");
        return 0;
    }
    os_strcpy((char *)wNetConfig.wifi_key, g_demo_p2p->wifi_key);

    os_free(g_demo_p2p->wifi_key);
    os_free(g_demo_p2p->wifi_ssid);

    g_demo_p2p->wifi_key = NULL;
    g_demo_p2p->wifi_ssid = NULL;

    wNetConfig.wifi_mode = BK_P2P;
    wNetConfig.dhcp_mode = DHCP_CLIENT;
    wNetConfig.wifi_retry_interval = 100;

    APP_DEMO_P2P_PRT("ssid:%s key:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);
    bk_wlan_start(&wNetConfig);
    //ip_address_set(BK_STATION, 0, "192.168.49.100", "255.255.255.0", "192.168.49.1", "192.168.49.1");
    return 1;
}

static void app_demo_p2p_main(beken_thread_arg_t data)
{
    int err;
    u32 status;

    g_demo_p2p->status = APS_WIFI_DISCONECTED;

    bk_wlan_status_register_cb(app_demo_p2p_rw_event_func);

    if (app_demo_p2p_setup() == 0)
    {
        goto app_demo_sta_exit;
    }

    while (1)
    {
        DRONE_MSG_T msg;
        status = g_demo_p2p->status;
        status = status;

        err = rtos_pop_from_queue(&g_demo_p2p->msg_que, &msg, BEKEN_WAIT_FOREVER);
        if (kNoErr == err)
        {
            switch (msg.dmsg)
            {
            case DAP_WIFI_DISCONECTED:
                if (g_demo_p2p->status != APS_WIFI_DISCONECTED)
                {
                    g_demo_p2p->status = APS_WIFI_DISCONECTED;
                    //app_led_send_msg(LED_DISCONNECT);
                    #if APP_DEMO_CFG_USE_UDP
                    app_demo_udp_deinit();
                    #endif

                    #if APP_DEMO_CFG_USE_TCP
                    app_demo_tcp_deinit();
                    #endif

                    #if APP_DEMO_CFG_USE_UDP_SDP
                    vudp_sdp_stop();
                    #endif

#ifdef CONFIG_COMPONENTS_P2P
		wlan_hw_reinit();
		if (msg.data == 1) {
			uap_ip_down();
			wlan_p2p_cancel();
			wlan_p2p_find();
		} else if (msg.data == 0) {
			sta_ip_down();
			sys_msleep(2000);
			wlan_p2p_find();
		}
#endif
                    APP_DEMO_P2P_PRT("wifi disconnected!\r\n");
                }
                break;

            case DAP_WIFI_CONECTED:
                if (g_demo_p2p->status == APS_WIFI_DISCONECTED)
                {
                    g_demo_p2p->status = APS_WIFI_CONECTED;
                    //app_led_send_msg(LED_CONNECT);
                    #if APP_DEMO_CFG_USE_UDP
                    app_demo_udp_init();
                    #endif

                    #if APP_DEMO_CFG_USE_TCP
                    app_demo_tcp_init();
                    #endif

                    #if APP_DEMO_CFG_USE_UDP_SDP
                    vudp_sdp_start();
                    #endif

                    APP_DEMO_P2P_PRT("wifi connected!\r\n");
                }
                break;

            case DAP_APP_CONECTED:
                APP_DEMO_P2P_PRT("app connected!\r\n");
                g_demo_p2p->status = APS_APP_CONECTED;
                break;

            case DAP_APP_DISCONECTED:
                APP_DEMO_P2P_PRT("app disconnected!\r\n");
                g_demo_p2p->status = APS_APP_DISCONECTED;
                break;

            case DAP_EXIT:
                goto app_demo_sta_exit;
                break;


            default:
                break;
            }
        }
    }

app_demo_sta_exit:

    rtos_deinit_queue(&g_demo_p2p->msg_que);

    app_demo_p2p_free_buffer();

    rtos_delete_thread(NULL);
}

void app_demo_p2p_start(char *oob_ssid, char *connect_key)
{
    int ret;

    //APP_DEMO_P2P_PRT("app_demo_p2p_init ssid:%s, key:%s\r\n");
    #if 1
    if (!g_demo_p2p)
    {
        if (app_demo_p2p_alloc_buffer() == 0)
        {
            return;
        }

        os_strcpy((char *)g_demo_p2p->wifi_ssid, oob_ssid);

        if (connect_key)
        {
            os_strcpy((char *)g_demo_p2p->wifi_key, connect_key);
        }
        else
        {
            os_strcpy((char *)g_demo_p2p->wifi_key, " ");
        }

        ret = rtos_init_queue(&g_demo_p2p->msg_que,
                              "app_demo_p2p_queue",
                              sizeof(DRONE_MSG_T),
                              APP_DEMO_P2P_QITEM_COUNT);
        if (kNoErr != ret)
        {
            APP_DEMO_P2P_FATAL("create queue failed\r\n");
            app_demo_p2p_free_buffer();
            return;
        }

        ret = rtos_create_thread(&g_demo_p2p->thread_hdl,
                                 BEKEN_DEFAULT_WORKER_PRIORITY,
                                 "app_demo_p2p",
                                 (beken_thread_function_t)app_demo_p2p_main,
                                 4096,
                                 (beken_thread_arg_t)NULL);
        if (ret != kNoErr)
        {
            APP_DEMO_P2P_FATAL("create thread: %d\r\n", ret);
            rtos_deinit_queue(&g_demo_p2p->msg_que);
            app_demo_p2p_free_buffer();
            return;
        }

    }
    #endif
}

void app_demo_p2p_exit(void)
{
    APP_DEMO_P2P_PRT("app_demo_sta_exit\r\n");

    if (g_demo_p2p)
    {
        app_demo_p2p_send_msg(DAP_EXIT, 0);

        while (g_demo_p2p)
        {
            rtos_delay_milliseconds(200);
        }
    }
    APP_DEMO_P2P_PRT("done!\r\n");
}
#endif  // (CONFIG_NET_WORK_VIDEO_TRANSFER == 1) && (APP_VIDEO_TRANSFER_MODE & VIDEO_TRANSFER_P2P_MODE)


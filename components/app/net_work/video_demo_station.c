#include <common/bk_include.h>
#include "video_demo_config.h"

#if (CONFIG_NET_WORK_VIDEO_TRANSFER == 1) && (APP_VIDEO_TRANSFER_MODE & VIDEO_TRANSFER_STA_MODE)
#include <os/str.h>
#include "bk_uart.h"
#include <os/mem.h>

#include <os/os.h>
#include <common/bk_kernel_err.h>

#include "wlan_ui_pub.h"

#include <lwip/inet.h>
#if CONFIG_FLASH_ORIGIN_API
#include "BkDriverFlash.h"
#endif
#include "video_transfer_tcp.h"
#include "video_transfer_udp.h"
#include "video_demo_pub.h"
#include "video_upd_spd.h"

#define APP_DEMO_STA_DEBUG        1
#if APP_DEMO_STA_DEBUG
#define APP_DEMO_STA_PRT      warning_prf
#define APP_DEMO_STA_WARN     warning_prf
#define APP_DEMO_STA_FATAL    fatal_prf
#else
#define APP_DEMO_STA_PRT      null_prf
#define APP_DEMO_STA_WARN     null_prf
#define APP_DEMO_STA_FATAL    null_prf
#endif

#define APP_DEMO_STA_QITEM_COUNT           4
#define APP_DEMO_STA_TIMER_INTVAL          500  // ms

typedef enum
{
    APS_WIFI_DISCONECTED          = 0,
    APS_WIFI_CONECTED,
    APS_APP_DISCONECTED,
    APS_APP_CONECTED,
} DEMO_STA_STATUS;

typedef struct app_demo_sta_st
{
    beken_thread_t thread_hdl;
    beken_queue_t msg_que;
    u32 status;
    char *wifi_ssid;
    char *wifi_key;
} APP_DEMO_STA_ST, *APP_DEMO_STA_PTR;

typedef struct head_param
{
    u32 type;
    u32 len;
} head_param_t;

APP_DEMO_STA_PTR g_demo_sta = NULL;

extern void bk_wlan_status_register_cb(FUNC_1PARAM_PTR cb);

uint32 app_demo_sta_alloc_buffer(void)
{
    if (g_demo_sta == NULL)
    {
        g_demo_sta = os_malloc(sizeof(APP_DEMO_STA_ST));
        if (!g_demo_sta)
        {
            APP_DEMO_STA_FATAL("app_demo_sta_init malloc failed\r\n");
            return 0;
        }
        os_memset(g_demo_sta, 0, sizeof(APP_DEMO_STA_ST));
    }

    if (g_demo_sta->wifi_ssid == NULL)
    {
        g_demo_sta->wifi_ssid = os_malloc(sizeof(char) * 33);
        if (!g_demo_sta->wifi_ssid)
        {
            APP_DEMO_STA_FATAL("ssid fail\r\n");
            os_free(g_demo_sta);
            g_demo_sta = NULL;
            return 0;
        }
    }

    if (g_demo_sta->wifi_key == NULL)
    {
        g_demo_sta->wifi_key = os_malloc(sizeof(char) * 64);
        if (!g_demo_sta->wifi_key)
        {
            APP_DEMO_STA_FATAL("key fail\r\n");
            os_free(g_demo_sta->wifi_ssid);
            g_demo_sta->wifi_ssid = NULL;
            os_free(g_demo_sta);
            g_demo_sta = NULL;
            return 0;
        }
    }

    return 1;
}

void app_demo_sta_free_buffer(void)
{
    if (g_demo_sta)
    {
        if (g_demo_sta->wifi_key)
        {
            os_free(g_demo_sta->wifi_key);
            g_demo_sta->wifi_key = NULL;
        }

        if (g_demo_sta->wifi_ssid)
        {
            os_free(g_demo_sta->wifi_ssid);
            g_demo_sta->wifi_ssid = NULL;
        }

        os_free(g_demo_sta);
        g_demo_sta = NULL;

        APP_DEMO_STA_FATAL("sta_free_buffer done\r\n");
    }
}

void app_demo_sta_send_msg(u32 new_msg, u32 new_data)
{
    bk_err_t ret;
    DRONE_MSG_T msg;

    if (g_demo_sta && g_demo_sta->msg_que)
    {
        msg.dmsg = new_msg;

        ret = rtos_push_to_queue(&g_demo_sta->msg_que, &msg, BEKEN_NO_WAIT);
        if (kNoErr != ret)
        {
            os_printf("app_demo_sta_send_msg failed\r\n");
        }
    }
}

static void app_demo_sta_rw_event_func(void *new_evt)
{
    wifi_linkstate_reason_t info = *((wifi_linkstate_reason_t *)new_evt);

    if (info.state == WIFI_LINKSTATE_STA_GOT_IP)
    {
        APP_DEMO_STA_PRT("WIFI_LINKSTATE_STA_GOT_IP\r\n");
        //if(g_demo_sta->status == DS_WIFI_DISCONECTED)
        app_demo_sta_send_msg(DAP_WIFI_CONECTED, 0);
    }
    else if (info.state == WIFI_LINKSTATE_STA_DISCONNECTED)
    {
        APP_DEMO_STA_PRT("WIFI_LINKSTATE_STA_DISCONNECTED\r\n");
        //if(g_demo_sta->status != DS_WIFI_DISCONECTED)
        app_demo_sta_send_msg(DAP_WIFI_DISCONECTED, 0);
    }
}

static int app_demo_sta_setup(void)
{
    wifi_sta_config_t sta_config = {0};
    int len;

    len = os_strlen(g_demo_sta->wifi_ssid);
    if (32 < len)
    {
        APP_DEMO_STA_PRT("ssid name more than 32 Bytes\r\n");
        return 0;
    }
    os_strcpy(sta_config.ssid, g_demo_sta->wifi_ssid);

    len = os_strlen(g_demo_sta->wifi_key);
    if (64 < len)
    {
        APP_DEMO_STA_PRT("key more than 64 Bytes\r\n");
        return 0;
    }
    os_strcpy(sta_config.password, g_demo_sta->wifi_key);

    os_free(g_demo_sta->wifi_key);
    os_free(g_demo_sta->wifi_ssid);

    g_demo_sta->wifi_key = NULL;
    g_demo_sta->wifi_ssid = NULL;

    APP_DEMO_STA_PRT("ssid:%s key:%s\r\n", sta_config.ssid, sta_config.password);
    BK_LOG_ON_ERR(bk_wifi_sta_set_config(&sta_config));
    BK_LOG_ON_ERR(bk_wifi_sta_start());

    return 1;
}

static void app_demo_sta_main(beken_thread_arg_t data)
{
    bk_err_t err;
    u32 status;

    g_demo_sta->status = APS_WIFI_DISCONECTED;

    bk_wlan_status_register_cb(app_demo_sta_rw_event_func);

    if (app_demo_sta_setup() == 0)
    {
        goto app_demo_sta_exit;
    }

    while (1)
    {
        DRONE_MSG_T msg;
        status = g_demo_sta->status;
        status = status;

        err = rtos_pop_from_queue(&g_demo_sta->msg_que, &msg, BEKEN_WAIT_FOREVER);
        if (kNoErr == err)
        {
            switch (msg.dmsg)
            {
            case DAP_WIFI_DISCONECTED:
                if (g_demo_sta->status != APS_WIFI_DISCONECTED)
                {
                    g_demo_sta->status = APS_WIFI_DISCONECTED;
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
                    APP_DEMO_STA_PRT("wifi disconnected!\r\n");
                }
                break;

            case DAP_WIFI_CONECTED:
                if (g_demo_sta->status == APS_WIFI_DISCONECTED)
                {
                    g_demo_sta->status = APS_WIFI_CONECTED;
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
                    APP_DEMO_STA_PRT("wifi connected!\r\n");
                }
                break;

            case DAP_APP_CONECTED:
                APP_DEMO_STA_PRT("app connected!\r\n");
                g_demo_sta->status = APS_APP_CONECTED;
                break;

            case DAP_APP_DISCONECTED:
                APP_DEMO_STA_PRT("app disconnected!\r\n");
                g_demo_sta->status = APS_APP_DISCONECTED;
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

    rtos_deinit_queue(&g_demo_sta->msg_que);

    app_demo_sta_free_buffer();

    rtos_delete_thread(NULL);
}

void app_demo_sta_start(char *oob_ssid, char *connect_key)
{
    int ret;

    APP_DEMO_STA_PRT("app_demo_sta_init ssid:%s, key:%s\r\n");
    #if 1
    if (!g_demo_sta)
    {
        if (app_demo_sta_alloc_buffer() == 0)
        {
            return;
        }

        os_strcpy((char *)g_demo_sta->wifi_ssid, oob_ssid);

        if (connect_key)
        {
            os_strcpy((char *)g_demo_sta->wifi_key, connect_key);
        }
        else
        {
            os_strcpy((char *)g_demo_sta->wifi_key, " ");
        }

        ret = rtos_init_queue(&g_demo_sta->msg_que,
                              "app_demo_sta_queue",
                              sizeof(DRONE_MSG_T),
                              APP_DEMO_STA_QITEM_COUNT);
        if (kNoErr != ret)
        {
            APP_DEMO_STA_FATAL("create queue failed\r\n");
            app_demo_sta_free_buffer();
            return;
        }

        ret = rtos_create_thread(&g_demo_sta->thread_hdl,
                                 BEKEN_DEFAULT_WORKER_PRIORITY,
                                 "app_demo_sta",
                                 (beken_thread_function_t)app_demo_sta_main,
                                 4096,
                                 (beken_thread_arg_t)NULL);
        if (ret != kNoErr)
        {
            APP_DEMO_STA_FATAL("create thread: %d\r\n", ret);
            rtos_deinit_queue(&g_demo_sta->msg_que);
            app_demo_sta_free_buffer();
            return;
        }

    }
    #endif
}

void app_demo_sta_exit(void)
{
    APP_DEMO_STA_PRT("app_demo_sta_exit\r\n");

    if (g_demo_sta)
    {
        app_demo_sta_send_msg(DAP_EXIT, 0);

        while (g_demo_sta)
        {
            rtos_delay_milliseconds(200);
        }
    }
    APP_DEMO_STA_PRT("done!\r\n");
}
#endif  // (CONFIG_NET_WORK_VIDEO_TRANSFER == 1) && (APP_VIDEO_TRANSFER_MODE & VIDEO_TRANSFER_STA_MODE)


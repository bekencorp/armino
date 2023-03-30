#include <common/bk_include.h>

#if (CONFIG_APP_DEMO_VIDEO_TRANSFER)
#include "video_transfer_config.h"

#if (APP_DEMO_CFG_USE_UDP)
//#include "FreeRTOS.h"
//#include "task.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>
#ifdef CONFIG_RTT
    #include <sys/socket.h>
#endif
#include "lwip/sockets.h"

#include "video_transfer_cs2_p2p.h"

#include "bk_uart.h"
#include <os/mem.h>
#include <components/video_transfer.h>
#if APP_DEMO_EN_VOICE_TRANSFER
    #include <components/aud_intf.h>
    #include <components/aud_intf_types.h>
#endif  //APP_DEMO_EN_VOICE_TRANSFER
#include "video_demo_pub.h"

#include "cs2_p2p_main.h"


#define APP_DEMO_CS2_P2P_DEBUG              1
#if APP_DEMO_CS2_P2P_DEBUG
    #define APP_DEMO_CS2_P2P_PRT                warning_prf
    #define APP_DEMO_CS2_P2P_WARN               warning_prf
    #define APP_DEMO_CS2_P2P_FATAL              fatal_prf
#else
    #define APP_DEMO_CS2_P2P_PRT                null_prf
    #define APP_DEMO_CS2_P2P_WARN               null_prf
    #define APP_DEMO_CS2_P2P_FATAL              null_prf
#endif


#define APP_DEMO_CS2_P2P_RCV_BUF_LEN        1472
#define APP_DEMO_CS2_P2P_SOCKET_TIMEOUT     100  // ms

media_camera_device_t *get_camera_device(void);

extern void app_demo_softap_send_msg(u32 new_msg, u32 new_data);
extern int32_t cs2_p2p_send_raw(uint8_t channel, uint8_t *buff, uint32_t size);

static uint32_t g_pkt_send_fail = 0;
static uint32_t g_pkt_send = 0;
static int app_demo_cs2_p2p_img_fd = -1;
static volatile int app_demo_cs2_p2p_romote_connected = 0;
static volatile uint8_t app_demo_cs2_p2p_run = 0;
static beken_thread_t app_demo_cs2_p2p_hdl = NULL;
static struct sockaddr_in *app_demo_remote = NULL;

#if APP_DEMO_EN_VOICE_TRANSFER
    static volatile int app_demo_cs2_p2p_voice_romote_connected = 0;
    static struct sockaddr_in *app_demo_cs2_p2p_voice_remote = NULL;
    static int app_demo_cs2_p2p_voice_fd = -1;
    static aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
    static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
    static aud_intf_voc_setup_t aud_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();
#endif  //APP_DEMO_EN_VOICE_TRANSFER

typedef struct tvideo_hdr_st
{
    UINT8 id;
    UINT8 is_eof;
    UINT8 pkt_cnt;
    UINT8 size;
#if SUPPORT_TIANZHIHENG_DRONE
    UINT32 unused;
#endif
} HDR_ST, *HDR_PTR;

/** define audio transfer cmd */
typedef enum
{
    VIDEO_TRAS_AUDIO_CLOSE = 0,
    VIDEO_TRAS_AUDIO_OPEN = 1,
} video_transfer_cmd_t;

enum
{
    CS2_P2P_CHANNEL_IMG,
    CS2_P2P_CHANNEL_VOICE,
    CS2_P2P_CHANNEL_CMD,
};

//typedef struct
//{
//    beken_thread_t tid;
//    int (*func)(char *pDIDString, char *pAPILicense, char *pInitString, char *pRepeat, char *pWakeupKey,
//                int32_t (*recv_callback)(uint8_t channel, uint8_t *buff, uint32_t len),
//                uint8_t *is_run);
//
//    char *did_dskey;
//    char *apilicense_crckey;
//    char *initstring_p2pkey;
//
//} middle_layer_task_arg_t;

static void app_demo_add_pkt_header(video_packet_t *param)
{
    HDR_PTR elem_tvhdr = (HDR_PTR)param->ptk_ptr;

    elem_tvhdr->id = (UINT8)param->frame_id;
    elem_tvhdr->is_eof = param->is_eof;
    elem_tvhdr->pkt_cnt = param->frame_len;
    elem_tvhdr->size = 0;

#if SUPPORT_TIANZHIHENG_DRONE
    elem_tvhdr->unused = 0;
#endif
}

#if APP_DEMO_EN_VOICE_TRANSFER
static int app_demo_cs2_p2p_voice_send_packet(UINT8 *data, UINT32 len)
{
    int send_byte = 0;

    if (!app_demo_cs2_p2p_voice_romote_connected)
    {
        return 0;
    }

    //    send_byte = sendto(app_demo_cs2_p2p_voice_fd, data, len, MSG_DONTWAIT | MSG_MORE,
    //                       (struct sockaddr *)app_demo_cs2_p2p_voice_remote, sizeof(struct sockaddr_in));

    send_byte = cs2_p2p_send_raw(CS2_P2P_CHANNEL_VOICE, data, len);

    if (send_byte < 0)
    {
        /* err */
        //LWIP_cs2_p2p_PRT("send return fd:%d\r\n", send_byte);
        send_byte = 0;
    }

    return send_byte;
}
#endif //APP_DEMO_EN_VOICE_TRANSFER

static void app_demo_cs2_p2p_handle_cmd_data(UINT8 *data, UINT16 len)
{
    UINT32 param = 0;
    UINT32 cmd = (UINT32)data[0] << 24 | (UINT32)data[1] << 16 | (UINT32)data[2] << 8 | data[3];

    if (len >= 8)
    {
        param = (UINT32)data[4] << 24 | (UINT32)data[5] << 16 | (UINT32)data[6] << 8 | data[7];
    }

    APP_DEMO_CS2_P2P_PRT("doorbell cmd: %08X, param: %d, len: %d\n", cmd, param, len);

    switch (cmd)
    {
#if APP_DEMO_EN_VOICE_TRANSFER

    case VIDEO_TRAS_AUDIO_CLOSE:
        bk_aud_intf_voc_stop();
        bk_aud_intf_voc_deinit();
        aud_work_mode = AUD_INTF_WORK_MODE_NULL;
        bk_aud_intf_set_mode(aud_work_mode);
        bk_aud_intf_drv_deinit();
        break;

    case VIDEO_TRAS_AUDIO_OPEN:
        //aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
        //aud_intf_drv_setup.task_config.priority = 3;
        //aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
        aud_intf_drv_setup.aud_intf_tx_mic_data = app_demo_cs2_p2p_voice_send_packet;
        bk_aud_intf_drv_init(&aud_intf_drv_setup);
        aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
        bk_aud_intf_set_mode(aud_work_mode);

        if (data[9] == 1)
        {
            aud_voc_setup.aec_enable = true;
        }
        else
        {
            aud_voc_setup.aec_enable = false;
        }

        //aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_G711A;
        //aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_PCM;
        //aud_voc_setup.mic_gain = 0x2d;
        //aud_voc_setup.spk_gain = 0x2d;
        aud_voc_setup.spk_mode = AUD_DAC_WORK_MODE_SIGNAL_END;

        //aud_voc_setup.mic_en = AUD_INTF_VOC_MIC_OPEN;
        //aud_voc_setup.spk_en = AUD_INTF_VOC_SPK_OPEN;
        if (data[8] == 1)
        {
            aud_voc_setup.mic_type = AUD_INTF_MIC_TYPE_UAC;
            aud_voc_setup.spk_type = AUD_INTF_SPK_TYPE_UAC;
            //aud_voc_setup.samp_rate = AUD_INTF_VOC_SAMP_RATE_16K;
        }
        else
        {
            aud_voc_setup.mic_type = AUD_INTF_MIC_TYPE_BOARD;
            aud_voc_setup.spk_type = AUD_INTF_SPK_TYPE_BOARD;
        }

        switch (data[10])
        {
        case 8:
            aud_voc_setup.samp_rate = AUD_INTF_VOC_SAMP_RATE_8K;
            break;

        case 16:
            aud_voc_setup.samp_rate = AUD_INTF_VOC_SAMP_RATE_16K;
            break;

        default:
            aud_voc_setup.samp_rate = AUD_INTF_VOC_SAMP_RATE_8K;
            break;
        }

        //aud_voc_setup.aec_cfg.ec_depth = 20;
        //aud_voc_setup.aec_cfg.TxRxThr = 30;
        //aud_voc_setup.aec_cfg.TxRxFlr = 6;
        //aud_voc_setup.aec_cfg.ref_scale = 1;
        //aud_voc_setup.aec_cfg.ns_level = 2;
        //aud_voc_setup.aec_cfg.ns_para = 1;
        bk_aud_intf_voc_init(aud_voc_setup);
        bk_aud_intf_voc_start();
        break;
#endif

    default:
        break;
    }
}


static void app_demo_cs2_p2p_app_connected(void)
{
    app_demo_softap_send_msg(DAP_APP_CONECTED, 0);
}

static void app_demo_cs2_p2p_app_disconnected(void)
{
    app_demo_softap_send_msg(DAP_APP_DISCONECTED, 0);
}

#if CFG_SUPPORT_HTTP_OTA
TV_OTA_ST ota_param =
{
    NULL,
    0,
    0
};
static void app_demo_cs2_p2p_http_ota_handle(char *rev_data)
{
    //if(app_demo_softap_is_ota_doing() == 0)
    //{
    // to do
    //
    //app_demo_softap_send_msg(DAP_START_OTA, (u32)&ota_param);

    //os_memset(&ota_param, 0, sizeof(TV_OTA_ST));
    //}
}
#endif

extern void rwnxl_set_video_transfer_flag(uint32_t video_transfer_flag);

static void app_demo_cs2_p2p_img_receiver(UINT8 *data, UINT32 len, struct sockaddr_in *app_demo_remote)
{
    GLOBAL_INT_DECLARATION();

    if (len < 2)
    {
        return;
    }

    if (data[0] == CMD_IMG_HEADER)
    {
        if (data[1] == CMD_START_IMG)
        {

            //            UINT8 *src_ipaddr = (UINT8 *)&app_demo_remote->sin_addr.s_addr;
            //            APP_DEMO_CS2_P2P_PRT("src_ipaddr: %d.%d.%d.%d\r\n", src_ipaddr[0], src_ipaddr[1],
            //                                 src_ipaddr[2], src_ipaddr[3]);
            APP_DEMO_CS2_P2P_PRT("cs2_p2p connect to new \n");

            GLOBAL_INT_DISABLE();
            app_demo_cs2_p2p_romote_connected = 1;
            GLOBAL_INT_RESTORE();

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
            video_setup_t setup;

            setup.open_type = TVIDEO_OPEN_SCCB;
            setup.send_type = TVIDEO_SND_INTF;
            setup.send_func = app_demo_cs2_p2p_send_packet;
            setup.start_cb = app_demo_cs2_p2p_app_connected;
            setup.end_cb = app_demo_cs2_p2p_app_disconnected;
            setup.device = get_camera_device();

            setup.pkt_header_size = sizeof(HDR_ST);
            setup.add_pkt_header = app_demo_add_pkt_header;

            rwnxl_set_video_transfer_flag(true);

            bk_video_transfer_init(&setup);
#endif
        }
        else if (data[1] == CMD_STOP_IMG)
        {
            APP_DEMO_CS2_P2P_PRT("cs2_p2p close\r\n");

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
            bk_video_transfer_deinit();

            rwnxl_set_video_transfer_flag(false);
#endif

            GLOBAL_INT_DISABLE();
            app_demo_cs2_p2p_romote_connected = 0;
            GLOBAL_INT_RESTORE();
            app_demo_cs2_p2p_app_disconnected();
        }

#if CFG_SUPPORT_HTTP_OTA
        else if (data[1] == CMD_START_OTA)
        {
            app_demo_cs2_p2p_http_ota_handle(&data[2]);
        }

#endif
    }

}

#if APP_DEMO_EN_VOICE_TRANSFER
static void app_demo_cs2_p2p_voice_receiver(UINT8 *data, UINT32 len, struct sockaddr_in *cs2_p2p_remote)
{
    bk_err_t ret = BK_OK;

    if (len > 0)
    {
        app_demo_cs2_p2p_voice_romote_connected = 1;
    }

    ret = bk_aud_intf_write_spk_data(data, len);

    if (ret != BK_OK)
    {
        os_printf("write speaker data fial \r\n", len);
    }

}
#endif // APP_DEMO_EN_VOICE_TRANSFER



static int32_t recv_callback(uint8_t channel, uint8_t *buff, uint32_t len)
{
    //uint32_t i = 0;

    if (len)
    {
        os_printf("%s len %d\n", __func__, len);

#if 1
#pragma message "for enable img debug !!!! remove it !!!!"
        static uint8_t first_flag = 1;

        if(first_flag)
        {
            const uint8_t cmd_img_start[] = {CMD_IMG_HEADER, CMD_START_IMG};
            first_flag = 0;
            app_demo_cs2_p2p_img_receiver((uint8_t *)cmd_img_start, sizeof(cmd_img_start), NULL);
            return 0;
        }
#endif

        //GLOBAL_INT_DIS();
        //GLOBAL_INT_RES();
        switch (channel)
        {
        case CS2_P2P_CHANNEL_CMD:
            app_demo_cs2_p2p_handle_cmd_data(buff, len);
            break;

        case CS2_P2P_CHANNEL_IMG:
            app_demo_cs2_p2p_img_receiver(buff, len, app_demo_remote);
            break;

#if APP_DEMO_EN_VOICE_TRANSFER

        case CS2_P2P_CHANNEL_VOICE:
            app_demo_cs2_p2p_voice_receiver(buff, len, app_demo_cs2_p2p_voice_remote);
            break;
#endif

        default:
            break;
        }
    }

    return 0;
}

#if 0
static void app_demo_cs2_p2p_main(beken_thread_arg_t data)
{
    GLOBAL_INT_DECLARATION();
    int maxfd, cs2_p2p_cmd_fd = -1, ret = 0;
    int rcv_len = 0;
    struct sockaddr_in cmd_remote;
    socklen_t srvaddr_len = 0;
    fd_set watchfd;
    struct timeval timeout;
    u8 *rcv_buf = NULL;

#if 1

    middle_layer_task_arg_t *targ = (typeof(targ))data;

    if (targ && targ->func)
    {
        app_demo_cs2_p2p_run = 1;
        targ->func(
            targ->did_dskey,
            targ->apilicense_crckey,
            targ->initstring_p2pkey,
            NULL, NULL,
            recv_callback, &app_demo_cs2_p2p_run
        );
    }

    (void)maxfd;
    (void)cs2_p2p_cmd_fd;
    (void)rcv_len;
    (void)cmd_remote;
    (void)srvaddr_len;
    (void)watchfd;
    (void)timeout;
    (void)rcv_buf;
    (void)ret;



    if (targ)
    {
        if (targ->did_dskey)
        {
            os_free(targ->did_dskey);
        }

        if (targ->apilicense_crckey)
        {
            os_free(targ->apilicense_crckey);
        }

        if (targ->initstring_p2pkey)
        {
            os_free(targ->initstring_p2pkey);
        }

        os_free(targ);
    }

    goto app_cs2_p2p_exit;

#else





    APP_DEMO_CS2_P2P_FATAL("app_demo_cs2_p2p_main entry\r\n");
    (void)(data);

    rcv_buf = (u8 *) os_malloc((APP_DEMO_CS2_P2P_RCV_BUF_LEN + 1) * sizeof(u8));

    if (!rcv_buf)
    {
        APP_DEMO_CS2_P2P_PRT("cs2_p2p os_malloc failed\r\n");
        goto app_cs2_p2p_exit;
    }

    app_demo_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));

    if (!app_demo_remote)
    {
        APP_DEMO_CS2_P2P_PRT("cs2_p2p os_malloc failed\r\n");
        goto app_cs2_p2p_exit;
    }

    // for data transfer
    app_demo_cs2_p2p_img_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (app_demo_cs2_p2p_img_fd == -1)
    {
        APP_DEMO_CS2_P2P_PRT("socket failed\r\n");
        goto app_cs2_p2p_exit;
    }

    app_demo_remote->sin_family = AF_INET;
    app_demo_remote->sin_port = htons(10086);
    app_demo_remote->sin_addr.s_addr = htonl(INADDR_ANY);

    srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);

    if (bind(app_demo_cs2_p2p_img_fd, (struct sockaddr *)app_demo_remote, srvaddr_len) == -1)
    {
        APP_DEMO_CS2_P2P_PRT("bind failed\r\n");
        goto app_cs2_p2p_exit;
    }

    //  for recv uart cmd
    cs2_p2p_cmd_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (cs2_p2p_cmd_fd == -1)
    {
        APP_DEMO_CS2_P2P_PRT("socket failed\r\n");
        goto app_cs2_p2p_exit;
    }

    cmd_remote.sin_family = AF_INET;
    cmd_remote.sin_port = htons(10087);
    cmd_remote.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(cs2_p2p_cmd_fd, (struct sockaddr *)&cmd_remote, srvaddr_len) == -1)
    {
        APP_DEMO_CS2_P2P_PRT("bind failed\r\n");
        goto app_cs2_p2p_exit;
    }

    maxfd = (cs2_p2p_cmd_fd > app_demo_cs2_p2p_img_fd) ? cs2_p2p_cmd_fd : app_demo_cs2_p2p_img_fd;

    // for voice transfer
#if APP_DEMO_EN_VOICE_TRANSFER
    app_demo_cs2_p2p_voice_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));

    if (!app_demo_cs2_p2p_voice_remote)
    {
        APP_DEMO_CS2_P2P_PRT("cs2_p2p os_malloc failed\r\n");
        goto app_cs2_p2p_exit;
    }

    app_demo_cs2_p2p_voice_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (app_demo_cs2_p2p_voice_fd == -1)
    {
        APP_DEMO_CS2_P2P_PRT("vo socket failed\r\n");
        goto app_cs2_p2p_exit;
    }

    app_demo_cs2_p2p_voice_remote->sin_family = AF_INET;
    app_demo_cs2_p2p_voice_remote->sin_port = htons(10088);
    app_demo_cs2_p2p_voice_remote->sin_addr.s_addr = htonl(INADDR_ANY);

    srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);

    if (bind(app_demo_cs2_p2p_voice_fd, (struct sockaddr *)app_demo_cs2_p2p_voice_remote, srvaddr_len) == -1)
    {
        APP_DEMO_CS2_P2P_PRT("bind failed\r\n");
        goto app_cs2_p2p_exit;
    }

    maxfd = (maxfd > app_demo_cs2_p2p_voice_fd) ? maxfd : app_demo_cs2_p2p_voice_fd;
#endif // APP_DEMO_EN_VOICE_TRANSFER

    timeout.tv_sec = APP_DEMO_CS2_P2P_SOCKET_TIMEOUT / 1000;
    timeout.tv_usec = (APP_DEMO_CS2_P2P_SOCKET_TIMEOUT % 1000) * 1000;

    GLOBAL_INT_DISABLE();
    app_demo_cs2_p2p_romote_connected = 0;
    app_demo_cs2_p2p_run = 1;
#if APP_DEMO_EN_VOICE_TRANSFER
    app_demo_cs2_p2p_voice_romote_connected = 0;
#endif
    GLOBAL_INT_RESTORE();

    while (app_demo_cs2_p2p_run)
    {
        FD_ZERO(&watchfd);
        FD_SET(app_demo_cs2_p2p_img_fd, &watchfd);
        FD_SET(cs2_p2p_cmd_fd, &watchfd);
#if APP_DEMO_EN_VOICE_TRANSFER
        FD_SET(app_demo_cs2_p2p_voice_fd, &watchfd);
#endif

        ret = select(maxfd + 1, &watchfd, NULL, NULL, &timeout);

        if (ret < 0)
        {
            APP_DEMO_CS2_P2P_PRT("select ret:%d\r\n", ret);
            break;
        }
        else
        {
            // is img fd, data transfer
            if (FD_ISSET(app_demo_cs2_p2p_img_fd, &watchfd))
            {
                rcv_len = recvfrom(app_demo_cs2_p2p_img_fd, rcv_buf, APP_DEMO_CS2_P2P_RCV_BUF_LEN, 0,
                                   (struct sockaddr *)app_demo_remote, &srvaddr_len);

                if (rcv_len <= 0)
                {
                    // close this socket
                    APP_DEMO_CS2_P2P_PRT("recv close fd:%d\r\n", app_demo_cs2_p2p_img_fd);
                    break;
                }
                else
                {
                    rcv_len = (rcv_len > APP_DEMO_CS2_P2P_RCV_BUF_LEN) ? APP_DEMO_CS2_P2P_RCV_BUF_LEN : rcv_len;
                    rcv_buf[rcv_len] = 0;

                    app_demo_cs2_p2p_img_receiver(rcv_buf, rcv_len, app_demo_remote);
                }
            }
            else if (FD_ISSET(cs2_p2p_cmd_fd, &watchfd))
            {
                rcv_len = recvfrom(cs2_p2p_cmd_fd, rcv_buf, APP_DEMO_CS2_P2P_RCV_BUF_LEN, 0,
                                   (struct sockaddr *)&cmd_remote, &srvaddr_len);

                if (rcv_len <= 0)
                {
                    // close this socket
                    APP_DEMO_CS2_P2P_PRT("recv close fd:%d\r\n", cs2_p2p_cmd_fd);
                    break;
                }
                else
                {
                    rcv_len = (rcv_len > APP_DEMO_CS2_P2P_RCV_BUF_LEN) ? APP_DEMO_CS2_P2P_RCV_BUF_LEN : rcv_len;
                    rcv_buf[rcv_len] = 0;

                    app_demo_cs2_p2p_handle_cmd_data(rcv_buf, rcv_len);
                }

            }

#if APP_DEMO_EN_VOICE_TRANSFER

            if (FD_ISSET(app_demo_cs2_p2p_voice_fd, &watchfd))
            {
                rcv_len = recvfrom(app_demo_cs2_p2p_voice_fd, rcv_buf, APP_DEMO_CS2_P2P_RCV_BUF_LEN, 0,
                                   (struct sockaddr *)app_demo_cs2_p2p_voice_remote, &srvaddr_len);

                if (rcv_len <= 0)
                {
                    // close this socket
                    APP_DEMO_CS2_P2P_PRT("recv close fd:%d\r\n", app_demo_cs2_p2p_voice_fd);
                    break;
                }
                else
                {
                    rcv_len = (rcv_len > APP_DEMO_CS2_P2P_RCV_BUF_LEN) ? APP_DEMO_CS2_P2P_RCV_BUF_LEN : rcv_len;
                    rcv_buf[rcv_len] = 0;

                    app_demo_cs2_p2p_voice_receiver(rcv_buf, rcv_len, app_demo_cs2_p2p_voice_remote);
                }
            }

#endif // APP_DEMO_EN_VOICE_TRANSFER
        }
    }

#endif

app_cs2_p2p_exit:

    APP_DEMO_CS2_P2P_FATAL("%s exit %d\r\n", __func__, app_demo_cs2_p2p_run);

#if (CONFIG_SPIDMA || CONFIG_CAMERA)
    bk_video_transfer_deinit();

    rwnxl_set_video_transfer_flag(false);
#endif

    if (rcv_buf)
    {
        os_free(rcv_buf);
        rcv_buf = NULL;
    }

#if APP_DEMO_EN_VOICE_TRANSFER

    if (app_demo_cs2_p2p_voice_fd != -1)
    {
        close(app_demo_cs2_p2p_voice_fd);
        app_demo_cs2_p2p_voice_fd = -1;
    }

    if (app_demo_cs2_p2p_voice_remote)
    {
        os_free(app_demo_cs2_p2p_voice_remote);
        app_demo_cs2_p2p_voice_remote = NULL;
    }

#endif // APP_DEMO_EN_VOICE_TRANSFER

    if (app_demo_remote)
    {
        os_free(app_demo_remote);
        app_demo_remote = NULL;
    }

    if (app_demo_cs2_p2p_img_fd != -1)
    {
        close(app_demo_cs2_p2p_img_fd);
        app_demo_cs2_p2p_img_fd = -1;
    }

    if (cs2_p2p_cmd_fd != -1)
    {
        close(cs2_p2p_cmd_fd);
        cs2_p2p_cmd_fd = -1;
    }

    GLOBAL_INT_DISABLE();
    app_demo_cs2_p2p_romote_connected = 0;
    app_demo_cs2_p2p_run = 0;
#if APP_DEMO_EN_VOICE_TRANSFER
    app_demo_cs2_p2p_voice_romote_connected = 0;
#endif
    GLOBAL_INT_RESTORE();

    rtos_delete_thread(NULL);
}

#endif

int app_demo_cs2_p2p_send_packet(uint8_t *data, uint32_t len)
{
    int send_byte = 0;
    uint32_t index = 0;

    if (!app_demo_cs2_p2p_romote_connected)
    {
        return 0;
    }

    g_pkt_send++;
#if 0

    //    send_byte = sendto(app_demo_udp_img_fd, data, len, MSG_DONTWAIT | MSG_MORE,
    //                       (struct sockaddr *)app_demo_remote, sizeof(struct sockaddr_in));

    send_byte = cs2_p2p_send_raw(CS2_P2P_CHANNEL_IMG, data, len);

    if (send_byte < 0)
    {
        /* err */
        //APP_DEMO_CS2_P2P_PRT("send return fd:%d\r\n", send_byte);
        g_pkt_send_fail++;
        send_byte = 0;
    }

    return send_byte;
#endif

    if (!cs2_p2p_is_connected())
    {
        return len;
    }

    do
    {
        send_byte = cs2_p2p_send_raw(CS2_P2P_CHANNEL_IMG, data, len - index);


        if (send_byte < 0)
        {
            //            LOGE("send return fd:%d\n", send_byte);
            g_pkt_send_fail++;
            return -1;
        }

        index += send_byte;

        if (index < len)
        {
//            APP_DEMO_CS2_P2P_PRT("%s delay %d %d\n", __func__, index, len);
            rtos_delay_milliseconds(10);
        }

    }
    while (index < len);

    return len;
}

UINT32 app_demo_cs2_p2p_init(    char *did_dskey,
                                 char *apilicense_crckey,
                                 char *initstring_p2pkey)
{
    bk_err_t ret = 0;
#if 0
    middle_layer_task_arg_t *targ = NULL;
    APP_DEMO_CS2_P2P_PRT("%s\r\n", __func__);

    if (!app_demo_cs2_p2p_hdl)
    {
        targ = (typeof(targ))os_malloc(sizeof(*targ));

        if (!targ)
        {
            os_printf("%s malloc fail\n");
            goto CREATE_FAIL;
        }

        targ->func = cs2_p2p_main;

        targ->did_dskey = os_malloc(strlen(did_dskey) + 1);

        if (!targ->did_dskey)
        {
            os_printf("%s malloc fail %d\n", __func__, ret);
            goto CREATE_FAIL;
        }

        memset(targ->did_dskey, 0, strlen(did_dskey) + 1);
        strcpy(targ->did_dskey, did_dskey);

        targ->apilicense_crckey = os_malloc(strlen(apilicense_crckey));

        if (!targ->apilicense_crckey)
        {
            os_printf("%s malloc fail %d\n", __func__, ret);
            goto CREATE_FAIL;
        }

        memset(targ->apilicense_crckey, 0, strlen(apilicense_crckey) + 1);
        strcpy(targ->apilicense_crckey, apilicense_crckey);

        targ->initstring_p2pkey = os_malloc(strlen(initstring_p2pkey));

        if (!targ->initstring_p2pkey)
        {
            os_printf("%s malloc fail %d\n", __func__, ret);
            goto CREATE_FAIL;
        }

        memset(targ->initstring_p2pkey, 0, strlen(initstring_p2pkey) + 1);
        strcpy(targ->initstring_p2pkey, initstring_p2pkey);

        ret = rtos_create_thread(&app_demo_cs2_p2p_hdl,
                                 4,
                                 "app_cs2_p2p",
                                 (beken_thread_function_t)app_demo_cs2_p2p_main,
                                 1024 * 12,
                                 (beken_thread_arg_t)targ);

        if (ret != kNoErr)
        {
            APP_DEMO_CS2_P2P_PRT("Error: Failed to create spidma_intfer: %d\r\n", ret);
            goto CREATE_FAIL;
        }
    }

    return kNoErr;

CREATE_FAIL:;

    if (targ)
    {
        if (targ->did_dskey)
        {
            os_free(targ->did_dskey);
        }

        if (targ->apilicense_crckey)
        {
            os_free(targ->apilicense_crckey);
        }

        if (targ->initstring_p2pkey)
        {
            os_free(targ->initstring_p2pkey);
        }

        os_free(targ);
    }

#else

    GLOBAL_INT_DECLARATION();

    cs2_p2p_main_task_create(did_dskey, apilicense_crckey, initstring_p2pkey, recv_callback);

    GLOBAL_INT_DISABLE();
    //    app_demo_cs2_p2p_romote_connected = 1;
    app_demo_cs2_p2p_run = 1;
    //#if APP_DEMO_EN_VOICE_TRANSFER
    //    app_demo_cs2_p2p_voice_romote_connected = 1;
    //#endif
    GLOBAL_INT_RESTORE();
    (void)app_demo_cs2_p2p_img_fd;
    (void)app_demo_cs2_p2p_hdl;
#if APP_DEMO_EN_VOICE_TRANSFER
    (void)app_demo_cs2_p2p_voice_fd;
#endif
    return ret;

#endif
}

void app_demo_cs2_p2p_deinit(void)
{
    GLOBAL_INT_DECLARATION();

    APP_DEMO_CS2_P2P_PRT("app_demo_cs2_p2p_deinit\r\n");

    if (app_demo_cs2_p2p_run == 0)
    {
        return;
    }

    cs2_p2p_main_task_destory();

    GLOBAL_INT_DISABLE();
    app_demo_cs2_p2p_romote_connected = 0;
    app_demo_cs2_p2p_run = 0;
#if APP_DEMO_EN_VOICE_TRANSFER
    app_demo_cs2_p2p_voice_romote_connected = 0;
#endif
    GLOBAL_INT_RESTORE();

    //    cs2_p2p_ternimal();

    //    rtos_thread_join(&app_demo_cs2_p2p_hdl);
    //    app_demo_cs2_p2p_hdl = 0;
}

#endif  // APP_DEMO_CFG_USE_cs2_p2p
#endif  //CONFIG_APP_DEMO_VIDEO_TRANSFER

// EOF


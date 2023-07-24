#include <common/bk_include.h>

#include "doorbell.h"

#include "lwip/tcp.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <os/os.h>
#include <os/str.h>
#include <common/bk_kernel_err.h>
//#include "av_audio.h"
#include <components/aud_intf.h>
//#include "av_audio_common.h"

#include "lwip/sockets.h"

#include <components/video_types.h>

#include "media_app.h"
#include "lcd_act.h"
#include "cs2_p2p_main.h"
#include <stdlib.h>

#define TAG "doorbell-cs2_p2p-Client"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)
#define SEND_BUFF_LEN 2048
#define THROUGHPUT_ANLAYSE_MS (2000)

enum
{
    CS2_P2P_CHANNEL_IMG,
    CS2_P2P_CHANNEL_VOICE,
    CS2_P2P_CHANNEL_CMD,
};

enum
{
    TRANSFER_TYPE_NONE = 0,
    TRANSFER_TYPE_VIDEO = (1 << 0),
    TRANSFER_TYPE_AUDIO = (1 << 1),
};


static uint8_t *s_send_buff = NULL;
static int doorbell_cs2_p2p_cli_video_fd = -1;
static uint8_t doorbell_cs2_p2p_cli_run = 0;
//static beken_thread_t doorbell_cs2_p2p_cli_hdl = NULL;
static struct sockaddr_in *doorbell_cs2_p2p_cli_remote = NULL;
static uint32_t video_pkt_seq = 0;
static uint8_t camera_type = APP_CAMERA_INVALIED;
static media_ppi_t camera_ppi = PPI_640X480;
static media_ppi_t lcd_ppi = PPI_480X272;
static char lcd_name[10] = {'0'};
static uint8_t s_tran_type = TRANSFER_TYPE_NONE;

static beken_timer_t s_throughput_timer;

static volatile  uint32_t s_send_video_count = 0;
static volatile  uint32_t s_send_video_bytes = 0;
static volatile  uint32_t s_send_audio_bytes = 0;
static volatile  uint32_t s_recv_audio_bytes = 0;
static volatile  uint32_t s_recv_total_bytes = 0;

static aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static aud_intf_voc_setup_t aud_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();



static void doorbell_cs2_p2p_cli_connected(void)
{
    //app_demo_softap_send_msg(DAP_APP_CONECTED, 0);
}

static void doorbell_cs2_p2p_cli_disconnected(void)
{
    //app_demo_softap_send_msg(DAP_APP_DISCONECTED, 0);
}

static int doorbell_cs2_p2p_client_send_video_packet(uint8_t *data, uint32_t len)
{
    int send_byte = 0;
    uint32_t index = 0;
    media_hdr_ext_t *tmp = (typeof(tmp))s_send_buff;
    //    send_byte = sendto(doorbell_cs2_p2p_cli_video_fd, data, len, 0,
    //                       (struct sockaddr *)doorbell_cs2_p2p_cli_remote, sizeof(struct sockaddr_in));



    if (!cs2_p2p_is_connected())
    {
        return len;
    }

    s_send_video_count++;

    tmp->magic_head = MAGIC_HEAD;
    tmp->package_len = len;

#if CS2_P2P_TRANSFER_CHECKSUM
    tmp->check_sum = 0;

    for (uint32_t i = 0; i < len; ++i)
    {
        tmp->check_sum += data[i];
    }

#endif

    //    LOGI("%s send len %d %02X %02X %02X %02X %02X %02X %02X %02X\n", __func__, len,
    //            data[0],
    //            data[1],
    //            data[2],
    //            data[3],
    //            data[4],
    //            data[5],
    //            data[6],
    //            data[7]);


    os_memcpy(s_send_buff + sizeof(*tmp), data, len);

    do
    {
        send_byte = cs2_p2p_send_raw(CS2_P2P_CHANNEL_IMG, s_send_buff + index, len + sizeof(*tmp) - index);


        if (send_byte < 0)
        {
            //            LOGE("send return fd:%d\n", send_byte);
            return -1;
        }

        index += send_byte;

        if (index < len + sizeof(*tmp))
        {
            //            LOGE("%s delay %d %d 0x%02X%02X%02X%02X%02X%02X%02X%02X\n", __func__, index, len + sizeof(*tmp),
            //                                data[0],
            //                                data[1],
            //                                data[2],
            //                                data[3],
            //                                data[4],
            //                                data[5],
            //                                data[6],
            //                                data[7]);

            rtos_delay_milliseconds(CS2_P2P_TRANSFER_DELAY);
        }

    }
    while (index < len + sizeof(*tmp));

    s_send_video_bytes += len + sizeof(*tmp);
#if 0

    if (send_byte < 0)
    {
        /* err */
        LOGE("send return fd:%d\n", send_byte);
        send_byte = 0;
    }
    else if (send_byte != len + sizeof(*tmp))
    {
        LOGE("%s len not match %d %d\n", __func__, len + sizeof(*tmp), send_byte);
        send_byte -= sizeof(*tmp);
    }
    else
    {
        send_byte -= sizeof(*tmp);
    }

    return send_byte;
#else
    return len;
#endif

}


static int av_cs2_p2p_voice_client_send_packet(unsigned char *data, unsigned int len)
{
    int send_byte = 0;
    uint32_t index = 0;

    //    send_byte = sendto(sock, data, len, 0, (struct sockaddr *)&sender, sizeof(struct sockaddr_in));

    LOGD("%s len: %d, send_byte: %d \n", __func__, len, send_byte);

    if (!cs2_p2p_is_connected())
    {
        return len;
    }

    do
    {
        send_byte = cs2_p2p_send_raw(CS2_P2P_CHANNEL_VOICE, data + index, len - index);


        if (send_byte < 0)
        {
            //            LOGE("send return fd:%d\n", send_byte);
            return -1;
        }

        index += send_byte;

        if (index < len)
        {
            rtos_delay_milliseconds(CS2_P2P_TRANSFER_DELAY);
        }

    }
    while (index < len);

    s_send_audio_bytes += len;
#if 0

    if (send_byte < len)
    {
        /* err */
        LOGE("need_send: %d, send_complete: %d, errno: %d \n", len, send_byte, errno);
        send_byte = 0;
    }

    return send_byte;
#else
    return len;
#endif

}

#if 0
static void demo_doorbell_cs2_p2p_client_main(beken_thread_arg_t data)
{
    LOGI("%s entry\n", __func__);
    (void)(data);


    doorbell_cs2_p2p_cli_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));

    if (!doorbell_cs2_p2p_cli_remote)
    {
        LOGE("cs2_p2p os_malloc failed\n");
        goto out;
    }

    // for data transfer
    doorbell_cs2_p2p_cli_video_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (doorbell_cs2_p2p_cli_video_fd == -1)
    {
        LOGE("socket failed\n");
        goto out;
    }

    doorbell_cs2_p2p_cli_remote->sin_family = AF_INET;
    doorbell_cs2_p2p_cli_remote->sin_port = htons(DEMO_DOORBELL_CS2_P2P_IMG_PORT);
    doorbell_cs2_p2p_cli_remote->sin_addr.s_addr = inet_addr("192.168.0.1");

    {
        video_setup_t setup;

        setup.open_type = TVIDEO_OPEN_SCCB;
        setup.send_type = TVIDEO_SND_UDP;
        setup.send_func = doorbell_cs2_p2p_client_send_video_packet;
        setup.start_cb = doorbell_cs2_p2p_cli_connected;
        setup.end_cb = doorbell_cs2_p2p_cli_disconnected;

        setup.pkt_header_size = sizeof(media_hdr_t);
        setup.add_pkt_header = NULL;

        if (camera_type == APP_CAMERA_DVP_JPEG)
        {
            media_app_camera_open(APP_CAMERA_DVP_JPEG, camera_ppi);
        }
        else if (camera_type == APP_CAMERA_UVC_MJPEG)
        {
            media_app_camera_open(APP_CAMERA_UVC_MJPEG, camera_ppi);
        }
        else
        {
            goto out;
        }

        media_app_transfer_open(&setup);

        lcd_open_t lcd_open;
        lcd_open.device_ppi = lcd_ppi;
        lcd_open.device_name = lcd_name;
        media_app_lcd_open(&lcd_open);
    }

    doorbell_cs2_p2p_cli_run = 1;

    while (doorbell_cs2_p2p_cli_run)
    {
        rtos_delay_milliseconds(500);
    }

out:

    LOGE("demo_doorbell_cs2_p2p_main exit %d\n", doorbell_cs2_p2p_cli_run);

    media_app_transfer_close();

    if (camera_type == APP_CAMERA_DVP_JPEG)
    {
        media_app_camera_close(APP_CAMERA_DVP_JPEG);
    }
    else if (camera_type == APP_CAMERA_UVC_MJPEG)
    {
        media_app_camera_close(APP_CAMERA_UVC_MJPEG);
    }

    video_pkt_seq = 0;

    if (doorbell_cs2_p2p_cli_remote)
    {
        os_free(doorbell_cs2_p2p_cli_remote);
        doorbell_cs2_p2p_cli_remote = NULL;
    }

    if (doorbell_cs2_p2p_cli_video_fd != -1)
    {
        close(doorbell_cs2_p2p_cli_video_fd);
        doorbell_cs2_p2p_cli_video_fd = -1;
    }

    doorbell_cs2_p2p_cli_run = 0;

    doorbell_cs2_p2p_cli_hdl = NULL;
    rtos_delete_thread(NULL);
}
#endif


static int32_t recv_callback(uint8_t channel, uint8_t *buff, uint32_t len)
{
    //uint32_t i = 0;

    if (len)
    {
        //        os_printf("%s len %d\n", __func__, len);

#if 0
#pragma message "for enable img debug !!!! remove it !!!!"
        static uint8_t first_flag = 1;

        if (first_flag)
        {
            const uint8_t cmd_img_start[] = {CMD_IMG_HEADER, CMD_START_IMG};
            first_flag = 0;
            app_demo_cs2_p2p_img_receiver((uint8_t *)cmd_img_start, sizeof(cmd_img_start), NULL);
            return 0;
        }

#endif

        //GLOBAL_INT_DIS();
        //GLOBAL_INT_RES();

        s_recv_total_bytes += len;



        switch (channel)
        {
        case CS2_P2P_CHANNEL_CMD:

            break;

        case CS2_P2P_CHANNEL_IMG:

            break;

        case CS2_P2P_CHANNEL_VOICE:
            s_recv_audio_bytes += len;

            if (s_tran_type & TRANSFER_TYPE_AUDIO)
            {
                bk_aud_intf_write_spk_data(buff, len);
            }

            break;

        default:
            break;
        }
    }

    return 0;
}

//static void demo_doorbell_add_pkt_header(video_packet_t *param)
//{
//    media_hdr_ext_t *elem_tvhdr = (typeof(elem_tvhdr))param->ptk_ptr;
//    uint32_t magic = MAGIC_HEAD;
//
//    os_memcpy(&elem_tvhdr->magic_head, &magic, sizeof(magic));
//
//    elem_tvhdr->id = (uint8_t)param->frame_id;
//    elem_tvhdr->is_eof = param->is_eof;
//    elem_tvhdr->frame_len = param->frame_len;
//    elem_tvhdr->package_len = param->ptklen;
//
//#if SUPPORT_TIANZHIHENG_DRONE
//    elem_tvhdr->unused = 0;
//#endif
//}
//


static void cs2_p2p_client_aud_voc_start(void)
{

    bk_err_t ret = BK_OK;

    //aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
    //aud_intf_drv_setup.task_config.priority = 3;
    //aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
    aud_intf_drv_setup.aud_intf_tx_mic_data = av_cs2_p2p_voice_client_send_packet;
    ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);

    if (ret != BK_ERR_AUD_INTF_OK)
    {
        LOGE("bk_aud_intf_drv_init fail, ret:%d\n", ret);
    }

    aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
    ret = bk_aud_intf_set_mode(aud_work_mode);

    if (ret != BK_ERR_AUD_INTF_OK)
    {
        LOGE("bk_aud_intf_set_mode fail, ret:%d\n", ret);
    }

    //aud_voc_setup.aec_enable = true;
    aud_voc_setup.samp_rate = AUD_INTF_VOC_SAMP_RATE_8K;
    aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_G711A;
    aud_voc_setup.spk_mode = AUD_DAC_WORK_MODE_SIGNAL_END;
    //aud_voc_setup.mic_en = AUD_INTF_VOC_MIC_OPEN;
    //aud_voc_setup.spk_en = AUD_INTF_VOC_SPK_OPEN;
    //aud_voc_setup.aec_cfg.ref_scale = 1;
    //aud_voc_setup.mic_gain = 0x2d;
    //aud_voc_setup.spk_gain = 0x2d;
    //aud_voc_setup.aec_cfg.ec_depth = 20;
    //aud_voc_setup.aec_cfg.TxRxThr = 30;
    //aud_voc_setup.aec_cfg.TxRxFlr = 6;
    //aud_voc_setup.aec_cfg.ns_level = 2;
    //aud_voc_setup.aec_cfg.ns_para = 1;

    ret = bk_aud_intf_voc_init(aud_voc_setup);

    if (ret != BK_ERR_AUD_INTF_OK)
    {
        LOGE("bk_aud_intf_voc_init fail, ret:%d\n", ret);
    }

    ret = bk_aud_intf_voc_start();

    if (ret != BK_ERR_AUD_INTF_OK)
    {
        LOGE("bk_aud_intf_voc_start fail, ret:%d\n", ret);
    }

}

static void cs2_p2p_client_aud_voc_stop(void)
{

    bk_aud_intf_voc_stop();

    bk_aud_intf_voc_deinit();

    bk_aud_intf_drv_deinit();

}

static void throughput_anlayse_timer_hdl(void *param)
{
    BK_LOGI("cs2_tp", "send %d video %.3f audio %.3f, recv audio %.3f total %.3f KB/s\n",
            s_send_video_count,
            (float)(s_send_video_bytes / (THROUGHPUT_ANLAYSE_MS / 1000) / 1024.0),
            (float)(s_send_audio_bytes / (THROUGHPUT_ANLAYSE_MS / 1000) / 1024.0),
            (float)(s_recv_audio_bytes / (THROUGHPUT_ANLAYSE_MS / 1000) / 1024.0),
            (float)(s_recv_total_bytes / (THROUGHPUT_ANLAYSE_MS / 1000) / 1024.0));


    s_send_video_count = s_recv_total_bytes = s_send_video_bytes = s_send_audio_bytes = s_recv_audio_bytes = 0;

    return;
}

#ifdef CONFIG_INTEGRATION_DOORBELL
static const media_transfer_cb_t doorbell_udp_callback = {
	.send = doorbell_cs2_p2p_client_send_video_packet,
	.prepare = NULL,
	.get_tx_buf = NULL,
	.get_tx_size = 0
};
#endif

static int8_t before_start(void)
{
    s_send_buff = os_malloc(SEND_BUFF_LEN);

    if (!s_send_buff)
    {
        BK_LOGE("p2p_client", "%s alloc err\n", __func__);
        BK_ASSERT(0);
        goto out;
    }

    if (s_tran_type & TRANSFER_TYPE_VIDEO)
    {
        lcd_open_t lcd_open;
        lcd_open.device_ppi = lcd_ppi;
        lcd_open.device_name = lcd_name;
        media_app_lcd_open(&lcd_open);

#ifdef CONFIG_INTEGRATION_DOORBELL
        media_app_transfer_open(&doorbell_udp_callback);
#else
		video_setup_t setup;

		setup.open_type = TVIDEO_OPEN_SCCB;
        setup.send_type = TVIDEO_SND_UDP;
        setup.send_func = doorbell_cs2_p2p_client_send_video_packet;
        setup.start_cb = doorbell_cs2_p2p_cli_connected;
        setup.end_cb = doorbell_cs2_p2p_cli_disconnected;

        setup.pkt_header_size = sizeof(media_hdr_t);
        setup.add_pkt_header = NULL;//demo_doorbell_add_pkt_header;
        media_app_transfer_open(&setup);
#endif

        if (camera_type == APP_CAMERA_DVP_JPEG)
        {
#ifdef CONFIG_INTEGRATION_DOORBELL
			camera_config_t camera_config;
	
			os_memset(&camera_config, 0, sizeof(camera_config_t));
	
			camera_config.type = DVP_CAMERA;
			camera_config.image_format = IMAGE_MJPEG;
			camera_config.width = camera_ppi >> 16;
			camera_config.height = camera_ppi & 0xFFFF;
						
			media_app_camera_open(&camera_config);
#else
            media_app_camera_open(APP_CAMERA_DVP_JPEG, camera_ppi);
#endif
        }
        else if (camera_type == APP_CAMERA_UVC_MJPEG)
        {
#ifdef CONFIG_INTEGRATION_DOORBELL
			camera_config_t camera_config;

			os_memset(&camera_config, 0, sizeof(camera_config_t));

			camera_config.type = DVP_CAMERA;
			camera_config.image_format = IMAGE_MJPEG;
			camera_config.width = camera_ppi >> 16;
			camera_config.height = camera_ppi & 0xFFFF;
						
			media_app_camera_open(&camera_config);
#else
            media_app_camera_open(APP_CAMERA_UVC_MJPEG, camera_ppi);
#endif
        }
        else
        {
            goto out;
        }
    }

    if (s_tran_type & TRANSFER_TYPE_AUDIO)
    {
        cs2_p2p_client_aud_voc_start();
    }


    if (rtos_is_timer_init(&s_throughput_timer))
    {
        if (rtos_is_timer_running(&s_throughput_timer))
        {
            rtos_stop_timer(&s_throughput_timer);
        }

        rtos_deinit_timer(&s_throughput_timer);
    }

    if (!rtos_is_timer_init(&s_throughput_timer))
    {
        //        rtos_init_timer(&bt_a2dp_source_write_timer, inter, bt_a2dp_source_write_from_file_timer_hdl, (void *)&pcm_file_fd);
        rtos_init_timer(&s_throughput_timer, THROUGHPUT_ANLAYSE_MS, throughput_anlayse_timer_hdl, NULL);

    }


    rtos_change_period(&s_throughput_timer, THROUGHPUT_ANLAYSE_MS);
    rtos_start_timer(&s_throughput_timer);

    return 0;
out:;
    return -1;
}

static int8_t after_end(void)
{
    LOGE("%s\n", __func__);

    if (rtos_is_timer_init(&s_throughput_timer))
    {
        if (rtos_is_timer_running(&s_throughput_timer))
        {
            rtos_stop_timer(&s_throughput_timer);
        }

        rtos_deinit_timer(&s_throughput_timer);
    }

    if (s_tran_type & TRANSFER_TYPE_AUDIO)
    {
        cs2_p2p_client_aud_voc_stop();
    }

    if (s_tran_type & TRANSFER_TYPE_VIDEO)
    {
        media_app_transfer_close();

        if (camera_type == APP_CAMERA_DVP_JPEG)
        {
            media_app_camera_close(APP_CAMERA_DVP_JPEG);
        }
        else if (camera_type == APP_CAMERA_UVC_MJPEG)
        {
            media_app_camera_close(APP_CAMERA_UVC_MJPEG);
        }

        video_pkt_seq = 0;
    }

    if (doorbell_cs2_p2p_cli_remote)
    {
        os_free(doorbell_cs2_p2p_cli_remote);
        doorbell_cs2_p2p_cli_remote = NULL;
    }

    if (doorbell_cs2_p2p_cli_video_fd != -1)
    {
        close(doorbell_cs2_p2p_cli_video_fd);
        doorbell_cs2_p2p_cli_video_fd = -1;
    }


    //    doorbell_cs2_p2p_cli_run = 0;

    //    doorbell_cs2_p2p_cli_hdl = NULL;
    return 0;
}

bk_err_t demo_doorbell_cs2_p2p_client_init(int argc, char **argv)
{
    char *did_dskey = NULL;
    char *apilicense_crckey = NULL;
    char *initstring_p2pkey = NULL;

    int ret;

    if (argc == 0)
    {
        camera_type = APP_CAMERA_DVP_JPEG;
        camera_ppi = PPI_640X480;
        lcd_ppi = PPI_480X272;
    }
    else if (argc == 1)
    {
        if (os_strcmp(argv[0], "uvc") == 0)
        {
            camera_type = APP_CAMERA_UVC;
        }
        else
        {
            camera_type = APP_CAMERA_DVP_JPEG;
        }

        camera_ppi = PPI_640X480;
        lcd_ppi = PPI_480X272;
    }
    else if (argc == 2)
    {
        if (os_strcmp(argv[0], "uvc") == 0)
        {
            camera_type = APP_CAMERA_UVC;
        }
        else
        {
            camera_type = APP_CAMERA_DVP_JPEG;
        }

        camera_ppi = get_string_to_ppi(argv[1]);

        if (camera_ppi == PPI_DEFAULT)
        {
            camera_ppi = PPI_640X480;
        }
    }
    else if (argc == 3 || argc == 6)
    {
        if (os_strcmp(argv[0], "uvc") == 0)
        {
            camera_type = APP_CAMERA_UVC;
        }
        else
        {
            camera_type = APP_CAMERA_DVP_JPEG;
        }

        camera_ppi = get_string_to_ppi(argv[1]);

        if (camera_ppi == PPI_DEFAULT)
        {
            camera_ppi = PPI_640X480;
        }

        lcd_ppi = get_string_to_ppi(argv[2]);

        if (lcd_ppi == PPI_DEFAULT)
        {
            lcd_ppi = PPI_480X272;
        }

        if (argc == 6)
        {
            did_dskey = argv[3];
            apilicense_crckey = argv[4];
            initstring_p2pkey = argv[5];
        }
    }
    else if (argc == 4)
    {
        if (os_strcmp(argv[0], "uvc") == 0)
        {
            camera_type = APP_CAMERA_UVC;
        }
        else
        {
            camera_type = APP_CAMERA_DVP_JPEG;
        }

        camera_ppi = get_string_to_ppi(argv[1]);

        if (camera_ppi == PPI_DEFAULT)
        {
            camera_ppi = PPI_640X480;
        }

        lcd_ppi = get_string_to_ppi(argv[2]);

        if (lcd_ppi == PPI_DEFAULT)
        {
            lcd_ppi = PPI_480X272;
        }

        os_memcpy(lcd_name, argv[3], os_strlen(argv[3]));

    }
    else if (argc == 5 || argc == 5 + 3)
    {
        if (os_strcmp(argv[0], "uvc") == 0)
        {
            camera_type = APP_CAMERA_UVC;
        }
        else
        {
            camera_type = APP_CAMERA_DVP_JPEG;
        }

        camera_ppi = get_string_to_ppi(argv[1]);

        if (camera_ppi == PPI_DEFAULT)
        {
            camera_ppi = PPI_640X480;
        }

        lcd_ppi = get_string_to_ppi(argv[2]);

        if (lcd_ppi == PPI_DEFAULT)
        {
            lcd_ppi = PPI_480X272;
        }

        os_memcpy(lcd_name, argv[3], os_strlen(argv[3]));

        s_tran_type = atoi(argv[4]);

        LOGI("%s s_tran_type 0x%X\n", __func__, s_tran_type);

        if (argc == 5 + 3)
        {
            did_dskey = argv[5];
            apilicense_crckey = argv[6];
            initstring_p2pkey = argv[7];
        }
    }

    LOGI("%s\n", __func__);

    if (!doorbell_cs2_p2p_cli_run)
    {
        //        ret = rtos_create_thread(&doorbell_cs2_p2p_cli_hdl,
        //                                 4,
        //                                 "doorbell_cli",
        //                                 (beken_thread_function_t)demo_doorbell_cs2_p2p_client_main,
        //                                 1024 * 4,
        //                                 (beken_thread_arg_t)NULL);

        //        ret = cs2_p2p_main_task_create(did_dskey, apilicense_crckey, initstring_p2pkey, recv_callback);
        if (s_tran_type & TRANSFER_TYPE_AUDIO)
        {
            ret = cs2_p2p_main_task_create_ext(did_dskey, apilicense_crckey, initstring_p2pkey, recv_callback, before_start, after_end);
        }
        else
        {
            ret = cs2_p2p_main_task_create_ext(did_dskey, apilicense_crckey, initstring_p2pkey, NULL, before_start, after_end);
        }

        if (ret != kNoErr)
        {
            LOGE("Error: Failed to create spidma_intfer: %d\n", ret);
            return kGeneralErr;
        }
    }

    doorbell_cs2_p2p_cli_run = 1;

    return kNoErr;
}

void demo_doorbell_cs2_p2p_cli_deinit(void)
{
    GLOBAL_INT_DECLARATION();

    LOGI("app_demo_cs2_p2p_deinit\n");

    if (doorbell_cs2_p2p_cli_run == 0)
    {
        return;
    }

    cs2_p2p_main_task_destory();

    GLOBAL_INT_DISABLE();
    doorbell_cs2_p2p_cli_run = 0;
    GLOBAL_INT_RESTORE();

}


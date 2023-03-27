#include <common/bk_include.h>

#include "doorbell.h"

#include "lwip/tcp.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <os/os.h>
#include <os/str.h>
#include <common/bk_kernel_err.h>
#include <components/video_types.h>
//#include "av_audio.h"
#include <components/aud_intf.h>
//#include "av_audio_common.h"

#include "lwip/sockets.h"

#include "lcd_act.h"

#include "media_app.h"
#include "cs2_p2p_main.h"
#include <stdlib.h>

#define TAG "doorbell-cs2_p2p-Server"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define RECV_BUFF_SIZE (3 * 1024)

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


//static beken_thread_t doorbell_cs2_p2p_ser_hdl = NULL;
static uint8_t *s_recv_buff = NULL;
static uint32_t s_recv_buff_write_index = 0;
static uint8_t doorbell_cs2_p2p_ser_run = 0;
static struct sockaddr_in *doorbell_cs2_p2p_ser_remote = NULL;
static int doorbell_cs2_p2p_ser_video_fd = -1;
static media_ppi_t camera_ppi = PPI_640X480;
static media_ppi_t lcd_ppi = PPI_480X272;
static char lcd_name[10] = {'0'};
static uint8_t s_tran_type = TRANSFER_TYPE_NONE;


static aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static aud_intf_voc_setup_t aud_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();



extern uint32_t bk_net_send_data(uint8_t *data, uint32_t length, video_send_type_t type);

#if 0
static void demo_doorbell_cs2_p2p_server_main(beken_thread_arg_t data)
{
    int rcv_len = 0;
    socklen_t srvaddr_len = 0;
    //  struct sockaddr_in server;
    struct sockaddr_in sender;

    u8 *rcv_buf = NULL;

    LOGI("%s entry\n", __func__);
    (void)(data);

    rcv_buf = (u8 *) os_malloc((APP_DEMO_RCV_BUF_LEN + 1) * sizeof(u8));

    if (!rcv_buf)
    {
        LOGE("cs2_p2p os_malloc failed\n");
        goto out;
    }

    doorbell_cs2_p2p_ser_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));

    if (!doorbell_cs2_p2p_ser_remote)
    {
        LOGE("doorbell_cs2_p2p_ser_remote os_malloc failed\n");
        goto out;
    }

    // for data transfer
    doorbell_cs2_p2p_ser_video_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (doorbell_cs2_p2p_ser_video_fd == -1)
    {
        LOGE("socket failed\n");
        goto out;
    }

    doorbell_cs2_p2p_ser_remote->sin_family = AF_INET;
    doorbell_cs2_p2p_ser_remote->sin_port = htons(DEMO_DOORBELL_cs2_p2p_IMG_PORT);
    doorbell_cs2_p2p_ser_remote->sin_addr.s_addr = inet_addr("0.0.0.0");

    sender.sin_family = AF_INET;
    sender.sin_port = htons(DEMO_DOORBELL_cs2_p2p_IMG_PORT);
    sender.sin_addr.s_addr = inet_addr("192.168.0.100");

    srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);

    if (bind(doorbell_cs2_p2p_ser_video_fd, (struct sockaddr *)doorbell_cs2_p2p_ser_remote, srvaddr_len) == -1)
    {
        LOGE("bind failed\n");
        goto out;
    }
    else
    {
        media_app_camera_open(APP_CAMERA_NET_MJPEG, camera_ppi);

        lcd_open_t lcd_open;
        lcd_open.device_ppi = lcd_ppi;
        lcd_open.device_name = lcd_name;
        LOGI("%s, lcd_ppi:%d-%d\r\n", __func__, lcd_ppi >> 16, lcd_ppi & 0xFFFF);
        media_app_lcd_open(&lcd_open);
    }

    doorbell_cs2_p2p_ser_run = 1;

    while (doorbell_cs2_p2p_ser_run)
    {
        rcv_len = recvfrom(doorbell_cs2_p2p_ser_video_fd, rcv_buf, APP_DEMO_RCV_BUF_LEN, 0,
                           (struct sockaddr *)&sender, &srvaddr_len);

        if (rcv_len > 0)
        {
            bk_net_send_data(rcv_buf, rcv_len, TVIDEO_SND_cs2_p2p);
        }
        else
        {
            LOGE("recvfrom:%d\r\n", rcv_len);
        }
    }

out:

    LOGE("%s exit %d\n", __func__, doorbell_cs2_p2p_ser_run);

    media_app_lcd_close();

    media_app_camera_close(APP_CAMERA_NET_MJPEG);

    if (rcv_buf)
    {
        os_free(rcv_buf);
        rcv_buf = NULL;
    }

    if (doorbell_cs2_p2p_ser_remote)
    {
        os_free(doorbell_cs2_p2p_ser_remote);
        doorbell_cs2_p2p_ser_remote = NULL;
    }

    if (doorbell_cs2_p2p_ser_video_fd != -1)
    {
        close(doorbell_cs2_p2p_ser_video_fd);
        doorbell_cs2_p2p_ser_video_fd = -1;
    }

    doorbell_cs2_p2p_ser_run = 0;

    doorbell_cs2_p2p_ser_hdl = NULL;
    rtos_delete_thread(NULL);
}
#endif

static int av_cs2_p2p_voice_server_send_packet(unsigned char *data, unsigned int len)
{

    int send_byte = 0;
    uint32_t index = 0;

    //    send_byte = sendto(sock, data, len, 0, (struct sockaddr *)&sender, sizeof(struct sockaddr_in));

    LOGD("len: %d, send_byte: %d \n", len, send_byte);

    if (!cs2_p2p_is_connected())
    {
        return len;
    }

    do
    {
        send_byte = cs2_p2p_send_raw(CS2_P2P_CHANNEL_VOICE, data, len - index);


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

static void cs2_p2p_server_aud_voc_start(void)
{

    bk_err_t ret = BK_OK;

    //aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
    //aud_intf_drv_setup.task_config.priority = 3;
    //aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
    aud_intf_drv_setup.aud_intf_tx_mic_data = av_cs2_p2p_voice_server_send_packet;
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

static void cs2_p2p_server_aud_voc_stop(void)
{

    bk_aud_intf_voc_stop();

    bk_aud_intf_voc_deinit();

    bk_aud_intf_drv_deinit();

}


static int8_t before_start(void)
{
    s_recv_buff = os_malloc(RECV_BUFF_SIZE);

    if (!s_recv_buff)
    {
        BK_LOGE("doorbell", "%s alloc err\n", __func__);
        BK_ASSERT(0);
        goto out;
    }


    if (s_tran_type & TRANSFER_TYPE_VIDEO)
    {
        media_app_camera_open(APP_CAMERA_NET_MJPEG, camera_ppi);

        lcd_open_t lcd_open;
        lcd_open.device_ppi = lcd_ppi;
        lcd_open.device_name = lcd_name;
        LOGI("%s, lcd_ppi:%d-%d\r\n", __func__, lcd_ppi >> 16, lcd_ppi & 0xFFFF);
        media_app_lcd_open(&lcd_open);
    }

    if (s_tran_type & TRANSFER_TYPE_AUDIO)
    {
        cs2_p2p_server_aud_voc_start();
    }

    return 0;

out:;
    return -1;
}


static int8_t after_end(void)
{
    LOGE("%s\n", __func__);

    if (s_tran_type & TRANSFER_TYPE_AUDIO)
    {
        cs2_p2p_server_aud_voc_stop();
    }

    if (s_tran_type & TRANSFER_TYPE_VIDEO)
    {
        media_app_lcd_close();

        media_app_camera_close(APP_CAMERA_NET_MJPEG);
    }

    if (doorbell_cs2_p2p_ser_remote)
    {
        os_free(doorbell_cs2_p2p_ser_remote);
        doorbell_cs2_p2p_ser_remote = NULL;
    }

    if (doorbell_cs2_p2p_ser_video_fd != -1)
    {
        close(doorbell_cs2_p2p_ser_video_fd);
        doorbell_cs2_p2p_ser_video_fd = -1;
    }

    if (s_recv_buff)
    {
        os_free(s_recv_buff);
    }

    s_recv_buff = NULL;

    doorbell_cs2_p2p_ser_run = 0;

    //    doorbell_cs2_p2p_ser_hdl = NULL;
    return 0;
}


static int32_t recv_callback(uint8_t channel, uint8_t *buff, uint32_t len)
{
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
        switch (channel)
        {
        case CS2_P2P_CHANNEL_CMD:

            break;

        case CS2_P2P_CHANNEL_IMG:
        {
            if ((s_tran_type & TRANSFER_TYPE_VIDEO) && len > 0)
            {

                //规避上层封包问题
                uint32_t already_len = 0;
                uint32_t recv_len = 0;
                uint32_t i = 0;
                uint32_t read_index = 0;

                //                LOGE("%s recv:%d\r\n", __func__, len);
                do
                {
                    already_len = ((RECV_BUFF_SIZE - s_recv_buff_write_index) < len - recv_len ? (RECV_BUFF_SIZE - s_recv_buff_write_index) : len - recv_len);
                    os_memcpy(s_recv_buff + s_recv_buff_write_index, buff, already_len);

                    s_recv_buff_write_index += already_len;
                    recv_len += already_len;

                    if (s_recv_buff_write_index > sizeof(media_hdr_ext_t))
                    {
                        for (i = 0, read_index = 0; i < s_recv_buff_write_index - sizeof(media_hdr_ext_t); ++i)
                        {
                            media_hdr_ext_t *m_head = (typeof(m_head))(s_recv_buff + i);

                            if (m_head->magic_head == MAGIC_HEAD)
                            {

                                uint32_t package_len = m_head->package_len;

                                read_index = i;

                                if (0)//package_len >= 1500 || package_len <= 200)
                                {
                                    LOGE("%s err package_len %d\n", __func__, package_len);
                                    read_index += sizeof(media_hdr_ext_t);
                                    continue;
                                }

                                if (package_len > s_recv_buff_write_index - sizeof(media_hdr_ext_t) - i)
                                {
                                    //not enough
                                    break;
                                }


#if CS2_P2P_TRANSFER_CHECKSUM
                                uint8_t sum = 0;

                                for (uint32_t j = 0; j < package_len; ++j)
                                {
                                    sum += (s_recv_buff + i + sizeof(media_hdr_ext_t))[j];
                                }

                                if (sum != m_head->check_sum)
                                {
                                    LOGE("%s check sum not match 0x%02X 0x%02X\n", __func__, sum, m_head->check_sum);
                                    read_index = i + sizeof(media_hdr_ext_t) + package_len;
                                    continue;
                                }

#endif
                                //                                LOGI("%s send len %d %02X %02X %02X %02X %02X %02X %02X %02X \n", __func__, package_len,
                                //                                     s_recv_buff[i + sizeof(media_hdr_ext_t) + 0],
                                //                                     s_recv_buff[i + sizeof(media_hdr_ext_t) + 1],
                                //                                     s_recv_buff[i + sizeof(media_hdr_ext_t) + 2],
                                //                                     s_recv_buff[i + sizeof(media_hdr_ext_t) + 3],
                                //                                     s_recv_buff[i + sizeof(media_hdr_ext_t) + 4],
                                //                                     s_recv_buff[i + sizeof(media_hdr_ext_t) + 5],
                                //                                     s_recv_buff[i + sizeof(media_hdr_ext_t) + 6],
                                //                                     s_recv_buff[i + sizeof(media_hdr_ext_t) + 7]);
                                bk_net_send_data(s_recv_buff + i + sizeof(media_hdr_ext_t), package_len, TVIDEO_SND_UDP);
                                //                                LOGI("%s send end\n", __func__);
                                read_index = i + sizeof(media_hdr_ext_t) + package_len;
                            }
                        }
                    }

                    if (read_index && read_index != s_recv_buff_write_index)
                    {
                        os_memmove(s_recv_buff, s_recv_buff + read_index, s_recv_buff_write_index - read_index);
                    }

                    s_recv_buff_write_index -= read_index;
                    read_index = 0;

                }
                while (len - recv_len > 0);

            }
            else
            {
                LOGE("%s recv:%d\r\n", __func__, len);
            }
        }

        break;



        case CS2_P2P_CHANNEL_VOICE:
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

bk_err_t demo_doorbell_cs2_p2p_server_init(int argc, char **argv)
{
    int ret;

    char *did_dskey = NULL;
    char *apilicense_crckey = NULL;
    char *initstring_p2pkey = NULL;

    if (argc == 0)
    {
        camera_ppi = PPI_640X480;
        lcd_ppi = PPI_480X272;
    }
    else if (argc == 1)
    {
        camera_ppi = get_string_to_ppi(argv[0]);

        if (camera_ppi == PPI_DEFAULT)
        {
            camera_ppi = PPI_640X480;
        }

        lcd_ppi = PPI_480X272;
    }
    else if (argc == 2)
    {
        camera_ppi = get_string_to_ppi(argv[0]);

        if (camera_ppi == PPI_DEFAULT)
        {
            camera_ppi = PPI_640X480;
        }

        lcd_ppi = get_string_to_ppi(argv[1]);

        if (lcd_ppi == PPI_DEFAULT)
        {
            lcd_ppi = PPI_480X272;
        }
    }
    else if (argc == 3)
    {
        camera_ppi = get_string_to_ppi(argv[0]);

        if (camera_ppi == PPI_DEFAULT)
        {
            camera_ppi = PPI_640X480;
        }

        lcd_ppi = get_string_to_ppi(argv[1]);

        if (lcd_ppi == PPI_DEFAULT)
        {
            lcd_ppi = PPI_480X272;
        }

        os_memcpy(lcd_name, argv[2], os_strlen(argv[2]));

    }
    else if (argc == 4 || argc == 4 + 3)
    {
        camera_ppi = get_string_to_ppi(argv[0]);

        if (camera_ppi == PPI_DEFAULT)
        {
            camera_ppi = PPI_640X480;
        }

        lcd_ppi = get_string_to_ppi(argv[1]);

        if (lcd_ppi == PPI_DEFAULT)
        {
            lcd_ppi = PPI_480X272;
        }

        os_memcpy(lcd_name, argv[2], os_strlen(argv[2]));

        s_tran_type = atoi(argv[3]);

        LOGI("%s s_tran_type 0x%X\n", __func__, s_tran_type);

        if (argc == 4 + 3)
        {
            did_dskey = argv[4];
            apilicense_crckey = argv[5];
            initstring_p2pkey = argv[6];
        }
    }
    else
    {
        camera_ppi = PPI_640X480;
        lcd_ppi = PPI_480X272;
    }

    LOGI("%s\n", __func__);

    if (!doorbell_cs2_p2p_ser_run)
    {
        //        ret = rtos_create_thread(&doorbell_cs2_p2p_ser_hdl,
        //                                 4,
        //                                 "doorbell_ser",
        //                                 (beken_thread_function_t)demo_doorbell_cs2_p2p_server_main,
        //                                 1024 * 2,
        //                                 (beken_thread_arg_t)NULL);

        ret = cs2_p2p_main_task_create_ext(did_dskey, apilicense_crckey, initstring_p2pkey, recv_callback, before_start, after_end);

        if (ret != kNoErr)
        {
            LOGE("Error: Failed to create doorbell_ser: %d\n", ret);
            return kGeneralErr;
        }
    }

    doorbell_cs2_p2p_ser_run = 1;

    return kNoErr;
}

void demo_doorbell_cs2_p2p_server_deinit(void)
{
    GLOBAL_INT_DECLARATION();

    LOGI("%s\n", __func__);

    if (doorbell_cs2_p2p_ser_run == 0)
    {
        return;
    }

    cs2_p2p_main_task_destory();

    GLOBAL_INT_DISABLE();
    doorbell_cs2_p2p_ser_run = 0;
    GLOBAL_INT_RESTORE();
}


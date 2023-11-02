#include <common/bk_include.h>

#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <driver/int.h>
#include <common/bk_err.h>

#ifdef CONFIG_RTT
#include <sys/socket.h>
#endif
#include "lwip/sockets.h"

#include "doorbell.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <components/video_transfer.h>
#include <driver/dma.h>
#include <modules/audio_ring_buff.h>
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#if CONFIG_DOORBELL_DEMO2
#include <driver/gpio.h>
#include "gpio_driver.h"
#endif
#include "lcd_act.h"

#include "aud_debug_tcp.h"

#include "media_app.h"

#define TAG "doorbell-UDP"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


#define APP_DEMO_UDP_RCV_BUF_LEN        1472
#define APP_DEMO_UDP_SOCKET_TIMEOUT     100  // ms

int demo_doorbell_udp_img_fd = -1;
volatile int demo_doorbell_udp_romote_connected = 0;
volatile int demo_doorbell_udp_run = 0;
beken_thread_t demo_doorbell_udp_hdl = NULL;
struct sockaddr_in *demo_doorbell_remote = NULL;
int demo_doorbell_udp_voice_romote_connected = 0;

struct sockaddr_in *demo_doorbell_voice_udp_remote = NULL;

int demo_doorbell_udp_voice_fd = -1;

#if AUDIO_TRANSFER_ENABLE
static aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static aud_intf_voc_setup_t aud_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();
#endif


int demo_doorbell_udp_send_packet(uint8_t *data, uint32_t len)
{
	int send_byte = 0;

	if (!demo_doorbell_udp_romote_connected)
	{
		return 0;
	}

	send_byte = sendto(demo_doorbell_udp_img_fd, data, len, MSG_DONTWAIT | MSG_MORE,
	                   (struct sockaddr *)demo_doorbell_remote, sizeof(struct sockaddr_in));

	if (send_byte < 0)
	{
		/* err */
		//DBD("send return fd:%d\n", send_byte);
		send_byte = 0;
	}

	return send_byte;
}

#ifdef CONFIG_INTEGRATION_DOORBELL
static const media_transfer_cb_t doorbell_udp_callback = {
	.send = demo_doorbell_udp_send_packet,
	.prepare = NULL,
	.get_tx_buf = NULL,
	.get_tx_size = NULL
};
#endif


#if DEMO_DOORBELL_EN_VOICE_TRANSFER
int demo_doorbell_udp_voice_send_packet(unsigned char *data, unsigned int len)
{
	int send_byte = 0;

	if (!demo_doorbell_udp_voice_romote_connected)
	{
		return 0;
	}

	send_byte = sendto(demo_doorbell_udp_voice_fd, data, len, MSG_DONTWAIT | MSG_MORE,
	                   (struct sockaddr *)demo_doorbell_voice_udp_remote, sizeof(struct sockaddr_in));

	if (send_byte < 0)
	{
		/* err */
		//LWIP_UDP_PRT("send return fd:%d\n", send_byte);
		send_byte = 0;
	}

	return send_byte;
}
#endif //DEMO_DOORBELL_EN_VOICE_TRANSFER

void demo_doorbell_add_pkt_header(video_packet_t *param)
{
	media_hdr_t *elem_tvhdr = (media_hdr_t *)param->ptk_ptr;

	elem_tvhdr->id = (uint8_t)param->frame_id;
	elem_tvhdr->is_eof = param->is_eof;
	elem_tvhdr->pkt_cnt = param->frame_len;
	elem_tvhdr->size = 0;

#if SUPPORT_TIANZHIHENG_DRONE
	elem_tvhdr->unused = 0;
#endif
}

void aud_intf_uac_connect_state_cb_handle(uint8_t state)
{
	os_printf("[--%s--] state: %d \n", __func__, state);
}

static void demo_doorbell_udp_handle_cmd_data(uint8_t *data, UINT16 len)
{
#if AUDIO_TRANSFER_ENABLE
	bk_err_t ret = BK_ERR_AUD_INTF_OK;
#endif
	uint32_t param = 0;
	bool lcd_rotate = 0;
	uint32_t cmd = (uint32_t)data[0] << 24 | (uint32_t)data[1] << 16 | (uint32_t)data[2] << 8 | data[3];
	if (len > 4)
	{
		param = (uint32_t)data[4] << 24 | (uint32_t)data[5] << 16 | (uint32_t)data[6] << 8 | data[7];
	}

	LOGI("doorbell cmd: %x, param: %d, len: %d\n", cmd, param, len);
	if (len > 8)
	{
		lcd_rotate = data[8];
		LOGI("%s, lcd_rotate:%d\r\n", __func__, lcd_rotate);
	}

	if (len >= 8)
	{
		switch (cmd)
		{
#if AUDIO_TRANSFER_ENABLE
			case AUDIO_CLOSE:
				LOGI("close audio \n");

				bk_aud_intf_voc_stop();
				bk_aud_intf_voc_deinit();
				aud_work_mode = AUD_INTF_WORK_MODE_NULL;
				bk_aud_intf_set_mode(aud_work_mode);
				bk_aud_intf_drv_deinit();
				break;

			case AUDIO_OPEN:
				LOGI("open audio \n");
				//aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
				//aud_intf_drv_setup.task_config.priority = 3;
				//aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
				aud_intf_drv_setup.aud_intf_tx_mic_data = demo_doorbell_udp_voice_send_packet;
				ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);
				if (ret != BK_ERR_AUD_INTF_OK)
				{
					LOGE("bk_aud_intf_drv_init fail, ret:%d\n", ret);
					break;
				}
				aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
				ret = bk_aud_intf_set_mode(aud_work_mode);
				if (ret != BK_ERR_AUD_INTF_OK)
				{
					LOGE("bk_aud_intf_set_mode fail, ret:%d\n", ret);
					break;
				}
				if (data[9] == 1) {
					aud_voc_setup.aec_enable = true;
				} else {
					aud_voc_setup.aec_enable = false;
				}
				//aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_G711A;
				//aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_PCM;
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
				
				if (data[9] == 1)
				{
					aud_voc_setup.aec_enable = true;
				}
				else
				{
					aud_voc_setup.aec_enable = false;
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

				//aud_voc_setup.aec_cfg.ref_scale = 1;
#if CONFIG_DOORBELL_DEMO2
				aud_voc_setup.mic_gain = 0x2d;
				aud_voc_setup.spk_gain = 0x27;
				aud_voc_setup.aec_cfg.ec_depth = 0x60;
				aud_voc_setup.aec_cfg.TxRxThr = 30;
				aud_voc_setup.aec_cfg.TxRxFlr = 6;
				aud_voc_setup.aec_cfg.ns_level = 2;
				aud_voc_setup.aec_cfg.ns_para = 1;
#else
				aud_voc_setup.mic_gain = 0x2d;
				aud_voc_setup.spk_gain = 0x2d;
				aud_voc_setup.aec_cfg.ec_depth = 20;
				aud_voc_setup.aec_cfg.TxRxThr = 30;
				aud_voc_setup.aec_cfg.TxRxFlr = 6;
				aud_voc_setup.aec_cfg.ns_level = 2;
				aud_voc_setup.aec_cfg.ns_para = 1;
#endif
				//aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_G711U;

				ret = bk_aud_intf_voc_init(aud_voc_setup);
				if (ret != BK_ERR_AUD_INTF_OK)
				{
					LOGE("bk_aud_intf_voc_init fail, ret:%d\n", ret);
					break;
				}
				ret = bk_aud_intf_voc_start();
				if (ret != BK_ERR_AUD_INTF_OK)
				{
					LOGE("bk_aud_intf_voc_start fail, ret:%d\n", ret);
					break;
				}

				ret = bk_aud_intf_register_uac_connect_state_cb(aud_intf_uac_connect_state_cb_handle);
				if (ret != BK_ERR_AUD_INTF_OK)
				{
					LOGE("bk_aud_intf_register_uac_connect_state_cb fail, ret:%d\n", ret);
					break;
				}

				ret = bk_aud_intf_uac_auto_connect_ctrl(true);
				if (ret != BK_ERR_AUD_INTF_OK)
				{
					LOGE("aud_tras_uac_auto_connect_ctrl fail, ret:%d\n", ret);
					break;
				}

				break;
#endif  //AUDIO_TRANSFER_ENABLE

			case DISPLAY_CLOSE:
				LOGI("LCD CLOSE\n");
				media_app_lcd_close();
				break;

			case DISPLAY_OPEN:
				LOGI("LCD OPEN: %dX%d ROTATE: %d\n", param >> 16, param & 0xFFFF, lcd_rotate);
				media_app_lcd_rotate(lcd_rotate);
				lcd_open_t lcd_open = {0};
				lcd_open.device_ppi = param;
				lcd_open.device_name = NULL;
				media_app_lcd_open(&lcd_open);
				break;

#if CONFIG_AUD_INTF
			case ECHO_DEPTH:
				LOGI("set ECHO_DEPTH: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_EC_DEPTH, param);
				break;

			case MAX_AMPLITUDE:
				LOGI("set MAX_AMPLITUDE: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_TXRX_THR, param);
				break;

			case MIN_AMPLITUDE:
				LOGI("set MIN_AMPLITUDE: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_TXRX_FLR, param);
				break;

			case NOISE_LEVEL:
				LOGI("set NOISE_LEVEL: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_NS_LEVEL, param);
				break;

			case NOISE_PARAM:
				LOGI("set NOISE_PARAM: %d\n", param);
				bk_aud_intf_set_aec_para(AUD_INTF_VOC_AEC_NS_PARA, param);
				break;

			/* open audio debug, create tcp port for audio debug */
			case AUD_DEBUG_OPEN:
				if (param)
				{
					LOGI("open audio debug: %d\n", param);
					bk_aud_debug_tcp_init();
				}
				else
				{
					LOGI("close audio debug: %d\n", param);
					bk_aud_debug_tcp_deinit();
				}
				break;
#endif // CONFIG_AUD_INTF
			default:
				break;
		}
	}
	else
	{
		LOGI("cmd:0x%x not need to handle\n", cmd);
	}
}

static void demo_doorbell_udp_app_connected(void)
{
	//app_demo_softap_send_msg(DAP_APP_CONECTED, 0);
}

static void demo_doorbell_udp_app_disconnected(void)
{
	//app_demo_softap_send_msg(DAP_APP_DISCONECTED, 0);
}


static void demo_doorbell_udp_receiver(uint8_t *data, uint32_t len, struct sockaddr_in *demo_doorbell_remote)
{
	LOGD("demo_doorbell_udp_receiver\n");
#if (defined(CONFIG_CAMERA) || defined(CONFIG_USB_UVC))
	GLOBAL_INT_DECLARATION();

	uint32_t ppi = PPI_DEFAULT;


	if (len < 2)
	{
		return;
	}

	if (data[0] == DOORBELL_CMD_IMG_HEADER)
	{

		switch (data[1])
		{
			case DOORBELL_DVP_START:
			{
				if (len >= 6)
				{
					ppi = data[2] << 24 | data[3] << 16 | data[4] << 8 | data[5];
				}

				LOGI("DVP START: %dX%d\n", ppi >> 16, ppi & 0xFFFF);


				uint8_t *src_ipaddr = (uint8_t *)&demo_doorbell_remote->sin_addr.s_addr;
				LOGI("src_ipaddr: %d.%d.%d.%d\n", src_ipaddr[0], src_ipaddr[1],
					src_ipaddr[2], src_ipaddr[3]);
				LOGI("udp connect to new port:%d\n", demo_doorbell_remote->sin_port);

				GLOBAL_INT_DISABLE();
				demo_doorbell_udp_romote_connected = 1;
				GLOBAL_INT_RESTORE();

#ifdef CONFIG_INTEGRATION_DOORBELL

				camera_config_t camera_config;

				os_memset(&camera_config, 0, sizeof(camera_config_t));

				camera_config.type = DVP_CAMERA;
				camera_config.image_format = IMAGE_MJPEG;
				camera_config.width = ppi >> 16;
				camera_config.height = ppi & 0xFFFF;

				media_app_camera_open(&camera_config);

				media_app_transfer_open(&doorbell_udp_callback);
#else
				video_setup_t setup;

				setup.open_type = TVIDEO_OPEN_SCCB;
				setup.send_type = TVIDEO_SND_UDP;
				setup.send_func = demo_doorbell_udp_send_packet;
				setup.start_cb = demo_doorbell_udp_app_connected;
				setup.end_cb = demo_doorbell_udp_app_disconnected;

				setup.pkt_header_size = sizeof(media_hdr_t);
				setup.add_pkt_header = demo_doorbell_add_pkt_header;

				media_app_camera_open(APP_CAMERA_DVP_JPEG, ppi);

				media_app_transfer_open(&setup);
#endif
			}
			break;

			case DOORBELL_DVP_STOP:
			{
				GLOBAL_INT_DISABLE();
				demo_doorbell_udp_romote_connected = 0;
				GLOBAL_INT_RESTORE();

				media_app_transfer_close();

				media_app_camera_close(APP_CAMERA_DVP_JPEG);

			}
			break;

			case DOORBELL_UVC_START:
			{
				if (len >= 6)
				{
					ppi = data[2] << 24 | data[3] << 16 | data[4] << 8 | data[5];
				}

				LOGI("UVC START: %dX%d\n", ppi >> 16, ppi & 0xFFFF);

				uint8_t *src_ipaddr = (uint8_t *)&demo_doorbell_remote->sin_addr.s_addr;
				LOGI("src_ipaddr: %d.%d.%d.%d\n", src_ipaddr[0], src_ipaddr[1],
				     src_ipaddr[2], src_ipaddr[3]);
				LOGI("udp connect to new port:%d\n", demo_doorbell_remote->sin_port);

				GLOBAL_INT_DISABLE();
				demo_doorbell_udp_romote_connected = 1;
				GLOBAL_INT_RESTORE();

#ifdef CONFIG_INTEGRATION_DOORBELL
				//if (fmt == DOORBELL_IMG_FMT_MJPEG)
				camera_config_t camera_config;

				os_memset(&camera_config, 0, sizeof(camera_config_t));

				camera_config.type = UVC_CAMERA;
				camera_config.image_format = IMAGE_MJPEG;
				camera_config.width = ppi >> 16;
				camera_config.height = ppi & 0xFFFF;

				media_app_camera_open(&camera_config);

				media_app_transfer_open(&doorbell_udp_callback);
#else
				video_setup_t setup;

				setup.open_type = TVIDEO_OPEN_SCCB;
				setup.send_type = TVIDEO_SND_UDP;
				setup.send_func = demo_doorbell_udp_send_packet;
				setup.start_cb = demo_doorbell_udp_app_connected;
				setup.end_cb = demo_doorbell_udp_app_disconnected;

				setup.pkt_header_size = sizeof(media_hdr_t);
				setup.add_pkt_header = demo_doorbell_add_pkt_header;

				media_app_camera_open(APP_CAMERA_UVC_MJPEG, ppi);
				media_app_transfer_open(&setup);
#endif
			}
			break;

			case DOORBELL_UVC_STOP:
			{
				GLOBAL_INT_DISABLE();
				demo_doorbell_udp_romote_connected = 0;
				GLOBAL_INT_RESTORE();

				media_app_transfer_close();
				media_app_camera_close(APP_CAMERA_UVC_MJPEG);
			}
			break;
		}
	}

#endif
}

#if DEMO_DOORBELL_EN_VOICE_TRANSFER
static void demo_doorbell_udp_voice_receiver(uint8_t *data, uint32_t len, struct sockaddr_in *udp_remote)
{
	LOGD("demo_doorbell_udp_voice_receiver, data len=%d\n", len);
#if AUDIO_TRANSFER_ENABLE
	bk_err_t ret = BK_OK;

	if (len > 0)
	{
		demo_doorbell_udp_voice_romote_connected = 1;
	}

	//DBD("len: %d \n", len);
	ret = bk_aud_intf_write_spk_data(data, len);
	if (ret != BK_OK)
	{
		LOGE("write speaker data fail\n", len);
	}
#endif  //AUDIO_TRANSFER_ENABLE

}
#endif // DEMO_DOORBELL_EN_VOICE_TRANSFER

static void demo_doorbell_udp_main(beken_thread_arg_t data)
{
	GLOBAL_INT_DECLARATION();
	int maxfd, udp_cmd_fd = -1, ret = 0;
	int rcv_len = 0;
	struct sockaddr_in cmd_remote;
	socklen_t srvaddr_len = 0;
	fd_set watchfd;
	struct timeval timeout;
	u8 *rcv_buf = NULL;

	LOGI("demo_doorbell_udp_main entry\n");
	(void)(data);

	rcv_buf = (u8 *) os_malloc((APP_DEMO_UDP_RCV_BUF_LEN + 1) * sizeof(u8));
	if (!rcv_buf)
	{
		LOGE("udp os_malloc failed\n");
		goto out;
	}

	demo_doorbell_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (!demo_doorbell_remote)
	{
		LOGE("udp os_malloc failed\n");
		goto out;
	}

	// for data transfer
	demo_doorbell_udp_img_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (demo_doorbell_udp_img_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	demo_doorbell_remote->sin_family = AF_INET;
	demo_doorbell_remote->sin_port = htons(DEMO_DOORBELL_UDP_IMG_PORT);
	demo_doorbell_remote->sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(demo_doorbell_udp_img_fd, (struct sockaddr *)demo_doorbell_remote, srvaddr_len) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}

	//  for recv uart cmd
	udp_cmd_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_cmd_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	cmd_remote.sin_family = AF_INET;
	cmd_remote.sin_port = htons(DEMO_DOORBELL_UDP_CMD_PORT);
	cmd_remote.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(udp_cmd_fd, (struct sockaddr *)&cmd_remote, srvaddr_len) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}

	maxfd = (udp_cmd_fd > demo_doorbell_udp_img_fd) ? udp_cmd_fd : demo_doorbell_udp_img_fd;

	// for voice transfer
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
	demo_doorbell_voice_udp_remote = (struct sockaddr_in *)os_malloc(sizeof(struct sockaddr_in));
	if (!demo_doorbell_voice_udp_remote)
	{
		LOGE("udp os_malloc failed\n");
		goto out;
	}

	demo_doorbell_udp_voice_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (demo_doorbell_udp_voice_fd == -1)
	{
		LOGE("vo socket failed\n");
		goto out;
	}

	demo_doorbell_voice_udp_remote->sin_family = AF_INET;
	demo_doorbell_voice_udp_remote->sin_port = htons(DEMO_DOORBELL_UDP_VOICE_PORT);
	demo_doorbell_voice_udp_remote->sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(struct sockaddr_in);
	if (bind(demo_doorbell_udp_voice_fd, (struct sockaddr *)demo_doorbell_voice_udp_remote, srvaddr_len) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}
	maxfd = (maxfd > demo_doorbell_udp_voice_fd) ? maxfd : demo_doorbell_udp_voice_fd;
#endif // DEMO_DOORBELL_EN_VOICE_TRANSFER

	timeout.tv_sec = APP_DEMO_UDP_SOCKET_TIMEOUT / 1000;
	timeout.tv_usec = (APP_DEMO_UDP_SOCKET_TIMEOUT % 1000) * 1000;

#if CONFIG_DOORBELL_DEMO2
	gpio_dev_unmap(GPIO_2);
	bk_gpio_disable_input(GPIO_2);
	bk_gpio_enable_output(GPIO_2);
	bk_gpio_disable_pull(GPIO_2);
	bk_gpio_set_output_high(GPIO_2);
	// pull up gpio39, enable audio pa vol

	gpio_dev_unmap(GPIO_3);
	gpio_dev_unmap(GPIO_4);

	rtos_delay_milliseconds(15);
	gpio_dev_unmap(GPIO_14);
	gpio_dev_map(GPIO_14, GPIO_DEV_SDIO_HOST_CLK);
	bk_gpio_pull_up(GPIO_14);
	bk_gpio_set_capacity(GPIO_14, GPIO_DRIVER_CAPACITY_3);

	gpio_dev_unmap(GPIO_15);
	gpio_dev_map(GPIO_15, GPIO_DEV_SDIO_HOST_CMD);
	bk_gpio_pull_up(GPIO_15);
	bk_gpio_set_capacity(GPIO_15, GPIO_DRIVER_CAPACITY_3);

	gpio_dev_unmap(GPIO_16);
	gpio_dev_map(GPIO_16, GPIO_DEV_SDIO_HOST_DATA0);
	bk_gpio_pull_up(GPIO_16);
	bk_gpio_set_capacity(GPIO_16, GPIO_DRIVER_CAPACITY_3);

	gpio_dev_unmap(GPIO_17);
	gpio_dev_map(GPIO_17, GPIO_DEV_SDIO_HOST_DATA1);
	bk_gpio_pull_up(GPIO_17);
	bk_gpio_set_capacity(GPIO_17, GPIO_DRIVER_CAPACITY_3);

	gpio_dev_unmap(GPIO_18);
	gpio_dev_map(GPIO_18, GPIO_DEV_SDIO_HOST_DATA2);
	bk_gpio_pull_up(GPIO_18);
	bk_gpio_set_capacity(GPIO_18, GPIO_DRIVER_CAPACITY_3);

	gpio_dev_unmap(GPIO_19);
	gpio_dev_map(GPIO_19, GPIO_DEV_SDIO_HOST_DATA3);
	bk_gpio_pull_up(GPIO_19);
	bk_gpio_set_capacity(GPIO_19, GPIO_DRIVER_CAPACITY_3);
#endif

	GLOBAL_INT_DISABLE();
	demo_doorbell_udp_romote_connected = 0;
	demo_doorbell_udp_run = 1;
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
	demo_doorbell_udp_voice_romote_connected = 0;
#endif
	GLOBAL_INT_RESTORE();

	while (demo_doorbell_udp_run)
	{
		FD_ZERO(&watchfd);
		FD_SET(demo_doorbell_udp_img_fd, &watchfd);
		FD_SET(udp_cmd_fd, &watchfd);
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
		FD_SET(demo_doorbell_udp_voice_fd, &watchfd);
#endif

		//DBD("wait for select\n");
		ret = select(maxfd + 1, &watchfd, NULL, NULL, &timeout);
		if (ret < 0)
		{
			LOGE("select ret:%d\n", ret);
			break;
		}
		else
		{
			// is img fd, data transfer
			if (FD_ISSET(demo_doorbell_udp_img_fd, &watchfd))
			{
				rcv_len = recvfrom(demo_doorbell_udp_img_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
				                   (struct sockaddr *)demo_doorbell_remote, &srvaddr_len);

				if (rcv_len <= 0)
				{
					// close this socket
					LOGE("recv close fd:%d\n", demo_doorbell_udp_img_fd);
					break;
				}
				else
				{
					rcv_len = (rcv_len > APP_DEMO_UDP_RCV_BUF_LEN) ? APP_DEMO_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;

					LOGE("demo_doorbell_udp_receiver\n");
					demo_doorbell_udp_receiver(rcv_buf, rcv_len, demo_doorbell_remote);
				}
			}
			else if (FD_ISSET(udp_cmd_fd, &watchfd))
			{
				rcv_len = recvfrom(udp_cmd_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
				                   (struct sockaddr *)&cmd_remote, &srvaddr_len);

				if (rcv_len <= 0)
				{
					// close this socket
					LOGE("recv close fd:%d\n", udp_cmd_fd);
					break;
				}
				else
				{
					rcv_len = (rcv_len > APP_DEMO_UDP_RCV_BUF_LEN) ? APP_DEMO_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;

					demo_doorbell_udp_handle_cmd_data(rcv_buf, rcv_len);
				}
			}
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
			if (FD_ISSET(demo_doorbell_udp_voice_fd, &watchfd))
			{
				rcv_len = recvfrom(demo_doorbell_udp_voice_fd, rcv_buf, APP_DEMO_UDP_RCV_BUF_LEN, 0,
				                   (struct sockaddr *)demo_doorbell_voice_udp_remote, &srvaddr_len);

				if (rcv_len <= 0)
				{
					// close this socket
					LOGE("recv close fd:%d\n", demo_doorbell_udp_voice_fd);
					break;
				}
				else
				{
					rcv_len = (rcv_len > APP_DEMO_UDP_RCV_BUF_LEN) ? APP_DEMO_UDP_RCV_BUF_LEN : rcv_len;
					rcv_buf[rcv_len] = 0;
					demo_doorbell_udp_voice_receiver(rcv_buf, rcv_len, demo_doorbell_voice_udp_remote);
				}
			}
#endif // DEMO_DOORBELL_EN_VOICE_TRANSFER
		}
	}

out:

	LOGE("demo_doorbell_udp_main exit %d\n", demo_doorbell_udp_run);
	media_app_transfer_close();

#if (defined(CONFIG_CAMERA) || defined(CONFIG_USB_UVC))

	media_app_camera_close(APP_CAMERA_DVP_JPEG);

	media_app_camera_close(APP_CAMERA_UVC_MJPEG);
#endif

#if AUDIO_TRANSFER_ENABLE
	/* deinit audio transfer driver */
	bk_aud_intf_drv_deinit();
#endif

	if (rcv_buf)
	{
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

#if DEMO_DOORBELL_EN_VOICE_TRANSFER
	if (demo_doorbell_udp_voice_fd != -1)
	{
		close(demo_doorbell_udp_voice_fd);
		demo_doorbell_udp_voice_fd = -1;
	}

	if (demo_doorbell_voice_udp_remote)
	{
		os_free(demo_doorbell_voice_udp_remote);
		demo_doorbell_voice_udp_remote = NULL;
	}
#endif // DEMO_DOORBELL_EN_VOICE_TRANSFER
	if (demo_doorbell_remote)
	{
		os_free(demo_doorbell_remote);
		demo_doorbell_remote = NULL;
	}

	if (demo_doorbell_udp_img_fd != -1)
	{
		close(demo_doorbell_udp_img_fd);
		demo_doorbell_udp_img_fd = -1;
	}

	if (udp_cmd_fd != -1)
	{
		close(udp_cmd_fd);
		udp_cmd_fd = -1;
	}

	GLOBAL_INT_DISABLE();
	demo_doorbell_udp_romote_connected = 0;
	demo_doorbell_udp_run = 0;
#if DEMO_DOORBELL_EN_VOICE_TRANSFER
	demo_doorbell_udp_voice_romote_connected = 0;
#endif
	GLOBAL_INT_RESTORE();

	demo_doorbell_udp_hdl = NULL;
	rtos_delete_thread(NULL);
}

bk_err_t demo_doorbell_udp_init(void)
{
	int ret;

	LOGI("app_demo_udp_init\n");
	if (!demo_doorbell_udp_hdl)
	{
		ret = rtos_create_thread(&demo_doorbell_udp_hdl,
		                         4,
		                         "doorbell",
		                         (beken_thread_function_t)demo_doorbell_udp_main,
		                         1024 * 2,
		                         (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			LOGE("Error: Failed to create spidma_intfer: %d\n", ret);
			return kGeneralErr;
		}
	}

	return kNoErr;
}

void demo_doorbell_udp_deinit(void)
{
	GLOBAL_INT_DECLARATION();

	LOGI("app_demo_udp_deinit\n");
	if (demo_doorbell_udp_run == 0)
	{
		return;
	}

	GLOBAL_INT_DISABLE();
	demo_doorbell_udp_run = 0;
	GLOBAL_INT_RESTORE();

	while (demo_doorbell_udp_hdl)
	{
		rtos_delay_milliseconds(10);
	}
}

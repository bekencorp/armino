#include <common/bk_include.h>

#include "doorbell.h"

#include "lwip/tcp.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <os/os.h>
#include <common/bk_kernel_err.h>

#include "lwip/sockets.h"

#include <components/video_transfer.h>

#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#if CONFIG_DOORBELL_DEMO2
#include <driver/gpio.h>
#endif
#include "lcd_act.h"


#include "media_app.h"

#define TAG "doorbell-TCP"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


#define DEMO_DOORBELL_TCP_RCV_BUF_LEN               (1460)//1460

#define TCP_HEAD_SIZE                               (8)
#define TCP_HEAD_ID_HB                              (0xFF)
#define TCP_HEAD_ID_LB                              (0xD5)
#define HEAD_IND_HB                                 (0)
#define HEAD_IND_LB                                 (1)
#define SEQ_IND_HB                                  (2)
#define SEQ_IND_LB                                  (3)
#define CHNL_IND_HB                                 (4)
#define CHNL_IND_LB                                 (5)
#define LEN_IND_HB                                  (6)
#define LEN_IND_LB                                  (7)

#define TCP_CHANNEL_CMD                             (0)
#define TCP_CHANNEL_CAMERA                          (1)
#define TCP_CHANNEL_AUDIO                           (2)


#define TCP_HEAD_ID (TCP_HEAD_ID_HB << 8 | TCP_HEAD_ID_LB)

beken_thread_t demo_doorbell_tcp_task = NULL;

int demo_doorbell_watch_fd_list[DEMO_DOORBELL_TCP_LISTEN_MAX];
int demo_doorbell_tcp_server_fd;
volatile int demo_doorbell_tcp_running = 0;

uint8_t *tcp_cache_buffer = NULL;
uint16_t tcp_cache_count = 0;

uint8_t *video_tcp_send_buffer = NULL;
uint8_t *audio_tcp_send_buffer = NULL;

uint16_t tcp_sequence = 0;

#if AUDIO_TRANSFER_ENABLE
static aud_intf_drv_setup_t aud_intf_drv_setup;
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static aud_intf_voc_setup_t aud_voc_setup;
#endif

beken_mutex_t send_lock;

static void demo_doorbell_tcp_set_keepalive(int fd)
{
	int opt = 1, ret;
	// open tcp keepalive
	ret = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(int));

	opt = 30;  // 5 second
	ret = setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &opt, sizeof(int));

	opt = 1;  // 1s second for intval
	ret = setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &opt, sizeof(int));

	opt = 3;  // 3 times
	ret = setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &opt, sizeof(int));
	ret = ret;
}

int demo_doorbell_tcp_video_send_packet(uint8_t *data, uint32_t len)
{
	int i = 0, snd_len = 0;

	if ((!demo_doorbell_tcp_task) || (demo_doorbell_tcp_server_fd == -1))
	{
		return 0;
	}

	tcp_sequence++;

	video_tcp_send_buffer[0] = TCP_HEAD_ID_HB;
	video_tcp_send_buffer[1] = TCP_HEAD_ID_LB;
	video_tcp_send_buffer[2] = (tcp_sequence >> 8) & 0xFF;
	video_tcp_send_buffer[3] = (tcp_sequence >> 0) & 0xFF;
	video_tcp_send_buffer[4] = (TCP_CHANNEL_CAMERA >> 8) & 0xFF;
	video_tcp_send_buffer[5] = (TCP_CHANNEL_CAMERA >> 0) & 0xFF;
	video_tcp_send_buffer[6] = (len >> 8) & 0xFF;
	video_tcp_send_buffer[7] = (len >> 0) & 0xFF;

	os_memcpy(video_tcp_send_buffer + TCP_HEAD_SIZE, data, len);

	//LOGI("sequence: %u, length: %u\n", tcp_sequence, len);

	for (i = 0; i < DEMO_DOORBELL_TCP_LISTEN_MAX; i++)
	{
		if (demo_doorbell_watch_fd_list[i] == -1)
		{
			continue;
		}

		rtos_lock_mutex(&send_lock);
		snd_len = write(demo_doorbell_watch_fd_list[i], video_tcp_send_buffer, TCP_HEAD_SIZE + len);
		rtos_unlock_mutex(&send_lock);
		if (snd_len < 0)
		{
			/* err */
			//APP_DEMO_TCP_PRT("send return fd:%d\r\n", snd_len);
			snd_len = 0;
		}
	}

	return snd_len - TCP_HEAD_SIZE;
}

int demo_doorbell_tcp_audio_send_packet(unsigned char *data, unsigned int len)
{
	int i = 0, snd_len = 0;

	if ((!demo_doorbell_tcp_task) || (demo_doorbell_tcp_server_fd == -1))
	{
		return 0;
	}

	tcp_sequence++;

	audio_tcp_send_buffer[0] = TCP_HEAD_ID_HB;
	audio_tcp_send_buffer[1] = TCP_HEAD_ID_LB;
	audio_tcp_send_buffer[2] = (tcp_sequence >> 8) & 0xFF;
	audio_tcp_send_buffer[3] = (tcp_sequence >> 0) & 0xFF;
	audio_tcp_send_buffer[4] = (TCP_CHANNEL_AUDIO >> 8) & 0xFF;
	audio_tcp_send_buffer[5] = (TCP_CHANNEL_AUDIO >> 0) & 0xFF;
	audio_tcp_send_buffer[6] = (len >> 8) & 0xFF;
	audio_tcp_send_buffer[7] = (len >> 0) & 0xFF;

	os_memcpy(audio_tcp_send_buffer + TCP_HEAD_SIZE, data, len);

	//LOGI("sequence: %u, length: %u\n", tcp_sequence, len);

	for (i = 0; i < DEMO_DOORBELL_TCP_LISTEN_MAX; i++)
	{
		if (demo_doorbell_watch_fd_list[i] == -1)
		{
			continue;
		}

		rtos_lock_mutex(&send_lock);
		snd_len = write(demo_doorbell_watch_fd_list[i], audio_tcp_send_buffer, TCP_HEAD_SIZE + len);
		rtos_unlock_mutex(&send_lock);
		if (snd_len < 0)
		{
			/* err */
			//APP_DEMO_TCP_PRT("send return fd:%d\r\n", snd_len);
			snd_len = 0;
		}
	}

	return snd_len - TCP_HEAD_SIZE;
}


void demo_doorbell_dump(uint8_t *data, uint32_t length)
{
	for (int i = 0; i < length; i++)
	{
		os_printf("%02X ", data[i]);

		if ((i + 1) % 20 == 0)
		{
			os_printf("\n");
		}
	}
	os_printf("\n");
}



static void demo_doorbell_tcp_camera_data_handle(uint8_t *data, uint16_t length)
{
	if (data[0] == DOORBELL_CMD_IMG_HEADER)
	{

		switch (data[1])
		{
			case DOORBELL_DVP_START:
			{
				uint32_t ppi = PPI_DEFAULT;

				if (length >= 6)
				{
					ppi = data[2] << 24 | data[3] << 16 | data[4] << 8 | data[5];
				}

				LOGI("DVP START: %dX%d\n", ppi >> 16, ppi & 0xFFFF);

				video_setup_t setup;

				setup.open_type = TVIDEO_OPEN_SCCB;
				setup.send_type = TVIDEO_SND_UDP;
				setup.send_func = demo_doorbell_tcp_video_send_packet;

				media_app_camera_open(APP_CAMERA_DVP, ppi);
				media_app_transfer_open(&setup);
			}
			break;

			case DOORBELL_DVP_STOP:
			{
				LOGI("DVP STOP\n");

				media_app_transfer_close();
				media_app_camera_close(APP_CAMERA_DVP);

			}
			break;

			case DOORBELL_UVC_START:
			{
				uint32_t ppi = PPI_DEFAULT;

				if (length >= 6)
				{
					ppi = data[2] << 24 | data[3] << 16 | data[4] << 8 | data[5];
				}

				LOGI("DVP START: %dX%d\n", ppi >> 16, ppi & 0xFFFF);

				video_setup_t setup;

				setup.open_type = TVIDEO_OPEN_SCCB;
				setup.send_type = TVIDEO_SND_UDP;
				setup.send_func = demo_doorbell_tcp_video_send_packet;

				media_app_camera_open(APP_CAMERA_UVC, ppi);
				media_app_transfer_open(&setup);
			}
			break;

			case DOORBELL_UVC_STOP:
			{
				LOGI("UVC STOP\n");

				media_app_transfer_close();
				media_app_camera_close(APP_CAMERA_UVC);
			}
			break;
		}
	}
}

static void demo_doorbell_tcp_cmd_data_handle(uint8_t *data, uint16_t length)
{
	bk_err_t ret = BK_ERR_AUD_INTF_OK;
	uint32_t param = 0;
	uint32_t cmd = (uint32_t)data[0] << 24 | (uint32_t)data[1] << 16 | (uint32_t)data[2] << 8 | data[3];
	if (length >= 8)
	{
		param = (uint32_t)data[4] << 24 | (uint32_t)data[5] << 16 | (uint32_t)data[6] << 8 | data[7];
	}

	LOGI("doorbell cmd: %08X, param: %d, len: %d\n", cmd, param, length);

	if (length >= 8)
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
				aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
				aud_intf_drv_setup.task_config.priority = 3;
				aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
				aud_intf_drv_setup.aud_intf_tx_mic_data = demo_doorbell_tcp_audio_send_packet;
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
				aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_G711A;
				//aud_voc_setup.data_type = AUD_INTF_VOC_DATA_TYPE_PCM;
				aud_voc_setup.spk_mode = AUD_DAC_WORK_MODE_SIGNAL_END;
				aud_voc_setup.mic_en = AUD_INTF_VOC_MIC_OPEN;
				aud_voc_setup.spk_en = AUD_INTF_VOC_SPK_OPEN;
				if (data[8] == 1) {
					aud_voc_setup.mic_type = AUD_INTF_MIC_TYPE_UAC;
					aud_voc_setup.spk_type = AUD_INTF_SPK_TYPE_UAC;
					aud_voc_setup.samp_rate = AUD_INTF_VOC_SAMP_RATE_16K;
				} else {
					aud_voc_setup.mic_type = AUD_INTF_MIC_TYPE_BOARD;
					aud_voc_setup.spk_type = AUD_INTF_SPK_TYPE_BOARD;
					aud_voc_setup.samp_rate = AUD_INTF_VOC_SAMP_RATE_8K;
				}
				aud_voc_setup.aec_cfg.ref_scale = 1;
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
				break;
#endif  //AUDIO_TRANSFER_ENABLE

			case DISPLAY_CLOSE:
				LOGI("LCD CLOSE\n");
				media_app_lcd_close();
				break;

			case DISPLAY_OPEN:
				LOGI("LCD OPEN: %dX%d\n", param >> 16, param & 0xFFFF);
				lcd_open_t lcd_open;
				lcd_open.device_ppi = param;
				media_app_lcd_open(&lcd_open);
				break;

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

			default:
				break;
		}
	}
	else
	{
		LOGI("cmd:0x%x not need to handle\n", cmd);
	}

}

static void demo_doorbell_tcp_voice_data_handle(uint8_t *data, uint32_t len)
{
	LOGD("demo_doorbell_udp_voice_receiver, data len=%d\n", len);
#if AUDIO_TRANSFER_ENABLE
	bk_err_t ret = BK_OK;

	if (len > 0)
	{
		//demo_doorbell_udp_voice_romote_connected = 1;
	}

	//DBD("len: %d \n", len);
	ret = bk_aud_intf_write_spk_data(data, len);
	if (ret != BK_OK)
	{
		LOGE("write speaker data fial\n", len);
	}
#endif  //AUDIO_TRANSFER_ENABLE

}

static void demo_doorbell_tcp_channel_data_handle(uint16_t sequence, uint16_t channel, uint8_t *data, uint16_t length)
{
	LOGD("%d, tpye: %d, size: %d\n", sequence, channel, length);

	switch (channel)
	{
		case TCP_CHANNEL_CMD:
		{
			demo_doorbell_tcp_cmd_data_handle(data, length);
		}
		break;

		case TCP_CHANNEL_CAMERA:
		{
			//demo_doorbell_dump(data, length);
			demo_doorbell_tcp_camera_data_handle(data, length);
		}
		break;

		case TCP_CHANNEL_AUDIO:
		{
			demo_doorbell_tcp_voice_data_handle(data, length);
		}
		break;


	}
}

uint8_t *find_tcp_head(uint8_t *data, uint32_t length)
{
	uint8_t *ret = NULL;

	for (uint32_t i = 0; i < length - 1; i++)
	{
		if (data[i] == TCP_HEAD_ID_HB
		    && data[i + 1] == TCP_HEAD_ID_LB)
		{
			ret = &data[i];
		}
	}

	return ret;
}

void demo_doorbell_tcp_data_recv_handle(uint8_t *data, uint32_t length)
{
	static uint32_t count = 0;

	LOGD("DUMP DATA %u, size: %u\n", count++, length);

	//demo_doorbell_dump(data, length);

	uint16_t channel_id = 0, channel_len = 0, head = 0, sequence = 0;
	uint8_t *ptr = data;
	uint32_t left = length;

	while (left != 0)
	{
		if (tcp_cache_count == 0)
		{
			if (left < TCP_HEAD_SIZE)
			{
				os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, left);
				tcp_cache_count += left;
				break;
			}

			head = ptr[HEAD_IND_HB] << 8 | ptr[HEAD_IND_LB];

			if (head == TCP_HEAD_ID)
			{
				channel_id = ptr[CHNL_IND_HB] << 8 | ptr[CHNL_IND_LB];
				channel_len = ptr[LEN_IND_HB] << 8 | ptr[LEN_IND_LB];
				sequence = ptr[SEQ_IND_HB] << 8 | ptr[SEQ_IND_LB];
			}
			else
			{
				LOGE("invaild src data\n");
				demo_doorbell_dump(ptr, left);
				LOGE("dump src data\n");
				//TODO FIXME
				demo_doorbell_dump(data, length);
				break;
			}

			if (left < channel_len + TCP_HEAD_SIZE)
			{
				LOGI("data not enough, left: %d\n", left);
				os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, left);
				tcp_cache_count += left;
				//LOGI("DUMP FULL DATA\n");
				//demo_doorbell_dump(data, length);
				//LOGI("DUMP LEFT DATA\n");
				//demo_doorbell_dump(ptr, left);
				break;
			}

			demo_doorbell_tcp_channel_data_handle(sequence, channel_id, ptr + TCP_HEAD_SIZE, channel_len);

			ptr += TCP_HEAD_SIZE + channel_len;
			left -= TCP_HEAD_SIZE + channel_len;
		}
		else
		{
			if (tcp_cache_count < TCP_HEAD_SIZE)
			{
				if (left < TCP_HEAD_SIZE)
				{
					os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, left);
					tcp_cache_count += left;
					left = 0;
					break;
				}
				else
				{
					os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, TCP_HEAD_SIZE - tcp_cache_count);
					tcp_cache_count += TCP_HEAD_SIZE - tcp_cache_count;
					ptr += TCP_HEAD_SIZE - tcp_cache_count;
					left -= TCP_HEAD_SIZE - tcp_cache_count;
				}
			}

			head = tcp_cache_buffer[HEAD_IND_HB] << 8 | tcp_cache_buffer[HEAD_IND_LB];

			if (head == TCP_HEAD_ID)
			{
				channel_id = tcp_cache_buffer[CHNL_IND_HB] << 8 | tcp_cache_buffer[CHNL_IND_LB];
				channel_len = tcp_cache_buffer[LEN_IND_HB] << 8 | tcp_cache_buffer[LEN_IND_LB];
				sequence = tcp_cache_buffer[SEQ_IND_HB] << 8 | tcp_cache_buffer[SEQ_IND_LB];
			}
			else
			{
				LOGE("invaild cached data, %04X\n", head);
				demo_doorbell_dump(tcp_cache_buffer, tcp_cache_count);
				//TODO FIXME
				break;
			}

			if (tcp_cache_count < TCP_HEAD_SIZE + channel_len)
			{
				if (left + tcp_cache_count < channel_len + TCP_HEAD_SIZE)
				{
					os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, left);
					tcp_cache_count += left;
					left = 0;
					break;
				}

				os_memcpy(tcp_cache_buffer + tcp_cache_count, ptr, channel_len + TCP_HEAD_SIZE - tcp_cache_count);
				left -= channel_len + TCP_HEAD_SIZE - tcp_cache_count;
				ptr += channel_len + TCP_HEAD_SIZE - tcp_cache_count;
				tcp_cache_count += channel_len + TCP_HEAD_SIZE - tcp_cache_count;

				demo_doorbell_tcp_channel_data_handle(sequence, channel_id, tcp_cache_buffer + TCP_HEAD_SIZE, channel_len);

				LOGD("cached: %d, left: %d\n", tcp_cache_count, left);

				tcp_cache_count = 0;
			}
			else
			{
				LOGE("invaild flow data\n");
				demo_doorbell_dump(tcp_cache_buffer, tcp_cache_count);
				//SHOULD NOT BE HERE
				//TODO FIMXME
				break;
			}
		}
	}
}

static void demo_doorbell_tcp_main(beken_thread_arg_t data)
{
	GLOBAL_INT_DECLARATION();
	int maxfd = -1;
	int ret = 0, i = 0;
	int rcv_len = 0;
	struct sockaddr_in server_addr;
	socklen_t srvaddr_len = 0;
	fd_set watchfd;
	u8 *rcv_buf = NULL;

	(void)(data);

	LOGI("%s entry\n", __func__);

	rcv_buf = (u8 *) os_malloc((DEMO_DOORBELL_TCP_RCV_BUF_LEN + 1) * sizeof(u8));
	if (!rcv_buf)
	{
		LOGE("tcp os_malloc failed\n");
		goto out;
	}

	demo_doorbell_tcp_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (demo_doorbell_tcp_server_fd == -1)
	{
		LOGE("socket failed\n");
		goto out;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(DEMO_DOORBELL_TCP_SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	srvaddr_len = (socklen_t)sizeof(server_addr);
	if (bind(demo_doorbell_tcp_server_fd, (struct sockaddr *)&server_addr, srvaddr_len) == -1)
	{
		LOGE("bind failed\n");
		goto out;
	}

	if (listen(demo_doorbell_tcp_server_fd, DEMO_DOORBELL_TCP_LISTEN_MAX) == -1)
	{
		LOGE("listen failed\n");
		goto out;
	}

	maxfd = demo_doorbell_tcp_server_fd;

	for (i = 0; i < DEMO_DOORBELL_TCP_LISTEN_MAX; i++)
	{
		demo_doorbell_watch_fd_list[i] = -1;
	}

	GLOBAL_INT_DISABLE();
	demo_doorbell_tcp_running = 1;
	GLOBAL_INT_RESTORE();

	LOGI("%s start\n", __func__);

	while (demo_doorbell_tcp_running)
	{
		LOGD("set fd\n");

		FD_ZERO(&watchfd);
		FD_SET(demo_doorbell_tcp_server_fd, &watchfd);

		for (i = 0; i < DEMO_DOORBELL_TCP_LISTEN_MAX; i++)
		{
			if (demo_doorbell_watch_fd_list[i] != -1)
			{
				FD_SET(demo_doorbell_watch_fd_list[i], &watchfd);
				if (maxfd < demo_doorbell_watch_fd_list[i])
				{
					maxfd = demo_doorbell_watch_fd_list[i];
				}
			}
		}

		LOGD("select fd\n");
		ret = select(maxfd + 1, &watchfd, NULL, NULL, NULL);
		if (ret <= 0)
		{
			LOGE("select ret:%d\n", ret);
			break;
		}
		else
		{
			// is new connection
			if (FD_ISSET(demo_doorbell_tcp_server_fd, &watchfd))
			{
				int new_cli_sockfd = -1;
				struct sockaddr_in client_addr;
				socklen_t cliaddr_len = 0;

				cliaddr_len = sizeof(client_addr);
				new_cli_sockfd = accept(demo_doorbell_tcp_server_fd, (struct sockaddr *)&client_addr, &cliaddr_len);
				if (new_cli_sockfd < 0)
				{
					LOGE("accept return fd:%d\n", new_cli_sockfd);
					break;
				}

				LOGI("new accept fd:%d\n", new_cli_sockfd);

				tcp_cache_count = 0;


				for (i = 0; i < DEMO_DOORBELL_TCP_LISTEN_MAX; i++)
				{
					if (demo_doorbell_watch_fd_list[i] == -1)
					{
						demo_doorbell_watch_fd_list[i] = new_cli_sockfd;

						demo_doorbell_tcp_set_keepalive(new_cli_sockfd);

						//TODO

						break;
					}
				}

				if (i == DEMO_DOORBELL_TCP_LISTEN_MAX)
				{
					LOGW("only accept %d clients\n", DEMO_DOORBELL_TCP_LISTEN_MAX);
					close(new_cli_sockfd);
				}
			}

			// search those added fd
			for (i = 0; i < DEMO_DOORBELL_TCP_LISTEN_MAX; i++)
			{
				if (demo_doorbell_watch_fd_list[i] == -1)
				{
					continue;
				}
				if (!FD_ISSET(demo_doorbell_watch_fd_list[i], &watchfd))
				{
					continue;
				}

				rcv_len = recv(demo_doorbell_watch_fd_list[i], rcv_buf, DEMO_DOORBELL_TCP_RCV_BUF_LEN, 0);

				FD_CLR(demo_doorbell_watch_fd_list[i], &watchfd);

				if (rcv_len <= 0)
				{
					// close this socket
					LOGI("recv close fd:%d, rcv_len:%d\n", demo_doorbell_watch_fd_list[i], rcv_len);
					close(demo_doorbell_watch_fd_list[i]);
					demo_doorbell_watch_fd_list[i] = -1;
				}
				else
				{
					//LOGI("read count: %d\n", rcv_len);
					demo_doorbell_tcp_data_recv_handle(rcv_buf, rcv_len);
				}

			}
		}// ret = select
	}

out:

	LOGE("app_demo_tcp_main exit\n");

	//TODO CLEANUP

	if (rcv_buf)
	{
		os_free(rcv_buf);
		rcv_buf = NULL;
	}

	for (i = 0; i < DEMO_DOORBELL_TCP_LISTEN_MAX; i++)
	{
		if (demo_doorbell_watch_fd_list[i] != -1)
		{
			close(demo_doorbell_watch_fd_list[i]);
			demo_doorbell_watch_fd_list[i] = -1;
		}
	}

	if (demo_doorbell_tcp_server_fd != -1)
	{
		close(demo_doorbell_tcp_server_fd);
		demo_doorbell_tcp_server_fd = -1;
	}

	if (video_tcp_send_buffer)
	{
		os_free(video_tcp_send_buffer);
		video_tcp_send_buffer = NULL;
	}

	if (audio_tcp_send_buffer)
	{
		os_free(audio_tcp_send_buffer);
		audio_tcp_send_buffer = NULL;
	}

	rtos_deinit_mutex(&send_lock);

	GLOBAL_INT_DISABLE();
	demo_doorbell_tcp_running = 0;
	GLOBAL_INT_RESTORE();

	demo_doorbell_tcp_task = NULL;
	rtos_delete_thread(NULL);
}

bk_err_t demo_doorbell_tcp_init(void)
{
	int ret;

	LOGI("%s\n", __func__);

	if (demo_doorbell_tcp_task)
	{
		LOGE("%s already init\n", __func__);
		ret = BK_OK;
		return ret;
	}

	if (tcp_cache_buffer == NULL)
	{
		LOGI("malloc tcp_cache_buffer\n");
		tcp_cache_buffer = (uint8_t *)os_malloc(1600);

		if (tcp_cache_buffer == NULL)
		{
			LOGE("malloc tcp_cache_buffer failed\n");
		}
	}

	if (video_tcp_send_buffer == NULL)
	{
		video_tcp_send_buffer = (uint8_t *)os_malloc(TCP_HEAD_SIZE + 1472);
	}

	if (audio_tcp_send_buffer == NULL)
	{
		audio_tcp_send_buffer = (uint8_t *)os_malloc(TCP_HEAD_SIZE + 1472);
	}

	rtos_init_mutex(&send_lock);

	if (!demo_doorbell_tcp_task)
	{
		ret = rtos_create_thread(&demo_doorbell_tcp_task,
		                         4,
		                         "demo_doorbell_tcp",
		                         (beken_thread_function_t)demo_doorbell_tcp_main,
		                         1024 * 6,
		                         (beken_thread_arg_t)NULL);
		if (ret != kNoErr)
		{
			LOGE("Error: Failed to create spidma_intfer: %d\n", ret);
			return kGeneralErr;
		}
	}

	return kNoErr;
}

void demo_doorbell_tcp_deinit(void)
{
	GLOBAL_INT_DECLARATION();

	LOGI("app_demo_tcp_deinit\n");

	if (demo_doorbell_tcp_running == 0)
	{
		return;
	}

	GLOBAL_INT_DISABLE();
	demo_doorbell_tcp_running = 0;
	GLOBAL_INT_RESTORE();
}


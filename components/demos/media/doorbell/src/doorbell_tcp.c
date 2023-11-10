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
#include <modules/audio_ring_buff.h>
//#include "BK7256_RegList.h"
#include "media_app.h"
#include <driver/timer.h>

#if (CONFIG_AUD_INTF)
#include "aud_tras.h"
#endif

#if (CONFIG_SOC_BK7236XX)
#include "aud_tras.h"
#endif


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
static aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;
static aud_intf_voc_setup_t aud_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();
static bool aud_tras_working = false;
static uint8 *aud_data_send_buff = NULL;
static RingBufferContext aud_data_rb;
static uint8_t *audio_data_temp = NULL;
#define AUD_DELAY_FRAME    (20)    //delay (20*N)ms
static uint32_t frame_size = 0;

static RingBufferContext aud_tras_rb;	//save mic data needed to send by aud_tras task
static uint8_t *aud_tras_buff_addr = NULL;
static aud_tras_setup_t aud_tras_setup;
extern beken_queue_t aud_tras_int_msg_que;
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

static int demo_doorbell_tcp_video_send_audio_packet(uint8_t *data, uint32_t len)
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
		//addAON_GPIO_Reg0x3 = 2;
		snd_len = write(demo_doorbell_watch_fd_list[i], audio_tcp_send_buffer, TCP_HEAD_SIZE + len);
		//addAON_GPIO_Reg0x3 = 0;
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

int demo_doorbell_tcp_video_send_packet(uint8_t *data, uint32_t len)
{
	int i = 0, snd_len = 0;

	if ((!demo_doorbell_tcp_task) || (demo_doorbell_tcp_server_fd == -1))
	{
		return 0;
	}

#if AUDIO_TRANSFER_ENABLE
	uint32_t fill_size = 0;

	GLOBAL_INT_DECLARATION();

	if (aud_tras_working) {
		fill_size = ring_buffer_get_fill_size(&aud_data_rb);
		if (fill_size) {
			GLOBAL_INT_DISABLE();
			ring_buffer_read(&aud_data_rb, audio_data_temp, fill_size);
			demo_doorbell_tcp_video_send_audio_packet(audio_data_temp, fill_size);
			GLOBAL_INT_RESTORE();
			bk_timer_stop(TIMER_ID5);
			//os_printf("video send audio data: %d \n", fill_size);
		}
	}
#endif

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
		//addAON_GPIO_Reg0x2 = 2;
		snd_len = write(demo_doorbell_watch_fd_list[i], video_tcp_send_buffer, TCP_HEAD_SIZE + len);
		//addAON_GPIO_Reg0x2 = 0;
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

static void demo_doorbell_aud_data_handle(timer_id_t timer_id)
{
#if AUDIO_TRANSFER_ENABLE

	GLOBAL_INT_DECLARATION();

	//uint32_t fill_size = 0;
	//fill_size = ring_buffer_get_fill_size(&aud_data_rb);
	GLOBAL_INT_DISABLE();
	ring_buffer_clear(&aud_data_rb);
	GLOBAL_INT_RESTORE();
	bk_timer_stop(TIMER_ID5);
	//os_printf("aud lost: %d \n", fill_size);
#endif
}

int demo_doorbell_tcp_audio_send_packet(unsigned char *data, unsigned int len)
{
#if AUDIO_TRANSFER_ENABLE

	int i = 0, snd_len = 0;
	bk_err_t ret = BK_OK;
	uint32_t fill_size = 0;
	GLOBAL_INT_DECLARATION();

	if ((!demo_doorbell_tcp_task) || (demo_doorbell_tcp_server_fd == -1))
	{
		return 0;
	}

	ret = rtos_trylock_mutex(&send_lock);
	if (ret != kNoErr) {
		/* write audio data to ring buffer */
		if (ring_buffer_get_free_size(&aud_data_rb) >= len) {
			//os_printf("write data to aud_data_rb \n");
			GLOBAL_INT_DISABLE();
			fill_size = ring_buffer_get_fill_size(&aud_data_rb);
			ring_buffer_write(&aud_data_rb, data, len);
			GLOBAL_INT_RESTORE();
			//return len;

			if (fill_size == 0) {
				ret = bk_timer_start(TIMER_ID5, 400, demo_doorbell_aud_data_handle);
				if (ret != BK_OK)
					LOGE("start audio data timer fail \r\n");
			}
			return len;
		} else {
			//os_printf("aud_data_rb is full \n");
			return 0;
		}
	}

	//rtos_lock_mutex(&send_lock);

	tcp_sequence++;

	audio_tcp_send_buffer[0] = TCP_HEAD_ID_HB;
	audio_tcp_send_buffer[1] = TCP_HEAD_ID_LB;
	audio_tcp_send_buffer[2] = (tcp_sequence >> 8) & 0xFF;
	audio_tcp_send_buffer[3] = (tcp_sequence >> 0) & 0xFF;
	audio_tcp_send_buffer[4] = (TCP_CHANNEL_AUDIO >> 8) & 0xFF;
	audio_tcp_send_buffer[5] = (TCP_CHANNEL_AUDIO >> 0) & 0xFF;
	fill_size = ring_buffer_get_fill_size(&aud_data_rb);
	len += fill_size;
	audio_tcp_send_buffer[6] = (len >> 8) & 0xFF;
	audio_tcp_send_buffer[7] = (len >> 0) & 0xFF;

	if (fill_size) {
		GLOBAL_INT_DISABLE();
		ring_buffer_read(&aud_data_rb, audio_data_temp, fill_size);
		GLOBAL_INT_RESTORE();
		os_memcpy(audio_tcp_send_buffer + TCP_HEAD_SIZE, audio_data_temp, fill_size);
		os_memcpy(audio_tcp_send_buffer + TCP_HEAD_SIZE + fill_size, data, len - fill_size);
	} else {
		os_memcpy(audio_tcp_send_buffer + TCP_HEAD_SIZE, data, len - fill_size);
	}
	//LOGI("sequence: %u, length: %u\n", tcp_sequence, len);

	for (i = 0; i < DEMO_DOORBELL_TCP_LISTEN_MAX; i++)
	{
		if (demo_doorbell_watch_fd_list[i] == -1)
		{
			continue;
		}

		//rtos_lock_mutex(&send_lock);
		//addAON_GPIO_Reg0x3 = 2;
		snd_len = write(demo_doorbell_watch_fd_list[i], audio_tcp_send_buffer, TCP_HEAD_SIZE + len);
		//addAON_GPIO_Reg0x3 = 0;
		//rtos_unlock_mutex(&send_lock);
		if (snd_len < 0)
		{
			/* err */
			//APP_DEMO_TCP_PRT("send return fd:%d\r\n", snd_len);
			snd_len = 0;
		}
	}
	rtos_unlock_mutex(&send_lock);

	return snd_len - TCP_HEAD_SIZE;
#else
	return len;
#endif
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
		uint32_t ppi = PPI_DEFAULT;
		uint8_t fmt = DOORBELL_IMG_FMT_MJPEG;

		switch (data[1])
		{
			case DOORBELL_DVP_START:
			{
				if (length >= 6)
				{
					ppi = data[2] << 24 | data[3] << 16 | data[4] << 8 | data[5];
				}

				if (length >= 7)
				{
					fmt = data[6];
				}

				LOGI("DVP START: %dX%d\n", ppi >> 16, ppi & 0xFFFF);

				video_setup_t setup;

				setup.open_type = TVIDEO_OPEN_SCCB;
				setup.send_type = TVIDEO_SND_UDP;
				setup.send_func = demo_doorbell_tcp_video_send_packet;

				if (fmt == DOORBELL_IMG_FMT_MJPEG) {
					setup.open_type = APP_CAMERA_DVP_JPEG;
					media_app_camera_open(APP_CAMERA_DVP_JPEG, ppi);
				}
				else
				{
					setup.open_type = APP_CAMERA_DVP_H264_ENC_LCD;
					media_app_camera_open(APP_CAMERA_DVP_H264_WIFI_TRANSFER, ppi);
				}

				media_app_transfer_open(&setup);
			}
			break;

			case DOORBELL_DVP_STOP:
			{
				LOGI("DVP STOP\n");

				if (length >= 7)
				{
					fmt = data[6];
				}

				media_app_transfer_close();

				if (fmt == DOORBELL_IMG_FMT_MJPEG) {
					media_app_camera_close(APP_CAMERA_DVP_JPEG);
				} else {
					media_app_camera_close(APP_CAMERA_DVP_H264_WIFI_TRANSFER);
				}

			}
			break;

			case DOORBELL_UVC_START:
			{
				if (length >= 6)
				{
					ppi = data[2] << 24 | data[3] << 16 | data[4] << 8 | data[5];
				}

				if (length >= 7)
				{
					fmt = data[6];
				}

				LOGI("DVP START: %dX%d\n", ppi >> 16, ppi & 0xFFFF);

				video_setup_t setup;

				setup.open_type = TVIDEO_OPEN_SCCB;
				setup.send_type = TVIDEO_SND_UDP;
				setup.send_func = demo_doorbell_tcp_video_send_packet;

				if (fmt == DOORBELL_IMG_FMT_MJPEG)
				{
					setup.open_type = APP_CAMERA_UVC_MJPEG;
					media_app_camera_open(APP_CAMERA_UVC_MJPEG, ppi);
				}
				else
				{
					setup.open_type = APP_CAMERA_UVC_H264;
					media_app_camera_open(APP_CAMERA_UVC_H264, ppi);
				}

				media_app_transfer_open(&setup);
			}
			break;

			case DOORBELL_UVC_STOP:
			{
				LOGI("UVC STOP\n");

				if (length >= 6)
				{
					ppi = data[2] << 24 | data[3] << 16 | data[4] << 8 | data[5];
				}

				if (length >= 7)
				{
					fmt = data[6];
				}

				media_app_transfer_close();

				if (fmt == DOORBELL_IMG_FMT_MJPEG)
				{
					media_app_camera_close(APP_CAMERA_UVC_MJPEG);
				}
				else
				{
					media_app_camera_close(APP_CAMERA_UVC_H264);
				}
			}
			break;
		}
	}

}

static int demo_doorbell_tcp_audio_handle(unsigned char *data, unsigned int len)
{
	uint32_t n = 0;

#if AUDIO_TRANSFER_ENABLE
	GLOBAL_INT_DECLARATION();

	if (ring_buffer_get_free_size(&aud_tras_rb) >= len) {
		//os_printf("write data to aud_data_rb \n");
		GLOBAL_INT_DISABLE();
		ring_buffer_write(&aud_tras_rb, data, len);
		GLOBAL_INT_RESTORE();
		n = len;
	} else {
		n = 0;
	}

	if (rtos_is_queue_empty(&aud_tras_int_msg_que)) {
		aud_tras_send_msg(AUD_TRAS_TX, NULL);
	}
#endif
	return n;
}


static void demo_doorbell_tcp_cmd_data_handle(uint8_t *data, uint16_t length)
{
#if AUDIO_TRANSFER_ENABLE
		bk_err_t ret = BK_ERR_AUD_INTF_OK;
#endif

	uint32_t param = 0;
	bool lcd_rotate = 0;
	uint32_t cmd = (uint32_t)data[0] << 24 | (uint32_t)data[1] << 16 | (uint32_t)data[2] << 8 | data[3];
	if (length >= 8)
	{
		param = (uint32_t)data[4] << 24 | (uint32_t)data[5] << 16 | (uint32_t)data[6] << 8 | data[7];
	}

	LOGI("doorbell cmd: %08X, param: %d, len: %d\n", cmd, param, length);

	if (length > 8)
	{
		lcd_rotate = data[8];
		LOGI("%s, lcd_rotate:%d\r\n", __func__, lcd_rotate);
	}

	if (length >= 8)
	{
		switch (cmd)
		{
#if AUDIO_TRANSFER_ENABLE
			case AUDIO_CLOSE:
				LOGI("close audio \n");
				bk_aud_intf_voc_stop();
				/* deinit aud_tras task */
				aud_tras_deinit();
				bk_aud_intf_voc_deinit();
				aud_work_mode = AUD_INTF_WORK_MODE_NULL;
				bk_aud_intf_set_mode(aud_work_mode);
				bk_aud_intf_drv_deinit();
				//GLOBAL_INT_DISABLE();
				aud_tras_working = false;
				//GLOBAL_INT_RESTORE();
				os_free(audio_data_temp);
				audio_data_temp = NULL;
				ring_buffer_clear(&aud_data_rb);
				os_free(aud_data_send_buff);
				aud_data_send_buff = NULL;
				ring_buffer_clear(&aud_tras_rb);
				os_free(aud_tras_buff_addr);
				aud_tras_buff_addr = NULL;
				os_free(audio_tcp_send_buffer);
				audio_tcp_send_buffer = NULL;
				break;

			case AUDIO_OPEN:
				LOGI("open audio \n");
				//aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
				//aud_intf_drv_setup.task_config.priority = 3;
				//aud_intf_drv_setup.aud_intf_rx_spk_data = NULL;
				aud_intf_drv_setup.aud_intf_tx_mic_data = demo_doorbell_tcp_audio_handle;
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
				ret = bk_aud_intf_voc_init(aud_voc_setup);
				if (ret != BK_ERR_AUD_INTF_OK)
				{
					LOGE("bk_aud_intf_voc_init fail, ret:%d\n", ret);
					break;
				}
				/* TODO */
				uint32_t data_type_size = 0;
				uint32_t data_sample_size = 0;

				if (aud_voc_setup.data_type == AUD_INTF_VOC_DATA_TYPE_PCM)
					data_type_size = 2;
				else
					data_type_size = 1;
				if (aud_voc_setup.samp_rate == AUD_INTF_VOC_SAMP_RATE_16K)
					data_sample_size = 320;
				else
					data_sample_size = 160;
				frame_size = data_sample_size * data_type_size;
				audio_data_temp = os_malloc(AUD_DELAY_FRAME * frame_size);
				if (audio_data_temp == NULL) {
					LOGE("malloc audio_data_temp: %d fail \n", AUD_DELAY_FRAME * frame_size);
					break;
				}
				os_memset(audio_data_temp, 0, AUD_DELAY_FRAME * frame_size);
				aud_data_send_buff = os_malloc(AUD_DELAY_FRAME * frame_size + 4);
				if (aud_data_send_buff == NULL)
				{
					LOGE("malloc aud_data_send_buff: %d fail \n", AUD_DELAY_FRAME * frame_size + 4);
					break;
				}
				ring_buffer_init(&aud_data_rb, aud_data_send_buff, AUD_DELAY_FRAME * frame_size + 4, DMA_ID_MAX, RB_DMA_TYPE_NULL);

				if (audio_tcp_send_buffer == NULL)
				{
					audio_tcp_send_buffer = (uint8_t *)os_malloc(TCP_HEAD_SIZE + (AUD_DELAY_FRAME+1) * frame_size);
				}

				uint32_t a_cn = 0;
				uint32_t b_cn = 0;
				a_cn = (AUD_DELAY_FRAME * frame_size)/1000;
				b_cn = (AUD_DELAY_FRAME * frame_size)%1000;
				os_printf("a_cn: %d, b_cn: %d \r\n", a_cn, b_cn);
				for(int i = 0; i < a_cn; i++) {
					demo_doorbell_tcp_audio_send_packet(audio_data_temp, 1000);
				}
				demo_doorbell_tcp_audio_send_packet(audio_data_temp, b_cn);

				/* init aud_tras task */
				aud_tras_buff_addr = os_malloc(AUD_DELAY_FRAME * frame_size + 4);
				if (aud_tras_buff_addr == NULL) {
					os_printf("malloc aud_tras_buff_addr fail \r\n");
					break;
				}
				ring_buffer_init(&aud_tras_rb, aud_tras_buff_addr, AUD_DELAY_FRAME * frame_size + 4, DMA_ID_MAX, RB_DMA_TYPE_NULL);
				aud_tras_setup.aud_tx_rb = &aud_tras_rb;
				aud_tras_setup.aud_tras_send_data_cb = demo_doorbell_tcp_audio_send_packet;
				ret = aud_tras_init(&aud_tras_setup);
				if (ret != BK_OK) {
					LOGI("aud_tras init fail\n");
					break;
				}

				ret = bk_aud_intf_voc_start();
				if (ret != BK_ERR_AUD_INTF_OK)
				{
					LOGE("bk_aud_intf_voc_start fail, ret:%d\n", ret);
					break;
				}
				//GLOBAL_INT_DISABLE();
				aud_tras_working = true;
				//GLOBAL_INT_RESTORE();
				break;
#endif  //AUDIO_TRANSFER_ENABLE

			case DISPLAY_CLOSE:
				LOGI("LCD CLOSE\n");
				media_app_lcd_close();
				break;

			case DISPLAY_OPEN:
				LOGI("LCD OPEN: %dX%d\n", param >> 16, param & 0xFFFF);
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
#endif
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
				//LOGI("data not enough, left: %d\n", left);
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

/*
	if (audio_tcp_send_buffer == NULL)
	{
		audio_tcp_send_buffer = (uint8_t *)os_malloc(TCP_HEAD_SIZE + AUD_DELAY_FRAME * frame_size);
	}
*/
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


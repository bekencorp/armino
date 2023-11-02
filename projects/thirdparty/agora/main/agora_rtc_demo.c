#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>
#include <components/shell_task.h>
#include <components/event.h>
#include <components/netif_types.h>
#include "bk_rtos_debug.h"
#include "agora_config.h"
#include "agora_rtc.h"
#include "audio_transfer.h"
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#include <driver/media_types.h>
#include "media_app.h"
#include "lcd_act.h"
#include "fpscc.h"

#include "BK7256_RegList.h"


#define DEFAULT_CHANNEL_NAME "hello_bk7256demo"

#define CONFIG_SEND_FRAME_RATE 10

#define TAG "agora_main"
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static bool g_connected_flag = false;
static char agora_appid[50] = {0};
static bool audio_en = false;
static bool video_en = false;
static bool lcd_en = false;
static app_camera_type_t camera_type = APP_CAMERA_UVC_MJPEG;
static media_ppi_t camera_ppi = PPI_800X480;
static media_ppi_t lcd_ppi = PPI_480X272;
static char lcd_name[10] = {'0'};
static uint8_t audio_type = 0;
static aud_intf_voc_samp_rate_t audio_samp_rate = AUD_INTF_VOC_SAMP_RATE_8K;
static bool aec_enable = false;

static beken_thread_t  agora_thread_hdl = NULL;
static bool agora_runing = false;
static agora_rtc_config_t agora_rtc_config = DEFAULT_AGORA_RTC_CONFIG();
static agora_rtc_option_t agora_rtc_option = DEFAULT_AGORA_RTC_OPTION();
static aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
static aud_intf_voc_setup_t aud_intf_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();
static aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;

static void* cc_handle = NULL;
static uint32_t g_target_bps = 0;


extern void rwnxl_set_video_transfer_flag(uint32_t video_transfer_flag);

static void cli_agora_rtc_help(void)
{
	os_printf("agora_test {audio start|stop appid audio_type sampple_rate aec_en} \r\n");
	os_printf("agora_test {video start|stop appid video_type ppi} \r\n");
	os_printf("agora_test {both start|stop appid audio_type sample_rate video_type ppi aec_en lcd_ppi lcd_name} \r\n");
}

static int send_agora_audio_frame(uint8_t *data, unsigned int len)
{
//	addAON_GPIO_Reg0x3 = 2;
	audio_frame_info_t info = { 0 };
	info.data_type = AUDIO_DATA_TYPE_PCMU;

	int rval = bk_agora_rtc_audio_data_send(data, len, &info);
	if (rval < 0) {
		LOGE("Failed to send audio data, reason: %s\n", agora_rtc_err_2_str(rval));
		return 0;
	}
//	addAON_GPIO_Reg0x3 = 0;
	return len;
}


static void agora_rtc_user_notify_msg_handle(agora_rtc_msg_t *p_msg)
{
	switch(p_msg->code)
	{
		case AGORA_RTC_MSG_JOIN_CHANNEL_SUCCESS:
			g_connected_flag = true;
			break;
		case AGORA_RTC_MSG_USER_JOINED:
		case AGORA_RTC_MSG_USER_OFFLINE:
			break;
		case AGORA_RTC_MSG_CONNECTION_LOST:
			LOGE("Lost connection. Please check wifi status.\n");
			g_connected_flag = false;
			break;
		case AGORA_RTC_MSG_INVALID_APP_ID:
			LOGE("Invalid App ID. Please double check.\n");
			break;
		case AGORA_RTC_MSG_INVALID_CHANNEL_NAME:
			LOGE("Invalid channel name. Please double check.\n");
			break;
		case AGORA_RTC_MSG_INVALID_TOKEN:
		case AGORA_RTC_MSG_TOKEN_EXPIRED:
			LOGE("Invalid token. Please double check.\n");
			break;
		case AGORA_RTC_MSG_BWE_TARGET_BITRATE_UPDATE:
			g_target_bps = p_msg->data.bwe.target_bitrate;
			break;
		default:
			break;
	}
}

static void _print_capture_fps(void) {
	static uint32_t last_print = 0;
	static uint32_t g_capture_fps = 0;
	uint32_t now = rtos_get_time();
	g_capture_fps++;
	if (last_print == 0) {
		last_print = now;
	}
	if (now > last_print + 1000) {
		LOGI("capture fps: %d, target_bps: %d \n", g_capture_fps, g_target_bps);
		g_capture_fps = 0;
		last_print = now;
	}
}

static void memory_free_show(void)
{
	uint32_t total_size,free_size,mini_size;

	LOGI("%-5s   %-5s   %-5s   %-5s   %-5s\r\n",
		"name", "total", "free", "minimum", "peak");

	total_size = rtos_get_total_heap_size();
	free_size  = rtos_get_free_heap_size();
	mini_size  = rtos_get_minimum_free_heap_size();
	LOGI("heap:\t%d\t%d\t%d\t%d\r\n",  total_size, free_size, mini_size, total_size - mini_size);

#if CONFIG_PSRAM_AS_SYS_MEMORY
	total_size = rtos_get_psram_total_heap_size();
	free_size  = rtos_get_psram_free_heap_size();
	mini_size  = rtos_get_psram_minimum_free_heap_size();
	LOGI("psram:\t%d\t%d\t%d\t%d\r\n", total_size, free_size, mini_size, total_size - mini_size);
#endif

}


void app_media_read_frame_callback(frame_buffer_t * frame)
{
//	addAON_GPIO_Reg0x2 = 2;
	video_frame_info_t info = { 0 };
	info.data_type = VIDEO_DATA_TYPE_GENERIC_JPEG;
	info.frame_rate = CONFIG_SEND_FRAME_RATE;
	info.frame_type = VIDEO_FRAME_KEY;
	info.stream_type = VIDEO_STREAM_HIGH;

	_print_capture_fps();
	uint32_t recent_bps = 0;
	uint32_t recent_fps = 0;
	get_bps_fps(cc_handle, &recent_bps, &recent_fps);
	if (0 == insert_frame_within_target_bitrate(cc_handle, frame->length, g_target_bps + 200000)) {
		bk_agora_rtc_video_data_send((uint8_t *)frame->frame, frame->length, &info);
	}
//	addAON_GPIO_Reg0x2 = 0;
	rtos_delay_milliseconds(60);
}

static int agora_rtc_user_audio_rx_data_handle(unsigned char *data, unsigned int size, const audio_frame_info_t *info_ptr)
{
	bk_err_t ret = BK_OK;

	if ((info_ptr->data_type == AUDIO_DATA_TYPE_PCMA && aud_intf_voc_setup.data_type == AUD_INTF_VOC_DATA_TYPE_G711A)
			|| (info_ptr->data_type == AUDIO_DATA_TYPE_PCMU && aud_intf_voc_setup.data_type == AUD_INTF_VOC_DATA_TYPE_G711U)
			|| (info_ptr->data_type == AUDIO_DATA_TYPE_PCM && aud_intf_voc_setup.data_type == AUD_INTF_VOC_DATA_TYPE_PCM)
		)
	{
		/* write a fram speaker data to speaker_ring_buff */
		ret = bk_aud_intf_write_spk_data((uint8_t *)data, (uint32_t)size);
		if (ret != BK_OK) {
			LOGE("write spk data fail \r\n");
		}
	} else {
		LOGE("audio data type:%d is not match voice data type: %d.\n", info_ptr->data_type, aud_intf_voc_setup.data_type);
		ret = BK_FAIL;
	}

	return ret;
}



void agora_main(void)
{
	bk_err_t ret = BK_OK;

	cc_handle = create_bps_fps_calculator(2);

	memory_free_show();

	// 2. API: init agora rtc sdk

	//service_opt.license_value[0] = '\0';
	agora_rtc_config.p_appid = (char *)os_malloc(strlen(agora_appid) + 1);
	os_strcpy((char *)agora_rtc_config.p_appid, agora_appid);
	ret = bk_agora_rtc_create(&agora_rtc_config, (agora_rtc_msg_notify_cb)agora_rtc_user_notify_msg_handle);
	if (ret != BK_OK) {
		LOGI("bk_agora_rtc_create fail \r\n");
	}
	LOGI("-----start agora rtc process-----\r\n");

	if (lcd_en) {
		LOGI("open lcd \r\n");
		memory_free_show();
		media_app_lcd_rotate(ROTATE_90);
		lcd_open_t lcd_open;
		lcd_open.device_ppi = lcd_ppi;
		lcd_open.device_name = lcd_name;//"st7796s" "h050iwv" "st7282"
		media_app_lcd_open(&lcd_open);
		memory_free_show();
	}

	if (audio_en) {
		aud_intf_drv_setup.aud_intf_tx_mic_data = send_agora_audio_frame;
		ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);
		if (ret != BK_ERR_AUD_INTF_OK) {
			LOGE("bk_aud_intf_drv_init fail, ret:%d \r\n", ret);
		} else {
			LOGI("bk_aud_intf_drv_init complete \r\n");
		}

		aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
		ret = bk_aud_intf_set_mode(aud_work_mode);
		if (ret != BK_ERR_AUD_INTF_OK) {
			LOGE("bk_aud_intf_set_mode fail, ret:%d \r\n", ret);
		} else {
			LOGI("bk_aud_intf_set_mode complete \r\n");
		}

		aud_intf_voc_setup.data_type  = AUD_INTF_VOC_DATA_TYPE_G711U;
		aud_intf_voc_setup.spk_mode   = AUD_DAC_WORK_MODE_SIGNAL_END;
		aud_intf_voc_setup.aec_enable = aec_enable;
		aud_intf_voc_setup.samp_rate = audio_samp_rate;
		if (audio_type == 1) {
			aud_intf_voc_setup.mic_type = AUD_INTF_MIC_TYPE_UAC;
			aud_intf_voc_setup.spk_type = AUD_INTF_MIC_TYPE_UAC;
		} else {
			aud_intf_voc_setup.mic_type = AUD_INTF_MIC_TYPE_BOARD;
			aud_intf_voc_setup.spk_type = AUD_INTF_MIC_TYPE_BOARD;
		}

		ret = bk_aud_intf_voc_init(aud_intf_voc_setup);
		if (ret != BK_ERR_AUD_INTF_OK) {
			LOGE("bk_aud_intf_voc_init fail, ret:%d \r\n", ret);
		} else {
			LOGI("bk_aud_intf_voc_init complete \r\n");
		}

		ret = bk_aggora_rtc_register_audio_rx_handle((agora_rtc_audio_rx_data_handle)agora_rtc_user_audio_rx_data_handle);
		if (ret != BK_OK) {
			LOGE("bk_aggora_rtc_register_audio_rx_handle fail, ret:%d \r\n", ret);
		} else {
			LOGI("bk_aggora_rtc_register_audio_rx_handle complete \r\n");
		}
	}

	agora_rtc_option.p_channel_name = (char *)os_malloc(os_strlen(DEFAULT_CHANNEL_NAME) + 1);
	os_strcpy((char *)agora_rtc_option.p_channel_name, DEFAULT_CHANNEL_NAME);
	ret = bk_agora_rtc_start(&agora_rtc_option);
	if (ret != BK_OK) {
		LOGE("bk_agora_rtc_start fail, ret:%d \r\n", ret);
		return;
	}

	LOGI("-----agora_rtc_join_channel end-----\r\n");
	// 5. wait until we join channel successfully
	while (!g_connected_flag) {
		memory_free_show();
		// rtos_dump_task_runtime_stats();
		//usleep(1 * 1000 * 1000);
		rtos_delay_milliseconds(1000);
	}
	LOGI("-----agora_rtc_join_channel success-----\r\n");

	if (audio_en) {
		LOGI("audio start \r\n");
		memory_free_show();
		ret = bk_aud_intf_voc_start();
		if (ret != BK_ERR_AUD_INTF_OK) {
			LOGE("bk_aud_intf_voc_start fail, ret:%d \r\n", ret);
		} else {
			LOGI("bk_aud_intf_voc_start complete \r\n");
		}
		memory_free_show();
	}

	if (video_en) {
		memory_free_show();
		rwnxl_set_video_transfer_flag(true);
		media_app_camera_open(camera_type, camera_ppi);	// PPI_320X240   PPI_640X480
		media_app_register_read_frame_cb(app_media_read_frame_callback);
		media_app_save_start("unused_name");
		memory_free_show();
	}

	agora_runing = true;

	while (agora_runing) {
		rtos_delay_milliseconds(1000);
		memory_free_show();
	}

	if (lcd_en) {
		media_app_lcd_close();
	}

	/* free audio  */
	if (audio_en) {
		bk_aggora_rtc_register_audio_rx_handle(NULL);

		/* stop voice */
		ret = bk_aud_intf_voc_stop();
		if (ret != BK_ERR_AUD_INTF_OK) {
			LOGE("bk_aud_intf_voc_stop fail, ret:%d \r\n", ret);
		} else {
			LOGI("bk_aud_intf_voc_stop complete \r\n");
		}

		/* deinit vioce */
		ret = bk_aud_intf_voc_deinit();
		if (ret != BK_ERR_AUD_INTF_OK) {
			LOGE("bk_aud_intf_voc_deinit fail, ret:%d \r\n", ret);
		} else {
			LOGI("bk_aud_intf_voc_deinit complete \r\n");
		}

		/* deinit audio */
		aud_work_mode = AUD_INTF_WORK_MODE_NULL;
		bk_aud_intf_set_mode(aud_work_mode);

		ret = bk_aud_intf_drv_deinit();
		if (ret != BK_ERR_AUD_INTF_OK) {
			LOGE("bk_aud_intf_drv_deinit fail, ret:%d \r\n", ret);
		} else {
			LOGI("bk_aud_intf_drv_deinit complete \r\n");
		}
	}

	/* free audio  */
	if (video_en) {
		media_app_save_stop();
		rwnxl_set_video_transfer_flag(false);
		media_app_camera_close(camera_type);
	}

	/* free agora */
	/* stop agora rtc */
	bk_agora_rtc_stop();

	/* destory agora rtc */
	bk_agora_rtc_destroy();

	destroy_bps_fps_calculator(cc_handle);

	if (agora_rtc_config.p_appid) {
		os_free((char *)agora_rtc_config.p_appid);
		agora_rtc_config.p_appid = NULL;
	}

	if (agora_rtc_option.p_channel_name) {
		os_free((char *)agora_rtc_option.p_channel_name);
		agora_rtc_option.p_channel_name = NULL;
	}

	audio_en = false;
	video_en = false;
	lcd_en = false;

	/* delete task */
	agora_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}

void agora_start(void)
{
	bk_err_t ret = BK_OK;

	ret = rtos_create_thread(&agora_thread_hdl,
						 3,
						 "agora",
						 (beken_thread_function_t)agora_main,
						 2*1024,
						 NULL);
	if (ret != kNoErr) {
		LOGE("create agora app task fail \r\n");
		agora_thread_hdl = NULL;
	}
	LOGI("create agora app task complete \r\n");
}


void cli_agora_rtc_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	/* audio test */
	if (os_strcmp(argv[1], "audio") == 0) {
		if (os_strcmp(argv[2], "start") == 0) {
			sprintf(agora_appid, "%s", argv[3]);
			audio_type = os_strtoul(argv[4], NULL, 16) & 0xFF;
			uint32_t value = os_strtoul(argv[5], NULL, 10);
			if (value == 16000) {
				audio_samp_rate = AUD_INTF_VOC_SAMP_RATE_16K;
			} else {
				audio_samp_rate = AUD_INTF_VOC_SAMP_RATE_8K;
			}
			uint32_t aec_en = os_strtoul(argv[6], NULL, 10);
			if (aec_en == 0) {
				aec_enable = false;
			} else {
				aec_enable = true;
			}
			LOGI("start agora audio test \r\n");
			audio_en = true;
			//video_en = false;
			agora_start();
		} else if (os_strcmp(argv[2], "stop") == 0) {
			//TODO stop audio
			agora_runing = false;
		} else {
			goto cmd_fail;
		}
	} else if (os_strcmp(argv[1], "video") == 0) {
		if (os_strcmp(argv[2], "start") == 0) {
			sprintf(agora_appid, "%s", argv[3]);
			LOGI("start agora video test \r\n");

			if (os_strcmp(argv[4], "DVP_JPEG") == 0) {
				camera_type = APP_CAMERA_DVP_JPEG;
			} else if (os_strcmp(argv[4], "DVP_YUV") == 0) {
				camera_type = APP_CAMERA_DVP_YUV;
			} else if (os_strcmp(argv[4], "UVC_MJPEG") == 0) {
				camera_type = APP_CAMERA_UVC_MJPEG;
			} else {
				LOGW("the type is not support \n");
				goto cmd_fail;
			}

			if (os_strcmp(argv[5], "320X240") == 0) {
				camera_ppi = PPI_320X240;
			} else if (os_strcmp(argv[5], "320X480") == 0) {
				camera_ppi = PPI_320X480;
			} else if (os_strcmp(argv[5], "480X320") == 0) {
				camera_ppi = PPI_480X320;
			} else if (os_strcmp(argv[5], "480X480") == 0) {
				camera_ppi = PPI_480X480;
			} else if (os_strcmp(argv[5], "480X800") == 0) {
				camera_ppi = PPI_480X800;
			} else if (os_strcmp(argv[5], "640X480") == 0) {
				camera_ppi = PPI_640X480;
			} else if (os_strcmp(argv[5], "800X480") == 0) {
				camera_ppi = PPI_800X480;
			} else {
				LOGW("the ppi is not support \n");
				goto cmd_fail;
			}
			audio_en = false;
			video_en = true;
			agora_start();
		} else if (os_strcmp(argv[2], "stop") == 0) {
			//TODO stop video
			agora_runing = false;
		} else {
			goto cmd_fail;
		}
	} else if (os_strcmp(argv[1], "both") == 0) {
		if (os_strcmp(argv[2], "start") == 0) {
			sprintf(agora_appid, "%s", argv[3]);
			LOGI("start agora video test \r\n");
			audio_type = os_strtoul(argv[4], NULL, 16) & 0xFF;
			uint32_t value = os_strtoul(argv[5], NULL, 10);
			if (value == 16000) {
				audio_samp_rate = AUD_INTF_VOC_SAMP_RATE_16K;
			} else {
				audio_samp_rate = AUD_INTF_VOC_SAMP_RATE_8K;
			}

			if (os_strcmp(argv[6], "DVP_JPEG") == 0) {
				camera_type = APP_CAMERA_DVP_JPEG;
			} else if (os_strcmp(argv[6], "DVP_YUV") == 0) {
				camera_type = APP_CAMERA_DVP_YUV;
			} else if (os_strcmp(argv[6], "UVC_MJPEG") == 0) {
				camera_type = APP_CAMERA_UVC_MJPEG;
			} else {
				LOGW("the type is not support \n");
				goto cmd_fail;
			}

			if (os_strcmp(argv[7], "320X240") == 0) {
				camera_ppi = PPI_320X240;
			} else if (os_strcmp(argv[7], "320X480") == 0) {
				camera_ppi = PPI_320X480;
			} else if (os_strcmp(argv[7], "480X320") == 0) {
				camera_ppi = PPI_480X320;
			} else if (os_strcmp(argv[7], "480X480") == 0) {
				camera_ppi = PPI_480X480;
			} else if (os_strcmp(argv[7], "480X800") == 0) {
				camera_ppi = PPI_480X800;
			} else if (os_strcmp(argv[7], "640X480") == 0) {
				camera_ppi = PPI_640X480;
			} else if (os_strcmp(argv[7], "800X480") == 0) {
				camera_ppi = PPI_800X480;
			} else {
				LOGW("the ppi is not support \n");
				goto cmd_fail;
			}

			uint32_t aec_en = os_strtoul(argv[8], NULL, 10);
			if (aec_en == 0) {
				aec_enable = false;
			} else {
				aec_enable = true;
			}

			if (os_strcmp(argv[9], "480X272") == 0) {
				lcd_ppi = PPI_480X272;
			} else if (os_strcmp(argv[7], "320X480") == 0) {
				lcd_ppi = PPI_320X480;
			} else if (os_strcmp(argv[7], "480X320") == 0) {
				lcd_ppi = PPI_480X320;
			} else if (os_strcmp(argv[7], "480X480") == 0) {
				lcd_ppi = PPI_480X480;
			} else if (os_strcmp(argv[7], "480X800") == 0) {
				lcd_ppi = PPI_480X800;
			} else if (os_strcmp(argv[7], "640X480") == 0) {
				lcd_ppi = PPI_640X480;
			} else if (os_strcmp(argv[7], "800X480") == 0) {
				lcd_ppi = PPI_800X480;
			} else {
				LOGW("the ppi is not support \n");
				goto cmd_fail;
			}

			os_memcpy(lcd_name, argv[10], os_strlen(argv[10]));

			audio_en = true;
			video_en = true;
			lcd_en = true;
			agora_start();
		} else if (os_strcmp(argv[2], "stop") == 0) {
			//TODO stop video
			agora_runing = false;
		} else {
			goto cmd_fail;
		}
	} else {
		cli_agora_rtc_help();
	}

	return;

cmd_fail:
	cli_agora_rtc_help();
}


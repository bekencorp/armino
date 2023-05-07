#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#include <components/shell_task.h>
#include <components/event.h>
#include <components/netif_types.h>

#include <components/aud_intf.h>
#include <components/aud_intf_types.h>

#include <components/video_transfer.h>

#include "bk_rtos_debug.h"

//#include "bk_private/bk_wifi.h"
//#include "bk_private/bk_wifi_wrapper.h"

#include "audio_control.h"

#include <driver/media_types.h>
#include "media_app.h"

//#include "video_control.h"
//#include "ble_proc.h"

#include "agora_config.h"
#include <agora_rtc_api.h>
#include "BK7256_RegList.h"
#include "amf_list.h"

#define DEFAULT_TOKEN NULL
#define DEFAULT_CHANNEL_NAME "hello_bk7256demo"
#define DEFAULT_USER_ID 0 // sdk will automatically assign a random user id
#define DEFAULT_AREA_CODE AREA_CODE_GLOB

#define CONFIG_PCM_CHANNEL_NUM (1)
#define CONFIG_PCM_SAMPLE_RATE (8000)

#define CONFIG_SEND_FRAME_RATE 10

#define DEFAULT_BANDWIDTH_ESTIMATE_MIN_BITRATE   (400000)
#define DEFAULT_BANDWIDTH_ESTIMATE_MAX_BITRATE   (5000000)
#define DEFAULT_BANDWIDTH_ESTIMATE_START_BITRATE (200000)
#define LDB_WORD(ptr) (uint16_t)(((uint16_t) * ((uint8_t *)(ptr)) << 8) | (uint16_t) * (uint8_t *)((ptr) + 1))

#define TAG "agora_main"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static bool g_connected_flag = false;
static connection_id_t g_conn_id;
static bool g_wifi_connected = false;
static connection_info_t g_conn_info;
static char agora_appid[50] = {0};

static beken_thread_t  agora_thread_hdl = NULL;
static bool agora_runing = false;
static bool audio_en = false;
static bool video_en = false;
static uint8_t video_send_fps_count = 0;
static media_ppi_t camera_ppi = PPI_320X240;
#define VIDEO_FRAME_NUM  25
#define AUDIO_BPS  200000	//8k sample rate


typedef struct {
	uint32_t band_width_bps;
	uint32_t video_latest_1_second_send_bps;
	video_frame_node_t *video_fram_list;

	uint8_t video_capture_fps;
} video_bitrat_info_t;

video_bitrat_info_t video_bitrat_info = {0};


static void cli_agora_help(void)
{
	os_printf("agora_test {audio|video|both start|stop appid ppi} \r\n");
}

static int _netif_event_cb(void *arg, event_module_t event_module, int event_id, void *event_data)
{
	netif_event_got_ip4_t *got_ip;

	switch (event_id) {
	case EVENT_NETIF_GOT_IP4:
		got_ip = (netif_event_got_ip4_t *)event_data;
		LOGI("%s got ip\n", got_ip->netif_if == NETIF_IF_STA ? "BK STA" : "unknown netif");
#if CONFIG_WIFI6_CODE_STACK
		// unsigned char vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);
    //     wlan_dhcp_done_ind(vif_idx);
#endif
		g_wifi_connected = true;
		break;
	default:
		LOGI("rx event <%d %d>\n", event_module, event_id);
		break;
	}

	return BK_OK;
}


int send_agora_audio_frame(uint8_t *data, uint32_t len)
{
	// API: send audio data
	audio_frame_info_t info = { 0 };
	info.data_type = AUDIO_DATA_TYPE_PCM;
	if (320 != len) {
		LOGE("get audio frame low level, length: %d\r\n", (int)len);
		return 0;
	}

	int rval = agora_rtc_send_audio_data(g_conn_id, data, len, &info);
	if (rval < 0) {
		LOGE("Failed to send audio data, reason: %s\n", agora_rtc_err_2_str(rval));
		return -1;
	}

	return 0;
}


static int send_agora_video_frame(uint8_t *data, uint32_t len)
{
	int rval = 0;
	// API: send audio data
	video_frame_info_t info = { 0 };
	info.data_type = VIDEO_DATA_TYPE_GENERIC_JPEG;
	info.frame_rate = CONFIG_SEND_FRAME_RATE;
	info.frame_type = VIDEO_FRAME_KEY;
	info.stream_type = VIDEO_STREAM_HIGH;

	rval = agora_rtc_send_video_data(g_conn_id, data, len, &info);
	if (rval < 0) {
		LOGE("Failed to send video data, reason: %s\n", agora_rtc_err_2_str(rval));
		return -1;
	}
	return 0;
}

static void __on_join_channel_success(connection_id_t conn_id, uint32_t uid, int elapsed)
{
	g_connected_flag = true;
	agora_rtc_get_connection_info(conn_id, &g_conn_info);
	LOGI("[conn-%u] Join the channel %s successfully, uid %u elapsed %d ms\n", conn_id, g_conn_info.channel_name, uid, elapsed);
}

static void __on_connection_lost(connection_id_t conn_id)
{
	g_connected_flag = false;
	LOGI("[conn-%u] Lost connection from the channel\n", conn_id);
}

static void __on_rejoin_channel_success(connection_id_t conn_id, uint32_t uid, int elapsed_ms)
{
	g_connected_flag = true;
	LOGI("[conn-%u] Rejoin the channel successfully, uid %u elapsed %d ms\n", conn_id, uid, elapsed_ms);
}

static void __on_user_joined(connection_id_t conn_id, uint32_t uid, int elapsed_ms)
{
	LOGI("[conn-%u] Remote user \"%u\" has joined the channel, elapsed %d ms\n", conn_id, uid, elapsed_ms);
}

static void __on_user_offline(connection_id_t conn_id, uint32_t uid, int reason)
{
	LOGI("[conn-%u] Remote user \"%u\" has left the channel, reason %d\n", conn_id, uid, reason);
}

static void __on_user_mute_audio(connection_id_t conn_id, uint32_t uid, bool muted)
{
	LOGI("[conn-%u] audio: uid=%u muted=%d\n", conn_id, uid, muted);
}

static void __on_user_mute_video(connection_id_t conn_id, uint32_t uid, bool muted)
{
	LOGI("[conn-%u] video: uid=%u muted=%d\n", conn_id, uid, muted);
}

static void __on_error(connection_id_t conn_id, int code, const char *msg)
{
	if (code == ERR_SEND_VIDEO_OVER_BANDWIDTH_LIMIT) {
		LOGE("Not enough uplink bandwdith. Error msg \"%s\"\n", msg);
		return;
	}

	if (code == ERR_INVALID_APP_ID) {
		LOGE("Invalid App ID. Please double check. Error msg \"%s\"\n", msg);
	} else if (code == ERR_INVALID_CHANNEL_NAME) {
		LOGE("Invalid channel name. Please double check. Error msg \"%s\"\n", msg);
	} else if (code == ERR_INVALID_TOKEN || code == ERR_TOKEN_EXPIRED) {
		LOGE("Invalid token. Please double check. Error msg \"%s\"\n", msg);
	} else if (code == ERR_DYNAMIC_TOKEN_BUT_USE_STATIC_KEY) {
		LOGE("Dynamic token is enabled but is not provided. Error msg \"%s\"\n", msg);
	} else {
		LOGE("Error %d is captured. Error msg \"%s\"\n", code, msg);
	}
}

static void __on_audio_data(connection_id_t conn_id, const uint32_t uid, uint16_t sent_ts, const void *data, size_t len, const audio_frame_info_t *info_ptr)
{
#if 1
	bk_err_t ret = BK_OK;

	/* write a fram speaker data to speaker_ring_buff */
	ret = bk_aud_intf_write_spk_data((uint8_t *)data, (uint32_t)len);
	if (ret != BK_OK) {
		LOGE("write spk data fail \r\n");
	}
#endif
}

static void __on_mixed_audio_data(connection_id_t conn_id, const void *data, size_t len,
                                  const audio_frame_info_t *info_ptr)
{
  //LOGI("[conn-%u] on_mixed_audio_data, data_type %d, len %zu\n", conn_id, info_ptr->data_type, len);
}

#ifndef CONFIG_AUDIO_ONLY
static void __on_video_data(connection_id_t conn_id, const uint32_t uid, uint16_t sent_ts, const void *data, size_t len,
                            const video_frame_info_t *info_ptr)
{
  /*LOGI("[conn-%u] on_video_data: uid %u sent_ts %u data_type %d frame_type %d stream_type %d len %zu\n", conn_id, uid,
         sent_ts, info_ptr->data_type, info_ptr->frame_type, info_ptr->stream_type, len);*/
}

static void __on_target_bitrate_changed(connection_id_t conn_id, uint32_t target_bps)
{
	LOGI("[conn-%u] Bandwidth change detected. Please adjust encoder bitrate to %u kbps\n", conn_id, target_bps / 1000);
	//band_width = target_bps;
	video_bitrat_info.band_width_bps = target_bps;
}

/* used in video h264 and h265 */
static void __on_key_frame_gen_req(connection_id_t conn_id, uint32_t uid, video_stream_type_e stream_type)
{
  //LOGI("[conn-%u] Frame loss detected. Please notify the encoder to generate key frame immediately\n", conn_id);
}
#endif

static void app_init_event_handler(agora_rtc_event_handler_t *event_handler)
{
  event_handler->on_join_channel_success = __on_join_channel_success;
  event_handler->on_connection_lost = __on_connection_lost;
  event_handler->on_rejoin_channel_success = __on_rejoin_channel_success;
  event_handler->on_user_joined = __on_user_joined;
  event_handler->on_user_offline = __on_user_offline;
  event_handler->on_user_mute_audio = __on_user_mute_audio;
#ifndef CONFIG_AUDIO_ONLY
  event_handler->on_user_mute_video = __on_user_mute_video;
  event_handler->on_target_bitrate_changed = __on_target_bitrate_changed;
  event_handler->on_key_frame_gen_req = __on_key_frame_gen_req;
  event_handler->on_video_data = __on_video_data;
#endif
  event_handler->on_error = __on_error;
  event_handler->on_mixed_audio_data = NULL;
  event_handler->on_audio_data = __on_audio_data;
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


static void calc_video_data_bps(video_bitrat_info_t *video_bitrat_info, uint32_t video_data_len)
{
	static int32_t last_time = 0;
	static uint32_t total_bytes_len = 0;
	static uint32_t total_frame_cnt = 0;
	int32_t 	   now;
	uint8_t video_send_fps = 0;

	//init last time
	if (0 == last_time) {
	  last_time = rtos_get_time();
	}

	total_bytes_len += video_data_len;
	++total_frame_cnt;
	now = rtos_get_time();

//	os_printf("last_time = %d, total_bytes_len = %d, now = %d, diff = %d, video_data_len = %d\n",
//	  last_time, total_bytes_len, now, now - last_time, video_data_len);
	//uint32_t wrap around
	if (now < last_time) {
		total_bytes_len = 0;
		total_frame_cnt = 0;
		last_time		= now;
		return;
	}
	uint32_t diff = (now - last_time);
	if (diff >= 1000) {
		video_bitrat_info->video_capture_fps = (total_frame_cnt * 1000) / diff;
		total_bytes_len 					 = 0;
		total_frame_cnt 					 = 0;
		last_time							 = now;
		video_send_fps = (video_send_fps_count * 1000) / diff;
		LOGI("video_capture_fps: %d, video_send_fps: %d \n", video_bitrat_info->video_capture_fps, video_send_fps);
		video_send_fps_count = 0;
		memory_free_show();
	}
}


static bool video_frame_send_check(video_bitrat_info_t *video_bitrat, uint32_t video_data_len)
{
	bool ret = false;
	uint32_t temp = 0;
	video_frame_node_t *node = NULL;
	video_frame_data_t video_data = {0};

	temp = video_fram_list_get_all_length(video_bitrat->video_fram_list);
	video_bitrat->video_latest_1_second_send_bps = temp<<3;
	//os_printf("video_latest_1_second_send_bps: %d, band_width_bps: %d \n", video_bitrat->video_latest_1_second_send_bps, video_bitrat->band_width_bps);

	if ((temp<<3) >= (video_bitrat->band_width_bps - AUDIO_BPS)) {
		ret = false;
		video_fram_list_pop_front(&video_bitrat->video_fram_list);
		video_data.frame_length = 0;
		node = video_fram_list_node_malloc(video_data);
		if (node == NULL) {
			LOGE("malloc node fail \n");
		} else {
			video_fram_list_push_back(&(video_bitrat->video_fram_list), node);
		}
		LOGD("band_width_bps: %d, video_latest_1_second_send_bps: %d \n", video_bitrat->band_width_bps, video_bitrat->video_latest_1_second_send_bps);
	} else {
		ret = true;
		video_fram_list_pop_front(&video_bitrat->video_fram_list);
		video_data.frame_length = video_data_len;
		node = video_fram_list_node_malloc(video_data);
		if (node == NULL) {
			LOGE("malloc node fail \n");
		} else {
			video_fram_list_push_back(&(video_bitrat->video_fram_list), node);
		}
	}

	return ret;
}

//static uint8_t *video_frame_buffer = NULL;
void app_media_read_frame_callback(frame_buffer_t * frame)
{
	calc_video_data_bps(&video_bitrat_info, frame->length);

	if (video_frame_send_check(&video_bitrat_info, frame->length)) {
		++video_send_fps_count;
		send_agora_video_frame((uint8_t *)frame->frame, frame->length);
	}
}

void agora_main(void)
{
	int rval;

	memory_free_show();
#if 0
	// 1. connect ap
	demo_sta_app_init((char *)"NXIOT", (char *)"88888881");
	bk_event_register_cb(EVENT_MOD_NETIF, EVENT_ID_ALL, _netif_event_cb, NULL);

	while (!g_wifi_connected) {
		usleep(100 * 1000);
	}

	memory_free_show();
#endif

	// 2. API: init agora rtc sdk
	static agora_rtc_event_handler_t event_handler = { 0 };
	app_init_event_handler(&event_handler);
	LOGI("main audio callback:%p\r\n", event_handler.on_audio_data);

	LOGI("-----start agora rtc process-----\r\n");

	if (audio_en) {
		audio_init();
		voice_init();
	}

	rtc_service_option_t service_opt = { 0 };
	service_opt.area_code           = AREA_CODE_GLOB;
	service_opt.log_cfg.log_disable = true;
	service_opt.license_value[0]    = '\0';

	LOGI("-----agora_rtc_init start\r\n");
	rval = agora_rtc_init(agora_appid, &event_handler, &service_opt);
	if (rval < 0) {
		LOGE("Failed to initialize Agora sdk, license %p, reason: %s\n", service_opt.license_value, agora_rtc_err_2_str(rval));
		return;
	}
	LOGI("-----agora_rtc_init success\r\n");
	agora_rtc_set_bwe_param(CONNECTION_ID_ALL, DEFAULT_BANDWIDTH_ESTIMATE_MIN_BITRATE,
                          DEFAULT_BANDWIDTH_ESTIMATE_MAX_BITRATE, DEFAULT_BANDWIDTH_ESTIMATE_START_BITRATE);
	// 3. API: Create connection
	rval = agora_rtc_create_connection(&g_conn_id);
	if (rval < 0) {
		LOGE("Failed to create connection, reason: %s\n", agora_rtc_err_2_str(rval));
		return;
	}
	LOGI("-----agora_rtc_create_connection success-----\r\n");
	// 4. API: join channel
	rtc_channel_options_t channel_options = { 0 };

	if (audio_en) {
		channel_options.auto_subscribe_audio = true;
	} else {
		channel_options.auto_subscribe_audio = false;
	}
	channel_options.auto_subscribe_video = false;

#ifdef CONFIG_SEND_PCM_DATA
	/* If we want to send PCM data instead of encoded audio like AAC or Opus, here please enable
	* audio codec, as well as configure the PCM sample rate and number of channels
	*/
#if 1
	channel_options.audio_codec_opt.audio_codec_type = AUDIO_CODEC_TYPE_G711U;//AUDIO_CODEC_TYPE; //2:G722, 4:G711U
	channel_options.audio_codec_opt.pcm_sample_rate  = CONFIG_PCM_SAMPLE_RATE; //G722:16000, G711U:8000
	channel_options.audio_codec_opt.pcm_channel_num  = CONFIG_PCM_CHANNEL_NUM;
#else
	channel_options.audio_codec_opt.audio_codec_type = AUDIO_CODEC_DISABLED;
#endif
#endif

	rval = agora_rtc_join_channel(g_conn_id, DEFAULT_CHANNEL_NAME, DEFAULT_USER_ID, DEFAULT_TOKEN, &channel_options);
	if (rval < 0) {
		LOGE("Failed to join channel \"%s\", reason: %s\n", DEFAULT_CHANNEL_NAME, agora_rtc_err_2_str(rval));
		return;
	}

	LOGI("-----agora_rtc_join_channel end-----\r\n");
	// 5. wait until we join channel successfully
	while (!g_connected_flag) {
		memory_free_show();
		// rtos_dump_task_runtime_stats();
		usleep(1 * 1000 * 1000);
	}

	if (audio_en) {
		voice_start();
	}
	LOGI("-----agora_rtc_join_channel success-----\r\n");

	if (video_en) {
		video_bitrat_info.band_width_bps = 50000; //50Kbps avoid dump at start
		video_bitrat_info.video_fram_list = NULL;
		video_frame_node_t *node = NULL;
		video_frame_data_t video_frame_data = {0};

		for (uint8_t i = 0; i < VIDEO_FRAME_NUM; i++) {
			node = video_fram_list_node_malloc(video_frame_data);
			if (node == NULL) {
				LOGE("malloc node fail \n");
			} else {
				node->data.frame_length = i;
				video_fram_list_push_back(&(video_bitrat_info.video_fram_list), node);
			}
		}
		memory_free_show();

		media_app_camera_open(APP_CAMERA_DVP_JPEG, camera_ppi);	// PPI_320X240   PPI_640X480
		media_app_register_read_frame_cb(app_media_read_frame_callback);
		media_app_save_start("unused_name");
	}

	agora_runing = true;

	while (agora_runing) {
		rtos_delay_milliseconds(100);
	}

	/* free audio  */
	if (audio_en) {
		voice_stop();
		voice_deinit();
		audio_deinit();
	}

	/* free audio  */
	if (video_en) {
		media_app_save_stop();
		media_app_camera_close(APP_CAMERA_DVP_JPEG);
		video_fram_list_destory(video_bitrat_info.video_fram_list);
	}

	/* free agora */
	// 7. API: leave agora channel
	agora_rtc_leave_channel(g_conn_id);
	// 8. API: destroy agora connection
	agora_rtc_destroy_connection(g_conn_id);
	// 9. API: fini rtc sdk
	agora_rtc_fini();

	audio_en = false;
	video_en = false;

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
						 4096,
						 NULL);
	if (ret != kNoErr) {
		LOGE("create agora app task fail \r\n");
		agora_thread_hdl = NULL;
	}
	LOGI("create agora app task complete \r\n");
}


void cli_agora_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 4) {
		cli_agora_help();
		return;
	}

	/* audio test */
	if (os_strcmp(argv[1], "audio") == 0) {
		if (os_strcmp(argv[2], "start") == 0) {
			sprintf(agora_appid, "%s", argv[3]);
			LOGI("start agora audio test \r\n");
			audio_en = true;
			video_en = false;
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
			if (os_strcmp(argv[4], "320X240") == 0) {
				camera_ppi = PPI_320X240;
			} else if (os_strcmp(argv[4], "640X480") == 0) {
				camera_ppi = PPI_640X480;
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
			audio_en = true;
			video_en = true;
			agora_start();
		} else if (os_strcmp(argv[2], "stop") == 0) {
			//TODO stop video
			agora_runing = false;
		} else {
			goto cmd_fail;
		}
	} else {
		cli_agora_help();
	}

cmd_fail:
		cli_agora_help();
}


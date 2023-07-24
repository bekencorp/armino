#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>

#include "agora_rtc.h"
#include "agora_config.h"
#include "agora_rtc_api.h"


#define TAG "agora_rtc"
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define DEFAULT_BANDWIDTH_ESTIMATE_MIN_BITRATE   (400000)
#define DEFAULT_BANDWIDTH_ESTIMATE_MAX_BITRATE   (5000000)
#define DEFAULT_BANDWIDTH_ESTIMATE_START_BITRATE (200000)


static agora_rtc_t agora_rtc = {0};

static agora_rtc_t *__get_rtc_instance(void)
{
	return &agora_rtc;
}

static void __send_message_2_user(agora_rtc_t *rtc, agora_rtc_msg_t *msg)
{
	if (rtc->user_message_callback) {
		rtc->user_message_callback(msg);
	}
}

static void __rtc_started(agora_rtc_t *rtc)
{
	rtc->state = AGORA_RTC_STATE_WORKING;
}

static bool __is_rtc_started(agora_rtc_t *rtc)
{
	if (rtc->state == AGORA_RTC_STATE_WORKING)
		return true;
	else
		return false;
}

static bool __get_rtc_status(agora_rtc_t *rtc)
{
	return rtc->state;
}

static void __rtc_stopped(agora_rtc_t *rtc)
{
	rtc->state = AGORA_RTC_STATE_IDLE;
}

static void __deep_copy_items_destroy(agora_rtc_t *rtc);

static void __key_frame_req_msg(agora_rtc_t *rtc)
{
	agora_rtc_msg_t msg;
	msg.code = AGORA_RTC_MSG_KEY_FRAME_REQUEST;
	__send_message_2_user(rtc, &msg);
}

static void __change_encode_bitrate_req_msg(agora_rtc_t *rtc, uint32_t target_bps)
{
	agora_rtc_msg_t msg;
	msg.code =  AGORA_RTC_MSG_BWE_TARGET_BITRATE_UPDATE;
	msg.data.bwe.target_bitrate = target_bps;
	__send_message_2_user(rtc, &msg);
}

static void __register_message_router(agora_rtc_t *rtc, agora_rtc_msg_notify_cb message_callback)
{
	rtc->user_message_callback = message_callback;
}

static void __on_join_channel_success(connection_id_t conn_id, uint32_t uid, int elapsed_ms)
{
	LOGD("conn_id: %d, uid: %d, elapsed_ms: %d \n", conn_id, uid, elapsed_ms);
	agora_rtc_t *rtc = __get_rtc_instance();

	rtc->b_channel_joined = true;
	agora_rtc_msg_t msg = { .code = AGORA_RTC_MSG_JOIN_CHANNEL_SUCCESS };
	__send_message_2_user(rtc, &msg);

	__rtc_started(rtc);
}

static void __error_msg(agora_rtc_t *rtc, agora_rtc_msg_e err)
{
	agora_rtc_msg_t msg;
	msg.code = err;
	__send_message_2_user(rtc, &msg);
}

static void __on_error(connection_id_t conn_id, int code, const char *msg)
{
	LOGD("conn_id: %d, code: %d\nmsg: %s \n", conn_id, code, msg ? msg : "null");

	agora_rtc_t *rtc = __get_rtc_instance();
	switch (code) {
		case ERR_INVALID_APP_ID:
			__error_msg(rtc, AGORA_RTC_MSG_INVALID_APP_ID);
			break;
		case ERR_INVALID_CHANNEL_NAME:
			__error_msg(rtc, AGORA_RTC_MSG_INVALID_CHANNEL_NAME);
			break;
		case ERR_INVALID_TOKEN:
			__error_msg(rtc, AGORA_RTC_MSG_INVALID_TOKEN);
			break;
		case ERR_TOKEN_EXPIRED:
			__error_msg(rtc, AGORA_RTC_MSG_TOKEN_EXPIRED);
			break;
		default:
			LOGI("Unknown Error Code \n");
			break;
	}
}

static void __on_user_joined(connection_id_t conn_id, uint32_t uid, int elapsed_ms)
{
	LOGD("conn_id: %d, uid: %d, elapsed_ms: %d \n", conn_id, uid, elapsed_ms);
	agora_rtc_t *rtc = __get_rtc_instance();

	rtc->b_user_joined = true;
	agora_rtc_msg_t msg = { .code = AGORA_RTC_MSG_USER_JOINED };
	__send_message_2_user(rtc, &msg);
}

static void __on_user_offline(connection_id_t conn_id, uint32_t uid, int reason)
{
	LOGD("conn_id: %d, uid: %d, reason: %d \n", conn_id, uid, reason);
	agora_rtc_t *rtc = __get_rtc_instance();

	rtc->b_user_joined = false;
	agora_rtc_msg_t msg = {
		.code = AGORA_RTC_MSG_USER_OFFLINE,
		.data.uid = uid
	};
	__send_message_2_user(rtc, &msg);
}

static void __on_key_frame_gen_req(connection_id_t conn_id, uint32_t uid, video_stream_type_e stream_type)
{
	LOGD("conn_id: %d, uid: %d, video stream type: %d \n", conn_id, uid, stream_type);
	agora_rtc_t *rtc = __get_rtc_instance();

	__key_frame_req_msg(rtc);
}

static void __on_audio_data(connection_id_t conn_id, uint32_t uid, uint16_t sent_ts,
                            const void *data_ptr, size_t data_len, const audio_frame_info_t *info_ptr)
{
	agora_rtc_t *rtc = __get_rtc_instance();
	if (!__is_rtc_started(rtc) || !rtc->b_channel_joined || !rtc->b_user_joined) {
		LOGD( "Wrong rtc state, rtc_started: %s, channel_joined: %s, user_joined: %s \n",
				__is_rtc_started(rtc) ? "true" : "false", rtc->b_channel_joined ? "true" : "false", rtc->b_user_joined ? "true" : "false");
		return;
	}
	LOGD("ch=%s, uid=%d sent_ts=%u codec=%u, data_ptr=%p, data_len=%d",
		rtc->agora_rtc_option.p_channel_name, (int)uid, sent_ts, info_ptr->data_type, data_ptr, (int)data_len);

	if (rtc->audio_rx_data_handle) {
		rtc->audio_rx_data_handle((unsigned char *)data_ptr, data_len, info_ptr);
	} else {
		LOGD("aud_rx_data_handle is NULL \n");
	}
}

static void __on_video_data(connection_id_t conn_id, uint32_t uid, uint16_t sent_ts, const void *data_ptr, size_t data_len,
                            const video_frame_info_t *info_ptr)
{
	agora_rtc_t *rtc = __get_rtc_instance();
	if (!__is_rtc_started(rtc) || !rtc->b_channel_joined || !rtc->b_user_joined) {
	LOGD("Wrong rtc state, rtc_started: %s, channel_joined: %s, user_joined: %s \n",
			__is_rtc_started(rtc) ? "true" : "false", rtc->b_channel_joined ? "true" : "false", rtc->b_user_joined ? "true" : "false");
		return;
	}
	LOGD("ch=%s, uid=%d sent_ts=%u codec=%u, data_ptr=%p, data_len=%d",
			 rtc->agora_rtc_option.p_channel_name, (int)uid, sent_ts, info_ptr->data_type, data_ptr, (int)data_len);

	if (rtc->video_rx_data_handle) {
		rtc->video_rx_data_handle(data_ptr, data_len, info_ptr);
	} else {
		LOGD("aud_rx_data_handle is NULL \n");
	}
}

static void __on_user_mute_audio(connection_id_t conn_id, uint32_t uid, bool muted)
{
	LOGI("[conn-%u] audio: uid=%u muted=%d\n", conn_id, uid, muted);
}

static void __on_user_mute_video(connection_id_t conn_id, uint32_t uid, bool muted)
{
	LOGI("[conn-%u] video: uid=%u muted=%d\n", conn_id, uid, muted);
}

static void __on_target_bitrate_changed(connection_id_t conn_id, uint32_t target_bps)
{

	agora_rtc_t *rtc = __get_rtc_instance();
	if (!__is_rtc_started(rtc)) {
		LOGI("pipeline not started. Skip bitrate changed process \n");
		return;
	}
	LOGD("conn_id: %d, target bitrate will change from %d to %d \n" ,
			conn_id, rtc->target_bitrate, target_bps);

	rtc->target_bitrate = target_bps;
	__change_encode_bitrate_req_msg(rtc, target_bps);
}

static void __on_connection_lost(connection_id_t conn_id)
{
	LOGD("conn_id: %d \n", conn_id);

	agora_rtc_t *rtc = __get_rtc_instance();
	if (!rtc->agora_rtc_option.p_channel_name) {
		LOGI( "Invalid arguments, rtc->agora_rtc_option.p_channel_name: %p \n", rtc->agora_rtc_option.p_channel_name);
		return;
	}
	LOGI("The connection of the channel: %s is lost \n", rtc->agora_rtc_option.p_channel_name);
	rtc->b_channel_joined = false;

	agora_rtc_msg_t msg = {
		.code = AGORA_RTC_MSG_CONNECTION_LOST
	};
	__send_message_2_user(rtc, &msg);
}

static void __on_rejoin_channel_success(connection_id_t conn_id, uint32_t uid, int elapsed_ms)
{
	LOGD("conn_id: %d \n", conn_id);
	agora_rtc_t *rtc = __get_rtc_instance();
	if (!rtc->agora_rtc_option.p_channel_name) {
		LOGI("Invalid arguments, rtc->channel_name: %p \n", rtc->agora_rtc_option.p_channel_name);
		return;
	}
	LOGI("The uid(%d) rejoins to the channel: %s \n", uid, rtc->agora_rtc_option.p_channel_name);

	rtc->b_channel_joined = true;
}

static void __register_agora_rtc_event_handler(agora_rtc_t *rtc)
{
	rtc->agora_rtc_event_handler.on_join_channel_success = __on_join_channel_success;
	rtc->agora_rtc_event_handler.on_error = __on_error;
	rtc->agora_rtc_event_handler.on_user_joined = __on_user_joined;
	rtc->agora_rtc_event_handler.on_user_offline = __on_user_offline;
	rtc->agora_rtc_event_handler.on_key_frame_gen_req = __on_key_frame_gen_req;
	rtc->agora_rtc_event_handler.on_audio_data = __on_audio_data;
	rtc->agora_rtc_event_handler.on_video_data = __on_video_data;
	rtc->agora_rtc_event_handler.on_target_bitrate_changed = __on_target_bitrate_changed;
	rtc->agora_rtc_event_handler.on_connection_lost = __on_connection_lost;
	rtc->agora_rtc_event_handler.on_rejoin_channel_success = __on_rejoin_channel_success;
	rtc->agora_rtc_event_handler.on_user_mute_audio = __on_user_mute_audio;
	rtc->agora_rtc_event_handler.on_user_mute_video = __on_user_mute_video;
}

static void __deep_copy_items_destroy(agora_rtc_t *rtc)
{
	if (rtc->agora_rtc_option.p_channel_name != NULL) {
		os_free((void *)rtc->agora_rtc_option.p_channel_name);
		rtc->agora_rtc_option.p_channel_name = NULL;
	}

	if (rtc->agora_rtc_option.p_token != NULL) {
		os_free((void *)rtc->agora_rtc_option.p_token);
		rtc->agora_rtc_option.p_token = NULL;
	}
}

static int32_t __agora_init(agora_rtc_config_t *p_config)
{
	int rval;
	uint32_t bwe_min_bps = DEFAULT_BANDWIDTH_ESTIMATE_MIN_BITRATE;
	uint32_t bwe_max_bps = DEFAULT_BANDWIDTH_ESTIMATE_MAX_BITRATE;
	uint32_t bwe_start_bps = DEFAULT_BANDWIDTH_ESTIMATE_START_BITRATE;
	rtc_service_option_t service_opt = { 0 };

	agora_rtc_t *rtc = __get_rtc_instance();

	rtc->state = AGORA_RTC_STATE_NULL;
	rtc->audio_rx_data_handle = NULL;
	rtc->conn_id = (connection_id_t)0;
	rtc->user_message_callback = NULL;
	rtc->b_user_joined = false;
	rtc->b_channel_joined = false;
	rtc->agora_rtc_event_handler.on_audio_data = NULL;
	rtc->agora_rtc_event_handler.on_connection_lost = NULL;
	rtc->agora_rtc_event_handler.on_error = NULL;
	rtc->agora_rtc_event_handler.on_join_channel_success = NULL;
#ifdef CONFIG_RTCM
	rtc->agora_rtc_event_handler.on_media_ctrl_receive = NULL;
#endif
	rtc->agora_rtc_event_handler.on_mixed_audio_data = NULL;
	rtc->agora_rtc_event_handler.on_rejoin_channel_success = NULL;
	rtc->agora_rtc_event_handler.on_token_privilege_will_expire = NULL;
	rtc->agora_rtc_event_handler.on_user_joined = NULL;
	rtc->agora_rtc_event_handler.on_user_mute_audio = NULL;
	rtc->agora_rtc_event_handler.on_user_mute_video = NULL;
	rtc->agora_rtc_event_handler.on_video_data = NULL;
	rtc->agora_rtc_event_handler.on_target_bitrate_changed = NULL;
	rtc->agora_rtc_event_handler.on_key_frame_gen_req = NULL;
	rtc->agora_rtc_event_handler.on_user_offline = NULL;
	rtc->target_bitrate = 0;
	rtc->audio_rx_data_handle = NULL;
	rtc->video_rx_data_handle = NULL;

	rtc->agora_rtc_config.license[0] = '\0';
	rtc->agora_rtc_config.enable_bwe_param = true;
	rtc->agora_rtc_config.bwe_param_max_bps = bwe_max_bps;
	rtc->agora_rtc_config.log_disable = true;
	rtc->agora_rtc_config.area_code = AREA_CODE_GLOB;

	rtc->agora_rtc_config.p_appid = (char *)os_malloc(os_strlen(p_config->p_appid) + 1);
	if (rtc->agora_rtc_config.p_appid == NULL) {
		LOGE("malloc app_id fail, size: %d \n", (os_strlen(p_config->p_appid) + 1));
		goto agora_init_error;
	}
	os_strcpy((char *)rtc->agora_rtc_config.p_appid, p_config->p_appid);
	LOGI("Agora RTC SDK app_id: %s \n", rtc->agora_rtc_config.p_appid);

	os_memcpy(rtc->agora_rtc_config.license, p_config->license, 32);
	LOGI("Agora RTC SDK license: ");
	for (int n = 0; n < 32; n++) {
		os_printf( "%c", rtc->agora_rtc_config.license[n]);
	}
	os_printf("\n");

	rtc->agora_rtc_config.enable_bwe_param = p_config->enable_bwe_param;
	rtc->agora_rtc_config.bwe_param_max_bps = p_config->bwe_param_max_bps;
	rtc->agora_rtc_config.log_disable = p_config->log_disable;
	rtc->agora_rtc_config.area_code = p_config->area_code;

	if (rtc->agora_rtc_config.enable_bwe_param) {
		bwe_max_bps = p_config->bwe_param_max_bps;
	} else {
		rtc->agora_rtc_config.bwe_param_max_bps = bwe_max_bps;
	}

	// step1. agora sdk version.
	LOGI("Agora RTC SDK v%s \n", agora_rtc_get_version());
	//step4. register event handler
	__register_agora_rtc_event_handler(rtc);

	//step6. agora rtc init.
	service_opt.area_code = p_config->area_code;
	service_opt.log_cfg.log_disable = p_config->log_disable;
	os_memcpy(service_opt.license_value, p_config->license, 33);

//	service_opt.area_code           = AREA_CODE_GLOB;
//	service_opt.log_cfg.log_disable = true;
//	service_opt.license_value[0]    = '\0';
	rval = agora_rtc_init(rtc->agora_rtc_config.p_appid, &rtc->agora_rtc_event_handler, &service_opt);
	if (rval < 0) {
		LOGI("agora rtc init failed, rval=%d error=%s\n", rval, agora_rtc_err_2_str(rval));
		goto agora_init_error;
	}

	LOGI("BWE[%u,%u,%u]\n", bwe_min_bps, bwe_max_bps, bwe_start_bps);
	rval = agora_rtc_set_bwe_param(CONNECTION_ID_ALL, bwe_min_bps, bwe_max_bps, bwe_start_bps);
	if (rval < 0) {
		LOGI("set BWE failed \n");
	} else {
		LOGI("agora_rtc_set_bwe_param ok \n");
	}

	return 0;

agora_init_error:
	if (rtc->agora_rtc_config.p_appid != NULL) {
		os_free((void *)rtc->agora_rtc_config.p_appid);
		rtc->agora_rtc_config.p_appid = NULL;
	}

	return -1;
}

bk_err_t bk_agora_rtc_create(agora_rtc_config_t *p_config, agora_rtc_msg_notify_cb message_callback)
{
	if (0 != __agora_init(p_config)) {
		LOGE("__agora_init fail.\n");
		return BK_FAIL;
	} else {
		LOGI("__agora_init ok.\n");
	}

	agora_rtc_t *rtc = __get_rtc_instance();
	if (rtc) {
		__register_message_router(rtc, message_callback);
	} else {
		return BK_FAIL;
	}
	LOGI("rtc create successfully.\n");
	return BK_OK;
}

bk_err_t bk_agora_rtc_destroy(void)
{
	int rval = agora_rtc_fini();
	if (rval != 0) {
		LOGI("agora_rtc_fini fail: %d, %s \n", rval, agora_rtc_err_2_str(rval));
	}

	agora_rtc_t *rtc = __get_rtc_instance();
	rtc->state = AGORA_RTC_STATE_IDLE;

	LOGI("rtc destroy successfully.\n");
	return BK_OK;
}

bk_err_t bk_agora_rtc_start(agora_rtc_option_t *option)
{
	int rval = 0;
	agora_rtc_t *rtc = __get_rtc_instance();
	if (!rtc)
		return BK_FAIL;

	/*copy channel name */
	if (rtc->agora_rtc_option.p_channel_name) {
		os_free((void *)rtc->agora_rtc_option.p_channel_name);
		rtc->agora_rtc_option.p_channel_name = NULL;
	}
	rtc->agora_rtc_option.p_channel_name = (char *)os_malloc(os_strlen(option->p_channel_name) + 1);
	if (rtc->agora_rtc_option.p_channel_name == NULL) {
		LOGE("malloc channel_name fail, size: %d \n", (os_strlen(option->p_channel_name) + 1));
		goto agora_rtc_start_fail;
	}
	os_strcpy((void *)rtc->agora_rtc_option.p_channel_name, option->p_channel_name);
	LOGI("Agora RTC SDK channel_name: %s \n", rtc->agora_rtc_option.p_channel_name);

	/*copy token */
	if (option->p_token) {
		if (rtc->agora_rtc_option.p_token) {
			os_free((void *)rtc->agora_rtc_option.p_token);
			rtc->agora_rtc_option.p_token = NULL;
		}
		rtc->agora_rtc_option.p_token = (char *)os_malloc(os_strlen(option->p_token) + 1);
		if (rtc->agora_rtc_option.p_token == NULL) {
			LOGE("malloc token fail, size: %d \n", (os_strlen(option->p_token) + 1));
			goto agora_rtc_start_fail;
		}
		os_strcpy((void *)rtc->agora_rtc_option.p_token, option->p_token);
	} else {
		rtc->agora_rtc_option.p_token = NULL;
	}
	LOGI("Agora RTC SDK token: %s \n", rtc->agora_rtc_option.p_token);

	rtc->agora_rtc_option.uid = option->uid;
	rtc->agora_rtc_option.auto_subscribe_audio = option->auto_subscribe_audio;
	rtc->agora_rtc_option.auto_subscribe_video = option->auto_subscribe_video;
	rtc->agora_rtc_option.audio_config.audio_data_type = option->audio_config.audio_data_type;
	rtc->agora_rtc_option.audio_config.pcm_sample_rate = option->audio_config.pcm_sample_rate;
	rtc->agora_rtc_option.audio_config.pcm_channel_num = option->audio_config.pcm_channel_num;

	rtc_channel_options_t channel_options = { 0 };

	LOGI("Subscribe audio and video by default \n");
	channel_options.auto_subscribe_audio = rtc->agora_rtc_option.auto_subscribe_audio;
	channel_options.auto_subscribe_video = rtc->agora_rtc_option.auto_subscribe_video;
	channel_options.audio_codec_opt.audio_codec_type = AUDIO_CODEC_DISABLED;

	LOGI("auto_subscribe_audio: %d, auto_subscribe_video: %d \n", 
				channel_options.auto_subscribe_audio ? 1 : 0, channel_options.auto_subscribe_video ? 1 : 0);

	// step8. join channel
	rval = agora_rtc_create_connection(&(rtc->conn_id));
	if (0 != rval) {
		LOGI("agora_rtc_create_connection failure: %d, %s \n", rval, agora_rtc_err_2_str(rval));
		goto agora_rtc_start_fail;
	}

	rval = agora_rtc_join_channel(rtc->conn_id, rtc->agora_rtc_option.p_channel_name, rtc->agora_rtc_option.uid, rtc->agora_rtc_option.p_token, &channel_options);
//	rval = agora_rtc_join_channel(rtc->conn_id, DEFAULT_CHANNEL_NAME, DEFAULT_USER_ID, DEFAULT_TOKEN, &channel_options);
	if (rval < 0) {
		LOGI("join channel %s failed, rval=%d error=%s \n",
			rtc->agora_rtc_option.p_channel_name ? rtc->agora_rtc_option.p_channel_name : "", rval, agora_rtc_err_2_str(rval));
		goto agora_rtc_start_fail;
	}

	LOGI("Joining channel %s ... \n", rtc->agora_rtc_option.p_channel_name);
	return 0;

agora_rtc_start_fail:
	__deep_copy_items_destroy(rtc);
	return -1;
}

bk_err_t bk_agora_rtc_stop(void)
{
	int rval = 0;
	agora_rtc_t *rtc = __get_rtc_instance();

	if (!rtc)
		return -1;

	__rtc_stopped(rtc);

	/*  maybe lost connect. but API has covered this case */
	rval = agora_rtc_leave_channel(rtc->conn_id);
	if (rval < 0) {
		LOGI("agora_rtc_leave_channel fail, rval=%d error=%s \n", rval, agora_rtc_err_2_str(rval));
		return -1;
	}

	rval = agora_rtc_destroy_connection(rtc->conn_id);
	if (rval < 0) {
		LOGI("agora_rtc_destroy_connection fail, rval=%d error=%s \n", rval, agora_rtc_err_2_str(rval));
		return -1;
	}

	if (rtc->agora_rtc_config.p_appid != NULL) {
		os_free((void *)rtc->agora_rtc_config.p_appid);
		rtc->agora_rtc_config.p_appid = NULL;
	}

	__deep_copy_items_destroy(rtc);

	return BK_OK;
}

int bk_agora_rtc_video_data_send(uint8_t *data_ptr, size_t data_len, video_frame_info_t *info_ptr)
{
	int rval = 0;

	agora_rtc_t *rtc = __get_rtc_instance();

	if (!rtc)
		return -1;

	// API: send audio data
	video_frame_info_t info = { 0 };
	info.data_type = info_ptr->data_type;
	info.frame_rate = info_ptr->frame_rate;
	info.frame_type = info_ptr->frame_type;
	info.stream_type = info_ptr->stream_type;

	rval = agora_rtc_send_video_data(rtc->conn_id, data_ptr, data_len, &info);
	if (rval < 0) {
		LOGI("send video data failed, rval=%d data_type=%d len=%d frame_type=%d \n",
				rval, info.data_type, (int)data_len, info.frame_type);
	} else {
	//LOGI( "send video data successfully. len=%d", (int)data_len);
	}

	return rval;
}

int bk_agora_rtc_audio_data_send(uint8_t *data_ptr, size_t data_len, audio_frame_info_t *info_ptr)
{
	// API: send audio data
	audio_frame_info_t info = { 0 };
	agora_rtc_t *rtc = __get_rtc_instance();

	if (!rtc)
		return -1;

	info.data_type = info_ptr->data_type;

//	addAON_GPIO_Reg0x3 = 2;
	int rval = agora_rtc_send_audio_data(rtc->conn_id, data_ptr, data_len, &info);
	if (rval < 0) {
		LOGE("Failed to send audio data, reason: %s\n", agora_rtc_err_2_str(rval));
		return -1;
	}
//	addAON_GPIO_Reg0x3 = 0;

	return 0;
}

bk_err_t bk_aggora_rtc_register_audio_rx_handle(agora_rtc_audio_rx_data_handle audio_rx_handle)
{
	agora_rtc_t *rtc = __get_rtc_instance();

	if (!rtc)
		return -1;

	rtc->audio_rx_data_handle = audio_rx_handle;

	return BK_OK;
}

bk_err_t bk_aggora_rtc_register_video_rx_handle(agora_rtc_video_rx_data_handle video_rx_handle)
{
	agora_rtc_t *rtc = __get_rtc_instance();

	if (!rtc)
		return -1;

	rtc->video_rx_data_handle = video_rx_handle;

	return BK_OK;
}


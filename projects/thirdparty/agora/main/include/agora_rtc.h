/*************************************************************
 *
 * This is a part of the Agora Media Framework Library.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __AGORA_RTC_H__
#define __AGORA_RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

//#include <stdbool.h>

#include "agora_rtc_api.h"

typedef enum {
  AGORA_RTC_MSG_JOIN_CHANNEL_SUCCESS = 0,
  AGORA_RTC_MSG_USER_JOINED,
  AGORA_RTC_MSG_USER_OFFLINE,
  AGORA_RTC_MSG_CONNECTION_LOST,
  AGORA_RTC_MSG_INVALID_APP_ID,
  AGORA_RTC_MSG_INVALID_CHANNEL_NAME,
  AGORA_RTC_MSG_INVALID_TOKEN,
  AGORA_RTC_MSG_TOKEN_EXPIRED,

  AGORA_RTC_MSG_KEY_FRAME_REQUEST,
  AGORA_RTC_MSG_BWE_TARGET_BITRATE_UPDATE,
} agora_rtc_msg_e;

typedef enum {
	AGORA_RTC_STATE_NULL = 0,
	AGORA_RTC_STATE_IDLE,
	AGORA_RTC_STATE_WORKING,
} agora_rtc_state_t;

typedef struct {
  uint32_t target_bitrate;
} agora_rtc_bwe_t;

typedef struct {
	agora_rtc_msg_e code;

	union {
		agora_rtc_bwe_t bwe;
		uint32_t uid;
	} data;
} agora_rtc_msg_t;

typedef void (*agora_rtc_msg_notify_cb)(agora_rtc_msg_t *p_msg);
typedef int (*agora_rtc_audio_rx_data_handle)(unsigned char *data, unsigned int size, const audio_frame_info_t *info_ptr);
typedef int (*agora_rtc_video_rx_data_handle)(const unsigned char *data, unsigned int size, const video_frame_info_t *info_ptr);


typedef struct {
	char *p_appid;
	char license[33];

	bool enable_bwe_param;
	uint32_t bwe_param_max_bps;

	bool log_disable;
	area_code_e area_code;
} agora_rtc_config_t;

#define DEFAULT_AGORA_RTC_CONFIG() {                  \
        .p_appid = NULL,                              \
        .license = {0},                               \
        .enable_bwe_param = true,                     \
        .bwe_param_max_bps = 5000000,                 \
        .log_disable = true,                          \
        .area_code = AREA_CODE_GLOB,                  \
    }

typedef struct {
	audio_data_type_e audio_data_type;
	int pcm_sample_rate;
	int pcm_channel_num;
} agora_rtc_audio_config_t;

typedef struct {
	char *p_channel_name;
	char *p_token;
	uint32_t uid;

	bool auto_subscribe_audio;
	bool auto_subscribe_video;

	agora_rtc_audio_config_t audio_config;
} agora_rtc_option_t;

#define DEFAULT_AGORA_RTC_OPTION() {                                        \
        .p_channel_name = NULL,                                             \
        .p_token = NULL,                                                    \
        .uid = 0,                                                           \
        .auto_subscribe_audio = true,                                    \
        .auto_subscribe_video = false,                                       \
        .audio_config = {                                                   \
                            .audio_data_type = AUDIO_DATA_TYPE_PCMU,        \
                            .pcm_sample_rate = 8000,                        \
                            .pcm_channel_num = 1,                           \
                        },                                                  \
    }



typedef struct {
	agora_rtc_state_t state;
	connection_id_t conn_id;
	agora_rtc_msg_notify_cb user_message_callback;
	bool b_user_joined;
	bool b_channel_joined;
	agora_rtc_event_handler_t agora_rtc_event_handler;

	uint32_t target_bitrate;

	/* receive data handle */
	agora_rtc_audio_rx_data_handle audio_rx_data_handle;
	agora_rtc_video_rx_data_handle video_rx_data_handle;

	agora_rtc_config_t agora_rtc_config;
	agora_rtc_option_t agora_rtc_option;
} agora_rtc_t;


/* API */
bk_err_t bk_agora_rtc_create(agora_rtc_config_t *p_config, agora_rtc_msg_notify_cb message_callback);
bk_err_t bk_agora_rtc_destroy(void);
bk_err_t bk_agora_rtc_start(agora_rtc_option_t *option);
bk_err_t bk_agora_rtc_stop(void);
int bk_agora_rtc_video_data_send(uint8_t *data_ptr, size_t data_len, video_frame_info_t *info_ptr);
int bk_agora_rtc_audio_data_send(uint8_t *data_ptr, size_t data_len, audio_frame_info_t *info_ptr);
bk_err_t bk_aggora_rtc_register_audio_rx_handle(agora_rtc_audio_rx_data_handle audio_rx_handle);
bk_err_t bk_aggora_rtc_register_video_rx_handle(agora_rtc_video_rx_data_handle video_rx_handle);


#ifdef __cplusplus
}
#endif
#endif /* __AGORA_RTC_H__ */

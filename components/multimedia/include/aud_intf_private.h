// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <common/bk_include.h>
#include <components/aud_intf_types.h>
#include <modules/audio_ring_buff.h>
#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_TF
#include "ff.h"
#include "diskio.h"
#endif //CONFIG_AUD_TRAS_AEC_DUMP_MODE_TF
#endif //CONFIG_AUD_TRAS_AEC_DUMP_DEBUG

#ifdef __cplusplus
extern "C" {
#endif


/**************** audio interface common ****************/
typedef enum {
	EVENT_AUD_TRAS_COM_INIT,
	EVENT_AUD_TRAS_COM_DEINIT,
	EVENT_AUD_TRAS_COM_SET_MODE,
	EVENT_AUD_TRAS_COM_SET_MIC_GAIN,
	EVENT_AUD_TRAS_COM_SET_SPK_GAIN,
	EVENT_AUD_TRAS_COM_UAC_REGIS_CONT_STATE_CB,
	EVENT_AUD_TRAS_COM_UAC_AUTO_CONT_CTRL,
	EVENT_AUD_TRAS_COM_MAX,
} aud_tras_drv_com_event_t;

typedef struct {
	aud_intf_drv_setup_t setup;
//	void (*aud_tras_drv_com_event_cb)(aud_tras_drv_com_event_t event, bk_err_t result);
} aud_intf_drv_config_t;


/**************** audio interface mic ****************/
typedef enum {
	EVENT_AUD_TRAS_MIC_INIT,
	EVENT_AUD_TRAS_MIC_DEINIT,
	EVENT_AUD_TRAS_MIC_START,
	EVENT_AUD_TRAS_MIC_PAUSE,
	EVENT_AUD_TRAS_MIC_STOP,
	EVENT_AUD_TRAS_MIC_SET_SAMP_RATE,
	EVENT_AUD_TRAS_MIC_SET_CHL,
	EVENT_AUD_TRAS_MIC_MAX,
} aud_tras_drv_mic_event_t;

/* audio interface mic setup configuration */
typedef struct {
	aud_intf_mic_chl_t mic_chl;
	aud_adc_samp_rate_t samp_rate;		/**< mic sample rate */
	uint16_t frame_size;				/**< size: a frame packet mic data size(byte) */
	uint8_t mic_gain;					/**< audio adc gain: value range:0x0 ~ 0x3f */
	aud_intf_mic_type_t mic_type;		/**< audio mic type: uac or microphone */

//	void (*aud_tras_drv_mic_event_cb)(aud_tras_drv_mic_event_t event, bk_err_t result);
} aud_intf_mic_config_t;


/**************** audio interface speaker ****************/
typedef enum {
	EVENT_AUD_TRAS_SPK_INIT,
	EVENT_AUD_TRAS_SPK_DEINIT,
	EVENT_AUD_TRAS_SPK_START,
	EVENT_AUD_TRAS_SPK_PAUSE,
	EVENT_AUD_TRAS_SPK_STOP,
	EVENT_AUD_TRAS_SPK_SET_SAMP_RATE,
	EVENT_AUD_TRAS_SPK_SET_CHL,
	EVENT_AUD_TRAS_SPK_MAX,
} aud_tras_drv_spk_event_t;

/* audio interface speaker setup configuration */
typedef struct {
	aud_intf_spk_chl_t spk_chl;
	aud_dac_samp_rate_t samp_rate;		/**< speaker sample rate */
	uint16_t frame_size;				/**< size: a frame packet speaker data size(byte) */
	uint16_t spk_gain;					/**< audio dac gain: value range:0x0 ~ 0x3f */
	aud_dac_work_mode_t work_mode;				/**< audio dac mode: signal_ended/differen */
	aud_intf_spk_type_t spk_type;				/**< audio speaker type: uac or speaker */

	uint32_t *spk_rx_ring_buff;				/**< the ring buffer address of received speaker data */
	uint16_t fifo_frame_num;			/**< audio dac start work when the fram number received is equal to fifo_frame_num */
	RingBufferContext *spk_rx_rb;		/**< speaker received ring buffer context */

//	void (*aud_tras_drv_spk_event_cb)(aud_tras_drv_spk_event_t event, bk_err_t result);
} aud_intf_spk_config_t;


/**************** audio interface voice ****************/

typedef enum {
	EVENT_AUD_TRAS_VOC_INIT,
	EVENT_AUD_TRAS_VOC_DEINIT,
	EVENT_AUD_TRAS_VOC_START,
	EVENT_AUD_TRAS_VOC_PAUSE,
	EVENT_AUD_TRAS_VOC_STOP,
	EVENT_AUD_TRAS_VOC_CTRL_SPK,
	EVENT_AUD_TRAS_VOC_CTRL_MIC,
	EVENT_AUD_TRAS_VOC_CTRL_AEC,
	EVENT_AUD_TRAS_VOC_SET_AEC_PARA,
	EVENT_AUD_TRAS_VOC_GET_AEC_PARA,
	EVENT_AUD_TRAS_VOC_TX_DEBUG,
	EVENT_AUD_TRAS_VOC_RX_DEBUG,
	EVENT_AUD_TRAS_VOC_AEC_DEBUG,
#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_TF
	EVENT_AUD_TRAS_VOC_AEC_DUMP,
#endif
#endif
	EVENT_AUD_TRAS_VOC_MAX,
} aud_tras_drv_voc_event_t;

typedef struct {
	/* default value */
	uint16_t init_flags;
	/* aec */
	uint32_t mic_delay;		//set delay points of ref data according to dump data
	uint32_t ec_depth;		//recommended value range: 1~50, the greater the echo, the greater the value setting
	uint8_t ref_scale;		//value range:0,1,2, the greater the signal amplitude, the greater the setting
	uint8_t voice_vol;		//the voice volume level
	uint32_t TxRxThr;		//the max amplitude of rx audio data
	uint32_t TxRxFlr;		//the min amplitude of rx audio data
	/* ns */
	uint8_t ns_level;		//recommended value range: 1~8, the lower the noise, the lower the level
	uint8_t ns_para;		//value range:0,1,2, the lower the noise, the lower the level, the default valude is recommended
	/* drc */
	uint8_t drc;			//recommended value range:0x10~0x1f, the greater the value, the greater the volume
} aec_config_t;

/* audio aec parameters control msg */
typedef struct {
	aud_intf_voc_aec_para_t op;
	uint32_t value;
} aud_intf_voc_aec_ctl_t;

/* audio config */
typedef struct {
	uint8_t adc_gain;
	uint16_t dac_gain;
	uint16_t mic_samp_rate_points;		//the number of points in mic frame
	uint8_t mic_frame_number;			//the max frame number of mic ring buffer
	uint16_t speaker_samp_rate_points;	//the number of points in speaker frame
	uint8_t speaker_frame_number;		//the max frame number of speaker ring buffer
	aud_dac_work_mode_t spk_mode;			/**< audio spk mode: signal_ended/differen */
} aud_intf_aud_config_t;

typedef struct {
	uint32_t *buff_addr;
	bool busy_status;
} tx_buff_t;

/* tx buffer config of audio transfer */
typedef struct {
	bool tx_buff_status;		//malloc tx buffer complete
	tx_buff_t ping;
	tx_buff_t pang;
	uint16_t buff_length;		//byte
} tx_info_t;

/* rx buffer config of audio transfer */
typedef struct {
	bool rx_buff_status;			//rx buffer status, true:rx ring buffer init complete, false:rx ring buffer not init
	//uint32_t ring_buff_addr;		//rx ring buffer address
	int32_t *decoder_ring_buff;		//save need to decode dac data received from apk
	RingBufferContext *decoder_rb;	//decoder ring buffer context, cpu0 malloc and init
	uint32_t frame_size;			//the size (byte) of one frame
	uint8_t frame_num;				//the number of frame in ring buffer
	//uint8_t fill_num;				//fill frame number
	uint16_t rx_buff_seq_tail;		//the rx data sequence in tail of filled frames
	//uint16_t rx_buff_seq_head;	//the rx data sequence in head of filled frames
	uint16_t aud_trs_read_seq;		//save the sequence number of audio dac readed
	uint16_t fifo_frame_num;
} rx_info_t;

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_TF
typedef struct {
	/* aec dump config */
	int16_t *mic_dump_addr;
	int16_t *ref_dump_addr;
	int16_t *out_dump_addr;
	uint32_t len;
} aec_dump_t;
#endif
#endif

/* audio transfer driver setup config */
typedef struct {
	//audio_tras_drv_mode_t aud_trs_mode;
	aud_intf_voc_samp_rate_t samp_rate;
	aud_intf_aud_config_t aud_setup;
	bool aec_enable;
	aec_config_t *aec_setup;
	tx_info_t tx_info;
	rx_info_t rx_info;

	aud_intf_voc_data_type_t data_type;
	aud_intf_voc_mic_ctrl_t mic_en;			/**< mic default status */
	aud_intf_voc_spk_ctrl_t spk_en;			/**< spk default status */
	aud_intf_mic_type_t mic_type;			/**< audio mic type: uac or microphone */
	aud_intf_spk_type_t spk_type;			/**< audio speaker type: uac or speaker */

//	aud_cb_t aud_cb;
#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
#if CONFIG_AUD_TRAS_AEC_DUMP_MODE_TF
	aec_dump_t aec_dump;
#endif
#endif
//	void (*aud_tras_drv_voc_event_cb)(aud_tras_drv_voc_event_t event, bk_err_t result);
} aud_intf_voc_config_t;


/**************** audio interface information ****************/

/* audio interface status */
typedef enum {
	AUD_INTF_DRV_STA_NULL = 0,
	AUD_INTF_DRV_STA_IDLE,
	AUD_INTF_DRV_STA_WORK,
	AUD_INTF_DRV_STA_MAX,
} aud_intf_drv_status_t;

/* voice transfer status */
typedef enum {
	AUD_INTF_VOC_STA_NULL = 0,		/**< default status: the voice is not init */
	AUD_INTF_VOC_STA_IDLE,			/**< idle status: the voice is init */
	AUD_INTF_VOC_STA_START, 		/**< start status: the voice is playing */
	AUD_INTF_VOC_STA_STOP,			/**< stop status: the voice is stop */
	AUD_INTF_VOC_STA_MAX,
} aud_intf_voc_sta_t;

/* mic transfer status */
typedef enum {
	AUD_INTF_MIC_STA_NULL = 0,		/**< default status: the voice is not init */
	AUD_INTF_MIC_STA_IDLE,			/**< idle status: the voice is init */
	AUD_INTF_MIC_STA_START, 		/**< start status: the voice is playing */
	AUD_INTF_MIC_STA_STOP,			/**< stop status: the voice is stop */
	AUD_INTF_MIC_STA_MAX,
} aud_intf_mic_sta_t;

/* mic transfer status */
typedef enum {
	AUD_INTF_SPK_STA_NULL = 0,		/**< default status: the voice is not init */
	AUD_INTF_SPK_STA_IDLE,			/**< idle status: the voice is init */
	AUD_INTF_SPK_STA_START, 		/**< start status: the voice is playing */
	AUD_INTF_SPK_STA_STOP,			/**< stop status: the voice is stop */
	AUD_INTF_SPK_STA_MAX,
} aud_intf_spk_sta_t;

/* audio interface api excute result */
typedef struct {
	bool busy_status;
} aud_intf_api_ex_t;

/* save audio transfer information */
typedef struct {
	aud_intf_drv_status_t drv_status;
	aud_intf_voc_sta_t voc_status;
	aud_intf_mic_sta_t mic_status;
	aud_intf_spk_sta_t spk_status;

	aud_intf_drv_config_t drv_info;
	aud_intf_mic_config_t mic_info;
	aud_intf_spk_config_t spk_info;
	aud_intf_voc_config_t voc_info;

	aud_intf_api_ex_t api_info;
	aud_intf_uac_sta_t uac_status;
	bool uac_auto_connect;
	void (*aud_intf_uac_connect_state_cb)(uint8_t state);
} aud_intf_info_t;

#define DEFAULT_AUD_INTF_CONFIG() {                                                 \
        .drv_status = AUD_INTF_DRV_STA_NULL,                                        \
        .voc_status = AUD_INTF_VOC_STA_NULL,                                        \
        .mic_status = AUD_INTF_MIC_STA_NULL,                                        \
        .spk_status = AUD_INTF_SPK_STA_NULL,                                        \
        .drv_info = {                                                               \
                       .setup = {                                                   \
                                   .work_mode = AUD_INTF_WORK_MODE_NULL,            \
                                   .task_config = {.priority = 3},                  \
                                   .aud_intf_tx_mic_data = NULL,                    \
                                   .aud_intf_rx_spk_data = NULL,                    \
                                },                                                  \
                    },                                                              \
        .mic_info = {                                                               \
                       .mic_chl = AUD_INTF_MIC_CHL_MAX,                             \
                       .samp_rate = AUD_ADC_SAMP_RATE_MAX,                          \
                       .frame_size = 0,                                             \
                       .mic_gain = 0x2d,                                            \
                       .mic_type = AUD_INTF_MIC_TYPE_MAX,                           \
                    },                                                              \
        .spk_info = {                                                               \
                       .spk_chl = AUD_INTF_SPK_CHL_MAX,                             \
                       .samp_rate = AUD_DAC_SAMP_RATE_MAX,                          \
                       .frame_size = 0,                                             \
                       .spk_gain = 0x2d,                                            \
                       .work_mode = AUD_DAC_WORK_MODE_DIFFEN,                       \
                       .spk_type = AUD_INTF_SPK_TYPE_MAX,                           \
                       .spk_rx_ring_buff = NULL,                                    \
                       .fifo_frame_num = 0,                                         \
                       .spk_rx_rb = NULL,                                           \
                    },                                                              \
        .voc_info = {                                                               \
                       .samp_rate = AUD_INTF_VOC_SAMP_RATE_MAX,                     \
                       .aud_setup = {                                               \
                                       .adc_gain = 0x2d,                            \
                                       .dac_gain = 0x2d,                            \
                                       .mic_samp_rate_points = 0,                   \
                                       .mic_frame_number = 0,                       \
                                       .speaker_samp_rate_points = 0,               \
                                       .speaker_frame_number = 0,                   \
                                       .spk_mode = AUD_DAC_WORK_MODE_DIFFEN,        \
                                    },                                              \
                       .aec_enable = false,                                         \
                       .aec_setup = NULL,                                           \
                       .tx_info = {                                                 \
                                     .tx_buff_status = false,                       \
                                     .ping = {                                      \
                                                .buff_addr = NULL,                  \
                                                .busy_status = false,               \
                                             },                                     \
                                     .pang = {                                      \
                                                .buff_addr = NULL,                  \
                                                .busy_status = false,               \
                                             },                                     \
                                     .buff_length = 0,                              \
                                  },                                                \
                       .rx_info = {                                                 \
                                     .rx_buff_status = false,                       \
                                     .decoder_ring_buff = NULL,                     \
                                     .decoder_rb = NULL,                            \
                                     .frame_size = 0,                               \
                                     .frame_num = 0,                                \
                                     .rx_buff_seq_tail = 0,                         \
                                     .aud_trs_read_seq = 0,                         \
                                     .fifo_frame_num = 0,                           \
                                  },                                                \
                       .data_type = AUD_INTF_VOC_DATA_TYPE_MAX,                     \
                       .mic_en = AUD_INTF_VOC_MIC_CLOSE,                            \
                       .spk_en = AUD_INTF_VOC_SPK_CLOSE,                            \
                       .mic_type = AUD_INTF_MIC_TYPE_MAX,                           \
                       .spk_type = AUD_INTF_SPK_TYPE_MAX,                           \
                    },                                                              \
        .api_info = {                                                               \
                       .busy_status = false,                                        \
                    },                                                              \
        .uac_status = AUD_INTF_UAC_NORMAL_DISCONNECTED,                             \
        .uac_auto_connect = true,                                                   \
        .aud_intf_uac_connect_state_cb = NULL,                                      \
    }

/**************** audio interface information ****************/

typedef enum {
	AUD_INTF_IDLE = 0,
	AUD_INTF_TX_MIC_DATA,
	AUD_INTF_READ_DONE,
	AUD_INTF_WRITE_REQ,
	AUD_INTF_WRITE_DONE,
	AUD_INTF_READY,
	AUD_INTF_EXIT,
	AUD_INTF_MAX,
} aud_intf_opcode_t;

typedef struct {
	aud_intf_opcode_t op;
	uint32_t param1;
	uint32_t param2;
	uint32_t param3;
} aud_intf_msg_t;

/**************** audio interface avi ****************/
typedef enum {
	EVENT_AUD_TRAS_AVI_START,
	EVENT_AUD_TRAS_AVI_STOP,
	EVENT_AUD_TRAS_AVI_MAX,
} aud_tras_drv_avi_event_t;

typedef void (*aud_tras_drv_avi_event_cb)(aud_tras_drv_avi_event_t event, bk_err_t result);

#ifdef __cplusplus
}
#endif

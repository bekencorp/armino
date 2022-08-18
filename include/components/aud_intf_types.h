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
//#include "aud_tras_drv.h"
#include <driver/aud_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/**************** audio interface common ****************/
/* audio interface work mode */
typedef enum {
	AUD_INTF_WORK_MODE_NULL = 0,
	AUD_INTF_WORK_MODE_GENERAL,		/**< General mode: support record mic data and play speaker */
	AUD_INTF_WORK_MODE_VOICE,		/**< Voice mode: support voice transfer  */
	AUD_INTF_WORK_MODE_MAX,
} aud_intf_work_mode_t;

/* audio interface task configuration */
typedef struct {
	uint8_t priority;				/**< the priority of audio interface task */
} aud_intf_task_config_t;

/* audio interface driver setup configuration */
typedef struct {
	aud_intf_work_mode_t work_mode;
	aud_intf_task_config_t task_config;
	int (*aud_intf_tx_mic_data)(unsigned char *data, unsigned int size);	/**< the api is called when collecting a frame mic packet data is complete */
	bk_err_t (*aud_intf_rx_spk_data)(unsigned int size);							/**< the api is called when playing a frame speaker packet data is complete */
} aud_intf_drv_setup_t;


/**************** audio interface mic ****************/
/* audio interface mic channel enum */
typedef enum {
	AUD_INTF_MIC_CHL_MIC1 = 0,		/**< MIC1: only enable mic1 channel */
	AUD_INTF_MIC_CHL_DUAL,			/**< DUAL: enable mic1 channel both and MIC2 channel */
	AUD_INTF_MIC_CHL_MAX,
} aud_intf_mic_chl_t;

/* audio interface mic setup configuration */
typedef struct {
	aud_intf_mic_chl_t mic_chl;
	aud_adc_samp_rate_t samp_rate;		/**< mic sample rate */
	uint16_t frame_size;				/**< size: a frame packet mic data size(byte) */
	uint8_t mic_gain;					/**< audio adc gain: value range:0x0 ~ 0x3f, suggest:0x2d */
} aud_intf_mic_setup_t;


/**************** audio interface speaker ****************/
/* audio interface speaker channel enum */
typedef enum {
	AUD_INTF_SPK_CHL_SPK1 = 0,		/**< SPK1: only enable SPK1 channel */
	AUD_INTF_SPK_CHL_DUAL,			/**< DUAL: enable SPK1 channel both and SPK2 channel */
	AUD_INTF_SPK_CHL_MAX,
} aud_intf_spk_chl_t;

/* audio interface speaker setup configuration */
typedef struct {
	aud_intf_spk_chl_t spk_chl;
	aud_dac_samp_rate_t samp_rate;		/**< speaker sample rate */
	uint16_t frame_size;				/**< size: a frame packet speaker data size(byte) */
	uint8_t spk_gain;					/**< audio dac gain: value range:0x0 ~ 0x3f, suggest:0x2d */
} aud_intf_spk_setup_t;



/**************** audio interface voice ****************/
/* audio interface voice transfer sample rate */
typedef enum {
	AUD_INTF_VOC_SAMP_RATE_8K = 0,
	AUD_INTF_VOC_SAMP_RATE_16K,
	AUD_INTF_VOC_SAMP_RATE_MAX,
} aud_intf_voc_samp_rate_t;

/* audio interface voice transfer data type */
typedef enum {
	AUD_INTF_VOC_DATA_TYPE_G711A = 0,
	AUD_INTF_VOC_DATA_TYPE_PCM,
	AUD_INTF_VOC_DATA_TYPE_MAX,
} aud_intf_voc_data_type_t;

/* audio interface voice setup configuration */
typedef struct {
	bool aec_enable;						/**< true: enable aec, false: disable aec */
	aud_intf_voc_samp_rate_t samp_rate;		/**< voice transfer sample rate */
	aud_intf_voc_data_type_t data_type;		/**< voice data type */
	uint8_t mic_gain;						/**< mic gain: value range:0x0 ~ 0x3f, suggest:0x2d */
	uint8_t spk_gain;						/**< spk gain: value range:0x0 ~ 0x3f, suggest:0x2d */
} aud_intf_voc_setup_t;

#ifdef __cplusplus
}
#endif

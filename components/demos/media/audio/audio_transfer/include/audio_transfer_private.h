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
#include "audio_transfer_driver.h"
#include <driver/aud_types.h>
#include <components/audio_transfer_types.h>
#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
#include "ff.h"
#include "diskio.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	uint16_t adc_gain;
	uint16_t dac_gain;
	uint16_t mic_samp_rate_points;		//the number of points in mic frame
	uint8_t mic_frame_number;			//the max frame number of mic ring buffer
	uint16_t speaker_samp_rate_points;	//the number of points in speaker frame
	uint8_t speaker_frame_number;		//the max frame number of speaker ring buffer
} aud_setup_t;

/* audio transfer driver setup config */
typedef struct {
	//audio_tras_drv_mode_t aud_trs_mode;
	audio_tras_samp_rate_t samp_rate;
	aud_setup_t aud_setup;
	bool aec_enable;
	aec_config_t *aec_setup;
	tx_context_t tx_context;
	rx_context_t rx_context;
	aud_cb_t aud_cb;
#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	aec_dump_t aec_dump;
#endif
} aud_tras_drv_setup_t;

typedef enum {
	AUD_TRAS_STA_IDLE = 0,
	AUD_TRAS_STA_INITING,
	AUD_TRAS_STA_READY,
	AUD_TRAS_STA_WORKING,
	AUD_TRAS_STA_STOPING,
	AUD_TRAS_STA_MAX,
} audio_tras_status_t;

typedef struct {
	audio_tras_status_t status;
#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	char sin_file_name[50];
	char ref_file_name[50];
	char out_file_name[50];
	FIL sin_file;
	FIL ref_file;
	FIL out_file;
#endif
} audio_tras_info_t;

/* used in cpu0 */
typedef enum {
	AUD_TRAS_IDLE = 0,
	AUD_TRAS_TX_MIC_DATA,
	AUD_TRAS_READ_DONE,
	AUD_TRAS_WRITE_REQ,
	AUD_TRAS_WRITE_DONE,
	AUD_TRAS_READY,
	AUD_TRAS_EXIT,
	AUD_TRAS_MAX,
} audio_tras_opcode_t;

typedef struct {
	audio_tras_opcode_t op;
	uint32_t param1;
	uint32_t param2;
	uint32_t param3;
} audio_tras_msg_t;


#ifdef __cplusplus
}
#endif

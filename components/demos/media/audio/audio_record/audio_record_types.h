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

#ifdef __cplusplus
extern "C" {
#endif


#define AUD_8K_FRAME_SAMP_SIZE           160*2
//#define MB_CHNL_AUD     2

typedef enum {
	ADC_TEST_MODE_NULL = 0,
	ADC_TEST_MODE_MCP,
	ADC_TEST_MODE_DMA,
	ADC_TEST_MODE_MAX
} aud_test_mode_t;

/* used in cpu1 */
typedef enum {
	AUDIO_SAMP_RATE_8K = 0,
	AUDIO_SAMP_RATE_16K,
	AUDIO_SAMP_RATE_MAX,
} audio_sample_rate_t;

typedef struct {
	audio_sample_rate_t samp_rate;
} audio_setup_t;

/* used in cpu0 */
typedef enum {
	AUDIO_IDLE = 0,
	AUDIO_MIC,
	AUDIO_EXIT,
	AUDIO_MAX,
} audio_record_opcode_t;

typedef struct {
	audio_record_opcode_t op;
} audio_record_msg_t;

#ifdef __cplusplus
}
#endif

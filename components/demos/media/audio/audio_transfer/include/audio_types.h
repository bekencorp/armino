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

#define psram_block_num    10
#define PSRAM_AUD_ADDR_BASE    0x60000000

#define AUD_AEC_8K_FRAME_SAMP_SIZE           160*2
#define AUD_AEC_16K_FRAME_SAMP_SIZE          AUD_AEC_8K_FRAME_SAMP_SIZE*2
#define PSRAM_AUD_ENCD_RING_BUFF_BASE        0x60000000
#define PSRAM_AUD_8K_ENCD_RING_BUFF_SIZE     AUD_AEC_8K_FRAME_SAMP_SIZE        //byte G711压缩后size小一倍
#define PSRAM_AUD_16K_ENCD_RING_BUFF_SIZE    AUD_AEC_8K_FRAME_SAMP_SIZE*2        //byte G711压缩后size小一倍
//#define PSRAM_AUD_8K_ENCD_TEMP_BUFF          AUD_AEC_8K_FRAME_SAMP_SIZE
//#define PSRAM_AUD_16K_ENCD_TEMP_BUFF         AUD_AEC_16K_FRAME_SAMP_SIZE

/* G711 decoder 每帧数据为一个字节，所以可以设置很小来节约memory，但是从系统task调度考虑，不易太小，*/
#define PSRAM_AUD_DECD_RING_BUFF_BASE        PSRAM_AUD_ENCD_RING_BUFF_BASE + AUD_AEC_8K_FRAME_SAMP_SIZE*2
#define PSRAM_AUD_DECD_RING_BUFF_SIZE        640     //根据解码器decoder每帧数据处理的尺寸设置，2*DECD_Frame_Size
#define AUD_DECD_FRAME_SAMP_SIZE             PSRAM_AUD_DECD_RING_BUFF_SIZE/2

//#define AUD_DMA_SIZE    0xFFFF
#define MB_CHNL_AUD     2

/* used in cpu1 */
typedef enum {
	AUDIO_SAMP_RATE_8K = 0,
	AUDIO_SAMP_RATE_16K,
	AUDIO_SAMP_RATE_MAX,
} audio_sample_rate_t;

typedef enum {
	AUDIO_CP1_IDLE = 0,
	AUDIO_CP1_AEC,
	AUDIO_CP1_ENCODER,
	AUDIO_CP1_DECODER,
	AUDIO_CP1_READ_REQ,
	AUDIO_CP1_WRITE_REQ,
	AUDIO_CP1_START,
	AUDIO_CP1_EXIT,
	AUDIO_CP1_MAILBOX,
	AUDIO_CP1_MAX,
} audio_cp1_opcode_t;

typedef struct {
	audio_cp1_opcode_t op;
} audio_cp1_msg_t;

typedef struct {
	audio_sample_rate_t samp_rate;
} audio_setup_t;

/* used in cpu0 */
typedef enum {
	AUDIO_CP0_IDLE = 0,
	AUDIO_CP0_GET_ENCODER_USED_SIZE_DONE,
	AUDIO_CP0_GET_DECODER_REMAIN_SIZE_DONE,
	AUDIO_CP0_READ_REQ,
	AUDIO_CP0_READ_DONE,
	AUDIO_CP0_WRITE_REQ,
	AUDIO_CP0_WRITE_DONE,
	AUDIO_CP0_EXIT,
	AUDIO_CP0_MAILBOX,
	AUDIO_CP0_MAX,
} audio_cp0_opcode_t;

typedef struct {
	audio_cp0_opcode_t op;
} audio_cp0_msg_t;

#ifdef __cplusplus
}
#endif

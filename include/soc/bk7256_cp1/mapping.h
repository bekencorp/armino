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

#define DISPLAY_FRAME_SIZE (800 * 600 * 2)
#define DISPLAY_720P_FRAME_SIZE (1280 * 720 * 2)
#define JPEG_DEC_ALINE_SIZE (1024 * 64)

#define JPEG_DEC_FRAME_SIZE ((DISPLAY_FRAME_SIZE / JPEG_DEC_ALINE_SIZE + 1) * JPEG_DEC_ALINE_SIZE)
#define JPEG_DEC_720P_FRAME_SIZE ((DISPLAY_720P_FRAME_SIZE / JPEG_DEC_ALINE_SIZE + 1) * JPEG_DEC_ALINE_SIZE)
#define JPEG_ENC_FRAME_SIZE (1024 * 250)
#define JPEG_ENC_FRAME_COUNT (4)
#define DISPLAY_FRAME_COUNT (4)
#define DISPLAY_720P_FRAME_COUNT (3)

#define AUDIO_FRAME_SIZE (1024 * 10)

#define PSRAM_NONCACHEABLE (0x60000000UL)
#define PSRAM_CACHEABLE (0x64000000UL)

typedef struct
{
	uint8_t display[DISPLAY_FRAME_COUNT][JPEG_DEC_FRAME_SIZE];
	uint8_t jpeg_enc[JPEG_ENC_FRAME_COUNT][JPEG_ENC_FRAME_SIZE];
	uint8_t aud_adc[AUDIO_FRAME_SIZE];
	uint8_t aud_dac[AUDIO_FRAME_SIZE];
} psram_map_t;

typedef struct
{
	uint8_t display[DISPLAY_720P_FRAME_COUNT][JPEG_DEC_720P_FRAME_SIZE];
	uint8_t jpeg_enc[JPEG_ENC_FRAME_COUNT][JPEG_ENC_FRAME_SIZE];
	uint8_t aud_adc[AUDIO_FRAME_SIZE];
	uint8_t aud_dac[AUDIO_FRAME_SIZE];
} psram_720p_map_t;

#define psram_map ((psram_map_t*)PSRAM_NONCACHEABLE)
#define psram_720p_map ((psram_720p_map_t*)PSRAM_NONCACHEABLE)


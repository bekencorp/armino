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

//#define AUDIO_MP3_PLAY_LIST_MAX		13


typedef enum {
	AUDIO_MP3_PLAY_PREV,
	AUDIO_MP3_PLAY_NEXT,
	AUDIO_MP3_PLAY_IDLE,
	AUDIO_MP3_PLAY_START,
	AUDIO_MP3_PLAY_STOP,
	AUDIO_MP3_PLAY_EXIT,
	ADUIO_MP3_PLAY_MAX,
} audio_mp3_play_opcode_t;

typedef struct {
	audio_mp3_play_opcode_t op;
} audio_mp3_play_msg_t;

#ifdef __cplusplus
}
#endif





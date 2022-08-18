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


#define AUD_8K_SPEAKER_SAMP_SIZE           160*2


/* used in cpu0 */
typedef enum {
	AUDIO_IDLE = 0,
	AUDIO_SPEAKER,
	AUDIO_EXIT,
	AUDIO_MAX,
} audio_play_opcode_t;

typedef struct {
	audio_play_opcode_t op;
} audio_play_msg_t;

#ifdef __cplusplus
}
#endif

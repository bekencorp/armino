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
#include <driver/media_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	media_trs_state_t state;
	uint8_t pause : 1;
	uint8_t debug : 1;
	uint32_t param;
} transfer_info_t;

typedef struct
{
	uint8_t id;
	uint8_t eof;
	uint8_t cnt;
	uint8_t size;
	uint8_t data[];
} transfer_data_t;

void transfer_event_handle(uint32_t event, uint32_t param);
void set_transfer_state(media_trs_state_t state);
media_trs_state_t get_transfer_state(void);
void transfer_init(void);


#ifdef __cplusplus
}
#endif

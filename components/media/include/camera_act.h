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
#include <driver/dvp_camera_types.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
	CAMERA_STATE_DISABLED,
	CAMERA_STATE_ENABLED,
} camera_state_t;

typedef struct
{
	media_camera_type_t type;
	camera_state_t state;
	uint32_t debug : 1;
	uint32_t param;
} camera_info_t;


void camera_event_handle(uint32_t event, uint32_t param);
void set_camera_state(camera_state_t state);
camera_state_t get_camera_state(void);
void camera_init(void);

#ifdef __cplusplus
}
#endif

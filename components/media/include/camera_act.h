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


typedef enum
{
	DVP_STATE_DISABLED,
	DVP_STATE_ENABLED,
} dvp_state_t;

typedef struct
{
	dvp_state_t state;
	uint32_t debug : 1;
	uint32_t param;
} dvp_info_t;

typedef enum
{
	UVC_STATE_DISABLED,
	UVC_STATE_ENABLED,
	UVC_STATE_STOP,
} uvc_state_t;

typedef struct
{
	uvc_state_t state;
	uint32_t debug : 1;
	uint32_t param;
} uvc_info_t;

void dvp_camera_event_handle(uint32_t event, uint32_t param);
void set_dvp_camera_state(dvp_state_t state);
dvp_state_t get_dvp_camera_state(void);
void dvp_camera_init(void);

void uvc_camera_event_handle(uint32_t event, uint32_t param);
void set_uvc_camera_state(uvc_state_t state);
uvc_state_t get_uvc_camera_state(void);
void uvc_camera_init(void);


#ifdef __cplusplus
}
#endif

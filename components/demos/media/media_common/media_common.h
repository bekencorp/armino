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


typedef enum {
	CMD_COM_MB_NULL = 0,
	CMD_COM_MB_AUD_TRAS_INIT,
	CMD_COM_MB_AUD_TRAS_INIT_CMPL,
	COM_MB_START_VIDEO_CMD,
	COM_MB_ACK,
	CMD_COM_MB_MAX_CMD,
} common_maibox_cmd_t;

typedef struct {
	common_maibox_cmd_t mb_cmd;
	uint32_t param1;
	uint32_t param2;
	uint32_t param3;
} common_mailbox_msg_t;

typedef enum {
	COM_IDLE = 0,
	COM_AUDIO,
	COM_VIDEO,
	COM_EXIT,
	COM_SEND,
	COM_MAX,
} com_opcode_t;

typedef struct {
	com_opcode_t op;
	uint32_t param;
} com_msg_t;

bk_err_t common_mb_init(void);
bk_err_t media_send_com_mb_msg(common_maibox_cmd_t cmd, uint32_t param1, uint32_t param2, uint32_t param3);

#ifdef __cplusplus
}
#endif

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

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	NETSTREAM_PLAYER_START,
	NETSTREAM_PLAYER_STOP,
	NETSTREAM_PLAYER_EXIT,
	NETSTREAM_PLAYER_MAX,
} netstream_player_opcode_t;

typedef struct {
	netstream_player_opcode_t op;
	const char *netstream_uri;
} netstream_player_msg_t;


bk_err_t netstream_player_stop(void);

bk_err_t netstream_player_send_msg(netstream_player_msg_t msg);

bk_err_t netstream_player_init(void);

bk_err_t netstream_player_deinit(void);


#ifdef __cplusplus
}
#endif



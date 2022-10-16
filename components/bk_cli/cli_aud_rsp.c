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

#include "cli.h"

extern void cli_aud_rsp_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);


#define AUD_RSP_CMD_CNT (sizeof(s_aud_rsp_commands) / sizeof(struct cli_command))
static const struct cli_command s_aud_rsp_commands[] = {
	{"aud_rsp_test", "aud_rsp_test {8000|16000|32000|48000 0|1 16 xxx.pcm 8000|16000|32000|48000 0|1 16 xxx.pcm 0~10}", cli_aud_rsp_test_cmd},
};

int cli_aud_rsp_init(void)
{
	return cli_register_commands(s_aud_rsp_commands, AUD_RSP_CMD_CNT);
}


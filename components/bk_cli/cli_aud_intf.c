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


extern void cli_aud_intf_record_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_play_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_sd_voc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_set_voc_param_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);


#define AUD_INTF_CMD_CNT (sizeof(s_aud_intf_commands) / sizeof(struct cli_command))
static const struct cli_command s_aud_intf_commands[] = {
	{"aud_intf_record_test", "aud_intf_record_test {start|pause|stop xx.pcm}", cli_aud_intf_record_cmd},
	{"aud_intf_play_test", "aud_intf_play_test {start|pause|stop xx.pcm}", cli_aud_intf_play_cmd},
	{"aud_intf_sd_voc_test", "aud_intf_sd_voc_test {start|stop xx.pcm, xx.pcm}", cli_aud_intf_sd_voc_cmd},
	{"aud_intf_voc_set_param_test", "aud_intf_voc_set_param_test {param value}", cli_aud_intf_set_voc_param_cmd},
};

int cli_aud_intf_init(void)
{
	return cli_register_commands(s_aud_intf_commands, AUD_INTF_CMD_CNT);
}


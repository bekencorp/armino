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


extern void cli_i2s_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_i2s_master_sin_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_i2s_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#if CONFIG_AUDIO
void cli_i2s_master_mic_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cli_i2s_slave_mic_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif

#define I2S_CMD_CNT (sizeof(s_i2s_commands) / sizeof(struct cli_command))
static const struct cli_command s_i2s_commands[] = {
	{"i2s_master_test", "i2s_master_test {start|stop}", cli_i2s_master_test_cmd},
	{"i2s_master_sin_test", "i2s_master_sin_test {start|stop}", cli_i2s_master_sin_test_cmd},
	{"i2s_slave_test", "i2s_slave_test {start|stop}", cli_i2s_slave_test_cmd},
#if CONFIG_AUDIO
	{"i2s_master_mic_test", "i2s_master_mic_test {start|stop}", cli_i2s_master_mic_test_cmd},
	{"i2s_slave_mic_test", "i2s_slave_mic_test {start|stop}", cli_i2s_slave_mic_test_cmd},
#endif
};

int cli_i2s_init(void)
{
	return cli_register_commands(s_i2s_commands, I2S_CMD_CNT);
}


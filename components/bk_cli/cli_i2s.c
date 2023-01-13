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
//extern void cli_i2s_master_sin_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
//extern void cli_i2s_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_i2s_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_pcm_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_pcm_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_dtm_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_dtm_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_2bd_master_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_2bd_slave_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define I2S_CMD_CNT (sizeof(s_i2s_commands) / sizeof(struct cli_command))
static const struct cli_command s_i2s_commands[] = {
	{"i2s_master_test", "i2s_master_test {start|stop}", cli_i2s_master_test_cmd},
	{"i2s_slave_test", "i2s_slave_test {start|stop}", cli_i2s_slave_test_cmd},
	{"pcm_master_test", "pcm_master_test {start|stop}", cli_pcm_master_test_cmd},
	{"pcm_slave_test", "pcm_slave_test {start|stop}", cli_pcm_slave_test_cmd},
	{"dtm_master_test", "dtm_master_test {start|stop}", cli_dtm_master_test_cmd},
	{"dtm_slave_test", "dtm_slave_test {start|stop}", cli_dtm_slave_test_cmd},
	{"2bd_master_test", "2bd_master_test {start|stop}", cli_2bd_master_test_cmd},
	{"2bd_slave_test", "2bd_slave_test {start|stop}", cli_2bd_slave_test_cmd},
};

int cli_i2s_init(void)
{
	return cli_register_commands(s_i2s_commands, I2S_CMD_CNT);
}


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


extern void cli_aud_adc_mcp_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_dtmf_mcp_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_adc_dma_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_adc_loop_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_dtmf_loop_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_pcm_mcp_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_pcm_dma_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_adc_to_file_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_adc_to_sd_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_enable_adc_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_eq_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_mic_to_pcm_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define AUD_CMD_CNT (sizeof(s_aud_commands) / sizeof(struct cli_command))
static const struct cli_command s_aud_commands[] = {
	{"aud_adc_mcp_test", "aud_adc_mcp_test {start|stop}", cli_aud_adc_mcp_test_cmd},
	{"aud_dtmf_mcp_test", "aud_dtmf_mcp_test {start|stop}", cli_aud_dtmf_mcp_test_cmd},
	{"aud_adc_dma_test", "aud_adc_dma_test {start|stop}", cli_aud_adc_dma_test_cmd},
	{"aud_adc_loop_test", "aud_adc_loop_test {start|stop}", cli_aud_adc_loop_test_cmd},
	{"aud_dtmf_loop_test", "aud_dtmf_loop_test {start|stop}", cli_aud_dtmf_loop_test_cmd},
	{"aud_pcm_mcp_test", "aud_pcm_mcp_test {8000|16000|44100|48000}", cli_aud_pcm_mcp_test_cmd},
	{"aud_pcm_dma_test", "aud_pcm_dma_test {8000|16000|44100|48000|stop}", cli_aud_pcm_dma_test_cmd},
	{"aud_adc_to_file_test", "aud_adc_to_file_test {start|stop}", cli_aud_adc_to_file_test_cmd},
//	{"aud_file_to_dac_test", "aud_file_to_dac_test {start|stop} {filename}", cli_aud_file_to_dac_test_cmd},
	{"aud_adc_to_sd_test", "aud_adc_to_sd_test {start|stop}", cli_aud_adc_to_sd_test_cmd},
	{"aud_enable_adc_test", "aud_enable_adc_test {start|stop}", cli_aud_enable_adc_test_cmd},
	{"aud_eq_test", "aud_eq_test {start|stop}", cli_aud_eq_test_cmd},
	{"aud_mic_to_pcm_test", "aud_mic_to_pcm_test {start|stop}", cli_aud_mic_to_pcm_cmd},
};

int cli_aud_init(void)
{
	return cli_register_commands(s_aud_commands, AUD_CMD_CNT);
}


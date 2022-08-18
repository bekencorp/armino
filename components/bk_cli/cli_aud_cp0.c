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

extern void cli_aud_cp0_adc_to_sd_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_cp0_psram_init_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_cp0_mic_to_pcm_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_cp0_sdcard_to_dac_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

extern void cli_aud_cp0_signal_to_dual_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_cp0_audio_record_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_cp0_audio_play_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define AUD_CP0_CMD_CNT (sizeof(s_aud_cp0_commands) / sizeof(struct cli_command))
static const struct cli_command s_aud_cp0_commands[] = {
	{"aud_cp0_adc_to_sd_test", "aud_cp0_adc_to_sd_test {start|stop}", cli_aud_cp0_adc_to_sd_test_cmd},
	{"aud_cp0_psram_init", "aud_cp0_psram_init {start|stop}", cli_aud_cp0_psram_init_cmd},
	{"aud_cp0_mic_to_pcm", "aud_cp0_mic_to_pcm {start|stop}", cli_aud_cp0_mic_to_pcm_cmd},
	{"aud_cp0_sdcard_play_test", "aud_cp0_sdcard_play_test{start}", cli_aud_cp0_sdcard_to_dac_test_cmd},
	{"aud_cp0_signal_to_dual_test", "aud_cp0_signal_to_dual_test", cli_aud_cp0_signal_to_dual_test_cmd},
	{"aud_cp0_audio_record_test", "aud_cp0_audio_record_test {start|stop xx.pcm}", cli_cp0_audio_record_cmd},
	{"aud_cp0_audio_play_test", "aud_cp0_audio_play_test {start|stop xx.pcm}", cli_cp0_audio_play_cmd},
};

int cli_aud_cp0_init(void)
{
	return cli_register_commands(s_aud_cp0_commands, AUD_CP0_CMD_CNT);
}


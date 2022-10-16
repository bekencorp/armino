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

extern void cli_aud_flac_decoder_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
/* memory is not enough */
//extern void cli_aud_flac_encoder_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);


#define AUD_FLAC_CMD_CNT (sizeof(s_aud_flac_commands) / sizeof(struct cli_command))
static const struct cli_command s_aud_flac_commands[] = {
	{"aud_flac_decoder_test", "aud_flac_decoder_test {xxx.flac xxx.wav}", cli_aud_flac_decoder_test_cmd},
	/* memory is not enough */
//	{"aud_flac_encoder_test", "aud_flac_encoder_test {xxx.wav xxx.flac}", cli_aud_flac_encoder_test_cmd},
};

int cli_aud_flac_init(void)
{
	return cli_register_commands(s_aud_flac_commands, AUD_FLAC_CMD_CNT);
}


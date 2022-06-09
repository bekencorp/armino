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

#include <os/os.h>
#include <os/str.h>
#include "sys_driver.h"
#include "audio_transfer_cp1.h"


static void cli_aud_cp1_help(void)
{
	os_printf("aud_cp1_audio_transfer_test {start|stop} \r\n");
}


void cli_cp1_audio_transfer_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;

	if (argc != 2) {
		cli_aud_cp1_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("cp1: start audio_transfer test \r\n");

		audio_setup_t config;
		config.samp_rate = AUDIO_SAMP_RATE_8K;
		/* init audio transfer task */
		ret = bk_audio_cp1_transfer_init(&config);
		if (ret != BK_OK) {
			os_printf("cp1: start audio_transfer fail \r\n");
			return;
		}

		os_printf("cp1: start audio_transfer test successful \r\n");
	} else {
		cli_aud_cp1_help();
		return;
	}

}


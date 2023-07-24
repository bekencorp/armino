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
#include "cli.h"
#include <driver/otp.h>

static void cli_otp_help(void)
{
	CLI_LOGI("otp_test read [addr] [length]\r\n");
}

static void cli_otp_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_otp_help();
		return;
	}

	uint32_t addr, length;
	uint8_t *buff_p, *record_p;

	if (os_strcmp(argv[1], "read") == 0) {
		addr   = os_strtoul(argv[2], NULL, 16);
		length = os_strtoul(argv[3], NULL, 16);

		record_p = buff_p = (uint8_t *)malloc(length);
		memset(buff_p, 0x0, length);

		BK_LOG_ON_ERR(bk_otp_read_bytes_nonsecure(buff_p, addr, length));

		for(int i=0; i<length; i++){
			bk_printf("%02x",*buff_p);
			buff_p++;
		}
		free(record_p);
		CLI_LOGI("\r\n read OTP suc! \r\n");
	} else {
		cli_otp_help();
		return;
	}
}

#define OTP_CMD_CNT (sizeof(s_otp_commands) / sizeof(struct cli_command))
static const struct cli_command s_otp_commands[] = {
	{"otp_test", "otp_test {read}", cli_otp_cmd}
};

int cli_otp_init(void)
{
	return cli_register_commands(s_otp_commands, OTP_CMD_CNT);
}

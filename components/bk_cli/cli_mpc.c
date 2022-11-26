// Copyright 2022 -2023 Beken
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
#include <driver/mpc.h>

static void cli_mpc_help(void)
{
	CLI_LOGI("mpc init - init mpc driver\r\n");
	CLI_LOGI("mpc deinit - deinit mpc driver\r\n");
	CLI_LOGI("mpc set {dev} {offset(hex)} {block_num} {1|0} - set lut\r\n");
	CLI_LOGI("mpc dump {dev} - dump lut bits\r\n");
	CLI_LOGI("mpc help - show this help\n");
}

static void cli_mpc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_mpc_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		CLI_LOGI("init mpc\r\n");
		BK_LOG_ON_ERR(bk_mpc_driver_init());
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		CLI_LOGI("deinit mpc\r\n");
		BK_LOG_ON_ERR(bk_mpc_driver_deinit());
	} else if (os_strcmp(argv[1], "set") == 0) {
		if (argc != 6) {
			cli_mpc_help();
			return;
		}

		mpc_dev_t dev =  os_strtoul(argv[2], NULL, 10);
		uint32_t offset = os_strtoul(argv[3], NULL, 16);
		uint32_t block_num = os_strtoul(argv[4], NULL, 10);
		uint32_t secure_type = os_strtoul(argv[5], NULL, 10);
		CLI_LOGI("mpc set, dev=%d offset=%x block_num=%d type=%d\r\n",
			dev, offset, block_num, secure_type);
		if (secure_type == 0) {
			BK_LOG_ON_ERR(bk_mpc_set_secure_attribute(dev, offset, block_num, MPC_BLOCK_NON_SECURE));
		} else {
			BK_LOG_ON_ERR(bk_mpc_set_secure_attribute(dev, offset, block_num, MPC_BLOCK_SECURE));
		}
	} else if (os_strcmp(argv[1], "dump") == 0) {
		if (argc != 3) {
			cli_mpc_help();
			return;
		}
		mpc_dev_t dev =  os_strtoul(argv[2], NULL, 10);
		bk_mpc_dump_secure_attribute(dev);
	} else if (os_strcmp(argv[1], "help") == 0) {
		cli_mpc_help();
	} else {
		cli_mpc_help();
		return;
	}
}

#define MPC_CMD_CNT (sizeof(s_mpc_commands) / sizeof(struct cli_command))
static const struct cli_command s_mpc_commands[] = {
	{"mpc", "{init|deinit|set|dump}", cli_mpc_cmd},
};

int cli_mpc_init(void)
{
	return cli_register_commands(s_mpc_commands, MPC_CMD_CNT);
}

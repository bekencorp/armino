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
#include  "driver/securityip.h"
static void cli_vault_help(void)
{
	CLI_LOGI("secure_vault_driver init\r\n");
	CLI_LOGI("secure_vault_driver deinit\r\n");
	
}
//extern bk_err_t bk_vault_driver_init(void);
//extern bk_err_t bk_vault_driver_deinit(void);
static void cli_vault_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_vault_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		bk_vault_driver_init();
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		bk_vault_driver_deinit();
	} else {
		cli_vault_help();
		return;
	}
}

static void cli_vault_system_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_vault_help();
		return;
	}

	if (os_strcmp(argv[1], "version") == 0) {
		
		CLI_LOGI("get vault system version sucess!\r\n");
	} else if (os_strcmp(argv[1], "state") == 0) {
		
		CLI_LOGI("get vault system state sucess!\r\n");
	}else if (os_strcmp(argv[1], "reset") == 0) {
		
		CLI_LOGI("vault system reset!\r\n");
	} else {
		cli_vault_help();
		return;
	}
}

#define VAULT_CMD_CNT (sizeof(s_vault_commands) / sizeof(struct cli_command))
static const struct cli_command s_vault_commands[] = {
	{"vault_driver", "vault_driver {init|deinit}", cli_vault_driver_cmd},
	{"vault_system", "vault_system {version|state|reset}", cli_vault_system_cmd}
};

int cli_vault_init(void)
{
	//BK_LOG_ON_ERR(bk_trng_driver_init());
	return cli_register_commands(s_vault_commands, VAULT_CMD_CNT);
}



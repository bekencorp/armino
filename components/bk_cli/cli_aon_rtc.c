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
#include <driver/aon_rtc.h>

static void cli_aon_rtc_help(void)
{
	CLI_LOGI("aon_rtc_driver init\r\n");
	CLI_LOGI("aon_rtc_driver deinit\r\n");
	CLI_LOGI("aon_rtc <id> create <tick> <period>\r\n");
	CLI_LOGI("aon_rtc <id> destroy\r\n");
	CLI_LOGI("aon_rtc_int <id> tick\r\n");
	CLI_LOGI("aon_rtc_int <id> upper\r\n");
}

static void cli_aon_rtc_tick_isr(aon_rtc_id_t id, void *param)
{
	CLI_LOGI("aon_rtc_tick_isr(%d)\n", id);
}

static void cli_aon_rtc_upper_isr(aon_rtc_id_t id, void *param)
{
	CLI_LOGI("aon_rtc_upper_isr(%d)\n", id);
}

static void cli_aon_rtc_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_aon_rtc_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_driver_init());
		CLI_LOGI("aon_rtc driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_driver_deinit());
		CLI_LOGI("aon_rtc driver deinit\n");
	} else {
		cli_aon_rtc_help();
		return;
	}
}

static void cli_aon_rtc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t aon_rtc_id;
	uint32_t tick;
	uint32_t period;

	aon_rtc_id = os_strtoul(argv[1], NULL, 10);
	if (os_strcmp(argv[2], "create") == 0) {
		tick = os_strtoul(argv[3], NULL, 10);
		period = os_strtoul(argv[4], NULL, 10);
		BK_LOG_ON_ERR(bk_aon_rtc_create(aon_rtc_id, tick, period));
		CLI_LOGI("aon_rtc id:%d bk_aon_rtc_create\n", aon_rtc_id);
	} else if (os_strcmp(argv[2], "destroy") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_destroy(aon_rtc_id));
		CLI_LOGI("aon_rtc id:%d bk_aon_rtc_destroy\n", aon_rtc_id);
	} else if (os_strcmp(argv[2], "init") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_tick_init());
		CLI_LOGI("aon_rtc id:%d bk_aon_rtc_tick_init\n", aon_rtc_id);
	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_destroy(aon_rtc_id));
		CLI_LOGI("aon_rtc id:%d bk_aon_rtc_deinit\n", aon_rtc_id);
	} else if (os_strcmp(argv[2], "set") == 0) {
		tick = os_strtoul(argv[3], NULL, 10);
		tick = 32 * 1000 * tick;
		BK_LOG_ON_ERR(bk_aon_rtc_create(0, tick, 1));
		CLI_LOGI("aon_rtc id:%d set rtc period = %d s.\n", aon_rtc_id, tick);
	} else if (os_strcmp(argv[2], "get") == 0) {
		tick = bk_aon_rtc_get_current_tick();
		tick = tick / 32000;
		CLI_LOGI("aon_rtc id:%d get rtc tick time = %d s.\n", aon_rtc_id, tick);
	} else {
		cli_aon_rtc_help();
		return;
	}
}

static void cli_aon_rtc_int_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t aon_rtc_id;

	if (argc != 3) {
		cli_aon_rtc_help();
		return;
	}

	aon_rtc_id = os_strtoul(argv[1], NULL, 10);
	if (os_strcmp(argv[2], "tick") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_register_tick_isr(aon_rtc_id, cli_aon_rtc_tick_isr, NULL));
		CLI_LOGI("aon_rtc id:%d register tick interrupt isr\n", aon_rtc_id);
	} else if (os_strcmp(argv[2], "upper") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_register_upper_isr(aon_rtc_id, cli_aon_rtc_upper_isr, NULL));
		CLI_LOGI("aon_rtc id:%d register upper interrupt isr\n", aon_rtc_id);
	} else {
		cli_aon_rtc_help();
		return;
	}
}

#define EFUSE_CMD_CNT (sizeof(s_aon_rtc_commands) / sizeof(struct cli_command))
static const struct cli_command s_aon_rtc_commands[] = {
	{"aon_rtc_driver", "aon_rtc_driver {init|deinit}", cli_aon_rtc_driver_cmd},
	{"aon_rtc", "aon_rtc {id} {create|destroy}", cli_aon_rtc_cmd},
	{"aon_rtc_int", "aon_rtc_int {id} {tick|upper}", cli_aon_rtc_int_cmd},
};

int cli_aon_rtc_init(void)
{
	BK_LOG_ON_ERR(bk_aon_rtc_driver_init());
	return cli_register_commands(s_aon_rtc_commands, EFUSE_CMD_CNT);
}


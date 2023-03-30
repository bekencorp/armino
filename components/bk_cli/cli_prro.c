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
#include "partitions.h"
#include "soc/soc.h"
#include "driver/prro.h"

static void cli_prro_help(void)
{
	CLI_LOGI("prro help - display this menu\r\n");

	CLI_LOGI("prro secure {dev_id} {0|1} - configure device secure\n");
	CLI_LOGI("prro secure gpio {gpio_0_31} {gpio_32_63} - configure gpio gpios secure\n");
	CLI_LOGI("prro secure ahb {ahb_bits} - configure ahb secure\n");
	CLI_LOGI("prro secure apb {apb_bits} - configure apb secure\n");
	CLI_LOGI("prro secure aon {aon_bits} - configure aon secure\n");

	CLI_LOGI("prro privilege {dev_id} {0|1} - configure device privilege\n");
	CLI_LOGI("prro privilege gpio {grou0} {group1} - configure gpio privilege\n");
	CLI_LOGI("prro privilege ahb {ahb_bits} - configure ahb privilege\n");
	CLI_LOGI("prro privilege apb {apb_bits} - configure apb privilege\n");
	CLI_LOGI("prro cmp condition {0|1|2} {start} {end} - configure compare condition\n");
	CLI_LOGI("prro cmp src {0|1|2} {src} - configure compare src\n");
	CLI_LOGI("prro cmp dst {0|1|2} {dst} - configure compare dst\n");
}

static void cli_prro_secure_cmd(int argc, char **argv)
{
	if (argc < 4) {
		cli_prro_help();
		return;
	}

	if (os_strcmp(argv[2], "gpio") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 5);
		uint32_t g0_secure_bits = os_strtoul(argv[3], NULL, 16);
		uint32_t g1_secure_bits = os_strtoul(argv[4], NULL, 16);
		CLI_LOGI("set gpio to g0-31=%x, g32-63=%x\r\n", g0_secure_bits, g1_secure_bits);
		BK_LOG_ON_ERR(bk_prro_set_gpios_secure(g0_secure_bits, g1_secure_bits));
	} else if (os_strcmp(argv[2], "ahb") == 0) {
		uint32_t ahb_secure_bits = os_strtoul(argv[3], NULL, 16);
		CLI_LOGI("set ahb to %x\r\n", ahb_secure_bits);
		BK_LOG_ON_ERR(bk_prro_set_ahb_dev_secure(ahb_secure_bits));
	} else if (os_strcmp(argv[2], "apb") == 0) {
		uint32_t apb_secure_bits = os_strtoul(argv[3], NULL, 16);
		CLI_LOGI("set apb to %x\r\n", apb_secure_bits);
		BK_LOG_ON_ERR(bk_prro_set_apb_dev_secure(apb_secure_bits));
	} else if (os_strcmp(argv[2], "aon") == 0) {
		uint32_t aon_secure_bits = os_strtoul(argv[3], NULL, 16);
		CLI_LOGI("set aon to %x\r\n", aon_secure_bits);
		//TODO
		REG_WRITE(0x41040000 + (0xa << 2), aon_secure_bits);
	} else {
		uint32_t dev_id = os_strtoul(argv[2], NULL, 10);
		uint32_t is_secure = os_strtoul(argv[3], NULL, 10);
		CLI_LOGI("set dev%d to %s\r\n", dev_id, is_secure ? "s" : "ns");
		if (is_secure) {
			BK_LOG_ON_ERR(bk_prro_set_secure(dev_id, PRRO_SECURE));
		} else {
			BK_LOG_ON_ERR(bk_prro_set_secure(dev_id, PRRO_NON_SECURE));
		}
	}
}

static void cli_prro_privilege_cmd(int argc, char **argv)
{
	if (argc < 4) {
		cli_prro_help();
		return;
	}

	if (os_strcmp(argv[2], "ahb") == 0) {
		uint32_t ahb_privilege_bits = os_strtoul(argv[3], NULL, 16);
		CLI_LOGI("set ahb to %x\r\n", ahb_privilege_bits);
		BK_LOG_ON_ERR(bk_prro_set_ahb_dev_privilege(ahb_privilege_bits));
	} else if (os_strcmp(argv[2], "apb") == 0) {
		uint32_t apb_privilege_bits = os_strtoul(argv[3], NULL, 16);
		CLI_LOGI("set apb to %x\r\n", apb_privilege_bits);
		BK_LOG_ON_ERR(bk_prro_set_apb_dev_privilege(apb_privilege_bits));
	} else {
		CLI_RET_ON_INVALID_ARGC(argc, 5);
		uint32_t dev_id = os_strtoul(argv[3], NULL, 10);
		uint32_t is_privilege = os_strtoul(argv[4], NULL, 10);
		CLI_LOGI("set dev%d to %s\r\n", dev_id, is_privilege ? "privilege" : "non-provilege");
		if (is_privilege) {
			BK_LOG_ON_ERR(bk_prro_set_privilege(dev_id, PRRO_SECURE));
		} else {
			BK_LOG_ON_ERR(bk_prro_set_privilege(dev_id, PRRO_NON_SECURE));
		}
	}
}

static void cli_prro_cmp_cmd(int argc, char **argv)
{
	if (argc < 4) {
		cli_prro_help();
		return;
	}

	if (os_strcmp(argv[2], "condition") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 5);
		uint32_t cmp_id = os_strtoul(argv[2], NULL, 10);
		uint32_t start = os_strtoul(argv[3], NULL, 16);
		uint32_t end = os_strtoul(argv[4], NULL, 16);
		CLI_LOGI("set cmp%s start=%x end=%x\r\n", cmp_id, start, end);
		BK_LOG_ON_ERR(bk_prro_set_hw_cmp_condition(cmp_id, start, end));
	} else if (os_strcmp(argv[2], "src") == 0) {
		uint32_t cmp_id = os_strtoul(argv[2], NULL, 10);
		uint32_t src = os_strtoul(argv[3], NULL, 16);
		CLI_LOGI("set cmp%s src=%x\r\n", cmp_id, src);
		BK_LOG_ON_ERR(bk_prro_set_hw_cmp_src(cmp_id, src));
	} else if (os_strcmp(argv[2], "dst") == 0) {
		uint32_t cmp_id = os_strtoul(argv[2], NULL, 10);
		uint32_t dst = os_strtoul(argv[3], NULL, 16);
		CLI_LOGI("set cmp%s dst=%x\r\n", cmp_id, dst);
		BK_LOG_ON_ERR(bk_prro_set_hw_cmp_dst(cmp_id, dst));
	} else  {
		cli_prro_help();
	}
}

static void cli_prro_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_prro_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		bk_prro_driver_init();
	} else if (os_strcmp(argv[1], "secure") == 0) {
		cli_prro_secure_cmd(argc, argv);
	} else if (os_strcmp(argv[1], "privilege") == 0) {
		cli_prro_privilege_cmd(argc, argv);
	} else if (os_strcmp(argv[1], "cmp") == 0) {
		cli_prro_cmp_cmd(argc, argv);
	} else {
		cli_prro_help();
	}
}

#define PRRO_CMD_CNT (sizeof(s_prro_commands) / sizeof(struct cli_command))
static const struct cli_command s_prro_commands[] = {
	{"prro", "prro {secure|privilege|cmp} ...", cli_prro_cmd},
};

int cli_prro_init(void)
{
	return cli_register_commands(s_prro_commands, PRRO_CMD_CNT);
}


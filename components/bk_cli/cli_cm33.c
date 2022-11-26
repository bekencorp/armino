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
#include "driver/mpc.h"

extern void jump_to_s(uint32_t addr);
extern void jump_to_ns(uint32_t addr);
extern void arch_interrupt_disable(void);
extern void arch_interrupt_retarget(void);

static void cli_cm33_help(void)
{
	CLI_LOGI("jump {s|ns}\n");
}

static void ns_config(void)
{
	bk_mpc_driver_init();
	bk_mpc_set_secure_attribute(MPC_DEV_FLASH, 256<<10, 10, MPC_BLOCK_NON_SECURE);

	bk_mpc_set_secure_attribute(MPC_DEV_SMEM3, 0, 32, MPC_BLOCK_NON_SECURE);
	bk_mpc_set_secure_attribute(MPC_DEV_SMEM4, 0, 32, MPC_BLOCK_NON_SECURE);

	bk_mpc_dump_secure_attribute(MPC_DEV_FLASH);
	bk_mpc_dump_secure_attribute(MPC_DEV_SMEM3);
	bk_mpc_dump_secure_attribute(MPC_DEV_SMEM4);
	CLI_LOGI("wait for TX over\r\n");

	while (!bk_uart_is_tx_over(UART_ID_0));

	//REG_SET(0x41040000 + 5 * 4, 1, 2, 3);
	REG_WRITE(0x41040000 + 5 * 4, 0xFFFFFFFF);
	//REG_SET(0x41040000 + 8 * 4, 0, 2, 7);
	REG_WRITE(0x41040000 + 8 * 4, 0xFFFFFFFF);
	//REG_WRITE(0x41040000 + 0xb * 4, 0xFFFF0000);
	REG_WRITE(0x41040000 + 0xb * 4, 0xFFFFFFFF);
	REG_WRITE(0x41040000 + 0xc * 4, 0xFFFFFFFF);
}

void jump_test(void)
{
	uint32_t addr = SOC_GET_NS_ADDR(CONFIG_PRIMARY_NS_VIRTUAL_CODE_START + SOC_FLASH_DATA_BASE);
	arch_interrupt_retarget();
	ns_config();
	jump_to_ns(addr);
}

void dump_int(void);

static void cli_jump_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t addr;

	if (argc != 2) {
		cli_cm33_help();
		return;
	}

	if (os_strcmp(argv[1], "s") == 0) {
		addr = SOC_GET_S_ADDR(CONFIG_PRIMARY_NS_VIRTUAL_CODE_START + SOC_FLASH_DATA_BASE);
		CLI_LOGI("jump to S: %x\n", addr);
		arch_interrupt_disable();
		jump_to_s(addr);
	} else if (os_strcmp(argv[1], "ns") == 0) {
		addr = SOC_GET_NS_ADDR(CONFIG_PRIMARY_NS_VIRTUAL_CODE_START + SOC_FLASH_DATA_BASE);
		CLI_LOGI("jump to NS: %x\n", addr);
		arch_interrupt_retarget();
		ns_config();
		jump_to_ns(addr);
	} else {
		cli_cm33_help();
	}
}

#define CM33_CMD_CNT (sizeof(s_cm33_commands) / sizeof(struct cli_command))
static const struct cli_command s_cm33_commands[] = {
	{"jump", "jump {s|ns} {addr}", cli_jump_cmd},
};

int cli_cm33_init(void)
{
	return cli_register_commands(s_cm33_commands, CM33_CMD_CNT);
}


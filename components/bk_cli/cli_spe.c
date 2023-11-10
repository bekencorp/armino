// Copyright 2022-2023 Beken
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
#include "driver/prro.h"
#include "arch_interrupt.h"

extern void jump_to_ns(uint32_t addr);
extern void arch_interrupt_disable(void);
extern void arch_interrupt_retarget(void);

static void cli_spe_help(void)
{
	CLI_LOGI("spe auto_jump - jump to NS world\n");
	CLI_LOGI("spe jump - jump to NS world\n");
}

static void cli_retarget_all_irq(void)
{
	CLI_LOGD("Target all interrupt to NS\r\n");
	for (uint32_t irq = 0; irq < INT_ID_MAX; irq++) {
		arch_int_set_target_state(irq);
	}
}

static void cli_disable_int(void)
{
	//Disable system tick
	REG_WRITE(0xe000e010, 0);

	//Disable UART0 interrupt
	CLI_LOGI("Disable CPU0 interrupt\r\n");
	REG_WRITE(0x44010000 + (0x20 * 4), 0);
	REG_WRITE(0x44010000 + (0x21 * 4), 0);
}

extern void arch_int_disable_all_irq(void);
static void cli_prepare_jump_to_ns(void)
{
#if 1
	cli_disable_int();
	cli_retarget_all_irq();

	CLI_LOGI("init mpc\r\n");
	bk_mpc_driver_init();

	CLI_LOGI("PRRO set FLASH NS partition to NS\r\n");
	bk_mpc_set_secure_attribute(MPC_DEV_FLASH, 256<<10, 10, MPC_BLOCK_NON_SECURE);

	CLI_LOGI("PRRO set RAM M3/M4 to NS\r\n");
	bk_mpc_set_secure_attribute(MPC_DEV_SMEM3, 0, 32, MPC_BLOCK_NON_SECURE);
	bk_mpc_set_secure_attribute(MPC_DEV_SMEM4, 0, 32, MPC_BLOCK_NON_SECURE);

	CLI_LOGI("Wait for TX over\r\n");
	while (!bk_uart_is_tx_over(UART_ID_0));

	bk_prro_driver_init();
	bk_prro_set_gpios_secure(0xFFFFFFFF, 0xFFFFFFFF);
	//Aon secure
	REG_WRITE(0x41040000 + 0xa *4, 0xF);
	bk_prro_set_ahb_dev_secure(0xFFFFFFFF);
	bk_prro_set_apb_dev_secure(0xFFFFFFFF);
#else
	extern void arch_int_enable_all_irq(void);
	arch_int_disable_all_irq();
	arch_int_set_target_state_all();
	bk_mpc_driver_init();
	bk_mpc_set_secure_attribute(MPC_DEV_FLASH, 1024<<10, 10, MPC_BLOCK_NON_SECURE);
	bk_mpc_set_secure_attribute(MPC_DEV_SMEM3, 0, 32, MPC_BLOCK_NON_SECURE);
	bk_mpc_set_secure_attribute(MPC_DEV_SMEM4, 0, 32, MPC_BLOCK_NON_SECURE);

	CLI_LOGI("Wait for TX over\r\n");
	while (!bk_uart_is_tx_over(UART_ID_0));

	REG_WRITE(0x41040000 + 0xa *4, 0xF);
	REG_WRITE(0x41040000 + 5 * 4, 0xFFFFFFFF);
	REG_WRITE(0x41040000 + 8 * 4, 0xFFFFFFFF);
	REG_WRITE(0x41040000 + 0xb * 4, 0xFFFFFFFF);
	REG_WRITE(0x41040000 + 0xc * 4, 0xFFFFFFFF);
#endif
}

void jump_test(void)
{
	uint32_t addr = SOC_GET_NS_ADDR(CONFIG_PRIMARY_NS_VIRTUAL_CODE_START + SOC_FLASH_DATA_BASE);
	cli_prepare_jump_to_ns();
	jump_to_ns(addr);
}

static void cli_jump_ns(void)
{
	uint32_t addr = SOC_GET_NS_ADDR(CONFIG_PRIMARY_NS_VIRTUAL_CODE_START + SOC_FLASH_DATA_BASE);

	CLI_LOGI("S -> NS: %x\n", addr);
	jump_to_ns(addr);
}

static void cli_auto_jump_ns(void)
{
	uint32_t addr = SOC_GET_NS_ADDR(CONFIG_PRIMARY_NS_VIRTUAL_CODE_START + SOC_FLASH_DATA_BASE);

	CLI_LOGD("Auto S -> NS: %x\n", addr);
	cli_prepare_jump_to_ns();
	jump_to_ns(addr);
}


static void cli_spe_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_spe_help();
		return;
	}

	if (os_strcmp(argv[1], "jump") == 0) {
		cli_jump_ns();
	} else if (os_strcmp(argv[1], "auto_jump") == 0) {
		cli_auto_jump_ns();
	} else if (os_strcmp(argv[1], "tmp_jump") == 0) {
		jump_test();
	} else {
		cli_spe_help();
	}
}

#define SPE_CMD_CNT (sizeof(s_spe_commands) / sizeof(struct cli_command))
static const struct cli_command s_spe_commands[] = {
	{"spe", "spe {jump}", cli_spe_cmd},
};

int cli_spe_init(void)
{
	return cli_register_commands(s_spe_commands, SPE_CMD_CNT);
}


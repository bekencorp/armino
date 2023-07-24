/**
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 BEKEN Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of BEKEN Corporation.
 *
 */
#include <stdlib.h>
#include "sys_rtos.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>

#include "bk_cli.h"
#include "stdarg.h"
#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include "bk_phy.h"
#include "cli.h"
#include "cli_config.h"
#include <components/log.h>
#include <driver/uart.h>
#include "bk_rtos_debug.h"
#if CONFIG_SHELL_ASYNCLOG
#include "components/shell_task.h"
#endif
#include "bk_api_cli.h"
#include "boot.h"

#define TAG    "debug"

static void debug_help_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#ifdef CONFIG_DUAL_CORE
#include "mb_ipc_cmd.h"
// #include <driver/gpio.h>

#include "amp_lock_api.h"

#include "spinlock.h"

static void debug_ipc_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static void debug_rpc_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static void debug_rpc_gpio_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static void debug_cpulock_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static void debug_spinlock_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

static u8     ipc_inited = 0;

spinlock_t		gpio_spinlock;
spinlock_t  *	gpio_spinlock_ptr;

#endif

#if CONFIG_ARCH_RISCV
static void debug_perfmon_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static void debug_show_boot_time(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif

//#define CORE_MARK_ENABLED

#ifdef CORE_MARK_ENABLED
static void debug_core_mark(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif

const struct cli_command debug_cmds[] = {
	{"help", "list debug cmds", debug_help_command},
#ifdef CONFIG_DUAL_CORE
	{"ipc", "ipc [spinlock addr]", debug_ipc_command},
//	{"rpc", "rpc", debug_rpc_command},
//	{"gpio_out", "gpio_out gpio_id {0|1}", debug_rpc_gpio_command},
	{"cpu_lock", "cpu_lock [timeout 1~20]", debug_cpulock_command},
	{"spin_lock", "spin_lock [timeout 1~20]", debug_spinlock_command},
#endif

#if CONFIG_ARCH_RISCV
	{"perfmon", "perfmon(calc MIPS)", debug_perfmon_command},
	{"boottime", "boottime(show boot mtime info)", debug_show_boot_time},
#endif

#ifdef CORE_MARK_ENABLED
#if !CONFIG_SLAVE_CORE
	{"core_mark", "core_mark [seed1 seed2 seed3 iteration Algorithms]", debug_core_mark},
#endif
#endif /* CORE_MARK_ENABLED */ 
};

const int cli_debug_table_size = ARRAY_SIZE(debug_cmds);

void print_cmd_table(const struct cli_command *cmd_table, int table_items)
{
	int i;

	for (i = 0; i < table_items; i++)
	{
		if (cmd_table[i].name)
		{
			if (cmd_table[i].help)
				os_printf("%s: %s\r\n", cmd_table[i].name, cmd_table[i].help);
			else
				os_printf("%s\r\n", cmd_table[i].name);
		}
	}
}

void print_cmd_help(const struct cli_command *cmd_table, int table_items, void *func)
{
	int i;

	for (i = 0; i < table_items; i++)
	{
		if(cmd_table[i].function == func)
		{
			if (cmd_table[i].help)
				os_printf("%s\r\n", cmd_table[i].help);

			break;
		}
	}
}

static void debug_help_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	os_printf("====Debug Commands====\r\n");

	print_cmd_table(debug_cmds, ARRAY_SIZE(debug_cmds));
}

#ifdef CONFIG_DUAL_CORE
static void print_debug_cmd_help(void *func)
{
	print_cmd_help(debug_cmds, ARRAY_SIZE(debug_cmds), func);
}

#if 0

static u8     rpc_inited = 0;

static void debug_rpc_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret_val;

	if(rpc_inited)
	{
		BK_LOGI(TAG,"rpc started\r\n");
		return;
	}

	ret_val = rpc_init();
	sprintf(pcWriteBuffer,"rpc init: %d\r\n", ret_val);
	rpc_inited = 1;

#if CONFIG_MASTER_CORE

	if(ipc_inited)
	{
		ret_val = ipc_send_test_cmd(0x12);
		BK_LOGI(TAG,"ipc server test: ret=%d\r\n", ret_val);

		ret_val = ipc_send_get_ps_flag();
		BK_LOGI(TAG,"ipc server ps: ret= 0x%x\r\n", ret_val);

		ret_val = ipc_send_get_heart_rate();
		BK_LOGI(TAG,"ipc server hr: ret= 0x%x\r\n", ret_val);

		ret_val = ipc_send_set_heart_rate(0x33);
		BK_LOGI(TAG,"ipc server set hr: ret= %d\r\n", ret_val);
	}

#endif

}
#endif

static void debug_ipc_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret_val;

	if(ipc_inited)
	{
		BK_LOGI(TAG,"ipc started\r\n");
		return;
	}

#if CONFIG_SLAVE_CORE

	if (argc < 2)
	{
		sprintf(pcWriteBuffer,"usage: ipc spinlock_addr\r\n");

		return;
	}

#endif

	ret_val = ipc_init();
	BK_LOGI(TAG,"ipc init: %d\r\n", ret_val);

	ipc_inited = 1;

#if CONFIG_SLAVE_CORE

	ret_val = ipc_send_power_up();
	BK_LOGI(TAG,"ipc client power: %d\r\n", ret_val);

	ret_val = ipc_send_heart_beat(0x34);
	BK_LOGI(TAG,"ipc client heartbeat: %d\r\n", ret_val);

	ret_val = ipc_send_test_cmd(0x12);
	BK_LOGI(TAG,"ipc client test: 0x%x\r\n", ret_val);

	gpio_spinlock_ptr = (spinlock_t *)strtoul(argv[1], NULL, 0);
	sprintf(pcWriteBuffer,"spinlock_addr: 0x%x\r\n", gpio_spinlock_ptr);

#else

	spinlock_init(&gpio_spinlock);
	sprintf(pcWriteBuffer,"spinlock_addr: 0x%x\r\n", &gpio_spinlock);

#endif
}

static void debug_spinlock_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	u32 timeout_second = 10;  // 10s

	if(ipc_inited == 0)
	{
		BK_LOGI(TAG,"Failed: no rpc client/server in CPU0/CPU1.\r\n");
		return;
	}

	if (argc > 1)
	{
		timeout_second = strtoul(argv[1], NULL, 0);

		if(timeout_second > 20)
			timeout_second = 20;
		if(timeout_second == 0)
			timeout_second = 10;
	}

	int i;

	for(i = 0; i < 10; i++)
	{
		BK_LOGI(TAG,"times: %d\r\n", i);

#if CONFIG_MASTER_CORE
		spinlock_acquire(&gpio_spinlock);
		BK_LOGI(TAG,"svr spinlock acquired %d\r\n", gpio_spinlock.locked);
		rtos_delay_milliseconds(timeout_second * 1000);
		spinlock_release(&gpio_spinlock);
		BK_LOGI(TAG,"svr spinlock released %d\r\n", gpio_spinlock.locked);
#endif

#if CONFIG_SLAVE_CORE
		spinlock_acquire(gpio_spinlock_ptr);
		BK_LOGI(TAG,"client spinlock acquired %d\r\n", gpio_spinlock_ptr->locked);
		rtos_delay_milliseconds(timeout_second * 1000);
		spinlock_release(gpio_spinlock_ptr);
		BK_LOGI(TAG,"client spinlock released %d\r\n", gpio_spinlock_ptr->locked);
#endif

	}
}

#if 0
extern bk_err_t bk_gpio_enable_output_rpc(gpio_id_t gpio_id);
extern bk_err_t bk_gpio_set_output_high_rpc(gpio_id_t gpio_id);
extern bk_err_t bk_gpio_set_output_low_rpc(gpio_id_t gpio_id);

static void debug_rpc_gpio_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_SLAVE_CORE

	int ret_val;

	u32 gpio_id = 0;
	u32 level = 0;

	if (argc < 3)
	{
		print_debug_cmd_help(debug_rpc_gpio_command);
		return;
	}

	gpio_id = strtoul(argv[1], NULL, 0);
	level   = strtoul(argv[2], NULL, 0);

	ret_val = bk_gpio_enable_output_rpc(gpio_id);
	BK_LOGI(TAG,"rpc gpio:ret=%d\r\n", ret_val);

	if(level)
		ret_val = bk_gpio_set_output_high_rpc(gpio_id);
	else
		ret_val = bk_gpio_set_output_low_rpc(gpio_id);

	BK_LOGI(TAG,"rpc gpio:ret=%d\r\n", ret_val);

#endif

}
#endif

static void debug_cpulock_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	u32 timeout_second = 10;  // 10s

	if(ipc_inited == 0)
	{
		BK_LOGI(TAG,"Failed: no ipc client/server in CPU0/CPU1.\r\n");
		return;
	}

	if (argc > 1)
	{
		timeout_second = strtoul(argv[1], NULL, 0);

		if(timeout_second > 20)
			timeout_second = 20;
		if(timeout_second == 0)
			timeout_second = 10;
	}
	else
	{
		print_debug_cmd_help(debug_cpulock_command);
		BK_LOGI(TAG,"default timeout 10s is used.\r\n");
	}

	int	ret_val = BK_FAIL;

	ret_val = amp_res_init(AMP_RES_ID_GPIO);
	BK_LOGI(TAG,"amp res init:ret=%d\r\n", ret_val);

	ret_val = amp_res_acquire(AMP_RES_ID_GPIO, timeout_second * 1000);
	BK_LOGI(TAG,"amp res acquire:ret=%d\r\n", ret_val);

	rtos_delay_milliseconds(timeout_second * 1000);

	if(ret_val == 0)
	{
		ret_val = amp_res_release(AMP_RES_ID_GPIO);
		BK_LOGI(TAG,"amp res release:ret=%d\r\n", ret_val);
	}
	else
	{
		BK_LOGI(TAG,"amp res release: no release\r\n");
	}

}
#endif

const struct cli_command * cli_debug_cmd_table(int *num)
{
	*num = ARRAY_SIZE(debug_cmds);

	return &debug_cmds[0];
}

#if CONFIG_ARCH_RISCV

extern u64 riscv_get_instruct_cnt(void);
extern u64 riscv_get_mtimer(void);

static u64 		saved_time = 0;
static u64 		saved_inst_cnt = 0;

static void debug_perfmon_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	u64 cur_time = riscv_get_mtimer();
	u64 cur_inst_cnt = riscv_get_instruct_cnt();

	BK_LOGI(TAG,"cur time: %x:%08x\r\n", (u32)(cur_time >> 32), (u32)(cur_time & 0xFFFFFFFF));
	BK_LOGI(TAG,"cur inst_cnt: %x:%08x\r\n", (u32)(cur_inst_cnt >> 32), (u32)(cur_inst_cnt & 0xFFFFFFFF));

	saved_time = (cur_time - saved_time) / 26;
	saved_inst_cnt = cur_inst_cnt - saved_inst_cnt;

//	BK_LOGI(TAG,"elapse time(us): %x:%08x\r\n", (u32)(saved_time >> 32), (u32)(saved_time & 0xFFFFFFFF));
//	BK_LOGI(TAG,"diff inst_cnt: %x:%08x\r\n", (u32)(saved_inst_cnt >> 32), (u32)(saved_inst_cnt & 0xFFFFFFFF));

	sprintf(pcWriteBuffer,"MIPS: %d KIPS\r\n", (u32)(saved_inst_cnt * 1000 / saved_time));

	saved_time = cur_time;
	saved_inst_cnt = cur_inst_cnt;
}

static void debug_show_boot_time(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	u64 cur_time = riscv_get_mtimer();
	u64 cur_inst_cnt = riscv_get_instruct_cnt();

	BK_LOGI(TAG,"cur time: %x:%08x\r\n", (u32)(cur_time >> 32), (u32)(cur_time & 0xFFFFFFFF));
	BK_LOGI(TAG,"cur time: %ldms\r\n", (u32)(cur_time/26000));
	BK_LOGI(TAG,"cur inst_cnt: %x:%08x\r\n", (u32)(cur_inst_cnt >> 32), (u32)(cur_inst_cnt & 0xFFFFFFFF));

#if	CONFIG_SAVE_BOOT_TIME_POINT
	show_saved_mtime_info();
#endif

}
#endif

#ifdef CORE_MARK_ENABLED
static void debug_core_mark(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	extern bk_err_t bk_wdt_stop(void);
	extern bk_err_t bk_wdt_start(uint32_t timeout_ms);

	extern void core_mark(int argc, char *argv[]);

	bk_wdt_stop();
	u32  int_mask = rtos_disable_int();

	core_mark(argc, argv);

	rtos_enable_int(int_mask);
	bk_wdt_start(CONFIG_INT_WDT_PERIOD_MS);
}

#include "./core_mark/core_main.c"
#endif /* CORE_MARK_ENABLED */


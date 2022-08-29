/*
 * Copyright (c) 2012-2021 Andes Technology Corporation
 * All rights reserved.
 *
 */

#include <stdint.h>
#include "boot.h"
#include "sdkconfig.h"
#include "reset_reason.h"
#include <os/os.h>
#include "bk_arch.h"
#include "stack_base.h"
#include "bk_uart.h"
#include "bk_rtos_debug.h"
#include <os/os.h>
#include <stdio.h>
#include "platform.h"
#include <components/system.h>
#include <os/mem.h>
#include <components/log.h>
#include <common/bk_assert.h>
#include <driver/wdt.h>
#include <bk_wdt.h>

#if (CONFIG_CACHE_ENABLE)
	#define SRAM_BLOCK_COUNT 4
	extern unsigned int g_sram_addr_map[SRAM_BLOCK_COUNT];
#else
	#if (CONFIG_SLAVE_CORE)
	#define RAM_BASE_ADDR 0x30060000
	#else
	#define RAM_BASE_ADDR 0x30000000
	#endif
#endif

#define SYS_DELAY_TIME_5S	    (85000000UL)

#if CONFIG_LITEOS_M_V3
typedef struct {
	union {
		struct {
			long x2;  //sp
			long x4;  //tp
			long x31; //t6
			long x30; //t5
			long x29; //t4
			long x28; //t3
			long x7;  //t2
			long x6;  //t1
			long x5;  //t0
			long x27; //s11
			long x26; //s10
			long x25; //s9
			long x24; //s8
			long x23; //s7
			long x22; //s6
			long x21; //s5
			long mstatus;
			long mepc;
			long x17; //a7
			long x16; //a6
			long x15; //a5
			long x14; //a4
			long x13; //a3
			long x12; //a2
			long x11; //a1
			long x10; //a0
			long x20; //s4
			long x19; //s3
			long x18; //s2
			long x9;  //s1
			long x8;  //s0
			long x1; //ra
		};
		long riscv_regs[32];
	};
} SAVED_CONTEXT;

#else // CONFIG_FREERTOS

typedef struct {
	union {
		struct {
			long mepc;
			long x1;
			long x5;
			long x6;
			long x7;
			long x8;
			long x9;
			long x10;
			long x11;
			long x12;
			long x13;
			long x14;
			long x15;
			long x16;
			long x17;
			long x18;
			long x19;
			long x20;
			long x21;
			long x22;
			long x23;
			long x24;
			long x25;
			long x26;
			long x27;
			long x28;
			long x29;
			long x30;
			long x31;
			long mstatus;
		};
		long riscv_regs[30];
	};
} SAVED_CONTEXT;
#endif

typedef void (*hook_func)(void);

extern char _dtcm_ema_start, _dtcm_bss_end;
extern char _end;  //BSS end in SRAM2

extern void mtime_handler(void);
extern void mswi_handler(void);
extern void mext_interrupt(void);
extern void stack_mem_dump(uint32_t stack_top, uint32_t stack_bottom);
extern void user_except_handler (unsigned long mcause, SAVED_CONTEXT *context);
extern void close_wdt(void);

static hook_func s_wifi_dump_func = NULL;
static hook_func s_ble_dump_func = NULL;

volatile unsigned int g_enter_exception = 0;

unsigned int arch_is_enter_exception(void) {
	return g_enter_exception;
}

void rtos_regist_wifi_dump_hook(hook_func wifi_func)
{
	s_wifi_dump_func = wifi_func;
}

void rtos_regist_ble_dump_hook(hook_func ble_func)
{
	s_ble_dump_func = ble_func;
}

void trap_handler(unsigned long mcause, SAVED_CONTEXT *context)
{
	if (0 == g_enter_exception) {
		// Make sure the interrupt is disable
		uint32_t int_level = rtos_disable_int();
		uint32_t mie_status = rtos_disable_mie_int();

#if CONFIG_INT_WDT
		close_wdt();
		bk_task_wdt_stop();
#endif
		/* Handled Trap */
		g_enter_exception = 1;
		BK_LOG_FLUSH();
		bk_set_printf_sync(true);
		
		user_except_handler(mcause, context);
		BK_DUMP_OUT("Unhandled Trap : mcause = 0x%x, mepc = 0x%x\r\n", mcause, context->mepc);
		while(g_enter_exception);

		rtos_enable_mie_int(mie_status);
		rtos_enable_int(int_level);
	}

}


/**
 * this function will show registers of CPU
 *
 * @param mcause
 * @param context
 */
void arch_dump_cpu_registers (unsigned long mcause, SAVED_CONTEXT *context)
{

	BK_DUMP_OUT("Current regs:\r\n");

	BK_DUMP_OUT("1 ra x 0x%lx\r\n", context->x1);
	BK_DUMP_OUT("2 sp x 0x%lx\r\n", context + 1);
	BK_DUMP_OUT("5 t0 x 0x%lx\r\n", context->x5);
	BK_DUMP_OUT("6 t1 x 0x%lx\r\n", context->x6);
	BK_DUMP_OUT("7 t2 x 0x%lx\r\n", context->x7);
	BK_DUMP_OUT("8 fp x 0x%lx\r\n", context->x8);
	BK_DUMP_OUT("9 s1 x 0x%lx\r\n", context->x9);
	BK_DUMP_OUT("10 a0 x 0x%lx\r\n", context->x10);
	BK_DUMP_OUT("11 a1 x 0x%lx\r\n", context->x11);
	BK_DUMP_OUT("12 a2 x 0x%lx\r\n", context->x12);
	BK_DUMP_OUT("13 a3 x 0x%lx\r\n", context->x13);
	BK_DUMP_OUT("14 a4 x 0x%lx\r\n", context->x14);
	BK_DUMP_OUT("15 a5 x 0x%lx\r\n", context->x15);
	BK_DUMP_OUT("16 a6 x 0x%lx\r\n", context->x16);
	BK_DUMP_OUT("17 a7 x 0x%lx\r\n", context->x17);
	BK_DUMP_OUT("18 s2 x 0x%lx\r\n", context->x18);
	BK_DUMP_OUT("19 s3 x 0x%lx\r\n", context->x19);
	BK_DUMP_OUT("20 s4 x 0x%lx\r\n", context->x20);
	BK_DUMP_OUT("21 s5 x 0x%lx\r\n", context->x21);
	BK_DUMP_OUT("22 s6 x 0x%lx\r\n", context->x22);
	BK_DUMP_OUT("23 s7 x 0x%lx\r\n", context->x23);
	BK_DUMP_OUT("24 s8 x 0x%lx\r\n", context->x24);
	BK_DUMP_OUT("25 s9 x 0x%lx\r\n", context->x25);
	BK_DUMP_OUT("26 s10 x 0x%lx\r\n", context->x26);
	BK_DUMP_OUT("27 s11 x 0x%lx\r\n", context->x27);
	BK_DUMP_OUT("28 t3 x 0x%lx\r\n", context->x28);
	BK_DUMP_OUT("29 t4 x 0x%lx\r\n", context->x29);
	BK_DUMP_OUT("30 t5 x 0x%lx\r\n", context->x30);
	BK_DUMP_OUT("31 t6 x 0x%lx\r\n", context->x31);

	if (mcause == TRAP_M_USER_ASSERT) {
		BK_DUMP_OUT("32 pc x 0x%lx\r\n", context->x1 - 4);
	} else {
		BK_DUMP_OUT("32 pc x 0x%lx\r\n", context->mepc - 4);
	}

	BK_DUMP_OUT("833 mstatus x 0x%lx\r\n", context->mstatus);

	BK_DUMP_OUT("838 mtvec x 0x%lx\r\n", read_csr(NDS_MTVEC));
	BK_DUMP_OUT("897 mscratch x 0x%lx\r\n", read_csr(NDS_MSCRATCH));
	BK_DUMP_OUT("898 mepc x 0x%lx\r\n", context->mepc);
	BK_DUMP_OUT("899 mcause x 0x%lx\r\n", mcause);
	BK_DUMP_OUT("900 mtval x 0x%lx\r\n", read_csr(NDS_MTVAL));
	BK_DUMP_OUT("2058 mdcause x 0x%lx\r\n", read_csr(NDS_MDCAUSE));

	BK_DUMP_OUT("\r\n");

	if (mcause == 0x2) {
		stack_mem_dump((uint32_t)(context->mepc - 32), (uint32_t)(context->mepc + 32));
	}
}

void sys_delay_sync(uint32_t time_count )
{
	volatile UINT32 i = 0;

	for (i = 0; i < time_count; i ++)
		;
}

void user_except_handler (unsigned long mcause, SAVED_CONTEXT *context) {
	BK_DUMP_OUT("***********************************************************************************************\r\n");
	BK_DUMP_OUT("***********************************user except handler begin***********************************\r\n");
	BK_DUMP_OUT("***********************************************************************************************\r\n");

	arch_dump_cpu_registers(mcause, context);

	if(NULL != s_wifi_dump_func) {
		s_wifi_dump_func();
	}

	if(NULL != s_ble_dump_func) {
		s_ble_dump_func();
	}

	BK_DUMP_OUT("System will dump memory in 5s, please ready to save whole log.........\r\n");

	sys_delay_sync(SYS_DELAY_TIME_5S);

#if CONFIG_MEMDUMP_ALL
	stack_mem_dump((uint32_t)&_dtcm_ema_start, (uint32_t)&_dtcm_bss_end);
#if CONFIG_CACHE_ENABLE && (!CONFIG_SLAVE_CORE)
	for (int i = 0; i < SRAM_BLOCK_COUNT; i++) {
		stack_mem_dump(g_sram_addr_map[i], g_sram_addr_map[i] + 0x20000);
	}
#else
	stack_mem_dump(RAM_BASE_ADDR, (uint32_t)&_end);
#endif
#endif

	rtos_dump_backtrace();
	rtos_dump_task_list();
#if CONFIG_FREERTOS
	rtos_dump_task_runtime_stats();
#endif

#if CONFIG_FREERTOS && CONFIG_MEM_DEBUG
	os_dump_memory_stats(0, 0, NULL);
#endif

	arch_dump_cpu_registers(mcause, context);


	BK_DUMP_OUT("***********************************************************************************************\r\n");
	BK_DUMP_OUT("************************************user except handler end************************************\r\n");
	BK_DUMP_OUT("***********************************************************************************************\r\n");

	bk_reboot();

}

void set_reboot_tag(uint32_t tag) {
	uint32_t p_tag = REBOOT_TAG_ADDR;
	*((uint32_t *)p_tag) = tag;
}

inline uint32_t get_reboot_tag(void) {
	return *((uint32_t *)REBOOT_TAG_ADDR);
}


void user_nmi_handler(unsigned long mcause, unsigned long ra) {
	if(mcause == MCAUSE_CAUSE_WATCHDOG) {
		if( REBOOT_TAG_REQ == get_reboot_tag() ) {
			BK_DUMP_OUT("Wait reboot.\r\n");
			while(1);
		}
	}

#if CONFIG_INT_WDT
	close_wdt();
	bk_task_wdt_stop();
#endif

	BK_DUMP_OUT("========Call NULL func pointer/WDT, please check the code near ra reg.========\r\n");
	BK_DUMP_OUT("1 ra x 0x%lx\r\n", ra);
	BK_DUMP_OUT("========Call NULL func pointer/WDT, please check the code near ra reg.========\r\n");
}

#if CONFIG_SAVE_BOOT_TIME_POINT

static uint64_t s_saved_boot_info[2*CPU_SAVED_TIME_MAX];

extern uint64_t riscv_get_mtimer(void);
extern uint64_t riscv_get_instruct_cnt(void);

static uint32_t get_saved_time_info_addr(uint32_t time_point) {
	uint32_t addr = 0;

	if (CPU_BOOT_TIME == time_point) {
		//The BSS section not ready at cpu boot time point
		addr = CPU_BOOT_TIME_ADDR;
	} else if (time_point < CPU_SAVED_TIME_MAX){
		addr = (uint32_t)&s_saved_boot_info[2*time_point];
	}

	return addr;
}

void save_mtime_point(uint32_t time_point) {
	uint32_t addr = get_saved_time_info_addr(time_point);

	if (0 != addr) {
		*((uint64_t *)addr) = riscv_get_mtimer();
		*((uint64_t *)addr + 1) = riscv_get_instruct_cnt();
	}
}

void show_saved_mtime_point(uint32_t time_point) {
	uint32_t addr = get_saved_time_info_addr(time_point);

	if (0 != addr) {
		uint64_t saved_time = *((uint64_t *)addr);
		uint64_t saved_inst_cnt = *((uint64_t *)addr + 1);
		uint32_t saved_time_ms = (u32)(saved_time & 0xFFFFFFFF) / 26000;
		BK_DUMP_OUT("saved time: 0x%x:0x%08x\r\n", (u32)(saved_time >> 32), (u32)(saved_time & 0xFFFFFFFF));
		BK_DUMP_OUT("saved time: %ldms\r\n", saved_time_ms);
		BK_DUMP_OUT("saved inst_cnt: 0x%x:0x%08x\r\n", (u32)(saved_inst_cnt >> 32), (u32)(saved_inst_cnt & 0xFFFFFFFF));
	}
}

void show_saved_mtime_info(void)
{
	BK_DUMP_OUT("==============Show Boot Time===================\r\n");
	show_saved_mtime_point(CPU_BOOT_TIME);

	BK_DUMP_OUT("==============Show Init Mem Time===================\r\n");
	show_saved_mtime_point(CPU_INIT_MEM_TIME);

	BK_DUMP_OUT("==============Show MAIN Emtry Time===================\r\n");
	show_saved_mtime_point(CPU_MAIN_ENTRY_TIME);

	BK_DUMP_OUT("==============Show INIT Driver Time===================\r\n");
	show_saved_mtime_point(CPU_INIT_DRIVER_TIME);

	BK_DUMP_OUT("==============Show App Entry Time===================\r\n");
	show_saved_mtime_point(CPU_APP_ENTRY_TIME);

	BK_DUMP_OUT("==============Show Start Sche Time===================\r\n");
	show_saved_mtime_point(CPU_START_SCHE_TIME);

	BK_DUMP_OUT("==============Show Start Wifi init Time===================\r\n");
	show_saved_mtime_point(CPU_START_WIFI_INIT_TIME);

	BK_DUMP_OUT("==============Show Finish Wifi init Time===================\r\n");
	show_saved_mtime_point(CPU_FINISH_WIFI_INIT_TIME);

	BK_DUMP_OUT("==============Show App Finish Time===================\r\n");
	show_saved_mtime_point(CPU_APP_FINISH_TIME);

	BK_DUMP_OUT("==============Show Main Finish Time===================\r\n");
	show_saved_mtime_point(CPU_MIAN_FINISH_TIME);

	BK_DUMP_OUT("==============Show Start Connect Time===================\r\n");
	show_saved_mtime_point(CPU_START_CONNECT_TIME);

	BK_DUMP_OUT("==============Show Wifi Connected Time===================\r\n");
	show_saved_mtime_point(CPU_CONNECTED_TIME);
}


void show_current_time_point(const char *info) {
	uint64_t current_time = riscv_get_mtimer();;
	uint32_t current_time_ms = (u32) (current_time/26000);
	BK_DUMP_OUT("%s: current time: %ldms\r\n", info, current_time_ms);
}

#endif

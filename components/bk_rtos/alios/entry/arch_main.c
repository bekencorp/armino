/**
 ****************************************************************************************
 *
 * @file arch_main.c
 *
 * @brief Main loop of the application.
 *
 * Copyright (C) Beken Corp 2011-2020
 *
 ****************************************************************************************
 */
#include <common/bk_include.h>
#include "bk_private/bk_driver.h"
#include "bk_private/legacy_init.h"
#include <os/mem.h>
#include "bk_uart.h"
#include "bk_private/bk_ate.h"
#include "k_api.h"
#include <os/os.h>
#include <components/system.h>

void print_exception_addr(unsigned int pc, unsigned int lr, unsigned int sp)
{
	cpu_intrpt_save();
	os_printf("pc is %x, lr is %x, sp is %x\n", pc, lr, sp);
	while (1);
}

beken_mutex_t stdio_tx_mutex;

extern void hw_start_hal(void);
extern void fclk_init(void);
extern void test_case_task_start(void);


static int test_cnt;

void task_test3(void *arg)
{
	beken_semaphore_t sem;

	rtos_init_semaphore(&sem, 0);
	while (1) {
		os_printf("test_cnt is %d\r\n", test_cnt++);
		rtos_get_semaphore(&sem, 1000);
	}
}

void entry_main(void)
{
	sys_start();
}

void soc_driver_init(void)
{
#if 0
#if CONFIG_ATE
	ate_app_init();
#endif

	driver_init();
#endif
}

void soc_system_init(void)
{
	components_init();

	bk_tick_init();

	hal_init();

#ifndef AOS_NO_WIFI
	legacy_init();

	hw_start_hal();
#endif
}

// eof


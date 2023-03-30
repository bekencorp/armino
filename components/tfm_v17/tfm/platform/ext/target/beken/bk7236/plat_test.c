/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "platform_retarget.h"
#include "timer_cmsdk.h"
#include "tfm_plat_test.h"
#include "psa/service.h"
#include "driver/timer.h"

//TODO implement timer interrupt via armino timer driver
extern psa_flih_result_t tfm_flih_test_timer_handler(void);
extern void timer_clear_isr_status(void);
extern void timer_isr(void);

void TFM_TIMER0_Handler_Backup(void)
{
	printf("timer_isr\r\n");
	
	timer_isr();
}

void timer0_handler(void)
{
	timer_isr();
}

void tfm_plat_test_secure_timer_start(void)
{
	*((volatile uint32_t *)(0x41040000 + 5 * 4)) = 0; // TODO wangzhilei
	
	bk_timer_driver_init();
	bk_timer_start(TIMER_ID1, 500, (timer_isr_t)NULL);
}

void tfm_plat_test_secure_timer_clear_intr(void) {
	timer_clear_isr_status();
}

void tfm_plat_test_secure_timer_stop(void)
{
	bk_timer_stop(TIMER_ID1);
}

void tfm_plat_test_non_secure_timer_start(void)
{
	bk_timer_driver_init();
	bk_timer_start(TIMER_ID2, 500, (timer_isr_t)NULL);
}

void tfm_plat_test_non_secure_timer_stop(void)
{
	bk_timer_stop(TIMER_ID2);
}

#include <stdio.h>
#include <stdlib.h>
#include "coremark.h"
#include "config.h"

#if VALIDATION_RUN
	volatile ee_s32 seed1_volatile=0x3415;
	volatile ee_s32 seed2_volatile=0x3415;
	volatile ee_s32 seed3_volatile=0x66;
#endif

#if PERFORMANCE_RUN
	volatile ee_s32 seed1_volatile=0x0;
	volatile ee_s32 seed2_volatile=0x0;
	volatile ee_s32 seed3_volatile=0x66;
#endif

#if PROFILE_RUN
	volatile ee_s32 seed1_volatile=0x8;
	volatile ee_s32 seed2_volatile=0x8;
	volatile ee_s32 seed3_volatile=0x8;
#endif

volatile ee_s32 seed4_volatile=ITERATIONS;
volatile ee_s32 seed5_volatile=0;

static CORE_TICKS t0, t1;

#if CONFIG_ARCH_RISCV

extern uint64_t riscv_get_mtimer(void);

uint64_t get_timer_value()
{

	return riscv_get_mtimer();
}

void timer_hal_us_init(uint32_t us)
{
}

#endif

#if CONFIG_ARCH_CM33

#include <soc/soc.h>

#define TIMER0_REG_SET(reg_id, l, h, v) REG_SET((SOC_TIMER0_REG_BASE + ((reg_id) << 2)), (l), (h), (v))
#define TIMER0_PERIOD 0xFFFFFFFF

static uint32_t timer_hal_get_timer0_cnt(void)
{
	TIMER0_REG_SET(8, 2, 3, 0);
	TIMER0_REG_SET(8, 0, 0, 1);
	while (REG_READ((SOC_TIMER0_REG_BASE + (8 << 2))) & BIT(0));

	return REG_READ(SOC_TIMER0_REG_BASE + (9 << 2));
}

extern void timer_hal_us_init(uint32_t us);

uint64_t get_timer_value()
{
	return timer_hal_get_timer0_cnt();
}

#endif

void start_time(void)
{
	t0 = get_timer_value();

}

void stop_time(void)
{
	t1 = get_timer_value();

}

CORE_TICKS get_time(void)
{
	return (CORE_TICKS)t1 - t0;
}

secs_ret time_in_secs(CORE_TICKS ticks)
{
	secs_ret delta = (secs_ret)ticks;
	secs_ret val=delta / 26000000UL;
	return val;
}


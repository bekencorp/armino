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

#include "icu_driver.h"
#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include "bk_timer.h"
#include "bk_icu.h"
#include "bk_fake_clock.h"
#include "bk_mcu_ps.h"
#include "bk_misc.h"
#include "wlan_ui_pub.h"
#include "bk_intc.h"
#include <components/system.h>
#include <driver/timer.h>
#include <driver/pwm.h>
#include "timer_hal.h"
#include "sys_driver.h"

#define PWM_CLK_32K                 (0x00)
#define PWM_CLK_26M                 (0x01)
#define PWM_CLK_DCO                 (0x02)

void ps_pwm_enable(void)
{
	bk_pwm_start(PS_PWM_ID);
	bk_pwm_enable_interrupt(PS_PWM_ID);
	bk_pwm_set_mode_timer(PS_PWM_ID);
}

void ps_pwm_disable(void)
{
	bk_pwm_stop(PS_PWM_ID);
}

void ps_pwm_set_period(uint32_t period, uint8_t clk_mux)
{
	pwm_period_duty_config_t period_duty_cfg = {0};

	sys_drv_pwm_set_clock(clk_mux, PS_PWM_ID);

	period_duty_cfg.period_cycle = period;
	period_duty_cfg.duty_cycle = 0;

	bk_pwm_set_period_duty(PS_PWM_ID, &period_duty_cfg);
}

void ps_pwm_reconfig(UINT32 period, UINT8 clk_mux)
{

	ps_pwm_disable();
	//new
	ps_pwm_set_period(period, clk_mux);
	//reenable
	ps_pwm_enable();
	delay(5);

	REG_WRITE(PWM_R_INT_STATUS, 0x3f);
}

void ps_pwm_suspend_tick(UINT32 period)
{
	ps_pwm_reconfig(period, PWM_MUX_LPO);
}

static uint32_t cal_endvalue(uint32_t mode)
{
	uint32_t value = 1;

	if (PWM_CLK_32K == mode) {
		/*32k clock*/
		value = bk_get_ms_per_tick() * TIMER_CLOCK_FREQ_32K;
	} else if (PWM_CLK_26M == mode) {
		/*26m clock*/
		value = bk_get_ms_per_tick() * TIMER_CLOCK_FREQ_26M;
	}

	return value;
}

void ps_pwm_resume_tick(void)
{
	ps_pwm_reconfig(cal_endvalue(PWM_CLK_26M), PWM_MUX_PCLK);
}

UINT32 ps_pwm_int_status(void)
{
	return ((icu_get_irq_int_status()) & (BIT(IRQ_PWM))) && (REG_READ(PWM_R_INT_STATUS) & 0x1);
}

static uint32_t timer_0_2_en_status = 0;

void ps_timer02_disable(void)
{
	timer_0_2_en_status = bk_timer_get_enable_status() & TIMER_F_EN_M;
	for (uint32_t timer_id = TIMER_ID0; timer_id < TIMER_ID3; timer_id++) {
		bk_timer_disable(timer_id);
	}
}

void ps_timer02_restore(void)
{
	uint32_t en_bit = 0;

	for (uint32_t timer_id = TIMER_ID0; timer_id < TIMER_ID3; timer_id++) {
		en_bit = (timer_0_2_en_status & BIT(timer_id));
		if (en_bit) {
			bk_timer_enable(en_bit);
		}
	}
}

uint32_t ps_timer2_get(void)
{
	uint32_t timer_cnt = 0;

#if CONFIG_TIMER_COUNTER
	timer_cnt = bk_timer_get_cnt(TIMER_ID2);
#endif

	return (timer_cnt / TIMER_CLOCK_FREQ_26M);
}

void ps_timer3_enable(uint32_t period)
{
	if (bk_get_tick_timer_id() == TIMER_ID3) {
		bk_timer_disable(TIMER_ID3);
		delay(2);
	}

	bk_timer_start_without_callback(TIMER_ID3, (period / TIMER_CLOCK_FREQ_32K));
}

uint32_t ps_timer3_disable(void)
{
	uint32_t less;
	if (bk_timer_is_interrupt_triggered(TIMER_ID3)) {
		less = bk_timer_get_period(TIMER_ID3);
	} else {
#if CONFIG_TIMER_COUNTER
		less = bk_timer_get_cnt(TIMER_ID3);
#else
		less = bk_timer_get_period(TIMER_ID3);
#endif
	}
	bk_timer_disable(TIMER_ID3);
	delay(2);
	if (bk_get_tick_timer_id() == TIMER_ID3) {
		/* below is old code, notice: delay(2)
		 *
		 * delay(2);
		 * REG_WRITE(TIMER3_CNT, bk_get_ms_per_tick() * 32);
		 * delay(2);
		 * reg = REG_READ(TIMER3_5_CTL);
		 * reg |= (TIMERCTL3_EN_BIT);
		 * REG_WRITE(TIMER3_5_CTL, reg);
		 */
		bk_timer_start_without_callback(TIMER_ID3, bk_get_ms_per_tick() * TIMER_CLOCK_FREQ_32K);
	}
	return (less / TIMER_CLOCK_FREQ_32K);
}


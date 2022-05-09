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

#pragma once

#include <soc/soc.h>
#include "pwm_hw.h"
#include "hal_port.h"
#include <driver/hal/hal_pwm_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PWM_LL_REG_BASE(_pwm_unit_id) (SOC_PWM_REG_BASE)

#define PWM0_LL_PIN    GPIO_6
#define PWM1_LL_PIN    GPIO_7
#define PWM2_LL_PIN    GPIO_8
#define PWM3_LL_PIN    GPIO_9
#define PWM4_LL_PIN    GPIO_24
#define PWM5_LL_PIN    GPIO_26

enum {
	PWM_LL_CHAN0 = 0,
	PWM_LL_CHAN1 = 1,
	PWM_LL_CHAN2 = 2,
	PWM_LL_CHAN3 = 3,
	PWM_LL_CHAN4 = 4,
	PWM_LL_CHAN5 = 5
};

static inline void pwm_ll_init(pwm_hw_t *hw)
{
	hw->ctrl.v = 0;
}

static inline void pwm_ll_enable(pwm_hw_t *hw, uint32_t chan)
{
	REG_MCHAN_SET_FIELD(chan, &hw->ctrl, PWM_F_EN, 1);
}

static inline void pwm_ll_disable(pwm_hw_t *hw, uint32_t chan)
{
	REG_MCHAN_SET_FIELD(chan, &hw->ctrl, PWM_F_EN, 0);
}

static inline bool pwm_ll_is_chan_started(pwm_hw_t *hw, uint32_t chan)
{
	return ((REG_MCHAN_GET_FIELD(chan, &hw->ctrl, PWM_F_EN)) == 1);
}

static inline void pwm_ll_enable_interrupt(pwm_hw_t *hw, uint32_t chan)
{
	REG_MCHAN_SET_FIELD(chan, &hw->ctrl, PWM_F_INT_EN, 1);
}

static inline void pwm_ll_disable_interrupt(pwm_hw_t *hw, uint32_t chan)
{
	REG_MCHAN_SET_FIELD(chan, &hw->ctrl, PWM_F_INT_EN, 0);
}

static inline uint32_t pwm_ll_get_interrupt_status(pwm_hw_t *hw)
{
	return REG_READ(&hw->int_status);
}

static inline void pwm_ll_clear_interrupt_status(pwm_hw_t *hw, uint32_t status)
{
	REG_WRITE(&hw->int_status, status);
}

static inline void pwm_ll_clear_chan_interrupt_status(pwm_hw_t *hw, uint32_t chan)
{
	REG_MCHAN_SET_FIELD(chan, &hw->int_status, PWM_F_INT_ST, 1);
}

static inline bool pwm_ll_is_interrupt_triggered(pwm_hw_t *hw, uint32_t chan, uint32_t status)
{
	return !!(status & (PWM_F_INT_ST << (PWM_F_INT_ST_MS(chan))));
}

static inline void pwm_ll_set_mode(pwm_hw_t *hw, uint32_t chan, uint32_t mode)
{
	REG_MCHAN_SET_FIELD(chan, &hw->ctrl, PWM_F_MODE, mode);
}

#define pwm_ll_set_mode_pwm(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_PWM)
#define pwm_ll_set_mode_timer(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_TIMER)
#define pwm_ll_set_mode_capture_pos(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_CAPTURE_EDGE_POS)
#define pwm_ll_set_mode_capture_neg(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_CAPTURE_EDGE_NEG)
#define pwm_ll_set_mode_capture_edge(hw, chan)

static inline void pwm_ll_set_t4(pwm_hw_t *hw, uint32_t chan, uint32_t t4)
{
	hw->freq_duty_capture_out[chan].period_cycle = t4;
}

static inline void pwm_ll_set_t1(pwm_hw_t *hw, uint32_t chan, uint32_t t1)
{
	hw->freq_duty_capture_out[chan].duty_cycle = t1;
}

static inline uint32_t pwm_ll_get_t1(pwm_hw_t *hw, uint32_t chan)
{
	return hw->freq_duty_capture_out[chan].duty_cycle;
}

static inline uint32_t pwm_ll_get_t4(pwm_hw_t *hw, uint32_t chan)
{
	return hw->freq_duty_capture_out[chan].period_cycle;
}

#define pwm_ll_set_t2(hw, chan, t2)
#define pwm_ll_set_t3(hw, chan, t3)

static inline uint32_t pwm_ll_get_capture_value(pwm_hw_t *hw, uint32_t chan)
{
	return hw->freq_duty_capture_out[chan].capture_out;
}

static inline bool pwm_ll_is_duty_valid(uint32_t period, uint32_t t1, uint32_t t2,
										uint32_t t3)
{
	//NOT consider uint32_t overflow!
	return (t1 <= period);
}

static inline bool pwm_ll_is_capture_edge_valid(uint32_t edge)
{
	return ((edge == PWM_CAPTURE_POS) || (edge == PWM_CAPTURE_NEG));
}

#define pwm_ll_reset_config_to_default(hw, chan)
#define pwm_ll_load_new_config(hw, chan)
#define pwm_ll_set_init_signal_low(hw, chan)
#define pwm_ll_set_init_signal_high(hw, chan)
#define pwm_ll_is_hardware_group(hw, chan1, chan2) false
#define pwm_ll_start_hardware_group(hw, chan1, chan2)
#define pwm_ll_stop_hardware_group(hw, chan1, chan2)
#define pwm_ll_is_pwm2_interrupt(hw, chan) false

#ifdef __cplusplus
}
#endif

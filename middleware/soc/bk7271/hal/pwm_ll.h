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
#define PWM_LL_GROUP(chan) ((chan) / SOC_PWM_CHAN_NUM_PER_GROUP)
#define PWM_LL_CHAN(chan) ((chan) % SOC_PWM_CHAN_NUM_PER_GROUP)

#define PWM0_LL_PIN    GPIO_3
#define PWM1_LL_PIN    GPIO_4
#define PWM2_LL_PIN    GPIO_5
#define PWM3_LL_PIN    GPIO_6
#define PWM4_LL_PIN    GPIO_7
#define PWM5_LL_PIN    GPIO_8
#define PWM6_LL_PIN    GPIO_18 // GPIO_26/GPIO_30
#define PWM7_LL_PIN    GPIO_19 // GPIO_27/GPIO_31
#define PWM8_LL_PIN    GPIO_20 // GPIO_28/GPIO_32
#define PWM9_LL_PIN    GPIO_21 // GPIO_29/GPIO_33
#define PWM10_LL_PIN   GPIO_25
#define PWM11_LL_PIN   GPIO_35

typedef enum {
	GROUP_CHAN0 = 0,
	GROUP_CHAN1 = 1,
	GROUP_CHAN2 = 2,
} group_chan_t;

static inline void pwm_ll_init(pwm_hw_t *hw)
{
	for (int group = 0; group < SOC_PWM_GROUP_NUM; group++)
		hw->group[group].ctrl.v = 0;
}

static inline void pwm_ll_set_enable(pwm_hw_t *hw, uint32_t group, uint32_t chan, uint32_t value)
{
	if (chan == GROUP_CHAN0)
		hw->group[group].ctrl.chan0_en = value;
	else if (chan == GROUP_CHAN1)
		hw->group[group].ctrl.chan1_en = value;
	else
		hw->group[group].ctrl.chan2_en = value;
}

static inline void pwm_ll_enable(pwm_hw_t *hw, uint32_t chan)
{
	pwm_ll_set_enable(hw, PWM_LL_GROUP(chan), PWM_LL_CHAN(chan), 1);
}

static inline void pwm_ll_disable(pwm_hw_t *hw, uint32_t chan)
{
	pwm_ll_set_enable(hw, PWM_LL_GROUP(chan), PWM_LL_CHAN(chan), 0);
}

static inline bool pwm_ll_is_chan_started(pwm_hw_t *hw, uint32_t chan)
{
	if (PWM_LL_CHAN(chan) == GROUP_CHAN0)
		return (hw->group[PWM_LL_GROUP(chan)].ctrl.chan0_en == 1);
	else if (PWM_LL_CHAN(chan) == GROUP_CHAN1)
		return (hw->group[PWM_LL_GROUP(chan)].ctrl.chan1_en == 1);
	else
		return (hw->group[PWM_LL_GROUP(chan)].ctrl.chan2_en == 1);
}

static inline void pwm_ll_set_interrupt(pwm_hw_t *hw, uint32_t group, uint32_t chan, uint32_t value)
{
	if (chan == GROUP_CHAN0)
		hw->group[group].ctrl.chan0_int_en = value;
	else if (chan == GROUP_CHAN1)
		hw->group[group].ctrl.chan1_int_en = value;
	else
		hw->group[group].ctrl.chan2_int_en = value;
}

static inline void pwm_ll_enable_interrupt(pwm_hw_t *hw, uint32_t chan)
{
	pwm_ll_set_interrupt(hw, PWM_LL_GROUP(chan), PWM_LL_CHAN(chan), 1);
}

static inline void pwm_ll_disable_interrupt(pwm_hw_t *hw, uint32_t chan)
{
	pwm_ll_set_interrupt(hw, PWM_LL_GROUP(chan), PWM_LL_CHAN(chan), 0);
}

/* If we found it impact the performance, we can rewrite it in register way!*/
static inline uint32_t pwm_ll_get_interrupt_status(pwm_hw_t *hw)
{
	uint32_t bit_pos = 0;
	uint32_t status = 0;

	for (int group = 0; group < SOC_PWM_GROUP_NUM; group++) {
		status |= hw->group[group].int_st.chan0_int_st << (bit_pos++);
		status |= hw->group[group].int_st.chan1_int_st << (bit_pos++);
		status |= hw->group[group].int_st.chan2_int_st << (bit_pos++);
	}

	return status;
}

static inline void pwm_ll_clear_interrupt_status(pwm_hw_t *hw, uint32_t status)
{
	uint32_t bit_pos = 0;

	for (int group = 0; group < SOC_PWM_GROUP_NUM; group++) {
		hw->group[group].int_st.chan0_int_st = (status >> (bit_pos++))& BIT(0);
		hw->group[group].int_st.chan1_int_st = (status >> (bit_pos++))& BIT(0);
		hw->group[group].int_st.chan2_int_st = (status >> (bit_pos++))& BIT(0);
	}
}

static inline void pwm_ll_set_chan_interrupt_status(pwm_hw_t *hw, uint32_t group, uint32_t chan)
{
	if (chan == GROUP_CHAN0)
		hw->group[group].int_st.chan0_int_st = 1;
	else if (chan == GROUP_CHAN1)
		hw->group[group].int_st.chan1_int_st = 1;
	else
		hw->group[group].int_st.chan2_int_st = 1;
}

static inline void pwm_ll_clear_chan_interrupt_status(pwm_hw_t *hw, uint32_t chan)
{
	pwm_ll_set_chan_interrupt_status(hw, PWM_LL_GROUP(chan), PWM_LL_CHAN(chan));
}

static inline bool pwm_ll_is_interrupt_triggered(pwm_hw_t *hw, uint32_t chan, uint32_t status)
{
	return (status & BIT(chan));
}

static inline void pwm_ll_set_mode(pwm_hw_t *hw, uint32_t chan, uint32_t mode)
{
	uint32_t group = PWM_LL_GROUP(chan);
	uint32_t group_chan = PWM_LL_CHAN(chan);

	if (group_chan == GROUP_CHAN0)
		hw->group[group].ctrl.chan0_mode = mode;
	else if (group_chan == GROUP_CHAN1)
		hw->group[group].ctrl.chan1_mode = mode;
	else
		hw->group[group].ctrl.chan2_mode = mode;
}

#define pwm_ll_set_mode_idle(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_IDLE)
#define pwm_ll_set_mode_pwm(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_PWM)
#define pwm_ll_set_mode_timer(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_TIMER)
#define pwm_ll_set_mode_counter(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_COUNTER)
#define pwm_ll_set_mode_capture_pos(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_CAPTURE_POS)
#define pwm_ll_set_mode_capture_neg(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_CAPTURE_NEG)
#define pwm_ll_set_mode_capture_edge(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_CAPTURE_EDGE)

static inline uint32_t pwm_ll_get_t4(pwm_hw_t *hw, uint32_t chan)
{
	return hw->group[PWM_LL_GROUP(chan)].end_dc_cap_out[PWM_LL_CHAN(chan)].counter_end;
}

static inline uint32_t pwm_ll_get_t1(pwm_hw_t *hw, uint32_t chan)
{
	return hw->group[PWM_LL_GROUP(chan)].end_dc_cap_out[PWM_LL_CHAN(chan)].duty_cycle;
}

static inline void pwm_ll_set_t4(pwm_hw_t *hw, uint32_t chan, uint32_t t4)
{
	hw->group[PWM_LL_GROUP(chan)].end_dc_cap_out[PWM_LL_CHAN(chan)].counter_end = t4;
}

static inline void pwm_ll_set_t1(pwm_hw_t *hw, uint32_t chan, uint32_t t1)
{
	hw->group[PWM_LL_GROUP(chan)].end_dc_cap_out[PWM_LL_CHAN(chan)].duty_cycle = t1;
}

#define pwm_ll_set_t2(hw, chan, t2)
#define pwm_ll_set_t3(hw, chan, t3)

static inline uint32_t pwm_ll_get_capture_value(pwm_hw_t *hw, uint32_t chan)
{
	return hw->group[PWM_LL_GROUP(chan)].end_dc_cap_out[PWM_LL_CHAN(chan)].capture_out;
}

static inline bool pwm_ll_is_duty_valid(uint32_t period, uint32_t duty1, uint32_t duty2,
										uint32_t duty3)
{
	return ((period > 0) && (period >= duty1));
}

static inline bool pwm_ll_is_capture_edge_valid(uint32_t edge)
{
	return ((edge == PWM_CAPTURE_POS) || (edge == PWM_CAPTURE_NEG) || (edge == PWM_CAPTURE_EDGE));
}

//TODO may need to reset more!
static inline void pwm_ll_reset_config_to_default(pwm_hw_t *hw, uint32_t chan)
{
	pwm_ll_set_t1(hw, chan, 0);
	pwm_ll_set_t2(hw, chan, 0);
	pwm_ll_set_t3(hw, chan, 0);
	pwm_ll_set_t4(hw, chan, 0);
}

static inline void pwm_ll_load_new_config(pwm_hw_t *hw, uint32_t chan)
{
	hw->group[PWM_LL_GROUP(chan)].config_update[PWM_LL_CHAN(chan)].chan_update = 1;
}

#define pwm_ll_set_init_signal_low(hw, chan)
#define pwm_ll_set_init_signal_high(hw, chan)
#define pwm_ll_is_hardware_group(hw, chan1, chan2) false
#define pwm_ll_start_hardware_group(hw, chan1, chan2)
#define pwm_ll_stop_hardware_group(hw, chan1, chan2)

static inline bool pwm_ll_is_pwm2_interrupt(pwm_hw_t *hw, uint32_t chan)
{
	if (chan >= 6)
		return true;

	return false;
}

#ifdef __cplusplus
}
#endif

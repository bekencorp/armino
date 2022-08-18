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

#define PWM0_LL_PIN    GPIO_6
#define PWM1_LL_PIN    GPIO_7
#define PWM2_LL_PIN    GPIO_8
#define PWM3_LL_PIN    GPIO_9
#define PWM4_LL_PIN    GPIO_24
#define PWM5_LL_PIN    GPIO_26

typedef enum {
	GROUP_CHAN0 = 0,
	GROUP_CHAN1 = 1,
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
	else
		hw->group[group].ctrl.chan1_en = value;
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
	else
		return (hw->group[PWM_LL_GROUP(chan)].ctrl.chan1_en == 1);
}

static inline void pwm_ll_set_interrupt(pwm_hw_t *hw, uint32_t group, uint32_t chan, uint32_t value)
{
	if (chan == GROUP_CHAN0)
		hw->group[group].ctrl.chan0_int_en = value;
	else
		hw->group[group].ctrl.chan1_int_en = value;
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
	uint32_t status = 0;

	for (int group = 0; group < SOC_PWM_GROUP_NUM; group++) {
		status |= hw->group[group].ctrl.chan0_int_st << (group << 1);
		status |= hw->group[group].ctrl.chan1_int_st << ((group << 1) + 1);
	}

	return status;
}

static inline void pwm_ll_clear_interrupt_status(pwm_hw_t *hw, uint32_t status)
{
	hw->group[0].ctrl.chan0_int_st = status & BIT(0);
	hw->group[0].ctrl.chan1_int_st = (status >> 1) & BIT(0);
	hw->group[1].ctrl.chan0_int_st = (status >> 2) & BIT(0);
	hw->group[1].ctrl.chan1_int_st = (status >> 3) & BIT(0);
	hw->group[2].ctrl.chan0_int_st = (status >> 4) & BIT(0);
	hw->group[2].ctrl.chan1_int_st = (status >> 5) & BIT(0);
}

static inline void pwm_ll_set_chan_interrupt_status(pwm_hw_t *hw, uint32_t group, uint32_t chan)
{
	if (chan == GROUP_CHAN0)
		hw->group[group].ctrl.chan0_int_st = 1;
	else
		hw->group[group].ctrl.chan1_int_st = 1;
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
	else
		hw->group[group].ctrl.chan1_mode = mode;
}

#define pwm_ll_set_mode_idle(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_IDLE)
#define pwm_ll_set_mode_pwm(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_PWM)
#define pwm_ll_set_mode_timer(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_TIMER)
#define pwm_ll_set_mode_counter(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_COUNTER)
#define pwm_ll_set_mode_capture_pos(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_CAPTURE_POS)
#define pwm_ll_set_mode_capture_neg(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_CAPTURE_NEG)
#define pwm_ll_set_mode_capture_edge(hw, chan) pwm_ll_set_mode(hw, chan, PWM_V_MODE_CAPTURE_EDGE)

static inline void pwm_ll_set_t4(pwm_hw_t *hw, uint32_t chan, uint32_t t4)
{
	hw->group[PWM_LL_GROUP(chan)].duty_cycle[PWM_LL_CHAN(chan)].t4 = t4;
}

static inline void pwm_ll_set_t1(pwm_hw_t *hw, uint32_t chan, uint32_t t1)
{
	hw->group[PWM_LL_GROUP(chan)].duty_cycle[PWM_LL_CHAN(chan)].t1 = t1;
}

static inline void pwm_ll_set_t2(pwm_hw_t *hw, uint32_t chan, uint32_t t2)
{
	hw->group[PWM_LL_GROUP(chan)].duty_cycle[PWM_LL_CHAN(chan)].t2 = t2;
}

static inline void pwm_ll_set_t3(pwm_hw_t *hw, uint32_t chan, uint32_t t3)
{
	hw->group[PWM_LL_GROUP(chan)].duty_cycle[PWM_LL_CHAN(chan)].t3 = t3;
}

static inline uint32_t pwm_ll_get_t1(pwm_hw_t *hw, uint32_t chan)
{
	return hw->group[PWM_LL_GROUP(chan)].duty_cycle[PWM_LL_CHAN(chan)].t1;
}

static inline uint32_t pwm_ll_get_t4(pwm_hw_t *hw, uint32_t chan)
{
	return hw->group[PWM_LL_GROUP(chan)].duty_cycle[PWM_LL_CHAN(chan)].t4;
}

static inline uint32_t pwm_ll_get_capture_value(pwm_hw_t *hw, uint32_t chan)
{
	uint32_t group = PWM_LL_GROUP(chan);
	uint32_t group_chan = PWM_LL_CHAN(chan);

	if (group_chan == GROUP_CHAN0) {
		hw->group[group].cnt_read_ctrl.rd0 = 1;

		//Wait hardware to prepare the data
		BK_WHILE (hw->group[group].cnt_read_ctrl.rd0);

		return hw->group[group].cnt_or_capture_rdata[0];
	} else {
		hw->group[group].cnt_read_ctrl.rd1 = 1;

		//Wait hardware to prepare the data
		BK_WHILE (hw->group[group].cnt_read_ctrl.rd1);

		return hw->group[group].cnt_or_capture_rdata[1];
	}
}

static inline bool pwm_ll_is_duty_valid(uint32_t period, uint32_t duty1, uint32_t duty2,
										uint32_t duty3)
{
	if (period == 0)
		return false;

	//Not consider (duty1 + duty2 + duty3) overflow
	if ((duty1 + duty2 + duty3) > period)
		return false;

	//Not consier (duty2 + duty3) overflow
	if ((duty1 == 0) && ((duty2 + duty3) != 0))
		return false;

	if ((duty2 == 0) && (duty3 != 0))
		return false;

	return true;
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
	if (PWM_LL_CHAN(chan) == GROUP_CHAN0)
		hw->group[PWM_LL_GROUP(chan)].ctrl.chan0_cfg_update = 1;
	else
		hw->group[PWM_LL_GROUP(chan)].ctrl.chan1_cfg_update = 1;
}

static inline void pwm_ll_set_init_signal(pwm_hw_t *hw, uint32_t group, uint32_t chan, uint32_t v)
{
	if (chan == GROUP_CHAN0)
		hw->group[group].ctrl.chan0_init_level = v;
	else
		hw->group[group].ctrl.chan1_init_level = v;
}

static inline void pwm_ll_set_init_signal_low(pwm_hw_t *hw, uint32_t chan)
{
	pwm_ll_set_init_signal(hw, PWM_LL_GROUP(chan), PWM_LL_CHAN(chan), 0);
}

static inline void pwm_ll_set_init_signal_high(pwm_hw_t *hw, uint32_t chan)
{
	pwm_ll_set_init_signal(hw, PWM_LL_GROUP(chan), PWM_LL_CHAN(chan), 1);
}

static inline bool pwm_ll_is_hardware_group(pwm_hw_t *hw, uint32_t chan1, uint32_t chan2)
{
	uint32_t small = chan1;
	uint32_t big = chan2;

	if (chan1 > chan2) {
		small = chan2;
		big = chan1;
	}

	if (((big - small) == 1) && ((small % SOC_PWM_CHAN_NUM_PER_GROUP) == 0))
		return true;

	return false;
}

//The caller make sure the parameters are valid!
static inline void pwm_ll_start_hardware_group(pwm_hw_t *hw, uint32_t chan1, uint32_t chan2)
{
	uint32_t group = PWM_LL_GROUP(chan1);

	hw->group[group].ctrl.group_en = 1;
	hw->group[group].ctrl.group_pwm_en = 1;
}

//The caller make sure the parameters are valid!
static inline void pwm_ll_stop_hardware_group(pwm_hw_t *hw, uint32_t chan1, uint32_t chan2)
{
	uint32_t group = PWM_LL_GROUP(chan1);

	hw->group[group].ctrl.group_en = 0;
	hw->group[group].ctrl.group_pwm_en = 0;
}

#define pwm_ll_is_pwm2_interrupt(hw, chan) false

#ifdef __cplusplus
}
#endif

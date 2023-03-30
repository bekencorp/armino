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

#include <pm/device.h>
#include <pm/pm.h>
#include "pm.h"
#include "sys_hal.h"
#include "sys.h"

DEVICE_EXPORT(sys);
pm_device_freq_t s_cpu_freq = PM_DEVICE_FREQ_120M;

static int pm_sys_enter_normal_sleep(const device_t *dev)
{
	PM_LOGV("%s+\r\n", __FUNCTION__);
	sys_hal_enter_normal_sleep();
	return BK_OK;
}

static int pm_sys_enter_low_voltage(const device_t *dev)
{
	PM_LOGV("%s+\r\n", __FUNCTION__);
	sys_hal_enter_low_voltage();
	return BK_OK;
}

static int pm_sys_enter_deep_sleep(const device_t *dev)
{
	PM_LOGV("%s+\r\n", __FUNCTION__);
	sys_hal_enter_deep_sleep(NULL);
	return BK_OK;
}

static const sys_pm_sm_t  s_pm_sys_sm[] = {
	{PM_STATE_ACTIVE,       NULL,                      NULL},
	{PM_STATE_NORMAL_SLEEP, pm_sys_enter_normal_sleep, NULL},
	{PM_STATE_LOW_VOLTAGE,  pm_sys_enter_low_voltage,  NULL},
	{PM_STATE_DEEP_SLEEP,   pm_sys_enter_deep_sleep,   NULL},
};

static const char* sys_get_freq_str(pm_device_freq_t freq)
{
	switch(freq) {
	case PM_DEVICE_FREQ_26M:
		return "26M";
	case PM_DEVICE_FREQ_120M:
		return "120M";
	case PM_DEVICE_FREQ_240M:
		return "240M";
	case PM_DEVICE_FREQ_320M:
		return "320M";
	default:
		return "UNKNOWN FREQ";
	}
}

static uint32_t sys_get_clock_rate(pm_device_freq_t freq)
{
	switch (freq) {
	case PM_DEVICE_FREQ_26M:
		return 26000000;
	case PM_DEVICE_FREQ_120M:
		return 120000000;
	case PM_DEVICE_FREQ_240M:
		return 240000000;
	case PM_DEVICE_FREQ_320M:
		return 320000000;
	default:
		return 26000000;
	}
}

//TODO move it to sys driver
typedef struct {
	pm_device_freq_t freq;
	uint32_t cksel_core;
	uint32_t clkdiv_core;
	uint32_t clkdiv_bus;
} cpu_freq_reg_map_t;

//TODO use correct core/bus configurations
#define CPU_FREQ_REG_MAP \
{\
	{PM_DEVICE_FREQ_26M, 0, 0, 0},  /* clock-XTAL-26M, core=26M, bus=26M */\
	{PM_DEVICE_FREQ_120M, 3, 3, 0}, /* clock-PLL-480M, core=120M, bus=120M */\
	{PM_DEVICE_FREQ_240M, 3, 2, 1}, /* clock-PLL-480M, core=240M, bus=120M */\
	{PM_DEVICE_FREQ_320M, 2, 1, 0} /* clock-PLL-320M, core=320M, bus=160M */\
}

static void sys_hal_set_cpu_clock_source(pm_device_freq_t freq)
{
	cpu_freq_reg_map_t map[] = CPU_FREQ_REG_MAP;
	sys_ll_set_cpu_clk_div_mode1_cksel_core(map[freq].cksel_core);
}

static void sys_hal_set_cpu_clock_div(pm_device_freq_t freq)
{
	cpu_freq_reg_map_t map[] = CPU_FREQ_REG_MAP;
	sys_ll_set_cpu_clk_div_mode1_clkdiv_core(map[freq].clkdiv_core);
	sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(map[freq].clkdiv_bus);
}


static int pm_sys_action_update_freq(void)
{
	const device_t *sys = DEVICE_ID2PTR(sys);
	pm_device_freq_t max_freq = PM_DEVICE_FREQ_26M;

	PM_DEPEND_FOREACH(sys->pm->children, entry_ptr) {
		if ((entry_ptr->type == PM_DEPEND_TYPE_VOTE_FREQ) && (entry_ptr->value > max_freq) ) {
			max_freq = entry_ptr->value;
		}
	}

	PM_LOGD("cpu freq %s -> %s\r\n", sys_get_freq_str(s_cpu_freq), sys_get_freq_str(max_freq));
	if (max_freq > s_cpu_freq) {
		sys_hal_set_cpu_clock_div(max_freq);
		sys_hal_set_cpu_clock_source(max_freq);
	} else if (max_freq < s_cpu_freq) {
		sys_hal_set_cpu_clock_source(max_freq);
		sys_hal_set_cpu_clock_div(max_freq);
	}
	PM_LOGD("sys_r8=%x\r\n", sys_ll_get_cpu_clk_div_mode1_value());
	s_cpu_freq = max_freq;
	return BK_OK;
}

static int pm_sys_action_cb(const device_t *device, pm_device_action_t action)
{
	pm_state_t state = pm_get_state();

	if (action == PM_DEVICE_ACTION_UPDATE_FREQ) {
		pm_sys_action_update_freq();
	} else if (action == PM_DEVICE_ACTION_SUSPEND) {
		if (s_pm_sys_sm[state].suspend) {
			s_pm_sys_sm[state].suspend(device);
		} 
	} else if (action == PM_DEVICE_ACTION_RESUME) {
		if (s_pm_sys_sm[state].resume) {
			s_pm_sys_sm[state].resume(device);
		}
	}

	return BK_OK;
}

int pm_sys_suspend(void)
{
	const device_t *sys = DEVICE_ID2PTR(sys);
	return pm_sys_action_cb(sys, PM_DEVICE_ACTION_SUSPEND);
}

int pm_sys_resume(void)
{
	const device_t *sys = DEVICE_ID2PTR(sys);
	return pm_sys_action_cb(sys, PM_DEVICE_ACTION_RESUME);
}

PM_DEVICE_DEFINE(sys, null, PM_DEVICE_FLAG_PASSIVE_DEEP_SLEEP, 0, pm_sys_action_cb);

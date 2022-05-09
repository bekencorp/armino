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

#include "hal_config.h"
#include "pwm_hw.h"
#include "pwm_hal.h"
#include "pwm_ll.h"

//TODO finally we will automatically generate this code

#if CFG_HAL_DEBUG_PWM
void pwm_struct_dump(void)
{
	pwm_hw_t *hw = (pwm_hw_t *)PWM_LL_REG_BASE(0); //Currently only 1 PWM unit
	SOC_LOGI("base=%x\n", (uint32_t)hw);
	SOC_LOGI("ctrl=%x value=%x\n", &hw->ctrl, hw->ctrl.v);

	SOC_LOGI("  chan0_en: %x\n", hw->ctrl.chan0_en);
	SOC_LOGI("  chan0_int_en: %x\n", hw->ctrl.chan0_int_en);
	SOC_LOGI("  chan0_mode: %x\n", hw->ctrl.chan0_mode);

	SOC_LOGI("  chan1_en: %x\n", hw->ctrl.chan1_en);
	SOC_LOGI("  chan1_int_en: %x\n", hw->ctrl.chan1_int_en);
	SOC_LOGI("  chan1_mode: %x\n", hw->ctrl.chan1_mode);

	SOC_LOGI("  chan2_en: %x\n", hw->ctrl.chan2_en);
	SOC_LOGI("  chan2_int_en: %x\n", hw->ctrl.chan2_int_en);
	SOC_LOGI("  chan2_mode: %x\n", hw->ctrl.chan2_mode);

	SOC_LOGI("  chan3_en: %x\n", hw->ctrl.chan3_en);
	SOC_LOGI("  chan3_int_en: %x\n", hw->ctrl.chan3_int_en);
	SOC_LOGI("  chan3_mode: %x\n", hw->ctrl.chan3_mode);

	SOC_LOGI("  chan4_en: %x\n", hw->ctrl.chan4_en);
	SOC_LOGI("  chan4_int_en: %x\n", hw->ctrl.chan4_int_en);
	SOC_LOGI("  chan4_mode: %x\n", hw->ctrl.chan4_mode);

	SOC_LOGI("  chan5_en: %x\n", hw->ctrl.chan5_en);
	SOC_LOGI("  chan5_int_en: %x\n", hw->ctrl.chan5_int_en);
	SOC_LOGI("  chan5_mode: %x\n", hw->ctrl.chan5_mode);

	SOC_LOGI("\n");
	SOC_LOGI("int_status=%x value=%x\n", &hw->int_status,
			 hw->int_status.v);

	SOC_LOGI("  chan0_int_status: %x\n", hw->int_status.chan0_int_status);
	SOC_LOGI("  chan1_int_status: %x\n", hw->int_status.chan1_int_status);
	SOC_LOGI("  chan2_int_status: %x\n", hw->int_status.chan2_int_status);
	SOC_LOGI("  chan3_int_status: %x\n", hw->int_status.chan3_int_status);
	SOC_LOGI("  chan4_int_status: %x\n", hw->int_status.chan4_int_status);
	SOC_LOGI("  chan5_int_status: %x\n", hw->int_status.chan5_int_status);

	for (int i = 0; i < SOC_PWM_CHAN_NUM_PER_UNIT; i++) {
		SOC_LOGI("\n");
		SOC_LOGI("freq_duty_cap_out[%x]=%x\n", &hw->freq_duty_capture_out[i], hw->freq_duty_capture_out[i]);
		SOC_LOGI("period_cycle: %x\n", hw->freq_duty_capture_out[i].period_cycle);
		SOC_LOGI("duty_cycle: %x\n", hw->freq_duty_capture_out[i].duty_cycle);
		SOC_LOGI("capture_out: %x\n", hw->freq_duty_capture_out[i].capture_out);
	}
}

#endif

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

	for (int group = 0; group < SOC_PWM_GROUP_NUM; group ++) {
		SOC_LOGI("group(%x)\n", group);
		SOC_LOGI("  ctrl=%x value=%x\n", &hw->group[group].ctrl, hw->group[group].ctrl.v);

		SOC_LOGI("    chan0_en: %x\n", hw->group[group].ctrl.chan0_en);
		SOC_LOGI("    chan0_int_en: %x\n", hw->group[group].ctrl.chan0_int_en);
		SOC_LOGI("    chan0_mode: %x\n", hw->group[group].ctrl.chan0_mode);

		SOC_LOGI("    chan1_en: %x\n", hw->group[group].ctrl.chan1_en);
		SOC_LOGI("    chan1_int_en: %x\n", hw->group[group].ctrl.chan1_int_en);
		SOC_LOGI("    chan1_mode: %x\n", hw->group[group].ctrl.chan1_mode);

		SOC_LOGI("    chan2_en: %x\n", hw->group[group].ctrl.chan2_en);
		SOC_LOGI("    chan2_int_en: %x\n", hw->group[group].ctrl.chan2_int_en);
		SOC_LOGI("    chan2_mode: %x\n", hw->group[group].ctrl.chan2_mode);

		SOC_LOGI("    rfu: %x\n", hw->group[group].ctrl.rfu);
		SOC_LOGI("    pre_div: %x\n", hw->group[group].ctrl.pre_div);
		SOC_LOGI("    reserved: %x\n", hw->group[group].ctrl.reserved);

		SOC_LOGI("\n");
		SOC_LOGI("  int_st(%x)=%x\n", &hw->group[group].int_st, hw->group[group].int_st);
		SOC_LOGI("    chan0_int_st=%x\n", hw->group[group].int_st.chan0_int_st);
		SOC_LOGI("    chan1_int_st=%x\n", hw->group[group].int_st.chan1_int_st);
		SOC_LOGI("    chan2_int_st=%x\n", hw->group[group].int_st.chan2_int_st);
		SOC_LOGI("    reserved=%x\n", hw->group[group].int_st.reserved);

		SOC_LOGI("\n");
		for (int chan = 0; chan < SOC_PWM_CHAN_NUM_PER_GROUP; chan++) {
			SOC_LOGI("  end_dc_cap_out(%x), chan(%x) counter_end=%x dc=%x capture_out=%x\n",
					&hw->group[group].end_dc_cap_out[chan], chan,
					 hw->group[group].end_dc_cap_out[chan].counter_end,
					 hw->group[group].end_dc_cap_out[chan].duty_cycle,
					 hw->group[group].end_dc_cap_out[chan].capture_out);
		}

		SOC_LOGI("\n");
		SOC_LOGI("  cnt_read_ctrl(%x)=%x\n", &hw->group[group].counter_read_ctrl, hw->group[group].counter_read_ctrl);
		SOC_LOGI("    read_end=%x\n", hw->group[group].counter_read_ctrl.read_en);
		SOC_LOGI("    chan_id=%x\n", hw->group[group].counter_read_ctrl.chan_id);

		SOC_LOGI("\n");
		SOC_LOGI("  cnt_read(%x)=%x\n", &hw->group[group].cnt_read, hw->group[group].cnt_read);
	}
}

#endif

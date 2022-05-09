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
#include "adc_hw.h"
#include "adc_hal.h"
#include "hal_config.h"

//TODO finally we will automatically generate this code
#if CFG_HAL_DEBUG_ADC

void adc_struct_dump(void)
{
	adc_hw_t *hw = (adc_hw_t *)ADC_LL_REG_BASE();
	SOC_LOGI("base=0x%x\n", (uint32_t)hw);

	SOC_LOGI("  ctrl=0x%x , v =0x%x\n", &hw->ctrl, hw->ctrl.v);
	SOC_LOGI("  adc_mode=%x\n", hw->ctrl.adc_mode);
	SOC_LOGI("  adc_enable: %x\n", hw->ctrl.adc_en);
	SOC_LOGI("  adc_channel=%x\n", hw->ctrl.adc_channel);
	SOC_LOGI("  adc_setting=%x\n", hw->ctrl.adc_setting);
	SOC_LOGI("  adc_int_clear=%x\n", hw->ctrl.adc_int_clear);
	SOC_LOGI("  adc_div=0x%x\n", hw->ctrl.adc_div);
	SOC_LOGI("  adc_32m_mode_enable=%x\n", hw->ctrl.adc_32m_mode);
	SOC_LOGI("  adc_sample_rate=0x%x\n", hw->ctrl.adc_samp_rate);
	SOC_LOGI("  adc_filter=%x\n", hw->ctrl.adc_filter);
	SOC_LOGI("  adc_busy=%x\n", hw->ctrl.adc_busy);
	SOC_LOGI("  adc_fifo_empty=%x\n", hw->ctrl.fifo_empty);
	SOC_LOGI("  adc_fifo_full=%x\r\n", hw->ctrl.fifo_full);

	SOC_LOGI("  adc_raw_data=0x%x\r\n", hw->adc_raw_data);

	SOC_LOGI("  steady_ctrl=0x%x, v = 0x%x \n", &hw->steady_ctrl, hw->steady_ctrl.v);
	SOC_LOGI("  fifo_level=%x\n", hw->steady_ctrl.fifo_level);
	SOC_LOGI("  steady_ctrl: %x\n", hw->steady_ctrl.steady_ctrl);
	SOC_LOGI("  reserved=%x\r\n", hw->steady_ctrl.reserved);

	SOC_LOGI("  sat_ctrl=0x%x, v = 0x%x \n", &hw->sat_ctrl, hw->sat_ctrl.v);
	SOC_LOGI("  sat_ctrl=%x\n", hw->sat_ctrl.sat_ctrl);
	SOC_LOGI("  sat_ctrl_en=%x\n", hw->sat_ctrl.sat_enable);
	SOC_LOGI("  over_flow_flag: %x\r\n", hw->sat_ctrl.over_flow);

	SOC_LOGI("  adc_data=0x%x\r\n", hw->adc_data);
}

#endif

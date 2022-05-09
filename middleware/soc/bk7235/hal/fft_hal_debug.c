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
#include "fft_hal.h"
#include "fft_ll_macro_def.h"

#if CFG_HAL_DEBUG_FFT

void fft_struct_dump(void)
{
	SOC_LOGI("\r\nbase=%x\r\n", (uint32_t)FFT_LL_REG_BASE);

	/* REG_0x00 */
	SOC_LOGI("  fft_config=0x%x value=0x%x\n", FFT_FFT_CONFIG_ADDR, fft_ll_get_fft_config_value());
	SOC_LOGI("    fft_mode: %x\n", fft_ll_get_fft_config_fft_mode());
	SOC_LOGI("    ifft:    %x\n", fft_ll_get_fft_config_ifft());
	SOC_LOGI("    fft_int_en:    %x\n", fft_ll_get_fft_config_fft_int_en());
	SOC_LOGI("    fft_enable:   %x\n", fft_ll_get_fft_config_fft_enable());
	SOC_LOGI("    fft_gat_off:   %x\n", fft_ll_get_fft_config_fft_gat_off());

	/* REG_0x01 */
	SOC_LOGI("\n");
	SOC_LOGI("  fir_config0=0x%x value=0x%x\n", FFT_FIR_CONFIG_ADDR, fft_ll_get_fir_config_value());
	SOC_LOGI("    fir_length:     %x\n", fft_ll_get_fir_config_fir_length());
	SOC_LOGI("    fir_mode:        %x\n", fft_ll_get_fir_config_fir_mode());
	SOC_LOGI("    fir_int_en:  %x\n", fft_ll_get_fir_config_fir_int_en());
	SOC_LOGI("    fir_enable: %x\n", fft_ll_get_fir_config_fir_enable());

	/* REG_0x06 */
	SOC_LOGI("\n");
	SOC_LOGI("  status=0x%x value=0x%x\n", FFT_STATUS_ADDR, fft_ll_get_status_value());
	SOC_LOGI("    fft_done:   %x\n", fft_ll_get_status_fft_done());
	SOC_LOGI("    fir_done:   %x\n", fft_ll_get_status_fir_done());
	SOC_LOGI("    self_proc_done: %x\n", fft_ll_get_status_self_proc_done());
	SOC_LOGI("    bit_ext: %x\n", fft_ll_get_status_bit_ext());

	/* REG_0x07 */
	SOC_LOGI("\n");
	SOC_LOGI("  start_trigger=0x%x value=0x%x\n", FFT_START_TRIGGER_ADDR, fft_ll_get_start_trigger_value());
	SOC_LOGI("    start_trigger:   %x\n", fft_ll_get_start_trigger_start_trigger());

	/* REG_0x09 */
	SOC_LOGI("\n");
	SOC_LOGI("  memory_config=0x%x value=0x%x\n", FFT_MEMORY_CONFIG_ADDR, fft_ll_get_memory_config_value());
	SOC_LOGI("    memory_config_mode: %x\n", fft_ll_get_memory_config_memory_config_mode());

}

#endif


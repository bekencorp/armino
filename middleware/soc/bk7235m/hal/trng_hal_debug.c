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
#include "trng_hw.h"
#include "trng_hal.h"
#include "trng_ll.h"

#if CFG_HAL_DEBUG_TRNG

void trng_struct_dump(void)
{
	trng_hw_t *hw = (trng_hw_t *)TRNG_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  ctrl=0x%x value=0x%x\r\n", &hw->ctrl, hw->ctrl.v);
	SOC_LOGI("    en: 0x%x\r\n", hw->ctrl.en);
	SOC_LOGI("\r\n");

	SOC_LOGI("  data addr=0x%x value=0x%x\r\n", &hw->data, hw->data);
}

#endif


// Copyright 2023-2028 Beken
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

#include "bk7236xx.h"
#include "cache.h"
#include "mpu.h"

void sram_dcache_map(void)
{

}


int show_cache_config_info(void)
{
	return 0;
}


void flush_dcache(void *va, long size)
{
    if (SCB->CLIDR & SCB_CLIDR_DC_Msk) {
        SCB_InvalidateDCache_by_Addr(va, size);
    }
}


void flush_all_dcache(void) {
    if (SCB->CLIDR & SCB_CLIDR_DC_Msk) {
        SCB_CleanInvalidateDCache();
    }
}


void enable_dcache(int enable) {
    if (enable == 0) {
        SCB_DisableDCache();
#if CONFIG_MPU
        mpu_disable();
#endif // #if CONFIG_MPU
    } else {
#if CONFIG_MPU
        mpu_enable();
#endif // #if CONFIG_MPU
        SCB_EnableDCache();
    }
}
void invalidate_icache(void) {
    SCB_InvalidateICache();
}

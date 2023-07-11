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

#include <common/bk_include.h>
#include <components/log.h>
#include <common/bk_err.h>
#include <components/system.h>
#include <driver/wdt.h>
#include "bk_misc.h"
#include "reset_reason.h"
#include "drv_model_pub.h"
#include "param_config.h"
#include "aon_pmu_driver.h"
#include "wdt_driver.h"

#define TAG "sys"



#if (CONFIG_SOC_BK7256XX)
static uint32_t bk_get_return_address_value()
{
	UINT32 return_address_value;

	__asm volatile( "mv %0, ra":"=r"( return_address_value ) );

	return return_address_value;
}
#endif

void bk_reboot_ex(uint32_t reset_reason)
{
#if !CONFIG_SLAVE_CORE
	BK_LOGI(TAG, "bk_reboot\r\n");

	if (reset_reason < RESET_SOURCE_UNKNOWN) {
		bk_misc_set_reset_reason(reset_reason);
	}

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236A) || (CONFIG_SOC_BK7256XX)
	delay_ms(100); //add delay for bk_writer BEKEN_DO_REBOOT cmd
#endif

#if CONFIG_AON_RTC_KEEP_TIME_SUPPORT
{
	/* 
	 * NOTES:special requirements
	 * Some customers system reboot, needs to reserve the network time,
	 * maybe after reboot, the network can't work at once.
	 * so before reboot, save the network time to flash.
	 */
	extern bk_err_t aon_rtc_enter_reboot(void);
	aon_rtc_enter_reboot();
}
#endif

	BK_LOGI(TAG, "wdt reboot\r\n");
	bk_wdt_force_reboot();
#endif //#if !CONFIG_SLAVE_CORE
}

void bk_reboot(void)
{
	bk_reboot_ex(RESET_SOURCE_REBOOT);
}

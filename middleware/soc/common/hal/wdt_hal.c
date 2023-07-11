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

#include "wdt_hal.h"
#include "wdt_ll.h"
#include "aon_pmu_driver.h"
#include "reset_reason.h"

static inline void wdt_hal_close_unused(wdt_unit_t id);

bk_err_t wdt_hal_init(wdt_hal_t *hal)
{
#if (CONFIG_SOC_BK7256XX)
#if CONFIG_DEBUG_FIRMWARE || CONFIG_NMI_WDT_EN
        hal->id = 1;  //debug version use nmi_wdt to dump
#else
	if (aon_pmu_hal_is_chipid_later_than_version_C())
		hal->id = 0;  //chip version C -> aon_wdt
	else
		hal->id = 1;  //chip version A -> nmi_wdt
#endif
#endif

	wdt_hal_close_unused(hal->id);

	hal->hw = (wdt_hw_t *)WDT_LL_REG_BASE(hal->id);
	wdt_ll_init(hal->hw);
	return BK_OK;
}

bk_err_t wdt_hal_init_wdt(wdt_hal_t *hal, uint32_t timeout)
{
	wdt_ll_set_period(hal->hw, timeout);
	return BK_OK;
}

#if (CONFIG_SOC_BK7256XX)
void wdt_hal_close(void)
{
        REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5A0000);
        REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA50000);

        REG_WRITE(SOC_WDT_REG_BASE, 0x5A0000);
        REG_WRITE(SOC_WDT_REG_BASE, 0xA50000);
}

void wdt_hal_force_feed(void)
{
        REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5AFFFC);
        REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA5FFFC);

        REG_WRITE(SOC_WDT_REG_BASE, 0x5AFFF0);
        REG_WRITE(SOC_WDT_REG_BASE, 0xA5FFF0);
}

static inline void wdt_hal_close_unused(wdt_unit_t id)
{
        if (id == 0) {
                // use aon_wdt, close nmi_wdt
                REG_WRITE(SOC_WDT_REG_BASE, 0x5A0000);
                REG_WRITE(SOC_WDT_REG_BASE, 0xA50000);
        } else {
                // use nmi_wdt, close aon_wdt
                REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5A0000);
                REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA50000);
        }
}

static inline void wdt_hal_nmi_reboot() {
        set_reboot_tag(REBOOT_TAG_REQ);
        set_nmi_vector();
        aon_pmu_drv_wdt_change_not_rosc_clk();
        aon_pmu_drv_wdt_rst_dev_enable();

        REG_WRITE(SOC_WDT_REG_BASE, 0x5A000A);
        REG_WRITE(SOC_WDT_REG_BASE, 0xA5000A);
}

void wdt_hal_force_reboot(void)
{
#if CONFIG_DEBUG_FIRMWARE || CONFIG_NMI_WDT_EN
        wdt_hal_nmi_reboot();
#else
        if (aon_pmu_hal_is_chipid_later_than_version_C())
        {
                REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5A000A);
                REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA5000A);
        } else {
                wdt_hal_nmi_reboot();
        }
#endif //#if CONFIG_DEBUG_FIRMWARE
}

#endif

#if (CONFIG_SOC_BK7236XX)
void wdt_hal_close(void)
{
        REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5A0000);
        REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA50000);

        REG_SET(SOC_WDT_REG_BASE + 4 * 2, 1, 1, 1);
        REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0x5A0000);
        REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0xA50000);
}

void wdt_hal_force_feed(void)
{
        REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5AFFFC);
        REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA5FFFC);

        REG_SET(SOC_WDT_REG_BASE + 4 * 2, 1, 1, 1);
        REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0x5AFFF0);
        REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0xA5FFF0);
}

static inline void wdt_hal_close_unused(wdt_unit_t id)
{
        if (id == 0) {
                // use aon_wdt, close nmi_wdt
                REG_SET(SOC_WDT_REG_BASE + 4 * 2, 1, 1, 1);
                REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0x5A0000);
                REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0xA50000);
        } else {
                // use nmi_wdt, close aon_wdt
                REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5A0000);
                REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA50000);
        }
}

void wdt_hal_force_reboot(void)
{
        REG_WRITE(SOC_AON_WDT_REG_BASE, 0x5A000A);
        REG_WRITE(SOC_AON_WDT_REG_BASE, 0xA5000A);
}

#endif

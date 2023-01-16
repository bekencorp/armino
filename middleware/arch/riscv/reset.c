/*
 * Copyright (c) 2012-2018 Andes Technology Corporation
 * All rights reserved.
 *
 */
#include"platform.h"
#include "cache.h"
#include <os/os.h>
#include "reset_reason.h"
#include "bk_pm_internal_api.h"
#include "boot.h"
#include "mon_call.h"

#if CONFIG_ATE_TEST
extern void c_startup_ate(void);
#endif
extern void c_startup(void);
// extern void system_init(void);
extern void __libc_init_array(void);
extern void init_pmp_config();
extern void entry_main(void);

void close_wdt(void)
{
	/*close the wdt*/
	*((volatile unsigned long *) (0x44000600)) = 0x5A0000;
	*((volatile unsigned long *) (0x44000600)) = 0xA50000;
	*((volatile unsigned long *) (0x44800000)) = 0x5A0000;
	*((volatile unsigned long *) (0x44800000)) = 0xA50000;
}
//volatile int g_debug_mode=1;
void reset_handler(void)
{
    /// TODO: DEBUG VERSION close the wdt
	close_wdt();

#if (!CONFIG_SLAVE_CORE)
	sram_dcache_map();
#endif

	/*
	 * Initialize LMA/VMA sections.
	 * Relocation for any sections that need to be copied from LMA to VMA.
	 */
	c_startup();

	/* Call platform specific hardware initialization */
	// system_init();

	/* Do global constructors */
	__libc_init_array();

	mon_app_started();  // app started, let monitor forward exceptions to U-mode.

	/*power manager init*/
	pm_hardware_init();

#if (CONFIG_SOC_BK7256XX)
	//clear mannully reboot flag
	set_reboot_tag(0);
#endif

	/*Init pmp configuration*/
	// init_pmp_config();
	
	/* Entry function */
	entry_main();

}

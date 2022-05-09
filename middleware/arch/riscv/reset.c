/*
 * Copyright (c) 2012-2018 Andes Technology Corporation
 * All rights reserved.
 *
 */
#include"core_v5.h"
#include <os/os.h>
#include "bk_private/reset_reason.h"
#include "bk_pm_internal_api.h"

extern void c_startup(void);
extern void system_init(void);
extern void __libc_init_array(void);
extern void init_pmp_config();
extern void entry_main(void);
extern void UartDbgInit();
extern int print_str(char * st);

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

    //write_csr(NDS_MCACHE_CTL, (read_csr(NDS_MCACHE_CTL) | 0x1)); // Enable  ICache
	//write_csr(NDS_MCACHE_CTL, (read_csr(NDS_MCACHE_CTL) | 0x2)); // Enable  DCache
	/*
	 * Initialize LMA/VMA sections.
	 * Relocation for any sections that need to be copied from LMA to VMA.
	 */
	c_startup();

	/*power manager init*/
	pm_hardware_init();

#if (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7256_CP1)
	//clear mannully reboot flag
	set_reboot_tag(0);
#endif

	/* Call platform specific hardware initialization */
	system_init();
    //while(g_debug_mode){};

	/* Do global constructors */
	__libc_init_array();

	/*Init pmp configuration*/
	init_pmp_config();
	
	/* Entry function */
	entry_main();

}

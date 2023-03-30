#include <stdio.h>
#include <stdlib.h>
#include <common/bk_include.h>
#include "cm_backtrace.h"
#include "sys_misc.h"

void _start(void)
{
	close_wdt();
	backtrace_init();
	bk7236_sys_init();
	psram_init();
	entry_main();
}

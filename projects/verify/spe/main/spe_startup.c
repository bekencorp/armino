#include <stdio.h>
#include <stdlib.h>
#include <common/bk_include.h>
#include "rtos_init.h"
#include <os/os.h>
#include "bk_private/bk_driver.h"
#include "bk_api_cli.h"
#include "cm_backtrace.h"
#include <os/os.h>
#include "bk_api_cli.h"
#include "sys_misc.h"

void _start(void)
{
	close_wdt();
	backtrace_init();
	bk7236_sys_init();
	driver_init();
	BK_LOG_ON_ERR(rtos_init());
	BK_LOG_ON_ERR(bk_cli_init());
	rtos_start_scheduler();
}


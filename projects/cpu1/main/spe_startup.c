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
#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include "sys_driver.h"
#include <driver/int.h>
#include <driver/timer.h>
#include <driver/gpio.h>
#include <driver/uart.h>
#include <driver/wdt.h>
#include <driver/aon_wdt.h>
#include <driver/aon_rtc.h>
#include "bk_driver.h"
#include "interrupt_base.h"

void _start(void)
{
	close_wdt();

#if CONFIG_MINI_INIT
	sys_drv_init();
	interrupt_init();
	bk_gpio_driver_init();
	bk_uart_driver_init();
	bk_timer_driver_init();
	bk_aon_rtc_driver_init();
#else
	backtrace_init();
	bk7236_sys_init();
	driver_init();
#endif
	BK_LOG_ON_ERR(rtos_init());
	BK_LOG_ON_ERR(bk_cli_init());
	rtos_start_scheduler();
}


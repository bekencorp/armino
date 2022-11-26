#include <stdio.h>
#include <stdlib.h>
#include <common/bk_include.h>
#include <os/os.h>
#include <driver/gpio.h>
#include <driver/uart.h>
#include "bk_private/bk_driver.h"
#include "bk_api_cli.h"
#include "rtos_init.h"
#include "sys_driver.h"
#include "interrupt_base.h"
#include "sys_misc.h"

void _start(void)
{
	//close_wdt();
	backtrace_init();
	BK_LOG_ON_ERR(rtos_init());
	sys_drv_init();
	interrupt_init();
	//bk7236_sys_init();
	bk_gpio_driver_init();
	bk_uart_driver_init();
	os_printf("hello armino NS\r\n");
	BK_LOG_ON_ERR(bk_cli_init());
	rtos_start_scheduler();
}


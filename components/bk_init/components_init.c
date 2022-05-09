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

// Notes:
// This file only contain OS-independent components initialization code,
// For OS-dependently initialization, put them to OSK or bk_system.

#include <common/bk_include.h>
#include "bk_sys_ctrl.h"
#include "bk_drv_model.h"
#include "bk_private/bk_ate.h"
#include <driver/wdt.h>
#include "bk_wdt.h"
#include <common/sys_config.h>
#include "release.h"
#include "sys_driver.h"
#if CONFIG_SOC_BK7256XX
#include "BK7256_RegList.h"
#endif
#if CONFIG_FULLY_HOSTED
#include "mmgmt.h"
#endif

#include "bk_private/reset_reason.h"

#if CONFIG_EASY_FLASH && (!CONFIG_RTT)
#include "easyflash.h"
#include "bk_ef.h"
#endif

#include <components/log.h>
#include "bk_pm_control.h"
#include <driver/trng.h>

#include "bk_arch.h"
#include "bk_private/bk_driver.h"

#include "mb_ipc_cmd.h"

#define TAG "init"

//TODO move to better place
#if CONFIG_MEM_DEBUG
/* memory leak timer at 1sec */
static beken_timer_t memleak_timer = {0};
int bmsg_memleak_check_sender();

void memleak_timer_cb(void *arg)
{
	bmsg_memleak_check_sender();
}

void mem_debug_start_timer(void)
{
	bk_err_t err;

	err = rtos_init_timer(&memleak_timer,
						  1000,
						  memleak_timer_cb,
						  (void *)0);
	BK_ASSERT(kNoErr == err);
	err = rtos_start_timer(&memleak_timer);
	BK_ASSERT(kNoErr == err);
}
#endif

int random_init(void)
{
#if (CONFIG_TRNG_SUPPORT)
	BK_LOGI(TAG, "trng enable\r\n");
	bk_trng_start();
#endif
	return BK_OK;
}

int wdt_init(void)
{
#if (CONFIG_FREERTOS)
#if CONFIG_INT_WDT
	BK_LOGI(TAG, "int watchdog enabled, period=%u\r\n", CONFIG_INT_WDT_PERIOD_MS);
	bk_wdt_start(CONFIG_INT_WDT_PERIOD_MS);
#else
#if (CONFIG_SOC_BK7271)
	BK_LOGI(TAG, "watchdog disabled\r\n");
	bk_wdt_start(CONFIG_INT_WDT_PERIOD_MS);
	bk_wdt_feed();
	bk_wdt_stop();
#endif
#endif //CONFIG_INT_WDT
#endif //CONFIG_FREERTOS

#if !(CONFIG_ALIOS)
#if CONFIG_TASK_WDT
	bk_task_wdt_start();
	BK_LOGI(TAG, "task watchdog enabled, period=%u\r\n", CONFIG_TASK_WDT_PERIOD_MS);
#endif
#endif
	return BK_OK;
}

int memory_debug_todo(void)
{
#if CONFIG_MEM_DEBUG
	mem_debug_start_timer();
#endif
	return BK_OK;
}
//TODO put it to better place, check with XB/HT
static int pm_init_todo(void)
{
#if CONFIG_SOC_BK7256XX || CONFIG_SOC_BK7256_CP1

#else
#if CONFIG_DEEP_PS
	bk_init_deep_wakeup_gpio_status();
#endif
#endif
	return BK_OK;
}

static inline void show_sdk_version(void)
{
#if (CONFIG_CMAKE)
	//BK_LOGI(TAG, "armino rev: %s\r\n", ARMINO_VER);
	BK_LOGI(TAG, "armino rev: %s\r\n", "");
#else
	//BK_LOGI(TAG, "armino rev: %s\r\n", BEKEN_SDK_REV);
	BK_LOGI(TAG, "armino rev: %s\r\n", "");
#endif
}

static inline void show_chip_id(void)
{
	// BK_LOGI(TAG, "armino soc id:%x_%x\r\n",sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_DEVICE_ID, NULL),
	// 	sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_CHIP_ID, NULL));
	BK_LOGI(TAG, "armino soc id:%x_%x\r\n", sys_drv_get_device_id(), sys_drv_get_chip_id());
}

static inline void show_sdk_lib_version(void)
{
#if (!CONFIG_SOC_BK7256_CP1)
	extern char* bk_get_internal_lib_version(void);
	char* ver = bk_get_internal_lib_version();
	BK_LOGI(TAG, "armino internal lib rev: %s\n", ver);
#endif
}

static void show_armino_version(void)
{
	show_sdk_version();
	show_sdk_lib_version();
	show_chip_id();
}

static void show_init_info(void)
{
#if CONFIG_SOC_BK7256XX || CONFIG_SOC_BK7256_CP1
    show_armino_version();
#else
	show_reset_reason();
	show_armino_version();
#endif
}
#if CONFIG_SOC_BK7256XX
#define NCV_SIM                     0x1
#define	TEST_ID_MAX					100  
#if (NCV_SIM == 0)
#define UART_BAUD_RATE1           115200
#else
#define UART_BAUD_RATE1           115200
#endif

#define UART_CLK_FREQ            26
int print_str(char * st)
{
  while (*st) {
		       addUART0_Reg0x3 = *st;
	           st++;
	          }
  return 1;
}
void UartDbgInit()
{
	unsigned int     uart_clk_div;
	  
   // clrf_SYS_Reg0x3_uart0_pwd ;   //open periph
	//*((volatile unsigned long *) (0x44010000+0xc*4)) =  0x4;
	setf_SYSTEM_Reg0xc_uart0_cken ; //uart0 enable


	//*((volatile unsigned long *) (0x44000400+10*4))  = 0x40 ; //second_func
    //*((volatile unsigned long *) (0x44000400+11*4))  = 0x40 ; //second_func
    addAON_GPIO_Reg0xa = 0x40 ;  //second_func
    addAON_GPIO_Reg0xb = 0x40 ;  //second_func


    uart_clk_div = (UART_CLK_FREQ*1000000)/UART_BAUD_RATE1 - 1;




    addUART0_Reg0x0 = (uart_clk_div << posUART0_Reg0x0_UART_CLK_DIVID) |
                      (0x0          << posUART0_Reg0x0_UART_STOP_LEN ) |
					  #if (NCV_SIM == 0)
                      (0x0          << posUART0_Reg0x0_UART_PAR_MODE ) |
                      (0x0          << posUART0_Reg0x0_UART_PAR_EN   ) |
					  #else
					            (0x0          << posUART0_Reg0x0_UART_PAR_MODE ) |
                      (0x0          << posUART0_Reg0x0_UART_PAR_EN   ) |
					  #endif
                      (0x3          << posUART0_Reg0x0_UART_LEN      ) |
                      (0x0          << posUART0_Reg0x0_UART_IRDA     ) |
                      (0x1          << posUART0_Reg0x0_UART_RX_ENABLE) |
                      (0x1          << posUART0_Reg0x0_UART_TX_ENABLE) ;

    addUART0_Reg0x1 = 0x00004010;
    addUART0_Reg0x4 = 0x42;
    addUART0_Reg0x6 = 0x0;
    addUART0_Reg0x7 = 0x0;
  
	//  setf_SYS_Reg0x10_int_uart0_en; //enable uart_int irq
   // *((volatile unsigned long *) (0x44010000+0x20*4)) =  0x10;  //enable uart_int
    addSYSTEM_Reg0x20 = 0x10 ;  //enable uart_int
		
}
#endif

int components_init(void)
{
/*for bringup test*/
#if CONFIG_SOC_BK7256XX || CONFIG_SOC_BK7256_CP1
    driver_init();
    pm_init_todo();
    show_init_info();
    random_init();

	ipc_init();

#else
	pm_init_todo();
	driver_init();
	reset_reason_init();
	random_init();
	wdt_init();
	show_init_info();
#endif
	return BK_OK;
}

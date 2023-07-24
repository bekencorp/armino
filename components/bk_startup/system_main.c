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
#include <sdkconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <common/bk_include.h>
#include "bk_private/components_init.h"
#include "rtos_init.h"
#include <os/os.h>
#include "sys_driver.h"
#if (!CONFIG_SLAVE_CORE)
#include <modules/pm.h>
#endif
#include <driver/flash_partition.h>

#include "boot.h"

#if CONFIG_FREERTOS_TRACE
#include "trcRecorder.h"
#endif

#include "stack_base.h"

#if (!CONFIG_SLAVE_CORE)

static beken_thread_function_t s_user_app_entry = NULL;
beken_semaphore_t user_app_sema = NULL;

void rtos_set_user_app_entry(beken_thread_function_t entry)
{
	s_user_app_entry = entry;
}

void rtos_user_app_preinit(void)
{
    int ret = rtos_init_semaphore(&user_app_sema, 1);
	if(ret < 0){
		os_printf("init queue failed");
	}
}

void rtos_user_app_launch_over(void)
{
	int ret;
	ret = rtos_set_semaphore(&user_app_sema);

	if(ret < 0){
		os_printf("set sema failed");
	}
}

void rtos_user_app_waiting_for_launch(void)
{
	int ret;

	ret = rtos_get_semaphore(&user_app_sema, BEKEN_WAIT_FOREVER);

	if(ret < 0){
		os_printf("get sema failed");
	}

#if CONFIG_SAVE_BOOT_TIME_POINT
	save_mtime_point(CPU_APP_ENTRY_TIME);
#endif
}
#endif

//#define RTOS_FUNC_TEST
#ifdef RTOS_FUNC_TEST
/*rtos thread func test, for bk7256 bringup.*/
#include "FreeRTOS.h"
#include "queue.h"

beken_queue_t test_thread_msg_que = NULL;
beken_semaphore_t test_thread_sema = NULL;


beken_thread_t test_sned_thread_handle = NULL;
beken_thread_t test_rec_thread_handle = NULL;

extern void vTaskDelay();
static void test_send_thread_func(void* parameter)
{
	int msg = 1;
	int ret = 0;

	while(1)
	{
		//write msg to queue
		ret = rtos_push_to_queue(&test_thread_msg_que, &msg, 0xffffffff );
		if(ret < 0){
			os_printf("test thread send msg failed\r\n");
			while(1);
		}
		os_printf("test_send_thread_func send msg %d\r\n", msg);

		vTaskDelay(1000);
		msg++;
		//give binary semaphore
		if(msg <10){
			ret = rtos_set_semaphore(&test_thread_sema);
			if(ret < 0){
				os_printf("get sema failed");
			}
		}
	}
}

static void test_rec_thread_func(void* parameter)
{
	int msg = 0;
	int ret = 0;

	while(1)
	{
		//get semaphore
		ret = rtos_get_semaphore(&test_thread_sema, BEKEN_WAIT_FOREVER);
		if(ret < 0){
			os_printf("get sema failed");
		}

		//read msg from queue
		ret = rtos_pop_from_queue(&test_thread_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if(ret < 0){
			os_printf("get smg failed");
		}
		os_printf("test_rec_thread_func rec msg %d\r\n", msg);
	}
}

void rtos_thread_func_test()
{
	int ret = 0;
	//init queue,depth 4, size 4 Bytes
	ret = rtos_init_queue(&test_thread_msg_que,
							  "thread_test_queue",
							  4,
							  4);
	if(ret < 0){
		os_printf("init queue failed");
	}

	//init binary semaphore
	ret = rtos_init_semaphore(&test_thread_sema, 1);
	if(ret < 0){
		os_printf("init sema failed");
	}

	// create send thread
	ret = rtos_create_thread(test_sned_thread_handle, 4,
		"test_send_thread",
		(beken_thread_function_t)test_send_thread_func,
		512,
		(beken_thread_arg_t)0);
	if(ret < 0){
		os_printf("error:@thread send creat failed @");
	}

	// create rec thread
	ret =rtos_create_thread(test_rec_thread_handle, 4,
		"test_rec_thread",
		(beken_thread_function_t)test_rec_thread_func,
		512,
		(beken_thread_arg_t)0);

	if(ret < 0){
		os_printf("error:@thread rec creat failed @");
	}
	os_printf("success :@thread rec creat\r\n @");
	//if necessary, close the main().
}
#endif

#if (CONFIG_MASTER_CORE)
extern void mon_reset_cpu1(u32 enable, u32 start_addr);

static uint32 get_partition_addr(uint32 slave_core_id)
{
	(void)slave_core_id;

	bk_logic_partition_t *pt;
	bk_partition_t   part_id;

	uint32    addr = -1;

	//if(slave_core_id == 1)
	{
		part_id = BK_PARTITION_APPLICATION1;
	}

	pt = bk_flash_partition_get_info(part_id);

	if((pt != NULL) && ((pt->partition_start_addr % 34) == 0))
	{
		addr = (pt->partition_start_addr / 34) * 32;   // CRC16 appended every 32 bytes in flash.  (32 bytes -> 34 bytes).
	}
	else
	{
		os_printf("slave core start addr not valid.\r\n");
	}

#if (!CONFIG_SOC_BK7256XX)
#if (CONFIG_SLAVE_CORE_OFFSET)
	return CONFIG_SLAVE_CORE_OFFSET;
#endif
#endif

	return addr;
}

static void reset_slave_core(uint32 offset, uint32_t reset_flag)
{
	uint32    start_flag = 0;

	if(reset_flag == 0)
		start_flag = 1;

	os_printf("reset_slave_core at: %08x, start=%d\r\n", offset, start_flag);

	#if CONFIG_SOC_BK7256XX //u-mode
	mon_reset_cpu1(start_flag, offset);
	#else
	sys_drv_set_cpu1_boot_address_offset(offset >> 8);
	sys_drv_set_cpu1_reset(start_flag);
	#endif
}

void start_slave_core(void)
{
	uint32  addr = get_partition_addr(1);
	reset_slave_core(addr, 0);
}

void stop_slave_core(void)
{
	reset_slave_core(0, 1);
}

#endif

#if (!CONFIG_SLAVE_CORE)
static void user_app_thread( void *arg )
{
	rtos_user_app_waiting_for_launch();
	/* add your user_main*/
	os_printf("user app entry(0x%0x)\r\n", s_user_app_entry);
	if(NULL != s_user_app_entry) {
		s_user_app_entry(0);
	}

#if CONFIG_SAVE_BOOT_TIME_POINT
	save_mtime_point(CPU_APP_FINISH_TIME);
#endif

	rtos_deinit_semaphore(&user_app_sema);

	rtos_delete_thread( NULL );
}

static void start_user_app_thread(void)
{
	os_printf("start user app thread.\r\n");
	rtos_create_thread(NULL,
					BEKEN_APPLICATION_PRIORITY,
					"app",
					(beken_thread_function_t)user_app_thread,
					CONFIG_APP_MAIN_TASK_STACK_SIZE,
					(beken_thread_arg_t)0);
}
#endif

extern int main(void);
extern void ChipTest(void);
extern bool ate_is_enabled(void);
static void app_main_thread(void *arg)
{
#if CONFIG_SAVE_BOOT_TIME_POINT
	save_mtime_point(CPU_MAIN_ENTRY_TIME);
#endif

#if (!CONFIG_SLAVE_CORE)
	rtos_user_app_preinit();
#endif

#ifdef RTOS_FUNC_TEST
	/*rtos thread func test, for bk7256 bringup.*/
	rtos_thread_func_test();
	//if nessary ,close the main() function.
#endif
	main();

#if (CONFIG_MASTER_CORE)
	//bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_ON);
	//start_slave_core();
#endif

#if CONFIG_MATTER_START
    beken_thread_t matter_thread_handle = NULL;

    os_printf("start matter\r\n");
    rtos_create_thread(&matter_thread_handle,
		BEKEN_DEFAULT_WORKER_PRIORITY,
		 "matter",
		(beken_thread_function_t)ChipTest,
		8192,
		0);
#endif
    if(ate_is_enabled())
    {
        os_printf("ATE enabled = 1\r\n");
    }

// #if (!CONFIG_SLAVE_CORE)
// 	 rtos_user_app_launch_over();
// #endif

#if CONFIG_SAVE_BOOT_TIME_POINT
	save_mtime_point(CPU_MIAN_FINISH_TIME);
#endif

	rtos_delete_thread(NULL);
}

void start_app_main_thread(void)
{
	rtos_create_thread(NULL, CONFIG_APP_MAIN_TASK_PRIO,
		"main",
		(beken_thread_function_t)app_main_thread,
		CONFIG_APP_MAIN_TASK_STACK_SIZE,
		(beken_thread_arg_t)0);
}

void entry_main(void)
{
#if CONFIG_SAVE_BOOT_TIME_POINT
	save_mtime_point(CPU_MAIN_ENTRY_TIME);
#endif

	rtos_init();

#if (CONFIG_ATE_TEST)
	bk_set_printf_enable(0);
#endif

	if(components_init())
		return;

#if (CONFIG_FREERTOS_TRACE)
	xTraceEnable(TRC_START);
	uint32_t trace_addr = (uint32_t)xTraceGetTraceBuffer();
	uint32_t trace_size = uiTraceGetTraceBufferSize();

	rtos_regist_plat_dump_hook(trace_addr, trace_size);
#endif


#if CONFIG_SAVE_BOOT_TIME_POINT
	save_mtime_point(CPU_INIT_DRIVER_TIME);
#endif

	start_app_main_thread();
#if (!CONFIG_SLAVE_CORE)
	start_user_app_thread();
#endif

#if (!CONFIG_SLAVE_CORE) && (CONFIG_FREERTOS_V10)
	extern void rtos_init_base_time(void);
	rtos_init_base_time();
#endif

#if CONFIG_SAVE_BOOT_TIME_POINT
	save_mtime_point(CPU_START_SCHE_TIME);
#endif



	rtos_start_scheduler();
}

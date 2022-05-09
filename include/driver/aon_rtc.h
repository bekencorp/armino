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

#pragma once

#include <driver/aon_rtc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	  Init the AON_RTC driver
 *
 * This API init the resoure common:
 *	 - Init AON_RTC driver control memory
 *	 - Register ISR to NVIC/INTC/PLIC...
 *
 * @attention 1. This API should be called before any other AON_RTC APIs.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_aon_rtc_driver_init(void);

/**
 * @brief	  Denit the AON_RTC driver
 *
 * This API deinit the resoure common:
 *	 - Deinit AON_RTC driver control memory
 *	 - Deinit AON RTC HW:Disable AON RTC, unregister isr handler
 *
 * @attention 1. This API should be called after any other AON_RTC APIs.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_aon_rtc_driver_deinit(void);

/**
 * @brief	  Create a request to use the selected AON RTC timer.
 *			  For period timer, every tick will come an isr,
 *			  the caller should call bk_aon_rtc_destroy if it won't use it.
 *			  For once timer, only come one isr, after the isr finish,
 *			  the timer will be destoried by driver layer, the caller no needs to destroy it.
 *
 * This API init the resoure common:
 *	 - Init AON_RTC driver control memory
 *	 - Enable the AON RTC HW, and set the AON RTC counter to run.
 *
 * @param id 	which AON RTC will be selected.
 * @param tick  The tick value for the AON RTC timer, it will be set to AON RTC.
 * @param isr	The counter runs to setted value(tick), will call this isr
 * @param p_isr_param	Interaction with APP and Driver layer
 * @param period	Once timer or period timer
 *
 * @attention 1. This API should be called after bk_aon_rtc_driver_init.
*				 Driver layer will call bk_aon_rtc_driver_init,
 *				 but can't guarantee APP call this function after driver inited.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_aon_rtc_create(aon_rtc_id_t id, uint32_t tick, bool period);

/**
 * @brief	  Dstory the requested AON RTC period timer which is created by the caller.
 *
 * This API destory the resoure common:
 *	 - Clear AON_RTC driver control memory.
 *	 - Disable the AON RTC HW, and set the AON RTC HW to default status.
 *
 * @attention 1. This API should be called after bk_aon_rtc_driver_init and bk_aon_rtc_create.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_aon_rtc_destroy(aon_rtc_id_t id);

/**
 * @brief     Register the tick interrupt service routine for AON_RTC id
 *
 * @param id aon_rtc id
 * @param isr AON_RTC tick callback
 * @param param AON_RTC tick callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_rtc_register_tick_isr(aon_rtc_id_t id, aon_rtc_isr_t isr, void *param);

/**
 * @brief     Register the upper interrupt service routine for AON_RTC id
 *
 * @param id aon_rtc id
 * @param isr AON_RTC upper callback
 * @param param AON_RTC upper callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_rtc_register_upper_isr(aon_rtc_id_t id, aon_rtc_isr_t isr, void *param);

/**
 * @brief     Register the rtc tick init
 *
 * @param param none
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_rtc_tick_init();
/**
 * @brief     Register rtc wakeup
 * @param    period: wake up timer
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aon_rtc_open_rtc_wakeup(uint32_t period);
/**
 * @brief     get_current_tick
 * @param     null
 *
 * @return
 *   current tick
 *    
 */
uint32_t bk_aon_rtc_get_current_tick();

/**
 * @}
 */

#ifdef __cplusplus
}
#endif




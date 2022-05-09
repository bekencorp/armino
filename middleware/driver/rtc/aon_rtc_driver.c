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

#include <common/bk_include.h>
#include <os/mem.h>
#include <driver/aon_rtc.h>
#include "aon_rtc_driver.h"
#include "aon_rtc_hal.h"

#include <driver/int.h>
#include "sys_driver.h"

#define AON_RTC_UNIT_NUM (AON_RTC_HAL_UNIT_NUM)

typedef struct {
	//aon_rtc_id_t id;	//no needs it until now:the id is matched from APP,DRIVER,HAL,HW/SOC layer.
	bool inited;
	uint8_t using_cnt;

	aon_rtc_hal_t hal;

	//Record APP param's
	bool period;
	uint32_t tick;
} aon_rtc_driver_t;

typedef struct {
	aon_rtc_isr_t callback;
	void *isr_param;
} aon_rtc_callback_t;

static aon_rtc_driver_t s_aon_rtc[AON_RTC_UNIT_NUM] = {0};
static aon_rtc_callback_t s_aon_rtc_tick_isr[AON_RTC_UNIT_NUM] = {NULL};
static aon_rtc_callback_t s_aon_rtc_upper_isr[AON_RTC_UNIT_NUM] = {NULL};

bk_err_t bk_aon_rtc_register_tick_isr(aon_rtc_id_t id, aon_rtc_isr_t isr, void *param)
{
	//AON_RTC_RETURN_ON_INVALID_ID(id);
	uint32_t int_level = rtos_disable_int();
	s_aon_rtc_tick_isr[id].callback = isr;
	s_aon_rtc_tick_isr[id].isr_param = param;
	rtos_enable_int(int_level);
	return BK_OK;
}

bk_err_t bk_aon_rtc_register_upper_isr(aon_rtc_id_t id, aon_rtc_isr_t isr, void *param)
{
	//AON_RTC_RETURN_ON_INVALID_ID(id);
	uint32_t int_level = rtos_disable_int();
	s_aon_rtc_upper_isr[id].callback = isr;
	s_aon_rtc_upper_isr[id].isr_param = param;
	rtos_enable_int(int_level);
	return BK_OK;
}

static bk_err_t aon_rtc_isr_handler(aon_rtc_id_t id)
{
	//uses tick as one time timer
	if(aon_rtc_hal_get_tick_int_status(&s_aon_rtc[id].hal))
	{
		if (s_aon_rtc_tick_isr[id].callback) {
			s_aon_rtc_tick_isr[id].callback(id, s_aon_rtc_tick_isr[id].isr_param);
		}
		aon_rtc_hal_clear_tick_int_status(&s_aon_rtc[id].hal);

		bk_aon_rtc_destroy(id);
	}

	//uses upper timer as period timer
	if(aon_rtc_hal_get_upper_int_status(&s_aon_rtc[id].hal))
	{
		if (s_aon_rtc_upper_isr[id].callback) {
			s_aon_rtc_upper_isr[id].callback(id, s_aon_rtc_upper_isr[id].isr_param);
		}

		aon_rtc_hal_clear_upper_int_status(&s_aon_rtc[id].hal);
	}

	//TODO: clear NVIC/INTC/PLIC int pending status

	return BK_OK;
}

static void aon_rtc1_isr_handler(void)
{
	aon_rtc_isr_handler(AON_RTC_ID_1);
}

#if (SOC_AON_RTC_UNIT_NUM > 1)
static void aon_rtc2_isr_handler(void)
{
	aon_rtc_isr_handler(AON_RTC_ID_2);
}
#endif

#if (CONFIG_SYSTEM_CTRL)
static void aon_rtc_interrupt_enable(aon_rtc_id_t id)
{
	switch(id)
	{
		case AON_RTC_ID_1:
			sys_drv_int_group2_enable(RTC_INTERRUPT_CTRL_BIT);
			break;
#if (SOC_AON_RTC_UNIT_NUM > 1)
		case AON_RTC_ID_2:
			sys_drv_int_group2_enable(RTC1_INTERRUPT_CTRL_BIT);
			break;
#endif
		default:
			break;
	}
}

static void aon_rtc_interrupt_disable(aon_rtc_id_t id)
{
	switch(id)
	{
		case AON_RTC_ID_1:
			sys_drv_int_group2_disable(RTC_INTERRUPT_CTRL_BIT);
			break;
#if (SOC_AON_RTC_UNIT_NUM > 1)
		case AON_RTC_ID_2:
			sys_drv_int_group2_disable(RTC1_INTERRUPT_CTRL_BIT);
			break;
#endif
		default:
			break;
	}
}
#endif

bk_err_t bk_aon_rtc_driver_init(void)
{
	aon_rtc_int_config_t int_config_table[] = AON_RTC_INT_CONFIG_TABLE;

	os_memset(&s_aon_rtc, 0, sizeof(s_aon_rtc));
	os_memset(&s_aon_rtc_tick_isr, 0, sizeof(s_aon_rtc_tick_isr));
	os_memset(&s_aon_rtc_upper_isr, 0, sizeof(s_aon_rtc_upper_isr));

	//TOTO: Enter critical protect
	for (int id = AON_RTC_ID_1; id < AON_RTC_ID_MAX; id++) {
		aon_rtc_int_config_t *cur_int_cfg = &int_config_table[id];
		bk_int_isr_register(cur_int_cfg->int_src, cur_int_cfg->isr, NULL);

		aon_rtc_interrupt_enable(id);
		s_aon_rtc[id].inited = true;
	}

	//TOTO: exit critical protect
	return BK_OK;
}

bk_err_t bk_aon_rtc_driver_deinit(void)
{
	aon_rtc_int_config_t int_cfg_table[] = AON_RTC_INT_CONFIG_TABLE;
	for (int id = AON_RTC_ID_1; id < AON_RTC_ID_MAX; id++) {
		bk_int_isr_unregister(int_cfg_table[id].int_src);

		aon_rtc_interrupt_disable(id);
		s_aon_rtc[id].inited = false;
	}

	return BK_OK;
}

extern void delay_ms(UINT32 ms_count);

bk_err_t bk_aon_rtc_create(aon_rtc_id_t id, uint32_t tick, bool period)
{
	//Avoid APP call this function before driver has done bk_aon_rtc_driver_init
	if(s_aon_rtc[id].inited == false)
	{
		//TODO: logs: call aon_rtc_init first.
		return BK_ERR_NOT_INIT;
	}

	if(s_aon_rtc[id].using_cnt)
	{
		//TODO: logs: call bk_aon_rtc_destroy first.
		return BK_ERR_BUSY;
	}

	//TOTO: Enter critical protect

	s_aon_rtc[id].using_cnt++;
	s_aon_rtc[id].tick = tick;
	s_aon_rtc[id].period = period;

	//init HW
	s_aon_rtc[id].hal.id = id;
	aon_rtc_hal_init(&s_aon_rtc[id].hal);

	if(period)	//use upper value int
	{
		aon_rtc_hal_set_upper_val(&s_aon_rtc[id].hal, tick);
		aon_rtc_hal_enable_upper_int(&s_aon_rtc[id].hal);
	}
	else
	{
		//confirm tick val <= upper value, or tick int never be entry.
		aon_rtc_hal_set_upper_val(&s_aon_rtc[id].hal, AON_RTC_UPPER_VAL_MAX);

		aon_rtc_hal_set_tick_val(&s_aon_rtc[id].hal, tick);
		aon_rtc_hal_enable_tick_int(&s_aon_rtc[id].hal);
	}

	//start to run
	aon_rtc_hal_start_counter(&s_aon_rtc[id].hal);
	aon_rtc_hal_reset_counter(&s_aon_rtc[id].hal);	//maybe the start value isn't 0
	delay_ms(1);
	aon_rtc_hal_clear_reset_counter(&s_aon_rtc[id].hal);

	//TOTO: Exit critical protect

	return BK_OK;
}

bk_err_t bk_aon_rtc_destroy(aon_rtc_id_t id)
{
	//TOTO: Enter critical protect

	if(s_aon_rtc[id].inited == false)
	{
		//TODO: logs: call aon_rtc_init first.
		//TOTO: Exit critical protect
		return BK_ERR_NOT_INIT;
	}

	if(s_aon_rtc[id].using_cnt == 0)
	{
		//TODO: logs: call bk_aon_rtc_create first.
		//TOTO: Exit critical protect
		return BK_ERR_NOT_INIT;
	}

	//stop HW before SW change value, avoid ISR status was update to INTC/NVIC/PLIC...
	//but doesn't response ISR, after HW deinit, the ISR comes caused error.
	aon_rtc_hal_deinit(&s_aon_rtc[id].hal);

	s_aon_rtc[id].using_cnt = 0;
	s_aon_rtc[id].tick = 0;
	s_aon_rtc[id].period = false;

	//TOTO: Exit critical protect

	return BK_OK;
}
bk_err_t bk_aon_rtc_tick_init()
{
    aon_rtc_hal_tick_init();
	return BK_OK;
}
bk_err_t bk_aon_rtc_open_rtc_wakeup(uint32_t period)
{
    aon_rtc_hal_open_rtc_wakeup(period);
	return BK_OK;
}
uint32_t bk_aon_rtc_get_current_tick()
{
    return aon_rtc_hal_get_current_tick();
}

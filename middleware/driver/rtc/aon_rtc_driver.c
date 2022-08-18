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
#include "string.h"

#include <driver/int.h>
#include "sys_driver.h"

#include <driver/aon_rtc.h>
#include "aon_rtc_hal.h"
#include "aon_rtc_driver.h"

#define AON_RTC_UNIT_NUM (AON_RTC_HAL_UNIT_NUM)
#define AON_RTC_NAME ((uint8_t *)"aon_rtc")

typedef struct {
	//aon_rtc_id_t id;	//no needs it until now:the id is matched from APP,DRIVER,HAL,HW/SOC layer.
	bool inited;
	//uint8_t using_cnt;	//remove it as only one HW can't for many APPs

	aon_rtc_hal_t hal;

	//Record APP param's
	//bool period;
	//uint32_t tick;

	//Alarm list
	alarm_node_t *alarm_head_p;
	uint32_t alarm_node_cnt;
} aon_rtc_driver_t;

typedef struct {
	aon_rtc_isr_t callback;
	void *isr_param;
} aon_rtc_callback_t;

typedef struct {
	alarm_node_t nodes[AON_RTC_MAX_ALARM_CNT];
	uint64_t busy_bits;
} aon_rtc_nodes_memory_t;

static aon_rtc_driver_t s_aon_rtc[AON_RTC_UNIT_NUM] = {0};
static aon_rtc_callback_t s_aon_rtc_tick_isr[AON_RTC_UNIT_NUM] = {NULL};
static aon_rtc_callback_t s_aon_rtc_upper_isr[AON_RTC_UNIT_NUM] = {NULL};
static aon_rtc_nodes_memory_t *s_aon_rtc_nodes_p[AON_RTC_UNIT_NUM];
static uint64_t s_high_tick[AON_RTC_UNIT_NUM];

static void alarm_dump_node(alarm_node_t *node_p)
{
#if AON_RTC_DEBUG
	AON_RTC_LOGD("%s[+]\r\n", __func__);

	AON_RTC_LOGD("node_p=0x%x\r\n", node_p);
	if(node_p)
	{		
		AON_RTC_LOGD("next=0x%x\r\n", node_p->next);
		AON_RTC_LOGD("name=%s\r\n", node_p->name);
		AON_RTC_LOGD("period_tick=0x%x\r\n", node_p->period_tick);
		AON_RTC_LOGD("period_cnt=%d\r\n", node_p->period_cnt);
		AON_RTC_LOGD("start_tick=0x%x\r\n", node_p->start_tick);
		AON_RTC_LOGD("expired_tick=0x%x\r\n", node_p->expired_tick);
	}

	AON_RTC_LOGD("%s[-]\r\n", __func__);
#endif
}

static void alarm_dump_list(alarm_node_t *head_p)
{
#if AON_RTC_DEBUG
	alarm_node_t *cur_p = head_p;
	uint32_t count = 0;
	uint32_t int_level = 0;

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	int_level = rtos_disable_int();
	while(cur_p)
	{
		alarm_dump_node(cur_p);
		count++;

		cur_p = cur_p->next;
	}
	rtos_enable_int(int_level);

	AON_RTC_LOGD("node cnt=%d\r\n", count);

	AON_RTC_LOGD("%s[-]\r\n", __func__);
#endif
}

static alarm_node_t* aon_rtc_request_node(aon_rtc_id_t id)
{
	uint32_t i = 0; 

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	for(i = 0; i < AON_RTC_MAX_ALARM_CNT; i++)
	{
		if((s_aon_rtc_nodes_p[id]->busy_bits & (0x1<<i)) == 0)
		{
			AON_RTC_LOGD("%s[-]:node[%d]=0x%x\r\n", __func__, i, &s_aon_rtc_nodes_p[id]->nodes[i]);
			s_aon_rtc_nodes_p[id]->busy_bits |= (0x1<<i);
			return &s_aon_rtc_nodes_p[id]->nodes[i];
		}
	}

	return NULL;
}

static void aon_rtc_release_node(aon_rtc_id_t id, alarm_node_t *node_p)
{
	uint32_t i = 0; 

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	for(i = 0; i < AON_RTC_MAX_ALARM_CNT; i++)
	{
		if(&s_aon_rtc_nodes_p[id]->nodes[i] == node_p)
		{
			s_aon_rtc_nodes_p[id]->busy_bits &= ~(0x1<<i);
			os_memset(&s_aon_rtc_nodes_p[id]->nodes[i], 0, sizeof(alarm_info_t));
			AON_RTC_LOGD("%s[-]:node[%d]=0x%x\r\n", __func__, i, &s_aon_rtc_nodes_p[id]->nodes[i]);
			break;
		}
	}

	BK_ASSERT(i < AON_RTC_MAX_ALARM_CNT);
}

static int32_t alarm_insert_node(aon_rtc_id_t id, alarm_node_t *node_p)
{
	alarm_node_t *cur_p = NULL;
	alarm_node_t *next_p = NULL;
	uint32_t int_level = 0;

	AON_RTC_LOGD("%s[+]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);

	alarm_dump_list(s_aon_rtc[id].alarm_head_p);

	int_level = rtos_disable_int();

	//check whether the same name
	cur_p = s_aon_rtc[id].alarm_head_p;
	while(cur_p)
	{
		if(strncmp((const char *)cur_p->name, (const char *)node_p->name, ALARM_NAME_MAX_LEN) == 0)
		{
			AON_RTC_LOGE("name=%s has registered\r\n", node_p->name);
			rtos_enable_int(int_level);
			return -1;
		}

		cur_p = cur_p->next;
	}

	//search the node position
	cur_p = s_aon_rtc[id].alarm_head_p;

	//no node
	if(cur_p == NULL)
	{
		s_aon_rtc[id].alarm_head_p = node_p;
		s_aon_rtc[id].alarm_node_cnt++;
		AON_RTC_LOGD("insert first node 0x%x,name=%s\r\n", node_p, node_p->name);
		
		rtos_enable_int(int_level);
		return 0;
	}

	//only one node
	next_p = cur_p->next;
	if(next_p == NULL)
	{
		if(cur_p->expired_tick <= node_p->expired_tick)
			cur_p->next = node_p;
		else
		{
			node_p->next = cur_p;
			s_aon_rtc[id].alarm_head_p = node_p;
		}
		s_aon_rtc[id].alarm_node_cnt++;
		rtos_enable_int(int_level);

		//TODO:log debug
		AON_RTC_LOGD("list total has two nodes\r\n");

		return 0;
	}

	//more then 2 nodes
	while(next_p)
	{
		if(cur_p->expired_tick <= node_p->expired_tick)	//move after cur_p
		{
			if(next_p->expired_tick <= node_p->expired_tick)	//search next
			{
				cur_p = next_p;
				next_p = next_p->next;
				continue;
			}
			else	//insert
			{
				node_p->next = next_p;
				cur_p->next = node_p;
				s_aon_rtc[id].alarm_node_cnt++;
				rtos_enable_int(int_level);
				return 0;
			}
		}
		else	//insert before cur_p, means the first node, head
		{
			node_p->next = cur_p;
			s_aon_rtc[id].alarm_head_p = node_p;
			s_aon_rtc[id].alarm_node_cnt++;
			rtos_enable_int(int_level);
			return 0;
		}
	}

	//the last one
	cur_p->next = node_p;
	s_aon_rtc[id].alarm_node_cnt++;
	rtos_enable_int(int_level);

	//dump list info
	alarm_dump_list(s_aon_rtc[id].alarm_head_p);

	AON_RTC_LOGD("%s[-]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);

	return 0;
}

static alarm_node_t *alarm_remove_node(aon_rtc_id_t id, uint8_t *name_p)
{
	alarm_node_t *cur_p = NULL;
	alarm_node_t *previous_p = NULL;
	alarm_node_t *remove_node_p = NULL;
	uint32_t int_level = 0;
	uint32_t node_cnt = 0;

	AON_RTC_LOGD("%s[+]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);

	int_level = rtos_disable_int();
	//
	previous_p = cur_p = s_aon_rtc[id].alarm_head_p;
	while(cur_p)
	{
		//double check pointer is valid
		node_cnt++;
		BK_ASSERT(node_cnt <= AON_RTC_MAX_ALARM_CNT);

		if(strncmp((const char *)cur_p->name, (const char *)name_p, ALARM_NAME_MAX_LEN) == 0)
		{
			//first one
			if(previous_p == cur_p)
			{
				remove_node_p = s_aon_rtc[id].alarm_head_p;
				s_aon_rtc[id].alarm_head_p = cur_p->next;
				s_aon_rtc[id].alarm_node_cnt--;

				AON_RTC_LOGD("free=0x%x,name=%s\r\n", cur_p, cur_p->name);
				aon_rtc_release_node(id, cur_p);
				break;
			}
			else
			{
				remove_node_p = cur_p;
				previous_p->next = cur_p->next;
				s_aon_rtc[id].alarm_node_cnt--;
				AON_RTC_LOGD("free=0x%x,name=%s\r\n", cur_p, cur_p->name);
				aon_rtc_release_node(id, cur_p);
				break;
			}
		}

		previous_p = cur_p;
		cur_p = cur_p->next;
	}

	rtos_enable_int(int_level);

	if(remove_node_p == NULL)
	{
		AON_RTC_LOGE("%s:can't find %s alarm\r\n", __func__, name_p);
	}

	//dump list info
	alarm_dump_list(s_aon_rtc[id].alarm_head_p);

	AON_RTC_LOGD("%s[-]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);

	return remove_node_p;
}

static void alarm_update_expeired_nodes(aon_rtc_id_t id)
{
	alarm_node_t *cur_p = NULL;
	alarm_node_t *next_p = NULL;
	uint32_t node_cnt = 0;
	uint64_t cur_tick = 0;
	uint32_t int_level = 0;

	AON_RTC_LOGD("%s[+]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);
	
	alarm_dump_list(s_aon_rtc[id].alarm_head_p);

	int_level = rtos_disable_int();

	//search the node position
	cur_p = s_aon_rtc[id].alarm_head_p;
	while(cur_p)
	{
		alarm_dump_node(cur_p);
		alarm_dump_node(cur_p->next);

		//double check pointer is valid
		node_cnt++;
		BK_ASSERT(node_cnt <= AON_RTC_MAX_ALARM_CNT);

		next_p = cur_p->next;

		cur_tick = bk_aon_rtc_get_current_tick(id);
		//maybe callback runs too much time,so assume the time in AON_RTC_MS_TICK_CNT means has expired
		if(cur_p->expired_tick <= cur_tick + AON_RTC_PRECISION_TICK)
		{
			if(cur_p->callback)
			{
				cur_p->callback(id, cur_p->name, cur_p->cb_param_p);
			}

			//last time alarm
			if(cur_p->period_cnt == 1)
			{
				cur_p->period_cnt = 0;
				s_aon_rtc[id].alarm_head_p = cur_p->next;	//head move to next
				s_aon_rtc[id].alarm_node_cnt--;
/* 
 * WARNING:As freertos doesn't support free memory in ISR context.
 * The chip no needs to use a task for AON RTC which wastes some memory.
 * so the APPLIACTION who calls bk_alarm_register should release the memory
 * returns by bk_alarm_register.
 */
#if 0
				AON_RTC_LOGD("last alarm:free=0x%x,name=%s\r\n", cur_p, cur_p->name);
				os_free(cur_p);
#endif
				aon_rtc_release_node(id, cur_p);
			}
			//loop timer
			else 
			{
				if(cur_p->period_cnt != ALARM_LOOP_FOREVER)
				{
					cur_p->period_cnt--;
					AON_RTC_LOGD("%s left %d times \r\n", cur_p->name, cur_p->period_cnt);
				}

				//has next
				if(next_p)	//move to switable position
				{
					s_aon_rtc[id].alarm_head_p = cur_p->next;	//head move to next
					cur_p->expired_tick += cur_p->period_tick;
					cur_p->next = NULL;		//cur_p is removed
					s_aon_rtc[id].alarm_node_cnt--; //it will ++ in alarm_insert_node
					if(alarm_insert_node(id, cur_p) != 0)
					{
						AON_RTC_LOGE("alarm name=%s insert fail\r\n", cur_p->name);
						rtos_enable_int(int_level);
						return;
					}
				}
				else	//only self
				{
					//just update self expired time
					cur_p->expired_tick += cur_p->period_tick;
					AON_RTC_LOGD("%s update next expired time %d \r\n", cur_p->name, cur_p->expired_tick);
				}
			}
		}
		else	//no expired
		{
			break;
		}

		cur_p = next_p;	//TODO:maybe cur_p offset is too small and calback excutes too much time, here can't switch to next NODE.

		alarm_dump_list(s_aon_rtc[id].alarm_head_p);
	}

	rtos_enable_int(int_level);

	alarm_dump_list(s_aon_rtc[id].alarm_head_p);

	AON_RTC_LOGD("%s[-]cnt=%d\r\n", __func__, s_aon_rtc[id].alarm_node_cnt);
}

bk_err_t bk_aon_rtc_register_tick_isr(aon_rtc_id_t id, aon_rtc_isr_t isr, void *param)
{
	//AON_RTC_RETURN_ON_INVALID_ID(id);
	uint32_t int_level = rtos_disable_int();
	s_aon_rtc_tick_isr[id].callback = isr;
	s_aon_rtc_tick_isr[id].isr_param = param;
	rtos_enable_int(int_level);
	return BK_OK;
}

/*
 * aon rtc set tick uses 3 cycles of 32k in ASIC,
 * cpu should check whether set tick success.
 * If twice set tick in 3/32 ms, the second time set tick value will be failed.
 */
static void aon_rtc_set_tick(aon_rtc_hal_t *hal, uint32_t val)
{
	volatile int i = 0;

	aon_rtc_hal_set_tick_val(hal, val);
	while(aon_rtc_hal_get_tick_val_lpo(hal) != val)
	{
		i++;
		if(i > 768)	//32k, 3ticks == 3/32 ms:++is 30 cycles
		{
			AON_RTC_LOGE("%s:set tick timeout\r\n", __func__);
			break;
		}
	}
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

#if 0
static bk_err_t aon_rtc_isr_handler(aon_rtc_id_t id)
{
	//uses tick as one time timer
	if(aon_rtc_hal_get_tick_int_status(&s_aon_rtc[id].hal))
	{
		if (s_aon_rtc_tick_isr[id].callback) {
			s_aon_rtc_tick_isr[id].callback(id, AON_RTC_NAME, s_aon_rtc_tick_isr[id].isr_param);
		}
		aon_rtc_hal_clear_tick_int_status(&s_aon_rtc[id].hal);

		bk_aon_rtc_destroy(id);
	}

	//uses upper timer as period timer
	if(aon_rtc_hal_get_upper_int_status(&s_aon_rtc[id].hal))
	{
		if (s_aon_rtc_upper_isr[id].callback) {
			s_aon_rtc_upper_isr[id].callback(id, AON_RTC_NAME, s_aon_rtc_upper_isr[id].isr_param);
		}

		aon_rtc_hal_clear_upper_int_status(&s_aon_rtc[id].hal);
	}

	//TODO: clear NVIC/INTC/PLIC int pending status

	return BK_OK;
}
#else

#if AON_RTC_DEBUG
#define AON_RTC_ISR_DEBUG_MAX_CNT (256)
static uint32_t s_isr_cnt = 0;
static uint32_t s_isr_debug_in_tick[AON_RTC_ISR_DEBUG_MAX_CNT];
static uint32_t s_isr_debug_out_tick[AON_RTC_ISR_DEBUG_MAX_CNT];
static uint32_t s_isr_debug_set_tick[AON_RTC_ISR_DEBUG_MAX_CNT];
#endif
static bk_err_t aon_rtc_isr_handler(aon_rtc_id_t id)
{
	uint32_t int_level = rtos_disable_int();

#if AON_RTC_DEBUG
	s_isr_debug_in_tick[(s_isr_cnt)%AON_RTC_ISR_DEBUG_MAX_CNT] = bk_aon_rtc_get_current_tick(id);
#endif

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	//uses tick as one time timer
	if(aon_rtc_hal_get_tick_int_status(&s_aon_rtc[id].hal))
	{
		//maybe the isr callback runs too much time and set next tick value too small, caused next isr can't response.
		aon_rtc_hal_clear_tick_int_status(&s_aon_rtc[id].hal);

		alarm_update_expeired_nodes(id);

		//reset the timer tick
		if(s_aon_rtc[id].alarm_head_p)
		{
			//+1:to assume set it valid,maybe aon rtc add 1 tick when set the value now.
			BK_ASSERT(bk_aon_rtc_get_current_tick(id) + 1/*AON_RTC_PRECISION_TICK*/ < s_aon_rtc[id].alarm_head_p->expired_tick);	//4:reserve enough time to set the tick
			aon_rtc_set_tick(&s_aon_rtc[id].hal, (uint32_t)s_aon_rtc[id].alarm_head_p->expired_tick);
#if AON_RTC_DEBUG
			s_isr_debug_set_tick[(s_isr_cnt)%AON_RTC_ISR_DEBUG_MAX_CNT] = (uint32_t)s_aon_rtc[id].alarm_head_p->expired_tick;
#endif
			AON_RTC_LOGD("next tick=0x%x, cur_tick=0x%x\r\n", (uint32_t)s_aon_rtc[id].alarm_head_p->expired_tick, (uint32_t)bk_aon_rtc_get_current_tick(id));
		}
		else
		{
			aon_rtc_set_tick(&s_aon_rtc[id].hal, AON_RTC_ROUND_TICK);
			AON_RTC_LOGI("no alarm:cur_tick=0x%x\r\n", (uint32_t)bk_aon_rtc_get_current_tick(id));
		}
	}

	//uses upper timer as period timer
	if(aon_rtc_hal_get_upper_int_status(&s_aon_rtc[id].hal))
	{
		s_high_tick[id]++;
		AON_RTC_LOGI("32bits data overflow:high_tick=%d\r\n", s_high_tick[id]);
		if (s_aon_rtc_upper_isr[id].callback) {
			s_aon_rtc_upper_isr[id].callback(id, AON_RTC_NAME, s_aon_rtc_upper_isr[id].isr_param);
		}

		aon_rtc_hal_clear_upper_int_status(&s_aon_rtc[id].hal);
	}

	//TODO: clear NVIC/INTC/PLIC int pending status

	AON_RTC_LOGD("%s[-]\r\n", __func__);
#if AON_RTC_DEBUG
	s_isr_debug_out_tick[(s_isr_cnt)%AON_RTC_ISR_DEBUG_MAX_CNT] = bk_aon_rtc_get_current_tick(id);
	s_isr_cnt++;
#endif
	rtos_enable_int(int_level);

	return BK_OK;
}

#endif

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

//run from 0
#if 0
static void aon_rtc_start_run(aon_rtc_id_t id)
{
	extern void delay_10us(UINT32 count);

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	//start to run
	aon_rtc_hal_start_counter(&s_aon_rtc[id].hal);
	aon_rtc_hal_reset_counter(&s_aon_rtc[id].hal);	//maybe the start value isn't 0
	delay_10us(4);	//one cycle of 32k
	aon_rtc_hal_clear_reset_counter(&s_aon_rtc[id].hal);

	AON_RTC_LOGD("%s[-]\r\n", __func__);
}
#endif

static bk_err_t aon_rtc_sw_init(aon_rtc_id_t id)
{
	os_memset(&s_aon_rtc[id], 0, sizeof(s_aon_rtc[id]));
	os_memset(&s_aon_rtc_tick_isr[id], 0, sizeof(s_aon_rtc_tick_isr[id]));
	os_memset(&s_aon_rtc_upper_isr[id], 0, sizeof(s_aon_rtc_upper_isr[id]));

	s_high_tick[id] = 0;

	s_aon_rtc_nodes_p[id] = os_zalloc(sizeof(aon_rtc_nodes_memory_t));
	if(s_aon_rtc_nodes_p[id] == NULL)
	{
		return BK_ERR_NO_MEM;
	}

	return BK_OK;
}

static void aon_rtc_hw_init(aon_rtc_id_t id)
{
	aon_rtc_int_config_t int_config_table[] = AON_RTC_INT_CONFIG_TABLE;
	aon_rtc_int_config_t *cur_int_cfg = &int_config_table[id];

	aon_rtc_hal_init(&s_aon_rtc[id].hal);

	//set upper to max value
	aon_rtc_hal_set_upper_val(&s_aon_rtc[id].hal, AON_RTC_ROUND_TICK);
	aon_rtc_hal_enable_upper_int(&s_aon_rtc[id].hal);

	bk_int_isr_register(cur_int_cfg->int_src, cur_int_cfg->isr, NULL);
#if (CONFIG_SYSTEM_CTRL)
	aon_rtc_interrupt_enable(id);
#endif
	aon_rtc_hal_start_counter(&s_aon_rtc[id].hal);
}

bk_err_t bk_aon_rtc_driver_init(void)
{

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	//TOTO: Enter critical protect
	for (int id = AON_RTC_ID_1; id < AON_RTC_ID_MAX; id++) {
		if(!s_aon_rtc[id].inited)
		{
			aon_rtc_sw_init(id);
			aon_rtc_hw_init(id);
			s_aon_rtc[id].inited = true;
		}
	}

	//TOTO: exit critical protect
	AON_RTC_LOGD("%s[-]\r\n", __func__);

	return BK_OK;
}

bk_err_t bk_aon_rtc_driver_deinit(void)
{
	aon_rtc_int_config_t int_cfg_table[] = AON_RTC_INT_CONFIG_TABLE;

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	for (int id = AON_RTC_ID_1; id < AON_RTC_ID_MAX; id++) {
		if(s_aon_rtc[id].inited)
		{
			bk_int_isr_unregister(int_cfg_table[id].int_src);
#if (CONFIG_SYSTEM_CTRL)
			aon_rtc_interrupt_disable(id);
#endif
			if(s_aon_rtc_nodes_p[id] != NULL)
			{
				os_free(s_aon_rtc_nodes_p[id]);
				s_aon_rtc_nodes_p[id] = NULL;
			}

			s_aon_rtc[id].inited = false;
		}
	}

	AON_RTC_LOGD("%s[-]\r\n", __func__);
	return BK_OK;
}

#if 0	//remove it, only one HW can't be used for many APPs.
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

		aon_rtc_set_tick(&s_aon_rtc[id].hal, tick);
		aon_rtc_hal_enable_tick_int(&s_aon_rtc[id].hal);
	}

	//start to run
	aon_rtc_start_run(id);

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
#endif

bk_err_t bk_alarm_register(aon_rtc_id_t id, alarm_info_t *alarm_info_p)
{
	alarm_node_t *node_p = NULL;
	uint32_t int_level = 0;

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	if(id >= AON_RTC_ID_MAX)
	{
		AON_RTC_LOGE("%s:id=%d\r\n", __func__, id);
		return BK_ERR_PARAM;
	}

	if(alarm_info_p == NULL)
	{
		return BK_ERR_PARAM;
	}

	if(alarm_info_p->period_tick < AON_RTC_PRECISION_TICK)	//in protect area to reduce consume time before set tick.
	{
		AON_RTC_LOGE("period_tick should not smaller then %d\r\n", AON_RTC_PRECISION_TICK);
		return BK_FAIL;
	}

	int_level = rtos_disable_int();

	if(s_aon_rtc[id].alarm_node_cnt >= AON_RTC_MAX_ALARM_CNT)
	{
		rtos_enable_int(int_level);
		AON_RTC_LOGE("alarm registered too much:%d\r\n", AON_RTC_MAX_ALARM_CNT);
		return BK_FAIL;
	}

	//request a node
	node_p = aon_rtc_request_node(id);
	if(node_p == NULL)
	{
		rtos_enable_int(int_level);
		AON_RTC_LOGE("alarm registered:no memory\r\n");
		return BK_ERR_NO_MEM;
	}

	memset(node_p, 0, sizeof(alarm_node_t));
	node_p->callback = alarm_info_p->callback;
	node_p->cb_param_p = alarm_info_p->param_p;
	memcpy(&node_p->name[0], alarm_info_p->name, ALARM_NAME_MAX_LEN);
	node_p->name[ALARM_NAME_MAX_LEN] = 0;
	node_p->start_tick = bk_aon_rtc_get_current_tick(id);	//tick
	node_p->period_tick = alarm_info_p->period_tick;
	BK_ASSERT(alarm_info_p->period_cnt);
	node_p->period_cnt = alarm_info_p->period_cnt;
	node_p->expired_tick = node_p->start_tick + (alarm_info_p->period_tick);
	
	//push to alarm list
	if(alarm_insert_node(id, node_p) != 0)
	{
		AON_RTC_LOGE("alarm name=%s has registered\r\n", alarm_info_p->name);
		aon_rtc_release_node(id, node_p);
		rtos_enable_int(int_level);
		return BK_FAIL;
	}

	//reset the timer tick
	if(node_p == s_aon_rtc[id].alarm_head_p)	//insert node is the first one, should reset tick val
	{
		//+1:to assume set it valid,maybe aon rtc add 1 tick when set the value now.
		BK_ASSERT(bk_aon_rtc_get_current_tick(id) + 1/*AON_RTC_PRECISION_TICK*/ < s_aon_rtc[id].alarm_head_p->expired_tick);	//4:reserve enough time to set the tick
		aon_rtc_set_tick(&s_aon_rtc[id].hal, (uint32_t)s_aon_rtc[id].alarm_head_p->expired_tick);
	}

	aon_rtc_hal_enable_tick_int(&s_aon_rtc[id].hal);
	AON_RTC_LOGD("next tick=0x%x, cur_tick=0x%x\r\n", (uint32_t)s_aon_rtc[id].alarm_head_p->expired_tick, (uint32_t)bk_aon_rtc_get_current_tick(id));

	rtos_enable_int(int_level);

	AON_RTC_LOGD("%s[-]\r\n", __func__);

	return BK_OK;
}


//the timer isn't expired, but app un-register it.
bk_err_t bk_alarm_unregister(aon_rtc_id_t id, uint8_t *name_p)
{
	alarm_node_t *remove_node_p = NULL;
	alarm_node_t *previous_head_node_p = NULL;
	uint32_t int_level = 0;

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	if(id >= AON_RTC_ID_MAX)
	{
		AON_RTC_LOGE("%s:id=%d\r\n", __func__, id);
		return BK_ERR_PARAM;
	}

	int_level = rtos_disable_int();
	previous_head_node_p = s_aon_rtc[id].alarm_head_p;
	remove_node_p = alarm_remove_node(id, name_p);

	//reset the timer tick
	if(previous_head_node_p == remove_node_p)	//the previous head is removed
	{
		if(s_aon_rtc[id].alarm_head_p)	//new head exist
		{
			//+1:to assume set it valid,maybe aon rtc add 1 tick when set the value now.
			BK_ASSERT(bk_aon_rtc_get_current_tick(id) + 1/*AON_RTC_PRECISION_TICK*/ < s_aon_rtc[id].alarm_head_p->expired_tick);	//reserve enough time to set the tick
			aon_rtc_set_tick(&s_aon_rtc[id].hal, (uint32_t)s_aon_rtc[id].alarm_head_p->expired_tick);
			AON_RTC_LOGD("next tick=0x%x, cur_tick=0x%x\r\n", s_aon_rtc[id].alarm_head_p->expired_tick, bk_aon_rtc_get_current_tick(id));
		}
		else	//has no nodes now
		{
			aon_rtc_set_tick(&s_aon_rtc[id].hal, AON_RTC_ROUND_TICK);
			AON_RTC_LOGI("no alarm:cur_tick=0x%x\r\n", bk_aon_rtc_get_current_tick(id));
		}
	}

	rtos_enable_int(int_level);

	AON_RTC_LOGD("%s[-]\r\n", __func__);
	return BK_OK;
}

#if (CONFIG_AON_RTC && (!CONFIG_AON_RTC_MANY_USERS))
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
#endif

uint64_t bk_aon_rtc_get_current_tick(aon_rtc_id_t id)
{
	if(id >= AON_RTC_ID_MAX)
	{
		AON_RTC_LOGE("%s:id=%d\r\n", __func__, id);
		return 0;
	}

	return ((s_high_tick[id] << 32) + aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal));
}

#if AON_RTC_DEBUG
void bk_aon_rtc_timing_test(aon_rtc_id_t id, uint32_t round, uint32_t cycles, uint32_t set_tick)
{
	uint32_t int_level = 0;
	uint32_t i = 0, j = 0;
	uint32_t start_tick = 0, end_tick = 0;
	uint64_t u64_start_tick = 0, u64_end_tick = 0;
	uint32_t max_offset_tick = 0, min_offset_tick = 0xffffffff;
	uint32_t fail_cnt = 0;

	AON_RTC_LOGD("%s[+]\r\n", __func__);

	int_level = rtos_disable_int();
	
	//get uint32_t tick counter check
	for(i = 0; i < round; i++)
	{
		start_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);
		for(j = 0; j < cycles; j++)
		{
			aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);
		}
		end_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);

		if(min_offset_tick > end_tick - start_tick)
			min_offset_tick = end_tick - start_tick;
		if(max_offset_tick < end_tick - start_tick)
			max_offset_tick = end_tick - start_tick;
	}
	AON_RTC_LOGI("Gettick uint32:%d rounds*%d times:max=%d,min=%d\r\n", i, j, max_offset_tick, min_offset_tick);

	//get uint64_t tick counter check
	max_offset_tick = 0;
	min_offset_tick = 0xffffffff;
	for(i = 0; i < round; i++)
	{
		u64_start_tick = bk_aon_rtc_get_current_tick(id);
		for(j = 0; j < cycles; j++)
		{
			bk_aon_rtc_get_current_tick(id);
		}
		u64_end_tick = bk_aon_rtc_get_current_tick(id);

		if(min_offset_tick > (uint32_t)(u64_end_tick - u64_start_tick))
			min_offset_tick = (uint32_t)(u64_end_tick - u64_start_tick);
		if(max_offset_tick < u64_end_tick - u64_start_tick)
			max_offset_tick = u64_end_tick - u64_start_tick;
	}
	AON_RTC_LOGI("Gettick uint64:%d rounds*%d times:max=%d,min=%d\r\n", i, j, max_offset_tick, min_offset_tick);

	//set tick val check
	max_offset_tick = 0;
	min_offset_tick = 0xffffffff;
	for(i = 0; i < round; i++)
	{
		start_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);
		for(j = 0; j < cycles; j++)
		{
			aon_rtc_set_tick(&s_aon_rtc[id].hal, set_tick);
		}
		end_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);

		if(min_offset_tick > end_tick - start_tick)
			min_offset_tick = end_tick - start_tick;
		if(max_offset_tick < end_tick - start_tick)
			max_offset_tick = end_tick - start_tick;
	}
	AON_RTC_LOGI("Settick:%d rounds*%d times:max=%d,min=%d\r\n", i, j, max_offset_tick, min_offset_tick);

	fail_cnt = 0;
	max_offset_tick = 0;
	min_offset_tick = 0xffffffff;
	for(i = 0; i < round; i++)
	{
		start_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);
		for(j = 0; j < cycles; j++)
		{
			aon_rtc_set_tick(&s_aon_rtc[id].hal, set_tick);
			if(set_tick != aon_rtc_hal_get_tick_val(&s_aon_rtc[id].hal))
			{
				fail_cnt++;
			}
		}
		end_tick = aon_rtc_hal_get_counter_val(&s_aon_rtc[id].hal);

		if(min_offset_tick > end_tick - start_tick)
			min_offset_tick = end_tick - start_tick;
		if(max_offset_tick < end_tick - start_tick)
			max_offset_tick = end_tick - start_tick;
	}
	AON_RTC_LOGI("Settick:%d rounds*%d times:max=%d,min=%d\r\n", i, j, max_offset_tick, min_offset_tick);
	AON_RTC_LOGI("Settick:%d rounds*%d times:check fail_cnt=%d\r\n", i, j, fail_cnt);

	rtos_enable_int(int_level);
	AON_RTC_LOGD("%s[-]\r\n", __func__);
}
#endif

void bk_aon_rtc_dump(aon_rtc_id_t id)
{
#if AON_RTC_DEBUG
	uint32_t i = 0, index = 0;

	for(i = s_isr_cnt - AON_RTC_ISR_DEBUG_MAX_CNT; i < s_isr_cnt; i++)
	{
		index = i % AON_RTC_ISR_DEBUG_MAX_CNT;

		for(volatile uint32_t j = 0; j < 1800; j++);	//confirm log output normarlly
		
		AON_RTC_LOGI("isr_in[%d]=0x%x,out=0x%x,set=0x%x\r\n", index, s_isr_debug_in_tick[index], s_isr_debug_out_tick[index], s_isr_debug_set_tick[index]);
	}
#endif
	aon_rtc_struct_dump();

	alarm_dump_list(s_aon_rtc[id].alarm_head_p);
}

#if AON_RTC_DEBUG
void bk_64bits_test(void)
{
	uint64_t val_64bits = 0xffffffffffff;
	uint64_t x1 = 0x111111111, x2 = 0x222222222, x3 = 0x333333333, t = 0;
	uint32_t xh1 = 0x1, xl1 = 0x11111111;
	//uint32_t xh2 = 0x2, xl2 = 0x22222222;
	//uint32_t xh3 = 0x3, xl3 = 0x33333333;
	//uint32_t th1 = 0, tl1 = 0;
	
	t = xh1;
	t = t<<32;
	t += xl1;
	if (t == 0x111111111)
	{
		AON_RTC_LOGD("left move 0x1<<32 is right\r\n");
	}

	if (t == x1)
	{
		AON_RTC_LOGD("uint64 compare is right\r\n");
	}

	if ((t & 0xffffffff) == xl1)
	{
		AON_RTC_LOGD("uint64 low 32bits is right\r\n");
	}

	if ((t >> 32) == xh1)
	{
		AON_RTC_LOGD("right move uint64 high 32bits is right\r\n");
	}

	if(x1 + x2 == x3)
	{
		AON_RTC_LOGD("uint64 add is right\r\n");
	}

	if(x3 - x2 == x1)
	{
		AON_RTC_LOGD("uint64 minus is right\r\n");
	}

	if((x1 * 2) == x2)
	{
		AON_RTC_LOGD("uint64 multi is right\r\n");
	}	

	if((x2 / 2) == x1)
	{
		AON_RTC_LOGD("uint64 divide2 is right\r\n");
	}	

	if((x2 / 32) == 0x11111111)
	{
		AON_RTC_LOGD("uint64 divide32 is right\r\n");
	}	

	//pass:only output low 32 bits valid data
	for(uint32_t i = 0; i < 64; i++)
		AON_RTC_LOGD("0xffffffffffff>>%d == 0x%llx\r\n", i, val_64bits>>i);		//64 bits printf is error

	//print:BIT64(i) low 32 bits
	for(uint32_t i = 0; i < 64; i++)
		AON_RTC_LOGD("Bit[%d] = 0x%llx, &=0x%llx\r\n", i, BIT64(i), (val_64bits & BIT64(i)));

	AON_RTC_LOGD("64bits move\r\n");
	for(uint64_t i = 0; i < 64; i++)
		AON_RTC_LOGD("Bit[%d] = 0x%llx, &=0x%llx\r\n", i, BIT64(i), (val_64bits & BIT64(i)));

	AON_RTC_LOGD("32bits move\r\n");
	val_64bits = 0xa0a0a0a0a0a0a0a0;
	for(uint32_t i = 0; i < 64; i++)
	{
		uint64_t ret = val_64bits & BIT64(i);
		AON_RTC_LOGD("ret[%d] = 0x%llx = 0x%llx \r\n", i, ret, ret);	//print ret twice as 64 bits
	}

	AON_RTC_LOGD("64bits move\r\n");
	val_64bits = 0xa0a0a0a0a0a0a0a0;
	for(uint64_t i = 0; i < 64; i++)
	{
		uint64_t ret = val_64bits & BIT64(i);
		AON_RTC_LOGD("ret[%d] = 0x%llx = 0x%llx \r\n", i, i, ret);		//print i twice as 64 bits
	}
}
#endif


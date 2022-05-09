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
#include "compiler.h"
#include <os/mem.h>
#include "bk_arm_arch.h"
#include "bk_ps.h"
#include "arch_interrupt.h"
#include "icu_driver.h"
#include "interrupt_base.h"
#include "interrupt.h"
#include "arch_interrupt.h"
#include <driver/int_types.h>
#include <driver/int.h>
#include <common/bk_assert.h>

#define ICU_RETURN_ON_INVALID_DEVS(dev) do {\
				if ((dev) >= INT_SRC_NONE) {\
					return BK_ERR_INT_DEVICE_NONE;\
				}\
			} while(0)

const icu_int_map_t icu_int_map_table[] = ICU_DEV_MAP;

#if CONFIG_ARCH_RISCV
bk_err_t bk_int_isr_register(icu_int_src_t src, int_group_isr_t isr_callback, void*arg)
{
	ICU_RETURN_ON_INVALID_DEVS(src);

	// icu_int_map_t icu_int_map_table[] = ICU_DEV_MAP;
	const icu_int_map_t *icu_int_map = &icu_int_map_table[src];

	uint8_t int_num = icu_int_map->int_bit;

	arch_interrupt_unregister_int(int_num);
	arch_interrupt_register_int(int_num,isr_callback);

	return 0;
}
void interrupt_init(void)
{
	soc_isr_init();
}

void interrupt_deinit(void)
{
	soc_isr_deinit();
}

bk_err_t bk_int_set_priority(icu_int_src_t int_src, uint32_t int_priority)
{
	const icu_int_map_t *icu_int_map = &icu_int_map_table[int_src];
	uint8_t int_num = icu_int_map->int_bit;

	arch_interrupt_set_priority(int_num, int_priority);

	return BK_OK;
}

bk_err_t bk_int_isr_unregister(icu_int_src_t int_number)
{
    arch_interrupt_unregister_int(int_number);
	return BK_OK;
}
#else
static uint32_t s_isr_mask[MAX_INT_GROUP_NUM] = {0};
static isr_list_t s_int_lists[MAX_INT_GROUP_NUM];

#if CONFIG_INT_STATIS
int_statis_t g_int_statis_num = {0};
#endif



static isr_t* int_isr_del(icu_int_src_t src)
{

	LIST_HEADER_T *pos, *next;

	isr_t *cur_ptr;
	isr_list_t *int_list;

	// icu_int_map_t icu_int_map_table[] = ICU_DEV_MAP;
	if (src >= ARRAY_SIZE(icu_int_map_table)) {
		ICU_LOGW("int_isr_del, int_src(%d) is out of icu_int_map_table range\r\n", src);
		return NULL;
	}
	const icu_int_map_t *icu_int_map = &icu_int_map_table[src];

	uint32_t int_num = icu_int_map->int_bit;
	uint32_t group_id = icu_int_map->group;

	int_list = &s_int_lists[group_id];

	ICU_LOGD("delete:src: %d pri=tem_ptr->pri, int_num = %d\r\n", src, int_num);
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	if (list_empty(&int_list->isr)) {
		GLOBAL_INT_RESTORE();
		return NULL;
	}

	list_for_each_safe(pos, next, &int_list->isr) {
		cur_ptr = list_entry(pos, isr_t, list);

		if (int_num == cur_ptr->int_num) {
			ICU_LOGD("delete:src: %d pri=tem_ptr->pri, int_num = %d\r\n", src, int_num);
			list_del(&cur_ptr->list);
			GLOBAL_INT_RESTORE();
			return cur_ptr;
		}
	}

	GLOBAL_INT_RESTORE();

	return NULL;
}


bk_err_t bk_int_isr_unregister(icu_int_src_t src)
{
	ICU_RETURN_ON_INVALID_DEVS(src);
	isr_t *cur_isr = int_isr_del(src);
	if (cur_isr) {
		os_free(cur_isr);
		cur_isr = NULL;
	}
	return BK_OK;
}

bk_err_t int_isr_add(isr_t *new_isr, int group_id)
{
	LIST_HEADER_T *pos, *next;
	isr_t *cur_ptr;
	isr_list_t *int_list;

	int_list = &s_int_lists[group_id];

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	if (list_empty(&int_list->isr)) {
		list_add_head(&new_isr->list, &int_list->isr);
		goto int_isr_add_exit;
	}

	/* Insert the ISR to the function list, this list is sorted by priority number */
	list_for_each_safe(pos, next, &int_list->isr) {
		cur_ptr = list_entry(pos, isr_t, list);

		ICU_LOGD("cur isr: num = %d, pri=%d\n", cur_ptr->int_num, cur_ptr->pri);
		if (new_isr->pri <= cur_ptr->pri) {
			/* add entry at the head of the queue */
			ICU_LOGD("add new isr: num = %d, pri=%d\n", new_isr->int_num, new_isr->pri);
			list_add_tail(&new_isr->list, &cur_ptr->list);
			goto int_isr_add_exit;
		}
	}

	list_add_tail(&new_isr->list, &int_list->isr);
	ICU_LOGD("add new isr: num = %d, pri=%d\n", new_isr->int_num, new_isr->pri);

int_isr_add_exit:
	s_isr_mask[group_id] |= BIT(new_isr->int_num);
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

bk_err_t bk_int_isr_register(icu_int_src_t src, int_group_isr_t isr, void*arg)
{
	ICU_RETURN_ON_INVALID_DEVS(src);

	bk_int_isr_unregister(src);

	// icu_int_map_t icu_int_map_table[] = ICU_DEV_MAP;
	if (src >= ARRAY_SIZE(icu_int_map_table)) {
		ICU_LOGW("int_isr_register, int_src(%d) is out of icu_int_map_table range\r\n", src);
		return BK_ERR_NOT_FOUND;
	}
	const icu_int_map_t *icu_int_map = &icu_int_map_table[src];

	uint8_t int_num = icu_int_map->int_bit;
	uint8_t int_pri = icu_int_map->int_prio;
	uint8_t group_id = icu_int_map->group;
	isr_t *new_isr;

	ICU_LOGD("register isr: src=%d, num=%d, pri=%d, group=%d\r\n", src, int_num, int_pri, group_id );
	new_isr = (isr_t*)os_malloc(sizeof(isr_t));
	if(!new_isr) {
		ICU_LOGE("cur_ptr malloc error\r\n");
		return BK_ERR_NO_MEM;
	}

	new_isr->isr_func = isr;
	new_isr->int_num = int_num;
	new_isr->pri = int_pri;

	return int_isr_add(new_isr, group_id);
}


void group_isr(uint32_t group_id, uint32_t int_status)
{
	uint32_t int_num;
	isr_t *cur_ptr;
	uint32_t status;
	LIST_HEADER_T *next;
	LIST_HEADER_T *pos;

	status = int_status & s_isr_mask[group_id];

	// ICU_LOGD("group_isr:%x:%x\r\n", int_status, status);

#if CONFIG_STA_PS
	mac_ps_dtim_wake(status);
#endif

	list_for_each_safe(pos, next, &s_int_lists[group_id].isr) {
		cur_ptr= list_entry(pos, isr_t, list);
		int_num = cur_ptr->int_num;

		if ((BIT(int_num) & status)) {
			cur_ptr->isr_func();
			status &= ~(BIT(int_num));
#if CONFIG_INT_STATIS
	if(group_id == 1)
		g_int_statis_num.fiq_int_statis[int_num-FIQ_START_COUNT_BIT] ++;
	else
		g_int_statis_num.irq_int_statis[int_num] ++;
#endif
		}

		if (0 == status)
			return;
	}
}


void interrupt_init(void)
{
	for(int i =0; i< MAX_INT_GROUP_NUM; i++) {
		INIT_LIST_HEAD(&s_int_lists[i].isr);
	}

	soc_isr_init();
}

void interrupt_deinit(void)
{
	soc_isr_deinit();
}


bk_err_t bk_int_set_priority(icu_int_src_t int_src, uint32_t int_priority)
{
	ICU_RETURN_ON_INVALID_DEVS(int_src);

	isr_t* update_isr = int_isr_del(int_src);

	if (!update_isr) {
		return BK_ERR_INT_NOT_EXIST;
	}

	// icu_int_map_t icu_int_map_table[] = ICU_DEV_MAP;
	const icu_int_map_t *icu_int_map = &icu_int_map_table[int_src];
	uint8_t group_id = icu_int_map->group;

	ICU_LOGI("set int prioty: from %d to %d\n", update_isr->pri, int_priority);
	update_isr->pri = int_priority;
	int_isr_add(update_isr, group_id);

	return BK_OK;
}

bk_err_t bk_int_set_group(void)
{
	return BK_ERR_NOT_SUPPORT;
}

void interrupt_spurious(void)
{
	//BK_ASSERT(0);
}

void rf_ps_wakeup_isr_idle_int_cb(void)
{
#if ( CONFIG_MP3PLAYER == 1 )
	uint32_t irq_status;

	irq_status = icu_get_irq_int_status();

	if (irq_status & 1 << IRQ_I2S_PCM) {
		irq_status &= 1 << IRQ_I2S_PCM;
		i2s_isr();
		icu_clear_irq_int_status(irq_status);
	}
#endif
}
#endif


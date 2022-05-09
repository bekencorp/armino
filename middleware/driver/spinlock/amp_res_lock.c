// Copyright 2020-2022 Beken
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

#include <stdio.h>
#include <string.h>

#include <os/os.h>
#include "mailbox_channel.h"
#include "mb_ipc_cmd.h"
#include "amp_res_lock.h"
#include "amp_lock_api.h"

#define AMP_CPU_CNT		2			/* < 32 CPUs. for the xchg_buff is 64-bytes. */

typedef struct
{
	u8					inited;
	beken_semaphore_t	res_sema;

#if CONFIG_MASTER_CORE
	u16		req_cnt[AMP_CPU_CNT];
#endif

} amp_res_sync_t;

static amp_res_sync_t	amp_res_sync[AMP_RES_ID_MAX];

#if CONFIG_MASTER_CORE

/* call this API in interrupt disabled state. */
bk_err_t amp_res_acquire_cnt(u16 res_id, u16 cpu_id, amp_res_req_cnt_t *cnt_list)
{
	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

	if(cpu_id >= AMP_CPU_CNT)
		return BK_ERR_PARAM;

	u16		i = 0;
	u16 	other_cnt = 0;
	
	cnt_list->self_req_cnt = amp_res_sync[res_id].req_cnt[cpu_id];

	for(i = 0; i < AMP_CPU_CNT; i++)
	{
		other_cnt += amp_res_sync[res_id].req_cnt[i];
	}

	cnt_list->others_req_cnt = other_cnt - cnt_list->self_req_cnt;

	amp_res_sync[res_id].req_cnt[cpu_id]++;

	return BK_OK;
	
}

/* call this API in interrupt disabled state. */
bk_err_t amp_res_release_cnt(u16 res_id, u16 cpu_id, amp_res_req_cnt_t *cnt_list)
{
	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

	if(cpu_id >= AMP_CPU_CNT)
		return BK_ERR_PARAM;

	u16		i = 0;

	if(amp_res_sync[res_id].req_cnt[cpu_id] > 0)
	{
		amp_res_sync[res_id].req_cnt[cpu_id]--;

		u16 	other_cnt = 0;

		for(i = 0; i < AMP_CPU_CNT; i++)
		{
			other_cnt += amp_res_sync[res_id].req_cnt[i];
		}

		cnt_list->self_req_cnt = amp_res_sync[res_id].req_cnt[cpu_id];
		cnt_list->others_req_cnt = other_cnt - cnt_list->self_req_cnt;

		return BK_OK;
	}
	else
	{
		return BK_FAIL;
	}
	
}

#endif

#ifdef CONFIG_DUAL_CORE

/* Apps can't call this API, it's for IPC isr only. */
bk_err_t amp_res_available(u16 res_id)
{
	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

	return rtos_set_semaphore(&amp_res_sync[res_id].res_sema);
}

#endif

bk_err_t amp_res_acquire(u16 res_id, u32 timeout_ms)
{
	bk_err_t	ret_val = BK_FAIL;

	if( rtos_is_in_interrupt_context() )
	{
		BK_LOGE("AMP", "can't call in ISR %s,%d\r\n", __FUNCTION__, __LINE__);

		return BK_FAIL;
	}

	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

#ifdef CONFIG_DUAL_CORE

	amp_res_req_cnt_t	cnt_list;

#if CONFIG_MASTER_CORE

	u32  int_mask = rtos_disable_int();

	ret_val = amp_res_acquire_cnt(res_id, SRC_CPU, &cnt_list);

	rtos_enable_int(int_mask);

#endif

#if CONFIG_SLAVE_CORE

	ret_val = ipc_send_res_acquire_cnt(res_id, SRC_CPU, &cnt_list);

#endif

	if(ret_val != BK_OK)
	{
		return ret_val;
	}

	if((cnt_list.self_req_cnt == 0) && (cnt_list.others_req_cnt > 0))
	{
		/* resource was occupied by other CPU, so set semaphore state to unavailable. */
		ret_val = rtos_get_semaphore(&amp_res_sync[res_id].res_sema, 0);

		if(ret_val != BK_OK)
		{
			return ret_val;
		}
	}

#endif

	ret_val = rtos_get_semaphore(&amp_res_sync[res_id].res_sema, timeout_ms);

	return ret_val;

}

bk_err_t amp_res_release(u16 res_id)
{
	bk_err_t	ret_val = BK_FAIL;

	if( rtos_is_in_interrupt_context() )
	{
		BK_LOGE("AMP", "can't call in ISR %s,%d\r\n", __FUNCTION__, __LINE__);

		return BK_FAIL;
	}

	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

#ifdef CONFIG_DUAL_CORE

	amp_res_req_cnt_t	cnt_list;

#if CONFIG_MASTER_CORE

	u32  int_mask = rtos_disable_int();

	ret_val = amp_res_release_cnt(res_id, SRC_CPU, &cnt_list);

	rtos_enable_int(int_mask);

#endif

#if CONFIG_SLAVE_CORE

	ret_val = ipc_send_res_release_cnt(res_id, SRC_CPU, &cnt_list);

#endif

	if(ret_val != BK_OK)
	{
		return ret_val;
	}

	if((cnt_list.self_req_cnt == 0) && (cnt_list.others_req_cnt > 0))
	{
		/* other CPU is waiting for the resource, so inform CPU that it is available. */
		/* which CPU is selected in multi-cores? (over than 2 cores)*/
		ret_val = ipc_send_available_ind(res_id);

		if(ret_val != BK_OK)
		{
			return ret_val;
		}
	}

#endif

	ret_val = rtos_set_semaphore(&amp_res_sync[res_id].res_sema);

	return ret_val;

}

bk_err_t amp_res_init(u16 res_id)
{
	bk_err_t	ret_val = BK_FAIL;

	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited != 0)
		return BK_OK;

#if CONFIG_MASTER_CORE

	u16 i = 0;

	for(i = 0; i < AMP_CPU_CNT; i++)
	{
		amp_res_sync[res_id].req_cnt[i] = 0;
	}

#endif

	ret_val = rtos_init_semaphore_adv(&amp_res_sync[res_id].res_sema, 1, 1);

	if(ret_val != BK_OK)
		return ret_val;

	amp_res_sync[res_id].inited = 1;

	return BK_OK;
}


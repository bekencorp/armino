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

#include <common/bk_include.h>
#include <common/bk_typedef.h>
#include "spinlock.h"

#if 0  /* dual core have the separated memory space. */
#ifdef CONFIG_DUAL_CORE
#define CONFIG_CPU_NUM		2
#else
#define CONFIG_CPU_NUM		1
#endif
#endif

extern void		arch_fence(void);
extern u32		arch_atomic_set(u32 * lock_addr);
extern void		arch_atomic_clear(u32 * lock_addr);

extern u32		arch_int_lock(void);
extern void		arch_int_restore(u32 int_flag);

typedef struct
{
//	u32		cpu_id;
	u32		nest_cnt;
	u32		int_flag;
} cpu_info_t;

//static cpu_info_t	cpu_info[CONFIG_CPU_NUM];
/* dual core have the separated memory space, so don't need cpu_info array for each core. */
static cpu_info_t	cpu_info = { .nest_cnt = 0, .int_flag = 0 };

#if 0
void init_cpu_info(void)
{
	u32	i;

	for(i = 0; i < CONFIG_CPU_NUM; i++)
	{
		cpu_info[i].cpu_id = i;
		cpu_info[i].int_flag = 0;
		cpu_info[i].nest_cnt = 0;
	}
}
#endif

static cpu_info_t * get_cur_cpu(void)
{
#if 0
#ifdef CONFIG_SLAVE_CORE

	return &cpu_info[1];

#endif

	return &cpu_info[0];
#else
	return &cpu_info;
#endif
}

static void push_disable_int_flag(void)
{
	cpu_info_t *cur_cpu = get_cur_cpu();
	u32			int_flag = arch_int_lock();
	
	if(cur_cpu->nest_cnt == 0)
		cur_cpu->int_flag = int_flag;

	cur_cpu->nest_cnt++;
}

static void pop_int_flag(void)
{
	cpu_info_t * cur_cpu = get_cur_cpu();

	if(cur_cpu->nest_cnt < 1)
	{
		/*
		 *  FATAL ERROR!!!
		 */
		return;
	}

	cur_cpu->nest_cnt--;

	if(cur_cpu->nest_cnt == 0)
		arch_int_restore(cur_cpu->int_flag);
}

#ifdef CONFIG_DUAL_CORE
static u32 spinlock_owner(spinlock_t *slock)
{
	if(slock->locked == 0)
		return 0;

	if(slock->owner_cpu != get_cur_cpu())
		return 0;

	return 1;
}
#endif

void spinlock_init(spinlock_t *slock)
{
	slock->owner_cpu = NULL;
	slock->locked = 0;
}

void spinlock_acquire(spinlock_t *slock)
{
	push_disable_int_flag();

#ifdef CONFIG_DUAL_CORE
	if(spinlock_owner(slock))
	{
		/*
		 *  FATAL ERROR!!!
		 */

		bk_printf("FATAL ERROR in %s\r\n", __func__);
		pop_int_flag();
		return;

	}

	while( arch_atomic_set(&slock->locked) ) ;

	arch_fence();

	slock->owner_cpu = get_cur_cpu();
#endif
}

void spinlock_release(spinlock_t *slock)
{
#ifdef CONFIG_DUAL_CORE
	if(!spinlock_owner(slock))
	{
		/*
		 *  FATAL ERROR!!!
		 */

		bk_printf("FATAL ERROR in %s\r\n", __func__);
		return;
	}

	slock->owner_cpu = NULL;

	arch_fence();

	arch_atomic_clear(&slock->locked);
#endif

	pop_int_flag();
}


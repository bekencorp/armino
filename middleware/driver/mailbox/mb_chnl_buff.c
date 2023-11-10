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
#include <driver/mailbox_channel.h>
#include <driver/mb_chnl_buff.h>

#if (CONFIG_DUAL_CORE)

#define MAILBOX_HOST		MAILBOX_CPU0

#define CHNL_BUFF_LEN		128

/* all slave CPUs must send msg to MAILBOX_HOST ONLY. */
/* all slave CPUs can't communicate with each other. */
typedef struct
{
	u8		host_tx_buff[SYSTEM_CPU_NUM - 1][CHNL_BUFF_LEN];
	u8		host_rx_buff[SYSTEM_CPU_NUM - 1][CHNL_BUFF_LEN];
} chnl_xchg_buff_t;

extern char _swap_start, _swap_end;
static chnl_xchg_buff_t * const xchg_buff = (chnl_xchg_buff_t *)(&_swap_start);
//static u8 * const xchg_buff_end = (u8 *)(&_swap_end);

static const u8   chni_id_map[] = 
{
	MB_CHNL_HW_CTRL,
	MB_CHNL_LOG,
};

#define CHNL_IDX_MAX	( sizeof(chni_id_map) / sizeof(chni_id_map[0]) )

static u8 get_chnl_index(u8 chnl_id)
{
	u32   i;

	for(i = 0; i < CHNL_IDX_MAX; i++)
	{
		if(chni_id_map[i] == chnl_id)
			return i;
	}

	return CHNL_IDX_MAX;
}

void * mb_chnl_get_tx_buff(u8 chnl_id)
{
	u8 * tx_buf = NULL;
	u8   idx = get_chnl_index(chnl_id);

	if(idx >= CHNL_IDX_MAX)
		return NULL;

	u8   src_cpu_id = GET_SRC_CPU_ID(chnl_id);
	u8   dst_cpu_id = GET_DST_CPU_ID(chnl_id);

	if(src_cpu_id == MAILBOX_HOST)
	{
		if(dst_cpu_id > MAILBOX_HOST)
			dst_cpu_id--;

		tx_buf = &xchg_buff[idx].host_tx_buff[dst_cpu_id][0];
	}
	else
	{
		if(src_cpu_id > MAILBOX_HOST)
			src_cpu_id--;

		tx_buf = &xchg_buff[idx].host_rx_buff[src_cpu_id][0];
	}

	#if 0
	if((tx_buf + CHNL_BUFF_LEN) > xchg_buff_end)  //  > or >= ?
	{
		return NULL; // buffer overflow.
	}
	#endif

	return (void *)tx_buf;
}

void * mb_chnl_get_rx_buff(u8 chnl_id)
{
	u8 * rx_buf = NULL;
	u8   idx = get_chnl_index(chnl_id);

	if(idx >= CHNL_IDX_MAX)
		return NULL;

	u8   src_cpu_id = GET_SRC_CPU_ID(chnl_id);
	u8   dst_cpu_id = GET_DST_CPU_ID(chnl_id);

	if(src_cpu_id == MAILBOX_HOST)
	{
		if(dst_cpu_id > MAILBOX_HOST)
			dst_cpu_id--;

		rx_buf = &xchg_buff[idx].host_rx_buff[dst_cpu_id][0];
	}
	else
	{
		if(src_cpu_id > MAILBOX_HOST)
			src_cpu_id--;

		rx_buf = &xchg_buff[idx].host_tx_buff[src_cpu_id][0];
	}

	#if 0
	if((rx_buf + CHNL_BUFF_LEN) > xchg_buff_end)  //  > or >= ?
	{
		return NULL; // buffer overflow.
	}
	#endif

	return (void *)rx_buf;
}

u32 mb_chnl_get_buff_len(void)
{
	return CHNL_BUFF_LEN;
}

#endif


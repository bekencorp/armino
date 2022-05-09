// Copyright 2022-2023 Beken
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

#include "bk_sdio.h"
//#include "sdio.h"
#include "sdio_slave_utils.h"
#include "sdio_slave_driver.h"
#include <os/mem.h>
#include "driver/sdio_types.h"


#if CONFIG_SDIO_SLAVE

#ifdef CONFIG_SDIO_DEBUG_EN
int32_t sdio_list_dump_info(SDIO_HEADER_T *head, SDIO_HEADER_T *tail, uint32_t count)
{
	SDIO_HEADER_T *cur_p = head;

	//TODO:log out each node info.
	for(uint32_t i = 0; i < count; i++)
	{
		if(cur_p)
		{
			SDIO_LOG_INFO("nd%d=0x%x,next=0x%x len=%d", i, cur_p, cur_p->next, cur_p->len);
		}
		else
			break;
	}

	if(tail && cur_p != tail)
		SDIO_LOG_INFO("tail=0x%x, len=%d", tail, tail->len);

	return 0;
}
#endif

/**
 * @brief	  Init a sdio single link list which includes the buffers.
 *
 * This API init a sdio single link list which includes the buffers.
 *	 - Malloc buffers
 *	 - Reserved the header for SDIO
 *   - Linked the buffers with single list
 *   - 
 * @param count How many buffers will be malloced.
 * @param size  Each buffer size for data which doesn't include SDIO header,it should be 4 bytes align
 * @param head	Saved the list head
 * @param tail	Saved the list tail
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_list_init(uint32_t count, uint32_t size, SDIO_HEADER_PTR *head_p, SDIO_HEADER_PTR *tail_p)
{
	uint32_t i = 0;
	SDIO_HEADER_T *cur_p = NULL, *next_p;
	uint32_t buf_len = 0;

	if(head_p == NULL || tail_p == NULL)
	{
		SDIO_LOG_ERR("list init");		
		return BK_ERR_SDIO_LIST_INIT;
	}

	size = ((size + 3) / 4) * 4;	//align to 4 bytes
	buf_len = sizeof(SDIO_HEADER_T) + size;
#ifdef CONFIG_SDIO_DEBUG_EN
	buf_len += SDIO_DEBUG_BUFFER_TAIL_LEN;
#endif
	//first node
	{
		cur_p = *head_p = *tail_p = (SDIO_HEADER_T *)beken_malloc(buf_len);
		if(*head_p == NULL)
		{
			SDIO_LOG_ERR("mem malloc");		
			return BK_ERR_SDIO_NO_MEM;
		}

		cur_p->len = 0;
#ifdef CONFIG_SDIO_DEBUG_EN
		//TODO:log info
		*(uint32_t *)(((uint8_t *)cur_p+buf_len-SDIO_DEBUG_BUFFER_TAIL_LEN)) = SDIO_DEBUG_MAGIC_VALUE;
#endif
	}

	for(i = 1; i  < count; i++)
	{
		next_p = (SDIO_HEADER_T *)beken_malloc(buf_len);
		if(next_p == NULL)
		{
			SDIO_LOG_ERR("mem malloc");		
			return BK_ERR_SDIO_NO_MEM;
		}
		next_p->len = 0;
#ifdef CONFIG_SDIO_DEBUG_EN
		*(uint32_t *)((uint8_t *)(cur_p+buf_len-SDIO_DEBUG_BUFFER_TAIL_LEN)) = SDIO_DEBUG_MAGIC_VALUE;
#endif

		cur_p->next = next_p;
		cur_p = cur_p->next;
	}
	cur_p->next = NULL;
	*tail_p = cur_p;

#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(*head_p, *tail_p, count);
#endif

	return BK_OK;
}

void sdio_list_deinit(SDIO_HEADER_T *head)
{

}

static bk_err_t sdio_list_check(SDIO_HEADER_T *push_head, SDIO_HEADER_T *push_tail, uint32_t count)
{
	SDIO_HEADER_T *cur_p = push_head;
	uint32_t compute = 0;

	if(push_head == NULL)
	{
		SDIO_LOG_ERR("head null");
		return BK_ERR_SDIO_LIST;
	}

	if(push_tail && push_tail->next != NULL)
	{
		SDIO_LOG_ERR("tail invalid");
		return BK_ERR_SDIO_LIST;
	}

	while(cur_p)
	{
		cur_p = cur_p->next;
		compute++;
		if(compute == count)
		{
			if(cur_p == push_tail)
			{
				return BK_OK;
			}
			else
			{
				SDIO_LOG_ERR("list is invalid");
				return BK_ERR_SDIO_LIST;
			}	
		}
	}

	return BK_OK;
}

/**
 * @brief	  Pop a node from the selected list.
 *
 * This API Pop a node from the selected list.
 *	 - Pop out a node from the selected list
 *   - Update the list info
 *   - 
 * @param list_p Selected pop node list
 * @param node_p Save the pop out node
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_list_pop_node(SDIO_LIST_T *list_p, SDIO_HEADER_PTR *node_p)
{
	bk_err_t ret = BK_OK;

	ret = sdio_list_check(list_p->head, list_p->tail, list_p->count);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("list fail");
		goto err_exit;
	}

	if(list_p->count)
	{
		SDIO_HEADER_PTR tmp_node_p = list_p->head;
		*node_p = tmp_node_p;
		if(list_p->head == list_p->tail)	//last one
		{
			list_p->head = list_p->tail =NULL;
		}
		else
		{
			list_p->head = list_p->head->next;
		}
		tmp_node_p->next = NULL;

		list_p->count--;
	}

#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(list_p->head, NULL, 1);
#endif

err_exit:
	*node_p = NULL;
	return ret;
}

/**
 * @brief	  Push a source list to target list.
 *
 * This API Push a source list to target list.
 *	 - Check list is whether valid
 *	 - Link two list together
 *   - Update the list info
 *   - 
 * @param list_p Selected target list
 * @param head_p Source list header node
 * @param tail_p Source list tail node
 * @param count How many nodes of source list
  *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_list_push_list(SDIO_LIST_T *list_p, SDIO_HEADER_T *head_p, SDIO_HEADER_T *tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;

	ret = sdio_list_check(head_p, tail_p, count);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("list fail");
		return ret;
	}

	if(list_p->head == NULL)	//ongoing is null, first in
	{
		list_p->head = head_p;
		list_p->tail = tail_p;
		list_p->count = count;
	}
	else
	{
		ret = sdio_list_check(list_p->head, 
							list_p->tail, 
							list_p->count);
		if(ret != BK_OK)
		{
			SDIO_LOG_ERR("list fail");
			return ret;
		}

		list_p->tail->next = head_p;
		list_p->tail = tail_p;
		list_p->count += count;
	}

	//TODO:Just debug
	ret = sdio_list_check(list_p->head, 
						list_p->tail, 
						list_p->count);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("list fail");
		return ret;
	}
	
	return ret;
}

SDIO_HEADER_T *sdio_list_pop_list(SDIO_HEADER_T *head, uint32_t count)
{
	return 0;
}

uint32_t sdio_list_get_node_count(SDIO_LIST_T *list_p)
{
	return list_p->count;
}
#endif
// EOF


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



#ifndef _SDIO_UTIL_H_
#define _SDIO_UTIL_H_

#define SDIO_DEBUG_BUFFER_TAIL_LEN (4)
#define SDIO_DEBUG_MAGIC_VALUE (0xA0A05050)

typedef struct sdio_header_t
{
	struct sdio_header_t *next;
	uint32_t len;
	uint32_t priv; 
	uint32_t rsvd;
}SDIO_HEADER_T, *SDIO_HEADER_PTR;

typedef struct sdio_list_t
{
	SDIO_HEADER_PTR head;
	SDIO_HEADER_PTR tail;
	uint32_t count;
}SDIO_LIST_T;

int32_t sdio_list_dump_info(SDIO_HEADER_T *head, SDIO_HEADER_T *tail, uint32_t count);

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
bk_err_t sdio_list_init(uint32_t count, uint32_t size, SDIO_HEADER_PTR *head_p, SDIO_HEADER_PTR *tail_p);

void sdio_list_deinit(SDIO_HEADER_T *head);

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
bk_err_t sdio_list_pop_node(SDIO_LIST_T *list_p, SDIO_HEADER_PTR *node_p);

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
bk_err_t sdio_list_push_list(SDIO_LIST_T *list_p, SDIO_HEADER_T *head_p, SDIO_HEADER_T *tail_p, uint32_t count);

SDIO_HEADER_T *sdio_list_pop_list(SDIO_HEADER_T *head, uint32_t count);

uint32_t sdio_list_get_node_count(SDIO_LIST_T *list_p);

#endif // _SDIO_UTIL_H_

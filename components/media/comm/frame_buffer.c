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

#include <os/os.h>
#include <components/log.h>

#include "media_core.h"

#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>
#include <soc/mapping.h>
#include <driver/media_types.h>

#include "frame_buffer.h"
#include "mlist.h"

#define TAG "frame_buffer"

#include "bk_list.h"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define list_for_each_safe_edge(pos, n, head) \
	for (pos = (head)->next, n = pos->next; (pos != (head)) && (pos->next != pos); \
	     pos = n, n = pos->next)

#define PSRAM_USE_IN_VIDEO     0
#define PSRAM_JPEG_SHARE_MEM   (1024 * 40)
#define PSRAM_H264_SHARE_MEM   (1024 * 80)

#if (PSRAM_USE_IN_VIDEO)
#define PSRAM_ADDRESS          (0x60000000UL + PSRAM_H264_SHARE_MEM)//PSRAM_JPEG_SHARE_MEM
#else
#define PSRAM_ADDRESS          (0x60000000UL)
#endif
#define ALINE_SIZE             (1024 * 64)

#if (CONFIG_SOC_BK7256XX)
#define DIS_800X600_SIZE       (((800 * 600 * 2) / ALINE_SIZE + 1) * ALINE_SIZE)
#else
#define DIS_800X600_SIZE       (800 * 600 * 2)
#endif
#define DIS_800X600_COUNT      (4)
#define DIS_800X600_ADDRESS    (PSRAM_ADDRESS)
#define JPG_800X600_SIZE       (1024 * 200)
#define JPG_800X600_COUNT      (4)
#define JPG_800X600_ADDRESS    (PSRAM_ADDRESS + (DIS_800X600_SIZE * DIS_800X600_COUNT))
#define H264_800X600_SIZE      (1024 * 40)
#define H264_800X600_COUNT     (60)
#define H264_800X600_ADDRESS   (JPG_800X600_ADDRESS)

#if (CONFIG_SOC_BK7256XX)
#define DIS_1280X720_SIZE      (((1280 * 720 * 2) / ALINE_SIZE + 1) * ALINE_SIZE)
#else
#define DIS_1280X720_SIZE      (1280 * 720 * 2)
#endif
#define DIS_1280X720_COUNT     (3)
#define DIS_1280X720_ADDRESS   (PSRAM_ADDRESS)
#define JPG_1280X720_SIZE      (1024 * 200)
#define JPG_1280X720_COUNT     (4)
#define JPG_1280X720_ADDRESS   (PSRAM_ADDRESS + (DIS_1280X720_SIZE * DIS_1280X720_COUNT))
#define H264_1280X720_SIZE     (1024 * 40)
#define H264_1280X720_COUNT    (40)
#define H264_1280X720_ADDRESS  (JPG_1280X720_ADDRESS)

extern uint32_t platform_is_in_interrupt_context(void);

const fb_layout_t fb_layout[] =
{
	{
		.ppi = PPI_800X600,
		.set = {
			{/* display */
				DIS_800X600_COUNT,
				DIS_800X600_SIZE,
				DIS_800X600_ADDRESS,
			},
			{/* jpeg */
				JPG_800X600_COUNT,
				JPG_800X600_SIZE,
				JPG_800X600_ADDRESS,
			},
			{/* h264 */
				H264_800X600_COUNT,
				H264_800X600_SIZE,
				H264_800X600_ADDRESS,
			},
		},
	},
	{
		.ppi = PPI_1280X720,
		.set = {
			{/* display */
				DIS_1280X720_COUNT,
				DIS_1280X720_SIZE,
				DIS_1280X720_ADDRESS,
			},
			{/* jpeg */
				JPG_1280X720_COUNT,
				JPG_1280X720_SIZE,
				JPG_1280X720_ADDRESS,
			},
			{/* h264 */
				H264_1280X720_COUNT,
				H264_1280X720_SIZE,
				H264_1280X720_ADDRESS
			}
		},
	},
};



fb_info_t *fb_info = NULL;
fb_mem_list_t fb_mem_list[FB_INDEX_MAX] = {0};


fb_mem_list_t *frame_buffer_list_get(pixel_format_t fmt)
{
	fb_mem_list_t *ret = NULL;

	switch (fmt)
	{
		case PIXEL_FMT_DVP_JPEG:
		case PIXEL_FMT_UVC_JPEG:
		case PIXEL_FMT_UVC_H264:
			ret = &fb_mem_list[FB_INDEX_JPEG];
			break;
		case PIXEL_FMT_RGB565:
		case PIXEL_FMT_RGB565_LE:
		case PIXEL_FMT_YUYV:
		case PIXEL_FMT_UYVY:
		case PIXEL_FMT_YYUV:
		case PIXEL_FMT_UVYY:
		case PIXEL_FMT_VUYY:
			ret = &fb_mem_list[FB_INDEX_DISPLAY];
			break;
		case PIXEL_FMT_DVP_H264:
			ret = &fb_mem_list[FB_INDEX_H264];
			break;
		case PIXEL_FMT_UNKNOW:
		default:
			break;
	}

	return ret;
}

fb_type_t frame_buffer_type_get(pixel_format_t fmt)
{
	fb_type_t ret = FB_INDEX_MAX;

	switch (fmt)
	{
		case PIXEL_FMT_DVP_JPEG:
		case PIXEL_FMT_UVC_JPEG:
		case PIXEL_FMT_UVC_H264:
			ret = FB_INDEX_JPEG;
			break;
		case PIXEL_FMT_RGB565_LE:
		case PIXEL_FMT_RGB565:
		case PIXEL_FMT_YUYV:
		case PIXEL_FMT_UYVY:
		case PIXEL_FMT_YYUV:
		case PIXEL_FMT_UVYY:
		case PIXEL_FMT_VUYY:
			ret = FB_INDEX_DISPLAY;
			break;
		case PIXEL_FMT_DVP_H264:
			ret = FB_INDEX_H264;
			break;
		case PIXEL_FMT_UNKNOW:
		default:
			break;
	}

	return ret;
}

bk_err_t frame_buffer_list_remove(frame_buffer_t *frame, LIST_HEADER_T *list)
{
	frame_buffer_node_t *tmp = NULL;
	frame_buffer_node_t *node = list_entry(frame, frame_buffer_node_t, frame);
	LIST_HEADER_T *pos, *n;
	bk_err_t ret = BK_FAIL;


	list_for_each_safe(pos, n, list)
	{
		tmp = list_entry(pos, frame_buffer_node_t, list);
		if (tmp != NULL
		    && (tmp->frame.frame == node->frame.frame))
		{
			list_del(pos);
			ret = BK_OK;
			break;
		}
	}

	return ret;
}

void frame_buffer_fb_free(frame_buffer_t *frame, frame_module_t index)
{
	if (frame == NULL)
	{
		LOGE("%s %d, frame is null\r\n", __func__, index);
		return;
	}

	fb_mem_list_t *mem_list = frame_buffer_list_get(frame->fmt);
	frame_buffer_node_t *node = list_entry(frame, frame_buffer_node_t, frame);
	uint32_t isr_context = platform_is_in_interrupt_context();
	GLOBAL_INT_DECLARATION();

	if (mem_list == NULL)
	{
		LOGE("%s invalid mem_list: %p, %d\n", __func__, mem_list, index);
		if (fb_info->modules[index].enable)
			rtos_set_semaphore(&fb_info->modules[index].sem);
		return;
	}

	if (index >= MODULE_MAX)
	{
		LOGE("%s invalid module: %d\n", __func__, index);
		return;
	}

	if (!isr_context)
	{
		rtos_lock_mutex(&mem_list->lock);
		GLOBAL_INT_DISABLE();
	}

	if (node->free_mask & INDEX_MASK(index))
	{
		LOGE("%s refree: %d\n", __func__, index);
		goto out;
	}
	else
	{
		node->free_mask |= INDEX_MASK(index);
	}

	if (node->read_mask)
	{
		if ((node->read_mask == node->free_mask)
		    && ((node->free_mask & fb_info->register_mask) == fb_info->register_mask))
		{
			if (BK_OK != frame_buffer_list_remove(frame, &mem_list->ready))
			{
				LOGE("%s remove failed\n", __func__);
			}

			node->free_mask = 0;
			node->read_mask = 0;
			list_add_tail(&node->list, &mem_list->free);
		}

		if (fb_info->modules[index].enable
		    && rtos_set_semaphore(&fb_info->modules[index].sem) != BK_OK)
		{
			LOGE("%s semaphore set faile\n", __func__);
		}
	}
	else
	{
		/* safte check */
		if (BK_OK != frame_buffer_list_remove(frame, &mem_list->ready))
		{
			LOGD("%s remove failed\n", __func__);
		}


		node->free_mask = 0;
		node->read_mask = 0;
		list_add_tail(&node->list, &mem_list->free);
	}

out:

	if (!isr_context)
	{
		GLOBAL_INT_RESTORE();
		rtos_unlock_mutex(&mem_list->lock);
	}
}


frame_buffer_t *frame_buffer_fb_malloc(fb_type_t type)
{
	frame_buffer_node_t *node = NULL, *tmp = NULL;
	fb_mem_list_t *mem_list = &fb_mem_list[type];
	LIST_HEADER_T *pos, *n;
	uint32_t isr_context = platform_is_in_interrupt_context();
	GLOBAL_INT_DECLARATION();

	if (mem_list == NULL)
	{
		LOGE("%s invalid mem_list: %p\n", __func__, mem_list);
		return NULL;
	}

	if (mem_list->lock == NULL)
	{
		LOGE("[%s]:mem_list->lock is NULL\r\n", __func__);
		return NULL;
	}

	if (!isr_context)
	{
		rtos_lock_mutex(&mem_list->lock);
		GLOBAL_INT_DISABLE();
	}

	list_for_each_safe(pos, n, &mem_list->free)
	{
		tmp = list_entry(pos, frame_buffer_node_t, list);
		if (tmp != NULL)
		{
			node = tmp;
			list_del(pos);
			break;
		}
	}

	if (node == NULL)
	{
		list_for_each_safe(pos, n, &mem_list->ready)
		{
			tmp = list_entry(pos, frame_buffer_node_t, list);
			if (tmp != NULL
			    && (tmp->read_mask == tmp->free_mask))
			{
				node = tmp;
				list_del(pos);
				break;
			}
		}
	}

	if (!isr_context)
	{
		GLOBAL_INT_RESTORE();
		rtos_unlock_mutex(&mem_list->lock);
	}

	if (node == NULL)
	{
		LOGE("%s failed\n", __func__);
		return NULL;
	}

	node->frame.length = 0;
	node->frame.width = 0;
	node->frame.height = 0;
	node->frame.fmt = 0;
	node->read_mask = 0;
	node->free_mask = 0;
	node->frame.mix = 0;

	return &node->frame;
}

void frame_buffer_fb_push(frame_buffer_t *frame)
{
	fb_mem_list_t *mem_list = NULL;
	fb_type_t type = frame_buffer_type_get(frame->fmt);
	frame_buffer_node_t *node = list_entry(frame, frame_buffer_node_t, frame);
	uint32_t isr_context = platform_is_in_interrupt_context();
	uint32_t i;
	bk_err_t ret;
	GLOBAL_INT_DECLARATION();

	if (type > FB_INDEX_MAX)
	{
		LOGE("%s invalid frame type\n", __func__);
		return;
	}

	mem_list = &fb_mem_list[type];

	if (!isr_context)
	{
		rtos_lock_mutex(&mem_list->lock);
		GLOBAL_INT_DISABLE();
	}

	node->read_mask = 0;
	node->free_mask = 0;

	list_add_tail(&node->list, &mem_list->ready);


	for (i = 0; i < MODULE_MAX; i++)
	{
		if (fb_info->modules[i].enable == true
		    && fb_info->modules[i].type == type)
		{
			LOGD("cmp plugin\n");

			if ((frame->fmt == PIXEL_FMT_DVP_JPEG)
			    && (i == MODULE_DECODER)
			    && (frame->mix == true))
			{
				//TODO: do not return for decoder
				continue;
			}

			if (mem_list->mode == FB_MEM_SHARED
			    && fb_info->modules[i].plugin == false)
			{
				ret = rtos_set_semaphore(&fb_info->modules[i].sem);

				if (ret != BK_OK)
				{
					LOGE("%s semaphore set failed: %d\n", __func__, ret);
				}

				fb_info->modules[i].plugin = true;
			}
		}
	}

    if (!isr_context)
    {
        GLOBAL_INT_RESTORE();
        rtos_unlock_mutex(&mem_list->lock);
    }
}


frame_buffer_t *frame_buffer_fb_pop(frame_module_t index, fb_type_t type)
{
	fb_mem_list_t *mem_list = &fb_mem_list[type];
	LIST_HEADER_T *pos, *n;
	frame_buffer_node_t *node = NULL, *tmp = NULL;
	uint32_t isr_context = platform_is_in_interrupt_context();
	GLOBAL_INT_DECLARATION();

	if (mem_list == NULL)
	{
		LOGE("%s invalid mem_list: %p\n", __func__, mem_list);
		return NULL;
	}

	if (fb_info == NULL)
	{
		LOGE("%s fb_info was NULL\n", __func__);
		return NULL;
	}

	if (!isr_context)
	{
		rtos_lock_mutex(&mem_list->lock);
		GLOBAL_INT_DISABLE();
	}

	LOGD("type: %d, %p\n", type, mem_list);

	list_for_each_safe_edge(pos, n, &mem_list->ready)
	{
		tmp = list_entry(pos, frame_buffer_node_t, list);
		if (tmp != NULL
		    && ((tmp->read_mask & INDEX_MASK(index)) == 0))
		{
			//LOGI("GET %u, %x, %x\n", tmp->frame.sequence, tmp->read_mask, tmp->read_mask & INDEX_MASK(index));
			node = tmp;
			break;
		}
	}

	if (node != NULL)
	{
		node->read_mask |= INDEX_MASK(index);
	}

	if (!isr_context)
	{
		GLOBAL_INT_RESTORE();
		rtos_unlock_mutex(&mem_list->lock);
	}

	if (node == NULL)
	{
		LOGD("pop NULL\n");
		return NULL;
	}

	//LOGI("pop %u, %x, %x\n", node->frame.sequence, node->read_mask, tmp->read_mask);

	return &node->frame;
}

fb_type_t frame_buffer_available_index(void)
{
	uint32_t isr_context = platform_is_in_interrupt_context();
	fb_type_t index = FB_INDEX_JPEG;
	GLOBAL_INT_DECLARATION();
	uint32_t i = 0;

	if (!isr_context)
	{
		rtos_lock_mutex(&fb_info->lock);
		GLOBAL_INT_DISABLE();
	}


	for (i = 0; i < FB_INDEX_MAX; i++)
	{
		if (!list_empty(&fb_mem_list[i].ready))
		{
			index = i;
			break;
		}
	}

	if (!isr_context)
	{
		GLOBAL_INT_RESTORE();
		rtos_unlock_mutex(&fb_info->lock);
	}

	return index;
}


frame_buffer_t *frame_buffer_fb_read(frame_module_t index)
{
	frame_buffer_t *frame = NULL;
	bk_err_t ret = BK_FAIL;
	uint32_t isr_context = platform_is_in_interrupt_context();
	GLOBAL_INT_DECLARATION();
	fb_type_t type;

	if (index >= MODULE_MAX)
	{
		LOGE("%s invalid module: %d\n", __func__, index);
		goto out;
	}

	if (fb_info->modules[index].enable == false)
	{
		LOGE("%s module not register: %d\n", __func__, index);
		goto out;
	}

	do
	{

		rtos_lock_mutex(&fb_info->modules[index].lock);

		ret = rtos_get_semaphore(&fb_info->modules[index].sem, BEKEN_NEVER_TIMEOUT);

		if (ret != BK_OK)
		{
			LOGD("%s semaphore get failed: %d\n", __func__, ret);
			rtos_unlock_mutex(&fb_info->modules[index].lock);
			goto out;
		}

		if (fb_info->modules[index].enable == false)
		{
			rtos_unlock_mutex(&fb_info->modules[index].lock);
			break;
		}

		rtos_unlock_mutex(&fb_info->modules[index].lock);

		//type = frame_buffer_available_index();
		type = fb_info->modules[index].type;

		frame = frame_buffer_fb_pop(index, type);

		if (frame == NULL)
		{
			LOGD("%s faild, plugin: %d\n", __func__, fb_info->modules[index].plugin);

			if (!isr_context)
			{
				rtos_lock_mutex(&fb_info->lock);
				GLOBAL_INT_DISABLE();
			}

			fb_info->modules[index].plugin = false;

			if (!isr_context)
			{
				GLOBAL_INT_RESTORE();
				rtos_unlock_mutex(&fb_info->lock);
			}
		}
		else
		{
			break;
		}
	}
	while (fb_info && fb_info->modules[index].enable);

out:

	return frame;
}

bk_err_t frame_buffer_fb_register(frame_module_t index, fb_type_t type)
{
	bk_err_t ret = BK_FAIL;
	uint32_t isr_context = platform_is_in_interrupt_context();
	GLOBAL_INT_DECLARATION();

	if (fb_info->modules[index].enable == true)
	{
		LOGE("%s frame_module index already register\n", __func__);
		return ret;
	}

	if (index >= MODULE_MAX)
	{
		LOGE("%s invalid module: %d\n", __func__, index);
		return ret;
	}

	if (fb_info == NULL)
	{
		LOGE("%s fb_info was NULL\n", __func__);
		return ret;
	}

	if (!isr_context)
	{
		rtos_lock_mutex(&fb_info->lock);
		GLOBAL_INT_DISABLE();
	}

	rtos_init_mutex(&fb_info->modules[index].lock);

	ret = rtos_init_semaphore_ex(&fb_info->modules[index].sem, 1, 0);

	if (ret != BK_OK)
	{
		LOGE("%s semaphore init failed: %d\n", __func__, ret);
		goto out;
	}

	fb_info->modules[index].enable = true;
	fb_info->modules[index].plugin = false;
	fb_info->modules[index].type = type;
	fb_info->register_mask |= INDEX_MASK(index);

out:

	if (!isr_context)
	{
		GLOBAL_INT_RESTORE();
		rtos_unlock_mutex(&fb_info->lock);
	}

	return ret;
}

bk_err_t frame_buffer_fb_deregister(frame_module_t index)
{
	bk_err_t ret = BK_FAIL;
	uint32_t isr_context = platform_is_in_interrupt_context();
	GLOBAL_INT_DECLARATION();

	if (fb_info->modules[index].enable == false)
	{
		LOGE("%s frame_module index already degister\n", __func__);
		return ret;
	}

	if (index >= MODULE_MAX)
	{
		LOGE("%s invalid module: %d\n", __func__, index);
		return ret;
	}

	if (fb_info == NULL)
	{
		LOGE("%s fb_info was NULL\n", __func__);
		return ret;
	}

	if (!isr_context)
	{
		rtos_lock_mutex(&fb_info->lock);
		GLOBAL_INT_DISABLE();
	}

	fb_info->modules[index].enable = false;
	fb_info->register_mask &= INDEX_UNMASK(index);

	ret = rtos_set_semaphore(&fb_info->modules[index].sem);


	rtos_lock_mutex(&fb_info->modules[index].lock);
	ret = rtos_deinit_semaphore(&fb_info->modules[index].sem);
	rtos_unlock_mutex(&fb_info->modules[index].lock);
	rtos_deinit_mutex(&fb_info->modules[index].lock);

	if (ret != BK_OK)
	{
		LOGE("%s semaphore deinit failed: %d\n", __func__, ret);
		goto out;
	}

out:

	if (!isr_context)
	{
		GLOBAL_INT_RESTORE();
		rtos_unlock_mutex(&fb_info->lock);
	}

	return ret;
}


int frame_buffer_fb_deinit(fb_type_t type)
{
	fb_mem_list_t *mem_list = NULL;
	frame_buffer_node_t *tmp = NULL;
	LIST_HEADER_T *pos, *n;

	mem_list = &fb_mem_list[type];

	if (type == FB_INDEX_DISPLAY || type == FB_INDEX_H264)
	{
		LOGI("display mem deinit\n");

		if (rtos_deinit_semaphore(&mem_list->free_sem) != BK_OK)
		{
			LOGE("%s free_sem init failed\n", __func__);
		}

		if (rtos_deinit_semaphore(&mem_list->ready_sem) != BK_OK)
		{
			LOGE("%s ready_sem init failed\n", __func__);
		}
	}
	else if (type == FB_INDEX_JPEG)
	{
		LOGI("jpeg mem deinit\n");
	}
	else
	{
		LOGE("%s unknow type: %d\n", __func__, type);
		return BK_FAIL;
	}

	if (mem_list->enable == false)
	{
		LOGE("%s already deinit\n", __func__);
		return BK_FAIL;
	}

	if (!list_empty(&mem_list->free))
	{
		list_for_each_safe(pos, n, &mem_list->free)
		{
			tmp = list_entry(pos, frame_buffer_node_t, list);
			LOGD("free list: %p\n", tmp);
			if (tmp != NULL)
			{
				list_del(pos);
				os_free(tmp);
			}
		}

		INIT_LIST_HEAD(&mem_list->free);
	}

	if (!list_empty(&mem_list->ready))
	{
		list_for_each_safe(pos, n, &mem_list->ready)
		{
			LOGD("ready list: %p\n", tmp);
			tmp = list_entry(pos, frame_buffer_node_t, list);
			if (tmp != NULL)
			{
				list_del(pos);
				os_free(tmp);
			}
		}

		INIT_LIST_HEAD(&mem_list->ready);
	}

	mem_list->enable = false;

	return BK_OK;
}


int frame_buffer_fb_init(media_ppi_t max_ppi, fb_type_t type)
{
	fb_mem_list_t *mem_list = NULL;
	const fb_mem_set_t *fb_mem_set = NULL;

	if (get_ppi_size(max_ppi) <= get_ppi_size(PPI_800X600))
	{
		fb_mem_set = &fb_layout[0].set[type];
	}
	else
	{
		fb_mem_set = &fb_layout[1].set[type];
	}

	mem_list = &fb_mem_list[type];

	if (mem_list->enable == true)
	{
		LOGE("%s already init\n", __func__);
		return BK_FAIL;
	}

	if (type == FB_INDEX_DISPLAY || type == FB_INDEX_H264)
	{
		mem_list->mode = FB_MEM_ALONE;
		mem_list->count = fb_mem_set->count;
		mem_list->free_request = false;
		mem_list->ready_request = false;

		if (rtos_init_semaphore_ex(&mem_list->free_sem, mem_list->count, 0) != BK_OK)
		{
			LOGE("%s free_sem init failed\n", __func__);
		}

		if (rtos_init_semaphore_ex(&mem_list->ready_sem, mem_list->count, 0) != BK_OK)
		{
			LOGE("%s ready_sem init failed\n", __func__);
		}

		LOGI("display mem init size: %X, count: %d\n", fb_mem_set->size, fb_mem_set->count);
	}
	else if (type == FB_INDEX_JPEG)
	{
		mem_list->mode = FB_MEM_SHARED;
		mem_list->count = fb_mem_set->count;
		LOGD("jpeg mem init size: %X, count: %d\n", fb_mem_set->size, fb_mem_set->count);
	}
	else
	{
		LOGE("%s unknow type: %d\n", __func__, type);
		return BK_FAIL;
	}

	for (int i = 0; i < fb_mem_set->count; i ++)
	{
		frame_buffer_node_t *node = (frame_buffer_node_t *)os_malloc(sizeof(frame_buffer_node_t));

		if (node == NULL)
		{
			LOGE("%s os_malloc node failed\n", __func__);
			return BK_FAIL;
		}

		os_memset(node, 0, sizeof(frame_buffer_node_t));

		node->frame.frame = (uint8_t *)(fb_mem_set->base + fb_mem_set->size * i);
		node->frame.size = fb_mem_set->size;
		node->frame.length = 0;
		node->frame.sequence = 0;

		LOGD("free list add: %p\n", node);

		list_add_tail(&node->list, &mem_list->free);

		LOGD("init: %p\n", node->frame.frame);
	}

	mem_list->enable = true;

	return BK_OK;
}


int frame_buffer_fb_jpeg_init(media_ppi_t max_ppi)
{
	return frame_buffer_fb_init(max_ppi, FB_INDEX_JPEG);
}

int frame_buffer_fb_display_init(media_ppi_t max_ppi)
{
	return frame_buffer_fb_init(max_ppi, FB_INDEX_DISPLAY);
}

int frame_buffer_fb_jpeg_deinit(void)
{
	return frame_buffer_fb_deinit(FB_INDEX_JPEG);
}

int frame_buffer_fb_display_deinit(void)
{
	return frame_buffer_fb_deinit(FB_INDEX_DISPLAY);
}

frame_buffer_t *frame_buffer_fb_jpeg_malloc(void)
{
	return frame_buffer_fb_malloc(FB_INDEX_JPEG);
}


void frame_buffer_fb_jpeg_free(frame_buffer_t *frame)
{
	if (frame == NULL)
	{
		LOGE("%s, frame is null\r\n", __func__);
		return;
	}

	fb_mem_list_t *mem_list = &fb_mem_list[FB_INDEX_JPEG];
	frame_buffer_node_t *node = list_entry(frame, frame_buffer_node_t, frame);
	uint32_t isr_context = platform_is_in_interrupt_context();
	GLOBAL_INT_DECLARATION();

	if (mem_list == NULL)
	{
		LOGE("%s invalid mem_list: %p\n", __func__, mem_list);
		return;
	}
	LOGI("%s ========= fb_jpeg_free mem_list: %p\n", __func__, mem_list);

	if (!isr_context)
	{
		rtos_lock_mutex(&mem_list->lock);
		GLOBAL_INT_DISABLE();
	}

	node->free_mask = 0;
	node->read_mask = 0;
	list_add_tail(&node->list, &mem_list->free);

	if (!isr_context)
	{
		GLOBAL_INT_RESTORE();
		rtos_unlock_mutex(&mem_list->lock);
	}
}


frame_buffer_t *frame_buffer_fb_display_malloc(void)
{
	return frame_buffer_fb_malloc(FB_INDEX_DISPLAY);
}

frame_buffer_t *frame_buffer_fb_display_malloc_wait(void)
{
	frame_buffer_t *frame = NULL;
	fb_mem_list_t *mem_list = &fb_mem_list[FB_INDEX_DISPLAY];
	uint32_t isr_context = platform_is_in_interrupt_context();
	bk_err_t ret;
	GLOBAL_INT_DECLARATION();

	do
	{
		frame = frame_buffer_fb_display_malloc();

		if (frame == NULL)
		{
			if (!isr_context)
			{
				rtos_lock_mutex(&mem_list->lock);
				GLOBAL_INT_DISABLE();
			}

			mem_list->free_request = true;

			if (!isr_context)
			{
				GLOBAL_INT_RESTORE();
				rtos_unlock_mutex(&mem_list->lock);
			}

			ret = rtos_get_semaphore(&mem_list->free_sem, BEKEN_NEVER_TIMEOUT);

			if (ret != BK_OK)
			{
				LOGD("%s semaphore get failed: %d\n", __func__, ret);
			}

			continue;
		}

	}
	while (0);

	return frame;
}

void frame_buffer_fb_display_free(frame_buffer_t *frame)
{
	if (frame == NULL)
	{
		LOGE("%s, frame is null\r\n", __func__);
		return;
	}

	fb_mem_list_t *mem_list = &fb_mem_list[FB_INDEX_DISPLAY];
	frame_buffer_node_t *tmp = NULL, *node = list_entry(frame, frame_buffer_node_t, frame);
	uint32_t length = 0, isr_context = platform_is_in_interrupt_context();
	GLOBAL_INT_DECLARATION() = 0;
	bk_err_t ret;
	LIST_HEADER_T *pos, *n;

	//LOGI("dis free\n");

	if (!isr_context)
	{
		rtos_lock_mutex(&mem_list->lock);
		GLOBAL_INT_DISABLE();
	}

	list_add_tail(&node->list, &mem_list->free);

	list_for_each_safe(pos, n, &mem_list->free)
	{
		tmp = list_entry(pos, frame_buffer_node_t, list);
		if (tmp != NULL)
		{
			length++;
		}
	}

	if (mem_list->free_request == true)
	{
		ret = rtos_set_semaphore(&mem_list->free_sem);

		if (ret != BK_OK)
		{
			LOGE("%s semaphore set failed: %d\n", __func__, ret);
		}

		mem_list->free_request = false;
	}


	if (!isr_context)
	{
		GLOBAL_INT_RESTORE();
		rtos_unlock_mutex(&mem_list->lock);
	}

	//LOGI("free\n");
}

void frame_buffer_fb_display_push(frame_buffer_t *frame)
{
	fb_mem_list_t *mem_list = &fb_mem_list[FB_INDEX_DISPLAY];
	frame_buffer_node_t *tmp = NULL, *node = list_entry(frame, frame_buffer_node_t, frame);
	uint32_t isr_context = platform_is_in_interrupt_context();
	uint32_t length = 0;
	GLOBAL_INT_DECLARATION() = 0;
	bk_err_t ret;
	LIST_HEADER_T *pos, *n;

	//LOGI("dis push\n");


	if (!isr_context)
	{
		rtos_lock_mutex(&mem_list->lock);
		GLOBAL_INT_DISABLE();
	}

	list_add_tail(&node->list, &mem_list->ready);

	list_for_each_safe(pos, n, &mem_list->ready)
	{
		tmp = list_entry(pos, frame_buffer_node_t, list);
		if (tmp != NULL)
		{
			length++;
		}
	}

	if (length == 1)
	{
		ret = rtos_set_semaphore(&mem_list->ready_sem);

		if (ret != BK_OK)
		{
			LOGD("%s semaphore set failed: %d\n", __func__, ret);
		}
	}


	if (!isr_context)
	{
		GLOBAL_INT_RESTORE();
		rtos_unlock_mutex(&mem_list->lock);
	}

	//LOGI("push\n");

}

frame_buffer_t *frame_buffer_fb_display_pop(void)
{
	frame_buffer_node_t *node = NULL, *tmp = NULL;
	fb_mem_list_t *mem_list = &fb_mem_list[FB_INDEX_DISPLAY];
	LIST_HEADER_T *pos, *n;
	GLOBAL_INT_DECLARATION() = 0;

	//LOGI("dis pop\n");
	rtos_lock_mutex(&mem_list->lock);
	GLOBAL_INT_DISABLE();

	list_for_each_safe(pos, n, &mem_list->ready)
	{
		tmp = list_entry(pos, frame_buffer_node_t, list);
		if (tmp != NULL)
		{
			node = tmp;
			list_del(pos);
			break;
		}
	}

	GLOBAL_INT_RESTORE();
	rtos_unlock_mutex(&mem_list->lock);

	if (node == NULL)
	{
		LOGD("%s failed\n", __func__);
		return NULL;
	}

	//LOGI("pop\n");

	return &node->frame;
}


frame_buffer_t *frame_buffer_fb_display_pop_wait(void)
{
	frame_buffer_t *frame = NULL;
	fb_mem_list_t *mem_list = &fb_mem_list[FB_INDEX_DISPLAY];
	bk_err_t ret;

	do
	{
		frame = frame_buffer_fb_display_pop();

		if (frame != NULL)
		{
			break;
		}

		ret = rtos_get_semaphore(&mem_list->ready_sem, 2000);

		if (ret != BK_OK)
		{
			LOGD("%s semaphore get failed: %d\n", __func__, ret);
			break;
		}

	}
	while (true);

	return frame;
}

int frame_buffer_fb_h264_init(media_ppi_t max_ppi)
{
	return frame_buffer_fb_init(max_ppi, FB_INDEX_H264);
}

int frame_buffer_fb_h264_deinit(void)
{
	return frame_buffer_fb_deinit(FB_INDEX_H264);
}

frame_buffer_t *frame_buffer_fb_h264_malloc(void)
{
	return frame_buffer_fb_malloc(FB_INDEX_H264);
}

frame_buffer_t *frame_buffer_fb_h264_malloc_wait(void)
{
	frame_buffer_t *frame = NULL;
	fb_mem_list_t *mem_list = &fb_mem_list[FB_INDEX_H264];
	uint32_t isr_context = platform_is_in_interrupt_context();
	bk_err_t ret;
	GLOBAL_INT_DECLARATION();

	do {
		frame = frame_buffer_fb_h264_malloc();

		if (frame == NULL) {
			if (!isr_context) {
				rtos_lock_mutex(&mem_list->lock);
				GLOBAL_INT_DISABLE();
			}

			mem_list->free_request = true;

			if (!isr_context) {
				GLOBAL_INT_RESTORE();
				rtos_unlock_mutex(&mem_list->lock);
			}

			ret = rtos_get_semaphore(&mem_list->free_sem, BEKEN_NEVER_TIMEOUT);

			if (ret != BK_OK) {
				LOGD("%s semaphore get failed: %d\n", __func__, ret);
			}

			continue;
		}

	} while (0);

	return frame;
}

void frame_buffer_fb_h264_free(frame_buffer_t *frame)
{
	if (frame == NULL)
	{
		LOGE("%s, frame is null\r\n", __func__);
		return;
	}

	fb_mem_list_t *mem_list = &fb_mem_list[FB_INDEX_H264];
	frame_buffer_node_t *tmp = NULL, *node = list_entry(frame, frame_buffer_node_t, frame);
	uint32_t length = 0, isr_context = platform_is_in_interrupt_context();
	GLOBAL_INT_DECLARATION() = 0;
	bk_err_t ret;
	LIST_HEADER_T *pos, *n;

	if (!isr_context) {
		rtos_lock_mutex(&mem_list->lock);
		GLOBAL_INT_DISABLE();
	}

	list_add_tail(&node->list, &mem_list->free);

	list_for_each_safe(pos, n, &mem_list->free) {
		tmp = list_entry(pos, frame_buffer_node_t, list);
		if (tmp != NULL) {
			length++;
		}
	}

	if (mem_list->free_request == true) {
		ret = rtos_set_semaphore(&mem_list->free_sem);

		if (ret != BK_OK) {
			LOGE("%s semaphore set failed: %d\n", __func__, ret);
		}

		mem_list->free_request = false;
	}


	if (!isr_context) {
		GLOBAL_INT_RESTORE();
		rtos_unlock_mutex(&mem_list->lock);
	}
}

void frame_buffer_fb_h264_push(frame_buffer_t *frame)
{
	fb_mem_list_t *mem_list = &fb_mem_list[FB_INDEX_H264];
	frame_buffer_node_t *tmp = NULL, *node = list_entry(frame, frame_buffer_node_t, frame);
	uint32_t isr_context = platform_is_in_interrupt_context();
	uint32_t length = 0;
	GLOBAL_INT_DECLARATION() = 0;
	bk_err_t ret;
	LIST_HEADER_T *pos, *n;

	if (!isr_context) {
		rtos_lock_mutex(&mem_list->lock);
		GLOBAL_INT_DISABLE();
	}

	list_add_tail(&node->list, &mem_list->ready);

	list_for_each_safe(pos, n, &mem_list->ready) {
		tmp = list_entry(pos, frame_buffer_node_t, list);
		if (tmp != NULL) {
			length++;
		}
	}

	//if (length == 1) {
		ret = rtos_set_semaphore(&mem_list->ready_sem);

		if (ret != BK_OK)
		{
			LOGD("%s semaphore set failed: %d\n", __func__, ret);
		}
	//}


	if (!isr_context) {
		GLOBAL_INT_RESTORE();
		rtos_unlock_mutex(&mem_list->lock);
	}
}

frame_buffer_t *frame_buffer_fb_h264_pop(void)
{
	frame_buffer_node_t *node = NULL, *tmp = NULL;
	fb_mem_list_t *mem_list = &fb_mem_list[FB_INDEX_H264];
	LIST_HEADER_T *pos, *n;
	GLOBAL_INT_DECLARATION() = 0;

	rtos_lock_mutex(&mem_list->lock);
	GLOBAL_INT_DISABLE();

	list_for_each_safe(pos, n, &mem_list->ready) {
		tmp = list_entry(pos, frame_buffer_node_t, list);
		if (tmp != NULL) {
			node = tmp;
			list_del(pos);
			break;
		}
	}

	GLOBAL_INT_RESTORE();
	rtos_unlock_mutex(&mem_list->lock);

	if (node == NULL) {
		LOGD("%s failed\n", __func__);
		return NULL;
	}

	return &node->frame;
}

frame_buffer_t *frame_buffer_fb_h264_pop_wait(void)
{
	frame_buffer_t *frame = NULL;
	fb_mem_list_t *mem_list = &fb_mem_list[FB_INDEX_H264];
	bk_err_t ret;

	do {
		frame = frame_buffer_fb_h264_pop();

		if (frame != NULL) {
			break;
		}

		ret = rtos_get_semaphore(&mem_list->ready_sem, 2000);

		if (ret != BK_OK) {
			LOGD("%s semaphore get failed: %d\n", __func__, ret);
			break;
		}

	} while (true);

	return frame;
}

void frame_buffer_init(void)
{
	uint32_t i = 0;

	for (i = 0; i < FB_INDEX_MAX; i++)
	{
		fb_mem_list[i].free.next = &fb_mem_list[i].free;
		fb_mem_list[i].free.prev = &fb_mem_list[i].free;
		fb_mem_list[i].ready.next = &fb_mem_list[i].ready;
		fb_mem_list[i].ready.prev = &fb_mem_list[i].ready;
		fb_mem_list[i].enable = false;
		rtos_init_mutex(&fb_mem_list[i].lock);
	}

	if (fb_info == NULL)
	{
		fb_info = (fb_info_t *)os_malloc(sizeof(fb_info_t));
		os_memset((void *)fb_info, 0, sizeof(fb_info_t));
		rtos_init_mutex(&fb_info->lock);
	}
}


void frame_buffer_deinit(void)
{
	if (fb_info)
	{
		os_free(fb_info);
		fb_info = NULL;
	}
}

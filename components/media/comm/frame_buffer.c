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


frame_buffer_info_t *frame_buffer_info = NULL;

LIST_HEADER_T jpeg_free_node_list = {&jpeg_free_node_list, &jpeg_free_node_list};
LIST_HEADER_T jpeg_ready_node_list = {&jpeg_ready_node_list, &jpeg_ready_node_list};

LIST_HEADER_T display_free_node_list = {&display_free_node_list, &display_free_node_list};
LIST_HEADER_T display_ready_node_list = {&display_ready_node_list, &display_ready_node_list};



void frame_buffer_free_frame(frame_buffer_t *buffer)
{
	LIST_HEADER_T *list = NULL;

	if (buffer->type == FRAME_JPEG)
	{
		list = &jpeg_free_node_list;
	}
	else if (buffer->type == FRAME_DISPLAY)
	{
		list = &display_free_node_list;
	}
	else
	{
		LOGE("%s unknow type: %d\n", __func__, buffer->type);
		return;
	}

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

	if (buffer->lock)
	{
		buffer->lock--;
	}
	else
	{
		frame_buffer_t *tmp = NULL;
		LIST_HEADER_T *pos, *n;

		list_for_each_safe(pos, n, list)
		{
			tmp = list_entry(pos, frame_buffer_t, list);
			if (tmp->frame == buffer->frame)
			{
				LOGW("%s refree\n", __func__);
				GLOBAL_INT_RESTORE();
				return;
			}
		}
	}

	LOGD("free: %p, lock: %d\n", buffer->frame, buffer->lock);

	if (buffer->lock == 0)
	{

		LOGD("free: %p\n");
		buffer->state = STATE_FRAMED;
		list_add_tail(&buffer->list, list);
	}

	GLOBAL_INT_RESTORE();
}


frame_buffer_t *frame_buffer_alloc(frame_type_t type)
{
	frame_buffer_t *frame = NULL, *tmp = NULL;
	LIST_HEADER_T *free_list = NULL, *ready_list = NULL;
	LIST_HEADER_T *pos, *n;

	if (type == FRAME_JPEG)
	{
		free_list = &jpeg_free_node_list;
		ready_list = &jpeg_ready_node_list;
	}
	else if (type == FRAME_DISPLAY)
	{
		free_list = &display_free_node_list;
		ready_list = &display_ready_node_list;
	}
	else
	{
		LOGE("%s unknow type: %d\n", __func__, type);
		return NULL;
	}

	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();

	list_for_each_safe(pos, n, free_list)
	{
		tmp = list_entry(pos, frame_buffer_t, list);
		if (tmp->state != STATE_ALLOCED)
		{
			frame = tmp;
			list_del(pos);
			break;
		}
	}

	if (frame == NULL)
	{
		list_for_each_safe(pos, n, ready_list)
		{
			tmp = list_entry(pos, frame_buffer_t, list);
			if (tmp->state != STATE_ALLOCED)
			{
				frame = tmp;
				list_del(pos);
				break;
			}
		}
	}

	if (frame != NULL)
	{
		frame->state = STATE_ALLOCED;
		frame->lock++;
	}

	LOGD("alloc: %p\n", frame->frame);

	GLOBAL_INT_RESTORE();

	return frame;
}


void frame_buffer_lock_frame(frame_buffer_t *frame)
{
	frame->lock++;
}

void frame_buffer_push_frame(frame_buffer_t *buffer)
{
	LIST_HEADER_T *list = NULL;

	if (buffer->type == FRAME_JPEG)
	{
		list = &jpeg_ready_node_list;
	}
	else if (buffer->type == FRAME_DISPLAY)
	{
		list = &display_ready_node_list;
	}
	else
	{
		LOGE("%s unknow type: %d\n", __func__, buffer->type);
		return;
	}

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

	buffer->lock--;
	buffer->state = STATE_FRAMED;
	list_add_tail(&buffer->list, list);

	GLOBAL_INT_RESTORE();

}


frame_buffer_t *frame_buffer_pop_frame(frame_type_t type)
{
	frame_buffer_t *frame = NULL, *tmp = NULL;
	LIST_HEADER_T *list = NULL;
	LIST_HEADER_T *pos, *n;

	if (type == FRAME_JPEG)
	{
		list = &jpeg_ready_node_list;
	}
	else if (type == FRAME_DISPLAY)
	{
		list = &display_ready_node_list;
	}
	else
	{
		LOGE("%s unknow type: %d\n", __func__, type);
		return NULL;
	}

	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();

	list_for_each_safe(pos, n, list)
	{
		tmp = list_entry(pos, frame_buffer_t, list);
		if (tmp->state == STATE_FRAMED)
		{
			frame = tmp;
			list_del(pos);
			break;
		}
	}

	frame->state = STATE_ALLOCED;
	frame->lock++;


	GLOBAL_INT_RESTORE();

	if (frame == NULL)
	{
		LOGE("%s get frame failed: %d\n", __func__, type);
	}

	return frame;
}


void frame_buffer_complete_notify(frame_buffer_t *buffer)
{
	if (buffer->type == FRAME_JPEG)
	{
		if (true == frame_buffer_info->wifi_register
		    && (!frame_buffer_info->wifi_lock)
		    && frame_buffer_info->wifi_comp_cb)
		{
			frame_buffer_lock_frame(buffer);
			frame_buffer_info->wifi_lock = true;
			frame_buffer_info->wifi_comp_cb(buffer);
		}

#ifdef CONFIG_LCD

		if (true == frame_buffer_info->decoder_register
		    && (!frame_buffer_info->decoder_lock)
		    && frame_buffer_info->decoder_comp_cb)
		{
			frame_buffer_lock_frame(buffer);
			frame_buffer_info->decoder_lock = true;
			frame_buffer_info->decoder_comp_cb(buffer);
			//LOGD("lcd alloc %p %d:%d:%d\n", buffer, buffer->id, buffer->state, buffer->lock);
			//lcd_frame_complete_notify(buffer);
		}
#endif

		if (true == frame_buffer_info->capture_register
		    && (!frame_buffer_info->capture_lock)
		    && frame_buffer_info->capture_comp_cb)
		{
			frame_buffer_lock_frame(buffer);
			frame_buffer_info->capture_lock = true;
			frame_buffer_info->capture_comp_cb(buffer);
			//storage_capture_frame_notify(buffer);
		}
	}

	if (buffer->type == FRAME_DISPLAY)
	{
#ifdef CONFIG_LCD

		if (true == frame_buffer_info->display_register
		    && (!frame_buffer_info->display_lock)
		    && frame_buffer_info->display_comp_cb)
		{
			frame_buffer_lock_frame(buffer);
			//frame_buffer_info->display_lock = true;
			frame_buffer_info->display_comp_cb(buffer);
		}

#endif
	}

	frame_buffer_free_frame(buffer);
}

bool is_workflow_freezing(frame_type_t type)
{
	bool ret = true;

	LOGD("WIFI %d:%d, DEC %d:%d\n",
	     frame_buffer_info->wifi_register, frame_buffer_info->wifi_lock,
	     frame_buffer_info->decoder_register, frame_buffer_info->decoder_lock);

	if (type == FRAME_JPEG)
	{
		if (true == frame_buffer_info->wifi_register
		    && (!frame_buffer_info->wifi_lock)
		    && frame_buffer_info->wifi_comp_cb)
		{
			ret = false;
		}

		if (true == frame_buffer_info->decoder_register
		    && (!frame_buffer_info->decoder_lock)
		    && frame_buffer_info->decoder_comp_cb)
		{
			ret = false;
		}

		if (true == frame_buffer_info->capture_register
		    && (!frame_buffer_info->capture_lock)
		    && frame_buffer_info->capture_comp_cb)
		{
			ret = false;
		}
	}

	if (type == FRAME_DISPLAY)
	{
		if (true == frame_buffer_info->display_register
		    && (!frame_buffer_info->display_lock)
		    && frame_buffer_info->display_comp_cb)
		{
			ret = false;
		}
	}

	return ret;
}

void frame_buffer_generate_complete(frame_buffer_t *buffer, frame_type_t type)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

	frame_buffer_push_frame(buffer);

	if (!is_workflow_freezing(type))
	{
		LOGD("notify frame[%d]: %u complete, %d\n", buffer->id, buffer->sequence, type);

		frame_buffer_t *frame = frame_buffer_pop_frame(type);
		frame_buffer_complete_notify(frame);
	}
	else
	{
		LOGD("covery frame[%d]: %u complete, %d\n", buffer->id, buffer->sequence, type);
	}

	GLOBAL_INT_RESTORE();
}

void frame_buffer_frame_register(frame_module_t module, void *callback)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

	switch (module)
	{
		case MODULE_WIFI:
			frame_buffer_info->wifi_register = true;
			frame_buffer_info->wifi_comp_cb = callback;
			break;
		case MODULE_DECODER:
			frame_buffer_info->decoder_register = true;
			frame_buffer_info->decoder_comp_cb = callback;
			break;
		case MODULE_RECODER:
			frame_buffer_info->recoder_register = true;
			frame_buffer_info->recoder_comp_cb = callback;
			break;
		case MODULE_CAPTURE:
			frame_buffer_info->capture_register = true;
			frame_buffer_info->capture_lock = false;
			frame_buffer_info->capture_comp_cb = callback;
			break;
		case MODULE_DISPLAY:
			frame_buffer_info->display_register = true;
			frame_buffer_info->display_comp_cb = callback;
			break;
	}

	GLOBAL_INT_RESTORE();
}

void frame_buffer_frame_deregister(frame_module_t module)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

	switch (module)
	{
		case MODULE_WIFI:
			frame_buffer_info->wifi_register = false;
			frame_buffer_info->wifi_lock = false;
			frame_buffer_info->wifi_comp_cb = NULL;
			break;
		case MODULE_DECODER:
			frame_buffer_info->decoder_register = false;
			frame_buffer_info->decoder_lock = false;
			frame_buffer_info->decoder_comp_cb = NULL;
			break;
		case MODULE_RECODER:
			frame_buffer_info->recoder_register = false;
			frame_buffer_info->recoder_lock = false;
			frame_buffer_info->recoder_comp_cb = NULL;
			break;
		case MODULE_CAPTURE:
			frame_buffer_info->capture_register = false;
			frame_buffer_info->capture_lock = false;
			frame_buffer_info->capture_comp_cb = NULL;
			break;
		case MODULE_DISPLAY:
			frame_buffer_info->capture_register = false;
			frame_buffer_info->capture_lock = false;
			frame_buffer_info->capture_comp_cb = NULL;
			break;
	}

	GLOBAL_INT_RESTORE();
}


void frame_buffer_free_request(frame_buffer_t *buffer, frame_module_t module)
{
	GLOBAL_INT_DECLARATION();

	if (buffer->lock == 0)
	{
		LOGE("%s invalid frame free\n", __func__);
		return;
	}

	frame_buffer_free_frame(buffer);

	GLOBAL_INT_DISABLE();

	switch (module)
	{
		case MODULE_WIFI:
			frame_buffer_info->wifi_lock = false;
			LOGD("wifi free %p %d:%d:%d:%d\n", buffer, buffer->id, buffer->state, buffer->lock, buffer->sequence);
			break;
		case MODULE_DECODER:
			frame_buffer_info->decoder_lock = false;
			LOGD("lcd free %p %d:%d:%d:%d\n", buffer, buffer->id, buffer->state, buffer->lock, buffer->sequence);
			break;
		case MODULE_RECODER:
			break;
		case MODULE_CAPTURE:
			LOGD("capture free %p %d:%d:%d:%d\n", buffer, buffer->id, buffer->state, buffer->lock, buffer->sequence);
			break;
		case MODULE_DISPLAY:
			frame_buffer_info->display_lock = false;
			LOGD("lcd free %p %d:%d:%d:%d\n", buffer, buffer->id, buffer->state, buffer->lock, buffer->sequence);
			break;
	}

	GLOBAL_INT_RESTORE();
}

int frame_buffer_jpeg_frame_init(void)
{
#ifdef CONFIG_PSRAM
	frame_buffer_t *tmp = NULL;
	LIST_HEADER_T *pos, *n;

	if (!list_empty(&jpeg_free_node_list))
	{
		list_for_each_safe(pos, n, &jpeg_free_node_list)
		{
			tmp = list_entry(pos, frame_buffer_t, list);
			if (tmp != NULL)
			{
				list_del(pos);
				os_free(tmp);
				break;
			}
		}

		INIT_LIST_HEAD(&jpeg_free_node_list);
	}

	if (!list_empty(&jpeg_ready_node_list))
	{
		list_for_each_safe(pos, n, &jpeg_ready_node_list)
		{
			tmp = list_entry(pos, frame_buffer_t, list);
			if (tmp != NULL)
			{
				list_del(pos);
				os_free(tmp);
				break;
			}
		}

		INIT_LIST_HEAD(&jpeg_ready_node_list);
	}

	if (frame_buffer_info->minimal_layout == true)
	{
		for (int i = 0; i < JPEG_ENC_FRAME_COUNT; i ++)
		{
			frame_buffer_t *frame = (frame_buffer_t *)os_malloc(sizeof(frame_buffer_t));

			os_memset(frame, 0, sizeof(frame_buffer_t));

			frame->state = STATE_INVALID;
			frame->frame = psram_map->jpeg_enc[i];
			frame->size = sizeof(psram_map->jpeg_enc[i]);
			frame->id = i;
			frame->type = FRAME_JPEG;
			frame->length = 0;
			frame->sequence = 0;
			frame->lock = 0;
			list_add_tail(&frame->list, &jpeg_free_node_list);
		}
	}
	else
	{
		for (int i = 0; i < JPEG_ENC_FRAME_COUNT; i ++)
		{
			frame_buffer_t *frame = (frame_buffer_t *)os_malloc(sizeof(frame_buffer_t));

			os_memset(frame, 0, sizeof(frame_buffer_t));

			frame->state = STATE_INVALID;
			frame->frame = psram_720p_map->jpeg_enc[i];
			frame->size = sizeof(psram_720p_map->jpeg_enc[i]);
			frame->id = i;
			frame->type = FRAME_JPEG;
			frame->length = 0;
			frame->sequence = 0;
			frame->lock = 0;
			list_add_tail(&frame->list, &jpeg_free_node_list);
		}
	}

#endif
	return BK_OK;
}

int frame_buffer_display_frame_init(void)
{
#ifdef CONFIG_PSRAM

	frame_buffer_t *tmp = NULL;
	LIST_HEADER_T *pos, *n;

	if (!list_empty(&display_free_node_list))
	{
		list_for_each_safe(pos, n, &display_free_node_list)
		{
			tmp = list_entry(pos, frame_buffer_t, list);
			if (tmp != NULL)
			{
				list_del(pos);
				os_free(tmp);
				break;
			}
		}

		INIT_LIST_HEAD(&display_free_node_list);
	}

	if (!list_empty(&display_ready_node_list))
	{
		list_for_each_safe(pos, n, &display_ready_node_list)
		{
			tmp = list_entry(pos, frame_buffer_t, list);
			if (tmp != NULL)
			{
				list_del(pos);
				os_free(tmp);
				break;
			}
		}

		INIT_LIST_HEAD(&display_ready_node_list);
	}

	if (frame_buffer_info->minimal_layout)
	{
		for (int i = 0; i < DISPLAY_FRAME_COUNT; i++)
		{
			frame_buffer_t *frame = (frame_buffer_t *)os_malloc(sizeof(frame_buffer_t));

			os_memset(frame, 0, sizeof(frame_buffer_t));

			frame->state = STATE_INVALID;
			frame->frame = psram_map->display[i];
			frame->size = sizeof(psram_map->display[i]);
			frame->id = i;
			frame->type = FRAME_DISPLAY;
			frame->length = 0;
			frame->sequence = 0;
			frame->lock = 0;
			list_add_tail(&frame->list, &display_free_node_list);
		}
	}
	else
	{
		for (int i = 0; i < DISPLAY_720P_FRAME_COUNT; i ++)
		{
			frame_buffer_t *frame = (frame_buffer_t *)os_malloc(sizeof(frame_buffer_t));

			os_memset(frame, 0, sizeof(frame_buffer_t));

			frame->state = STATE_INVALID;
			frame->frame = psram_720p_map->display[i];
			frame->size = sizeof(psram_720p_map->display[i]);
			frame->id = i;
			frame->type = FRAME_DISPLAY;
			frame->length = 0;
			frame->sequence = 0;
			frame->lock = 0;
			list_add_tail(&frame->list, &display_free_node_list);
		}
	}

#endif
	return BK_OK;
}


void frame_buffer_init(void)
{
	if (frame_buffer_info == NULL)
	{
		frame_buffer_info = (frame_buffer_info_t *)os_malloc(sizeof(frame_buffer_info_t));
		os_memset((void *)frame_buffer_info, 0, sizeof(frame_buffer_info_t));
	}

	frame_buffer_info->minimal_layout = true;
}

int frame_buffer_set_ppi(media_ppi_t ppi)
{
	uint16 width, heigth;

	LOGI("%s, ppi %dX%d\n", __func__, ppi >> 16, ppi & 0xFFFF);


	if (frame_buffer_info == NULL)
	{
		LOGE("%s frame_buffer_info is NULL\n", __func__);
		return BK_FAIL;
	}

	width = ppi_to_pixel_x(ppi);
	heigth = ppi_to_pixel_y(ppi);

	if (width * heigth > PIXEL_800 * PIXEL_600)
	{
		LOGI("%s, 720P Memory Layout Set\n", __func__);
		frame_buffer_info->minimal_layout = false;
	}

	frame_buffer_jpeg_frame_init();

	return BK_OK;
}
bool frame_buffer_get_state(void)
{
	bool ret = false;

	if (frame_buffer_info)
	{
		ret = frame_buffer_info->enable;
	}

	return ret;
}

void frame_buffer_enable(bool enable)
{
	if (frame_buffer_info)
	{
		os_memset((void *)frame_buffer_info, 0, sizeof(frame_buffer_info_t));

		frame_buffer_info->enable = enable;
	}
}


void frame_buffer_deinit(void)
{
	if (frame_buffer_info)
	{
		os_free(frame_buffer_info);
		frame_buffer_info = NULL;
		frame_buffer_info->minimal_layout = true;
	}
}

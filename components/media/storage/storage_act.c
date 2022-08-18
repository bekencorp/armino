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
#include <stdio.h>

#include <driver/dma.h>

#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>


#include <driver/jpeg_enc.h>
#include <driver/jpeg_enc_types.h>
#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>

#include "frame_buffer.h"

#include "media_core.h"
#include "media_evt.h"
#include "storage_act.h"

#if (CONFIG_SDCARD_HOST)
#include "ff.h"
#include "diskio.h"
#endif

#define TAG "storage"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static beken_queue_t storage_task_queue = NULL;
static beken_thread_t storage_task_thread = NULL;

typedef struct
{
	uint8_t type;
	uint32_t data;
} storages_task_msg_t;


typedef enum
{
	STORAGE_TASK_DATA,
	STORAGE_TASK_EXIT,
} storage_task_evt_t;

storage_info_t storage_info;

char *capture_name = NULL;

bk_err_t storage_task_send_msg(uint8_t msg_type, uint32_t data)
{
	bk_err_t ret;
	storages_task_msg_t msg;

	if (storage_task_queue)
	{
		msg.type = msg_type;
		msg.data = data;

		ret = rtos_push_to_queue(&storage_task_queue, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
		{
			os_printf("video_transfer_cpu1_send_msg failed\r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}


void storage_task_stop(void)
{
	storage_task_send_msg(STORAGE_TASK_EXIT, 0);
	while (storage_task_thread)
	{
		rtos_delay_milliseconds(10);
	}
}

void storage_frame_buffer_dump(frame_buffer_t *frame, char *name)
{
	LOGI("%s dump frame: %d, %p, %u, size: %d\n", __func__, frame->id, frame->frame, frame->sequence, frame->length);

#if (CONFIG_SDCARD_HOST)
	FIL fp1;
	unsigned int uiTemp = 0;
	char file_name[50] = {0};

	sprintf(file_name, "%d:/No.%d_%s", DISK_NUMBER_SDIO_SD, frame->sequence, name);

	FRESULT fr = f_open(&fp1, file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK)
	{
		LOGE("can not open file: %s, error: %d\n", file_name, fr);
		return;
	}

	LOGI("open file:%s!\n", file_name);

	fr = f_write(&fp1, (char *)frame->frame, frame->length, &uiTemp);
	if (fr != FR_OK)
	{
		LOGE("f_write failed 1 fr = %d\r\n", fr);
	}

	f_close(&fp1);

	LOGI("%s, complete\n", __func__);
#endif
}


static void storage_capture_save(frame_buffer_t *frame)
{
	media_msg_t msg;

	LOGI("%s save frame: %d, size: %d", __func__, frame->id, frame->length);

#if (CONFIG_SDCARD_HOST)
	FIL fp1;
	unsigned int uiTemp = 0;
	char file_name[50] = {0};

	sprintf(file_name, "%d:/%s", DISK_NUMBER_SDIO_SD, capture_name);

	FRESULT fr = f_open(&fp1, file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK)
	{
		LOGE("can not open file: %s, error: %d\n", file_name, fr);
		goto error;
	}

	LOGI("open file:%s!\n", file_name);

	fr = f_write(&fp1, (char *)frame->frame, frame->length, &uiTemp);
	if (fr != FR_OK)
	{
		LOGE("f_write failed 1 fr = %d\r\n", fr);
	}

	f_close(&fp1);

	LOGI("%s, complete\n", __func__);

error:

#endif

	msg.event = EVENT_COM_FRAME_CAPTURE_FREE_IND;
	msg.param = (uint32_t)frame;
	media_send_msg(&msg);

	storage_info.capture_state = STORAGE_STATE_DISABLED;
}

static void storage_task_entry(beken_thread_arg_t data)
{
	bk_err_t ret = BK_OK;
	storages_task_msg_t msg;

	while (1)
	{
		ret = rtos_pop_from_queue(&storage_task_queue, &msg, BEKEN_WAIT_FOREVER);

		if (kNoErr == ret)
		{
			switch (msg.type)
			{
				case STORAGE_TASK_DATA:
					storage_capture_save((frame_buffer_t *)msg.data);
					break;

				case STORAGE_TASK_EXIT:
					goto exit;
				default:
					break;
			}
		}
	}

exit:

	rtos_deinit_queue(&storage_task_queue);
	storage_task_queue = NULL;

	storage_task_thread = NULL;
	rtos_delete_thread(NULL);
}

int storage_task_start(void)
{
	int ret;

	if (storage_task_queue == NULL)
	{
		ret = rtos_init_queue(&storage_task_queue, "trs_task_queue", sizeof(storages_task_msg_t), 60);

		if (kNoErr != ret)
		{
			LOGE("%s trs_task_queue init failed\n");
			goto error;
		}
	}

	if (storage_task_thread == NULL)
	{
		ret = rtos_create_thread(&storage_task_thread,
		                         4,
		                         "storage_task_thread",
		                         (beken_thread_function_t)storage_task_entry,
		                         4 * 1024,
		                         NULL);

		if (kNoErr != ret)
		{
			LOGE("%s trs_task_thread init failed\n");
			goto error;
		}
	}

	return kNoErr;

error:

	if (storage_task_queue)
	{
		rtos_deinit_queue(&storage_task_queue);
		storage_task_queue = NULL;
	}

	if (storage_task_thread)
	{
		storage_task_thread = NULL;
		rtos_delete_thread(NULL);
	}

	return kGeneralErr;
}


void storage_open_handle(void)
{
	LOGI("%s\n", __func__);

	LOGI("%s register camera init\n", __func__);
}

void storage_capture_frame_callback(frame_buffer_t *frame)
{
	storage_task_send_msg(STORAGE_TASK_DATA, (uint32_t)frame);
}


void storage_capture_handle(param_pak_t *param)
{
	LOGI("%s, %s\n", __func__, (char *)param->param);

	if (storage_info.capture_state == STORAGE_STATE_ENABLED)
	{
		LOGI("%s already capture\n", __func__);
		goto out;
	}

	if (capture_name == NULL)
	{
		capture_name = (char *)os_malloc(32);
	}

	os_memcpy(capture_name, (char *)param->param, 31);
	capture_name[31] = 0;

	frame_buffer_frame_register(MODULE_CAPTURE, storage_capture_frame_callback);

	storage_info.capture_state = STORAGE_STATE_ENABLED;

out:
	MEDIA_EVT_RETURN(param, kNoErr);
}

void storage_event_handle(uint32_t event, uint32_t param)
{
	switch (event)
	{
		case EVENT_STORAGE_OPEN_IND:
			storage_open_handle();
			break;

		case EVENT_STORAGE_CAPTURE_IND:
			storage_capture_handle((param_pak_t *)param);
			break;
	}
}

storage_state_t get_storage_state(void)
{
	return storage_info.state;
}

void set_storage_state(storage_state_t state)
{
	storage_info.state = state;
}

void storage_init(void)
{
	os_memset(&storage_info, 0, sizeof(storage_info_t));

	storage_info.state = STORAGE_STATE_DISABLED;
	storage_info.capture_state = STORAGE_STATE_DISABLED;
	storage_task_start();
}


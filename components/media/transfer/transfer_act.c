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
#include <components/video_transfer.h>

#include "media_core.h"
#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>

#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>
#include <driver/i2c.h>
#include <driver/jpeg_enc.h>
#include <driver/jpeg_enc_types.h>
#include <driver/uvc_camera.h>

#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>
#include <driver/media_types.h>

#include <soc/mapping.h>

#include <driver/timer.h>


#include "bk_general_dma.h"

#include "transfer_act.h"
#include "media_evt.h"

#include "frame_buffer.h"

#include "wlan_ui_pub.h"

#include "bk_misc.h"

#define TAG "transfer"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

typedef struct
{
	uint8_t type;
	uint32_t data;
} trs_task_msg_t;

typedef enum
{
	TRS_TRANSFER_DATA,
	TRS_TRANSFER_EXIT,
} trs_task_msg_type_t;

typedef struct
{
	uint8_t id;
	uint8_t eof;
	uint8_t cnt;
	uint8_t size;
	uint8_t data[];
} transfer_data_t;



transfer_info_t transfer_info;


#define MAX_TX_SIZE (1472)
#define MAX_COPY_SIZE (1472 - sizeof(transfer_data_t))
#define MAX_RETRY (10000)
#define RETRANSMITS_TIME (5)

static beken_queue_t trs_task_queue = NULL;
static beken_thread_t trs_task_thread = NULL;

extern media_debug_t *media_debug;

transfer_data_t *wifi_tranfer_data = NULL;

uint8_t frame_id = 0;
bool runing = false;


frame_buffer_t *wifi_tranfer_frame = NULL;


video_setup_t vido_transfer_info = {0};

uint32_t lost_size = 0;
uint32_t complete_size = 0;
uint32_t transfer_timer_us = 0; // unit us

extern void rwnxl_set_video_transfer_flag(uint32_t video_transfer_flag);

int dvp_frame_send(uint8_t *data, uint32_t size, uint32_t retry_max, uint32_t ms_time, uint32_t us_delay_time)
{
	int ret = BK_FAIL;

	if (!vido_transfer_info.send_func)
	{
		return ret;
	}

	do
	{
		ret = vido_transfer_info.send_func(data, size);

		if (ret == size)
		{
			//LOGI("size: %d\n", size);
			complete_size += size;
			delay_us(us_delay_time);
			break;
		}

		//LOGI("retry\n");
		lost_size += size;
		rtos_delay_milliseconds(ms_time);
	}
	while (retry_max-- && runing);

	return ret == size ? BK_OK : BK_FAIL;
}

static void dvp_frame_handle(frame_buffer_t *buffer)
{
	uint32_t i;
	uint32_t count = buffer->length / MAX_COPY_SIZE;
	uint32_t tail = buffer->length % MAX_COPY_SIZE;
	uint8_t id = frame_id++;
	int ret;

	uint32_t delay_time = (transfer_timer_us / (count + 1));
	if (delay_time > 500)
		delay_time -= 500;

	LOGD("id: %u, seq: %u, length: %u, size: %u\n", buffer->id, buffer->sequence, buffer->length, buffer->size);
	wifi_tranfer_data->id = id;
	wifi_tranfer_data->size = 0;
	wifi_tranfer_data->eof = 0;
	wifi_tranfer_data->cnt = 0;

	for (i = 0; i < count && runing; i++)
	{
		if ((tail == 0) && (i == count - 1))
		{
			wifi_tranfer_data->eof = 1;
			wifi_tranfer_data->cnt = count;
		}

		dma_memcpy(wifi_tranfer_data->data, buffer->frame + (MAX_COPY_SIZE * i), MAX_COPY_SIZE);

		ret = dvp_frame_send((uint8_t *)wifi_tranfer_data, MAX_TX_SIZE, MAX_RETRY, RETRANSMITS_TIME, delay_time);

		if (ret != BK_OK)
		{
			LOGE("send failed\n");
		}
	}

	if (tail)
	{
		wifi_tranfer_data->eof = 1;
		wifi_tranfer_data->cnt = count + 1;

		/* fix for psram 4bytes alignment */
		dma_memcpy(wifi_tranfer_data->data, buffer->frame + (MAX_COPY_SIZE * i), (tail % 4) ? ((tail / 4 + 1) * 4) : tail);

		ret = dvp_frame_send((uint8_t *)wifi_tranfer_data, tail + sizeof(transfer_data_t), MAX_RETRY, RETRANSMITS_TIME, delay_time);

		if (ret != BK_OK)
		{
			LOGE("send failed\n");
		}
	}

	media_debug->fps_wifi++;

	LOGD("seq: %u, length: %u, tail: %u, count: %u\n", id, buffer->length, tail, count);

	if (wifi_tranfer_frame)
	{
		media_msg_t msg;

		msg.event = EVENT_COM_FRAME_WIFI_FREE_IND;
		msg.param = (uint32_t)buffer;
		media_send_msg(&msg);
		wifi_tranfer_frame = NULL;
	}
}

void transfer_dump(uint32_t ms)
{
	uint32_t lost = lost_size, complete = complete_size, speed;
	lost_size = 0;
	complete_size = 0;

	if (transfer_info.state == TRS_STATE_DISABLED)
	{
		return;
	}

	lost = lost / 1024 / (ms / 1000);
	complete = complete / 1024 / (ms / 1000);
	speed = (complete * 8) / 1024;

	LOGI("Lost: %uKB/s, Complete: %uKB/s, Speed: %uMb/s\n", lost, complete, speed);
}


static void trs_task_entry(beken_thread_arg_t data)
{
	bk_err_t ret = BK_OK;
	trs_task_msg_t msg;

	dvp_camera_device_t *dvp_device = NULL;

	dvp_device = bk_dvp_camera_get_device();

	if (dvp_device == NULL || dvp_device->id == ID_UNKNOW)
	{
		LOGE("dvp camera was not init\n");
#if (CONFIG_USB_UVC)
		uvc_camera_device_t *uvc_device = NULL;

		uvc_device = bk_uvc_camera_get_device();

		if (uvc_device == NULL)
		{
			LOGE("uvc camera was not init\n");
			goto exit;
		}

		transfer_timer_us = 1000 * 1000 / uvc_device->fps; //us
		LOGI("transfer_timer_us: %d, fps:%d\r\n", transfer_timer_us, uvc_device->fps);
#else
		goto exit;
#endif
	}
	else
	{
		uint32_t fps = 20;
		switch (dvp_device->fps)
		{
			case FPS5:
				fps = 5;
				break;
			case FPS10:
				fps = 10;
				break;
			case FPS15:
				fps = 15;
				break;
			case FPS20:
				fps = 20;
				break;
			case FPS25:
				fps = 25;
				break;
			case FPS30:
				fps = 30;
				break;
		}

		transfer_timer_us = 1000 * 1000 / fps - 1000;//us
		LOGI("transfer_timer_us: %d, fps:%d\r\n", transfer_timer_us, fps);
	}

	while (1)
	{
		ret = rtos_pop_from_queue(&trs_task_queue, &msg, BEKEN_WAIT_FOREVER);

		if (BK_OK == ret)
		{
			switch (msg.type)
			{
				case TRS_TRANSFER_DATA:
					if (false == frame_buffer_get_state() && runing)
					{
						break;
					}
					dvp_frame_handle((frame_buffer_t *)msg.data);
					break;

				case TRS_TRANSFER_EXIT:
					goto exit;
				default:
					break;
			}
		}
	}

exit:

	if (wifi_tranfer_data != NULL)
	{
		os_free(wifi_tranfer_data);
		wifi_tranfer_data = NULL;
	}

	frame_id = 0;

	rtos_deinit_queue(&trs_task_queue);
	trs_task_queue = NULL;

	trs_task_thread = NULL;
	rtos_delete_thread(NULL);
}

int transfer_task_start(video_setup_t *setup_cfg)
{
	int ret;

	os_memcpy(&vido_transfer_info, setup_cfg, sizeof(video_setup_t));
	media_debug->fps_wifi = 0;

	runing = true;

	if (wifi_tranfer_data == NULL)
	{
		wifi_tranfer_data = (transfer_data_t *) os_malloc(MAX_TX_SIZE);
	}

	frame_id = 0;

	if (trs_task_queue == NULL)
	{
		ret = rtos_init_queue(&trs_task_queue, "trs_task_queue", sizeof(trs_task_msg_t), 60);

		if (BK_OK != ret)
		{
			LOGE("%s trs_task_queue init failed\n");
			goto error;
		}
	}

	if (trs_task_thread == NULL)
	{
		ret = rtos_create_thread(&trs_task_thread,
		                         4,
		                         "trs_task_thread",
		                         (beken_thread_function_t)trs_task_entry,
		                         4 * 1024,
		                         NULL);

		if (BK_OK != ret)
		{
			LOGE("%s trs_task_thread init failed\n");
			goto error;
		}
	}

	return BK_OK;

error:

	if (wifi_tranfer_frame)
	{
		media_msg_t msg;

		msg.event = EVENT_COM_FRAME_WIFI_FREE_IND;
		msg.param = (uint32_t)wifi_tranfer_frame;
		media_send_msg(&msg);
		wifi_tranfer_frame = NULL;
	}

	if (trs_task_queue)
	{
		rtos_deinit_queue(&trs_task_queue);
		trs_task_queue = NULL;
	}

	if (trs_task_thread)
	{
		trs_task_queue = NULL;
		rtos_delete_thread(NULL);
	}

	return BK_FAIL;
}

bk_err_t transfer_task_send_msg(uint8_t msg_type, uint32_t data)
{
	bk_err_t ret;
	trs_task_msg_t msg;

	if (trs_task_queue)
	{
		msg.type = msg_type;
		msg.data = data;

		ret = rtos_push_to_queue(&trs_task_queue, &msg, BEKEN_NO_WAIT);
		if (BK_OK != ret)
		{
			LOGE("video_transfer_cpu1_send_msg failed\r\n");
			return BK_FAIL;
		}

		return ret;
	}
	return kNoResourcesErr;
}


void transfer_task_stop(void)
{
	runing = false;
	transfer_task_send_msg(TRS_TRANSFER_EXIT, 0);
	while (trs_task_thread)
	{
		rtos_delay_milliseconds(10);
	}
}

frame_buffer_t *get_wifi_transfer_frame(void)
{
	return wifi_tranfer_frame;
}


void transfer_frame_complete_callback(frame_buffer_t *buffer)
{
	LOGD("%s\n", __func__);
	wifi_tranfer_frame = buffer;

	if (transfer_info.pause)
	{
		frame_buffer_free_request(buffer, MODULE_WIFI);
		return;
	}

	transfer_task_send_msg(TRS_TRANSFER_DATA, (uint32_t)buffer);
}


void transfer_open_handle(param_pak_t *param)
{
	video_setup_t *setup_cfg = (video_setup_t *)param->param;

	LOGI("%s ++\n", __func__);

	frame_buffer_frame_register(MODULE_WIFI, transfer_frame_complete_callback);

	transfer_task_start(setup_cfg);

	set_transfer_state(TRS_STATE_ENABLED);

	MEDIA_EVT_RETURN(param, BK_OK);

	LOGI("%s --\n", __func__);
}

void transfer_close_handle(param_pak_t *param)
{
	LOGI("%s\n", __func__);

	wifi_tranfer_frame = NULL;

	transfer_task_stop();
	set_transfer_state(TRS_STATE_DISABLED);

	frame_buffer_frame_deregister(MODULE_WIFI);

	rwnxl_set_video_transfer_flag(false);

	bk_wlan_ps_enable();

	MEDIA_EVT_RETURN(param, BK_OK);
}

void transfer_pause_handle(param_pak_t *param)
{
	LOGI("%s, %d\n", __func__, param->param);

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

	transfer_info.pause = param->param;

	GLOBAL_INT_RESTORE();

	MEDIA_EVT_RETURN(param, BK_OK);
}


void transfer_event_handle(uint32_t event, uint32_t param)
{
	switch (event)
	{
		case EVENT_TRANSFER_OPEN_IND:
			transfer_open_handle((param_pak_t *)param);
			break;
		case EVENT_TRANSFER_CLOSE_IND:
			transfer_close_handle((param_pak_t *)param);
			break;
		case EVENT_TRANSFER_PAUSE_IND:
			transfer_pause_handle((param_pak_t *)param);
			break;
	}
}

trs_state_t get_transfer_state(void)
{
	return transfer_info.state;
}

void set_transfer_state(trs_state_t state)
{
	transfer_info.state = state;
}

void transfer_init(void)
{
	transfer_info.state = TRS_STATE_DISABLED;
	transfer_info.debug = false;
	transfer_info.pause = false;
}

// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <os/os.h>
#include <os/mem.h>
#include <components/log.h>

#include "media_core.h"
#include "media_evt.h"
#include "camera_act.h"
#include "lcd_act.h"
#include "storage_act.h"
#include "transfer_act.h"
#include "frame_buffer.h"
#include "media_mailbox_list_util.h"

#include <driver/timer.h>


#define TAG "media_ui"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define DEBUG_INTERVAL (4)

media_debug_t *media_debug = NULL;
media_debug_t *media_debug_cached = NULL;

static beken_thread_t media_ui_task = NULL;
static beken_queue_t media_ui_msg_que = NULL;

bk_err_t media_send_msg(media_msg_t *msg)
{
	int ret = kNoErr;

	if (media_ui_msg_que)
	{
		ret = rtos_push_to_queue(&media_ui_msg_que, msg, BEKEN_NO_WAIT);

		if (kNoErr != ret)
		{
			LOGE("%s failed\n", __func__);
			return kOverrunErr;
		}

		return ret;
	}

	return kNoResourcesErr;
}

static void media_debug_dump(void)
{
	if (get_camera_state() == CAMERA_STATE_DISABLED && get_lcd_state() == LCD_STATE_DISABLED)
		return;

	uint16_t jpg = (media_debug->isr_jpeg - media_debug_cached->isr_jpeg) / DEBUG_INTERVAL;
	uint16_t dec = (media_debug->isr_decoder - media_debug_cached->isr_decoder) / DEBUG_INTERVAL;
	uint16_t lcd = (media_debug->isr_lcd - media_debug_cached->isr_lcd) / DEBUG_INTERVAL;
	uint16_t fps = (media_debug->fps_lcd - media_debug_cached->fps_lcd) / DEBUG_INTERVAL;
	uint16_t wifi = (media_debug->fps_wifi - media_debug_cached->fps_wifi) / DEBUG_INTERVAL;
	uint16_t err_dec = (media_debug->err_dec - media_debug_cached->err_dec) / DEBUG_INTERVAL;

	media_debug_cached->isr_jpeg = media_debug->isr_jpeg;
	media_debug_cached->isr_decoder = media_debug->isr_decoder;
	media_debug_cached->isr_lcd = media_debug->isr_lcd;
	media_debug_cached->fps_lcd = media_debug->fps_lcd;
	media_debug_cached->fps_wifi = media_debug->fps_wifi;
	media_debug_cached->err_dec = media_debug->err_dec;

	if (jpg > 30 || jpg == 0)
	{
		media_debug->transfer_timer_us = 50000;
	}
	else
	{
		media_debug->transfer_timer_us = 1000000 / jpg;
	}

	LOGI("jpg: %d[%d, %d], dec: %d[%d, %d], lcd: %d[%d], fps: %d[%d], wifi: %d[%d], wifi_read: [%d]\n",
	     jpg, media_debug->isr_jpeg, media_debug->psram_busy,
	     dec, media_debug->isr_decoder, err_dec,
	     lcd, media_debug->isr_lcd,
	     fps, media_debug->fps_lcd,
	     wifi, media_debug->fps_wifi, media_debug->wifi_read);

	/*if ((jpg == 0) && (camera_info.type == MEDIA_DVP_MJPEG)
	{
		if (CAMERA_STATE_DISABLED == get_camera_state())
		{
			// have been closed by up layer
			return;
		}

		media_mailbox_msg_t *node = NULL;

		node = (media_mailbox_msg_t *)os_malloc(sizeof(media_mailbox_msg_t));
		if (node != NULL)
		{
			node->event = EVENT_CAM_UVC_RESET_IND;
			node->param = state;
		}
		else
		{
			LOGW("%s\n", __func__);
			return;
		}

		media_msg_t msg;

		msg.event = EVENT_CAM_DVP_RESET_OPEN_IND;
		media_msg.event = EVENT_CAM_UVC_RESET_IND;
		media_msg.param = (uint32_t)node;

		media_send_msg(&msg);

	}*/
}


static void media_ui_timer_debug_handle(timer_id_t timer_id)
{
	media_msg_t msg;
	msg.event = EVENT_MEDIA_DEBUG_IND;
	msg.param = 0;
	media_send_msg(&msg);
}

static void media_ui_task_main(beken_thread_arg_t data)
{
	int ret = kNoErr;

	while (1)
	{
		media_msg_t msg;
		media_mailbox_msg_t *mb_msg = NULL;

		ret = rtos_pop_from_queue(&media_ui_msg_que, &msg, BEKEN_WAIT_FOREVER);

		if (kNoErr == ret)
		{
			switch (msg.event >> MEDIA_EVT_BIT)
			{
//				case COM_EVENT:
//					//comm_event_handle(msg.event, msg.param);
//					break;

#if (defined(CONFIG_DVP_CAMERA) || defined(CONFIG_USB_UVC))
				case CAM_EVENT:
					mb_msg = (media_mailbox_msg_t *)msg.param;
					camera_event_handle(mb_msg);
					break;
#endif

//#ifdef CONFIG_AUDIO
//				case AUD_EVENT:
//					audio_event_handle(msg.event, msg.param);
//					break;
//#endif

#ifdef CONFIG_LCD
				case LCD_EVENT:
					mb_msg = (media_mailbox_msg_t *)msg.param;
					lcd_event_handle(mb_msg);
					break;
#endif

#ifdef CONFIG_LVGL
				case LVGL_EVENT:
					mb_msg = (media_mailbox_msg_t *)msg.param;
					extern void lvgl_event_handle(media_mailbox_msg_t *msg);
					lvgl_event_handle(mb_msg);
					break;
#endif

#ifdef CONFIG_WIFI_TRANSFER
				case TRS_EVENT:
					mb_msg = (media_mailbox_msg_t *)msg.param;
					transfer_major_event_handle(mb_msg);
					break;
#endif

#ifdef CONFIG_IMAGE_STORAGE
				case STORAGE_EVENT:
					mb_msg = (media_mailbox_msg_t *)msg.param;
					storage_major_event_handle(mb_msg);
					break;
#endif

#ifdef CONFIG_USB_TRANSFER
				case USB_TRS_EVENT:
					mb_msg = (media_mailbox_msg_t *)msg.param;
					usb_major_event_handle(mb_msg);
					break;
#endif

//#ifdef CONFIG_DUAL_CORE
//				case MAILBOX_EVT:
//					mailbox_evt_handle(msg.event, msg.param);
//					break;
//#endif
				case QUEUE_EVENT:

					break;

				case MEDIA_DEBUG:
					media_debug_dump();
					break;

				case EXIT_EVENT:
					goto exit;
					break;

				default:
					break;
			}
		}
	}

exit:
	bk_timer_stop(TIMER_ID1);

	if (media_debug)
	{
		os_free(media_debug);
		media_debug = NULL;
	}

	if (media_debug_cached)
	{
		os_free(media_debug_cached);
		media_debug_cached = NULL;
	}

	rtos_deinit_queue(&media_ui_msg_que);
	media_ui_msg_que = NULL;

	media_ui_task = NULL;
	rtos_delete_thread(NULL);
}


bk_err_t media_ui_task_init(void)
{
	int ret = kNoErr;

	if ((!media_ui_task) && (!media_ui_msg_que))
	{
		ret = rtos_init_queue(&media_ui_msg_que,
								"media_ui_msg_que",
								sizeof(media_msg_t),
								20);
		if (kNoErr != ret)
		{
			LOGE("media_ui_msg_que init failed\n");
			ret = kNoMemoryErr;
			goto error;
		}

		ret = rtos_create_thread(&media_ui_task,
								BEKEN_DEFAULT_WORKER_PRIORITY,
								"media_ui_task",
								(beken_thread_function_t)media_ui_task_main,
								2 * 1024,
								NULL);

		if (kNoErr != ret)
		{
			LOGE("media_ui_task init failed\n");
			ret = kNoMemoryErr;
			goto error;
		}
	}

	bk_timer_start(TIMER_ID1, DEBUG_INTERVAL * 1000, media_ui_timer_debug_handle);

	return ret;

error:

	bk_timer_stop(TIMER_ID1);

	if (media_ui_msg_que)
	{
		rtos_deinit_queue(&media_ui_msg_que);
		media_ui_msg_que = NULL;
	}

	if (media_ui_task)
	{
		media_ui_task = NULL;
	}

	return ret;
}


void media_init()
{
	frame_buffer_init();

#if (defined(CONFIG_DVP_CAMERA) || defined(CONFIG_USB_UVC))
	camera_init();
#endif

#ifdef CONFIG_IMAGE_STORAGE
	storage_init();
#endif

#ifdef CONFIG_WIFI_TRANSFER
	transfer_init();
#endif

#ifdef CONFIG_LCD
	lcd_init();
#endif

	if (media_debug == NULL)
	{
		media_debug = (media_debug_t *)os_malloc(sizeof(media_debug_t));

		if (media_debug == NULL)
		{
			LOGE("malloc media_debug fail\n");
		}
	}

	if (media_debug_cached == NULL)
	{
		media_debug_cached = (media_debug_t *)os_malloc(sizeof(media_debug_t));
		if (media_debug_cached == NULL)
		{
			LOGE("malloc media_debug_cached fail\n");
		}
	}

	os_memset(media_debug, 0, sizeof(media_debug_t));
	os_memset(media_debug_cached, 0, sizeof(media_debug_t));

	media_ui_task_init();
}

void media_deinit()
{
	media_msg_t msg;

	msg.event = EXIT_EVENT << MEDIA_EVT_BIT;

	media_send_msg(&msg);

	frame_buffer_deinit();
	if (media_debug)
	{
		os_free(media_debug);
		media_debug = NULL;
	}

	if (media_debug_cached)
	{
		os_free(media_debug_cached);
		media_debug_cached = NULL;
	}
}


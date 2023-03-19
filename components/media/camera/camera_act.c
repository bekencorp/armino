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
#include "camera_act.h"
#include "media_evt.h"
#include "storage_act.h"
#include "media_app.h"
#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>
#include <driver/i2c.h>
#include <driver/jpeg_enc.h>
#include <driver/jpeg_enc_types.h>

#include "camera.h"
#include "frame_buffer.h"


#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>

#include <modules/ble.h>

#include <driver/timer.h>

#define TAG "cam_act"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define DEBUG_INTERVAL (1000 * 2)

typedef void (*camera_connect_state_t)(uint8_t state);

extern void transfer_dump(uint32_t ms);

extern media_debug_t *media_debug;
extern media_debug_t *media_debug_cached;

camera_info_t camera_info;
bool dvp_camera_reset_open_ind = false;
beken_semaphore_t camera_act_sema = NULL;

static beken_thread_t disc_task = NULL;

camera_connect_state_t camera_connect_state_change_cb = NULL;

static void camera_debug_dump(timer_id_t timer_id)
{
	transfer_dump(DEBUG_INTERVAL);

	uint16_t jpg = (media_debug->isr_jpeg - media_debug_cached->isr_jpeg) / 2;
	uint16_t dec = (media_debug->isr_decoder - media_debug_cached->isr_decoder) / 2;
	uint16_t lcd = (media_debug->isr_lcd - media_debug_cached->isr_lcd) / 2;
	uint16_t fps = (media_debug->fps_lcd - media_debug_cached->fps_lcd) / 2;
	uint16_t wifi = (media_debug->fps_wifi - media_debug_cached->fps_wifi) / 2;
	uint16_t err_dec = (media_debug->err_dec - media_debug_cached->err_dec) / 2;

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

	if ((jpg == 0) && (camera_info.type == MEDIA_DVP_MJPEG || camera_info.type == MEDIA_DVP_MIX))
	{
		if (CAMERA_STATE_DISABLED == get_camera_state())
		{
			// have been closed by up layer
			return;
		}

		media_msg_t msg;

		msg.event = EVENT_CAM_DVP_RESET_OPEN_IND;
		msg.param = camera_info.param;

		media_send_msg(&msg);

	}
}

void camera_dvp_reset_open_handle(uint32_t param)
{
#ifdef CONFIG_CAMERA

	bk_err_t ret = kNoErr;

	if (dvp_camera_reset_open_ind)
	{
		bk_dvp_camera_close();
		dvp_camera_reset_open_ind = false;
	}

	ret = bk_dvp_camera_open(param, camera_info.type);
	if (ret != kNoErr)
	{
		dvp_camera_reset_open_ind = false;
	}
	else
	{
		dvp_camera_reset_open_ind = true;
	}
#endif
}

void camera_dvp_open_handle(param_pak_t *param, media_camera_type_t type)
{
	int ret = 0;

	LOGI("%s\n", __func__);
#ifdef CONFIG_CAMERA

	if (CAMERA_STATE_DISABLED != get_camera_state())
	{
		LOGI("%s already opened\n", __func__);
		ret = kNoErr;
		goto out;
	}

	camera_info.param = param->param;
	camera_info.type = type;

	ret = bk_dvp_camera_open(param->param, type);


	if (ret != kNoErr)
	{
		LOGE("%s open failed\n", __func__);
		goto out;
	}

	dvp_camera_reset_open_ind = true;

	set_camera_state(CAMERA_STATE_ENABLED);

	if (camera_info.debug)
	{
		bk_timer_start(TIMER_ID1, DEBUG_INTERVAL, camera_debug_dump);
	}

out:
#endif

	MEDIA_EVT_RETURN(param, ret);
}

void camera_dvp_close_handle(param_pak_t *param)
{
	int ret = 0;

	LOGI("%s\n", __func__);

#ifdef CONFIG_CAMERA

	if (CAMERA_STATE_DISABLED == get_camera_state())
	{
		LOGI("%s already close\n", __func__);
		ret = kNoErr;
		goto out;
	}

	bk_dvp_camera_close();

	set_camera_state(CAMERA_STATE_DISABLED);

	dvp_camera_reset_open_ind = false;

	if (camera_info.debug)
	{
		bk_timer_stop(TIMER_ID1);
	}

out:
#endif

	MEDIA_EVT_RETURN(param, ret);
}

void camera_dvp_drop_frame(uint32_t param)
{
#ifdef CONFIG_CAMERA

	if (param)
	{
		bk_jpeg_enc_set_gpio_enable(0, JPEG_GPIO_HSYNC_DATA);
	}
	else
	{
		bk_jpeg_enc_set_gpio_enable(1, JPEG_GPIO_HSYNC_DATA);
	}
#endif
}
//#endif

void uvc_ble_notice_cb(ble_notice_t notice, void *param)
{
	switch (notice) {
	case BLE_5_SHUTDOWN_SUCCEED:
		LOGI("ble shutdown ok\n");
		rtos_set_semaphore(&camera_act_sema);
		break;
	default:
		break;
	}
}

static void camera_uvc_connect_state_change_task_entry(beken_thread_arg_t data)
{
	uint8_t state = *(uint8_t *)data;

	LOGI("%s, state:%d\r\n", __func__, state);

	if (camera_connect_state_change_cb)
	{
		camera_connect_state_change_cb(state);
	}
	else
	{
		// restart by self
		if (CAMERA_STATE_ENABLED != get_camera_state())
		{
			LOGI("%s, state:%d\r\n", __func__, state);
			if (state == UVC_CONNECTED)
			{
				int ret = bk_uvc_camera_open(camera_info.param, camera_info.type);

				if (ret != kNoErr)
				{
					LOGE("%s open failed\n", __func__);
					return;
				}

				set_camera_state(CAMERA_STATE_ENABLED);

				if (camera_info.debug)
				{
					bk_timer_start(TIMER_ID1, DEBUG_INTERVAL, camera_debug_dump);
				}
			}
		}
	}

	disc_task = NULL;
	rtos_delete_thread(NULL);
}

void camera_uvc_conect_state(uint8_t state)
{
	int ret = rtos_create_thread(&disc_task,
						 4,
						 "disc_task",
						 (beken_thread_function_t)camera_uvc_connect_state_change_task_entry,
						 1024,
						 (beken_thread_arg_t)&state);

	if (BK_OK != ret)
	{
		LOGE("%s camera_uvc_disconnect_task_entry init failed\n");
		return;
	}
}

void camera_uvc_open_handle(param_pak_t *param, media_camera_type_t type)
{
	int ret = 0;

	LOGI("%s\n", __func__);

#if CONFIG_USB_UVC

	if (CAMERA_STATE_DISABLED != get_camera_state())
	{
		LOGI("%s already opened\n", __func__);
		ret = kNoErr;
		goto out;
	}

#if CONFIG_BTDM_5_2
	if (bk_ble_get_env_state())
	{
		bk_ble_set_notice_cb(uvc_ble_notice_cb);
		LOGI("bluetooth is enabled, shutdown bluetooth\n");
		rtos_init_semaphore(&camera_act_sema, 1);
		bk_ble_deinit();
		if (rtos_get_semaphore(&camera_act_sema, 1000) != BK_OK)
		{
			LOGI("%s TIMEOUT\n", __func__);
		}

		rtos_deinit_semaphore(&camera_act_sema);
		camera_act_sema = NULL;
		bk_ble_set_notice_cb(NULL);
	}
	else
	{
		LOGI("bluetooth state: %d\n", bk_ble_get_env_state());
	}
#endif

	camera_info.type = type;
	camera_info.param = param->param;

	ret = bk_uvc_camera_open(param->param, type);

	if (ret != kNoErr)
	{
		LOGE("%s open failed\n", __func__);
		goto out;
	}

	set_camera_state(CAMERA_STATE_ENABLED);

	if (camera_info.debug)
	{
		bk_timer_start(TIMER_ID1, DEBUG_INTERVAL, camera_debug_dump);
	}

out:
#endif

	MEDIA_EVT_RETURN(param, ret);
}

void camera_uvc_close_handle(param_pak_t *param)
{
	int ret = 0;

	LOGI("%s\n", __func__);

#ifdef CONFIG_USB_UVC

	if (CAMERA_STATE_DISABLED == get_camera_state())
	{
		LOGI("%s already close\n", __func__);
		ret = kNoErr;
		goto out;
	}

	if (camera_info.debug)
	{
		bk_timer_stop(TIMER_ID1);
	}

	bk_uvc_camera_close();

	set_camera_state(CAMERA_STATE_DISABLED);

	//frame_buffer_enable(false);

	LOGI("%s complete!\n", __func__);

out:
#endif

	MEDIA_EVT_RETURN(param, ret);
}

void camera_uvc_reset_handle(uint32_t param)
{
	LOGI("%s\n", __func__);

#ifdef CONFIG_USB_UVC

	if (param == UVC_DISCONNECT_ABNORMAL)
	{
		if (CAMERA_STATE_DISABLED == get_camera_state())
		{
			LOGI("%s already close\n", __func__);
			return;
		}

		if (camera_info.debug)
		{
			bk_timer_stop(TIMER_ID1);
		}

		bk_uvc_camera_close();

		set_camera_state(CAMERA_STATE_DISABLED);

		//camera_uvc_disconect();
	}

	camera_uvc_conect_state((uint8_t)param);

#endif

	LOGI("%s, complete!\n", __func__);

}

void camera_net_open_handle(param_pak_t *param, media_camera_type_t type)
{
	int ret = 0;

	LOGI("%s\n", __func__);

#ifdef CONFIG_WIFI_TRANSFER

	if (CAMERA_STATE_DISABLED != get_camera_state())
	{
		LOGI("%s already opened\n", __func__);
		ret = kNoErr;
		goto out;
	}

	camera_info.type = MEDIA_CAMERA_UNKNOW;

#if CONFIG_BTDM_5_2
	if (bk_ble_get_env_state())
	{
		bk_ble_set_notice_cb(uvc_ble_notice_cb);
		LOGI("bluetooth is enabled, shutdown bluetooth\n");
		rtos_init_semaphore(&camera_act_sema, 1);
		bk_ble_deinit();
		if (rtos_get_semaphore(&camera_act_sema, 1000) != BK_OK)
		{
			LOGI("%s TIMEOUT\n", __func__);
		}

		rtos_deinit_semaphore(&camera_act_sema);
		camera_act_sema = NULL;
		bk_ble_set_notice_cb(NULL);
	}
	else
	{
		LOGI("bluetooth state: %d\n", bk_ble_get_env_state());
	}
#endif

	ret = bk_net_camera_open(param->param, type);

	if (ret != kNoErr)
	{
		LOGE("%s open failed\n", __func__);
		goto out;
	}

	set_camera_state(CAMERA_STATE_ENABLED);

	if (camera_info.debug)
	{
		bk_timer_start(TIMER_ID1, DEBUG_INTERVAL, camera_debug_dump);
	}

out:
#endif

	MEDIA_EVT_RETURN(param, ret);

}

void camera_net_close_handle(param_pak_t *param)
{
	int ret = 0;

	LOGI("%s\n", __func__);

#ifdef CONFIG_WIFI_TRANSFER
	if (CAMERA_STATE_DISABLED == get_camera_state())
	{
		LOGI("%s already close\n", __func__);
		ret = kNoErr;
		goto out;
	}

	bk_net_camera_close();

	set_camera_state(CAMERA_STATE_DISABLED);

out:
#endif

	MEDIA_EVT_RETURN(param, ret);
}
//#endif

void camera_event_handle(uint32_t event, uint32_t param)
{
	switch (event)
	{
		case EVENT_CAM_DVP_JPEG_OPEN_IND:
			camera_dvp_open_handle((param_pak_t *)param, MEDIA_DVP_MJPEG);
			break;
		case EVENT_CAM_DVP_YUV_OPEN_IND:
			camera_dvp_open_handle((param_pak_t *)param, MEDIA_DVP_YUV);
			break;
		case EVENT_CAM_DVP_MIX_OPEN_IND:
			camera_dvp_open_handle((param_pak_t *)param, MEDIA_DVP_MIX);
			break;
		case EVENT_CAM_DVP_CLOSE_IND:
			camera_dvp_close_handle((param_pak_t *)param);
			break;
		case EVENT_CAM_DVP_RESET_OPEN_IND:
			camera_dvp_reset_open_handle(param);
			break;
		case EVENT_CAM_UVC_MJPEG_OPEN_IND:
			camera_uvc_open_handle((param_pak_t *)param, MEDIA_UVC_MJPEG);
			break;
		case EVENT_CAM_UVC_H264_OPEN_IND:
			camera_uvc_open_handle((param_pak_t *)param, MEDIA_UVC_H264);
			break;
		case EVENT_CAM_UVC_CLOSE_IND:
			camera_uvc_close_handle((param_pak_t *)param);
			break;
		case EVENT_CAM_UVC_RESET_IND:
			camera_uvc_reset_handle(param);
			break;
		case EVENT_CAM_NET_MJPEG_OPEN_IND:
			camera_net_open_handle((param_pak_t *)param, MEDIA_UVC_MJPEG);
			break;
		case EVENT_CAM_NET_H264_OPEN_IND:
			camera_net_open_handle((param_pak_t *)param, MEDIA_UVC_H264);
			break;
		case EVENT_CAM_NET_CLOSE_IND:
			camera_net_close_handle((param_pak_t *)param);
			break;
		case EVENT_CAM_DVP_DROP_FRAME_IND:
			camera_dvp_drop_frame(param);
			break;

		default:
			break;
	}
}

camera_state_t get_camera_state(void)
{
	return camera_info.state;
}

void set_camera_state(camera_state_t state)
{
	camera_info.state = state;
}

void camera_init(void)
{
	camera_info.state = CAMERA_STATE_DISABLED;
	camera_info.debug = true;
}

bk_err_t media_app_register_uvc_connect_state_cb(void *cb)
{
	camera_connect_state_change_cb = cb;

	return BK_OK;
}



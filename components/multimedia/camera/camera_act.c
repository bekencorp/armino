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

#define TAG "cam_act"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

typedef void (*camera_connect_state_t)(uint8_t state);

camera_info_t camera_info;
bool dvp_camera_reset_open_ind = false;
beken_semaphore_t camera_act_sema = NULL;

static beken_thread_t disc_task = NULL;

camera_connect_state_t camera_connect_state_change_cb = NULL;

bk_err_t camera_dvp_reset_open_handle(media_mailbox_msg_t *msg)
{
	bk_err_t ret = BK_OK;

#ifdef CONFIG_CAMERA

	if (dvp_camera_reset_open_ind)
	{
		bk_dvp_camera_close();
		dvp_camera_reset_open_ind = false;
	}

	ret = bk_dvp_camera_open((media_ppi_t)msg->param, camera_info.type);
	if (ret != BK_OK)
	{
		dvp_camera_reset_open_ind = false;
	}
	else
	{
		dvp_camera_reset_open_ind = true;
	}
#endif

	os_free(msg);

	return ret;
}

bk_err_t camera_dvp_open_handle(media_mailbox_msg_t *msg, media_camera_type_t type)
{
	int ret = BK_OK;

	LOGI("%s\n", __func__);
#ifdef CONFIG_CAMERA

	if (CAMERA_STATE_DISABLED != get_camera_state())
	{
		LOGI("%s already opened\n", __func__);
		goto end;
	}

	camera_info.param = msg->param;
	camera_info.type = type;

	ret = bk_dvp_camera_open((media_ppi_t)msg->param, type);

	if (ret != BK_OK)
	{
		LOGE("%s open failed\n", __func__);
		goto end;
	}

	dvp_camera_reset_open_ind = true;

	set_camera_state(CAMERA_STATE_ENABLED);


end:
	msg_send_to_media_major_mailbox(msg, ret, APP_MODULE);

#endif

	return ret;
}

bk_err_t camera_dvp_close_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;

	LOGI("%s\n", __func__);

#ifdef CONFIG_CAMERA

	if (CAMERA_STATE_DISABLED == get_camera_state())
	{
		LOGI("%s already close\n", __func__);
		goto end;
	}

	bk_dvp_camera_close();

	set_camera_state(CAMERA_STATE_DISABLED);

	dvp_camera_reset_open_ind = false;


end:
	msg_send_to_media_major_mailbox(msg, ret, APP_MODULE);

#endif

	return ret;
}

bk_err_t camera_dvp_free_encode_mem_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;

#ifdef CONFIG_CAMERA
	ret = bk_dvp_camera_free_encode_mem();
#endif
	LOGI("%s complete\n", __func__);

	msg_send_to_media_major_mailbox(msg, ret, APP_MODULE);

	return ret;
}

bk_err_t camera_dvp_drop_frame(uint32_t param)
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

	return BK_OK;
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
						 1024 * 2,
						 (beken_thread_arg_t)&state);

	if (BK_OK != ret)
	{
		LOGE("%s camera_uvc_disconnect_task_entry init failed\n");
		return;
	}
}

bk_err_t camera_uvc_open_handle(media_mailbox_msg_t *msg, media_camera_type_t type)
{
	int ret = BK_OK;

	LOGI("%s\n", __func__);

#if CONFIG_USB_UVC

	if (CAMERA_STATE_DISABLED != get_camera_state())
	{
		LOGI("%s already opened\n", __func__);
		goto end;
	}

	camera_info.type = type;
	camera_info.param = msg->param;

	ret = bk_uvc_camera_open((media_ppi_t)msg->param, type);

	if (ret != BK_OK)
	{
		LOGE("%s open failed\n", __func__);
	}
	else
	{
		set_camera_state(CAMERA_STATE_ENABLED);
	}

#endif

end:
	msg_send_to_media_major_mailbox(msg, ret, APP_MODULE);

	return ret;
}

bk_err_t camera_uvc_close_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;

	LOGI("%s\n", __func__);

#ifdef CONFIG_USB_UVC

	if (CAMERA_STATE_DISABLED == get_camera_state())
	{
		LOGI("%s already close\n", __func__);
		goto end;
	}

	bk_uvc_camera_close();

	set_camera_state(CAMERA_STATE_DISABLED);

	LOGI("%s complete!\n", __func__);

#endif

end:
	msg_send_to_media_major_mailbox(msg, ret, APP_MODULE);

	return ret;
}

bk_err_t camera_uvc_reset_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;
	LOGI("%s\n", __func__);

#ifdef CONFIG_USB_UVC

	if (msg->param == UVC_DISCONNECT_ABNORMAL)
	{
		if (CAMERA_STATE_DISABLED == get_camera_state())
		{
			LOGI("%s already close\n", __func__);
			goto end;
		}

		bk_uvc_camera_close();

		set_camera_state(CAMERA_STATE_DISABLED);
	}

	camera_uvc_conect_state((uint8_t)msg->param);

#endif

end:

	os_free(msg);

	LOGI("%s, complete!\n", __func__);

	return ret;
}

bk_err_t camera_net_open_handle(media_mailbox_msg_t *msg, media_camera_type_t type)
{
	int ret = BK_OK;

	LOGI("%s\n", __func__);

#ifdef CONFIG_WIFI_TRANSFER

	if (CAMERA_STATE_DISABLED != get_camera_state())
	{
		LOGI("%s already opened\n", __func__);
		goto end;
	}

	camera_info.type = MEDIA_CAMERA_UNKNOW;

	ret = bk_net_camera_open((media_ppi_t)msg->param, type);

	if (ret != BK_OK)
	{
		LOGE("%s open failed\n", __func__);
		goto end;
	}

	set_camera_state(CAMERA_STATE_ENABLED);

#endif

end:
	msg_send_to_media_major_mailbox(msg, ret, APP_MODULE);

	return ret;
}

bk_err_t camera_net_close_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;

	LOGI("%s\n", __func__);

#ifdef CONFIG_WIFI_TRANSFER

	if (CAMERA_STATE_DISABLED == get_camera_state())
	{
		LOGI("%s already close\n", __func__);
		goto end;
	}

	bk_net_camera_close();

	set_camera_state(CAMERA_STATE_DISABLED);

#endif

end:
	msg_send_to_media_major_mailbox(msg, ret, APP_MODULE);

	return ret;
}

bk_err_t camera_event_handle(media_mailbox_msg_t *msg)
{
	bk_err_t ret = BK_FAIL;
	switch (msg->event)
	{
		case EVENT_CAM_DVP_JPEG_OPEN_IND:
			ret = camera_dvp_open_handle(msg, MEDIA_DVP_MJPEG);
			break;
		case EVENT_CAM_DVP_YUV_OPEN_IND:
			ret = camera_dvp_open_handle(msg, MEDIA_DVP_YUV);
			break;
		case EVENT_CAM_DVP_MIX_OPEN_IND:
			ret = camera_dvp_open_handle(msg, MEDIA_DVP_MIX);
			break;
		case EVENT_CAM_DVP_H264_WIFI_TRANSFER_OPEN_IND:
			ret = camera_dvp_open_handle(msg, MEDIA_DVP_H264_WIFI_TRANSFER);
			break;
		case EVENT_CAM_DVP_H264_USB_TRANSFER_OPEN_IND:
			ret = camera_dvp_open_handle(msg, MEDIA_DVP_H264_USB_TRANSFER);
			break;
		case EVENT_CAM_DVP_H264_LOCAL_OPEN_IND:
			ret = camera_dvp_open_handle(msg, MEDIA_DVP_H264_LOCAL);
			break;
		case EVENT_CAM_DVP_H264_ENC_LCD_OPEN_IND:
			ret = camera_dvp_open_handle(msg, MEDIA_DVP_H264_ENC_LCD);
			break;
		case EVENT_CAM_DVP_CLOSE_IND:
			ret = camera_dvp_close_handle(msg);
			break;
		case EVENT_CAM_DVP_FREE_ENCODE_MEM_IND:
			ret = camera_dvp_free_encode_mem_handle(msg);
			break;
		case EVENT_CAM_DVP_RESET_OPEN_IND:
			ret = camera_dvp_reset_open_handle(msg);
			break;
		case EVENT_CAM_UVC_MJPEG_OPEN_IND:
			ret = camera_uvc_open_handle(msg, MEDIA_UVC_MJPEG);
			break;
		case EVENT_CAM_UVC_H264_OPEN_IND:
			ret = camera_uvc_open_handle(msg, MEDIA_UVC_H264);
			break;
		case EVENT_CAM_UVC_CLOSE_IND:
			ret = camera_uvc_close_handle(msg);
			break;
		case EVENT_CAM_UVC_RESET_IND:
			ret = camera_uvc_reset_handle(msg);
			break;
#if 0
		case EVENT_CAM_NET_MJPEG_OPEN_IND:
			ret = camera_net_open_handle(msg, MEDIA_UVC_MJPEG);
			break;
		case EVENT_CAM_NET_H264_OPEN_IND:
			ret = camera_net_open_handle(msg, MEDIA_UVC_H264);
			break;
		case EVENT_CAM_NET_CLOSE_IND:
			ret = camera_net_close_handle(msg);
			break;
#endif
		case EVENT_CAM_DVP_DROP_FRAME_IND:
			ret = camera_dvp_drop_frame(msg->param);
			break;

		default:
			break;
	}

	return ret;
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



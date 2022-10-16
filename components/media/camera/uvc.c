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

#include "camera_act.h"
#include "media_evt.h"
#include "media_app.h"

#include <os/mem.h>

#include <driver/uvc_camera.h>

#include "frame_buffer.h"

#define TAG "uvc"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#ifdef CONFIG_USB_UVC

static bool uvc_state_flag = false;

uvc_camera_device_t uvc_camera_device =
{
	.width = 640,
	.height = 480,
	.fps = 25,
};


void uvc_device_disconnect_callback(void)
{
	LOGI("%s\n", __func__);

	if (!uvc_state_flag)
	{
		return;
	}

	uvc_state_flag = false;

	media_app_camera_close(APP_CAMERA_UVC);
}

const uvc_camera_config_t uvc_camera_config =
{
	.device = &uvc_camera_device,
	.fb_init = frame_buffer_fb_jpeg_init,
	.fb_deinit = frame_buffer_fb_jpeg_deinit,
	.fb_complete = frame_buffer_fb_push,
	.fb_malloc = frame_buffer_fb_jpeg_malloc,
	.fb_free = frame_buffer_fb_jpeg_free,
	.uvc_disconnect = uvc_device_disconnect_callback,
};


bk_err_t bk_uvc_camera_open(media_ppi_t ppi)
{
	if (ppi != 0)
	{
		uvc_camera_device.width = ppi >> 16;
		uvc_camera_device.height = ppi & 0xFFFF;
	}
	uvc_state_flag = true;

	return bk_uvc_camera_driver_init(&uvc_camera_config);
}

bk_err_t bk_uvc_camera_start(void)
{
	return bk_uvc_camera_driver_start();
}

bk_err_t bk_uvc_camera_stop(void)
{
	return bk_uvc_camera_driver_stop();
}

bk_err_t bk_uvc_camera_close(void)
{
	if (uvc_state_flag)
	{
		uvc_state_flag = false;
		bk_uvc_camera_driver_deinit();
	}
	return 0;
}

bk_err_t bk_uvc_camera_param_set(param_pak_t *param)
{
	uvc_camera_device_t *config = (uvc_camera_device_t *)param->param;

	if (config->fps != 0 && config->width != 0 && config->height != 0)
	{
		uvc_camera_device.width = config->width;
		uvc_camera_device.height = config->height;
		uvc_camera_device.fps = config->fps;
	}
	else
	{
		LOGE("%s, param error!\n", __func__);
		return BK_FAIL;
	}

	return bk_uvc_camera_set_config(&uvc_camera_device);
}

#endif

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
#include <os/str.h>
#include <os/mem.h>

#include <driver/int.h>
#include <driver/dvp_camera_types.h>
#include <driver/pwr_clk.h>

#include <components/log.h>
#include <common/bk_include.h>
#include <components/video_transfer.h>

#include "media_core.h"
#include "media_evt.h"
#include "media_app.h"
#include "transfer_act.h"
#include "camera_act.h"
#include "lcd_act.h"
#include "storage_act.h"

#include "media_mailbox_list_util.h"

#define TAG "media_app"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static app_camera_type_t app_camera_type = APP_CAMERA_INVALIED;
static char *capture_name = NULL;
static media_modules_state_t *media_modules_state = NULL;

extern void rwnxl_set_video_transfer_flag(uint32_t video_transfer_flag);
#if CONFIG_CHERRY_USB && CONFIG_USB_DEVICE
extern void usbd_video_h264_init();
extern void usbd_video_h264_deinit();
#endif

bk_err_t media_send_msg_sync(uint32_t event, uint32_t param)
{
	int ret = kGeneralErr;

	media_mailbox_msg_t *node = NULL;

	node = os_malloc(sizeof(media_mailbox_msg_t));
	if (node != NULL)
	{
		ret = rtos_init_semaphore_ex(&node->sem, 1, 0);

		if (ret != kNoErr)
		{
			LOGE("%s init semaphore failed\n", __func__);
			goto out;
		}
		node->event = event;
		node->param = param;
		LOGD("====>>>>1 %s %x\n", __func__, node->sem);
		ret = msg_send_req_to_media_app_mailbox_sync(node);
	}
	else
	{
		goto out;
	}

out:

	if (node)
	{
		if(node->sem)
		{
			rtos_deinit_semaphore(&node->sem);
			node->sem = NULL;
		}

		os_free(node);
	}

	return ret;
}

#ifdef CONFIG_INTEGRATION_DOORBELL
bk_err_t media_app_camera_open(camera_config_t *camera_config)
{
	int ret = BK_FAIL;
	int type;
	media_ppi_t ppi = camera_config->width << 16 | camera_config->height;

	if (camera_config->type == UVC_CAMERA)
	{
		if (camera_config->image_format == IMAGE_MJPEG)
		{
			type = APP_CAMERA_UVC_MJPEG;
		}
		else
		{
			type = APP_CAMERA_UVC_H264;
		}
	}
	else // DVP
	{
		if (camera_config->image_format == IMAGE_MJPEG)
		{
			type = APP_CAMERA_DVP_JPEG;
		}
		else
		{
			type = APP_CAMERA_DVP_H264_ENC_LCD;
		}
	}

	app_camera_type = type;

	LOGI("%s, type:%d, ppi:%d-%d\n", __func__, app_camera_type, ppi >> 16, ppi & 0xFFFF);

#else
bk_err_t media_app_camera_open(app_camera_type_t type, media_ppi_t ppi)
{
	int ret = BK_FAIL;

	LOGI("%s, type:%d, ppi:%d-%d\n", __func__, type, ppi >> 16, ppi & 0xFFFF);

	app_camera_type = type;
#endif

	bk_pm_module_vote_boot_cp1_ctrl(PM_BOOT_CP1_MODULE_NAME_VIDP_JPEG_EN, PM_POWER_MODULE_STATE_ON);

	switch (type)
	{
		case APP_CAMERA_DVP_JPEG:
			ret = media_send_msg_sync(EVENT_CAM_DVP_JPEG_OPEN_IND, ppi);
			break;

		case APP_CAMERA_DVP_YUV:
			ret = media_send_msg_sync(EVENT_CAM_DVP_YUV_OPEN_IND, ppi);
			break;

		case APP_CAMERA_DVP_MIX:
			ret = media_send_msg_sync(EVENT_CAM_DVP_MIX_OPEN_IND, ppi);
			break;

		case APP_CAMERA_UVC_MJPEG:
			ret = media_send_msg_sync(EVENT_CAM_UVC_MJPEG_OPEN_IND, ppi);
			break;

		case APP_CAMERA_UVC_MJPEG_TO_H264:
			ret = media_send_msg_sync(EVENT_CAM_UVC_MJPEG_TO_H264_OPEN_IND, ppi);
			break;

		case APP_CAMERA_UVC_H264:
			ret = media_send_msg_sync(EVENT_CAM_UVC_H264_OPEN_IND, ppi);
			break;

		case APP_CAMERA_NET_MJPEG:
			ret = media_send_msg_sync(EVENT_CAM_NET_MJPEG_OPEN_IND, ppi);
			break;

		case APP_CAMERA_NET_H264:
			ret = media_send_msg_sync(EVENT_CAM_NET_H264_OPEN_IND, ppi);
			break;

		case APP_CAMERA_DVP_H264_TRANSFER:
			ret = media_send_msg_sync(EVENT_CAM_DVP_H264_TRANSFER_OPEN_IND, ppi);
			break;
		
		case APP_CAMERA_DVP_H264_LOCAL:
			ret = media_send_msg_sync(EVENT_CAM_DVP_H264_LOCAL_OPEN_IND, ppi);
			break;
		
		case APP_CAMERA_DVP_H264_ENC_LCD:
			ret = media_send_msg_sync(EVENT_CAM_DVP_H264_ENC_LCD_OPEN_IND, ppi);
			break;

		default:
			ret = BK_OK;
	}

	LOGI("%s complete\n", __func__);

	return ret;
}

bk_err_t media_app_camera_close(app_camera_type_t type)
{
	int ret = BK_OK;

	LOGI("%s\n", __func__);

	switch (type)
	{
		case APP_CAMERA_DVP_JPEG:
		case APP_CAMERA_DVP_YUV:
		case APP_CAMERA_DVP_H264_LOCAL:
		case APP_CAMERA_DVP_H264_ENC_LCD:
		case APP_CAMERA_DVP_H264_TRANSFER:
			ret = media_send_msg_sync(EVENT_CAM_DVP_CLOSE_IND, 0);
			break;

		case APP_CAMERA_UVC_H264:
		case APP_CAMERA_UVC_MJPEG:
		case APP_CAMERA_UVC_MJPEG_TO_H264:
			ret = media_send_msg_sync(EVENT_CAM_UVC_CLOSE_IND, 0);
			break;

		case APP_CAMERA_NET_MJPEG:
		case APP_CAMERA_NET_H264:
			ret = media_send_msg_sync(EVENT_CAM_NET_CLOSE_IND, 0);
			break;

		case APP_CAMERA_INVALIED:
			ret = media_send_msg_sync(EVENT_CAM_DVP_FREE_ENCODE_MEM_IND, 0);

		default:
			ret = BK_OK;
	}

	app_camera_type = APP_CAMERA_INVALIED;

	bk_pm_module_vote_boot_cp1_ctrl(PM_BOOT_CP1_MODULE_NAME_VIDP_JPEG_EN, PM_POWER_MODULE_STATE_OFF);

	LOGI("%s complete\n", __func__);

	return ret;
}

bk_err_t media_app_mailbox_test(void)
{
	bk_err_t ret = BK_FAIL;
	uint32_t param = 0x88888888;

	LOGI("%s +++\n", __func__);

	ret = media_send_msg_sync(EVENT_LCD_DEFAULT_CMD, param);

	LOGI("%s ---\n", __func__);

	return ret;
}

#ifdef CONFIG_INTEGRATION_DOORBELL
bk_err_t media_app_transfer_open(const media_transfer_cb_t *cb)
{
	int ret = BK_OK;
	int type = APP_CAMERA_UVC_MJPEG;
	uint32_t param = FB_INDEX_JPEG;

	if (media_modules_state->trs_state == TRS_STATE_ENABLED)
	{
		LOGI("%s, transfer have been opened!\r\n", __func__);
		return ret;
	}

	rwnxl_set_video_transfer_flag(true);

	ret = transfer_app_task_init(cb);

	type = app_camera_type;

#else
bk_err_t media_app_transfer_open(video_setup_t *setup_cfg)
{

	int ret = kNoErr;

	uint32_t param = FB_INDEX_JPEG;

	app_camera_type_t type = setup_cfg->open_type;

	if (media_modules_state->trs_state == TRS_STATE_ENABLED)
	{
		LOGI("%s, transfer have been opened!\r\n", __func__);
		return ret;
	}

	rwnxl_set_video_transfer_flag(true);

	ret = transfer_app_task_init((video_setup_t *)setup_cfg);
#endif

	if (ret != kNoErr)
	{
		LOGE("transfer_app_task_init failed\r\n");
		return ret;
	}

	switch (type)
	{
		case APP_CAMERA_DVP_JPEG:
		case APP_CAMERA_UVC_MJPEG:
		case APP_CAMERA_UVC_H264:
		case APP_CAMERA_NET_MJPEG:
			param = FB_INDEX_JPEG;
			break;

		case APP_CAMERA_NET_H264:
		case APP_CAMERA_DVP_H264_TRANSFER:
		case APP_CAMERA_DVP_H264_LOCAL:
		case APP_CAMERA_DVP_H264_ENC_LCD:
		case APP_CAMERA_UVC_MJPEG_TO_H264:
			param = FB_INDEX_H264;
			break;

		default:
			break;
	}

	LOGI("%s, %d %d\n", __func__, type, param);

	ret = media_send_msg_sync(EVENT_TRANSFER_OPEN_IND, param);

	if (ret == BK_OK)
	{
		media_modules_state->trs_state = TRS_STATE_ENABLED;
	}

	LOGI("%s complete\n", __func__);

	return ret;
}

bk_err_t media_app_transfer_pause(bool pause)
{
	int ret = kNoErr;

	ret = media_send_msg_sync(EVENT_TRANSFER_PAUSE_IND, pause);

	return ret;
}

bk_err_t media_app_transfer_close(void)
{
	bk_err_t ret = BK_OK;

	media_mailbox_msg_t mb_msg;

	if (media_modules_state->trs_state == TRS_STATE_DISABLED)
	{
		LOGI("%s, transfer have been closed!\r\n", __func__);
		return ret;
	}

	LOGI("%s\n", __func__);

	mb_msg.event = EVENT_TRANSFER_CLOSE_IND;
	transfer_app_event_handle(&mb_msg);

	ret = media_send_msg_sync(EVENT_TRANSFER_CLOSE_IND, 0);

	if (ret == BK_OK)
	{
		media_modules_state->trs_state = TRS_STATE_DISABLED;
	}

	return ret;
}

bk_err_t media_app_usb_open(video_setup_t *setup_cfg)
{
	int ret = kNoErr;
#if CONFIG_CHERRY_USB && CONFIG_USB_DEVICE

	uint32_t param_fmt = FB_INDEX_H264;

	app_camera_type_t type = setup_cfg->open_type;

	usbd_video_h264_init();

	ret = usb_app_task_init((video_setup_t *)setup_cfg);
	if (ret != kNoErr)
	{
		LOGE("usb_app_task_init failed\r\n");
		return ret;
	}

	switch (type)
	{
		case APP_CAMERA_DVP_H264_TRANSFER:
			param_fmt = FB_INDEX_H264;
			break;
		case APP_CAMERA_DVP_JPEG:
			param_fmt = FB_INDEX_JPEG;

		default:
			LOGE("unsupported format! \r\n");
			break;
	}

	LOGI("%s, %d %d\n", __func__, type, param_fmt);

	ret = media_send_msg_sync(EVENT_TRANSFER_USB_OPEN_IND, param_fmt);

	LOGI("%s complete\n", __func__);
#endif

	return ret;
}

bk_err_t media_app_usb_close(void)
{
	bk_err_t ret = BK_OK;
	
#if CONFIG_CHERRY_USB && CONFIG_USB_DEVICE
	media_mailbox_msg_t mb_msg;

	LOGI("%s\n", __func__);

	usbd_video_h264_deinit();

	mb_msg.event = EVENT_TRANSFER_USB_CLOSE_IND;
	usb_app_event_handle(&mb_msg);

	ret = media_send_msg_sync(EVENT_TRANSFER_USB_CLOSE_IND, 0);
#endif

	return ret;
}

bk_err_t media_app_lcd_rotate(media_rotate_t rotate)
{
	bk_err_t ret;

	LOGI("%s\n", __func__);

	ret = media_send_msg_sync(EVENT_LCD_ROTATE_ENABLE_IND, rotate);

	LOGI("%s complete\n", __func__);

	return ret;
}

bk_err_t media_app_lcd_resize(media_ppi_t ppi)
{
	bk_err_t ret;

	LOGI("%s\n", __func__);

	ret = media_send_msg_sync(EVENT_LCD_RESIZE_IND, ppi);

	LOGI("%s complete\n", __func__);

	return ret;
}


bk_err_t media_app_lcd_display_beken(void *lcd_display)
{
	bk_err_t ret;

	lcd_display_t *ptr = NULL;
	ptr = (lcd_display_t *)os_malloc(sizeof(lcd_display_t));
	os_memcpy(ptr, (lcd_display_t *)lcd_display, sizeof(lcd_display_t));

	ret = media_send_msg_sync(EVENT_LCD_BEKEN_LOGO_DISPLAY, (uint32_t)ptr);
	LOGI("%s complete\n", __func__);
	os_free(ptr);
	ptr =NULL;

	return ret;
}

bk_err_t media_app_lcd_open(void *lcd_open)
{
	int ret = kNoErr;
	lcd_open_t *ptr = NULL;

	ptr = (lcd_open_t *)os_malloc(sizeof(lcd_open_t));
	if (ptr == NULL) {
		LOGE("malloc lcd_open_t failed\r\n");
		return kGeneralErr;
	}
	os_memcpy(ptr, (lcd_open_t *)lcd_open, sizeof(lcd_open_t));

	bk_pm_module_vote_boot_cp1_ctrl(PM_BOOT_CP1_MODULE_NAME_VIDP_LCD, PM_POWER_MODULE_STATE_ON);

	ret = media_send_msg_sync(EVENT_LCD_OPEN_IND, (uint32_t)ptr);

	if (ptr) {
		os_free(ptr);
		ptr =NULL;
	}

	LOGI("%s complete %x\n", __func__, ret);

	return ret;
}

bk_err_t media_app_lcd_display_file(char *file_name)
{
	int ret;

	LOGI("%s ,%s\n", __func__,file_name);
	
	if (file_name != NULL)
	{
		uint32_t len = os_strlen(file_name) + 1;

		if (len > 31)
		{
			len = 31;
		}

		if (capture_name == NULL)
		{
			capture_name = (char *)os_malloc(len);
		}
		else
		{
			os_free(capture_name);
			capture_name = NULL;
			capture_name = (char *)os_malloc(len);
		}
		os_memset(capture_name, 0, len);
		os_memcpy(capture_name, file_name, len);
		capture_name[len - 1] = '\0';
	}
	
	ret = media_send_msg_sync(EVENT_LCD_DISPLAY_FILE_IND, 0);

	return ret;

}

bk_err_t read_storage_file_to_mem_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;
	LOGI("%s\n", __func__);

	media_mailbox_msg_t *resd_storage_node = msg;
	frame_buffer_t *frame = (frame_buffer_t  *)msg->param;
	if (capture_name == NULL)
	{
		LOGE("%s  display file is none \n", __func__);
		return BK_FAIL;
	}

#if (CONFIG_FATFS)
	ret = sdcard_read_to_mem((char *)capture_name, (uint32_t *)frame->frame, &frame->length );
#endif

	os_free(capture_name);
	capture_name = NULL;

	resd_storage_node->param = (uint32_t)frame;
	resd_storage_node->event = EVENT_LCD_PICTURE_ECHO_NOTIFY;
	resd_storage_node->result = ret;
	ret = msg_send_rsp_to_media_app_mailbox(resd_storage_node, ret);

	return ret;
}

bk_err_t media_app_lcd_display(void *lcd_display)
{
	bk_err_t ret;
	lcd_display_t *ptr = NULL;
	ptr = (lcd_display_t *)os_malloc(sizeof(lcd_display_t));
	os_memcpy(ptr, (lcd_display_t *)lcd_display, sizeof(lcd_display_t));

	ret = media_send_msg_sync(EVENT_LCD_DISPLAY_IND, (uint32_t)ptr);
	LOGI("%s complete\n", __func__);

	os_free(ptr);
	return ret;
}

bk_err_t media_app_lcd_blend(void *param)
{
	bk_err_t ret;
	lcd_blend_msg_t *ptr = NULL;
	ptr = (lcd_blend_msg_t *)os_malloc(sizeof(lcd_blend_msg_t));
	os_memcpy(ptr, (lcd_blend_msg_t *)param, sizeof(lcd_blend_msg_t));

	ret = media_send_msg_sync(EVENT_LCD_BLEND_IND, (uint32_t)ptr);

	os_free(ptr);
	return ret;
}

bk_err_t media_app_lcd_close(void)
{
	bk_err_t ret = 0;

	LOGI("%s\n", __func__);

	ret = media_send_msg_sync(EVENT_LCD_CLOSE_IND, 0);

	bk_pm_module_vote_boot_cp1_ctrl(PM_BOOT_CP1_MODULE_NAME_VIDP_LCD, PM_POWER_MODULE_STATE_OFF);

	LOGI("%s complete\n", __func__);

	return ret;
}


bk_err_t media_app_lcd_set_backlight(uint8_t level)
{
	bk_err_t ret = BK_OK;

	LOGI("%s\n", __func__);

	ret = media_send_msg_sync(EVENT_LCD_SET_BACKLIGHT_IND, level);

	LOGI("%s complete\n", __func__);

	return ret;
}

bk_err_t media_app_storage_enable(app_camera_type_t type, uint8_t enable)
{
	int ret = BK_OK;
	uint32_t param = FB_INDEX_JPEG;
	media_mailbox_msg_t msg = {0};

	if (enable)
	{
		if (media_modules_state->stor_state == STORAGE_STATE_ENABLED)
		{
			LOGD("%s, storage have been opened!\r\n", __func__);
			return ret;
		}

		msg.event = EVENT_STORAGE_OPEN_IND;
		ret = storage_app_event_handle(&msg);
		if (ret != BK_OK)
		{
			return ret;
		}

		if (type == APP_CAMERA_DVP_H264_LOCAL)
			param = FB_INDEX_H264;

		ret = media_send_msg_sync(EVENT_STORAGE_OPEN_IND, param);

		if (ret == BK_OK)
		{
			media_modules_state->stor_state = STORAGE_STATE_ENABLED;
		}
	}
	else
	{
		if (media_modules_state->stor_state == STORAGE_STATE_DISABLED)
		{
			LOGI("%s, storage have been closed!\r\n", __func__);
			return ret;
		}

		ret = media_send_msg_sync(EVENT_STORAGE_CLOSE_IND, 0);
		if (ret != BK_OK)
		{
			LOGE("storage_major_task deinit failed\r\n");
		}

		msg.event = EVENT_STORAGE_CLOSE_IND;
		ret = storage_app_event_handle(&msg);

		if (ret == BK_OK)
		{
			media_modules_state->stor_state = STORAGE_STATE_DISABLED;
		}
	}

	return ret;
}

bk_err_t media_app_capture(char *name)
{
	int ret = BK_OK;

	if (name == NULL)
	{
		return ret;
	}

	media_app_storage_enable(app_camera_type, 1);

	ret = storage_app_set_frame_name(name);
	if (ret != BK_OK)
	{
		return ret;
	}

	ret = media_send_msg_sync(EVENT_STORAGE_CAPTURE_IND, 0);

	LOGI("%s complete\n", __func__);

	return ret;
}

bk_err_t media_app_save_start(char *name)
{
	int ret = BK_OK;

	if (name == NULL)
	{
		return ret;
	}

	media_app_storage_enable(app_camera_type, 1);

	ret = storage_app_set_frame_name(name);
	if (ret != BK_OK)
	{
		return ret;
	}

	ret = media_send_msg_sync(EVENT_STORAGE_SAVE_START_IND, 0);

	LOGI("%s complete\n", __func__);

	return ret;
}


bk_err_t media_app_save_stop(void)
{
	int ret = BK_OK;
	media_mailbox_msg_t msg = {0};

	if (media_modules_state->stor_state == STORAGE_STATE_DISABLED)
	{
		LOGI("%s storage function not init\n", __func__);
		return ret;
	}

	ret = media_send_msg_sync(EVENT_STORAGE_SAVE_STOP_IND, 0);
	if (ret != BK_OK)
	{
		LOGE("storage_major_task stop save video failed\r\n");
	}

	msg.event = EVENT_STORAGE_SAVE_STOP_IND;
	ret = storage_app_event_handle(&msg);

	LOGI("%s complete\n", __func__);

	return ret;
}

bk_err_t media_app_init(void)
{
	bk_err_t ret = BK_OK;

	if (media_modules_state == NULL)
	{
		media_modules_state = (media_modules_state_t *)os_malloc(sizeof(media_modules_state_t));
		if (media_modules_state == NULL)
		{
			LOGE("%s, media_modules_state malloc failed!\n");
			return BK_ERR_NO_MEM;
		}
	}

	media_modules_state->aud_state = AUDIO_STATE_DISABLED;
	media_modules_state->cam_state = CAMERA_STATE_DISABLED;
	media_modules_state->lcd_state = LCD_STATE_DISABLED;
	media_modules_state->stor_state = STORAGE_STATE_DISABLED;
	media_modules_state->trs_state = TRS_STATE_DISABLED;

	return ret;
}

bk_err_t media_app_dump_display_frame(void)
{
	return media_send_msg_sync(EVENT_LCD_DUMP_DISPLAY_IND, 0);
}

bk_err_t media_app_dump_decoder_frame(void)
{
	return media_send_msg_sync(EVENT_LCD_DUMP_DECODER_IND, 0);
}

bk_err_t media_app_dump_jpeg_frame(void)
{
	return media_send_msg_sync(EVENT_LCD_DUMP_JPEG_IND, 0);
}

bk_err_t media_app_lcd_step_mode(bool enable)
{
	return media_send_msg_sync(EVENT_LCD_STEP_MODE_IND, enable);
}

bk_err_t media_app_lcd_step_trigger(void)
{
	return media_send_msg_sync(EVENT_LCD_STEP_TRIGGER_IND, 0);
}

#if CONFIG_VIDEO_AVI
bk_err_t media_app_avi_open(void)
{
	return media_send_msg_sync(EVENT_AVI_OPEN_IND, 1);
}

bk_err_t media_app_avi_close(void)
{
	return media_send_msg_sync(EVENT_AVI_CLOSE_IND, 0);
}
#endif

bk_err_t media_app_lvgl_draw(void *lcd_open)
{
	int ret = BK_OK;
	lcd_open_t *ptr = NULL;

	ptr = (lcd_open_t *)os_malloc(sizeof(lcd_open_t));
	if (ptr == NULL) {
		LOGE("malloc lcd_open_t failed\r\n");
		return BK_ERR_NO_MEM;
	}
	os_memcpy(ptr, (lcd_open_t *)lcd_open, sizeof(lcd_open_t));

	ret = media_send_msg_sync(EVENT_LVGL_DRAW_IND, (uint32_t)ptr);

	if (ptr) {
		os_free(ptr);
		ptr = NULL;
	}

	LOGI("%s complete %x\n", __func__, ret);

	return ret;
}


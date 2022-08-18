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

#pragma once

#include <common/bk_include.h>
#include "media_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MEDIA_EVT_RETURN(res, val) \
	do { \
		if (res != NULL) { \
			res->ret = val; \
			rtos_set_semaphore(&res->sem); \
		} \
	} while(0)


/*
*   Message Type:
*       CMD <-> EVT     for     CPU0 <-> CPU1
*       REQ <-> EVT     for     modules
*       IND             for     unicast instruction
*    _______________________________________                   _______________________________________
*   |                                       |                 |                                       |
*   |             Major CPU(CPU0)           |                 |             Minor CPU(CPU1)           |
*   |_______________________________________|                 |_______________________________________|
*   |                   |                   |     CMD1 ==>    |                   |                   |
*   |    Moudle A       |    Moudle B       |     <== EVT1    |    Moudle C       |    Moudle D       |
*   |  _____________    |    _____________  |                 |  _____________    |    _____________  |
*   | |             |   |   |             | |     CMD2 ==>    | |             |   |   |             | |
*   | | EVENT_REQ1  | <-|-> | EVENT_RES1  | |     <== EVT2    | | EVENT_REQ1  | <-|-> | EVENT_RES1  | |
*   | | EVENT_REQ2  | <-|-> | EVENT_RES2  | |                 | | EVENT_REQ2  | <-|-> | EVENT_RES2  | |
*   | | EVENT_RES1  | <-|-> | EVENT_REQ1  | |     CMD3 ==>    | | EVENT_RES1  | <-|-> | EVENT_REQ1  | |
*   | | EVENT_RES2  | <-|-> | EVENT_REQ2  | |     <== EVT3    | | EVENT_RES2  | <-|-> | EVENT_REQ2  | |
*   | |_____________|   |   |_____________| |                 | |_____________|   |   |_____________| |
*   |___________________|___________________|                 |___________________|___________________|
*
*/


typedef enum
{
	COM_EVENT = 1,
	DVP_EVENT,
	UVC_EVENT,
	AUD_EVENT,
	LCD_EVENT,
	STORAGE_EVENT,
	TRS_EVENT,
	EXIT_EVENT,
	QUEUE_EVENT,
	MAILBOX_CMD,
	MAILBOX_EVT,
} media_mask_t;

typedef enum
{
	EVENT_COM_DEFAULT_IND = (COM_EVENT << MEDIA_EVT_BIT),
	EVENT_COM_FRAME_WIFI_FREE_IND,
	EVENT_COM_FRAME_DECODER_FREE_IND,
	EVENT_COM_FRAME_CAPTURE_FREE_IND,
	EVENT_COM_FRAME_REQUEST_IND,


	EVENT_AUD_ADC_OPEN = (AUD_EVENT << MEDIA_EVT_BIT),
	EVENT_AUD_DAC_OPEN,

	EVENT_DVP_JPEG_OPEN_IND = (DVP_EVENT << MEDIA_EVT_BIT),
	EVENT_DVP_YUV_OPEN_IND,
	EVENT_DVP_CLOSE_IND,

	EVENT_LCD_OPEN_IND = (LCD_EVENT << MEDIA_EVT_BIT),
	EVENT_LCD_DVP_REG_CAM_INIT_RES,
	EVENT_LCD_FRAME_COMPLETE_IND,
	EVENT_LCD_RESIZE_IND,
	EVENT_LCD_CLOSE_IND,
	EVENT_LCD_SET_BACKLIGHT_IND,
	EVENT_LCD_ROTATE_ENABLE_IND,
	EVENT_LCD_FRAME_LOCAL_ROTATE_IND,
	EVENT_LCD_DUMP_DECODER_IND,
	EVENT_LCD_DUMP_JPEG_IND,
	EVENT_LCD_DUMP_DISPLAY_IND,
	EVENT_LCD_STEP_MODE_IND,
	EVENT_LCD_STEP_TRIGGER_IND,

	EVENT_STORAGE_OPEN_IND = (STORAGE_EVENT << MEDIA_EVT_BIT),
	EVENT_STORAGE_CAPTURE_IND,

	EVENT_TRS_VIDEO_TRANSFER_OPEN_IND = (TRS_EVENT << MEDIA_EVT_BIT),
	EVENT_TRS_VIDEO_TRANSFER_CLOSE_IND,

	EVENT_UVC_OPEN_IND = (UVC_EVENT << MEDIA_EVT_BIT),
	EVENT_UVC_CLOSE_IND,
	EVENT_UVC_START_IND,
	EVENT_UVC_STOP_IND,
	EVENT_UVC_PARAM_IND,


	EVENT_LCD_DEFAULT_CMD = (MAILBOX_CMD << MEDIA_EVT_BIT),
	EVENT_LCD_ROTATE_RIGHT_CMD,

	EVENT_LCD_DEFAULT_EVT = (MAILBOX_EVT << MEDIA_EVT_BIT),
	EVENT_LCD_ROTATE_RIGHT_COMP_EVT,
} media_event_t;


typedef enum
{
	EVENT_APP_DVP_OPEN,
	EVENT_APP_EXIT,
} media_app_event_t;


typedef struct
{
	uint32_t event;
	uint32_t param;
} media_msg_t;


typedef struct
{
	beken_semaphore_t sem;
	uint32_t param;
	int ret;
} param_pak_t;


bk_err_t media_send_msg(media_msg_t *msg);
bk_err_t media_app_send_msg(media_msg_t *msg);


#ifdef __cplusplus
}
#endif

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
#include "media_evt.h"
#include "lcd_act.h"
#include "frame_buffer.h"

#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>

#include <driver/jpeg_enc.h>
#include <driver/jpeg_enc_types.h>
#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>

#include <soc/mapping.h>

#include <driver/lcd.h>
#include <driver/dma.h>
#include <driver/gpio.h>
#include <driver/jpeg_dec.h>
#include <driver/dma2d.h>
//#include <lcd_dma2d_config.h>
#include <driver/jpeg_dec_types.h>
#include "modules/image_scale.h"
//#include "lcd_blend_config.h"
#include <driver/uvc_camera_types.h>
#include <driver/uvc_camera.h>

#include <driver/pwm.h>
#include "modules/image_scale.h"

#define TAG "rotate"

#if CONFIG_SLAVE_CORE
#define MINOOR_DTCM __attribute__((section(".dtcm_sec_data ")))
#else
#define MINOOR_DTCM
#endif

#if CONFIG_SLAVE_CORE
#define MINOOR_ITCM __attribute__((section(".itcm_sec_code ")))
#else
#define MINOOR_ITCM
#endif


#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

frame_buffer_t *rotate_frame = NULL;
frame_buffer_t *decoder_frame = NULL;

uint32_t lcd_width = 0;
uint32_t lcd_height = 0;

lcd_info_t *lcd_info_ptr = NULL;


#define BLOCK_WIDTH     (60)
#define BLOCK_HEIGHT    (80)
#define BLOCK_SIZE      (BLOCK_WIDTH * BLOCK_HEIGHT * 2)

MINOOR_DTCM uint8_t rx_block[BLOCK_SIZE];
MINOOR_DTCM uint8_t tx_block[BLOCK_SIZE];

MINOOR_ITCM int vuyy_rotate_degree902(unsigned char *vuyy, unsigned char *rotatedVuyy, int width, int height)
{
	int lineDataSize = width * 2;
	int rotatedLineDataSize = height * 2;
	int rotatedVuyyIndex = 0;
	int finalLineStartIndex = (height - 2) * lineDataSize;
	for (int w = 0; w < lineDataSize; w += 4)
	{
		int vuyyStartIndex = finalLineStartIndex + w;
		int offset = 0;
		for (int h = 0; h < height; h += 2)
		{
			/**
			* y1 y2 u1 v1   y3 y4  u2 v2
			*                               ->
			*y5 y6 u3 v3    y7 y8  u4 v4
			*/
			//v1
			*(rotatedVuyy + rotatedVuyyIndex) = *(vuyy + vuyyStartIndex - offset + 3);
			//y1
			*(rotatedVuyy + rotatedVuyyIndex + 1) = *(vuyy + vuyyStartIndex - offset);
			//u3
			*(rotatedVuyy + rotatedVuyyIndex + 2) = *(vuyy + vuyyStartIndex - offset + lineDataSize + 2);
			//y5
			*(rotatedVuyy + rotatedVuyyIndex + 3) = *(vuyy + vuyyStartIndex - offset + lineDataSize);

			//v1
			*(rotatedVuyy + rotatedVuyyIndex + rotatedLineDataSize) = *(vuyy + vuyyStartIndex - offset + 3);
			//y2
			*(rotatedVuyy + rotatedVuyyIndex + rotatedLineDataSize + 1) =  *(vuyy + vuyyStartIndex - offset + 1);
			//u3
			*(rotatedVuyy + rotatedVuyyIndex + rotatedLineDataSize + 2) = *(vuyy + vuyyStartIndex - offset + lineDataSize + 2);
			//y6
			*(rotatedVuyy + rotatedVuyyIndex + rotatedLineDataSize + 3) = *(vuyy + vuyyStartIndex - offset + lineDataSize + 1);

			rotatedVuyyIndex += 4;
			offset += lineDataSize * 2;
		}
		rotatedVuyyIndex += rotatedLineDataSize;
	}
	return 0;
}

void rotate_complete(frame_buffer_t *frame)
{
	media_msg_t msg;
	msg.event = EVENT_LCD_ROTATE_RIGHT_COMP_EVT;
	msg.param = (uint32_t)frame;
	media_send_msg(&msg);
}


MINOOR_ITCM void memcpy_word(uint32_t *dst, uint32_t *src, uint32_t size)
{
	uint32_t i = 0;

	for (i = 0; i < size; i++)
	{
		dst[i] = src[i];
	}
}


MINOOR_ITCM void lcd_act_rotate_degree90(uint32_t param)
{
	lcd_info_ptr = (lcd_info_t *)param;
	uint32_t i, j, k;
	uint8_t *cp_ptr = NULL;
	uint16_t src_width, src_height;

	decoder_frame = lcd_info_ptr->decoder_frame;
	rotate_frame = lcd_info_ptr->rotate_frame;

	src_width = lcd_info_ptr->src_pixel_x;
	src_height = lcd_info_ptr->src_pixel_y;

	lcd_width = lcd_info_ptr->lcd_pixel_x;
	lcd_height = lcd_info_ptr->lcd_pixel_y;

	LOGD("camera %d:%d, lcd %d:%d\n", src_width, src_height, lcd_width, lcd_height);


	if (lcd_width == src_width
	    && lcd_width == src_width)
	{
		LOGD("do not rotate\n");
		memcpy_word((uint32_t *)rotate_frame->frame, (uint32_t *)decoder_frame->frame, decoder_frame->length / 4);
		rotate_complete(rotate_frame);
		return;
	}

	//bk_gpio_pull_up(GPIO_8);

	for (j = 0; j < (src_height / BLOCK_HEIGHT); j++)
	{

		for (i = 0; i < (src_width / BLOCK_WIDTH); i++)
		{
			for (k = 0; k < BLOCK_HEIGHT; k++)
			{
				cp_ptr = decoder_frame->frame + i * BLOCK_WIDTH * 2 + j * BLOCK_HEIGHT * src_width * 2 + k * src_width * 2;
				memcpy_word((uint32_t *)(rx_block + BLOCK_WIDTH * 2 * k), (uint32_t *)cp_ptr, BLOCK_WIDTH * 2 / 4);
			}

			vuyy_rotate_degree902(rx_block, tx_block, BLOCK_WIDTH, BLOCK_HEIGHT);

			for (k = 0; k < BLOCK_WIDTH; k++)
			{
				cp_ptr = rotate_frame->frame + (src_height / BLOCK_HEIGHT - j - 1) * BLOCK_HEIGHT * 2 + (i) * BLOCK_WIDTH * src_height * 2 + k * src_height * 2;
				memcpy_word((uint32_t *)cp_ptr, (uint32_t *)(tx_block + BLOCK_HEIGHT * 2 * k), BLOCK_HEIGHT * 2 / 4);
			}
		}
	}

	//bk_gpio_pull_down(GPIO_8);

#if CONFIG_SLAVE_CORE
	rotate_complete(rotate_frame);
#endif
}


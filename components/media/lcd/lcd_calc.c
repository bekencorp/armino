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

#include <soc/mapping.h>
#include "modules/image_scale.h"
#include <components/jpeg_decode.h>
#include <modules/tjpgd.h>

#if CONFIG_ARCH_RISCV && CONFIG_CACHE_ENABLE
#include "cache.h"
#endif


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

#if CONFIG_SLAVE_CORE
#define MINOOR_ITCM_BSS __attribute__((section(".itcm_sec_bss ")))
#else
#define MINOOR_ITCM_BSS
#endif

#include "mailbox_channel.h"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

frame_buffer_t *rotate_frame = NULL;
frame_buffer_t *decoder_frame = NULL;

uint32_t lcd_width = 0;
uint32_t lcd_height = 0;

lcd_info_t *lcd_info_ptr = NULL;

/**<
	case img_width=864  block_width=108
	case img_width=800/480  block_width=160
*/

typedef struct {
	uint16_t width;
	uint16_t height;
}block_ppi_t;

#define MAX_BLOCK_WIDTH     (80)
#define MAX_BLOCK_HEIGHT    (40)

const block_ppi_t block_ppi_aray[] = {
	{54, 40},
	{80, 40},

	{MAX_BLOCK_WIDTH, MAX_BLOCK_HEIGHT}
};

MINOOR_DTCM uint16_t block_width;
MINOOR_DTCM uint16_t block_height;
MINOOR_DTCM uint16_t block_size;


#define BLOCK_SIZE      (MAX_BLOCK_WIDTH * MAX_BLOCK_HEIGHT * 2)
MINOOR_DTCM uint8_t rx_block[BLOCK_SIZE];
MINOOR_ITCM_BSS uint8_t tx_block[BLOCK_SIZE];

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

MINOOR_ITCM void lcd_jpeg_dec_sw(uint32_t param)
{
#if CONFIG_JPEG_DECODE
	bk_err_t ret = BK_FAIL;
	mb_chnl_cmd_t mb_cmd;
	sw_jpeg_dec_res_t result;
	jd_output_format *format = (jd_output_format *)param;

	jd_set_output_format(format);

	ret = bk_jpeg_dec_sw_start(decoder_frame->length, decoder_frame->frame, rotate_frame->frame, &result);

	if (ret != BK_OK)
	{
		LOGE("%s sw decoder error\n", __func__);
	}
	else
	{
		rotate_frame->height = result.pixel_y;
		rotate_frame->width = result.pixel_x;
	}

	mb_cmd.hdr.cmd = EVENT_LCD_DEC_SW_MBRSP;
	mb_cmd.param1 = (uint32_t)decoder_frame;
	mb_cmd.param2 = (uint32_t)rotate_frame;
	mb_cmd.param3 = ret;

	mb_chnl_write(MB_CHNL_VID, &mb_cmd);
#endif
}


MINOOR_ITCM void lcd_act_vuyy_resize(uint32_t param)
{
	register uint32_t i;
	register uint8_t *cp_ptr = NULL;
	mb_chnl_cmd_t mb_cmd;

	uint32_t src_copy_byte;
	uint32_t dst_copy_byte;
	uint32_t max_width, max_copy_row, max_dst_copy_row;/*need according BLOCK_SIZE and max_width and src_heigth/dst_height to set*/

#if 0
	register uint8_t *dst_frame_temp = rotate_frame->frame + 0x4000000;
	register uint8_t *src_frame_temp = decoder_frame->frame + 0x4000000;
#else
	register uint8_t *dst_frame_temp = rotate_frame->frame;
	register uint8_t *src_frame_temp = decoder_frame->frame;
#endif

	if (decoder_frame->width >= rotate_frame->width)
	{
		max_width = decoder_frame->width;
	}
	else
	{
		max_width = rotate_frame->width;
	}

	// copy max_copy_row line one time

	int (*func)(uint8_t *srcVuyy, uint8_t *dstVuyy, uint8_t row_count);

	if (rotate_frame->width == 800 && rotate_frame->height == 480)
	{
		max_copy_row = BLOCK_SIZE / max_width / 2;
		max_dst_copy_row = max_copy_row;
		func = vuyy_image_vga_to_lvga;
	}
	else if (rotate_frame->width == 480 && rotate_frame->height == 320)
	{
		max_copy_row = 3;
		max_dst_copy_row = 2;
		func = vuyy_image_vga_to_rsvga;
	}
	else if (rotate_frame->width == 320 && rotate_frame->height == 240)
	{
		max_copy_row = 4;
		max_dst_copy_row = 2;
		func = vuyy_image_vga_to_qvga;
	}
	else
	{
		max_copy_row = BLOCK_SIZE / (max_width * 2);
		max_dst_copy_row = 6;
		func = NULL;//vuyy_image_resize;
		goto error;
	}

	src_copy_byte = decoder_frame->width * 2 * max_copy_row;
	dst_copy_byte = rotate_frame->width * 2 * max_dst_copy_row;

	LOGD("src:%d-%d, dst:%d-%d, src_row:dst_row:%d-%d\r\n", decoder_frame->width, decoder_frame->height, rotate_frame->width,
		rotate_frame->height, max_copy_row, max_dst_copy_row);

#if 0
	flush_dcache(src_frame_temp, JPEG_DEC_FRAME_SIZE);
	flush_dcache(dst_frame_temp, JPEG_DEC_FRAME_SIZE);
#endif

	for (i = 0; i < decoder_frame->height / max_copy_row; i++)
	{
		cp_ptr = src_frame_temp + i * src_copy_byte;
		memcpy_word((uint32_t *)rx_block, (uint32_t *)cp_ptr, src_copy_byte >> 2);

		func(rx_block, tx_block, max_copy_row);

		cp_ptr = dst_frame_temp + i * dst_copy_byte;
		memcpy_word((uint32_t *)cp_ptr, (uint32_t *)tx_block, dst_copy_byte >> 2);
	}

error:

	mb_cmd.hdr.cmd = EVENT_LCD_RESIZE_MBRSP;
	mb_cmd.param1 = (uint32_t)decoder_frame;
	mb_cmd.param2 = (uint32_t)rotate_frame;
	mb_cmd.param3 = 1;

	mb_chnl_write(MB_CHNL_VID, &mb_cmd);
}

MINOOR_ITCM void lcd_act_rotate_degree90(uint32_t param)
{
	register uint32_t i, j, k;
	register uint8_t *cp_ptr = NULL;
	register uint16_t src_width, src_height;
	mb_chnl_cmd_t mb_cmd;

	int (*func)(unsigned char *vuyy, unsigned char *rotatedVuyy, int width, int height);
	src_width = decoder_frame->width;
	src_height = decoder_frame->height;

	switch ((src_width << 16) | src_height)
	{
		case PPI_864X480:
			block_width = block_ppi_aray[0].width;
			block_height = block_ppi_aray[0].height;
			block_size = block_width * block_height * 2;
			break;

		default:
			block_width = block_ppi_aray[1].width;
			block_height = block_ppi_aray[1].height;
			block_size = block_width * block_height * 2;
			break;
	};
	
	register uint8_t *dst_frame_temp = rotate_frame->frame + 0x4000000;
	register uint8_t *src_frame_temp = decoder_frame->frame + 0x4000000;

	switch (rotate_frame->fmt)
	{
		case PIXEL_FMT_VUYY:

			if (param == ROTATE_90)
			{
				func = vuyy_rotate_degree90_to_yuyv;
			}
			else if (param == ROTATE_270)
			{
				func = vuyy_rotate_degree270_to_yuyv;
			}
			else if (param == ROTATE_180)
			{
				func = vuyy_rotate_degree180_to_yuyv;
			}
			else
				func = vuyy_rotate_degree90_to_yuyv;

			rotate_frame->fmt = PIXEL_FMT_YUYV;
			break;
		case PIXEL_FMT_YUYV:
		default:
			if (param == ROTATE_90)
			{
				func = yuyv_rotate_degree90_to_yuyv;
			}
			else if (param == ROTATE_270)
			{
				func = yuyv_rotate_degree270_to_yuyv;
			}
			else if (param == ROTATE_180)
			{
				func = yuyv_rotate_degree180_to_yuyv;
			}
			else
				func = yuyv_rotate_degree90_to_yuyv;
			rotate_frame->fmt = PIXEL_FMT_YUYV;
			break;
	}

#if CONFIG_ARCH_RISCV && CONFIG_CACHE_ENABLE
	flush_dcache(src_frame_temp, JPEG_DEC_FRAME_SIZE);
	flush_dcache(dst_frame_temp, JPEG_DEC_FRAME_SIZE);
#endif

	//LOGI("width:-%d-%d, height:%d-%d\r\n", src_width, rotate_frame->height, src_height, rotate_frame->width);
	if (param == ROTATE_180)
	{
		const int line_bytes = src_width << 1;
		const int rot_line_num = src_height;
		const int revert_Bytes = 4;
		const int totalBytes = src_width * src_height << 1;
		const int copy_line_word = src_width >> 1;
		register uint8_t *dst_addr1_end = rx_block + line_bytes - revert_Bytes;
		register uint8_t *dst_addr2_end = dst_frame_temp + totalBytes - line_bytes;
	
		for(i = 0; i < rot_line_num; i++)
		{
			memcpy_word((uint32_t *)tx_block, (uint32_t *)(src_frame_temp + i*line_bytes), copy_line_word);
			func(tx_block, dst_addr1_end, src_width, 1);
			memcpy_word((uint32_t *)(dst_addr2_end - (i*line_bytes)), (uint32_t *)rx_block, copy_line_word);
		}
	}
	else {
	if (src_width == rotate_frame->height && src_height == rotate_frame->width)
	{
		// rotate a complete frame
		for (j = 0; j < (src_height / block_height); j++)
		{

			for (i = 0; i < (src_width / block_width); i++)
			{
				for (k = 0; k < block_height; k++)
				{
					cp_ptr = src_frame_temp + i * block_width * 2 + j * block_height * src_width * 2 + k * src_width * 2;
					memcpy_word((uint32_t *)(rx_block + block_width * 2 * k), (uint32_t *)cp_ptr, block_width * 2 / 4);
				}

				func(rx_block, tx_block, block_width, block_height);

				for (k = 0; k < block_width; k++)
				{
					if (param == ROTATE_90)
					{
						cp_ptr = dst_frame_temp + (src_height / block_height - j - 1) * block_height * 2 + (i) * block_width * src_height * 2 + k * src_height * 2;
						memcpy_word((uint32_t *)cp_ptr, (uint32_t *)(tx_block + block_height * 2 * k), block_height * 2 / 4);
					}
					else //270
					{
						cp_ptr = dst_frame_temp + (src_width / block_width - 1 - i) * block_width * src_height * 2 + block_height * j * 2 + k * src_height * 2;
						memcpy_word((uint32_t *)cp_ptr, (uint32_t *)(tx_block + block_height * 2 * k), block_height * 2 / 4);
					}
				}


			}
		}
	}
	else
	{
		// ensure the roi range need rotate
		uint16_t width_top_left = (src_width - rotate_frame->height) >> 2;
		uint16_t height_top_left = (src_height - rotate_frame->width) >> 2;
		//uint16_t width_bottom_right = width_top_left + rotate_frame->height;
		//uint16_t height_bottom_right = height_top_left + rotate_frame->width;
		src_frame_temp += height_top_left * src_width * 2 + width_top_left * 2;

		for (j = 0; j < (rotate_frame->width / block_height); j++)
		{

			for (i = 0; i < (rotate_frame->height / block_width); i++)
			{
				for (k = 0; k < block_height; k++)
				{
					cp_ptr = src_frame_temp + i * block_width * 2 + j * block_height * src_width * 2 + k * src_width * 2;
					memcpy_word((uint32_t *)(rx_block + block_width * 2 * k), (uint32_t *)cp_ptr, block_width * 2 / 4);
				}

				func(rx_block, tx_block, block_width, block_height);

				for (k = 0; k < block_width; k++)
				{
					cp_ptr = dst_frame_temp + (rotate_frame->width / block_height - j - 1) * block_height * 2 + (i) * block_width * rotate_frame->width * 2 + k * rotate_frame->width * 2;
					memcpy_word((uint32_t *)cp_ptr, (uint32_t *)(tx_block + block_height * 2 * k), block_height * 2 / 4);
				}
			}
		}
	}}

	mb_cmd.hdr.cmd = EVENT_LCD_ROTATE_MBRSP;
	mb_cmd.param1 = (uint32_t)decoder_frame;
	mb_cmd.param2 = (uint32_t)rotate_frame;
	mb_cmd.param3 = 1;

	mb_chnl_write(MB_CHNL_VID, &mb_cmd);


}

static void lcd_calc_mailbox_rx_isr(void *param, mb_chnl_cmd_t *cmd_buf)
{
	//LOGI("%s, %08X\n", __func__, cmd_buf->hdr.cmd);

	if (cmd_buf->hdr.cmd == EVENT_LCD_ROTATE_MBCMD)
	{
		decoder_frame = (frame_buffer_t *)cmd_buf->param1;
		rotate_frame = (frame_buffer_t *)cmd_buf->param2;

		media_msg_t msg;
		msg.event = EVENT_LCD_ROTATE_RIGHT_CMD;
		msg.param = cmd_buf->param3;
		media_send_msg(&msg);
	}

	if (cmd_buf->hdr.cmd == EVENT_LCD_RESIZE_MBCMD)
	{
		decoder_frame = (frame_buffer_t *)cmd_buf->param1;
		rotate_frame = (frame_buffer_t *)cmd_buf->param2;

		media_msg_t msg;
		msg.event = EVENT_LCD_RESIZE_CMD;
		media_send_msg(&msg);
	}

    if (cmd_buf->hdr.cmd == EVENT_LCD_DEC_SW_MBCMD)
	{
		decoder_frame = (frame_buffer_t *)cmd_buf->param1;
		rotate_frame = (frame_buffer_t *)cmd_buf->param2;

		media_msg_t msg;
		msg.event = EVENT_LCD_DEC_SW_CMD;
		msg.param = cmd_buf->param3;
		media_send_msg(&msg);
	}
}


static void lcd_calc_mailbox_tx_isr(void *param)
{
	LOGD("%s\n", __func__);

}

static void lcd_calc_mailbox_tx_cmpl_isr(void *param, mb_chnl_ack_t *ack_buf)
{
	LOGD("%s\n", __func__);

}

void lcd_calc_init(void)
{
	mb_chnl_open(MB_CHNL_VID, NULL);
	mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_RX_ISR, lcd_calc_mailbox_rx_isr);
	mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_TX_ISR, lcd_calc_mailbox_tx_isr);
	mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_TX_CMPL_ISR, lcd_calc_mailbox_tx_cmpl_isr);
#if CONFIG_JPEG_DECODE
	bk_jpeg_dec_sw_init();
#endif
}

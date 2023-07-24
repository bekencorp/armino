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

#include <stdlib.h>
#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <math.h>
#include "arch_interrupt.h"
#include "rott_ll_macro_def.h"
#include <driver/media_types.h>
#include <driver/rott_types.h>
#include "bk_misc.h"
#include <driver/int.h>
#include "sys_driver.h"

#define TAG "rott_drv"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static bool s_rott_driver_is_init = false;

#define ROTT_RETURN_ON_NOT_INIT() do {\
	if (!s_rott_driver_is_init) {\
		return BK_ERR_ROTT_NOT_INIT;\
	}\
} while(0)

#define ROTT_MAX_SIZE 10
/*< params: picture_xpixel, picture_ypixel, block_xpixel, block_ypixel, block_cnt, block_xpixel*block_ypixel max 4800 */
uint16_t TABLE_ROTT_BLOCK[ROTT_MAX_SIZE][5] = {
	{640, 480, 80, 60, 64},
	{480, 640, 60, 80, 64},
	{480, 800, 60, 80, 80},
	{800, 480, 80, 60, 80},
	{864, 480, 54, 80, 96},
	{480, 320, 60, 80, 32},
	{1280, 720, 80, 60, 190},
	{320, 480, 80, 60, 32},

};


#if (USE_ROTT_REGISTER_CALLBACKS == 1)
typedef struct
{
	rott_isr_t rott_complete_handler;
	rott_isr_t rott_watermark_block_handler;
	rott_isr_t rott_cfg_err_handler;
} rott_driver_t;


static rott_driver_t s_rott = {0};

bk_err_t  bk_rott_isr_register(rott_int_type_t int_type, rott_isr_t isr)
{
	if (int_type & ROTATE_COMPLETE_INT)
	{
		s_rott.rott_complete_handler = isr;
	}
	if (int_type & ROTATE_WARTERMARK_INT)
	{
		s_rott.rott_watermark_block_handler = isr;
	}
	if (int_type & ROTATE_CFG_ERR_INT)
	{
		s_rott.rott_cfg_err_handler = isr;
	}

	return BK_OK;
}

void rott_cb_isr(void)
{
	if (rott_ll_get_int_status_rotate_finish() == 1)
	{
		rott_ll_set_int_status_rotate_finish(1);
		if (s_rott.rott_complete_handler)
		{
			s_rott.rott_complete_handler();
		}
	}
	if (rott_ll_get_int_status_rotate_wtmk() == 1)
	{
		rott_ll_set_int_status_rotate_wtmk(1);
		if (s_rott.rott_watermark_block_handler)
		{
			s_rott.rott_watermark_block_handler();
		}
	}
	if (rott_ll_get_int_status_rotate_cfg_err() == 1)
	{
		rott_ll_set_int_status_rotate_cfg_err(1);
		if (s_rott.rott_cfg_err_handler)
		{
			s_rott.rott_cfg_err_handler();
		}
	}
}

#else
bk_err_t bk_rott_isr_register(rott_isr_t rott_isr)
{
	bk_int_isr_register(INT_SRC_ROTT, rott_isr, NULL);
	return BK_OK;
}


uint32_t bk_rott_int_status_get(void)
{
	return rott_ll_get_int_status_value();
}


bk_err_t bk_rott_int_status_clear(rott_int_type_t int_type)
{
	switch(int_type)
	{
		case ROTATE_COMPLETE_INT:
			rott_ll_set_int_status_rotate_finish(1);
			break;
		case ROTATE_WARTERMARK_INT:
			rott_ll_set_int_status_rotate_wtmk(1);
			break;
		case ROTATE_CFG_ERR_INT:
			rott_ll_set_int_status_rotate_cfg_err(1);
			break;
		default:
			break;
	}
	return BK_OK;
}
#endif
bk_err_t bk_rott_driver_init(void)
{
	bk_err_t ret = BK_OK;
	if (s_rott_driver_is_init) {
		LOGE("%s already init. \n", __func__);
		return BK_OK;
	}

#if (USE_ROTT_REGISTER_CALLBACKS == 1)
	bk_int_isr_register(INT_SRC_ROTT, rott_cb_isr, NULL);
#endif
	s_rott_driver_is_init = true;
	return ret;
}

bk_err_t bk_rott_driver_deinit(void)
{
	if (!s_rott_driver_is_init) {
		LOGE("%s, already deinit. \n", __func__);
		return BK_OK;
	}
	sys_hal_set_rott_int_en(0);
	bk_int_isr_unregister(INT_SRC_ROTT);

	s_rott_driver_is_init = false;
	return BK_OK;
}

bk_err_t bk_rott_set_module_contol_clk_gate(uint32_t en)
{
	rott_ll_set_module_contol_clk_gate(en);
	return BK_OK;
}

bk_err_t bk_rott_int_enable(rott_int_type_t int_type, bool en)
{
	sys_drv_int_group2_enable(ROTT_INTERRUPT_CTRL_BIT);
	//sys_hal_set_rott_int_en(en);

	if (int_type & ROTATE_COMPLETE_INT)
	{
		rott_ll_set_rotate_ctrl_rotate_done_int_ena(en);
	}
	if (int_type & ROTATE_WARTERMARK_INT)
	{
		rott_ll_set_rotate_ctrl_rotate_wtmk_int_ena(en);
	}
	if (int_type & ROTATE_CFG_ERR_INT)
	{
		rott_ll_set_rotate_ctrl_rotate_cfg_err_int_ena(en);
	}
	return BK_OK;
}


bk_err_t bk_rott_mode_config(media_rotate_t rot_mode)
{
	switch (rot_mode)
	{
		case YUV2RGB565:
			rott_ll_set_rotate_ctrl_rotate_bps(1);
			rott_ll_set_rotate_fmt_rotate_anticlock(0);
			break;
		case ROTATE_90:
			rott_ll_set_rotate_ctrl_rotate_bps(0);
			rott_ll_set_rotate_fmt_rotate_anticlock(0);
			break;
		case ROTATE_270:
			rott_ll_set_rotate_ctrl_rotate_bps(0);
			rott_ll_set_rotate_fmt_rotate_anticlock(1);
			break;
		default:
			rott_ll_set_rotate_ctrl_rotate_bps(0);
			rott_ll_set_rotate_fmt_rotate_anticlock(0);
			break;
	}
	return BK_OK;
}



bk_err_t bk_rott_input_data_format(pixel_format_t rot_input_fmt)
{
	switch (rot_input_fmt)
	{
		case PIXEL_FMT_RGB565_LE:
			rott_ll_set_rotate_fmt_yuv_fmt(0);
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(0);
			break;
		case PIXEL_FMT_RGB565:
			rott_ll_set_rotate_fmt_yuv_fmt(0);
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(1);
			break;
		case PIXEL_FMT_YUYV:
			rott_ll_set_rotate_fmt_yuv_fmt(1);
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(0);
			break;
		case PIXEL_FMT_UYVY:
			rott_ll_set_rotate_fmt_yuv_fmt(2);
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(0);
			break;
		case PIXEL_FMT_YYUV:
			rott_ll_set_rotate_fmt_yuv_fmt(3);
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(0);
			break;
		case PIXEL_FMT_UVYY:
			rott_ll_set_rotate_fmt_yuv_fmt(4);
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(0);
			break;
		case PIXEL_FMT_VUYY:
			rott_ll_set_rotate_fmt_yuv_fmt(5);
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(0);
			break;
		default:
			break;
	}
	return BK_OK;
}

bk_err_t bk_rott_data_reverse(rott_input_data_flow_t input_flow, rott_output_data_flow_t output_flow)
{
	switch(input_flow)
	{
		case ROTT_INPUT_REVESE_BYTE_BY_BYTE:
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(0);
			rott_ll_set_rotate_fmt_pfc_i_byte_reve(1);
			break;

		case ROTT_INPUT_REVESE_HALFWORD_BY_HALFWORD:
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(1);
			rott_ll_set_rotate_fmt_pfc_i_byte_reve(0);
			break;
		case ROTT_INPUT_NORMAL:
		default:
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(0);
			rott_ll_set_rotate_fmt_pfc_i_byte_reve(0);
			break;
	}

	switch(output_flow)
	{
		case ROTT_OUTPUT_REVESE_HALFWORD_BY_HALFWORD:
			rott_ll_set_rotate_fmt_pfc_o_hword_reve(1);
			break;

		case ROTT_OUTPUT_NORMAL:
		default:
			rott_ll_set_rotate_fmt_pfc_o_hword_reve(0);
			break;
	}
	return BK_OK;
}


bk_err_t bk_rott_block_rotate_config(uint16_t picture_xpixel, uint16_t picture_ypixel, uint16_t block_xpixel, uint16_t block_ypixel, uint16_t block_cnt)
{
	if ((picture_xpixel == 0) || (picture_ypixel == 0) || (block_xpixel ==  0) || (block_ypixel == 0) || (block_cnt == 0))
	{
		LOGE("%s rotate pixel and rotate block is zaro .\n", __func__);
		return BK_FAIL;
	}
	rott_ll_set_block_resolu_blk_line_pixel(block_xpixel);
	rott_ll_set_block_resolu_blk_clum_pixel(block_ypixel);
	rott_ll_set_picture_resolu_pic_line_pixel(picture_xpixel);
	rott_ll_set_picture_resolu_pic_clum_pixel(picture_ypixel);
	rott_ll_set_block_count_rotate_blk_count(block_cnt);
	return BK_OK;
}

/* rott_rw_addr_set
*  rott rd pic addr set, rott wr pic addr set.
*/
bk_err_t bk_rott_wr_addr_config(uint32_t rd_base_addr, uint32_t wr_base_addr)
{
	rott_ll_set_rd_addr_value(rd_base_addr);
	rott_ll_set_wr_addr_value(wr_base_addr);
	return BK_OK;
}

	/* rott_wtmk_blk_set
	*  set waterwark int block postion.
	*/
bk_err_t bk_rott_wartermark_block_config(uint16_t wtmk_block)
{
	if (rott_ll_get_rotate_ctrl_rotate_wtmk_int_ena() == 1)
	{
		if (0 == wtmk_block)
		{
			LOGD("%s: block num is 0, so disable watermark int\n", __func__);
			rott_ll_set_rotate_ctrl_rotate_wtmk_int_ena(0);
		}
		else
		{
			rott_ll_set_block_count_wtmk_clum_pixel(wtmk_block);
		}
	}
	else
	{
		return BK_FAIL;
	}
	return BK_OK;
}

/* rott_ena
*  enable rott module.
*/
bk_err_t bk_rott_enable(void)
{
	rott_ll_set_rotate_ctrl_rotate_ena(1);
	return BK_OK;
}

/*reset function,
 *soft rest*/
bk_err_t bk_rott_soft_rst(void)
{
	rott_ll_set_module_contol_soft_reset(0);
	delay(10);
	rott_ll_set_module_contol_soft_reset(1);
	return BK_OK;
}



bk_err_t rott_config(rott_config_t *rott)
{
	int i = 0;
	if ((rott->picture_xpixel == 0) || (rott->picture_xpixel == 0))
		return BK_FAIL;

	bk_rott_mode_config(rott->rot_mode);
	bk_rott_input_data_format(rott->input_fmt);
	bk_rott_data_reverse(rott->input_flow, rott->output_flow);

	if ((rott->block_xpixel == 0) || (rott->block_ypixel == 0) || (rott->block_cnt == 0))
	{
		for (i = 0; i < ROTT_MAX_SIZE; i++)
		{
			if ((rott->picture_xpixel << 16 | rott->picture_ypixel) == (TABLE_ROTT_BLOCK[i][0] << 16 | TABLE_ROTT_BLOCK[i][1]))
			{
				break;
			}
		}
		bk_rott_block_rotate_config(rott->picture_xpixel, rott->picture_ypixel, TABLE_ROTT_BLOCK[i][2], TABLE_ROTT_BLOCK[i][3], TABLE_ROTT_BLOCK[i][4]);
	}
	else
		bk_rott_block_rotate_config(rott->picture_xpixel, rott->picture_ypixel, rott->block_xpixel, rott->block_ypixel, rott->block_cnt);

	bk_rott_wartermark_block_config(rott->watermark_blk);
	if ((rott->input_addr == NULL) || (rott->output_addr == NULL))
	{
		if (rott->input_addr == NULL)
			LOGE("%s: rott->input_addr == NULL\n", __func__);
		else
			LOGE("%s:rott->output_addr == NULL \n", __func__);
		return BK_FAIL;
	}
	bk_rott_wr_addr_config((uint32_t)rott->input_addr, (uint32_t)rott->output_addr);
	return BK_OK;
}

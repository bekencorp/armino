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

/***********************************************************************************************************************************
* This file is generated from BK7236_ADDR_Mapping_s.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7236,GENARATE TIME:2023-03-20 20:21:54                                                 
************************************************************************************************************************************/

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

/***********************************************************************************************************************************
* This file is generated from BK7236_ADDR_Mapping_s.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7236,GENARATE TIME:2023-03-20 20:21:54                                                 
************************************************************************************************************************************/

                            
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


bool rott_param_check(int value)
{
	if (value == 0)
	{
		LOGE("block or pixel is zero \n");
		return false;
	}
	return true;
}
#define CHECK_ROTT_PARAM(value)     rott_param_check(value)



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
		if (s_rott.rott_complete_handler)
		{
			s_rott.rott_complete_handler();
		}
		rott_ll_set_int_status_rotate_finish(1);
	}
	if (rott_ll_get_int_status_rotate_wtmk() == 1)
	{
		if (s_rott.rott_watermark_block_handler)
		{
			s_rott.rott_watermark_block_handler();
		}
		rott_ll_set_int_status_rotate_wtmk(1);
	}
	if (rott_ll_get_int_status_rotate_cfg_err() == 1)
	{
		if (s_rott.rott_cfg_err_handler)
		{
			s_rott.rott_cfg_err_handler();
		}
		rott_ll_set_int_status_rotate_cfg_err(1);
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
		rott_ll_set_rotate_ctrl_rotate_wtmk_int_ena(en);
	}
	return BK_OK;
}


bk_err_t bk_rott_mode_config(rott_mode_t rot_mode)
{
	switch (rot_mode)
	{
		case NO_ROTATE_ONLY_YUV2RGB565:
			rott_ll_set_rotate_ctrl_rotate_bps(1);
			break;
		case CLOKEWISE_ROTATE90_YUV2RGB565:
			rott_ll_set_rotate_ctrl_rotate_bps(0);
			rott_ll_set_rotate_fmt_rotate_anticlock(0);
			break;
		case ANTICLOCK_ROTATE90_YUV2RGB565:
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
		case PIXEL_FMT_RGB565:
			rott_ll_set_rotate_fmt_yuv_fmt(0);
		rott_ll_set_rotate_fmt_pfc_i_hword_reve(0);
			break;
		case PIXEL_FMT_RGB565_HF_FLIP:
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

bk_err_t bk_rott_input_data_reverse(rott_input_data_flow_t input_flow,  rott_output_data_flow_t output_flow)
{
	switch(input_flow)
	{
		case INPUT_NORMAL:
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(0);
			rott_ll_set_rotate_fmt_pfc_i_byte_reve(0);
			break;

		case INPUT_REVESE_BYTE_BY_BYTE:
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(0);
			rott_ll_set_rotate_fmt_pfc_i_byte_reve(1);
			break;

		case INPUT_REVESE_HALFWORD_BY_HALFWORD:
			rott_ll_set_rotate_fmt_pfc_i_hword_reve(1);
			rott_ll_set_rotate_fmt_pfc_i_byte_reve(0);
			break;

			default:
			break;
	}
	
	switch(output_flow)
	{
		case OUTPUT_NORMAL:
			rott_ll_set_rotate_fmt_pfc_o_hword_reve(0);
			break;
		case OUTPUT_REVESE_HALFWORD_BY_HALFWORD:
			rott_ll_set_rotate_fmt_pfc_o_hword_reve(1);
			break;
		default:
			break;
	}
	return BK_OK;
}


bk_err_t bk_rott_block_rotate_config(uint16_t picture_line_pixel, uint16_t picture_clum_pixel, uint16_t block_line_pixel, uint16_t block_clum_pixel, uint16_t block_cnt)
{
	CHECK_ROTT_PARAM(picture_line_pixel);
	CHECK_ROTT_PARAM(picture_clum_pixel);
	CHECK_ROTT_PARAM(block_line_pixel);
	CHECK_ROTT_PARAM(block_clum_pixel);
	CHECK_ROTT_PARAM(block_cnt);

	rott_ll_set_block_resolu_blk_line_pixel(block_line_pixel);
	rott_ll_set_block_resolu_blk_clum_pixel(block_clum_pixel);
	rott_ll_set_picture_resolu_pic_line_pixel(picture_line_pixel);
	rott_ll_set_picture_resolu_pic_clum_pixel(picture_clum_pixel);
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
		if (false == CHECK_ROTT_PARAM(wtmk_block))
		{
			rott_ll_set_rotate_ctrl_rotate_wtmk_int_ena(0);
		}
		rott_ll_set_block_count_wtmk_clum_pixel(wtmk_block);
	}
	else
	{
		LOGW(" watermark interrupt not open \n");
		return BK_FAIL;
	}

	return BK_OK;
}

/* rott_ena
*  enable rott module.
*/		
bk_err_t bk_rott_enable(bool en)
{
	rott_ll_set_rotate_ctrl_rotate_ena(en);
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



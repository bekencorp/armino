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

#include <common/bk_include.h>
#include <soc/soc.h>
#include <driver/hal/hal_dma2d_types.h>
#include "dma2d_ll.h"
#include "dma2d_hal.h"

dma2d_hal_t s_dma2d_hal;

bk_err_t dma2d_hal_init(DMA2D_HandleTypeDef *dma2d)
{
	s_dma2d_hal.hw = (dma2d_hw_t *)SOC_DMA2D_REG_BASE;

	// 000:m2m; 001:m2m pixel convert with fg; 010:m2m blend; 011:r2m.only with output; 100: m2m blend fix fg; 101:m2m blend fix bg;
	dma2d_ll_set_dma2d_control_reg_mode(s_dma2d_hal.hw, dma2d->init.Mode);

	dma2d_ll_set_dma2d_control_reg_line_offset_mode(s_dma2d_hal.hw, dma2d->init.LineOffsetMode);

	//0:argb888; 1:rgb888; 010:rgb565; 011:argb1555; 100:argb444
	dma2d_ll_set_out_pfc_contrl_out_color_mode(s_dma2d_hal.hw, dma2d->init.ColorMode);
	
	dma2d_ll_set_out_pfc_contrl_out_alpha_invert(s_dma2d_hal.hw, dma2d->init.AlphaInverted);
	dma2d_ll_set_output_offset_out_line_offset(s_dma2d_hal.hw, dma2d->init.OutputOffset);
	dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_rb_swap(s_dma2d_hal.hw, dma2d->init.RedBlueSwap);
	return BK_OK;
}

bk_err_t dma2d_hal_deinit(void)
{
	s_dma2d_hal.hw = (dma2d_hw_t *)SOC_DMA2D_REG_BASE;	

	// 000:m2m; 001:m2m pixel convert with fg; 010:m2m blend; 011:r2m.only with output; 100: m2m blend fix fg; 101:m2m blend fix bg;
	dma2d_ll_set_dma2d_control_reg_value(s_dma2d_hal.hw,0);
	dma2d_ll_set_dma2d_int_clear_value(s_dma2d_hal.hw, 0x3f);
	//0:argb888; 1:rgb888; 010:rgb565; 011:argb1555; 100:argb444
	dma2d_ll_set_out_pfc_contrl_value(s_dma2d_hal.hw, 0);
	dma2d_ll_set_output_offset_value(s_dma2d_hal.hw, 0);
	dma2d_ll_set_dma2d_fg_address_value(s_dma2d_hal.hw, 0);
	dma2d_ll_set_dma2d_bg_address_value(s_dma2d_hal.hw, 0);
	dma2d_ll_set_dma2d_fg_pfc_ctrl_value(s_dma2d_hal.hw, 0);
	dma2d_ll_set_dma2d_fg_pfc_ctrl_value(s_dma2d_hal.hw, 0);

	return BK_OK;
}


/**
  * @brief  Set the DMA2D transfer parameters.
  * @param  dma2d     Pointer to a DMA2D_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA2D.
  * @param  src_addr      The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  Width      The width of data to be transferred from source to destination.(number of pixel every line)
  * @param  Height     The height of data to be transferred from source to destination.(number of line)
  * @retval HAL status, uint32_t src_addr, uint32_t dst_addr, uint32_t width, uint32_t height
  */
bk_err_t dma2d_hal_config(DMA2D_HandleTypeDef *dma2d, uint32_t src_addr, uint32_t dst_addr, uint32_t width, uint32_t height)
{
	uint32_t tmp;
	uint32_t tmp1;
	uint32_t tmp2;
	uint32_t tmp3;
	uint32_t tmp4;

	dma2d_ll_set_dma2d_number_of_line_pixel_line(s_dma2d_hal.hw, width);
	dma2d_ll_set_dma2d_number_of_line_number_line(s_dma2d_hal.hw, height);
	dma2d_ll_set_dma2d_out_mem_address_out_mem_address(s_dma2d_hal.hw, dst_addr);

	if (dma2d->init.Mode == DMA2D_R2M) {
		tmp1 = src_addr & DMA2D_OCOLR_ALPHA_1;
		tmp2 = src_addr & DMA2D_OCOLR_RED_1;
		tmp3 = src_addr & DMA2D_OCOLR_GREEN_1;
		tmp4 = src_addr & DMA2D_OCOLR_BLUE_1;
		/* Prepare the value to be written to the OCOLR register according to the color mode */
		if (dma2d->init.ColorMode == DMA2D_OUTPUT_ARGB8888) {
			tmp = (tmp3 | tmp2 | tmp1| tmp4);
		}
		else if (dma2d->init.ColorMode == DMA2D_OUTPUT_RGB888) {
			tmp = (tmp3 | tmp2 | tmp4);
		}
		else if (dma2d->init.ColorMode == DMA2D_OUTPUT_RGB565) {
			tmp2 = (tmp2 >> 19U);
			tmp3 = (tmp3 >> 10U);
			tmp4 = (tmp4 >> 3U );
			tmp  = ((tmp3 << 5U) | (tmp2 << 11U) | tmp4);
		}
		else if (dma2d->init.ColorMode == DMA2D_OUTPUT_ARGB1555) {
			tmp1 = (tmp1 >> 31U);
			tmp2 = (tmp2 >> 19U);
			tmp3 = (tmp3 >> 11U);
			tmp4 = (tmp4 >> 3U );
			tmp  = ((tmp3 << 5U) | (tmp2 << 10U) | (tmp1 << 15U) | tmp4);
		}
		else /* Ddma2d->Init.ColorMode = DMA2D_OUTPUT_ARGB4444 */ {
			tmp1 = (tmp1 >> 28U);
			tmp2 = (tmp2 >> 20U);
			tmp3 = (tmp3 >> 12U);
			tmp4 = (tmp4 >> 4U );
			tmp  = ((tmp3 << 4U) | (tmp2 << 8U) | (tmp1 << 12U) | tmp4);
		}
		dma2d_ll_set_out_color_reg_output_clor_reg(s_dma2d_hal.hw, tmp);
	}
	else if (dma2d->init.Mode == DMA2D_M2M_BLEND_FG) /*M2M_blending with fixed color FG DMA2D Mode selected*/ {
		dma2d_ll_set_dma2d_bg_address_value(s_dma2d_hal.hw, src_addr);
	}

	else /* M2M, M2M_PFC,M2M_Blending or M2M_blending with fixed color BG DMA2D Mode */ {
		/* Configure DMA2D source address */
		dma2d_ll_set_dma2d_fg_address_value(s_dma2d_hal.hw, src_addr);
	}

	return BK_OK;
}


bk_err_t dma2d_hal_start_transfer(bool start_transfer)
{
	dma2d_ll_set_dma2d_control_reg_tran_start(s_dma2d_hal.hw, start_transfer); 
	return BK_OK;
}

bool dma2d_hal_is_transfer_done(void)
{
	return dma2d_ll_get_dma2d_control_reg_tran_start(s_dma2d_hal.hw);
}

/**
  * @brief  Start the multi-source DMA2D Transfer with interrupt enabled.
  * @param  dma2d     Pointer to a DMA2D_HandleTypeDef structure that contains
  *                     the configuration information for the DMA2D.
  * @param  src_addr1 The source memory Buffer address for the foreground layer.
  * @param  src_addr2 The source memory Buffer address for the background layer.
  * @param  dst_addr  The destination memory Buffer address.
  * @param  Width       The width of data to be transferred from source to destination (expressed in number of pixels per line).
  * @param  Height      The height of data to be transferred from source to destination (expressed in number of lines).
  * @retval HAL status
  */
bk_err_t dma2d_hal_blending_start(DMA2D_HandleTypeDef *dma2d, uint32_t src_addr1, uint32_t src_addr2, uint32_t dst_addr, uint32_t Width,  uint32_t Height)
{
	if(dma2d->init.Mode == DMA2D_M2M_BLEND_FG)
	{
		/*blending & fixed FG*/
		dma2d_ll_set_dma2d_fg_color_reg_value(s_dma2d_hal.hw, src_addr1);
		/* Configure the source, destination address and the data size */
		dma2d_hal_config(dma2d, src_addr2, dst_addr, Width, Height);
	}
	else if (dma2d->init.Mode == DMA2D_M2M_BLEND_BG)
	{
		/*blending & fixed BG*/
		dma2d_ll_set_dma2d_bg_color_reg_value(s_dma2d_hal.hw, src_addr2);
		/* Configure the source, destination address and the data size */
		dma2d_hal_config(dma2d, src_addr1, dst_addr, Width, Height);
	}
	else
	{
		dma2d_ll_set_dma2d_bg_address_bg_address(s_dma2d_hal.hw, src_addr2);
		/* Configure the source, destination address and the data size */
		dma2d_hal_config(dma2d, src_addr1, dst_addr, Width, Height);
	}
	dma2d_ll_set_dma2d_control_reg_tran_start(s_dma2d_hal.hw, 1); 
	return BK_OK;
}


bk_err_t dma2d_hal_suspend(bool suspend)
{
	dma2d_ll_set_dma2d_control_reg_tran_suspend(s_dma2d_hal.hw, suspend);
	return BK_OK;
}

/**
  * @brief  Configure the DMA2D CLUT Transfer.
  * @param  dma2d   Pointer to a DMA2D_HandleTypeDef structure that contains
  *                   the configuration information for the DMA2D.
  * @param  CLUTCfg  Pointer to a DMA2D_CLUTCfgTypeDef structure that contains
  *                   the configuration information for the color look up table.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @note API obsolete and maintained for compatibility with legacy. User is invited
  *      to resort to HAL_DMA2D_CLUTStartLoad() instead to benefit from code compactness,
  *      code size and improved heap usage.
  * @retval HAL status
  */
bk_err_t dma2d_hal_clut_config(DMA2D_CLUTCfgTypeDef CLUTCfg, uint32_t LayerIdx)
{
	if(LayerIdx == DMA2D_BACKGROUND_LAYER)
	{
		/* Write background CLUT memory address */
		dma2d_ll_set_bg_clut_mem_address_value(s_dma2d_hal.hw, (uint32_t)CLUTCfg.pCLUT);
		/* Write background CLUT size and CLUT color mode */
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_clut_color_mode(s_dma2d_hal.hw, CLUTCfg.CLUTColorMode);
//		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_start_clut();
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_clut_size(s_dma2d_hal.hw, CLUTCfg.Size);
	}
	/* Configure the CLUT of the foreground DMA2D layer */
	else
	{
		/* Write foreground CLUT memory address */
		dma2d_ll_set_fg_clut_mem_address_value(s_dma2d_hal.hw,  (uint32_t)CLUTCfg.pCLUT);
		/* Write foreground CLUT size and CLUT color mode */
		dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_color_mode(s_dma2d_hal.hw, CLUTCfg.CLUTColorMode);
		dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_clut_size(s_dma2d_hal.hw, CLUTCfg.Size);
	}
	return BK_OK;
}

/**
  * @brief  Configure the DMA2D Layer according to the specified
  *         parameters in the DMA2D_HandleTypeDef.
  * @param  dma2d Pointer to a DMA2D_HandleTypeDef structure that contains
  *                 the configuration information for the DMA2D.
  * @param  LayerIdx DMA2D Layer index.
  *                   This parameter can be one of the following values:
  *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
  * @retval HAL status
  */
bk_err_t dma2d_hal_layer_config(DMA2D_HandleTypeDef *dma2d, uint32_t LayerIdx)
{
	DMA2D_LAYER_CFG *pLayerCfg;
	
	pLayerCfg = &dma2d->LayerCfg[LayerIdx];

	/* Configure the background DMA2D layer */
	if (LayerIdx == DMA2D_BACKGROUND_LAYER) {
		 /* Write DMA2D BGPFCCR register */
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_color_mode(s_dma2d_hal.hw, pLayerCfg->InputColorMode);
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_alpha_mode(s_dma2d_hal.hw, pLayerCfg->AlphaMode);
		dma2d_ll_set_dma2d_bg_pfc_ctrl_alpha_invert(s_dma2d_hal.hw, pLayerCfg->AlphaInverted);
		dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_rb_swap(s_dma2d_hal.hw, pLayerCfg->RedBlueSwap);

		if (pLayerCfg->AlphaMode == DMA2D_REPLACE_ALPHA) {
			dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_alpha(s_dma2d_hal.hw, pLayerCfg->InputAlpha);
		}
		if (pLayerCfg->AlphaMode == DMA2D_COMBINE_ALPHA) {
			dma2d_ll_set_dma2d_bg_pfc_ctrl_bg_alpha(s_dma2d_hal.hw, (pLayerCfg->InputAlpha & dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_alpha(s_dma2d_hal.hw)));
		}

		/* DMA2D BGOR register configuration -------------------------------------*/
		dma2d_ll_set_dma2d_bg_offset_bg_line_offset(s_dma2d_hal.hw, pLayerCfg->InputOffset);

		/* DMA2D BGCOLR register configuration -------------------------------------*/
		if ((pLayerCfg->InputColorMode == DMA2D_INPUT_A4) || (pLayerCfg->InputColorMode == DMA2D_INPUT_A8))
		{
			dma2d_ll_set_dma2d_bg_color_reg_value(s_dma2d_hal.hw, pLayerCfg->Inputcolor);
		}
	}
	 /* Configure the foreground DMA2D layer */
	else {
		/* Write DMA2D FGPFCCR register */
		dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_color_mode(s_dma2d_hal.hw, pLayerCfg->InputColorMode);
		dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_alpha_mode(s_dma2d_hal.hw, pLayerCfg->AlphaMode);
		dma2d_ll_set_dma2d_fg_pfc_ctrl_alpha_invert(s_dma2d_hal.hw, pLayerCfg->AlphaInverted);
		dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_rb_swap(s_dma2d_hal.hw, pLayerCfg->RedBlueSwap);

		if (pLayerCfg->AlphaMode == DMA2D_REPLACE_ALPHA) {
			dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_alpha(s_dma2d_hal.hw, pLayerCfg->InputAlpha);
		}
		if (pLayerCfg->AlphaMode == DMA2D_COMBINE_ALPHA) {
			dma2d_ll_set_dma2d_fg_pfc_ctrl_fg_alpha(s_dma2d_hal.hw, (pLayerCfg->InputAlpha & dma2d_ll_get_dma2d_fg_pfc_ctrl_fg_alpha(s_dma2d_hal.hw)));
		}

		/* DMA2D FGOR register configuration -------------------------------------*/
		dma2d_ll_set_dma2d_fg_offset_fg_line_offset(s_dma2d_hal.hw, pLayerCfg->InputOffset);
	
		/* DMA2D FGCOLR register configuration -------------------------------------*/
		if ((pLayerCfg->InputColorMode == DMA2D_INPUT_A4) || (pLayerCfg->InputColorMode == DMA2D_INPUT_A8)) {
			dma2d_ll_set_dma2d_bg_color_reg_value(s_dma2d_hal.hw, pLayerCfg->Inputcolor);
		}
	}
	return BK_OK;
}


bk_err_t dma2d_hal_abort(bool abort)
{
	dma2d_ll_set_dma2d_control_reg_tran_abort(s_dma2d_hal.hw, abort);
	return BK_OK;
}


bk_err_t dma2d_hal_int_config(DMA2D_INT_TYPE int_type, bool enable)
{
	uint32_t int_value;

	int_value = dma2d_ll_get_dma2d_control_reg_value(s_dma2d_hal.hw);
	
	if (1 == enable) {
		dma2d_ll_set_dma2d_control_reg_value(s_dma2d_hal.hw, int_value | int_type);
	}
	else {
		dma2d_ll_set_dma2d_control_reg_value(s_dma2d_hal.hw, int_value & ~(int_type));
	}

	return BK_OK;
}

bk_err_t dma2d_hal_int_status_get(void)
{
	return dma2d_ll_get_dma2d_int_status_value(s_dma2d_hal.hw);
}

bk_err_t dma2d_hal_int_status_clear(DMA2D_INT_STATUS int_status)
{
	dma2d_ll_set_dma2d_int_clear_value(s_dma2d_hal.hw, int_status);
	return BK_OK;
}

/**
  * @brief  Configure the line watermark.
  * @param  Line   Line Watermark configuration (maximum 16-bit long value expected).
  * @note   dma2d_hal_line_Watermar_cfg() API used with the transfer watermark interrupt.
  * @note   The transfer watermark interrupt is disabled once it has occurred.
  * @retval HAL status
  */
bk_err_t dma2d_hal_line_Watermar_cfg(uint32_t Line)
{
	if (Line > 0xFFFF) {
		return BK_FAIL;
	}
	else {
		dma2d_ll_set_dma2d_line_watermark_line_watermark(s_dma2d_hal.hw, Line);
	}
	return BK_OK;
}

/**
  * @brief  Start the multi-source DMA2D Transfer.
  * @param  dma2d      Pointer to a DMA2D_HandleTypeDef structure that contains
  *                      the configuration information for the DMA2D.
  * @param  SrcAddress1 The source memory Buffer address for the foreground layer.
  * @param  SrcAddress2 The source memory Buffer address for the background layer.
  * @param  DstAddress  The destination memory Buffer address.
  * @param  Width       The width of data to be transferred from source to destination (expressed in number of pixels per line).
  * @param  Height      The height of data to be transferred from source to destination (expressed in number of lines).
  * @retval HAL status
  */
bk_err_t dma2d_hal_blending_config(uint32_t SrcAddress1, uint32_t SrcAddress2, uint32_t DstAddress, uint32_t Width, uint32_t Height)
{
	return dma2d_ll_get_dma2d_int_status_value(s_dma2d_hal.hw);
	return BK_OK;
}


/**
  * @brief Configure dead time.
  * @note The dead time value represents the guaranteed minimum number of cycles between
  *       two consecutive transactions on the AHB bus.
  * @param DeadTime dead time value.
  * @param en.
  * @retval HAL status
  */
bk_err_t dma2d_hal_deadtime_config(uint8_t DeadTime, bool en)
{
	dma2d_ll_set_dma2d_master_time_config_master_time_ena(s_dma2d_hal.hw, en);
	dma2d_ll_set_dma2d_master_time_config_dead_time(s_dma2d_hal.hw, DeadTime);

	return BK_OK;
}



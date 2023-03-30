// Copyright 2022-2023 Beken
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

#include "jpeg_hal.h"
#include "yuv_buf_hal.h"
#include "yuv_buf_ll.h"

static jpeg_hal_t s_jpeg_hal = {
	.hw = (jpeg_hw_t *)JPEG_LL_REG_BASE(0),
};

static bk_err_t yuv_buf_hal_set_config_common(yuv_buf_hal_t *hal, const yuv_buf_config_t *config)
{
	yuv_buf_ll_set_mclk_div(hal->hw, config->mclk_div);
	yuv_buf_ll_set_x_pixel(hal->hw, config->x_pixel);
	yuv_buf_ll_set_y_pixel(hal->hw, config->y_pixel);
	yuv_buf_ll_set_frame_blk(hal->hw, config->x_pixel * config->y_pixel / 2);

	yuv_buf_ll_enable_sync_edge_dect(hal->hw);
	yuv_buf_ll_set_encode_begin_hsync_posedge(hal->hw);
	yuv_buf_ll_enable_int(hal->hw);

	return BK_OK;
}

bk_err_t yuv_buf_hal_init(yuv_buf_hal_t *hal)
{
	hal->hw = (yuv_buf_hw_t *)YUV_BUF_LL_REG_BASE;
	yuv_buf_ll_init(hal->hw);
	return BK_OK;
}

bk_err_t yuv_buf_hal_set_yuv_mode_config(yuv_buf_hal_t *hal, const yuv_buf_config_t *config)
{
	yuv_buf_hal_set_config_common(hal, config);

	yuv_buf_ll_set_yuv_format(hal->hw, config->yuv_mode_cfg.yuv_format);
	/* yuv_mode set em_base_addr as psram */
	yuv_buf_ll_set_em_base_addr(hal->hw, SOC_PSRAM_DATA_BASE);

	return BK_OK;
}

bk_err_t yuv_buf_hal_set_jpeg_mode_config(yuv_buf_hal_t *hal, const yuv_buf_config_t *config)
{
	yuv_buf_hal_set_config_common(hal, config);
	/* jpeg_mode set em_base_addr as share memory */
	yuv_buf_ll_set_em_base_addr(hal->hw, jpeg_hal_get_jpeg_share_mem_addr());

	return BK_OK;
}

bk_err_t yuv_buf_hal_set_h264_mode_config(yuv_buf_hal_t *hal, const yuv_buf_config_t *config)
{
	yuv_buf_ll_set_mclk_div(hal->hw, config->mclk_div);
	yuv_buf_ll_set_x_pixel(hal->hw, config->x_pixel);
	yuv_buf_ll_set_y_pixel(hal->hw, config->y_pixel);
	yuv_buf_ll_set_frame_blk(hal->hw, config->x_pixel*config->y_pixel/2);
	yuv_buf_ll_set_yuv_format(hal->hw, config->yuv_mode_cfg.yuv_format);
	yuv_buf_ll_enable_int(hal->hw);

	return BK_OK;
}

bk_err_t yuv_buf_hal_enable_h264_nosensor_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_enable_reverse_yuv_data(hal->hw);
	yuv_buf_ll_enable_bps_cis(hal->hw);
	yuv_buf_ll_enable_memrev(hal->hw);

	return BK_OK;
}

bk_err_t yuv_buf_hal_start_yuv_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_enable_yuv_buf_mode(hal->hw);
	yuv_buf_ll_disable_h264_encode_mode(hal->hw);
	return BK_OK;
}

bk_err_t yuv_buf_hal_stop_yuv_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_disable_yuv_buf_mode(hal->hw);
	return BK_OK;
}

bk_err_t yuv_buf_hal_start_jpeg_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_disable_yuv_buf_mode(hal->hw);
	yuv_buf_ll_disable_h264_encode_mode(hal->hw);
	jpeg_hal_enable_jpeg_mode(&s_jpeg_hal);
	return BK_OK;
}

bk_err_t yuv_buf_hal_stop_jpeg_mode(yuv_buf_hal_t *hal)
{
	jpeg_hal_disable_jpeg_mode(&s_jpeg_hal);
	return BK_OK;
}

bk_err_t yuv_buf_hal_start_h264_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_enable_h264_encode_mode(hal->hw);
	return BK_OK;
}

bk_err_t yuv_buf_hal_stop_h264_mode(yuv_buf_hal_t *hal)
{
	yuv_buf_ll_disable_h264_encode_mode(hal->hw);
	return BK_OK;
}

bk_err_t yuv_buf_hal_set_partial_display_offset_config(yuv_buf_hal_t *hal, const yuv_buf_partial_offset_config_t *offset_config)
{
	yuv_buf_ll_set_x_partial_offset_l(hal->hw, offset_config->x_partial_offset_l);
	yuv_buf_ll_set_x_partial_offset_r(hal->hw, offset_config->x_partial_offset_r);
	yuv_buf_ll_set_y_partial_offset_l(hal->hw, offset_config->y_partial_offset_l);
	yuv_buf_ll_set_y_partial_offset_r(hal->hw, offset_config->y_partial_offset_r);

	return BK_OK;
}


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

#include "jpeg_ll.h"
#include "jpeg_hal.h"
#include <driver/hal/hal_jpeg_types.h>
#include <driver/jpeg_enc_types.h>

#define JPEG_BITRATE_MAX_SIZE_320_240     (20 * 1024)
#define JPEG_BITRATE_MIN_SIZE_320_240     (5 * 1024)

#define JPEG_BITRATE_MAX_SIZE_640_480     (35 * 1024)//30
#define JPEG_BITRATE_MIN_SIZE_640_480     (20 * 1024)//10

#define JPEG_BITRATE_MAX_SIZE_1280_720    (50 * 1024)
#define JPEG_BITRATE_MIN_SIZE_1280_720    (30 * 1024)

#define JPEG_BITRATE_MAX_SIZE             JPEG_BITRATE_MAX_SIZE_640_480
#define JPEG_BITRATE_MIN_SIZE             JPEG_BITRATE_MIN_SIZE_640_480


__attribute__((section(".video_spec_data"))) uint8_t video_sram[40*1024];
#define JPEG_SHARE_MEM                    (&video_sram[0])


bk_err_t jpeg_hal_init(jpeg_hal_t *hal)
{
	hal->hw = (jpeg_hw_t *)JPEG_LL_REG_BASE(hal->id);
	jpeg_ll_init(hal->hw);
	return BK_OK;
}

static void jpeg_hal_set_target_bitrate(jpeg_hal_t *hal, uint32_t x_pixel)
{
	switch(x_pixel){
	case X_PIXEL_320:
		jpeg_ll_set_target_high_byte(hal->hw, JPEG_BITRATE_MAX_SIZE_320_240);
		jpeg_ll_set_target_low_byte(hal->hw, JPEG_BITRATE_MIN_SIZE_320_240);
		break;
	case X_PIXEL_640:
		jpeg_ll_set_target_high_byte(hal->hw, JPEG_BITRATE_MAX_SIZE_640_480);
		jpeg_ll_set_target_low_byte(hal->hw, JPEG_BITRATE_MIN_SIZE_640_480);
		break;
	case X_PIXEL_1280:
	case X_PIXEL_1600:
		//jpeg_ll_enable_bitrate_ctrl(hal->hw, 1);
		jpeg_ll_set_target_high_byte(hal->hw, JPEG_BITRATE_MAX_SIZE_1280_720);
		jpeg_ll_set_target_low_byte(hal->hw, JPEG_BITRATE_MIN_SIZE_1280_720);
		break;
	default:
		os_printf("Not adapt this image resolution\r\n");
		jpeg_ll_set_target_high_byte(hal->hw, JPEG_BITRATE_MAX_SIZE);
		jpeg_ll_set_target_low_byte(hal->hw, JPEG_BITRATE_MIN_SIZE);
		break;
	}
}

bk_err_t jpeg_hal_set_em_base_addr(jpeg_hal_t *hal, uint8_t *address)
{
	jpeg_ll_set_em_base_addr(hal->hw, (uint32_t)address);
	return BK_OK;
}

bk_err_t jpeg_hal_set_yuv_config(jpeg_hal_t *hal, const jpeg_config_t *config)
{
	jpeg_ll_init(hal->hw);
	jpeg_ll_clear_config(hal->hw);

	jpeg_ll_enable_end_yuv_int(hal->hw);
	jpeg_ll_enable_vsync_negedge_int(hal->hw);
	jpeg_ll_set_mclk_div(hal->hw, config->mclk_div);

	jpeg_ll_enable_sync_edge_dect(hal->hw);

	jpeg_ll_set_x_pixel(hal->hw, config->x_pixel);
	jpeg_ll_set_y_pixel(hal->hw, config->y_pixel);
	jpeg_ll_enable_yuv_word_reverse(hal->hw);
	jpeg_ll_set_yuv_mode(hal->hw, 1);
	jpeg_ll_set_em_base_addr(hal->hw, PSRAM_BASEADDR);//PSRAM_BASEADDR

	return BK_OK;
}

bk_err_t jpeg_hal_set_encode_config(jpeg_hal_t *hal, const jpeg_config_t *config)
{
	jpeg_ll_init(hal->hw);
	jpeg_ll_clear_config(hal->hw);

	jpeg_ll_enable_end_frame_int(hal->hw);
	jpeg_ll_set_mclk_div(hal->hw, config->mclk_div);

	jpeg_ll_enable_sync_edge_dect(hal->hw);

	jpeg_ll_set_x_pixel(hal->hw, config->x_pixel);
	jpeg_ll_set_y_pixel(hal->hw, config->y_pixel);
	jpeg_ll_init_quant_table(hal->hw);
	jpeg_hal_set_target_bitrate(hal, config->x_pixel);

	jpeg_ll_set_default_bitrate_step(hal->hw);
	jpeg_ll_enable_video_byte_reverse(hal->hw);
	jpeg_ll_enable_enc_size(hal->hw);
	jpeg_ll_set_em_base_addr(hal->hw, (uint32_t)JPEG_SHARE_MEM);
	jpeg_ll_enable(hal->hw);

	return BK_OK;
}

bk_err_t jpeg_hal_enable_clk(jpeg_hal_t *hal, uint32_t value)
{
	jpeg_ll_set_mclk_div(hal->hw, value);
	jpeg_ll_enable(hal->hw);
	return BK_OK;
}

bk_err_t jpeg_hal_yuv_fmt_sel(jpeg_hal_t *hal, uint32_t value)
{
	jpeg_ll_yuv_fml_sel(hal->hw, value);
	return BK_OK;
}

bk_err_t jpeg_hal_start_common(jpeg_hal_t *hal, uint8_t mode)
{
	if (mode)
		jpeg_ll_set_yuv_mode(hal->hw, 1);
	else
		jpeg_ll_enable(hal->hw);
	return BK_OK;
}

bk_err_t jpeg_hal_stop_common(jpeg_hal_t *hal, uint8_t mode)
{
	if (mode)
		jpeg_ll_set_yuv_mode(hal->hw, 0);
	else
		jpeg_ll_disable(hal->hw);
	return BK_OK;
}

bk_err_t jpeg_hal_enable_partial_display(jpeg_hal_t *hal, uint32_t enable)
{
	if (enable) {
		jpeg_ll_enable_partial_display(hal->hw);
	} else {
		jpeg_ll_disable_partial_display(hal->hw);
	}

	return BK_OK;
}

bk_err_t jpeg_hal_partial_display_offset_config(jpeg_hal_t *hal, const jpeg_partial_offset_config_t *offset_config)
{
	jpeg_ll_set_x_partial_offset_l(hal->hw, offset_config->x_partial_offset_l);
	jpeg_ll_set_x_partial_offset_r(hal->hw, offset_config->x_partial_offset_r);
	jpeg_ll_set_y_partial_offset_l(hal->hw, offset_config->y_partial_offset_l);
	jpeg_ll_set_y_partial_offset_r(hal->hw, offset_config->y_partial_offset_r);

	return BK_OK;
}

bk_err_t jpeg_hal_enable_bitrate_ctrl(jpeg_hal_t *hal, uint8_t enable)
{
	if (enable)
	{
		jpeg_ll_enable_bitrate_ctrl(hal->hw, 1);
	}
	else
	{
		jpeg_ll_enable_bitrate_ctrl(hal->hw, 0);
	}

	return BK_OK;
}

bk_err_t jpeg_hal_set_target_size(jpeg_hal_t *hal, uint32_t up_size, uint32_t low_size)
{
	jpeg_ll_set_target_high_byte(hal->hw, up_size);
	jpeg_ll_set_target_low_byte(hal->hw, low_size);

	return BK_OK;
}
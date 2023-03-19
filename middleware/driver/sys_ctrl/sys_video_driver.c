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

#include "sys_hal.h"
#include "sys_driver.h"
#include "sys_driver_common.h"

/**  Video Start **/
uint32_t sys_drv_lcd_set(uint8_t clk_src_sel, uint8_t clk_div_l, uint8_t clk_div_h, uint8_t int_en,uint8_t clk_always_on)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_lcd_disp_clk_en(clk_src_sel, clk_div_l,clk_div_h, int_en, clk_always_on);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);

	return ret;
}

uint32_t sys_drv_lcd_close(void)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_lcd_disp_close();

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);

	return ret;
}

uint32_t sys_drv_dma2d_set(uint8_t clk_always_on, uint8_t sys_int_en)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_dma2d_clk_en(clk_always_on, sys_int_en);
	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);

	return ret;
}

uint32_t sys_drv_jpeg_dec_set(uint8_t clk_always_on, uint8_t int_en)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_jpeg_dec_ctrl(clk_always_on, int_en);
	if(!ret)
		ret = sys_amp_res_release();
	rtos_enable_int(int_level);

	return ret;
}

/**  Video End **/

/**  Jpeg Start **/
uint32_t sys_drv_mclk_mux_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_mclk_mux_set(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_set_jpeg_clk_sel(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_jpeg_clk_sel(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_set_clk_div_mode1_clkdiv_jpeg(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_set_clk_div_mode1_clkdiv_jpeg(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_set_jpeg_disckg(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();
	sys_hal_set_jpeg_disckg(value);

	if(!ret)
		ret = sys_amp_res_release();
	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_set_cpu_clk_div_mode1_clkdiv_bus(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_set_cpu_clk_div_mode1_clkdiv_bus(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_video_power_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_video_power_en(value);

	if(!ret)
		ret = sys_amp_res_release();


	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_set_auxs(uint32_t cksel, uint32_t ckdiv)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;

	sys_hal_set_auxs_clk_sel(cksel);
	sys_hal_set_auxs_clk_div(ckdiv);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

/**  Jpeg End **/


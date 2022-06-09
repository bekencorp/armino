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
#include "arch_interrupt.h"
#include "lcd_disp_ll_macro_def.h"
#include "lcd_disp_hal.h"
#include <driver/lcd.h>
#include "gpio_map.h"
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "gpio_map.h"
#include <driver/int.h>
#include "dma_hal.h"
#include <driver/dma.h>
#include "dma_driver.h"
#include "sys_driver.h"
#include <driver/lcd_disp_types.h>
#include "jpeg_dec_macro_def.h"
#include "jpeg_dec_hal.h"
#include <driver/jpeg_dec.h>
#include "driver/jpeg_dec_types.h"
//#include "BK7256_RegList.h"

static JDEC jdec;
volatile uint32_t mcu_idex = 0;
volatile uint32_t mcu_y_num = 0;

#if (USE_JPEG_DEC_COMPLETE_CALLBACKS == 1)
typedef struct {
	jpeg_dec_isr_t callback;
	void *param;
} jpeg_decoder_callback_t;

static jpeg_decoder_callback_t s_jpeg_decoder_isr = {NULL};

static void jpeg_decoder_isr(void);
static void jpeg_decoder_isr_common(void);

#endif

bk_err_t bk_jpeg_dec_driver_init(void)
{
	if(sys_drv_jpeg_dec_set(1, 1) != 0) {
		os_printf("jpeg dec sys clk config error \r\n");
		return BK_FAIL;
	}
#if (USE_JPEG_DEC_COMPLETE_CALLBACKS == 1)
	bk_int_isr_register(INT_SRC_JPEG_DEC, jpeg_decoder_isr, NULL);
#endif
	jpg_decoder_init();

	return BK_OK;
}


bk_err_t bk_jpeg_dec_driver_deinit(void)
{
	if(sys_drv_jpeg_dec_set(0, 0) != 0) {
		os_printf("jpeg dec sys clk config error \r\n");
		return BK_FAIL;
	}
	bk_int_isr_unregister(INT_SRC_JPEG_DEC);
	return BK_OK;
}

bk_err_t bk_jpeg_dec_init(uint32_t * dec_src_addr, uint32_t *dec_dest_addr)
{
	JpegdecInit(&jdec, dec_src_addr);
	jd_decomp(&jdec, 2, dec_src_addr, dec_dest_addr);
	dec_busy2_clr;
	return BK_OK;
}

bk_err_t bk_jpeg_dec_start(void)
{
	//hal_jpeg_dec_start();
	REG_DC_CLR;
	REG_DEC_START;

	return BK_OK;
}

/**
 * @brief  bk_jpeg_dec_isr_register
 * @param1  jpeg_dec_isr
 * @return  none .
 */


#if (USE_JPEG_DEC_COMPLETE_CALLBACKS == 1)
bk_err_t bk_jpeg_dec_complete_cb(jpeg_dec_isr_t isr, void *param)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_jpeg_decoder_isr.callback = isr;
	s_jpeg_decoder_isr.param = param;
	GLOBAL_INT_RESTORE();
	return BK_OK;
}

static void jpeg_decoder_isr(void)
{
	//bk_gpio_set_output_high(GPIO_4);
	uint8_t  bm4;

	mcu_y_num = mcu_y_num +1;
	if(mcu_y_num == X_PIXEL_RGB/4)
	{
		mcu_y_num = 0;
		REG_JPEG_MCUY = REG_JPEG_MCUY + 8;
	}
	mcu_idex++;
	bm4 = mcu_idex % 4;
	REG_JPEG_MCUX = (mcu_y_num >> 2) * 16;

	if((bm4 == 2) || (bm4 == 3))
		REG_JPEG_DCUV = 0x1;
	else
		REG_JPEG_DCUV = 0x0;
	if(mcu_idex == (3000))
	{
		jpeg_decoder_isr_common();
	}
	if(mcu_idex == (4080))
	{
		mcu_idex = 0;
		REG_JPEG_MCUX= 0;;
		REG_JPEG_MCUY = 0;;
		dec_busy2_clr;
		REG_JPEG_ACC_REG0 = 0;;
		//jpeg_decoder_isr_common();
	}
	else
	{
		REG_DEC_START;
		dec_busy2_clr;
	}
	//bk_gpio_set_output_low(GPIO_4);
}

static void jpeg_decoder_isr_common(void)
{
	if (s_jpeg_decoder_isr.callback) {
		s_jpeg_decoder_isr.callback(s_jpeg_decoder_isr.param);
	}

}


#else
bk_err_t  bk_jpeg_dec_isr_register(jpeg_dec_isr_t jpeg_dec_isr)
{
	bk_int_isr_register(INT_SRC_JPEG_DEC, jpeg_dec_isr, NULL);
	return BK_OK;
}
#endif




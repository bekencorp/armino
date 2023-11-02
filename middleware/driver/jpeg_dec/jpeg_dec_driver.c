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
#include "gpio_map.h"
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "gpio_map.h"
#include <driver/int.h>
#include "dma_hal.h"
#include <driver/dma.h>
#include "dma_driver.h"
#include "sys_driver.h"
#include "jpeg_dec_macro_def.h"
#include "jpeg_dec_ll_macro_def.h"
#include "jpeg_dec_hal.h"
#include <driver/jpeg_dec.h>
#include "driver/jpeg_dec_types.h"
#include <driver/hal/hal_jpeg_dec_types.h>
#include <modules/pm.h>
#include "bk_general_dma.h"

#define JPEGDEC_TAG "jpeg hw_decode"
#define JPEGDEC_LOGI(...) BK_LOGI(JPEGDEC_TAG, ##__VA_ARGS__)
#define JPEGDEC_LOGW(...) BK_LOGW(JPEGDEC_TAG, ##__VA_ARGS__)
#define JPEGDEC_LOGE(...) BK_LOGE(JPEGDEC_TAG, ##__VA_ARGS__)
#define JPEGDEC_LOGD(...) BK_LOGD(JPEGDEC_TAG, ##__VA_ARGS__)

#if (USE_JPEG_DEC_COMPLETE_CALLBACKS == 1)
jpeg_dec_isr_cb_t  s_jpeg_dec_isr[DEC_ISR_MAX] = {NULL};
static jpeg_dec_res_t result = {0};
static void jpeg_decoder_isr(void);
#endif

#define JPEGDEC_RETURN_ON_NOT_INIT() do {\
	if (!s_jpegdec_driver_is_init) {\
		return BK_ERR_JPEGDEC_NOT_INIT;\
	}\
} while(0)
static bool s_jpegdec_driver_is_init = false;

#define JPEG_SRAM_ADDRESS (0x30000000)

uint32_t image_ppi = 0;
uint8_t *jpeg_address = NULL;
uint32_t jpeg_size = 0;

#define JPEG_TAIL_SIZE (2)
#define JPEG_DEC_STRIP

uint8_t jpeg_dec_dma = 0;

bk_err_t bk_jpeg_dec_driver_init(void)
{
	if (s_jpegdec_driver_is_init) {
		JPEGDEC_LOGW("%s, jpegdec already init. \n", __func__);
		return BK_OK;
	}

	jpeg_dec_dma = bk_dma_alloc(DMA_DEV_JPEG);
	if ((jpeg_dec_dma < DMA_ID_0) || (jpeg_dec_dma >= DMA_ID_MAX))
	{
		JPEGDEC_LOGE("%s, jpeg dec malloc dma fail \r\n", __func__);
		return BK_FAIL;
	}

	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_JPEG_DE, PM_POWER_MODULE_STATE_ON);
	if(sys_drv_jpeg_dec_set(1, 1) != 0) {
		JPEGDEC_LOGE("jpeg dec sys clk config error \r\n");
		return BK_FAIL;
	}
#if (USE_JPEG_DEC_COMPLETE_CALLBACKS == 1)
	bk_int_isr_register(INT_SRC_JPEG_DEC, jpeg_decoder_isr, NULL);
#endif
	jpg_decoder_init();
	s_jpegdec_driver_is_init = true;
	JPEGDEC_LOGI("hw jpeg dec init, malloc dma channal %d\n", jpeg_dec_dma);
	return BK_OK;
}

bk_err_t bk_jpeg_dec_driver_deinit(void)
{
	if (!s_jpegdec_driver_is_init) {
		JPEGDEC_LOGW("%s, jpegdec already deinit. \n", __func__);
		return BK_OK;
	}

	if(sys_drv_jpeg_dec_set(0, 0) != 0) {
		JPEGDEC_LOGE("jpeg dec sys clk config error \r\n");
		return BK_FAIL;
	}
	bk_int_isr_unregister(INT_SRC_JPEG_DEC);
	jpg_decoder_deinit();
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_JPEG_DE, PM_POWER_MODULE_STATE_OFF);

	bk_dma_stop(jpeg_dec_dma);
	bk_dma_deinit(jpeg_dec_dma);
	bk_dma_free(DMA_DEV_JPEG, jpeg_dec_dma);

	s_jpegdec_driver_is_init = false;
	JPEGDEC_LOGI("hw jpeg dec deinit, free dma channal %d\n", jpeg_dec_dma);

	return BK_OK;
}


bk_err_t bk_jpeg_dec_line_int_en(uint32_t line_en)
{
	jpeg_dec_auto_line_num_int_en(line_en);  //line num mast be multiple of 8
	return BK_OK;
}


bk_err_t bk_jpeg_dec_dma_start(uint32_t length, unsigned char *input_buf, unsigned char * output_buf)
{
	int ret = 0;
	uint32_t left_len = 0;
	JPEGDEC_RETURN_ON_NOT_INIT();

	jpeg_address = input_buf;
	jpeg_size = length;

	if (jpeg_size <= 0xFFFF)
	{
		dma_memcpy_by_chnl((void*)JPEG_SRAM_ADDRESS,
		input_buf,
		(length % 4) ? ((length  / 4 + 1) * 4) : length,
		jpeg_dec_dma);
	}
	else
	{
		dma_memcpy_by_chnl((void*)JPEG_SRAM_ADDRESS,
						  input_buf,
						  0xFFFF,
						  jpeg_dec_dma);

		left_len = length - 0xFFFF;
		dma_memcpy_by_chnl((void*)JPEG_SRAM_ADDRESS + 0xFFFF,
						  input_buf + 0xFFFF,
						  (left_len % 4) ? ((left_len  / 4 + 1) * 4) : left_len,
						  jpeg_dec_dma);
	}

	ret = JpegdecInit(length, (void*)JPEG_SRAM_ADDRESS, output_buf, &image_ppi);
	if(ret != JDR_OK)
	{
		JPEGDEC_LOGE("JpegdecInit error %x \r\n", ret);
		return ret;
	}

	ret = jd_decomp_hw();
	if(ret != JDR_OK)
	{
		JPEGDEC_LOGE("jd_decomp error %x \r\n", ret);
		return ret;
	}
	return JDR_OK;
}

bk_err_t bk_jpeg_dec_hw_start(uint32_t length, unsigned char *input_buf, unsigned char * output_buf)
{
	int ret = 0;
	JPEGDEC_RETURN_ON_NOT_INIT();

	jpeg_address = input_buf;
	jpeg_size = length;

	ret = JpegdecInit(length, input_buf, output_buf, &image_ppi);
	if(ret != JDR_OK)
	{
		JPEGDEC_LOGE("JpegdecInit error %x \r\n", ret);
		return ret;
	}
	ret = jd_decomp_hw();
	if(ret != JDR_OK)
	{
		JPEGDEC_LOGE("jd_decomp error %x \r\n", ret);
		return ret;
	}
	return JDR_OK;
}

bk_err_t bk_jpeg_dec_stop(void)
{
	jpeg_dec_ll_set_reg0x5_mcu_x(0);
	jpeg_dec_ll_set_reg0x6_mcu_y(0);
	jpeg_dec_ll_set_reg0x8_dec_cmd(JPEGDEC_DC_CLEAR);
	jpeg_dec_ll_set_reg0x5f_value(0x1ff);
	jpeg_dec_ll_set_reg0x3_value(0);
	jpeg_dec_ll_set_reg0x0_jpeg_dec_en(0);
	return BK_OK;
}


#if (USE_JPEG_DEC_COMPLETE_CALLBACKS == 1)
bk_err_t bk_jpeg_dec_isr_register(jpeg_dec_isr_type_t isr_id, jpeg_dec_isr_cb_t cb_isr)
{
	if ((isr_id) >= DEC_ISR_MAX)
		return BK_FAIL;

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	if(isr_id == DEC_END_OF_FRAME)
	{
		jpeg_dec_auto_frame_end_int_en(1);
	}
	else
	{
		jpeg_dec_auto_line_num_int_en(1);
	}
	s_jpeg_dec_isr[isr_id] = cb_isr;
	GLOBAL_INT_RESTORE();
	return BK_OK;
}

#ifdef JPEG_DEC_STRIP
bool jpeg_dec_comp_status(uint8_t *src, uint32_t src_size, uint32_t dec_size)
{
	bool ok = false;
	uint32_t i, tail = src_size - 1, strip = 0, max = 50;

	for (i = tail; i > 0 && max > 0; i--, max--)
	{
		if (src[i] == 0xD9
			&& src[i - 1] == 0xFF)
		{
			tail = i - JPEG_TAIL_SIZE;
			break;
		}

		if (src[i] == 0x00)
		{
			strip++;
		}
	}

	for (i = tail; i > 0 && max > 0; i--, max--)
	{
		if (src[i] == 0xFF)
		{
			strip++;
		}
		else
		{
			break;
		}
	}

	if (max > 0
		&& strip + dec_size == src_size - JPEG_TAIL_SIZE)
	{
		ok = true;
	}
	else
	{
		//os_printf("decoder error, %u, %u, %u, %u\n", src_size, dec_size, strip, max);
	}

	return ok;
}
#endif

static void jpeg_decoder_isr(void)
{
	if (jpeg_dec_ll_get_reg0x5f_dec_frame_int_clr()) {

		result.size = jpeg_dec_ll_get_reg0x5d_value();
		result.pixel_x = image_ppi >> 16;
		result.pixel_y = image_ppi & 0xFFFF;

#ifdef JPEG_DEC_STRIP
		result.ok = jpeg_dec_comp_status(jpeg_address, jpeg_size, result.size);
#else
		result.ok = result.size == (jpeg_size - JPEG_TAIL_SIZE);
#endif

		if (jpeg_dec_ll_get_reg0x2_jpeg_dec_linen())  //enable line num en
		{
			if (jpeg_dec_ll_get_reg0x1_mcu_index() == 0)
			{
				jpeg_dec_ll_set_reg0x0_jpeg_dec_en(0);

				register uint32_t rd_cnt = jpeg_dec_ll_get_reg0x5d_master_rd_cnt();
				register uint32_t base_raddr = jpeg_dec_ll_get_reg0x58_base_raddr();
				register uint8_t dri = *((uint8_t *)(base_raddr + rd_cnt));
				if (dri == 0xff)
				{
					dri = *((uint8_t *)(base_raddr + rd_cnt + 1));
					if ((dri == 0xD0) || (dri == 0xD1) || (dri == 0xD2) || (dri == 0xD3) || (dri == 0xD4) || (dri == 0xD5) || (dri == 0xD6) || (dri == 0xD7))
					{
						dri = *(uint8_t *)(base_raddr + rd_cnt + 2);
						if ((dri == 0xD0) || (dri == 0xD1) || (dri == 0xD2) || (dri == 0xD3) || (dri == 0xD4) || (dri == 0xD5) || (dri == 0xD6) || (dri == 0xD7) || (dri == 0x00))
						{
							bk_jpeg_dec_stop();
							return;
						}
					}
				}
				if (s_jpeg_dec_isr[DEC_END_OF_LINE_NUM])
				{
					s_jpeg_dec_isr[DEC_END_OF_LINE_NUM](&result);
				}
			}
			else
			{
				register uint32_t rd_cnt = jpeg_dec_ll_get_reg0x5d_master_rd_cnt();
				register uint32_t base_raddr = jpeg_dec_ll_get_reg0x58_base_raddr();
				register uint8_t dri = *(uint8_t *)(base_raddr + rd_cnt);
				if (dri == 0xff)
				{
					dri = *((uint8_t *)(base_raddr + rd_cnt + 1));
					if ((dri == 0xD0) || (dri == 0xD1) || (dri == 0xD2) || (dri == 0xD3) || (dri == 0xD4) || (dri == 0xD5) || (dri == 0xD6) || (dri == 0xD7))
					{
						dri = *(uint8_t *)(base_raddr + rd_cnt + 2);
						if ((dri == 0xD0) || (dri == 0xD1) || (dri == 0xD2) || (dri == 0xD3) || (dri == 0xD4) || (dri == 0xD5) || (dri == 0xD6) || (dri == 0xD7) || (dri == 0x00))
						{
							bk_jpeg_dec_stop();
							return;
						}
					}
				}
				jpeg_dec_ll_set_reg0x8_dec_cmd(JPEGDEC_START);
			}
			jpeg_dec_ll_set_reg0x5f_dec_frame_int_clr(1);

		}
		else
		{
//			jpeg_dec_ll_set_reg0x0_jpeg_dec_en(3);
			jpeg_dec_ll_set_reg0x5_mcu_x(0);
			jpeg_dec_ll_set_reg0x6_mcu_y(0);
			jpeg_dec_ll_set_reg0x8_dec_cmd(JPEGDEC_DC_CLEAR);
			jpeg_dec_ll_set_reg0x5f_value(0x1ff);
			jpeg_dec_ll_set_reg0x3_value(0);
//			jpeg_dec_ll_set_reg0x5f_dec_frame_int_clr(1);
			jpeg_dec_ll_set_reg0x0_jpeg_dec_en(0);

			if (s_jpeg_dec_isr[DEC_END_OF_FRAME]) {
				s_jpeg_dec_isr[DEC_END_OF_FRAME](&result);
			}
		}
	} else {
		JPEGDEC_LOGE("int status = %x not auto int and line int \r\n", jpeg_dec_ll_get_reg0x5f_value());
	}
}



#else
bk_err_t  bk_jpeg_dec_isr_register(jpeg_dec_isr_t jpeg_dec_isr)
{
	bk_int_isr_register(INT_SRC_JPEG_DEC, jpeg_dec_isr, NULL);
	return BK_OK;
}
#endif




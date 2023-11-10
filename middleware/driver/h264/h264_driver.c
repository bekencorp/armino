// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <driver/dma.h>
#include <driver/int.h>
#include <os/mem.h>
#include <driver/h264.h>
#include <driver/h264_types.h>
#include <driver/gpio.h>
#include "clock_driver.h"
#include "gpio_driver.h"
#include "sys_driver.h"
#include "sys_hal.h"
#include "h264_hal.h"
#include "h264_driver.h"
#include "h264_default_config.h"

typedef struct {
	h264_isr_t isr_handler;
	void *param;
} h264_isr_handler_t;

typedef struct {
	h264_hal_t hal;
	h264_isr_handler_t h264_isr_handler[H264_ISR_MAX];
} h264_driver_t;

#define H264_RETURN_ON_DRIVER_NOT_INIT() do {\
		if (!s_h264_driver_is_init) {\
			return BK_ERR_H264_DRIVER_NOT_INIT;\
		}\
	} while (0)

extern void delay(int num);

static h264_driver_t s_h264 = {0};
static uint8_t h264_dma_rx_id = 0;
static bool s_h264_driver_is_init = false;
static uint32_t h264_int_count[H264_ISR_MAX] = {0};

static void h264_isr(void);

bk_err_t h264_int_enable(void)
{
	uint32_t int_level = rtos_disable_int();
	sys_drv_int_group2_enable(H264_INTERRUPT_CTRL_BIT);
	h264_hal_int_config(&s_h264.hal, H264_INT_ENABLE);
	rtos_enable_int(int_level);
	return BK_OK;
}

static void h264_init_common(void)
{
	/* h264 clock set */
	sys_drv_set_clk_div_mode1_clkdiv_h264(1);
	sys_drv_set_h264_clk_sel(H264_CLK_SEL_480M);
	sys_drv_set_jpeg_clk_en();

	/* h264 AHB bus clock config */
	sys_drv_set_h264_clk_en();
	//sys_drv_mclk_mux_set(H264_AHB_CLK_SEL);
	//sys_drv_h264_set_mclk_div(H264_AHB_CLK_DIV);

	/* h264 global ctrl set */
	h264_hal_set_global_ctrl(&s_h264.hal);
	h264_hal_set_internal_clk_en(&s_h264.hal, 2);
}

bk_err_t bk_h264_pingpong_in_psram_clk_set(void)
{
	h264_hal_set_internal_clk_en(&s_h264.hal, 2);
	return BK_OK;
}

bk_err_t bk_h264_dma_rx_init(h264_dma_config_t *config)
{
	if((config->rx_buf == NULL) || (config->transfer_len <= 0)) {
		return BK_ERR_DMA_INVALID_ADDR;
	}
	dma_config_t dma_config = {0};

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_H264;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = H264_R_RX_FIFO;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_DISABLE;
	dma_config.dst.start_addr = (uint32_t)config->rx_buf;

	h264_dma_rx_id = bk_dma_alloc(DMA_DEV_H264);
	if(h264_dma_rx_id >= DMA_ID_MAX) {
		return BK_ERR_DMA_ID;
	}
	BK_LOG_ON_ERR(bk_dma_init(h264_dma_rx_id, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(h264_dma_rx_id, config->transfer_len));
	#if (CONFIG_SPE)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(h264_dma_rx_id, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(h264_dma_rx_id, DMA_ATTR_SEC));
	#endif
	BK_LOG_ON_ERR(bk_dma_start(h264_dma_rx_id));

	return BK_OK;
}

bk_err_t bk_h264_enc_lcd_dma_cpy(void *out, const void *in, uint32_t len, dma_id_t cpy_chnl)
{
	dma_config_t dma_config = {0};
	os_memset(&dma_config, 0, sizeof(dma_config_t));

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 1;

	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
    dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.src.start_addr = (uint32_t)in;
	dma_config.src.end_addr = (uint32_t)(in + len);

	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
    dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.start_addr = (uint32_t)out;
	dma_config.dst.end_addr = (uint32_t)(out + len);

	BK_LOG_ON_ERR(bk_dma_init(cpy_chnl, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(cpy_chnl, len));
	BK_LOG_ON_ERR(bk_dma_set_src_burst_len(cpy_chnl, 3));
	BK_LOG_ON_ERR(bk_dma_set_dest_burst_len(cpy_chnl, 3));
#if (CONFIG_SPE)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(cpy_chnl, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(cpy_chnl, DMA_ATTR_SEC));
#endif

	return BK_OK;
}

bk_err_t bk_h264_dma_rx_deinit(void)
{
	BK_LOG_ON_ERR(bk_dma_stop(h264_dma_rx_id));
	BK_LOG_ON_ERR(bk_dma_free(DMA_DEV_H264, h264_dma_rx_id));
	h264_dma_rx_id = 0;
	return BK_OK;
}

bk_err_t h264_int_disable(void)
{
	uint32_t int_level = rtos_disable_int();
	h264_hal_int_config(&s_h264.hal, H264_CPU_INT_DISABLE);
	sys_drv_int_group2_disable(H264_INTERRUPT_CTRL_BIT);
	rtos_enable_int(int_level);
	return BK_OK;
}

bk_err_t bk_h264_driver_init(void)
{
	if (s_h264_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_h264, 0, sizeof(s_h264));
	bk_int_isr_register(INT_SRC_H264, h264_isr, NULL);
	h264_hal_init(&s_h264.hal);
	s_h264_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_h264_driver_deinit(void)
{
	if (!s_h264_driver_is_init)
	{
		return BK_OK;
	}

	bk_int_isr_unregister(INT_SRC_H264);
	h264_int_disable();
	bk_h264_reset();
	os_memset(&s_h264, 0, sizeof(s_h264));
	s_h264_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_h264_config_init(const h264_config_t *config, uint16_t media_width, uint16_t media_height)
{
	if(config == NULL) {
		return BK_ERR_H264_INVALID_CONFIG_PARAM;
	}
	h264_init_common();
	delay(1000);
	if(h264_hal_set_img_width(&s_h264.hal, media_width, media_height)) {
		return BK_ERR_H264_INVALID_RESOLUTION_TYPE;
	}
	h264_hal_gop_config(&s_h264.hal, config);
	h264_hal_qp_config(&s_h264.hal, config);
	h264_hal_idc_config(&s_h264.hal, config);
	h264_hal_mb_config(&s_h264.hal, config);
	h264_hal_scale_factor_config(&s_h264.hal);
	h264_hal_vui_config(&s_h264.hal, config);
	h264_hal_cpb_vui_config(&s_h264.hal, config);
	h264_hal_frame_cbr_config(&s_h264.hal, config);
	h264_int_enable();

	return BK_OK;
}

bk_err_t bk_h264_init(media_ppi_t media_ppi)
{
	uint16_t media_width = ppi_to_pixel_x(media_ppi);
	uint16_t media_height = ppi_to_pixel_y(media_ppi);
	if (media_height % 16 != 0) {
		H264_LOGE("h264 input pixel height is not n times of 16!! \r\n");
		return BK_ERR_H264_INVALID_PIXEL_HEIGHT;
	}

	BK_LOG_ON_ERR(bk_h264_config_init(&h264_commom_config, media_width, media_height));

	return BK_OK;
}

bk_err_t bk_h264_set_pframe_num(uint32_t pframe_number)
{
	if(pframe_number > MAX_PFRAME || pframe_number < MIN_PFRAME) {
		return BK_ERR_H264_INVALID_PFRAME_NUMBER;
	}
	h264_config_t param_set = {0};
	param_set.skip_mode = DEFAULT_SKIP_MODE;
	param_set.num_pframe = pframe_number;

	const h264_config_t *para = &param_set;
	h264_hal_gop_config(&s_h264.hal, para);

	return BK_OK;
}

bk_err_t bk_h264_set_qp(uint32_t qp)
{
	if(qp > MAX_QP || qp < MIN_QP) {
		return BK_ERR_H264_INVALID_QP;
	}
	h264_config_t param_set = {0};
	param_set.qp = qp;
	param_set.cqp_off = MIN_QP;
	param_set.tqp = qp;

	const h264_config_t *para = &param_set;
	h264_hal_qp_config(&s_h264.hal,para);

	return BK_OK;
}

bk_err_t bk_h264_set_quality(uint32_t imb_bits, uint32_t pmb_bits)
{
	if(imb_bits > MAX_FRAME_BITS || imb_bits < MIN_FRAME_BITS) {
		return BK_ERR_H264_INVALID_IMB_BITS;
	}
	if(pmb_bits > MAX_FRAME_BITS || pmb_bits < MIN_FRAME_BITS) {
		return BK_ERR_H264_INVALID_PMB_BITS;
	}
	h264_config_t param_set = {0};
	param_set.imb_bits = imb_bits;
	param_set.pmb_bits = pmb_bits;

	const h264_config_t *para = &param_set;
	h264_hal_mb_bits_config(&s_h264.hal, para);

	return BK_OK;
}

bk_err_t bk_h264_deblocking_filter_config_init(const h264_config_t *config, uint32_t alpha_off, uint32_t beta_off)
{
	return h264_hal_filter_config(&s_h264.hal, config, alpha_off, beta_off);
}

bk_err_t bk_h264_encode_enable()
{
	h264_hal_encode_enable(&s_h264.hal);
	return BK_OK;
}

bk_err_t bk_h264_encode_disable()
{
	h264_hal_encode_disable(&s_h264.hal);
	return BK_OK;
}

bk_err_t bk_h264_camera_gpio_set(void)
{
	h264_gpio_map_t h264_gpio_map_table[] = H264_GPIO_MAP;

	for (uint32_t i = 0; i < 10; i++) {
		gpio_dev_unmap(h264_gpio_map_table[i].gpio_id);
		gpio_dev_map(h264_gpio_map_table[i].gpio_id, h264_gpio_map_table[i].dev);
	}

	return BK_OK;
}

bk_err_t bk_h264_camera_clk_enhance(void)
{
	h264_gpio_map_t h264_gpio_map_table[] = H264_GPIO_MAP;

	for (uint32_t i = 10; i < 12; i++) {
		gpio_dev_unmap(h264_gpio_map_table[i].gpio_id);
		gpio_dev_map(h264_gpio_map_table[i].gpio_id, h264_gpio_map_table[i].dev);
	}

	bk_gpio_disable_output(h264_gpio_map_table[0].gpio_id);
	bk_gpio_set_capacity(h264_gpio_map_table[0].gpio_id,1);

	return BK_OK;
}

uint32_t bk_h264_get_encode_count()
{
	return h264_hal_get_encode_count(&s_h264.hal);
}

bk_err_t bk_h264_get_fifo_addr(uint32_t *fifo_addr)
{
	*fifo_addr = H264_R_RX_FIFO;
	return BK_OK;
}

bk_err_t bk_h264_register_isr(h264_isr_type_t type_id, h264_isr_t isr, void *param)
{
	if(type_id >= H264_ISR_MAX) {
		return BK_ERR_H264_ISR_INVALID_ID;
	}
	uint32_t int_level = rtos_disable_int();
	s_h264.h264_isr_handler[type_id].isr_handler = isr;
	s_h264.h264_isr_handler[type_id].param = param;
	rtos_enable_int(int_level);
	return BK_OK;
}

bk_err_t bk_h264_unregister_isr(h264_isr_type_t type_id)
{
	if(type_id >= H264_ISR_MAX) {
		return BK_ERR_H264_ISR_INVALID_ID;
	}
	uint32_t int_level = rtos_disable_int();
	s_h264.h264_isr_handler[type_id].isr_handler = NULL;
	s_h264.h264_isr_handler[type_id].param = NULL;
	rtos_enable_int(int_level);
	return BK_OK;
}

static void h264_isr(void)
{
	uint32_t int_status = h264_hal_get_int_stat(&s_h264.hal);
	h264_hal_int_clear(&s_h264.hal, int_status);

	H264_LOGD("h264 int status: %x \r\n ", int_status);

	if (int_status & INT_SKIP_FRAME)
	{
		H264_INT_INC(h264_int_count,H264_SKIP_FRAME);
		if (s_h264.h264_isr_handler[H264_SKIP_FRAME].isr_handler)
		{
			s_h264.h264_isr_handler[H264_SKIP_FRAME].isr_handler(s_h264.h264_isr_handler[H264_SKIP_FRAME].param);
		}
	}

	if (int_status & INT_FINAL_OUT)
	{
		H264_INT_INC(h264_int_count,H264_FINAL_OUT);
		if (s_h264.h264_isr_handler[H264_FINAL_OUT].isr_handler)
		{
			s_h264.h264_isr_handler[H264_FINAL_OUT].isr_handler(s_h264.h264_isr_handler[H264_FINAL_OUT].param);
		}
	}

	if (int_status & INT_LINE_DONE)
	{
		H264_INT_INC(h264_int_count,H264_LINE_DONE);
		if (s_h264.h264_isr_handler[H264_LINE_DONE].isr_handler)
		{
			s_h264.h264_isr_handler[H264_LINE_DONE].isr_handler(s_h264.h264_isr_handler[H264_LINE_DONE].param);
		}
	}
}

bk_err_t bk_h264_int_count_show(void)
{
	H264_LOGI("h264 int SKIP_FRAME count: %d \r\n ", h264_int_count[H264_SKIP_FRAME]);
	H264_LOGI("h264 int FINAL_OUT count: %d \r\n ", h264_int_count[H264_FINAL_OUT]);
	H264_LOGI("h264 int LINE_DONE count: %d \r\n ", h264_int_count[H264_LINE_DONE]);
	return BK_OK;
}

bk_err_t bk_h264_reset(void)
{
	h264_hal_reset(&s_h264.hal);
	return BK_OK;
}

/**
 * temporarily used for analog register write
*/
static void sys_hal_analog_set_t(uint8_t reg_idx, uint32_t data)
{
	REG_WRITE(0x44010000+(0x40+reg_idx)*4,data);
	while(((REG_READ(0x44010000+0x3a*4)) >> (reg_idx)));
}

bk_err_t bk_h264_clk_check(void)
{
	/*old h264 clk check*/
	/**
	extern void delay(INT32 num);
	volatile uint32_t val = sys_hal_analog_get(0x5);
	val |= (0x1 << 5) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1);
	sys_hal_analog_set_t(0x5, val);

	val = sys_hal_analog_get(0x0);
	val |= (0x1 << 21);
	val |= (0x1 << 19);
	val &= ~(0x1 << 4);
	sys_hal_analog_set_t(0x0, val);

	delay(10);

	val |= (0x1 << 4);
	val &= ~(0x1 << 19);

	sys_hal_analog_set_t(0x0, val);
	return BK_OK;
	**/

	/*new h264 clk check*/
	volatile uint32_t val = sys_hal_analog_get(0x5);
	val |=  (0x1 << 14) | (0x1 << 5) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1);
	sys_hal_analog_set_t(0x5, val);

	val = sys_hal_analog_get(0x0);
	val |= (0x13 << 20) ;
	sys_hal_analog_set_t(0x0, val);

	sys_hal_analog_set_t(0x0,0xF1305B57);
	sys_hal_analog_set_t(0x0,0xF5305B57);
	sys_hal_analog_set_t(0x0,0xF1305B57);
	sys_hal_analog_set_t(0x2, 0x7E003450);
	sys_hal_analog_set_t(0x3, 0xC5F00B88);
	sys_hal_analog_set_t(0x8, 0x57E627FA);
	sys_hal_analog_set_t(0x9, 0x787FC6A4);

	delay(10);
	val = sys_hal_analog_get(0x0);
	val &= ~(0x1 << 19);
	val |= (0x1 << 19);
	sys_hal_analog_set_t(0x0,val);
	delay(100);
		
	val = sys_hal_analog_get(0x0);//ffe7 31a7
	val |= (0x13 << 20) ;
	sys_hal_analog_set_t(0x0, val);

	val = REG_READ((0x44000000+0x41*4));
	val &= ~(0x3 << 10);
	val |= (0x2 << 10);
	REG_WRITE((0x44000000+0x41*4), val);

	val = REG_READ((0x44000000+0x41*4));
	val &= (~0x3);
	val |= 0x3;
	REG_WRITE((0x44000000+0x41*4), val);

	return BK_OK;
}

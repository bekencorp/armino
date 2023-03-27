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
#include <driver/dma.h>
#include <driver/i2c.h>
#include <driver/jpeg_enc.h>
#include <os/mem.h>
#include <driver/timer.h>
#include "bk_arm_arch.h"
#include <components/video_transfer.h>
#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>
#include "bk_drv_model.h"
#include <gpio_map.h>
#include <driver/gpio.h>
#include "gpio_driver.h"


#define EJPEG_DROP_COUNT              (0)
#define EJPEG_DELAY_HTIMER_CHANNEL     5
#define EJPEG_I2C_DEFAULT_BAUD_RATE    I2C_BAUD_RATE_100KHZ
#define EJPEG_DELAY_HTIMER_VAL         (2)  // 2ms
#define EJPEG_DROP_FRAME               (0)

#define TAG "dvp_camera"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


static uint8_t dvp_video_dma = DMA_ID_MAX;
static const dvp_sensor_config_t *current_sensor = NULL;
static video_config_t *dvp_video_config = NULL;
static uint32_t frame_total_len = 0;

static uint8_t jpeg_drop_image = 0;
static beken_semaphore_t ejpeg_sema = NULL;
static volatile uint8_t jpeg_eof_flag = 0;
#if (EJPEG_DROP_FRAME)
static uint8_t vsync_index = 0;
#endif
static void camera_intf_delay_timer_hdl(timer_id_t timer_id)
{
#if CONFIG_GENERAL_DMA
	uint16_t already_len = dvp_video_config->rx_read_len;
	GLOBAL_INT_DECLARATION();
	uint32_t left_len = bk_dma_get_remain_len(dvp_video_dma);
	uint32_t rec_len = dvp_video_config->node_len - left_len;
	uint32_t frame_len = bk_jpeg_enc_get_frame_size();
	frame_total_len += rec_len - 5;

	if (frame_len != frame_total_len) {
		LOGE("jpeg frame len crc error:%d-%d\r\n", frame_len, frame_total_len);
		LOGE("dma_state:%d\r\n", bk_dma_get_enable_status(dvp_video_dma));
	}

	if (jpeg_drop_image == 0)
	{
		if (dvp_video_config && dvp_video_config->node_full_handler && rec_len > 0)
			dvp_video_config->node_full_handler(dvp_video_config->rxbuf + already_len, rec_len, 1, frame_total_len);
	}

	frame_total_len = 0;

	already_len += rec_len;
	if (already_len >= dvp_video_config->rxbuf_len)
		already_len -= dvp_video_config->rxbuf_len;
	GLOBAL_INT_DISABLE();
	dvp_video_config->rx_read_len = already_len;
	GLOBAL_INT_RESTORE();
	// turn off dma, so dma can start from first configure. for easy handler
	bk_dma_stop(dvp_video_dma);
	dvp_video_config->rx_read_len = 0;
	bk_dma_start(dvp_video_dma);
#endif

	if (jpeg_drop_image == 0)
	{
		if (dvp_video_config && dvp_video_config->data_end_handler)
			dvp_video_config->data_end_handler();
	}

	if (jpeg_drop_image != 0)
		jpeg_drop_image--;

#if (!CONFIG_SYSTEM_CTRL)
	bk_timer_disable(EJPEG_DELAY_HTIMER_CHANNEL);
#endif

}

static void camera_intf_start_delay_timer(void)
{
#if (!CONFIG_SYSTEM_CTRL)
	bk_timer_start(EJPEG_DELAY_HTIMER_CHANNEL, EJPEG_DELAY_HTIMER_VAL, camera_intf_delay_timer_hdl);
#else
	camera_intf_delay_timer_hdl(EJPEG_DELAY_HTIMER_CHANNEL);
#endif
}

static void camera_intf_ejpeg_rx_handler(dma_id_t dma_id)
{
	uint16_t already_len = dvp_video_config->rx_read_len;
	uint16_t copy_len = dvp_video_config->node_len;
	frame_total_len += copy_len;
	GLOBAL_INT_DECLARATION();

	if (jpeg_drop_image == 0)
	{
		if (dvp_video_config && dvp_video_config->node_full_handler != NULL)
			dvp_video_config->node_full_handler(dvp_video_config->rxbuf + already_len, copy_len, 0, 0);
	}

	already_len += copy_len;
	if (already_len >= dvp_video_config->rxbuf_len)
		already_len = 0;
	GLOBAL_INT_DISABLE();
	dvp_video_config->rx_read_len = already_len;
	GLOBAL_INT_RESTORE();
}

static void camera_intf_dma_config(void)
{
	dma_config_t dma_config = {0};
	uint32_t jpeg_fifo_addr;

	bk_jpeg_enc_get_fifo_addr(&jpeg_fifo_addr);

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_JPEG;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = jpeg_fifo_addr;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
	dma_config.dst.start_addr = (uint32_t)dvp_video_config->rxbuf;
	dma_config.dst.end_addr = (uint32_t)(dvp_video_config->rxbuf + dvp_video_config->rxbuf_len);

	BK_LOG_ON_ERR(bk_dma_init(dvp_video_dma, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(dvp_video_dma, dvp_video_config->node_len));
	BK_LOG_ON_ERR(bk_dma_register_isr(dvp_video_dma, NULL, camera_intf_ejpeg_rx_handler));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(dvp_video_dma));
	BK_LOG_ON_ERR(bk_dma_start(dvp_video_dma));
}

static void camera_intf_ejpeg_end_handler(jpeg_unit_t id, void *param)
{
	camera_intf_start_delay_timer();
}

static void camera_intf_vsync_negedge_handler(jpeg_unit_t id, void *param)
{
	if (jpeg_eof_flag)
	{
#if (CONFIG_I2C_SIM)
		current_sensor->power_down();
#endif
		bk_jpeg_enc_set_enable(0, JPEG_ENC_MODE);
		jpeg_eof_flag = 0;
		if (ejpeg_sema != NULL)
		{
			rtos_set_semaphore(&ejpeg_sema);
		}
	}

#if (EJPEG_DROP_FRAME)
	if (vsync_index == 1)
	{
		vsync_index = 0;
		if (dvp_video_config->frame_drop)
			dvp_video_config->frame_drop(vsync_index);
	}
	else
	{
		vsync_index = 1;
		if (dvp_video_config->frame_drop)
			dvp_video_config->frame_drop(vsync_index);
	}
#endif

}

bk_err_t bk_dvp_camera_init(void *data)
{
	int err = kNoErr;
	i2c_config_t i2c_config = {0};
	jpeg_config_t jpeg_config = {0};
	jpeg_drop_image = EJPEG_DROP_COUNT;

#if (EJPEG_DROP_FRAME)
	vsync_index = 0;
#endif

	dvp_video_config = (video_config_t *)data;

	err = rtos_init_semaphore(&ejpeg_sema, 1);
	if (err != kNoErr)
	{
		LOGE("jpeg sema not init ok\r\n");
		return err;
	}

#if CONFIG_SOC_BK7256XX
	// step 1: enbale dvp power
	bk_dvp_camera_power_enable(1);

	bk_jpeg_enc_driver_init();
#endif

	// step 2: enable jpeg mclk for i2c communicate with dvp
	bk_jpeg_enc_mclk_enable();


	rtos_delay_milliseconds(5);

	// step 3: init i2c
	i2c_config.baud_rate = EJPEG_I2C_DEFAULT_BAUD_RATE;
	i2c_config.addr_mode = I2C_ADDR_MODE_7BIT;
	bk_i2c_init(CONFIG_CAMERA_I2C_ID, &i2c_config);

	current_sensor = bk_dvp_get_sensor_auto_detect();
	if (current_sensor == NULL)
	{
		LOGE("NOT find camera\r\n");
		err = kParamErr;
		return err;
	}

	LOGI("camera_type:%d,ppi:%d-%d, fps:%d\r\n", dvp_video_config->device->camera_type,
	dvp_video_config->device->ppi >> 16, dvp_video_config->device->ppi & 0xFFFF, dvp_video_config->device->fps);

	if (!(pixel_ppi_to_cap(dvp_video_config->device->ppi) & (current_sensor->ppi_cap)))
	{
		dvp_video_config->device->ppi = current_sensor->def_ppi;
	}

	if (!(dvp_video_config->device->fps & current_sensor->fps_cap))
	{
		dvp_video_config->device->fps = current_sensor->def_fps;
	}

	jpeg_config.mode = JPEG_ENC_MODE;
	jpeg_config.x_pixel = ppi_to_pixel_x(dvp_video_config->device->ppi) / 8;
	jpeg_config.y_pixel = ppi_to_pixel_y(dvp_video_config->device->ppi) / 8;
	jpeg_config.vsync = current_sensor->vsync;
	jpeg_config.hsync = current_sensor->hsync;
	jpeg_config.clk = current_sensor->clk;

	switch (current_sensor->fmt)
	{
		case PIXEL_FMT_YUYV:
			jpeg_config.sensor_fmt = JPEG_YUYV;
			break;

		case PIXEL_FMT_UYVY:
			jpeg_config.sensor_fmt = JPEG_UYVY;
			break;

		case PIXEL_FMT_YYUV:
			jpeg_config.sensor_fmt = JPEG_YYUV;
			break;

		case PIXEL_FMT_UVYY:
			jpeg_config.sensor_fmt = JPEG_UVYY;
			break;

		default:
			LOGE("JPEG MODULE not support this sensor input format\r\n");
			err = kParamErr;
			return err;
	}

	LOGI("x_pixel:%d, y_pixel:%d\r\n", dvp_video_config->device->ppi >> 16, dvp_video_config->device->ppi & 0xFFFF);

	// config jpeg module
	err = bk_jpeg_enc_init(&jpeg_config);
	if (err != BK_OK)
	{
		LOGE("jpeg init error\n");
		return err;
	}

	bk_jpeg_enc_register_isr(JPEG_EOF, camera_intf_ejpeg_end_handler, NULL);
	bk_jpeg_enc_register_isr(JPEG_VSYNC_NEGEDGE, camera_intf_vsync_negedge_handler, NULL);

	// dma config
	if (dvp_video_dma >= DMA_ID_MAX)
	{
		dvp_video_dma = bk_dma_alloc(DMA_DEV_JPEG);
		if ((dvp_video_dma < DMA_ID_0) || (dvp_video_dma >= DMA_ID_MAX))
		{
			LOGE("malloc dvp_camera_drv->psram_dma fail \r\n");
			err = kNoResourcesErr;
			return err;
		}
	}
	LOGI("jpeg dma:%d\r\n", dvp_video_dma);

	camera_intf_dma_config();

	current_sensor->init();
	current_sensor->set_ppi(dvp_video_config->device->ppi);
	current_sensor->set_fps(dvp_video_config->device->fps);

	bk_jpeg_enc_set_gpio_enable(1, JPEG_GPIO_DATA);

	LOGI("%s finish\r\n", __func__);

	return err;
}

bk_err_t bk_dvp_camera_deinit(void)
{
	int ret = kNoErr;

	jpeg_eof_flag = 1;

	ret = rtos_get_semaphore(&ejpeg_sema, 200);
	if (ret != kNoErr)
	{
		LOGE("Not wait jpeg eof!\r\n");
	}

	bk_jpeg_enc_deinit();

	bk_dma_stop(dvp_video_dma);
	bk_dma_deinit(dvp_video_dma);
	bk_dma_free(DMA_DEV_JPEG, dvp_video_dma);

	bk_i2c_deinit(CONFIG_CAMERA_I2C_ID);

#if CONFIG_SOC_BK7256XX
	bk_jpeg_enc_driver_deinit();

	bk_dvp_camera_power_enable(0);
#endif

	jpeg_eof_flag = 0;
	jpeg_drop_image = 0;
	frame_total_len = 0;
	dvp_video_dma = DMA_ID_MAX;

	rtos_deinit_semaphore(&ejpeg_sema);
	ejpeg_sema = NULL;

	current_sensor = NULL;
	dvp_video_config = NULL;

	LOGI("camera deinit finish\r\n");
	return ret;
}



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
#include <driver/psram.h>
#include <os/mem.h>
#include <driver/timer.h>
#include "bk_arm_arch.h"
#include <components/video_transfer.h>
#include <components/dvp_camera.h>
#include <components/dvp_camera_types.h>
#include "dvp_camera_config.h"
#include "bk_drv_model.h"
#include "dvp_capture.h"

#if CONFIG_GENERAL_DMA
#include "bk_general_dma.h"
#endif

#define EJPEG_DELAY_HTIMER_CHANNEL     5
#define EJPEG_I2C_DEFAULT_BAUD_RATE    I2C_BAUD_RATE_100KHZ
#define EJPEG_DELAY_HTIMER_VAL         (2)  // 2ms

static uint32_t s_camera_sensor = 0x01E00014;//(480 << 16) | 20;

static jpegenc_desc_t ejpeg_cfg;
static uint32_t frame_total_len = 0;
static uint32_t upper_size = 50 * 1024;
static uint32_t lower_size = 10 * 1024;
static uint8_t jpeg_auto_encode = 0;
static uint8_t jpeg_config_set = 0;

static void camera_intf_delay_timer_hdl(timer_id_t timer_id)
{
	if (jpeg_config_set)
	{
		if (jpeg_auto_encode)
		{
			bk_jpeg_enc_enable_encode_auto_ctrl(jpeg_auto_encode);
			bk_jpeg_enc_set_target_size(upper_size, lower_size);
		}
		else
		{
			bk_jpeg_enc_enable_encode_auto_ctrl(jpeg_auto_encode);
		}

		jpeg_config_set = 0;
	}
#if CONFIG_GENERAL_DMA
	uint16_t already_len = ejpeg_cfg.rx_read_len;
	GLOBAL_INT_DECLARATION();
	uint32_t left_len = bk_dma_get_remain_len(ejpeg_cfg.dma_channel);
	uint32_t rec_len = ejpeg_cfg.node_len - left_len;
	uint32_t frame_len = bk_jpeg_enc_get_frame_size();
	frame_total_len += rec_len - 5;

	if (frame_len != frame_total_len) {
		CAMERA_LOGE("jpeg frame len crc error:%d-%d\r\n", frame_len, frame_total_len);
	}

	frame_total_len = 0;

	if ((ejpeg_cfg.node_full_handler != NULL) && (rec_len > 0))
		ejpeg_cfg.node_full_handler(ejpeg_cfg.rxbuf + already_len, rec_len, 1, frame_len);
	already_len += rec_len;
	if (already_len >= ejpeg_cfg.rxbuf_len)
		already_len -= ejpeg_cfg.rxbuf_len;
	GLOBAL_INT_DISABLE();
	ejpeg_cfg.rx_read_len = already_len;
	GLOBAL_INT_RESTORE();
	// turn off dma, so dma can start from first configure. for easy handler
	bk_dma_stop(ejpeg_cfg.dma_channel);
	ejpeg_cfg.rx_read_len = 0;
	bk_dma_start(ejpeg_cfg.dma_channel);
#endif
	if ((ejpeg_cfg.data_end_handler))
		ejpeg_cfg.data_end_handler();
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
	uint16_t already_len = ejpeg_cfg.rx_read_len;
	uint16_t copy_len = ejpeg_cfg.node_len;
	frame_total_len += copy_len;
	GLOBAL_INT_DECLARATION();

	if (ejpeg_cfg.node_full_handler != NULL)
		ejpeg_cfg.node_full_handler(ejpeg_cfg.rxbuf + already_len, copy_len, 0, 0);
	already_len += copy_len;
	if (already_len >= ejpeg_cfg.rxbuf_len)
		already_len = 0;
	GLOBAL_INT_DISABLE();
	ejpeg_cfg.rx_read_len = already_len;
	GLOBAL_INT_RESTORE();
}

static void camera_intf_ejpeg_end_handler(jpeg_unit_t id, void *param)
{
	camera_intf_start_delay_timer();
}

static void camera_intf_init_ejpeg_pixel(uint32_t ppi_type)
{
	switch (ppi_type) {
	case QVGA_320_240:
		ejpeg_cfg.x_pixel = X_PIXEL_320;
		ejpeg_cfg.y_pixel = Y_PIXEL_240;
		break;
	case VGA_480_272:
		ejpeg_cfg.x_pixel = X_PIXEL_480;
		ejpeg_cfg.y_pixel = Y_PIXEL_272;
		break;
	case VGA_640_480:
		ejpeg_cfg.x_pixel = X_PIXEL_640;
		ejpeg_cfg.y_pixel = Y_PIXEL_480;
		break;
	case VGA_800_600:
		ejpeg_cfg.x_pixel = X_PIXEL_800;
		ejpeg_cfg.y_pixel = Y_PIXEL_600;
		break;
	case VGA_1280_720:
		ejpeg_cfg.x_pixel = X_PIXEL_1280;
		ejpeg_cfg.y_pixel = Y_PIXEL_720;
		break;
	case VGA_1600_1200:
		ejpeg_cfg.x_pixel = X_PIXEL_1600;
		ejpeg_cfg.y_pixel = Y_PIXEL_1200;
		break;
	default:
		CAMERA_LOGW("cm PPI unknown, use QVGA\r\n");
		ejpeg_cfg.x_pixel = X_PIXEL_640;
		ejpeg_cfg.y_pixel = Y_PIXEL_480;
		break;
	}
}

static void camera_intf_set_sener_cfg(uint32_t cfg)
{
	uint32_t ppi = (cfg >> 16) & 0xFFFF;
	uint32_t fps = cfg & 0xFFFF;
	ejpeg_cfg.sener_cfg = 0;

	CAMERA_LOGI("%s, ppi:%d\r\n", __func__, ppi);
	switch (ppi) {
		case 240:
			CMPARAM_SET_PPI(ejpeg_cfg.sener_cfg, QVGA_320_240);
			break;
		case 272:
			CMPARAM_SET_PPI(ejpeg_cfg.sener_cfg, VGA_480_272);
			break;
		case 480:
			CMPARAM_SET_PPI(ejpeg_cfg.sener_cfg, VGA_640_480);
			break;
		case 481:
			CMPARAM_SET_PPI(ejpeg_cfg.sener_cfg, VGA_320_480);
			break;
		case 482:
			CMPARAM_SET_PPI(ejpeg_cfg.sener_cfg, VGA_480_320);
			break;
		case 600:
			CMPARAM_SET_PPI(ejpeg_cfg.sener_cfg, VGA_800_600);
			break;
		case 720:
			CMPARAM_SET_PPI(ejpeg_cfg.sener_cfg, VGA_1280_720);
			break;
		case 1200:
			CMPARAM_SET_PPI(ejpeg_cfg.sener_cfg, VGA_1600_1200);
			break;
		default:
			CAMERA_LOGE("not support this ppi\r\n");
			CMPARAM_SET_PPI(ejpeg_cfg.sener_cfg, VGA_640_480);
	}

	CAMERA_LOGI("%s, fps:%d\r\n", __func__, fps);
	switch (fps) {
		case 5:
			CMPARAM_SET_FPS(ejpeg_cfg.sener_cfg, TYPE_5FPS);
			break;
		case 10:
			CMPARAM_SET_FPS(ejpeg_cfg.sener_cfg, TYPE_10FPS);
			break;
		case 15:
			CMPARAM_SET_FPS(ejpeg_cfg.sener_cfg, TYPE_15FPS);
			break;
		case 20:
			CMPARAM_SET_FPS(ejpeg_cfg.sener_cfg, TYPE_20FPS);
			break;
		case 25:
			CMPARAM_SET_FPS(ejpeg_cfg.sener_cfg, TYPE_25FPS);
			break;
		case 30:
			CMPARAM_SET_FPS(ejpeg_cfg.sener_cfg, TYPE_30FPS);
			break;
		default:
			CAMERA_LOGE("not support this fps\r\n");
			CMPARAM_SET_PPI(ejpeg_cfg.sener_cfg, TYPE_20FPS);
	}

	camera_intf_set_sener_cfg_value(ejpeg_cfg.sener_cfg);
}

static void camera_intf_config_ejpeg(void *data)
{
	os_memset(&ejpeg_cfg, 0, sizeof(jpegenc_desc_t));
	os_memcpy(&ejpeg_cfg, data, sizeof(video_config_t));
	camera_intf_set_sener_cfg(s_camera_sensor);
	camera_intf_init_ejpeg_pixel(CMPARAM_GET_PPI(ejpeg_cfg.sener_cfg));

#if CONFIG_GENERAL_DMA
	ejpeg_cfg.dma_rx_handler = camera_intf_ejpeg_rx_handler;
	ejpeg_cfg.dma_channel = bk_dma_alloc(DMA_DEV_JPEG);
	if ((ejpeg_cfg.dma_channel < DMA_ID_0) || (ejpeg_cfg.dma_channel >= DMA_ID_MAX)) {
		CAMERA_LOGE("malloc dma fail \r\n");
		return;
	}
#endif
}

bk_err_t bk_camera_init(void *data)
{
	int err = kNoErr;
	i2c_config_t i2c_config = {0};
	jpeg_config_t jpeg_config = {0};
	uint8_t ppi = 0;

#if CONFIG_SOC_BK7256XX
	bk_jpeg_enc_driver_init();
#endif
	camera_intf_config_ejpeg(data);

	ppi = CMPARAM_GET_PPI(ejpeg_cfg.sener_cfg);
	jpeg_config.rx_buf = ejpeg_cfg.rxbuf;
	jpeg_config.rx_buf_len = ejpeg_cfg.rxbuf_len;
	jpeg_config.node_len = ejpeg_cfg.node_len;
	jpeg_config.x_pixel = ejpeg_cfg.x_pixel;
	jpeg_config.y_pixel = ejpeg_cfg.y_pixel;
	if (ppi >= VGA_1600_1200) {
		jpeg_config.sys_clk_div = 4; // sys jpeg div (4+1)
		jpeg_config.mclk_div = 1;// jpeg 6 div
	} else if ((ppi < VGA_1600_1200) && (ppi >= VGA_1280_720)) {
		jpeg_config.sys_clk_div = 3; // sys jpeg div (3+1)
		jpeg_config.mclk_div = 0;// jpeg 4 div
	} else {
		jpeg_config.sys_clk_div = 4; // sys jpeg div (4+1)
		jpeg_config.mclk_div = 0; // jpeg 4 div
	}

	jpeg_config.dma_rx_finish_handler = ejpeg_cfg.dma_rx_handler;
	jpeg_config.dma_channel = ejpeg_cfg.dma_channel;

	bk_jpeg_enc_init(&jpeg_config);
	bk_jpeg_enc_register_isr(END_OF_FRAME, camera_intf_ejpeg_end_handler, NULL);

	i2c_config.baud_rate = EJPEG_I2C_DEFAULT_BAUD_RATE;
	i2c_config.addr_mode = I2C_ADDR_MODE_7BIT;
	bk_i2c_init(CONFIG_CAMERA_I2C_ID, &i2c_config);

	bk_camera_sensor_config();
	return err;
}

bk_err_t bk_camera_deinit(void)
{

	os_printf("bk_jpeg_enc_deinit\r\n");
	bk_jpeg_enc_deinit();
	os_printf("bk_dma_deinit\r\n");
	bk_dma_deinit(ejpeg_cfg.dma_channel);
	bk_dma_free(DMA_DEV_JPEG, ejpeg_cfg.dma_channel);

	bk_i2c_deinit(CONFIG_CAMERA_I2C_ID);

#if CONFIG_SOC_BK7256XX
	bk_jpeg_enc_driver_deinit();
#endif

	frame_total_len = 0;
	os_memset(&ejpeg_cfg, 0, sizeof(jpegenc_desc_t));

	CAMERA_LOGI("camera deinit finish\r\n");
	return kNoErr;
}

bk_err_t bk_camera_set_param(uint32_t dev, uint32_t cfg)
{
	camera_intf_set_device(dev);
	
	s_camera_sensor = cfg;
	camera_intf_set_sener_cfg(cfg);
	return kNoErr;
}

void bk_camera_set_config(uint8_t auto_ctrl, uint32_t up_size, uint32_t low_size)
{
	jpeg_config_set = 1;

	if (auto_ctrl)
	{
		if (up_size > low_size)
		{
			upper_size = up_size;
			lower_size = low_size;
		}
	}

	jpeg_auto_encode = auto_ctrl;
}


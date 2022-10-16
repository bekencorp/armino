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

#pragma once

#include <common/bk_err.h>
#include <driver/media_types.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
	DVP_MODE_JPG = 0,  /**< camera work in jpeg encode mode */
	DVP_MODE_YUV,      /**< camera work in jpeg yuv mode */
	DVP_MODE_MIX,
	DVP_MODE_INVALIED,
} dvp_mode_t;

/**
 * @brief jpeg module work clk and sensor work clk
 * @{
 */
typedef enum
{
	JPEG_96M_MCLK_16M,   /**< jpeg module work in 96MHz and sensor work in 16MHz */
	JPEG_96M_MCLK_24M,
	JPEG_120M_MCLK_20M,
	JPEG_120M_MCLK_30M,
} sensor_clk_t;

/**
 * @brief dvp sensor id type
 * @{
 */
typedef enum
{
	ID_UNKNOW = 0,
	ID_PAS6329,
	ID_OV7670,
	ID_PAS6375,
	ID_GC0328C,
	ID_BF2013,
	ID_GC0308C,
	ID_HM1055,
	ID_GC2145,
	ID_OV2640
} sensor_id_t;


typedef enum
{
	FPS5    = (1 << 0), /**< 5fps */
	FPS10   = (1 << 1), /**< 10fps */
	FPS15   = (1 << 2), /**< 15fps */
	FPS20   = (1 << 3), /**< 20fps */
	FPS25   = (1 << 4), /**< 25fps */
	FPS30   = (1 << 5), /**< 30fps */
} sensor_fps_t;

/**
 * @brief dvp_camera config
 * @{
 */
typedef struct
{
	media_ppi_t ppi;  /**< sensor image resolution */
	dvp_mode_t mode;  /**< sensor output mode yuv/jpeg */
	int (*fb_jpeg_init) (media_ppi_t max_ppi); /**< init jpeg psram buffer */
	int (*fb_jpeg_deinit) (void); /**< deinit jpeg psram buffer */
	int (*fb_display_init) (media_ppi_t max_ppi); /**< init yuv psram buffer */
	int (*fb_display_deinit) (void); /**< deinit yuv psram buffer */
	void (*fb_jpeg_complete) (frame_buffer_t* buffer);  /**< dvp notify upper layer a complete frame ready */
	void (*fb_display_complete) (frame_buffer_t* buffer);
	frame_buffer_t* (*fb_jpeg_malloc)(void); /**< malloc jpeg psram buffer */
	void (*fb_jpeg_free)(frame_buffer_t* buffer);
	void (*fb_display_free)(frame_buffer_t* buffer);
	frame_buffer_t* (*fb_display_malloc)(void); /**< malloc yuv psram buffer */
} dvp_camera_config_t;

/**
 * @brief dvp_camera i2c process
 * @{
 */
typedef struct
{
	int (*read_uint8) (uint8_t addr, uint8_t reg, uint8_t *value);  /**< i2c read by byte */
	int (*read_uint16) (uint8_t addr, uint16_t reg, uint8_t *value);  /**< i2c read by two byte */
	int (*write_uint8) (uint8_t addr, uint8_t reg, uint8_t value);  /**< i2c write by byte */
	int (*write_uint16) (uint8_t addr, uint16_t reg, uint8_t value);  /**< i2c write by two byte */
} dvp_camera_i2c_callback_t;

/**
 * @brief dvp sensor configure
 * @{
 */
typedef struct
{
	char *name;  /**< sensor name */
	media_ppi_t def_ppi;  /**< sensor default resolution */
	sensor_fps_t def_fps;  /**< sensor default fps */
	uint16 id;  /**< sensor type, sensor_id_t */
	uint8 clk;  /**< sensor work clk in config fps and ppi */
	uint16 address;  /**< sensor write register address by i2c */
	uint16 fps_cap;  /**< sensor support fps */
	uint16 ppi_cap;  /**< sensor support resoultions */
	bool (*detect)(const dvp_camera_i2c_callback_t *cb);  /**< auto detect used dvp sensor */
	int (*init)(const dvp_camera_i2c_callback_t *cb);  /**< init dvp sensor */
	int (*set_ppi)(const dvp_camera_i2c_callback_t *cb, media_ppi_t ppi);  /**< set resolution of sensor */
	int (*set_fps)(const dvp_camera_i2c_callback_t *cb, sensor_fps_t fps);  /**< set fps of sensor */
	int (*power_down)(const dvp_camera_i2c_callback_t *cb);  /**< power down or reset of sensor */
	int (*dump_register)(const dvp_camera_i2c_callback_t *cb, media_ppi_t ppi);  /**< dump sensor register */
	void (*read_register)(bool enable);  /**< read sensor register when write*/
} dvp_sensor_config_t;

#define GC_QVGA_USE_SUBSAMPLE          1

/**
 * @brief current used camera config
 * @{
 */
typedef struct
{
	media_ppi_t ppi;  /**< sensor image resolution */
	sensor_fps_t fps;  /**< sensor fps */
	char *name;  /**< sensor name */
	uint16 id;  /**< sensor type, sensor_id_t */
	uint16 fps_cap;  /**< sensor support fps */
	uint16 ppi_cap;  /**< sensor support resoultions */
	uint32 pixel_size;  /**< sensor output image width */
	dvp_mode_t mode;  /**< dvp camera work mode */
} dvp_camera_device_t;


/*
 * @}
 */

#ifdef __cplusplus
}
#endif

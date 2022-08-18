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
#include "driver/dvp_camera_types.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	media_ppi_t ppi;
	dvp_mode_t mode;
	int (*frame_set_ppi)(media_ppi_t ppi);
	void (*frame_complete)(frame_buffer_t* buffer);
	frame_buffer_t* (*frame_alloc)(void);
} dvp_camera_config_t;

typedef struct
{
	int (*read_uint8) (uint8_t addr, uint8_t reg, uint8_t *value);
	int (*read_uint16) (uint8_t addr, uint16_t reg, uint8_t *value);
	int (*write_uint8) (uint8_t addr, uint8_t reg, uint8_t value);
	int (*write_uint16) (uint8_t addr, uint16_t reg, uint8_t value);
} dvp_camera_i2c_callback_t;


typedef struct
{
	char *name;
	media_ppi_t def_ppi;
	sensor_fps_t def_fps;
	uint16 id;
	uint8 clk;
	uint16 address;
	uint16 fps_cap;
	uint16 ppi_cap;
	bool (*detect)(const dvp_camera_i2c_callback_t *cb);
	int (*init)(const dvp_camera_i2c_callback_t *cb);
	int (*set_ppi)(const dvp_camera_i2c_callback_t *cb, media_ppi_t ppi);
	int (*set_fps)(const dvp_camera_i2c_callback_t *cb, sensor_fps_t fps);
} dvp_sensor_config_t;

#define GC_QVGA_USE_SUBSAMPLE          1

typedef struct
{
	media_ppi_t ppi;
	sensor_fps_t fps;
	char *name;
	uint16 id;
	uint16 fps_cap;
	uint16 ppi_cap;
	uint32 pixel_size;
	dvp_mode_t mode;
} dvp_camera_device_t;


bk_err_t bk_dvp_camera_driver_init(dvp_camera_config_t *config);
bk_err_t bk_dvp_camera_driver_deinit(void);
dvp_camera_device_t *bk_dvp_camera_get_device(void);

int dvp_camera_i2c_read_uint8(uint8_t addr, uint8_t reg, uint8_t *value);
int dvp_camera_i2c_read_uint16(uint8_t addr, uint16_t reg, uint8_t *value);
int dvp_camera_i2c_write_uint8(uint8_t addr, uint8_t reg, uint8_t value);
int dvp_camera_i2c_write_uint16(uint8_t addr, uint16_t reg, uint8_t value);

bk_err_t bk_dvp_camera_encode_config(uint8_t auto_ctrl, uint32_t up_size, uint32_t low_size);

#ifdef __cplusplus
}
#endif

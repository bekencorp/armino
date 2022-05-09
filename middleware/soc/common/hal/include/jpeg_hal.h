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

#include "hal_config.h"
#include "jpeg_hw.h"
#include "jpeg_ll.h"
#include <driver/hal/hal_jpeg_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PSRAM_BASEADDR                    0x60000000
#define JPEG_SHARE_MEM                    0x30060000

typedef struct {
	jpeg_hw_t *hw;
	jpeg_unit_t id;
} jpeg_hal_t;

typedef struct {
	gpio_id_t gpio_id;
	gpio_dev_t dev;
} jpeg_gpio_map_t;

#define jpeg_hal_enable_start_frame_int(hal) jpeg_ll_enable_start_frame_int((hal)->hw)
#define jpeg_hal_disable_start_frame_int(hal) jpeg_ll_disable_start_frame_int((hal)->hw)
#define jpeg_hal_enable_end_frame_int(hal) jpeg_ll_enable_end_frame_int((hal)->hw)
#define jpeg_hal_disable_end_frame_int(hal) jpeg_ll_disable_end_frame_int((hal)->hw)
#define jpeg_hal_reset_config_to_default(hal) jpeg_ll_reset_config_to_default((hal)->hw)
#define jpeg_hal_set_x_pixel(hal, x_pixel) jpeg_ll_set_x_pixel((hal)->hw, x_pixel)
#define jpeg_hal_set_y_pixel(hal, x_pixel) jpeg_ll_set_y_pixel((hal)->hw, x_pixel)
#define jpeg_hal_set_yuv_mode(hal, mode) jpeg_ll_set_yuv_mode((hal)->hw, mode)

#define jpeg_hal_get_frame_byte_number(hal) jpeg_ll_get_frame_byte_number((hal)->hw)

#define jpeg_hal_get_interrupt_status(hal) jpeg_ll_get_interrupt_status((hal)->hw)
#define jpeg_hal_clear_interrupt_status(hal, int_status) jpeg_ll_clear_interrupt_status((hal)->hw, int_status)
#define jpeg_hal_is_frame_start_int_triggered(hal, int_status) jpeg_ll_is_frame_start_int_triggered((hal)->hw, int_status)
#define jpeg_hal_is_frame_end_int_triggered(hal, int_status) jpeg_ll_is_frame_end_int_triggered((hal)->hw, int_status)
#define jpeg_hal_is_yuv_end_int_triggered(hal, int_status) jpeg_ll_is_yuv_end_int_triggered((hal)->hw, int_status)
#define jpeg_hal_is_head_output_int_triggered(hal, int_status) jpeg_ll_is_head_output_int_triggered((hal)->hw, int_status)
#define jpeg_hal_is_sync_negedge_int_triggered(hal, int_status) jpeg_ll_is_vsync_negedge_int_triggered((hal)->hw, int_status)

bk_err_t jpeg_hal_init(jpeg_hal_t *hal);
bk_err_t jpeg_hal_configure(jpeg_hal_t *hal, const jpeg_config_t *config);
bk_err_t jpeg_hal_start_common(jpeg_hal_t *hal);
bk_err_t jpeg_hal_stop_common(jpeg_hal_t *hal);

#if CFG_HAL_DEBUG_JPEG
void jpeg_struct_dump(void);
#else
#define jpeg_struct_dump()
#endif

#ifdef __cplusplus
}
#endif


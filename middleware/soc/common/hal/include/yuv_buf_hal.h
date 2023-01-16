// Copyright 2022-2023 Beken
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
#include "yuv_buf_hw.h"
#include "yuv_buf_ll.h"
#include <driver/hal/hal_yuv_buf_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	yuv_buf_hw_t *hw;
} yuv_buf_hal_t;

#define yuv_buf_hal_enable_mclk_hold_on(hal) yuv_buf_ll_enable_mclk_hold_on((hal)->hw)
#define yuv_buf_hal_set_em_base_addr(hal, address) yuv_buf_ll_set_em_base_addr((hal)->hw, address)
#define yuv_buf_hal_reset_config_to_default(hal) yuv_buf_ll_reset_config_to_default((hal)->hw)
#define yuv_buf_hal_set_mclk_div(hal, mclk_div) yuv_buf_ll_set_mclk_div((hal)->hw, mclk_div)
#define yuv_buf_hal_send_data_to_h264(hal)		yuv_buf_ll_send_data_to_h264((hal)->hw)

#define yuv_buf_hal_get_interrupt_status(hal) yuv_buf_ll_get_interrupt_status((hal)->hw)
#define yuv_buf_hal_clear_interrupt_status(hal, int_status) yuv_buf_ll_clear_interrupt_status((hal)->hw, int_status)
#define yuv_buf_hal_is_vsync_negedge_int_triggered(hal, int_status) yuv_buf_ll_is_vsync_negedge_int_triggered((hal)->hw, int_status)
#define yuv_buf_hal_is_yuv_arv_int_triggered(hal, int_status) yuv_buf_ll_is_yuv_arv_int_triggered((hal)->hw, int_status)
#define yuv_buf_hal_is_sm0_wr_int_triggered(hal, int_status) yuv_buf_ll_is_sm0_wr_int_triggered((hal)->hw, int_status)
#define yuv_buf_hal_is_sm1_wr_int_triggered(hal, int_status) yuv_buf_ll_is_sm1_wr_int_triggered((hal)->hw, int_status)
#define yuv_buf_hal_is_fifo_full_int_triggered(hal, int_status) yuv_buf_ll_is_fifo_full_int_triggered((hal)->hw, int_status)
#define yuv_buf_hal_is_enc_line_done_int_triggered(hal, int_status) yuv_buf_ll_is_enc_line_done_int_triggered((hal)->hw, int_status)
#define yuv_buf_hal_is_sensor_resolution_err_int_triggered(hal, int_status) yuv_buf_ll_is_sensor_resolution_err_int_triggered((hal)->hw, int_status)
#define yuv_buf_hal_is_h264_err_int_triggered(hal, int_status) yuv_buf_ll_is_h264_err_int_triggered((hal)->hw, int_status)
#define yuv_buf_hal_is_enc_slow_int_triggered(hal, int_status) yuv_buf_ll_is_enc_slow_int_triggered((hal)->hw, int_status)

bk_err_t yuv_buf_hal_init(yuv_buf_hal_t *hal);
bk_err_t yuv_buf_hal_set_yuv_mode_config(yuv_buf_hal_t *hal, const yuv_buf_config_t *config);
bk_err_t yuv_buf_hal_set_jpeg_mode_config(yuv_buf_hal_t *hal, const yuv_buf_config_t *config);
bk_err_t yuv_buf_hal_set_h264_mode_config(yuv_buf_hal_t *hal, const yuv_buf_config_t *config);
bk_err_t yuv_buf_hal_start_yuv_mode(yuv_buf_hal_t *hal);
bk_err_t yuv_buf_hal_stop_yuv_mode(yuv_buf_hal_t *hal);
bk_err_t yuv_buf_hal_start_jpeg_mode(yuv_buf_hal_t *hal);
bk_err_t yuv_buf_hal_stop_jpeg_mode(yuv_buf_hal_t *hal);
bk_err_t yuv_buf_hal_start_h264_mode(yuv_buf_hal_t *hal);
bk_err_t yuv_buf_hal_stop_h264_mode(yuv_buf_hal_t *hal);
bk_err_t yuv_buf_hal_enable_h264_nosensor_mode(yuv_buf_hal_t *hal);

#if CFG_HAL_DEBUG_YUV_BUF
void yuv_buf_struct_dump(void);
#else
#define yuv_buf_struct_dump()
#endif

#ifdef __cplusplus
}
#endif


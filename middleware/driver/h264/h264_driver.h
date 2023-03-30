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

#include <components/log.h>

#define H264_TAG "H264"
#define H264_LOGI(...) BK_LOGI(H264_TAG, ##__VA_ARGS__)
#define H264_LOGW(...) BK_LOGW(H264_TAG, ##__VA_ARGS__)
#define H264_LOGE(...) BK_LOGE(H264_TAG, ##__VA_ARGS__)
#define H264_LOGD(...) BK_LOGD(H264_TAG, ##__VA_ARGS__)

#ifdef CONFIG_H264_STATIS
#define H264_INT_INC(arr,offset)	(*(arr + offset))++;
#else
#define H264_INT_INC(arr,offset)
#endif


bk_err_t bk_h264_dma_rx_init(h264_dma_config_t *config);

bk_err_t bk_h264_dma_rx_deinit(void);

bk_err_t bk_h264_camera_gpio_set(void);

bk_err_t bk_h264_camera_clk_enhance(void);

bk_err_t bk_h264_config_init(const h264_config_t *config, uint16_t media_width, uint16_t media_height);

bk_err_t bk_h264_pingpong_in_psram_clk_set(void);

bk_err_t bk_h264_deblocking_filter_config_init(const h264_config_t *config, uint32_t alpha_off, uint32_t beta_off);

uint32_t bk_h264_get_encode_count(void);

bk_err_t bk_h264_int_count_show(void);

bk_err_t bk_h264_clk_check(void);
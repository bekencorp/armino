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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the QSPI oled
 *
 * This API init the qspi oled module:
 *  - open the backlight of oled
 *  - init the psram
 *  - init the oled device register
 *  - enable quad write
 *  - enable oled display on 
 * 
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_QSPI_NOT_INIT: QSPI driver not init
 *    - others: other errors.
 */
bk_err_t bk_qspi_oled_init(void);

/**
 * @brief     Set the percent of QSPI oled backlight
 *
 * @param percent the percent value of backlight, the max is 100
 * 
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_oled_set_backlight(uint8_t percent);

/**
 * @brief     Deinit the QSPI oled
 *
 * This API deinit the qspi oled module:
 *  - close the backlight of oled
 *  - stop quad write
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_oled_deinit(void);

/**
 * @brief     Start the quad write of qspi
 * 
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_oled_quad_write_start(void);

/**
 * @brief     Stop the quad write of qspi
 * 
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_qspi_oled_quad_write_stop(void);

/**
 * @brief     dma2d fill the pure color data to qspi oled display on
 *
 * @param width the width of pixel
 * @param height the height of pixel
 * @param color the color data to be filled
 * 
 * @return None
 */
void bk_qspi_oled_dma2d_fill(uint32_t width, uint32_t height, uint32_t color);

/**
 * @brief     dma2d fill the pitcure data store in the mem to qspi oled display on
 *
 * @param Psrc the picture data mem addr
 * @param xsize the width of pixel
 * @param ysize the height of pixel
 * 
 * @return None
 */
void bk_qspi_oled_dma2d_memcpy(uint32_t *Psrc, uint32_t xsize, uint32_t ysize);

#ifdef __cplusplus
}
#endif


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

#include "driver/lcd_disp_types.h"

#ifdef __cplusplus
extern "C" {
#endif



/* @brief Overview about this API header
 *
 */

/**
 * @brief LCD API
 * @{
 */

 /**
 * @brief    This API select LCD module clk source
 *          - config lcd freq div
 *          - config video power
 *          - open lcd sys interrupt enable
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_driver_init(lcd_clk_t clk);


/**
 * @brief This API init the 8080 lcd interface
 *    - Set lcd display mode is 8080 interface
 *    - init 8080 lcd gpio 
 *    - enable 8080 display
 *    -enable 8080 end of frame interrupt
 *    - if you want enable start of frame interrupt, please use API bk_lcd_8080_int_enable
 *
 * @param
 *     - x_pixel defult by 320, user can set by any value.
 *     - y_pixel defult by 480, user can set by any value.
 *
 * @attention 1. int the next version, the xpixel and ypixel deside the transfer number of lcd display
 *               will config with another two register x offset and y offset.
 * @attention 2. in this sdk version x_pixel/y_pixel only set once in 8080_init,if you want set twice,should
 *               set bk_lcd_8080_display_enable(0).
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_8080_init(display_pixel_format_t x_pixel, display_pixel_format_t y_pixel);


/**
* @brief 8080 lcd interface reg deinit
*     - This API reset all lcd reg include power 
*     - close 8080/rgb lcd enable and display
*     - reset x pixel and y pixel zero
*     - unregister lcd isr
*
* @return
*	  - BK_OK: succeed
*	  - others: other errors.
*/
bk_err_t bk_lcd_8080_deinit(void);

/**
 * @brief This API config lcd display x size and y size
 *
 * @param
 *     - width lcd display width
 *     - height lcd display height
 *
 * @attention 1. int the next version, the width and height deside the transfer number of lcd display.
 *              will config with another two register x offset and y offset.
 * @attention 2. in this sdk version width/height only set once in 8080_init,if you want set twice,should 
 *               set bk_lcd_8080_display_enable(0).
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_pixel_config(uint16_t x_pixel, uint16_t y_pixel);


/**
 * @brief write 8080 lcd cmd
 *
 * @param lcd cmd date, rang bit[15:0]
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t  bk_lcd_8080_write_cmd(uint32_t cmd);


/**
 * @brief write 8080 lcd data 
 *
 * @param lcd date, rang bit[15:0]
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_8080_write_data(uint32_t data);

/**
 * @brief This API config 8080 lcd interrupt

 * @param
 *     - is_sof_en enable start of frame interrupt
 *     - is_eof_en enable end of frame interrupt
 *
 * @attention 8080 end of frame int is open in API bk_lcd_8080_init
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_8080_int_enable(bool is_sof_en, bool is_eof_en);


/**
 * @brief This API start 8080 lcd transfer data to display
 *
 * @param start_transfer 
 *      - 1:data start transfer to lcd display on; 
 *      - 0:stop  transfer
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_8080_start_transfer(bool start);


/**
 * @brief  8080 lcd display enable
 *
 * @param   1 enable 8080 interface display
 *        - 0 disable 8080 interface display
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_8080_display_enable(bool en);



/**
 * @brief This API init the rgb lcd interface
 *    - Set lcd display mode is rgb interface
 *    - init rgb lcd gpio 
 *    - enable rgb display
 *    - enable rgb end of frame interrupt
 *
 * @param
 *     - clk_div: value rang bit[0~4]: 0~0x1f
 *     - x_pixel: defult by 320, user can set by any value
 *     - y_pixel: defult by 480, user can set by any value
 *     - input_data_format: select from RGB_DATA_FORMAT enum:rgb565, yuyv etc.
 *
 * Usage example:
 *
 *            if rgb_clk_div = 5,the rgb lcd clk is 96/(div+1)=16Mhz
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_rgb_init(uint32_t rgb_clk_div, display_pixel_format_t x_pixel, display_pixel_format_t y_pixel, rgb_input_data_format_t input_data_format);


/**
 * @brief     rgb lcd interface reg deinit
 *           - This API reset all lcd reg include power 
 *           - close 8080/rgb lcd enable and display
 *           - reset x pixel and y pixel zero
 *           - unregister lcd isr
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_rgb_deinit(void);

/**
 * @brief  enable rgb lcd enable
 * @param  bool en
 *       - 1: enable rgb display
 *       - 0: disable rgb display
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_rgb_display_en(bool en);


/**
* @brief This API config rgb lcd interrupt

 * @param
 *     - is_sof_en enable start of frame interrupt
 *     - is_eof_en enable end of frame interrupt
 *
 * @attention rgb end of frame int is open in API bk_lcd_rgb_init
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_rgb_int_enable(bool is_sof_en, bool is_eof_en);


/**
 * @brief This API register  8080/rgb lcd int isr
 * 
 * @param
 *     - int_type  include  8080/rgb  end of frame int and  8080/rgb  start of frame int
 *     - isr: isr function
 *
 * Usage example:
 *
 *       bk_lcd_isr_register(I8080_OUTPUT_EOF, lcd_i8080_isr); //register 8080 end of frame isr
 *
  * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_isr_register(lcd_int_type_t int_type, lcd_isr_t isr);

/**
 * @brief open lcd power,always set in the modile init
 *
 * @param is_lcd_power_on 1:power on,  0:power down
 *
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
bk_err_t bk_lcd_power_on_ctrl(bool is_lcd_power_on);

/**
  * @}
  */

#ifdef __cplusplus
 }
#endif






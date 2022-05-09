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
#include "lcd_disp_ll_macro_def.h"
#include "jpeg_reg.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef void (*lcd_isr_t)(void);

/**
 * @brief  lcd macos define
 * @{
 */
#define REG_DISP_DAT_FIFO                 LCD_DISP_I8080_DAT_FIFO_ADDR /**<define 8080 data fifo addr */
#define REG_DISP_RGB_FIFO                 LCD_DISP_RGB_FIFO_ADDR       /**<define rgb fifo addr */
#define JPEG_RX_FIFO                      JPEG_R_RX_FIFO

/**
 * @}
 */

/**
 * @brief LCD enum defines
 * @{
 */


/** define display module pixel */
typedef enum {
	X_PIXEL_8080 = 320,  /**<define 8080 lcd x size */
	Y_PIXEL_8080 = 480,  /**<define 8080 lcd y size */
	X_PIXEL_RGB = 480,   /**<define rgb lcd x size */
	Y_PIXEL_RGB = 272,   /**<define rgb lcd x size */
}display_pixel_format_t;

/** rgb lcd input data format */
typedef enum {
	RGB565_DATA = 0,        /**< input data format is rgb 565,support data width is 16bits, this is to say
		                       input data is low 16 bit valid*/
	ORGINAL_YUYV_DATA,    /**< input data is yuyv format, data width is 32bits*/
	UYVY_DATA,
	YYUV_DATA,            /**< input data is yyuv format, data width is 32bits*/
	UVYY_DATA,            /**< input data is uvyy format,data width is 32bits*/
	VUYY_DATA,            /**< input data is uvyy format,data width is 32bits*/
}rgb_input_data_format_t;


typedef enum {
	LCD_20M, /**< LCD Display module clk sel*/
	LCD_30M,
	LCD_40M,
	LCD_48M,
	LCD_60M,
	LCD_80M,
	LCD_96M,
	LCD_160M,
	LCD_240M,
}lcd_clk_t;


/**< rgb lcd clk select*/
typedef enum {
	RGB_OUTPUT_EOF =1 << 5 , 	/**< reg end of frame int,*/
	RGB_OUTPUT_SOF =1 << 4,      /**< reg display output start of frame  */
	I8080_OUTPUT_SOF =1 << 6,   /**< 8080 display output start of frame  */
	I8080_OUTPUT_EOF = 1 << 7,   /**< 8080 display output end of frame    */
}lcd_int_type_t;

/*
 * @}
 */

#ifdef __cplusplus
}
#endif



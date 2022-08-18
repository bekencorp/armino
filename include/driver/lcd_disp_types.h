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

typedef void (*lcd_isr_t)(void);

/**
 * @}
 */

/**
 * @brief LCD enum defines
 * @{
 */

/** define display module pixel */
typedef enum {
	LCD_8080_16BIT = 1,
	LCD_RGB_YUV,
	LCD_RGB_RGB565,
	LCD_RGB_RGB888
}lcd_interface_data_t;


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

/** rgb data output in clk rising or falling */
typedef enum {
	POSEDGE_OUTPUT = 0,    /**< output in clk falling*/
	NEGEDGE_OUTPUT,        /**< output in clk rising*/
}rgb_out_clk_edge_t ;

typedef enum {
	LCD_TYPE_480_272 = 0,    /**< output in clk falling*/
	LCD_TYPE_1024_600        /**< output in clk rising*/
}lcd_types_t;



/**< rgb lcd clk select*/
typedef enum {
	RGB_OUTPUT_EOF =1 << 5 , 	/**< reg end of frame int,*/
	RGB_OUTPUT_SOF =1 << 4,      /**< reg display output start of frame  */
	I8080_OUTPUT_SOF =1 << 6,   /**< 8080 display output start of frame  */
	I8080_OUTPUT_EOF = 1 << 7,   /**< 8080 display output end of frame    */
}lcd_int_type_t;


#define JPEGDEC_FRAME_SIZE  0x200000 
#define DISPLAY_FRAME_COUNTS (2)

#define PSRAM_BASEADDR (0x60000000UL)

typedef struct
{
	uint8_t display[DISPLAY_FRAME_COUNTS][JPEGDEC_FRAME_SIZE];
} psram_lcd_t;

#define psram_lcd ((psram_lcd_t*)PSRAM_BASEADDR)

/*
 * @}
 */

#ifdef __cplusplus
}
#endif



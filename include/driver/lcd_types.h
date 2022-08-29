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

#include <driver/media_types.h>

#define  USE_LCD_REGISTER_CALLBACKS  1
typedef void (*lcd_isr_t)(void);

typedef enum {
	LCD_DEVICE_UNKNOW,
	LCD_DEVICE_ST7282,  /**< 480X270  RGB */
	LCD_DEVICE_HX8282,  /**< 1024X600 RGB  */
	LCD_DEVICE_GC9503V, /**< 480X800 RGB  */
	LCD_DEVICE_ST7796S, /**< 320X480 MCU  */
} lcd_device_id_t;

typedef enum {
	LCD_TYPE_RGB565,  /**< lcd devicd output data hardware interface is RGB565 format */
	LCD_TYPE_MCU8080, /**< lcd devicd output data hardware interface is MCU 8BIT format */
} lcd_type_t;

typedef enum {
	RGB_OUTPUT_EOF =1 << 5 ,	/**< reg end of frame int,*/
	RGB_OUTPUT_SOF =1 << 4, 	 /**< reg display output start of frame  */
	I8080_OUTPUT_SOF =1 << 6,	/**< 8080 display output start of frame  */
	I8080_OUTPUT_EOF = 1 << 7,	 /**< 8080 display output end of frame	  */
}lcd_int_type_t;

/** rgb lcd clk select, infulence pfs, user should select according to lcd device spec*/
typedef enum {
	LCD_320M = 0,    /**< diaplay module clk config 320MHz*/
	LCD_160M,
	LCD_120M,
	LCD_80M,
	LCD_60M,
	LCD_54M,  //5
	LCD_40M,
	LCD_32M,
	LCD_26M,
	LCD_20M,
	LCD_12M,//10
	LCD_10M,
	LCD_8M   //12
}lcd_clk_t;

/** rgb data output in clk rising or falling */
typedef enum {
	POSEDGE_OUTPUT = 0,    /**< output in clk falling*/
	NEGEDGE_OUTPUT, 	   /**< output in clk rising*/
}rgb_out_clk_edge_t;


/** rgb lcd input data format, data save in mem is little endian, like VUYY format is [bit31-bit0] is [V U Y Y]*/
typedef enum {
	LCD_FMT_RGB565 = 0,        /**< input data format is rgb565*/
	LCD_FMT_ORGINAL_YUYV,    /**< input data is yuyv format, diaplay module internal may convert to rgb565 data*/
	LCD_FMT_UYVY,
	LCD_FMT_YYUV,            /**< input data is yyuv format */
	LCD_FMT_UVYY,            /**< input data is uvyy format*/
	LCD_FMT_VUYY,            /**< input data is vuyy format */
	LCD_FMT_YVYU,
	LCD_FMT_VYUY,
	LCD_FMT_YYVU
}lcd_data_format_t;


typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
}lcd_rect_t;

typedef struct
{
    void *buffer;
    lcd_rect_t rect;
} lcd_disp_framebuf_t;


/** rgb config param */
typedef struct
{
	lcd_clk_t clk;                         /**< config lcd clk */
	rgb_out_clk_edge_t data_out_clk_edge;  /**< rgb data output in clk edge, should refer lcd device spec*/

	uint16_t hsync_back_porch;            /**< rgb hsync back porch, should refer lcd device spec*/
	uint16_t hsync_front_porch;           /**< rgb hsync front porch, should refer lcd device spec*/
	uint16_t vsync_back_porch;            /**< rgb vsyn back porch, should refer lcd device spec*/
	uint16_t vsync_front_porch;           /**< rgb vsyn front porch, should refer lcd device spec*/
} lcd_rgb_t;

typedef struct
{
	lcd_clk_t clk; /**< config lcd clk */
	void (*set_display_area)(uint16 xs, uint16 xe, uint16 ys, uint16 ye);  /**< if lcd size is smaller then image, 
									 and set api bk_lcd_pixel_config is image x y, should set partical display */
} lcd_mcu_t;



typedef struct
{
	lcd_device_id_t id;  /**< lcd device type, user can add if you want to add another lcd device */
	char *name;          /**< lcd device name */
	lcd_type_t type;     /**< lcd device hw interface */
	media_ppi_t ppi;     /**< lcd device x y size */
	union {
		const lcd_rgb_t *rgb;  /**< RGB interface lcd device config */
		const lcd_mcu_t *mcu;  /**< MCU interface lcd device config */
	};
	void (*init)(void);       /**< lcd device initial function */
} lcd_device_t;


typedef struct
{
	const lcd_device_t *device;  /**< lcd device config */
	lcd_data_format_t fmt;       /**< display module input data format */
	uint16_t pixel_x;            /**< lcd display src picture size */
	uint16_t pixel_y;            /**< lcd display src picture size */
#if	(USE_LCD_REGISTER_CALLBACKS == 1) 
	void (*complete_callback)(void); /**< lcd display a frame complete callbace */
#endif
} lcd_config_t;



/*
 * @}
 */

#ifdef __cplusplus
}
#endif



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

#include <common/bk_include.h>
#include <driver/hal/hal_jpeg_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief JPEGENC APIs Version 1.0
 * @{
 */

#define Y_PIXEL_272     (34)  /**< image resolution for hight: Y * 8 = 272  */
#define Y_PIXEL_320     (40)  /**< image resolution for hight: Y * 8 = 320  */
#define X_PIXEL_480     (60)  /**< image resolution for width: X * 8 = 480  */
#define Y_PIXEL_480     (60)  /**< image resolution for hight: Y * 8 = 480  */
#define X_PIXEL_640     (80)  /**< image resolution for hight: X * 8 = 640  */
#define Y_PIXEL_240     (30)  /**< image resolution for hight: Y * 8 = 240  */
#define X_PIXEL_320     (40)  /**< image resolution for hight: X * 8 = 320  */
#define Y_PIXEL_600     (75)  /**< image resolution for hight: Y * 8 = 600  */
#define X_PIXEL_800     (100) /**< image resolution for hight: X * 8 = 800  */
#define Y_PIXEL_720     (90)  /**< image resolution for hight: Y * 8 = 720  */
#define X_PIXEL_1280    (160) /**< image resolution for hight: X * 8 = 1280 */
#define X_PIXEL_1600    (200) /**< image resolution for hight: X * 8 = 1600 */
#define Y_PIXEL_1200    (150) /**< image resolution for hight: Y * 8 = 1200 */

typedef enum {
	JPEG_ENC_MODE = 0,
	JPEG_YUV_MODE,
}jpeg_mode_t;

/**
 * @brief jpeg_yuv_format_t when jpeg encode at yuv mode, the output yuv format type
 */
typedef enum {
	JPEG_YUYV = 0, /**< YUYV, bit[31-0]=[YUYV] */
	JPEG_UYVY = 1, /**< UYVY, bit[31-0]=[UYVY] */
	JPEG_YYUV = 2, /**< YYUV, bit[31-0]=[YYUV] */
	JPEG_UVYY = 3, /**< UVYY, bit[31-0]=[UVYY] */
}jpeg_yuv_format_t;

typedef enum {
	END_OF_YUV = 0, /**< when work at yuv mode, transfer a complete frame will trigger this isr */
	HEAD_OUTPUT,    /**< when work at jpeg encode mode, the head output complete will trigger this isr */
	START_OF_FRAME, /**< when work at jpeg encode mode, detect vsync rising edge after few cycle will trigger this isr */
	END_OF_FRAME,   /**< when work at jpeg encode mode, transfer a complete frame will trigger this isr */
	VSYNC_NEGEDGE,  /**< when detect vsync negedge will trigger this isr */
	ISR_MAX,
}jpeg_isr_type_t;

/**
 * @brief jpeg int type
 */
typedef enum {
	JPEG_END_YUV_INT = (1 << 0),
	JPEG_HEAD_OUTPUT_INT = (1 << 1),
	JPEG_START_FRAME_INT = (1 << 2),
	JPEG_END_FRAME_INT = (1 << 3),
	JPEG_VSYNC_NEGEDGE_INT = (1 << 6),
} jpeg_int_type_t;

/**
 * @brief jpeg isr callback relay jpeg driver
 *
 * @param id: only 0 useful, set 0 default
 * @param param: NULL default
 */
typedef void (*jpeg_isr_t)(jpeg_unit_t id, void *param);

typedef struct {
	uint8_t *rxbuf; /**< the address of receiving jpeg data */

	/**
	 * @brief node full handler
	 *
	 * This is a transfer jpeg data to uplayer api, when transfer node_len jpeg data finish , this function will be called
	 *
	 * @param curptr: the start address of transfer data.
	 * @param newlen: the transfer data length
	 * @param is_eof: 0/1: whether this packet data is the last packet of this frame, will called in jpeg_end_frame isr
	 * @param frame_len: the complete jpeg frame size, if is_eof=1, the frame_len is the true value of jpeg frame size, 
	 * is_eof=0, the frame_len=0, in other words, only when transfer really frame_len at the last packet in jpeg_end_frame isr
	 *
	**/
	void (*node_full_handler)(void *curptr, uint32_t newlen, uint32_t is_eof, uint32_t frame_len);

	/**
	 * brief data_end_handler
	 *
	 * This api use to inforamte video transfer thread to deal transfer jpegenc data
	 *
	**/
	void (*data_end_handler)(void);

	uint16_t rxbuf_len;   /**< The length  of receiving jpegenc data buff */
	uint16_t rx_read_len; /**< manage the node_full_handler callback function input params */
	uint32_t node_len;    /**< dma transfer length */
	uint32_t sener_cfg;   /**< The sensor config:[0-15]:image_resolution, [16-31]:image rate */
	uint16_t x_pixel;     /**< jpeg encoder image resolution for width */
	uint16_t y_pixel;     /**< jpeg encoder image resolution for height */

#if CONFIG_GENERAL_DMA
	dma_isr_t dma_rx_handler;       /**< dma transfer finish isr callbck */
	uint8_t dma_channel;           /**< dma channel used for transfer jpeg encoder data */
#endif

#if CONFIG_VIDEO_DVP_LCD
	uint8_t dma_lcd_channel;      /**< dma channel used for lcd display */
	uint8_t *jpeg_dec_src_addr;   /**< jepg data, and jpeg dec arc addr */
	uint8_t *jpeg_dec_dst_addr;    /**< jepg dec data dst addr */
	uint8_t *lcd_display_addr;     /**< lcd diaplay data base addr */
	uint16_t jpeg_dec_pixel_x;    /**< jepg dec x_pixel */
	uint8_t dma_lcd_int_cnt;      /**< lcd disaply dma transfer cnt */
	uint8_t lcd_blend_frame_cnt;
	void (*jpeg_dec_callback)(void * src, void * dst); /**< jpeg dec callback */
#endif
} jpegenc_desc_t;

typedef enum {
	READY = 0, 	       	 /**<  jpeg deca and display ready */
	MEMCPYING, 	       	 /**<  jepg data mem cpying */
	JPEGDE_START, 	     /**<  jepg dec start */
	JPEGDECING,	      	 /**<  jepg decing */
	DISPLAYING, 	       	 /**<  jepg dec complete, lcd display */
	JPEGDED,
}lcd_satus_t;




/**
 * @}
 */

#ifdef __cplusplus
}
#endif


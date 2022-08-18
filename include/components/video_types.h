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

#ifdef __cplusplus
extern "C" {
#endif

#define    PARAM_JPEG_SRC_ADDR_BASE       0X60100000 /**< LCD DISPLAY Define jpeg dec src addr*/
#define    PARAM_JPEG_DEC_DST_ADDR_BASE   0X60300000 /**< LCD DISPLAY Define jpeg dec dst addr*/
#define    PARAM_LCD_DISPLAY_ADDR_BASE    0X60500000 /**< if lcd display data is jpegdec data,the display addr = PARAM_JPEG_DEC_DST_ADDR_BASE*/
#define    PARAM_LCD_ROTATE_BASE          0X60700000 /**< if ENABLE rotate function*/

/**
 * @brief camera image resolution type
 */
typedef enum {
	QVGA_320_240    = 0,  /**< 320*240 */
	VGA_480_272,          /**< 480*272 */
	VGA_320_480,          /**< 640*480 */
	VGA_480_320,          /**< 480*320 */
	VGA_640_480,          /**< 640*480 */
	VGA_800_600,          /**< 800*600 */
	VGA_1280_720,         /**< 1280*720 */
	VGA_1600_1200,        /**< 1600*1200 */
	PPI_MAX
} ppi_type_t; // Pixel per inch

/**
 * @brief camera frame rate type
 */
typedef enum {
	TYPE_5FPS     = 0,     /**< 5fps */
	TYPE_10FPS,            /**< 10fps */
	TYPE_15FPS,            /**< 15fps */
	TYPE_20FPS,            /**< 20fps */
	TYPE_25FPS,            /**< 25fps */
	TYPE_30FPS,            /**< 30fps */
	FPS_MAX
} fps_type_t; // frame per second

#define PPI_POSI        0        /**< replace ppi bit position, start bit 0 */
#define PPI_MASK        0xFF     /**< ppi occupy 8bits, bit[7-0] */
#define FPS_POSI        8        /**< replace fps bit position, start bit 8 */
#define FPS_MASK        0xFF     /**< fps occupy 8bits, bit[15-8] */

/**
 * @brief set ppi type to camera sensor
 */
#define CMPARAM_SET_PPI(p, x)   (p = ((p & ~(PPI_MASK << PPI_POSI)) | ((x & PPI_MASK) << PPI_POSI)))

/**
 * @brief get ppi type from camera sensor param
 */
#define CMPARAM_GET_PPI(p)      ((p >> PPI_POSI) & PPI_MASK)

/**
 * @brief set fps type to camera sensor
 */
#define CMPARAM_SET_FPS(p, x)   (p = ((p & ~(FPS_MASK << FPS_POSI)) | ((x & FPS_MASK) << FPS_POSI)))

/**
 * @brief get fps type from camera sensor param
 */
#define CMPARAM_GET_FPS(p)      ((p >> FPS_POSI) & FPS_MASK)

/**
 * @brief video sample module protocol type
 */
typedef enum {
	TVIDEO_OPEN_NONE         = 0, /**< not sample module */
	TVIDEO_OPEN_SCCB,             /**< sample module follow sccb protocol */
	TVIDEO_OPEN_SPIDMA,           /**< sample module follow spidma protocol */
} video_open_type_t;

/**
 * @brief video transfer network comunication protocol type
 */
typedef enum {
	TVIDEO_SND_NONE         = 0,  /**< not transfer */
	TVIDEO_SND_UDP,               /**< follow udp protocol */
	TVIDEO_SND_TCP,               /**< follow tcp protocol */
	TVIDEO_SND_INTF,              /**< transfer to inter frame */
	TVIDEO_SND_BUFFER,            /**< transfer to buffer */
} video_send_type_t;

typedef struct {
	uint8_t *rxbuf; /**< the buffer save camera data */

	/**
	 * @brief node full handler
	 *
	 * This is a transfer camera data to uplayer api, when transfer node_len jpeg data finish , this function will be called
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
	 * This api use to inforamte video transfer thread to deal transfer camera data
	 *
	**/
	void (*data_end_handler)(void);

	uint16_t rxbuf_len;  /**< The length  of receiving camera data buff */
	uint16_t rx_read_len;/**< manage the node_full_handler callback function input params */
	uint32_t node_len;   /**< video transfer network comunication protocol length a time */
	uint32_t sener_cfg; /**< camera config, ppi[15-8], fps[7-0] */
} video_config_t;

typedef struct {
	uint8_t *ptk_ptr;
	uint32_t ptklen;     /**< The current packet length */
	uint32_t frame_id;   /**< The current packet frame id */
	uint32_t is_eof;     /**< The current packet is the last packet */
	uint32_t frame_len;  /**< The frame length */
} video_packet_t;

typedef void (*tvideo_add_pkt_header)(video_packet_t *param);
typedef int (*video_transfer_send_func)(uint8_t *data, uint32_t len);
typedef void (*video_transfer_start_cb)(void);
typedef void (*video_transfer_end_cb)(void);

typedef struct {
	uint16_t open_type;                  /**< video transfer network comunication protocol type, video_open_type_t */
	uint16_t send_type;                  /**< video transfer network comunication protocol type, video_send_type_t */
	video_transfer_send_func send_func;  /**< function ptr for send to uplayer */
	video_transfer_start_cb start_cb;    /**< function ptr for start to send to uplayer */
	video_transfer_start_cb end_cb;      /**< function ptr for end to send to uplayer */

	uint32_t pkt_header_size;            /**< packet header size */
	tvideo_add_pkt_header add_pkt_header;/**< function ptr for add packet header */
} video_setup_t;

#ifdef __cplusplus
}
#endif

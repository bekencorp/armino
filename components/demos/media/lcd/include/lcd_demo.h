#pragma once

#ifdef __cplusplus
extern "C" {
#endif


/* @brief Overview about lcds demo
 *
 */

/**
 * @brief lcd Demo mocro
 * @defgroup lcd demo group
 * @{
 */
//#define JPEG_DMA_CH     DMA_ID_0
//#define LCD_DMA_CH      DMA_ID_1
#define LCD_FRAMEADDR   0x60000000 /**<define frame base addr */
/*
 * @}
 */


/**
 * @brief lcd demo enum defines
 * @{
 */
typedef struct {
	volatile uint32_t dma_int_cnt;
	uint32_t          dma_transfer_cnt;
	volatile uint32_t lcd_isr_cnt;
	uint32_t          dma_transfer_len;
	volatile uint32_t dma_frame_end_flag;
} dma_transfer_t;

typedef struct{
	uint32_t src_addr;
	uint32_t dst_addr;
	uint8_t x;  /*< src crop addr*/
	uint8_t y;
	uint32_t src_width;
	uint32_t src_height;
	uint32_t dst_width;
	uint32_t dst_height;
}dma2d_crop_params_t;


/**
 * @brief lcd demo enum defines
 * @{
 */
void dma2d_crop_image(dma2d_crop_params_t *crop_params);


/*
 * @}
 */

#ifdef __cplusplus
	}
#endif



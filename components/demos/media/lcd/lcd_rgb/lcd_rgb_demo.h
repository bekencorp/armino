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




#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <driver/int.h>
#include <common/bk_err.h>
#include "sdkconfig.h"
#include <driver/dma2d.h>
#include <driver/psram.h>
#include <driver/dma.h>
#include "bk_general_dma.h"
#include "driver/lcd_disp_types.h"
#include "dma2d_demo.h"

static volatile uint32_t     transferErrorDetected = 0;    /**< Set to 1 if an error transfer is detected */
static volatile uint32_t     transferCompleteDetected = 0; /**< Set to 1 if the DMA2D transfer complete is detected */
static uint32_t              offset_address_area_blended_image_in_lcd_buffer =  0;
static volatile uint32_t     dma_int_cnt = 0;
static volatile uint32_t     dma_int_flag = 0;
extern dma_id_t lcd_dma_id;
extern const uint16_t        rgb_565_green[640];

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
static void mda2d_r2m_transfer_error(dma2d_config_t *dma2d);
static void mda2d_r2m_transfer_complete(dma2d_config_t *dma2d);
#endif

static void dma_fill_finish_isr(dma_id_t id)
{
	dma_int_cnt++;
	if(dma_int_cnt == 5)
	{
		dma_int_cnt = 0;
		dma_int_flag = 1;
		bk_dma_set_src_start_addr(lcd_dma_id, (uint32_t)LCD_FRAMEADDR);
	}
	else {
		bk_dma_set_src_start_addr(lcd_dma_id, (uint32)(LCD_FRAMEADDR + (61440 * dma_int_cnt)));
		BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(lcd_dma_id));
		bk_dma_start(lcd_dma_id);
	}
}

void dma2d_isr()
{
	uint32_t int_status;
	int_status = bk_dma2d_int_status_get();

	if (int_status & DMA2D_CFG_ERROR) {
		bk_dma2d_int_status_clear(DMA2D_CFG_ERROR_STATUS);
		bk_dma2d_int_enable(DMA2D_CFG_ERROR, 0);
		os_printf("transferErrorDetected \r\n");
	}

	/**< Transfer Error Interrupt management ***************************************/
	if (int_status & DMA2D_TRANS_ERROR_STATUS) {
		transferErrorDetected = 1;
		bk_dma2d_int_status_clear(DMA2D_TRANS_ERROR_STATUS);
		bk_dma2d_int_enable(DMA2D_TRANS_ERROR, 0);
		os_printf("transferErrorDetected \r\n");
	}

	if (int_status & DMA2D_TRANS_COMPLETE_STATUS) {
		transferCompleteDetected = 1;
		bk_dma2d_int_status_clear(DMA2D_TRANS_COMPLETE_STATUS);
		bk_dma2d_int_enable(DMA2D_TRANS_COMPLETE, 0);
	}
}

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
static void mda2d_r2m_transfer_error(dma2d_config_t *dma2d)
{
	bk_dma2d_int_status_clear(DMA2D_TRANS_ERROR_STATUS);
	bk_dma2d_int_enable(DMA2D_TRANS_ERROR, DISABLE);
}
static void mda2d_r2m_transfer_complete(dma2d_config_t *dma2d)
{
	bk_dma2d_int_status_clear(DMA2D_TRANS_COMPLETE_STATUS);
	bk_dma2d_int_enable(DMA2D_TRANS_COMPLETE, DISABLE);
}
#endif

static void  dma_start_transfer(uint32_t dma_ch)
{
	dma_config_t dma_config = {0};
	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = (uint32) LCD_FRAMEADDR;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.start_addr = (uint32_t) REG_DISP_DAT_FIFO;
	dma_config.dst.dev = DMA_DEV_LCD_DATA;
	dma_config.dst.width = DMA_DATA_WIDTH_16BITS;
	
	BK_LOG_ON_ERR(bk_dma_init(dma_ch, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(dma_ch, 61440));
	BK_LOG_ON_ERR(bk_dma_register_isr(dma_ch, NULL, dma_fill_finish_isr));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(dma_ch));
	BK_LOG_ON_ERR(bk_dma_start(dma_ch));
}

/**
  * @brief dma2d reg to mem mode,dma2d fill function
           attention: data format is rgb565
  * @param1  the display framebuffer base addr, by defult psram 0x60000000
  * @param2  the lcd_x 
  * @param3  the lcd_y 
  * @param4  the display width , width <= lcd_x - x
  * @param5  the display high , high <= lcd_y - y
  * @param6  color the fill color
  * @return none
  */
void dma2d_fill(uint32_t frameaddr, uint16_t x, uint16_t y, uint16_t width, uint16_t high, uint16_t color)
{
	void *pDiSt=&(((uint16_t *)frameaddr)[y*320+x]);

	dma2d_config_t dma2d_config = {0};

	dma2d_config.init.mode   = DMA2D_R2M; 			 /**< Mode Register to Memory */
	dma2d_config.init.color_mode	   = DMA2D_OUTPUT_RGB565;  /**< DMA2D Output color mode is ARGB4444 (16 bpp) */
	dma2d_config.init.output_offset  = 320 - width;					 /**< No offset in output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;		 /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;	 /**< No alpha inversion for the output image */
	bk_dma2d_driver_init(&dma2d_config);

/*	bk_dma2d_int_config(DMA2D_CFG_ERROR | DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE ,1);
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_ERROR_ISR, mda2d_r2m_transfer_error);
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, mda2d_r2m_transfer_complete);
#else
	bk_dma2d_isr_register(dma2d_isr);
#endif */

	bk_dma2d_start_transfer(&dma2d_config, color, (uint32_t)pDiSt, width, high); 

/*	while (transferCompleteDetected == 0) {;}
	transferCompleteDetected = 0;*/
	while (bk_dma2d_is_transfer_busy()) {
	}
}

void lcd_fill(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint16_t x, y, w, h, color;
	dma_int_cnt  = 0;

	uint32_t frameaddr = 0x60000000;
	x = os_strtoul(argv[1], NULL, 10) & 0xFFFF;
	os_printf("x = %d \r\n", x);

	y = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
	os_printf("y = %d \r\n", y);

	w = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
	os_printf("w = %d \r\n", w);

	
	h= os_strtoul(argv[4], NULL, 10) & 0xFFFF;
	os_printf("h = %d \r\n", h);
	
	color = os_strtoul(argv[5], NULL, 16) & 0xFFFF;
	os_printf("fill_color = %x \r\n", color);

	dma2d_fill(frameaddr, x, y, w, h, color);
	dma_start_transfer(lcd_dma_id);
}



/**
  * @brief dma2d mem to mem mode, dma2d memcpy function
           attention: data format is rgb565
  * @param1  input pixelformat, copy data format
  * @param2  Psrc, source data addr
  * @param3  Pdst, destination data addr
  * @param4  xsize the display mem x size
  * @param5  ysize the display mem y size
  * @param6  src_offline. copy src mem data from offsetline addr,offsetline is calculate by pixel
  * @param6  dest_offline. src mem data copy to destination offsetline addr, offsetline is calculate by pixel
  * @return none
  */

static void dma2d_memcpy(uint32_t pixelformat, void *Psrc, void *Pdst, uint32_t xsize, uint32_t ysize, uint32_t src_offline, uint32_t dest_offline)
{
	dma2d_config_t dma2d_config = {0};

	/*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
	dma2d_config.init.mode         = DMA2D_M2M;             /**< Mode Memory To Memory */
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB565; /**< Output color mode is ARGB4444 : 16 bpp */
	dma2d_config.init.output_offset = dest_offline;                   /**< No offset on output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;     /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;  /**< No alpha inversion for the output image */

	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;      /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                     /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = pixelformat; /**< Input color is ARGB4444 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = src_offline;                     /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;      /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA;   /**< No alpha inversion for the input image */

	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

	bk_dma2d_start_transfer(&dma2d_config, (uint32_t)Psrc, (uint32_t)Pdst, xsize, ysize); 
	while (bk_dma2d_is_transfer_busy()) {}
}


static void dma2d_display(uint16_t index, void *pdst)
{
	void * pdist = (void *) pdst;
	uint16_t *pstart = (uint16_t *)love;
	pstart += (index/PICTURE_COUNT_ROW) * (PICTURE_WIDTH_PIXEL * PICTURE_HIGHT * PICTURE_COUNT_ROW);
	pstart += (index % PICTURE_COUNT_ROW) * PICTURE_WIDTH_PIXEL;
	uint32_t offlinesrc = (PICTURE_COUNT_ROW - 1) * PICTURE_WIDTH_PIXEL;
	uint32_t offlinedist = 320 - PICTURE_WIDTH_PIXEL;

	dma2d_memcpy(DMA2D_OUTPUT_RGB565, (void *)pstart, pdist, PICTURE_WIDTH_PIXEL, PICTURE_HIGHT, offlinesrc, offlinedist);
}

void dma2d_memcpy_display(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t pdst;
	
	pdst = os_strtoul(argv[1], NULL, 16) & 0xFFFFFFFF;
	os_printf("pdst = 0x%x . \r\n", pdst);
		
	//while(1) {
	for (int i = 0; i<	PICTURE_COUNT ; i++) {
			dma2d_display(i, (void *)pdst);
			dma_start_transfer(lcd_dma_id);
		}
	//}
}

/**
  * @brief dma2d mem to mem with blending mode
           attention: data format is rgb565
  * @param1  pFgaddr, foreground layer data addr
  * @param2  pBgaddr, background layer data addr
  * @param3  Pdst, blend destination data addr
  * @param4  fg_offline foreground layer blend from fg_offline addr, offsetline is calculate by pixel
  * @param5  bg_offline bg_offline layer blend from fg_offline addr,offsetline is calculate by pixel
  * @param6  dest_offline. blend complete,and display on dest addr edst_offline
  * @param7  xsize the display lcd x size
  * @param8  ysize the display lcd y size
  * @param9  alpha_value  foreground layer alpha_value
  * @return none
  */

static void dma2d_blend(void *pFgaddr, void *pBgaddr, void *pDst,
							uint32_t fg_offline, uint32_t bg_offline, uint32_t dest_offline,
							uint16_t xsize, uint16_t ysize, int8_t alpha_value)
{
	dma2d_config_t dma2d_config = {0};

	/*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
	dma2d_config.init.mode         = DMA2D_M2M_BLEND;                  /**< Mode Memory To Memory */
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB565;            /**< output format of DMA2D */
	dma2d_config.init.output_offset= dest_offline;   /**< output offset */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;                /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;             /**< No alpha inversion for the output image */

	/**< Foreground layer Configuration */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_REPLACE_ALPHA;    /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = alpha_value;                   /**< 127 : semi-transparent */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565; /**< Foreground color is RGB565 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = fg_offline;                   /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;    /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA; /**< No alpha inversion for the input image */

	/**< Background layer Configuration */
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].alpha_mode  = DMA2D_REPLACE_ALPHA;
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_alpha   = 0x80;                            /**< 255 : fully opaque */
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565;          /**< Background format is ARGB8888*/
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_offset    = bg_offline;/**< Background input offset*/
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].red_blue_swap    = DMA2D_RB_REGULAR;              /**< No R&B swap for the input background image */
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].alpha_inverted  = DMA2D_REGULAR_ALPHA;           /**< No alpha inversion for the input background image */

	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_BACKGROUND_LAYER);

//	bk_dma2d_int_config(DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE ,1);
//	bk_dma2d_isr_register(dma2d_isr);

	/*## RGB565_240x130_1[] is the foreground layer and LCD_FRAME_BUFFER is the background layer */
	bk_dma2d_start_blending(&dma2d_config,
                                  (uint32_t)pFgaddr,                                                      /**< Foreground image */
                                  (uint32_t)pBgaddr,   /**< Background image */
                                  (uint32_t)pDst,  /**< Destination address */
                                  xsize ,                                                                    /**< width in pixels   */
                                  ysize);                                                                    /**< height in pixels   */


	while (bk_dma2d_is_transfer_busy()) {}
}

void bk_example_dma2d_rgb565_to_argb8888pixel(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t RedBlueSwapConfig = 0;

	RedBlueSwapConfig = os_strtoul(argv[1], NULL, 10) & 0xFFFF;
	os_printf("RedBlueSwapConfig = %d \r\n", RedBlueSwapConfig);

	dma2d_config_t dma2d_config = {0};

	/*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
	dma2d_config.init.mode         = DMA2D_M2M_PFC;                  /**< DMA2D Mode memory to memory  with PFC*/
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_ARGB8888;          /**< output format of DMA2D */
	dma2d_config.init.output_offset = 0; 
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;               /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;            /**< No alpha inversion for the output image */

	/**< Foreground layer Configuration */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;    /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                   /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565; /**< Input color is RGB565 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = 0x0;                   /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = RedBlueSwapConfig;   /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA; /**< No alpha inversion for the input image */

	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

	bk_dma2d_start_transfer(&dma2d_config,
                                  (uint32_t)LCD_FRAMEADDR, /**< Source buffer in format RGB565 and size 320x240      */
                                  (uint32_t)(LCD_FRAMEADDR + 0x50000),/**< framebuf+2*(LCD_X_SIZE*sy+sx)*/
                                  IMAGE_SIZE_X,           /**< width in pixels  */
                                  IMAGE_SIZE_Y);        /**< height in pixels */

	while (bk_dma2d_is_transfer_busy()) {}
}

void bk_example_dma2d_argb8888_to_rgb565pixel(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t RedBlueSwapConfig = 0;

	RedBlueSwapConfig = os_strtoul(argv[1], NULL, 10) & 0xFFFF;
	os_printf("RedBlueSwapConfig = %d \r\n", RedBlueSwapConfig);

	dma2d_config_t dma2d_config = {0};

	/*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
	dma2d_config.init.mode         = DMA2D_M2M_PFC;                  /**< DMA2D Mode memory to memory  with PFC*/
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB565;          /**< output format of DMA2D */
	dma2d_config.init.output_offset = 0; 
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;               /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;            /**< No alpha inversion for the output image */

	/**< Foreground layer Configuration */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;    /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                   /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_ARGB8888; /**< Input color is RGB565 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = 0x0;                   /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = RedBlueSwapConfig;   /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA; /**< No alpha inversion for the input image */

	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

	bk_dma2d_start_transfer(&dma2d_config,
                                  (uint32_t)(LCD_FRAMEADDR + 0x50000), /**< Source buffer in format RGB565 and size 320x240      */
                                  (uint32_t)LCD_FRAMEADDR,/**< framebuf+2*(LCD_X_SIZE*sy+sx)*/
                                  IMAGE_SIZE_X,           /**< width in pixels  */
                                  IMAGE_SIZE_Y);        /**< height in pixels */
	while (bk_dma2d_is_transfer_busy()) {}
	dma_start_transfer(lcd_dma_id);
}

void bk_example_alpha_blend(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	const uint8_t cnvFrames = 60;

	///calculate address of output
	offset_address_area_blended_image_in_lcd_buffer =  ((((LCD_Y_SIZE - DEMO_IMG_HEIGHT) / 2) * LCD_X_SIZE)
                                                    + ((LCD_X_SIZE - DEMO_IMG_WIDTH) / 2)) * 2;
	//uint16_t *pFb = &RGB565_320x480[0];
	//uint16_t *pDiSt = pFb + offset_address_area_blended_image_in_lcd_buffer;
	uint16_t *pDiSt = (void *)0x60000000 + offset_address_area_blended_image_in_lcd_buffer;

	uint16_t dist_offline = LCD_X_SIZE - DEMO_IMG_WIDTH;
	uint32_t nextimg = 1;
	uint32_t opa = 0;
	void * pFgaddr = 0;
	void * pBgaddr = 0;
	dma_int_flag  = 0;

	os_printf("pFb addr = %x \r\n", offset_address_area_blended_image_in_lcd_buffer);
	os_printf("pDiSt addr = %x \r\n", pDiSt);

	while(1){
		if(nextimg){
				pFgaddr = (void*)yellow_color;
				pBgaddr = (void*)love;
		}
		else{
			pFgaddr = (void*)love;
			pBgaddr = (void*)yellow_color;
		}
		for(int i = 0; i < cnvFrames; i++) {
			opa = 255 * i / (cnvFrames-1);
			dma2d_blend(pFgaddr, pBgaddr, pDiSt,0, 0, dist_offline,DEMO_IMG_WIDTH, DEMO_IMG_HEIGHT, 
							opa);
			dma_start_transfer(lcd_dma_id);
			while(dma_int_flag == 0) {}
			dma_int_flag = 0;
		}
		nextimg = !nextimg;
	}
}

void bk_example_fill_2p_alpha_blend(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	const uint8_t cnvFrames = 60;
	uint32_t nextimg = 1;
	uint32_t opa = 0;
	void * pFgaddr = 0;
	void * pBgaddr = 0;
	dma_int_cnt  = 0;
	dma_int_flag  = 0;

	uint16_t *pDiSt = (void *)LCD_FRAMEADDR;

	dma2d_fill(0x60050000, 0, 0, 320, 480, 0xffff);
	dma2d_fill(0x60050000, 20, 20, 280, 440, 0xffe0);
	dma2d_fill(0x60050000, 60, 60, 200, 360, 0x001f);
	dma2d_fill(0x60050000, 100, 100, 120, 280, 0xf60F);
	dma2d_fill(0x60050000, 140, 140, 40, 200, 0x07ff);
	
	dma2d_fill(0x60100000, 0,0,320,480,0xffff);
	dma2d_fill(0x60100000, 270,80,10,340,0xf800);
	dma2d_fill(0x60100000, 220,420,60,10,0xf800);
	dma2d_fill(0x60100000, 220,80,10,350,0xf800);
	dma2d_fill(0x60100000, 220,250,60,10,0xf800);
	dma2d_fill(0x60100000, 120,80,60,10,0xf800);
	dma2d_fill(0x60100000, 180,80,10,350,0xf800);
	dma2d_fill(0x60100000, 120,420,60,10,0xf800);
	dma2d_fill(0x60100000, 30,80, 60,10,0xf800);
	dma2d_fill(0x60100000, 80,80, 10,350,0xf800);

	while(1) {
		if(nextimg){
			pFgaddr = (void*)0x60050000;
			pBgaddr = (void*)0x60100000;
		}
		else {
			pFgaddr = (void*)0x60100000;
			pBgaddr = (void*)0x60050000;
		}
		for (int i = 0; i < cnvFrames; i++) {
			opa = 255 * i / (cnvFrames-1);
			dma2d_blend(pFgaddr, pBgaddr, pDiSt, 0, 0, 0, 320, 480, opa);
			dma_start_transfer(lcd_dma_id);
			while(dma_int_flag == 0) {}
			dma_int_flag = 0;
		}
		nextimg = !nextimg;
	}
}

void bk_example_dma2d_deinit(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	dma_int_flag  = 0;

	ret = bk_dma2d_driver_deinit();
	if (ret != BK_OK) {
		os_printf("dma2d driver init failed\r\n");
		return;
	}
}




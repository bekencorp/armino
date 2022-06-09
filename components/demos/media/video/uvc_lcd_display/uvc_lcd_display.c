#include <common/bk_include.h>
#include <stdio.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include "bk_cli.h"

#include "sys_driver.h"
#include <common/bk_err.h>
#include <driver/lcd.h>
#include <driver/dma.h>
#include <driver/gpio.h>
#include <driver/psram.h>
#include <components/jpeg_decode.h>
#include <components/uvc_camera.h>

#if (CONFIG_SDCARD_HOST)
#include "ff.h"
#include "diskio.h"
#include "test_fatfs.h"
#endif

#define PSRAM_BASEADDR        (0x60000000) // JPEG DATA
#define JPEGDEC_DATA_ADDR     (0x601E0000) // JPEG DEC DATA
#define YUV_RESIZE_ADDR       (0x60500000) // YUV RESIZE DATA
#define DMA_TRANSFER_LEN      0xFF00//480*272/2;

extern void delay(INT32 num);

static uint8_t dma_int_cnt = 0;
static uint8_t dma_channel_id = 0;
static bool g_uvc_enable = false;
static uint32_t g_image_width = 640;
static uint32_t g_image_height = 480;

static void cli_jpegdec_help(void)
{
	os_printf("lcd_jpegdec {init|jdec_to_sd|display_picture|stop_display|uvc_dispaly_init|start_uvc|close_uvc|yuvresize|display_uvc|deinit}\n");
	os_printf("-init---------------(init power/sdcard/dma/lcd)-----\r\n");
	os_printf("       width--------(input image width, 640/1280)---\r\n");
	os_printf("       hight--------(input image hight, 480/720)----\r\n");
	os_printf("       ratio--------(decode output image size, 0)---\r\n");
	os_printf("       rgb_clk_div--(rgb clk div, 8/25)-------------\r\n");
	os_printf("\r\n-jdec_to_sd----(jdec data save to sdcard, yuv422)---\r\n");
	os_printf("       input_path--(input image name)-------------------\r\n");
	os_printf("       ouput_path--(output image name)------------------\r\n");
	os_printf(" (op)  resize_path-(resize output image name)-----------\r\n");
	os_printf("\r\n-display_picture---(display yuv422 image, 480*272)-----------\r\n");
	os_printf("       input_path------(input image name)------------------------\r\n");
	os_printf("\r\n-stop_display---(rgb stop display)--------\r\n");
	os_printf("\r\n-uvc_dispaly_init---(init uvc)--------------\r\n");
	os_printf("       width------------(uvc width, 640/1280)---\r\n");
	os_printf("       hight------------(uvc hight, 480/720)----\r\n");
	os_printf("\r\n-start_uvc---(start uvc)----\r\n");
	os_printf("\r\n-close_uvc-- (stop uvc)----\r\n");
	os_printf("\r\n-yuvresize-- (resize original yuv422 to 480*272 and display)----\r\n");
	os_printf("       input_path--(input image name)-------------------------------\r\n");
	os_printf("       ouput_path--(output image name)------------------------------\r\n");
	os_printf("\r\n-display_uvc-- (display 480*272 yyuv image)----\r\n");
	os_printf("       input_path--(input image name)--------------\r\n");
	os_printf("\r\n-dinit-- (deinit all modules)----\r\n");
}

#if (CONFIG_USB_UVC || CONFIG_SDCARD_HOST)
static void image_resize_to_480_272(uint32_t input_size, uint8_t *input_ptr, uint8_t *output_ptr)
{
	uint8_t *left_top_ptr = NULL;
	uint32_t left_top_x = 0;
	uint32_t left_top_y = 0;
	uint32_t width = 0;
	uint32_t compy_length = 480 * 2; // every col have 480 pixel
	switch (input_size) {
	case 720:
		left_top_x = 400;
		left_top_y = 224;
		width = 1280;
		break;
	case 480:
		left_top_x = 80;
		left_top_y = 104;
		width = 640;
		break;
	default:
		return;
	}

	left_top_ptr = ((left_top_y - 1) * width + left_top_x) * 2 + input_ptr;
	for (uint32_t col = left_top_y; col < (left_top_y + 272); col++) {
		os_memcpy(output_ptr, left_top_ptr, compy_length);
		left_top_ptr += width * 2;
		output_ptr += compy_length;
	}
}
#endif

static void lcd_display_frame_isr(void)
{
#if CONFIG_USB_UVC
	if (g_uvc_enable) {
		bk_lcd_rgb_display_en(0);// close rgb display
		bk_uvc_set_mem_status(UVC_MEM_IDLE);
	}
#endif
}

static void dma_jpeg_dec_to_lcdfifo_isr(dma_id_t id)
{
	uint32_t source_start_addr;
	dma_int_cnt++;
	if(dma_int_cnt == 4)
	{
		if (g_uvc_enable) {
			dma_int_cnt = 0;
			bk_dma_stop(dma_channel_id);
			source_start_addr = (uint32_t)YUV_RESIZE_ADDR;
			bk_dma_set_src_addr(dma_channel_id, source_start_addr, 0);
		} else {
			dma_int_cnt = 0;
			source_start_addr = (uint32_t)YUV_RESIZE_ADDR;
			bk_dma_set_src_addr(dma_channel_id, source_start_addr, 0);
			bk_dma_start(dma_channel_id);
		}
	}
	else {
		source_start_addr = (uint32_t)YUV_RESIZE_ADDR + (uint32_t)(DMA_TRANSFER_LEN * dma_int_cnt);
		bk_dma_set_src_addr(dma_channel_id, source_start_addr, 0);
		bk_dma_start(dma_channel_id);
	}
}

static void dma_jpeg_dec_to_lcdfifo(void)
{
	dma_config_t dma_config = {0};

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = (uint32) YUV_RESIZE_ADDR;
	dma_config.src.end_addr = (uint32) YUV_RESIZE_ADDR + DMA_TRANSFER_LEN;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.dev = DMA_DEV_LCD_DATA;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.start_addr =  (uint32) REG_DISP_RGB_FIFO;

	dma_channel_id = bk_dma_alloc(DMA_DEV_DTCM);
	if ((dma_channel_id < DMA_ID_0) || (dma_channel_id >= DMA_ID_MAX)) {
		os_printf("malloc dma fail \r\n");
		return;
	}

	BK_LOG_ON_ERR(bk_dma_init(dma_channel_id, &dma_config));

	BK_LOG_ON_ERR(bk_dma_set_transfer_len(dma_channel_id, DMA_TRANSFER_LEN));
	BK_LOG_ON_ERR(bk_dma_register_isr(dma_channel_id, NULL, dma_jpeg_dec_to_lcdfifo_isr));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(dma_channel_id));
}

#if (CONFIG_USB_UVC || CONFIG_SDCARD_HOST)
static void uvc_jpegdec_frame_end_callback(void)
{
	image_resize_to_480_272(g_image_height, (uint8_t *)JPEGDEC_DATA_ADDR, (uint8_t *)YUV_RESIZE_ADDR);
	bk_lcd_rgb_display_en(1);
	bk_dma_start(dma_channel_id);
}
#endif

#if CONFIG_USB_UVC
static void uvc_jpeg_frame_end_callback(uint32_t pic_size)
{
	bk_err_t ret = BK_OK;
	ret = bk_jpeg_dec_sw_start(pic_size);
	if (ret != BK_OK) {
		bk_uvc_set_mem_status(UVC_MEM_IDLE);
	}
}
#endif // CONFIG_USB_UVC

void lcd_jpeg_dec_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = 0;
	uint32_t rgb_clk_div = 0;

	if (argc < 2) {
		cli_jpegdec_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		uint32_t mode = 0x00054043;
		if (argc < 5) {
			os_printf("init: width height \r\n");
			return;
		}

		g_image_width = os_strtoul(argv[2], NULL, 10);
		g_image_height = os_strtoul(argv[3], NULL, 10);
		rgb_clk_div = os_strtoul(argv[4], NULL, 10);

		// step 1: init psram
		err = bk_psram_init(mode);
		if (err != kNoErr) {
			os_printf("init psram failed\r\n");
			return;
		}

		// step 2: video power_up
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_VIDP,POWER_MODULE_STATE_ON);

		// step 3: init lcd
		bk_lcd_driver_init(LCD_96M);
		if (rgb_clk_div == 0)
			rgb_clk_div = 25;

		bk_lcd_rgb_init(rgb_clk_div, X_PIXEL_RGB, Y_PIXEL_RGB, ORGINAL_YUYV_DATA);

		bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_display_frame_isr);

		// step 4: init lcd_dma, jpeg_dec to rgb_fifo
		dma_jpeg_dec_to_lcdfifo();

		// step 5: init jpeg_dec
		err = bk_jpeg_dec_sw_init((uint8_t *)PSRAM_BASEADDR, (uint8_t *)JPEGDEC_DATA_ADDR);
		if (err != kNoErr) {
			os_printf("init jpeg_decoder failed\r\n");
			return;
		}

		//jpeg_sw_decode_register_finish_callback(uvc_jpegdec_frame_end_callback);

		// step 6: f_mount sdcard if need read sdcard
#if (CONFIG_SDCARD_HOST)
		test_mount(DISK_NUMBER_SDIO_SD);
#endif
	} else if (os_strcmp(argv[1], "jdec_to_sd") == 0) {
#if (CONFIG_SDCARD_HOST)
		char *filename = NULL;
		char *fileout = NULL;
		char cFileName[FF_MAX_LFN];
		FIL file;
		FRESULT fr;
		FSIZE_t size_64bit = 0;
		char *ucRdTemp = (char *)PSRAM_BASEADDR;
		unsigned int uiTemp = 0;

		if (argc < 4) {
			os_printf("jdec_to_sd: picture_name output_file_name\r\n");
			return;
		}

		filename = argv[2];
		fileout = argv[3];

		// step 1: read picture from sd to psram
		sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);

		fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", filename);
			return;
		}

		size_64bit = f_size(&file);

		uint32_t total_size = (uint32_t)size_64bit;// total byte
		fr = f_read(&file, ucRdTemp, total_size, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read file fail.\r\n");
			return;
		}

		fr = f_close(&file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", filename);
			return;
		}

		// step 2: start jpeg_dec
		bk_jpeg_dec_sw_register_finish_callback(NULL);
		err = bk_jpeg_dec_sw_start(total_size);
		if (err != kNoErr) {
			os_printf("jpeg_decoder failed\r\n");
			return;
		}

		// step 3: save jpeg_dec data to sdcard
		sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, fileout);

		fr = f_open(&file, cFileName, FA_OPEN_APPEND | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", fileout);
			return;
		}

		fr = f_write(&file, (char *)JPEGDEC_DATA_ADDR, g_image_width * g_image_height * 2, &uiTemp);
		if (fr != FR_OK) {
			os_printf("write %s fail.\r\n", fileout);
			return;
		}

		fr = f_close(&file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", fileout);
			return;
		}

		if (argc == 5) {
			char * yuv_resize = argv[4];

			image_resize_to_480_272(g_image_height, (uint8_t *)JPEGDEC_DATA_ADDR, (uint8_t *)YUV_RESIZE_ADDR);
			sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, yuv_resize);

			fr = f_open(&file, cFileName, FA_OPEN_APPEND | FA_WRITE);
			if (fr != FR_OK) {
				os_printf("open %s fail.\r\n", yuv_resize);
				return;
			}

			fr = f_write(&file, (char *)YUV_RESIZE_ADDR, 480 * 272 * 2, &uiTemp);
			if (fr != FR_OK) {
				os_printf("write %s fail.\r\n", yuv_resize);
				return;
			}

			fr = f_close(&file);
			if (fr != FR_OK) {
				os_printf("close %s fail!\r\n", yuv_resize);
				return;
			}
		}
#else
		os_printf("Not support\r\n");
#endif
	}else if (os_strcmp(argv[1], "display_picture") == 0) {
#if (CONFIG_SDCARD_HOST)
		char *filename = NULL;
		char cFileName[FF_MAX_LFN];
		FIL file;
		FRESULT fr;
		FSIZE_t size_64bit = 0;
		char *ucRdTemp = (char *)PSRAM_BASEADDR;
		unsigned int uiTemp = 0;

		if (argc != 3) {
			os_printf("display_picture: picture_name\r\n");
			return;
		}

		filename = argv[2];

		// step 1: stop dma from jpeg_dec data to rgb fifo, if rgb display is working
		bk_dma_stop(dma_channel_id);
		dma_int_cnt = 0;

		// step 2: read picture from sd to psram
		sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);

		fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", filename);
			return;
		}

		size_64bit = f_size(&file);

		uint32_t total_size = (uint32_t)size_64bit;// total byte
		fr = f_read(&file, ucRdTemp, total_size, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read file fail.\r\n");
			return;
		}

		fr = f_close(&file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", filename);
			return;
		}

		// step 3: disable uvc, for uvc_jpegdec_frame_end_callback
		g_uvc_enable = false;

		// step 4: in jpeg_dec finish callback start dma
		bk_jpeg_dec_sw_register_finish_callback(uvc_jpegdec_frame_end_callback);

		// step 5: start jpeg_dec
		err = bk_jpeg_dec_sw_start(total_size);
		if (err != kNoErr) {
			os_printf("jpeg_decoder failed\r\n");
			return;
		}
#else
		os_printf("Not support\r\n");
#endif
	} else if (os_strcmp(argv[1], "stop_display") == 0) {
		// step 1: stop dma
		bk_dma_stop(dma_channel_id);
		dma_int_cnt = 0;

		// step 2: stop display
		bk_lcd_rgb_display_en(0);
	} else if (os_strcmp(argv[1], "uvc_display_init") == 0) {
#if CONFIG_USB_UVC
		uint8_t fps = 0;
		uint16_t ppi = 0;

		if (argc != 4) {
			os_printf("uvc_dispaly_init: pps fps\r\n");
			return;
		}

		ppi = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		fps = os_strtoul(argv[3], NULL, 10) & 0xFF;

		// step 1: stop dma from jpeg_dec data to rgb fifo, if rgb display is working
		bk_dma_stop(dma_channel_id);
		dma_int_cnt = 0;

		// step 2: init uvc
		err = bk_uvc_init();
		if (err != kNoErr) {
			os_printf("uvc init failed!\r\n");
			return;
		}

		// step 3: set uvc_pps_fps if need
		err = bk_uvc_set_ppi_fps(ppi, fps);
		if (err != kNoErr) {
			os_printf("uvc set ppi and fps failed!\r\n");
			return;
		}

		// step 4: register frame_end_callback, and excute jpeg_dec
		bk_uvc_register_frame_end_callback(uvc_jpeg_frame_end_callback);

		// step 5: reset lcd_dma if need

		// step 6 enable lcd_dma in jpeg_dec_finish_callback
		bk_jpeg_dec_sw_register_finish_callback(uvc_jpegdec_frame_end_callback);
#else
		os_printf("Not support\r\n");
#endif
	}else if (os_strcmp(argv[1], "start_uvc") == 0) {
#if CONFIG_USB_UVC
		if (argc != 2) {
			os_printf("start_uvc: error\r\n");
			return;
		}
		// step 1: stop dma
		bk_dma_stop(dma_channel_id);
		dma_int_cnt = 0;

		// step 2: enable uvc_enable flag
		g_uvc_enable = true;

		// step 3: start uvc
		err = bk_uvc_set_start();
		if (err != kNoErr) {
			os_printf("uvc set start failed!\r\n");
			return;
		}
#else
		os_printf("Not support\r\n");
#endif
	} else if (os_strcmp(argv[1], "close_uvc") == 0) {
#if CONFIG_USB_UVC
		if (argc != 2) {
			os_printf("close_uvc: error\r\n");
			return;
		}

		// step 1: stop uvc
		err = bk_uvc_set_stop();
		if (err != kNoErr) {
			os_printf("uvc set stop failed!\r\n");
			return;
		}

		// step 2: diasble uvc_enable flag
		g_uvc_enable = false;
#else
		os_printf("Not support\r\n");
#endif
	}else if (os_strcmp(argv[1], "yuvresize") == 0) {
#if (CONFIG_SDCARD_HOST)
		char *filename = NULL;
		char *fileout = NULL;
		char cFileName[FF_MAX_LFN];
		FIL file;
		FRESULT fr;
		FSIZE_t size_64bit = 0;
		char *ucRdTemp = (char *)PSRAM_BASEADDR;
		unsigned int uiTemp = 0;
	
		filename = argv[2];
		fileout = argv[3];

		// step 1: read picture from sd to psram
		sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);

		fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", filename);
			return;
		}

		size_64bit = f_size(&file);

		uint32_t total_size = (uint32_t)size_64bit;// total byte
		fr = f_read(&file, ucRdTemp, total_size, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read file fail.\r\n");
			return;
		}

		fr = f_close(&file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", filename);
			return;
		}

		// step 2: start resize
		image_resize_to_480_272(g_image_height, (uint8_t *)PSRAM_BASEADDR, (uint8_t *)YUV_RESIZE_ADDR);

		// step 3: save jpeg_dec data to sdcard
		sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, fileout);

		fr = f_open(&file, cFileName, FA_OPEN_APPEND | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", fileout);
			return;
		}

		fr = f_write(&file, (char *)YUV_RESIZE_ADDR, 480 * 272 * 2, &uiTemp);
		if (fr != FR_OK) {
			os_printf("write %s fail.\r\n", fileout);
			return;
		}

		fr = f_close(&file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", fileout);
			return;
		}

		g_uvc_enable = false;
		dma_int_cnt = 0;

		bk_lcd_rgb_display_en(1);
		bk_dma_start(dma_channel_id);
#else
		os_printf("Not support\r\n");
#endif
	} else if (os_strcmp(argv[1], "display_uvc") == 0) {
#if (CONFIG_SDCARD_HOST)
		char *filename = NULL;
		char cFileName[FF_MAX_LFN];
		FIL file;
		FRESULT fr;
		FSIZE_t size_64bit = 0;
		char *ucRdTemp = (char *)YUV_RESIZE_ADDR;
		unsigned int uiTemp = 0;
	
		filename = argv[2];

		// step 1: read picture from sd to psram
		sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);

		fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", filename);
			return;
		}

		size_64bit = f_size(&file);

		uint32_t total_size = (uint32_t)size_64bit;// total byte
		fr = f_read(&file, ucRdTemp, total_size, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read file fail.\r\n");
			return;
		}

		fr = f_close(&file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", filename);
			return;
		}

		g_uvc_enable = false;
		dma_int_cnt = 0;

		bk_lcd_rgb_display_en(1);
		bk_dma_start(dma_channel_id);
#else
		os_printf("Not support\r\n");
#endif
	}else if (os_strcmp(argv[1], "deinit") == 0) {
		if (argc != 2) {
			os_printf("deinit: error\r\n");
			return;
		}

		// step 1: deinit uvc if need
#if CONFIG_USB_UVC
		err = bk_uvc_deinit();
		if (err != kNoErr) {
			os_printf("uvc deinit failed!\r\n");
			return;
		}
#else

		// step 2: deinit jpeg_dec
		bk_jpeg_dec_sw_deinit();

		// step 3: stop dma and deinit
		err = bk_dma_deinit(dma_channel_id);
		if (err != kNoErr) {
			os_printf("dma deinit failed!\r\n");
			return;
		}

		err = bk_dma_free(DMA_DEV_DTCM, dma_channel_id);
		if (err != BK_OK) {
			os_printf("dma free failed!\r\n");
			return;
		}

		dma_channel_id = 0;
		dma_int_cnt = 0;

		// step 3: deinit lcd
		bk_lcd_rgb_display_en(0);

		// step 4: video power off
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_VIDP,POWER_MODULE_STATE_OFF);

		// step 5: deinit psram
		delay(2000);
		err = bk_psram_deinit();
		if (err != kNoErr) {
			os_printf("uvc deinit failed!\r\n");
			return;
		}
#endif
		g_uvc_enable = false;
	} else {
		// jpeg_dec cmd help
		cli_jpegdec_help();
		return;
	}
}




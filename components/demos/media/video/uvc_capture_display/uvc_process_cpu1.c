#include <common/bk_include.h>
#include <stdio.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include "bk_cli.h"
#include <modules/pm.h>

#include <common/bk_err.h>
#include <driver/lcd.h>
#include <driver/dma.h>
#include <components/uvc_camera.h>
#include "uvc_mailbox.h"
#include <st7796s.h>
#include <driver/dma2d.h>
#include <lcd_dma2d_config.h>

#if CONFIG_JPEG_DECODE
#include <components/jpeg_decode.h>
#else
#include <driver/jpeg_dec.h>
#include <driver/jpeg_dec_types.h>
#endif


#define JPEGDEC_DATA_ADDR     (0x601E0000) // JPEG DEC DATA
#define YUV_RESIZE_ADDR       (0x60500000) // YUV RESIZE DATA

extern void delay(INT32 num);
static uint8_t g_uvc_frame_rate = 10;
static bool g_uvc_enable = false;
static uint32_t g_image_width = 640;
static uint32_t g_image_height = 480;

#if (CONFIG_VIDEO_UVC_LCD)
static lcd_interface_data_t lcd_if;
static volatile uint8_t dma_int_cnt = 0;
static uint8_t g_lcd_dev = 0;
static uint8_t dma_channel_id = 0;
static uint8_t dma_all_cnt = 0;
static volatile uint8_t  dma_frame_end_flag = 0;
static uint32_t g_dma_transfer_len = 0xFF00;
#endif
beken_thread_t  uvc_cpu1_demo_thread_handle = NULL;
beken_queue_t uvc_cpu1_demo_msg_que = NULL;


static void uvc_process_cpu1_help(void)
{
	os_printf("cpu1 uvc {cpu1_init|cpu1_deinit}\n");
	os_printf("-cpu1_init---------------(init cpu1 uvc mailbox)-----\r\n");
	os_printf("-cpu1_deinit-------------(deinit cpu1 all resource)---\r\n");
}

static bk_err_t uvc_cpu1_send_msg(uint8_t type, uint32_t data)
{
	bk_err_t ret;
	uvc_cpu_msg_t msg;

	if (uvc_cpu1_demo_msg_que) {
		msg.type = type;
		msg.data = data;

		ret = rtos_push_to_queue(&uvc_cpu1_demo_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("uvc_send_msg failed, type:%d\r\n", type);
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

#if (CONFIG_VIDEO_UVC_LCD)
static void image_resize_to_480_272(uint32_t width, uint32_t height, uint8_t *input_ptr, uint8_t *output_ptr)
{
	if (g_lcd_dev) {
		uint8_t *left_top_ptr = NULL;
		uint32_t left_top_x = 0;
		uint32_t left_top_y = 0;
		uint32_t compy_length = 480 * 2; // every col have 480 pixel

		if (width == 1280 && height == 720) {
			left_top_x = 400;
			left_top_y = 224;
		} else if (width == 640 && height == 480) {
			left_top_x = 80;
			left_top_y = 104;
		} else if (width == 480 && height == 272) {
			os_memcmp(output_ptr, input_ptr, width * height * 2);
			return;
		} else {
			os_printf("Not support\r\n");
		}

		left_top_ptr = ((left_top_y - 1) * width + left_top_x) * 2 + input_ptr;
		for (uint32_t col = left_top_y; col < (left_top_y + 272); col++) {
			os_memcpy(output_ptr, left_top_ptr, compy_length);
			left_top_ptr += width * 2;
			output_ptr += compy_length;
		}
	} else {
		//output_ptr = input_ptr;
		os_memcpy(output_ptr, input_ptr, width * height * 2);
	}
}

static void lcd_display_frame_isr(void)
{
	if (g_uvc_enable) {
		bk_lcd_rgb_display_en(0);// close rgb display
		bk_uvc_set_mem_status(UVC_MEM_IDLE);
	}
}

static void lcd_8080_display_frame_isr(void)
{
	if (g_uvc_enable)
	{
		bk_lcd_rgb_display_en(0);
		bk_lcd_pixel_config(X_PIXEL_8080, Y_PIXEL_8080);
		bk_lcd_rgb_display_en(1);
		bk_lcd_8080_write_cmd(RAM_WRITE); //CONTINUE_WRITE
		bk_uvc_set_mem_status(UVC_MEM_IDLE);
		
	}
}
static void dma_jpeg_dec_to_lcdfifo_isr(dma_id_t id)
{
	uint32_t source_start_addr;
	dma_int_cnt++;
	if(dma_int_cnt == dma_all_cnt)
	{
		if (lcd_if == LCD_8080_16BIT) {
			if (dma_frame_end_flag == 0) {
				dma_int_cnt = 0;
				dma_frame_end_flag = 1;
			}
		}
		if (g_uvc_enable) {
			dma_int_cnt = 0;
			bk_dma_stop(dma_channel_id);
			source_start_addr = (uint32_t)YUV_RESIZE_ADDR;
			bk_dma_set_src_addr(dma_channel_id, source_start_addr, 0);
		} else {
			dma_int_cnt = 0;
			source_start_addr = (uint32_t)YUV_RESIZE_ADDR;
			bk_dma_set_src_addr(dma_channel_id, source_start_addr, 0);
			bk_dma_stop(dma_channel_id);
		}
	}
	else {
		source_start_addr = (uint32_t)YUV_RESIZE_ADDR + (uint32_t)(g_dma_transfer_len * dma_int_cnt);
		bk_dma_set_src_addr(dma_channel_id, source_start_addr, 0);
		bk_dma_start(dma_channel_id);
	}
}

static void dma_jpeg_dec_to_lcdfifo(lcd_interface_data_t lcd_if)
{
	uint32_t rgb_fifo = bk_lcd_get_rgb_data_fifo_addr();
	uint32_t i8080_data_fifo = bk_lcd_get_8080_data_fifo_addr();

	dma_config_t dma_config = {0};

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = (uint32) YUV_RESIZE_ADDR;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.dev = DMA_DEV_LCD_DATA;
	
	if (lcd_if == LCD_8080_16BIT) {
		dma_config.dst.start_addr = i8080_data_fifo;
		dma_config.dst.width = DMA_DATA_WIDTH_16BITS;
	} else if ((lcd_if == LCD_RGB_RGB565) || (lcd_if == LCD_RGB_RGB888)) {//rgb fifo
		dma_config.dst.start_addr = (uint32) rgb_fifo;
		dma_config.dst.width = DMA_DATA_WIDTH_16BITS;
	} else if (lcd_if == LCD_RGB_YUV) {
		dma_config.dst.start_addr = (uint32) rgb_fifo;
		dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	} else{
		os_printf("LCD interface select error, LCD_8080_16BIT | LCD_RGB_RGB565 | LCD_RGB_YUV \r\n");
	}

	BK_LOG_ON_ERR(bk_dma_init(dma_channel_id, &dma_config));

	BK_LOG_ON_ERR(bk_dma_set_transfer_len(dma_channel_id, g_dma_transfer_len));
	BK_LOG_ON_ERR(bk_dma_register_isr(dma_channel_id, NULL, dma_jpeg_dec_to_lcdfifo_isr));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(dma_channel_id));
}

static void uvc_jpegdec_frame_end_callback(void *param)
{
	if(lcd_if == LCD_8080_16BIT) {
		dma2d_crop_params_t  crop_params;
		crop_params.src_addr = (uint32_t)JPEGDEC_DATA_ADDR;
		crop_params.dst_addr = (uint32_t)YUV_RESIZE_ADDR;
		crop_params.x = (g_image_width - 320)/2;
		crop_params.y = (g_image_height - 480)/2;
		crop_params.src_width = g_image_width;
		crop_params.src_height = g_image_height;
		crop_params.dst_width = 320;
		crop_params.dst_height = 480;
		dma2d_crop_image(&crop_params);
	} else if ((lcd_if == LCD_RGB_RGB565) || (lcd_if == LCD_RGB_YUV)) {
		image_resize_to_480_272(g_image_width, g_image_height, (uint8_t *)JPEGDEC_DATA_ADDR, (uint8_t *)YUV_RESIZE_ADDR);
		bk_lcd_rgb_display_en(1);
	}
	bk_dma_start(dma_channel_id);
}

static void dma_pre_config(void)
{
	dma_config_t dma_config = {0};

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = (uint32_t)YUV_RESIZE_ADDR;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.start_addr = (uint32_t) JPEGDEC_DATA_ADDR;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;

	BK_LOG_ON_ERR(bk_dma_init(dma_channel_id, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(dma_channel_id, 640)); 
	BK_LOG_ON_ERR(bk_dma_register_isr(dma_channel_id, NULL, dma_jpeg_dec_to_lcdfifo_isr));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(dma_channel_id));
	BK_LOG_ON_ERR(bk_dma_start(dma_channel_id));
}

static void uvc_jpeg_frame_end_callback(void *buf, uint32_t pic_size)
{
	bk_err_t ret = BK_OK;
#if (CONFIG_JPEG_DECODE)
	ret = bk_jpeg_dec_sw_start(pic_size);
#else
	ret = bk_jpeg_dec_init((uint32_t *)buf, (uint32_t *)JPEGDEC_DATA_ADDR);
#endif
	if (ret != BK_OK) {
		bk_uvc_set_mem_status(UVC_MEM_IDLE);
	}
}
#endif

static void uvc_frame_capture_callback(void *buf, uint32_t pic_size)
{
	uvc_cpu1_send_msg(UVC_CPU1_CAPTURE, pic_size);
}



//lcd_if: 0-RGB, 1-8080
static bk_err_t uvc_cpu1_uvc_init(uint32_t frame_resolution, uint32_t fps, lcd_interface_data_t lcd_if)
{
	bk_err_t err = BK_OK;

	g_image_width = frame_resolution >> 16;
	g_image_height = frame_resolution & 0xFFFF;

#if (CONFIG_VIDEO_UVC_LCD)
	uint32_t dis_clk_div = 25;

	// step 1: init rgb lcd
	if( lcd_if != 0) {
		// step 1: lcd power_up
		bk_lcd_driver_init(LCD_96M);

		dma_channel_id = bk_dma_alloc(DMA_DEV_LCD_DATA);
		if ((dma_channel_id < DMA_ID_0) || (dma_channel_id >= DMA_ID_MAX)) {
			os_printf("malloc dma fail \r\n");
			return err;
		}
		os_printf("malloc dma ch%d \r\n", dma_channel_id);

		// step 2: init jpeg_dec
#if (CONFIG_JPEG_DECODE)
		err = bk_jpeg_dec_sw_init((uint8_t *)UVC_DATA_ADDR, (uint8_t *)JPEGDEC_DATA_ADDR);
		if (err != kNoErr) {
			os_printf("init jpeg_decoder failed\r\n");
			return err;
		}

		bk_jpeg_dec_sw_register_finish_callback(uvc_jpegdec_frame_end_callback);
#else
		jpeg_dec_xpixel_t x_pixel;
		err = bk_jpeg_dec_driver_init();
		if (err != kNoErr) {
			os_printf("init jpeg_decoder driver failed\r\n");
			return err;
		}

		switch (g_image_width) {
			case 320:
				x_pixel = JPEGDEC_X_PIXEL_320;
				break;
			case 480:
				x_pixel = JPEGDEC_X_PIXEL_480;
				break;
			case 640:
				x_pixel = JPEGDEC_X_PIXEL_640;
				break;
			case 1280:
				x_pixel = JPEGDEC_X_PIXEL_1280;
				break;
			default:
				x_pixel = JPEGDEC_X_PIXEL_480;
				break;
		}
		bk_jpeg_dec_complete_cb(uvc_jpegdec_frame_end_callback, x_pixel);
#endif
	}
	if ((lcd_if == LCD_RGB_RGB565)  || (lcd_if == LCD_RGB_RGB888)) {
			g_lcd_dev = 1;
			bk_lcd_rgb_init(dis_clk_div, X_PIXEL_RGB, Y_PIXEL_RGB, RGB565_DATA);
			dma_all_cnt = 4;
			g_dma_transfer_len = 0xFF00;

		bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_display_frame_isr);

		// step 3: init lcd_dma, jpeg_dec to rgb_fifo
		dma_jpeg_dec_to_lcdfifo(LCD_RGB_RGB565);

	} else if (lcd_if == LCD_RGB_YUV) {
		if (g_image_width == X_PIXEL_RGB && g_image_height == Y_PIXEL_RGB_800) {
			g_lcd_dev = 0;
			bk_lcd_rgb_init(dis_clk_div, X_PIXEL_RGB, Y_PIXEL_RGB_800, ORGINAL_YUYV_DATA);
			dma_all_cnt = 15;
			g_dma_transfer_len = 0xC800;
		} else {
			g_lcd_dev = 1;
			bk_lcd_rgb_init(dis_clk_div, X_PIXEL_RGB, Y_PIXEL_RGB, ORGINAL_YUYV_DATA);
			dma_all_cnt = 4;
			g_dma_transfer_len = 0xFF00;
		}

		bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_display_frame_isr);

		// step 3: init lcd_dma, jpeg_dec to rgb_fifo
		dma_jpeg_dec_to_lcdfifo(LCD_RGB_YUV );
	} else if (lcd_if == LCD_8080_16BIT) {
		os_printf("i8080 lcd init. \r\n");
		bk_lcd_8080_init(X_PIXEL_8080, Y_PIXEL_8080);
		dma_all_cnt = 5;
		g_dma_transfer_len = 0xF000;

		os_printf("st7796 init. \r\n");
		err = st7796s_init();
		if (err != BK_OK) {
			os_printf("st7796s init failed\r\n");
			return err;
		}
		bk_lcd_isr_register(I8080_OUTPUT_EOF, lcd_8080_display_frame_isr);
		os_printf("i8080 pre dma1 config. \r\n");

		dma_pre_config();
		while(dma_frame_end_flag == 0) {
			os_printf("dma_frame_end_flag= %d. \r\n", dma_frame_end_flag);
		}

		// step 3: init lcd_dma, jpeg_dec to rgb_fifo
		dma_jpeg_dec_to_lcdfifo(LCD_8080_16BIT);
		os_printf("8080 lcd start transfer. \r\n");
		bk_lcd_8080_start_transfer(1);
		bk_lcd_8080_write_cmd(RAM_WRITE);
	}else{
		os_printf("no lcd interface select . \r\n");
	}
	os_printf("jpeg_decoder UVC_DATA_ADDR src = %X, %x\r\n", UVC_DATA_ADDR, JPEGDEC_DATA_ADDR);
#endif
	err = bk_uvc_init();
	if (err != kNoErr) {
		os_printf("uvc open failed!\r\n");
		return err;
	}

	err = bk_uvc_set_ppi_fps(frame_resolution, (uint8_t)fps);
	if (err != kNoErr) {
		os_printf("uvc set ppi and fps failed!\r\n");
		return err;
	}

	return err;
}

static bk_err_t uvc_cpu1_deinit(void)
{
	bk_err_t err = BK_OK;
	// step 1: deinit uvc if need
	err = bk_uvc_deinit();
	if (err != kNoErr) {
		os_printf("uvc deinit failed!\r\n");
	}
	g_uvc_enable = false;

#if (CONFIG_VIDEO_UVC_LCD)
	if (lcd_if != 0) {
		// step 2: deinit jpeg_dec
#if CONFIG_JPEG_DECODE
		err = bk_jpeg_dec_sw_deinit();
#else
		err = bk_jpeg_dec_driver_deinit();
#endif
		if (err != kNoErr) {
			os_printf("jpeg decode deinit failed!\r\n");
		}

		// step 3: stop dma and deinit
		err = bk_dma_deinit(dma_channel_id);
		if (err != kNoErr) {
			os_printf("dma deinit failed!\r\n");
		}

		err = bk_dma_free(DMA_DEV_LCD_DATA, dma_channel_id);
		if (err != BK_OK) {
			os_printf("dma free failed!\r\n");
		}

		dma_channel_id = 0;
		dma_int_cnt = 0;

		// step 3: deinit lcd
		bk_lcd_rgb_display_en(0);

		// step 4: lcd power off
		bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_LCD, PM_POWER_MODULE_STATE_OFF);
	}
#endif
	return BK_OK;
}

static bk_err_t uvc_cpu1_process_cpu0_cmd(uint32_t cmd_type)
{
	bk_err_t err = BK_OK;
	switch (cmd_type) {
		case UVC_MB_UVC_START:
		{
			// step 1: enable uvc_enable flag
			g_uvc_enable = true;

			// step 2: start uvc
			err = bk_uvc_set_start();
			if (err != kNoErr) {
				os_printf("uvc set start failed!\r\n");
			}
			os_printf("cpu1 uvc start ok\r\n");
			break;
		}
		case UVC_MB_UVC_STOP:
		{
			// step 1: stop uvc
			err = bk_uvc_set_stop();
			if (err != kNoErr) {
				os_printf("uvc set stop failed!\r\n");
				break;
			}
#if (CONFIG_VIDEO_UVC_LCD)
			if (lcd_if != 0) {
				// step 2: stop rgb display
				bk_lcd_rgb_display_en(0);
				// step 3: stop dma
				bk_dma_stop(dma_channel_id);
				if(lcd_if == LCD_8080_16BIT) {
					bk_lcd_pixel_config(X_PIXEL_8080, Y_PIXEL_8080);
					bk_lcd_rgb_display_en(1);
					bk_lcd_8080_write_cmd(RAM_WRITE); //CONTINUE_WRITE
				}
				dma_int_cnt = 0;
				bk_dma_set_src_addr(dma_channel_id, (uint32_t)YUV_RESIZE_ADDR, 0);
			}
#endif			// step 4: diasble uvc_enable flag
			g_uvc_enable = false;
			os_printf("cpu1 uvc stop ok\r\n");
			break;
		}
		case UVC_MB_CAPTURE_START:
		{
			err = bk_uvc_save_frame(0);
			if (err != kNoErr) {
				os_printf("uvc capture failed!\r\n");
			}
			os_printf("cpu1 uvc capture ok\r\n");
			break;
		}
		case UVC_MB_UVC_DEINIT:
		{
			if (uvc_cpu1_demo_thread_handle != NULL) {
				uvc_cpu1_send_msg(UVC_CPU1_DEINIT, 0);
			}
			os_printf("cpu1 uvc deinit ok\r\n");
			break;
		}
		default:
			os_printf("cmd not find\r\n");
	}

	return err;
}

static void uvc_cp1_mailbox_tx_cmpl_isr(uvc_mb_t *uvc_mb, mb_chnl_ack_t *cmd_buf)
{
	//os_printf("enter cp1_mailbox_tx_cmpl \r\n");
	//os_printf("ack  hdr:0x%08x,data1:0x%08x,data2:0x%08x,state:0x%08x\r\n", cmd_buf->hdr.data, cmd_buf->ack_data1, cmd_buf->ack_data2, cmd_buf->ack_state);
}

static void uvc_cp1_mailbox_rx_isr(uvc_mb_t *uvc_mb, mb_chnl_cmd_t *cmd_buf)
{
	bk_err_t err = BK_OK;
	if (cmd_buf->hdr.cmd == UVC_MB_UVC_INIT) {
		g_uvc_frame_rate = cmd_buf->param2;
		uvc_cpu1_send_msg(UVC_CPU1_INIT, cmd_buf->param1);
	} else if (cmd_buf->hdr.cmd == UVC_MB_DISPLAY_UVC_INIT) {
#if (CONFIG_VIDEO_UVC_LCD)
		g_uvc_frame_rate = cmd_buf->param2;
		lcd_if = cmd_buf->param3;
		uvc_cpu1_send_msg(UVC_CPU1_DISPLAY_INIT, cmd_buf->param1);
#endif
	} else if (cmd_buf->hdr.cmd == UVC_MB_CAPTURE_DONE) {
		bk_uvc_set_mem_status(UVC_MEM_IDLE);
	} else {
		err = uvc_cpu1_process_cpu0_cmd(cmd_buf->hdr.cmd);
		if (err != BK_OK) {
			os_printf("uvc init failed\r\n");
		}
	}
}

static void uvc_cpu1_process_main(void)
{
	bk_err_t err;

	while (1) {
		uvc_cpu_msg_t msg;
		err = rtos_pop_from_queue(&uvc_cpu1_demo_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == err) {
			switch (msg.type) {
			case UVC_CPU1_INIT:
			{
				err = uvc_cpu1_uvc_init(msg.data, g_uvc_frame_rate, 0);
				if (err != BK_OK) {
					os_printf("cpu1 init uvc failed\r\n");
					goto exit;
				}
				bk_uvc_register_frame_capture_callback(uvc_frame_capture_callback);
				break;
			}
			case UVC_CPU1_DISPLAY_INIT:
			{
#if (CONFIG_VIDEO_UVC_LCD)
				err = uvc_cpu1_uvc_init(msg.data, g_uvc_frame_rate, lcd_if);
				if (err != BK_OK) {
					os_printf("cpu1 init display_uvc failed\r\n");
					goto exit;
				}
				bk_uvc_register_frame_end_callback(uvc_jpeg_frame_end_callback);
#endif
				break;
			}
			case UVC_CPU1_CAPTURE:
			{
				uvc_mailbox_msg_t mb_msg;
				mb_msg.mb_cmd = UVC_MB_CAPTURE_BUSY;
				mb_msg.param1 = msg.data;
				mb_msg.param2 = 0;
				mb_msg.param3 = 0;
				uvc_mailbox_send_msg(&mb_msg);
				break;
			}
			case UVC_CPU1_DEINIT:
				goto exit;
			default:
				break;
			}
		}
	}

exit:
	uvc_cpu1_deinit();

	uvc_mailbox_deinit();

	rtos_deinit_queue(&uvc_cpu1_demo_msg_que);
	uvc_cpu1_demo_msg_que = NULL;

	uvc_cpu1_demo_thread_handle = NULL;
	rtos_delete_thread(NULL);
	os_printf("cpu1 deinit finish\r\n");
}


static bk_err_t uvc_cpu1_init(void)
{
	bk_err_t ret;

	// step1: init cpu1 uvc mailbox channel, and callback
	uvc_mailbox_init(uvc_cp1_mailbox_rx_isr, NULL, uvc_cp1_mailbox_tx_cmpl_isr);

	if ((!uvc_cpu1_demo_thread_handle) && (!uvc_cpu1_demo_msg_que)) {

		ret = rtos_init_queue(&uvc_cpu1_demo_msg_que,
							  "uvc_cpu1_queue",
							  sizeof(uvc_cpu_msg_t),
							  30);
		if (kNoErr != ret) {
			return kGeneralErr;
		}

		ret = rtos_create_thread(&uvc_cpu1_demo_thread_handle,
								 4,
								 "uvc_cpu1_init",
								 (beken_thread_function_t)uvc_cpu1_process_main,
								 4 * 1024,
								 (beken_thread_arg_t)0);

		if (ret != kNoErr) {
			rtos_deinit_queue(&uvc_cpu1_demo_msg_que);
			uvc_cpu1_demo_msg_que = NULL;
			uvc_cpu1_demo_thread_handle = NULL;
			return kGeneralErr;
		}

		return kNoErr;
	} else
		return kInProgressErr;
}


void uvc_process_cpu1(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t err = BK_OK;
	if (os_strcmp(argv[1], "cpu1_init") == 0) {
		err = uvc_cpu1_init();
		if (err != BK_OK) {
			os_printf("uvc cpu1 init failed\r\n");
		}
	} else if (os_strcmp(argv[1], "ppi_fps") == 0) {
		uint16_t count = os_strtoul(argv[2], NULL, 10);
		if (count > 12) {
			os_printf("NOT Support over 12 kind of image resolution type\r\n");
			return;
		}
		uvc_camera_device_t * param = os_malloc(count * sizeof (uvc_camera_device_t));
		if (param == NULL) {
			os_printf("memory not enough\r\n");
			return;
		}

		os_memset(param, 0, sizeof(param));
		bk_uvc_get_ppi_fps(param, count);

		for (uint8_t i = 0; i < count; i++) {
			if (param[i].width == 0)
				break;
			os_printf("width:%d, height:%d, fps:%d\r\n", param[i].width, param[i].height, param[i].fps);
		}

		os_free(param);
		param = NULL;
	} else if (os_strcmp(argv[1], "cpu1_deinit") == 0) {
		if (uvc_cpu1_demo_thread_handle != NULL) {
			uvc_cpu1_send_msg(UVC_CPU1_DEINIT, 0);
		}
		os_printf("cpu1 deinit ok\r\n");
	} else {
		os_printf("Cmd not find\r\n");
		uvc_process_cpu1_help();
	}
}


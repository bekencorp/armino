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
#include "uvc_mailbox.h"

#define PSRAM_BASEADDR        (0x60000000) // JPEG DATA
#define JPEGDEC_DATA_ADDR     (0x601E0000) // JPEG DEC DATA
#define YUV_RESIZE_ADDR       (0x60500000) // YUV RESIZE DATA
#define DMA_TRANSFER_LEN      0xFF00//480*272/2;

extern void delay(INT32 num);

static uint8_t dma_int_cnt = 0;
static uint8_t dma_channel_id = 0;
static uint8_t dis_clk_div = 13;
static uint8_t g_uvc_frame_rate = 10;
static bool g_uvc_enable = false;
static uint32_t g_image_width = 640;
static uint32_t g_image_height = 480;

beken_thread_t  uvc_cpu1_demo_thread_handle = NULL;
beken_queue_t uvc_cpu1_demo_msg_que = NULL;


static void uvc_process_cpu1_help(void)
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

static void uvc_jpegdec_frame_end_callback(void)
{
	image_resize_to_480_272(g_image_height, (uint8_t *)JPEGDEC_DATA_ADDR, (uint8_t *)YUV_RESIZE_ADDR);
	bk_lcd_rgb_display_en(1);
	bk_dma_start(dma_channel_id);
}

#if CONFIG_USB_UVC
static void uvc_jpeg_frame_end_callback(uint32_t pic_size)
{
	bk_err_t ret = BK_OK;
	ret = bk_jpeg_dec_sw_start(pic_size);
	if (ret != BK_OK) {
		bk_uvc_set_mem_status(UVC_MEM_IDLE);
	}
}

static void uvc_frame_capture_callback(uint32_t pic_size)
{
	uvc_cpu1_send_msg(UVC_CPU1_CAPTURE, pic_size);
}
#endif // CONFIG_USB_UVC

static bk_err_t uvc_cpu1_uvc_init(uint32_t frame_resolution, uint32_t fps, uint32_t dis_clk_div)
{
	bk_err_t err = BK_OK;

	// step 1: video power_up
	sys_drv_module_power_ctrl(POWER_MODULE_NAME_VIDP,POWER_MODULE_STATE_ON);

	g_image_width = frame_resolution >> 16;
	g_image_height = frame_resolution & 0xFFFF;

	// step 2: init lcd
	bk_lcd_driver_init(LCD_96M);
	if (dis_clk_div == 0)
		dis_clk_div = 25;

	bk_lcd_rgb_init(dis_clk_div, X_PIXEL_RGB, Y_PIXEL_RGB, ORGINAL_YUYV_DATA);

	bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_display_frame_isr);

	// step 3: init lcd_dma, jpeg_dec to rgb_fifo
	dma_jpeg_dec_to_lcdfifo();

	// step 5: init jpeg_dec
	err = bk_jpeg_dec_sw_init((uint8_t *)PSRAM_BASEADDR, (uint8_t *)JPEGDEC_DATA_ADDR);
	if (err != kNoErr) {
		os_printf("init jpeg_decoder failed\r\n");
		return err;
	}

	bk_jpeg_dec_sw_register_finish_callback(uvc_jpegdec_frame_end_callback);

#if CONFIG_USB_UVC
	err = bk_uvc_init();
	if (err != kNoErr) {
		os_printf("uvc open failed!\r\n");
		return err;
	}

	err = bk_uvc_set_ppi_fps((uint16_t)g_image_height, (uint8_t)fps);
	if (err != kNoErr) {
		os_printf("uvc set ppi and fps failed!\r\n");
		return err;
	}
#endif
	return err;
}

static bk_err_t uvc_cpu1_deinit(void)
{
	bk_err_t err = BK_OK;
#if CONFIG_USB_UVC
	// step 1: deinit uvc if need
	err = bk_uvc_deinit();
	if (err != kNoErr) {
		os_printf("uvc deinit failed!\r\n");
	}
	g_uvc_enable = false;
#endif
	// step 2: deinit jpeg_dec
	err = bk_jpeg_dec_sw_deinit();
	if (err != kNoErr) {
		os_printf("jpeg decode deinit failed!\r\n");
	}

	// step 3: stop dma and deinit
	err = bk_dma_deinit(dma_channel_id);
	if (err != kNoErr) {
		os_printf("dma deinit failed!\r\n");
	}

	err = bk_dma_free(DMA_DEV_DTCM, dma_channel_id);
	if (err != BK_OK) {
		os_printf("dma free failed!\r\n");
	}

	dma_channel_id = 0;
	dma_int_cnt = 0;

	// step 3: deinit lcd
	bk_lcd_rgb_display_en(0);

	// step 4: video power off
	sys_drv_module_power_ctrl(POWER_MODULE_NAME_VIDP,POWER_MODULE_STATE_OFF);
	return BK_OK;
}

static bk_err_t uvc_cpu1_process_cpu0_cmd(uint32_t cmd_type)
{
	bk_err_t err = BK_OK;
	switch (cmd_type) {
		case UVC_MB_UVC_START:
		{
#if CONFIG_USB_UVC
			// step 1: stop dma
			bk_dma_stop(dma_channel_id);
			dma_int_cnt = 0;

			// step 2: enable uvc_enable flag
			g_uvc_enable = true;

			// step 3: start uvc
			err = bk_uvc_set_start();
			if (err != kNoErr) {
				os_printf("uvc set start failed!\r\n");
			}
			os_printf("cpu1 uvc start ok\r\n");
			break;
#else
			os_printf("Not support\r\n");
			err = BK_FAIL;
			break;
#endif
		}
		case UVC_MB_UVC_STOP:
		{
#if CONFIG_USB_UVC
			// step 1: stop uvc
			err = bk_uvc_set_stop();
			if (err != kNoErr) {
				os_printf("uvc set stop failed!\r\n");
				break;
			}

			// step 2: diasble uvc_enable flag
			g_uvc_enable = false;
			os_printf("cpu1 uvc start ok\r\n");
			break;
#else
			os_printf("Not support\r\n");
			err = BK_FAIL;
			break;
#endif
		}
		case UVC_MB_CAPTURE_START:
		{
#if CONFIG_USB_UVC
			err = bk_uvc_save_frame(0);
			if (err != kNoErr) {
				os_printf("uvc capture failed!\r\n");
			}
			os_printf("cpu1 uvc start ok\r\n");
			break;
#else
			os_printf("Not support\r\n");
			err = BK_FAIL;
			break;
#endif
		}
		case UVC_MB_UVC_DEINIT:
		{
			uvc_cpu1_send_msg(UVC_CPU1_DEINIT, 0);
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
	//os_printf("enter cp1_mailbox_rx_isr \r\n");
	//os_printf("cmd  hdr:0x%08x,pa1:0x%08x,pa2:0x%08x,pa3:0x%08x\r\n", cmd_buf->hdr.data, cmd_buf->param1, cmd_buf->param2, cmd_buf->param3);
	if (cmd_buf->hdr.cmd == UVC_MB_UVC_INIT) {
		g_uvc_frame_rate = cmd_buf->param2;
		uvc_cpu1_send_msg(UVC_CPU1_INIT, cmd_buf->param1);
	} else if (cmd_buf->hdr.cmd == UVC_MB_DISPLAY_UVC_INIT) {
		g_uvc_frame_rate = cmd_buf->param2;
		dis_clk_div = cmd_buf->param3;
		uvc_cpu1_send_msg(UVC_CPU1_DISPLAY_INIT, cmd_buf->param1);
	} else if (cmd_buf->hdr.cmd == UVC_MB_CAPTURE_DONE) {
		bk_uvc_set_mem_status(UVC_MEM_IDLE);
	} else if (cmd_buf->hdr.cmd == UVC_MB_UVC_DEINIT) {
		uvc_cpu1_send_msg(UVC_CPU1_DEINIT, 0);
	}else {
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
				err = uvc_cpu1_uvc_init(msg.data, g_uvc_frame_rate, dis_clk_div);
				if (err != BK_OK) {
					os_printf("cpu1 init uvc failed\r\n");
					goto exit;
				}
				bk_uvc_register_frame_capture_callback(uvc_frame_capture_callback);
				break;
			}
			case UVC_CPU1_DISPLAY_INIT:
			{
				err = uvc_cpu1_uvc_init(msg.data, g_uvc_frame_rate, dis_clk_div);
				if (err != BK_OK) {
					os_printf("cpu1 init display_uvc failed\r\n");
					goto exit;
				}
				bk_uvc_register_frame_end_callback(uvc_jpeg_frame_end_callback);
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
							  "uvc_queue",
							  sizeof(uvc_cpu_msg_t),
							  30);
		if (kNoErr != ret) {
			return kGeneralErr;
		}

		ret = rtos_create_thread(&uvc_cpu1_demo_thread_handle,
								 4,
								 "uvc_init",
								 (beken_thread_function_t)uvc_cpu1_process_main,
								 2 * 1024,
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
	} else if (os_strcmp(argv[1], "cpu1_deinit") == 0) {
		uvc_cpu1_send_msg(UVC_CPU1_DEINIT, 0);
	} else {
		os_printf("Cmd not find\r\n");
		uvc_process_cpu1_help();
	}
}


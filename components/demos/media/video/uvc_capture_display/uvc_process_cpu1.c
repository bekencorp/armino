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


#define JPEGDEC_DATA_ADDR     (0x601E0000) // JPEG DEC DATA
#define YUV_RESIZE_ADDR       (0x60500000) // YUV RESIZE DATA

extern void delay(INT32 num);
static uint8_t g_uvc_frame_rate = 10;
static bool g_uvc_enable = false;
static uint32_t g_image_width = 640;
static uint32_t g_image_height = 480;


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


static void uvc_frame_capture_callback(void *buf, uint32_t pic_size)
{
	uvc_cpu1_send_msg(UVC_CPU1_CAPTURE, pic_size);
}



//lcd_if: 0-RGB, 1-8080
static bk_err_t uvc_cpu1_uvc_init(uint32_t frame_resolution, uint32_t fps)
{
	bk_err_t err = BK_OK;

	g_image_width = frame_resolution >> 16;
	g_image_height = frame_resolution & 0xFFFF;

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
				err = uvc_cpu1_uvc_init(msg.data, g_uvc_frame_rate);
				if (err != BK_OK) {
					os_printf("cpu1 init uvc failed\r\n");
					goto exit;
				}
				bk_uvc_register_frame_capture_callback(uvc_frame_capture_callback);
				break;
			}
			case UVC_CPU1_DISPLAY_INIT:
			{
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


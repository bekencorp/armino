#include <stdio.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include "bk_cli.h"
//#include <components/uvc_camera.h>
#include <driver/psram.h>
#include "uvc_mailbox.h"


#if (CONFIG_SDCARD_HOST)
#include "ff.h"
#include "test_fatfs.h"
#endif


#define UVC_DATA_ADDR    (0x60000000)
beken_thread_t  uvc_cpu0_demo_thread_handle = NULL;
beken_queue_t uvc_cpu0_demo_msg_que = NULL;


static void uvc_cpu0_cmd_help(void)
{}

static bk_err_t uvc_cpu0_send_msg(uint8_t type, uint32_t data)
{
	bk_err_t ret;
	uvc_cpu_msg_t msg;

	if (uvc_cpu0_demo_msg_que) {
		msg.type = type;
		msg.data = data;

		ret = rtos_push_to_queue(&uvc_cpu0_demo_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("uvc_send_msg failed, type:%d\r\n", type);
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static void uvc_save_image_to_sdcard(uint32_t frame_len)
{
#if (CONFIG_SDCARD_HOST)
	char *file_path = "uvc.jpg";
	char cFileName[FF_MAX_LFN];
	FIL fp1;
	unsigned int uiTemp = 0;
	uint32_t addr = UVC_DATA_ADDR;
	char *content = (char *)addr;

	sprintf(cFileName, "%d:/%d_%s", DISK_NUMBER_SDIO_SD, frame_len, file_path);

	FRESULT fr = f_open(&fp1, cFileName, FA_OPEN_APPEND | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("can not open file:%s!\n", cFileName);
		return;
	}

	fr = f_write(&fp1, content, frame_len, &uiTemp);
	if (fr != FR_OK) {
		os_printf("can not write file:%s!\n", cFileName);
		return;
	}
	os_printf("%s, length:%d\r\n", __func__, frame_len);

	fr = f_close(&fp1);
	if (fr != FR_OK) {
		os_printf("can not close file:%s!\n", cFileName);
		return;
	}
#endif

	// send mailbox to cpu1, inform data have been saved, cpu1 can save next frame
}


static void uvc_cp0_mailbox_tx_cmpl_isr(uvc_mb_t *uvc_mb, mb_chnl_ack_t *cmd_buf)
{
	//os_printf("enter cp0_mailbox_tx_cmpl \r\n");
	//os_printf("ack  hdr:0x%08x,data1:0x%08x,data2:0x%08x,state:0x%08x\r\n", cmd_buf->hdr.data, cmd_buf->ack_data1, cmd_buf->ack_data2, cmd_buf->ack_state);
}

static void uvc_cp0_mailbox_rx_isr(uvc_mb_t *uvc_mb, mb_chnl_cmd_t *cmd_buf)
{
	//os_printf("enter cp0_mailbox_rx_isr \r\n");
	//os_printf("cmd  hdr:0x%08x,pa1:0x%08x,pa2:0x%08x,pa3:0x%08x\r\n", cmd_buf->hdr.data, cmd_buf->param1, cmd_buf->param2, cmd_buf->param3);
	if (cmd_buf->hdr.cmd == UVC_MB_CAPTURE_BUSY) {
		uint32_t frame_len = cmd_buf->param1;
		uvc_cpu0_send_msg(UVC_CPU0_SAVE_BUSY, frame_len);
	} else {
		os_printf("cmd not find:%d\r\n", cmd_buf->hdr.cmd);
	}
}

static bk_err_t uvc_cpu0_inform_cpu1_req(uint8_t req_type)
{
	uvc_mailbox_msg_t msg;
	switch (req_type) {
	case UVC_MB_UVC_START:
		msg.mb_cmd = UVC_MB_UVC_START;
		break;
	case UVC_MB_UVC_STOP:
		msg.mb_cmd = UVC_MB_UVC_STOP;
		break;
	case UVC_MB_CAPTURE_START:
		msg.mb_cmd = UVC_MB_CAPTURE_START;
		break;
	case UVC_MB_CAPTURE_DONE:
		msg.mb_cmd = UVC_MB_CAPTURE_DONE;
	case UVC_MB_UVC_DEINIT:
		msg.mb_cmd = UVC_MB_UVC_DEINIT;
		break;
	default:
		os_printf("Not Suuport this uvc cmd\r\n");
		return BK_ERR_PARAM;
	}

	msg.param1 = 0;
	msg.param2 = 0;
	msg.param3 = 0;

	return uvc_mailbox_send_msg(&msg);
}

static void uvc_cpu0_process_main(void)
{
	bk_err_t err;

	while (1) {
		uvc_cpu_msg_t msg;
		err = rtos_pop_from_queue(&uvc_cpu0_demo_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == err) {
			switch (msg.type) {
			case UVC_CPU0_SAVE_BUSY:
				uvc_save_image_to_sdcard(msg.data);
				uvc_cpu0_inform_cpu1_req(UVC_MB_CAPTURE_DONE);
				break;
			case UVC_CPU0_DEINIT:
				goto exit;
			default:
				break;
			}
		}
	}

exit:
	uvc_mailbox_deinit();

	rtos_deinit_queue(&uvc_cpu0_demo_msg_que);
	uvc_cpu0_demo_msg_que = NULL;

	uvc_cpu0_demo_thread_handle = NULL;
	rtos_delete_thread(NULL);
	os_printf("cpu0 deinit finish\r\n");
}


static bk_err_t uvc_cpu0_init(void)
{
	bk_err_t ret;

	// step1: init cpu0 uvc mailbox channel, and callback
	uvc_mailbox_init(uvc_cp0_mailbox_rx_isr, NULL, uvc_cp0_mailbox_tx_cmpl_isr);

	if ((!uvc_cpu0_demo_thread_handle) && (!uvc_cpu0_demo_msg_que)) {

		ret = rtos_init_queue(&uvc_cpu0_demo_msg_que,
							  "uvc_queue",
							  sizeof(uvc_cpu_msg_t),
							  30);
		if (kNoErr != ret) {
			return kGeneralErr;
		}

		ret = rtos_create_thread(&uvc_cpu0_demo_thread_handle,
								 4,
								 "uvc_init",
								 (beken_thread_function_t)uvc_cpu0_process_main,
								 2 * 1024,
								 (beken_thread_arg_t)0);

		if (ret != kNoErr) {
			rtos_deinit_queue(&uvc_cpu0_demo_msg_que);
			uvc_cpu0_demo_msg_que = NULL;
			uvc_cpu0_demo_thread_handle = NULL;
			return kGeneralErr;
		}

		return kNoErr;
	} else
		return kInProgressErr;
}



void uvc_process_cpu0(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t err;
	if (os_strcmp(argv[1], "cpu0_init") == 0) {
		// step1: init psram
		uint32_t mode = 0x00054043;
		err = bk_psram_init(mode);
		if (err != BK_OK) {
			os_printf("psram init error\n");
			return;
		}

		err = uvc_cpu0_init();
		if (err != BK_OK) {
			os_printf("uvc cpu0 init error\n");
			return;
		}
	} else if (os_strcmp(argv[1], "init") == 0) {
		uint32_t width, height;
		uint32_t frame_resolution;
		uint32_t frame_rate;
		uvc_mailbox_msg_t msg;
		if (argc < 5) {
			os_printf("Input param error\r\n");
			uvc_cpu0_cmd_help();
			return;
		}

		width = os_strtoul(argv[2], NULL, 10);
		height = os_strtoul(argv[3], NULL, 10);
		frame_resolution = (width << 16) | height;
		frame_rate = os_strtoul(argv[4], NULL, 10);
		// step3: cpu0 inform cpu1 to init uvc, before this cpu1's mailbox channel have been built.
		msg.mb_cmd = UVC_MB_UVC_INIT;
		msg.param1 = frame_resolution;
		msg.param2 = frame_rate;
		msg.param3 = 0;
		err = uvc_mailbox_send_msg(&msg);
		if (err != BK_OK) {
			os_printf("cpu0 inform cpu1 init uvc failed\n");
			return;
		}
	} else if (os_strcmp(argv[1], "display_init") == 0) {
		uint32_t width, height;
		uint32_t frame_resolution;
		uint32_t frame_rate;
		uint32_t dis_clk_div;
		uvc_mailbox_msg_t msg;
		width = os_strtoul(argv[2], NULL, 10);
		height = os_strtoul(argv[3], NULL, 10);
		frame_resolution = (width << 16) | height;
		frame_rate = os_strtoul(argv[4], NULL, 10);
		dis_clk_div = os_strtoul(argv[5], NULL, 10);
		// step3: cpu0 inform cpu1 to init uvc, before this cpu1's mailbox channel have been built.
		msg.mb_cmd = UVC_MB_DISPLAY_UVC_INIT;
		msg.param1 = frame_resolution;
		msg.param2 = frame_rate;
		msg.param3 = dis_clk_div;
		err = uvc_mailbox_send_msg(&msg);
		if (err != BK_OK) {
			os_printf("cpu0 inform cpu1 init uvc failed\n");
			return;
		}
	} else if (os_strcmp(argv[1], "start") == 0) {
		err = uvc_cpu0_inform_cpu1_req(UVC_MB_UVC_START);
		if (err != BK_OK) {
			os_printf("cpu0 inform cpu1 start uvc failed\n");
			return;
		}
	} else if (os_strcmp(argv[1], "stop") == 0) {
		err = uvc_cpu0_inform_cpu1_req(UVC_MB_UVC_STOP);
		if (err != BK_OK) {
			os_printf("cpu0 inform cpu1 stop uvc failed\n");
			return;
		}
	} else if (os_strcmp(argv[1], "capture") == 0) {
		err = uvc_cpu0_inform_cpu1_req(UVC_MB_CAPTURE_START);
		if (err != BK_OK) {
			os_printf("cpu0 inform cpu1 deinit uvc failed\n");
			return;
		}
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		err = uvc_cpu0_inform_cpu1_req(UVC_MB_UVC_DEINIT);
		if (err != BK_OK) {
			os_printf("cpu0 inform cpu1 deinit uvc failed\n");
			return;
		}

		uvc_cpu0_send_msg(UVC_CPU0_DEINIT, 0);
	} else if (os_strcmp(argv[1], "cpu1_deinit") == 0) {
		err = bk_psram_deinit();
		if (err != BK_OK) {
			os_printf("uvc deinit psram error!\r\n");
			return;
		}
	}
}



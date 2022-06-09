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

#include <common/bk_include.h>
#include <os/os.h>
#include <stdio.h>
#include <components/uvc_camera.h>
#include <components/uvc_camera_types.h>
#include <os/mem.h>
#include "bk_usb.h"
#include "bk_drv_model.h"
#include <driver/dma.h>
#if CONFIG_GENERAL_DMA
#include "bk_general_dma.h"
#endif

#if CONFIG_PSRAM
#include <driver/psram.h>
#endif

extern void delay(INT32 num);

#define UVC_DATA_ADDR               (0x30060000)
#define UVC_DATA_PSRAM              (0x60000000)
#define USB_UVC_FIFO_ADDR           (0x46002028)
#define UVC_DATA_LENGTH             512
#define USB_UVC_HEAD_LEN            12

uvc_desc_t uvc_intf;
static uint8_t s_uvc_save = 1;
static uint8_t g_file_id = 0;
static uint8_t g_uvc_start = 0;

beken_thread_t  uvc_thread_handle = NULL;
beken_queue_t uvc_msg_que = NULL;

static bk_err_t uvc_send_msg(uint8_t type, uint32_t data)
{
	bk_err_t ret;
	uvc_msg_t msg;

	if (uvc_msg_que) {
		msg.type = type;
		msg.data = data;

		ret = rtos_push_to_queue(&uvc_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("uvc_send_msg failed, type:%d\r\n", type);
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static void uvc_process_data_packet(void *curptr, uint32_t newlen, uint8_t is_eof, uint32_t frame_len)
{
	uint8_t *data;
	uint8_t bmhead_info;

	bmhead_info = *((uint8_t *)curptr + 1);
	data = (uint8_t *)curptr + USB_UVC_HEAD_LEN;

	if (bmhead_info & 0x40) {// bit6 = 1, payload error
		return;
	}

	if (bmhead_info & 0x02) { // bit1 = 1, end frame
		/*uint8_t *eof;
		eof = (uint8_t *)curptr + newlen - 2;
		os_printf("%s, %02x, %02x\r\n", __func__, eof[0], eof[1]);*/
		uvc_intf.frame_id += 1;
		if (uvc_intf.frame_id == 0xFFFFFFFE)
			uvc_intf.frame_id = 0;
	} else {
			if ((data[0] == 0xff) && (data[1] == 0xd8)) { // strat frame
				os_printf("uvc start, %02x, %02x\r\n", data[0], data[1]);
				uvc_intf.frame_len = 0;
				if (uvc_intf.mem_status == UVC_MEM_IDLE) {
					s_uvc_save = 1;
				} else {
					s_uvc_save = 0;
				}
			}
	}

	if (s_uvc_save == 1) {
//#if CONFIG_GENERAL_DMA
//		dma_memcpy((void *)(UVC_DATA_PSRAM + uvc_intf.frame_len), data, frame_len);
//#else
		os_memcpy((void *)(UVC_DATA_PSRAM + uvc_intf.frame_len), data, frame_len);
//#endif
		uvc_intf.frame_len += frame_len;
	} else {
		uvc_intf.frame_len += frame_len;//not update frame length when not save uvc data
	}

	if ((s_uvc_save == 1) && (bmhead_info & 0x02)) {
		//os_printf("frame_len1:%d\r\n", uvc_intf.frame_len);
		if ((uvc_intf.end_frame_handler != NULL) && (uvc_intf.frame_len > 0)) {
			uvc_intf.mem_status = UVC_MEM_BUSY;
			//s_uvc_save = 0;
			uvc_send_msg(UVC_EOF, uvc_intf.frame_len);
		}

		if (uvc_intf.capture_enable == 1) {
			uvc_intf.capture_enable = 0;
			uvc_intf.mem_status = UVC_MEM_BUSY;
			//s_uvc_save = 0;
			uvc_send_msg(UVC_CAPTURE, uvc_intf.frame_len);
		}
	}
}


static void uvc_dma_rx_done_handler(dma_id_t dma)
{
	uint32_t already_len = uvc_intf.rx_read_len;
	uint32_t copy_len = uvc_intf.node_len;
	uint32_t frame_len = uvc_intf.node_len - USB_UVC_HEAD_LEN;
	GLOBAL_INT_DECLARATION();
	if (uvc_intf.node_full_handler != NULL)
		uvc_intf.node_full_handler(uvc_intf.rxbuf + already_len, copy_len, 0, frame_len);

	already_len += copy_len;

	if (already_len >= uvc_intf.rxbuf_len)
		already_len = 0;

	GLOBAL_INT_DISABLE();
	uvc_intf.rx_read_len = already_len;
	GLOBAL_INT_RESTORE();
}

static void uvc_intfer_config_desc()
{
	os_memset(&uvc_intf, 0, sizeof(uvc_desc_t));
	uvc_intf.rxbuf = os_malloc(sizeof(uint8_t) * UVC_DATA_LENGTH * 4);
	if (uvc_intf.rxbuf == NULL) {
		os_printf("malloc failed\r\n");
		BK_ASSERT(1);
	}
	uvc_intf.mem_status = UVC_MEM_IDLE;
	uvc_intf.capture_enable = 0;
	uvc_intf.rxbuf_len = UVC_DATA_LENGTH * 4;
	uvc_intf.node_len = 0;
	uvc_intf.rx_read_len = 0;
	uvc_intf.frame_id = 0;
	uvc_intf.frame_len = 0;
	uvc_intf.node_full_handler = uvc_process_data_packet;
	uvc_intf.end_frame_handler = NULL;
	uvc_intf.frame_capture_handler = NULL;

#if CONFIG_GENERAL_DMA
	uvc_intf.dma_rx_handler = uvc_dma_rx_done_handler;
	uvc_intf.dma_channel = bk_dma_alloc(DMA_DEV_DTCM);
	if ((uvc_intf.dma_channel < DMA_ID_0) || (uvc_intf.dma_channel >= DMA_ID_MAX)) {
		os_printf("malloc dma fail \r\n");
		return;
	}

	os_printf("%s, uvc_dma_id:%d\r\n", __func__, uvc_intf.dma_channel);
#endif
}

static void uvc_dma_config(void)
{
#if CONFIG_GENERAL_DMA
	dma_config_t dma_config = {0};
	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;//DMA_DEV_USB;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = USB_UVC_FIFO_ADDR;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.start_addr = (uint32_t)&uvc_intf.rxbuf[0];
	dma_config.dst.end_addr = (uint32_t)(&uvc_intf.rxbuf[0] + uvc_intf.rxbuf_len);

	BK_LOG_ON_ERR(bk_dma_init(uvc_intf.dma_channel, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(uvc_intf.dma_channel, uvc_intf.node_len));
	BK_LOG_ON_ERR(bk_dma_register_isr(uvc_intf.dma_channel, NULL, uvc_intf.dma_rx_handler));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(uvc_intf.dma_channel));
	BK_LOG_ON_ERR(bk_dma_enable_half_finish_interrupt(uvc_intf.dma_channel));
#endif
}

static void uvc_notify_uvc_configed_callback(void)
{
}

static void uvc_fiddle_rx_vs_callback(void)
{
	if (g_uvc_start)
		bk_uvc_receive_video_stream();
	else
		bk_uvc_stop();
}

static void uvc_get_packet_rx_vs_callback(uint8_t *arg, uint32_t count)
{
	//if (uvc_intf.mem_status == UVC_MEM_IDLE) {
		uint32_t start_addr = (uint32_t)arg;
		uint32_t left_len = 0;
		GLOBAL_INT_DECLARATION();
		bk_dma_set_src_addr(uvc_intf.dma_channel, start_addr, 0);

		GLOBAL_INT_DISABLE();
		uvc_intf.node_len = count;
		GLOBAL_INT_RESTORE();
		left_len = uvc_intf.rxbuf_len - uvc_intf.rx_read_len;
		if (left_len < uvc_intf.node_len) {
			GLOBAL_INT_DISABLE();
			uvc_intf.rx_read_len = 0;
			GLOBAL_INT_RESTORE();
		}

		uint32_t dest_start_addr = (uint32_t)&uvc_intf.rxbuf[0] + uvc_intf.rx_read_len;
		bk_dma_set_dest_addr(uvc_intf.dma_channel, dest_start_addr, 0);
		bk_dma_set_transfer_len(uvc_intf.dma_channel, uvc_intf.node_len);
		bk_dma_start(uvc_intf.dma_channel);
	//} else
	//		return;
}

static void uvc_capture_frame(uint32_t data)
{
	if (uvc_intf.frame_capture_handler != NULL) {
		uvc_intf.frame_capture_handler(data);
	} else {
		uvc_intf.mem_status = UVC_MEM_IDLE;
	}
}

static bk_err_t uvc_camera_init(void)
{
	uint32_t param;
	void *parameter;
	bk_err_t err = BK_OK;

	// step 1: init psram, before call this api

	// step 2: init isr_func
	uvc_intfer_config_desc();

	// step 3: init dma
	uvc_dma_config();

	parameter = (void *)uvc_notify_uvc_configed_callback;
	err = bk_uvc_register_config_callback(parameter);
	if (err != BK_OK) {
		os_printf("register uvc config callback error!\r\n");
		goto init_error;
	}

	parameter = (void *)uvc_fiddle_rx_vs_callback;
	err = bk_uvc_register_VSrxed_callback(parameter);
	if (err != BK_OK) {
		os_printf("register uvc rx video stream callback error!\r\n");
		goto init_error;
	}

	parameter = (void *)uvc_get_packet_rx_vs_callback;
	err = bk_uvc_register_VSrxed_packet_callback(parameter);
	if (err != BK_OK) {
		os_printf("register uvc rx every packet callback error!\r\n");
		goto init_error;
	}

	parameter = (void *)UVC_DATA_ADDR;
	err = bk_uvc_register_rx_vstream_buffptr(parameter);
	if (err != BK_OK) {
		os_printf("uvc set rx video stream buf addr error!\r\n");
		goto init_error;
	}

	param = UVC_DATA_LENGTH;
	err = bk_uvc_register_rx_vstream_bufflen(param);
	if (err != BK_OK) {
		os_printf("uvc set rx video stream buf length error!\r\n");
		goto init_error;
	}

	bk_uvc_register_link(0);

	err = bk_uvc_set_parameter(UVC_FRAME_640_480, FPS_30);
	if (err != BK_OK) {
		os_printf("uvc set fps and ppi error!\r\n");
		//goto init_error;
	}

#ifdef UVC_DEMO_SUPPORT100
	bk_uvc_set_parameter(U1_FRAME_640_480, FPS_30);
	if (err != BK_OK) {
		os_printf("uvc set fps and ppi error!\r\n");
		goto init_error;
	}

#endif

	if (err != BK_OK) {
		goto init_error;
	}

	return err;

init_error:
	return err;
}

static bk_err_t uvc_camera_deinit(void)
{
	// uvc deinit
	bk_err_t status = 0;
	g_uvc_start = 0;
	delay(100);

	bk_dma_stop(uvc_intf.dma_channel);
	status = bk_dma_deinit(uvc_intf.dma_channel);
	if (status != BK_OK) {
		os_printf("uvc deinit dma error!\r\n");
		//status = kOptionErr;
		//return status;
	}

	status = bk_dma_free(DMA_DEV_DTCM, uvc_intf.dma_channel);
	if (status != BK_OK) {
		os_printf("uvc free dma error!\r\n");
		//status = kOptionErr;
		//return status;
	}

	if (uvc_intf.rxbuf) {
		os_free(uvc_intf.rxbuf);
		uvc_intf.rxbuf = NULL;
	}
	os_memset(&uvc_intf, 0, sizeof(uvc_desc_t));

	return status;
}

static void uvc_set_ppi_fps(uint32_t data)
{
	uint32_t resolution_id;
	uint32_t fps;
	bk_err_t status;
	resolution_id = data >> 16;
	fps = data & 0xFFFF;
	status = bk_uvc_set_parameter(resolution_id, fps);
	if (status != kNoErr) {
		os_printf("Set uvc param0 error!\r\n");
		status = kOptionErr;
	}

#ifdef UVC_DEMO_SUPPORT100
	status = bk_uvc_set_parameter(U1_FRAME_640_480, FPS_30);
	if (status != kNoErr) {
		os_printf("Set uvc param1 error!\r\n");
		status = kOptionErr;
	}
#endif

	if (status != kNoErr) {
		uvc_send_msg(UVC_EXIT, 0);
	}
}

static void uvc_set_start(uint32_t data)
{
	bk_err_t status = BK_OK;
	uvc_intf.mem_status = UVC_MEM_IDLE;
	g_uvc_start = 1;
	status = bk_uvc_start();
	if (status != BK_OK) {
		os_printf("start uvc error!\r\n");
		uvc_send_msg(UVC_EXIT, 0);
	}
}

static void uvc_set_stop(uint32_t data)
{ 
	uvc_intf.mem_status = UVC_MEM_BUSY;
	g_uvc_start = 0;
	uvc_intf.rx_read_len = 0;
}

static void uvc_process_eof(uint32_t data)
{
	if (uvc_intf.end_frame_handler)
		uvc_intf.end_frame_handler(data);
	else
		uvc_intf.mem_status = UVC_MEM_IDLE;
}

static void uvc_process_main(void)
{
	bk_err_t err;

	while (1) {
		uvc_msg_t msg;
		err = rtos_pop_from_queue(&uvc_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == err) {
			switch (msg.type) {
			case UVC_SET_PARAM:
				uvc_set_ppi_fps(msg.data);
				break;
			case UVC_START:
				uvc_set_start(msg.data);
				break;
			case UVC_STOP:
				uvc_set_stop(msg.data);
				break;
			case UVC_EOF:
				uvc_process_eof(msg.data);
				break;
			case UVC_CAPTURE:
				uvc_capture_frame(msg.data);
				break;
			case UVC_EXIT:
				goto uvc_exit;
				break;
			default:
				break;
			}
		}
	}

uvc_exit:

	uvc_camera_deinit();

	rtos_deinit_queue(&uvc_msg_que);
	uvc_msg_que = NULL;

	uvc_thread_handle = NULL;
	rtos_delete_thread(NULL);
}

bk_err_t bk_uvc_save_frame(uint8_t file_id)
{
	g_file_id = file_id;
	uvc_intf.capture_enable = 1;
	return BK_OK;
}

bk_err_t bk_uvc_set_mem_status(uint8_t type)
{
	uvc_intf.mem_status = type;

	return BK_OK;
}

bk_err_t bk_uvc_register_frame_end_callback(void *cb)
{
	uvc_intf.end_frame_handler = cb;

	return BK_OK;
}

bk_err_t bk_uvc_register_frame_capture_callback(void *cb)
{
	uvc_intf.frame_capture_handler = cb;

	return BK_OK;
}

bk_err_t bk_uvc_set_ppi_fps(uint16_t ppi, uint8_t fps)
{
	int status = kNoErr;
	uint32_t param = 0;

	switch (ppi) {
		case 120:
			ppi = UVC_FRAME_160_120;
			break;
		case 144:
			ppi = UVC_FRAME_176_144;
			break;
		case 240:
			ppi = UVC_FRAME_320_240;
			break;
		case 288:
			ppi = UVC_FRAME_352_288;
			break;
		case 360:
			ppi = UVC_FRAME_640_360;
			break;
		case 480:
			ppi = UVC_FRAME_640_480;
			break;
		case 540:
			ppi = UVC_FRAME_960_540;
			break;
		case 600:
			ppi = UVC_FRAME_800_600;
			break;
		case 720:
			ppi = UVC_FRAME_1280_720;
			break;
		default:
			os_printf("Set PPI unknow: %d\r\n", ppi);
			status = kParamErr;
			return status;
	}

	if (fps < FPS_5 || fps > FPS_30) {
		os_printf("Set FPS unknow: %d\r\n", fps);
		status = kParamErr;
		return status;
	}

	param |= (ppi << 16);//bit[31-16]:ppi
	param |= fps; // bit[15-0]:fps

	status = uvc_send_msg(UVC_SET_PARAM, param);

	return status;
}

bk_err_t bk_uvc_set_start(void)
{
	uvc_send_msg(UVC_START, 0);

	return BK_OK;
}

bk_err_t bk_uvc_set_stop(void)
{
	uvc_send_msg(UVC_STOP, 0);

	return BK_OK;
}

bk_err_t bk_uvc_init(void)
{
	int ret;

	if (uvc_camera_init() != BK_OK) {
		os_printf("uvc init failed\r\n");
		return kInProgressErr;
	}

	if ((!uvc_thread_handle) && (!uvc_msg_que)) {

		ret = rtos_init_queue(&uvc_msg_que,
							  "uvc_queue",
							  sizeof(uvc_msg_t),
							  30);
		if (kNoErr != ret) {
			return kGeneralErr;
		}

		ret = rtos_create_thread(&uvc_thread_handle,
								 4,
								 "uvc_init",
								 (beken_thread_function_t)uvc_process_main,
								 2 * 1024,
								 (beken_thread_arg_t)0);

		if (ret != kNoErr) {
			rtos_deinit_queue(&uvc_msg_que);
			uvc_msg_que = NULL;
			uvc_thread_handle = NULL;
			return kGeneralErr;
		}

		return kNoErr;
	} else
		return kInProgressErr;
}

bk_err_t bk_uvc_deinit(void)
{
	if (!uvc_thread_handle)
		return BK_OK;
	uvc_send_msg(UVC_EXIT, 0);

	while (uvc_thread_handle)
			rtos_delay_milliseconds(10);

	return BK_OK;
}


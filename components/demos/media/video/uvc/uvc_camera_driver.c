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
#include <stdio.h>
#if CONFIG_USB_UVC
#include <components/uvc_camera.h>
#include "uvc_camera_types.h"
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

#if (CONFIG_SDCARD_HOST || CONFIG_USB_HOST)
#include "ff.h"
#include "test_fatfs.h"
#endif

extern void delay(INT32 num);

#define UVCDMA_CHANNEL              DMA_ID_4
#define UVC_DATA_ADDR               (0x30060000)
#define UVC_DATA_PSRAM              (0x60000000)
#define USB_UVC_FIFO_ADDR           (0x46002028)
#define UVC_DATA_LENGTH             512
#define USB_UVC_HEAD_LEN            12

DD_HANDLE uvc_hdl = DD_HANDLE_UNVALID;
uvc_desc_t uvc_intf;
static uint8_t s_uvc_save = 1;
static uint8_t uvc_data[512 * 4];
static uint32_t frame_total_len = 0;
/// the video data receive complete
static beken_semaphore_t aready_semaphore = NULL;
static uint8_t s_uvc_start = 0;

void uvc_process_data_packet(void *curptr, uint32_t newlen, uint8_t is_eof, uint32_t frame_len)
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
	} else {
			if ((data[0] == 0xff) && (data[1] == 0xd8)) { // strat frame
				uvc_intf.frame_len = 0;
			}
	}

	if (s_uvc_save == 1) {
#if CONFIG_GENERAL_DMA
		dma_memcpy((void *)(UVC_DATA_PSRAM + uvc_intf.frame_len), data, frame_len);
#else
		os_memcpy((void *)(UVC_DATA_PSRAM + uvc_intf.frame_len), data, frame_len);
#endif
		uvc_intf.frame_len += frame_len;
		frame_total_len = uvc_intf.frame_len;

		if (bmhead_info & 0x02) {
			if (uvc_intf.end_frame_handler && uvc_intf.node_len > 0)
				uvc_intf.end_frame_handler(uvc_intf.frame_len);
		}
	} else {
		uvc_intf.frame_len += frame_len;
	}

	if ((aready_semaphore != NULL) && (bmhead_info & 0x02)) {
		rtos_set_semaphore(&aready_semaphore);
		s_uvc_save = 0;
	}
}


#if CONFIG_GENERAL_DMA
static void uvc_intf_node_rx_handler(dma_id_t dma)
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
#endif

static void uvc_intf_frame_end_handler(uint32_t frame_len)
{
#if 0
#if CONFIG_GENERAL_DMA
	uint16_t already_len = uvc_intf.rx_read_len;
	uint32_t channel = uvc_intf.dma_channel;
	int left_len = bk_dma_get_remain_len(channel);
	int rec_len = uvc_intf.node_len - left_len;

	if ((uvc_intf.node_full_handler != NULL) && (rec_len > 0))
		uvc_intf.node_full_handler(uvc_intf.rxbuf + already_len, rec_len, 1, 0);

	already_len += rec_len;
	if (already_len >= uvc_intf.rxbuf_len)
		already_len -= uvc_intf.rxbuf_len;

	uvc_intf.rx_read_len = already_len;

	// turn off dma, so dma can start from first configure. for easy handler
	bk_dma_stop(channel);

	uvc_intf.rx_read_len = 0;
	bk_dma_start(channel);
#endif

	if ((uvc_intf.data_end_handler))
		uvc_intf.data_end_handler();
#endif
}

static void uvc_intfer_config_desc()
{
	os_memset(&uvc_intf, 0, sizeof(uvc_desc_t));
	uvc_intf.rxbuf = &uvc_data[0];
	uvc_intf.rxbuf_len = UVC_DATA_LENGTH * 4;
	uvc_intf.node_len = 0;
	uvc_intf.rx_read_len = 0;
	uvc_intf.frame_id = 0;
	uvc_intf.frame_len = 0;
	uvc_intf.node_full_handler = uvc_process_data_packet;
	uvc_intf.end_frame_handler = uvc_intf_frame_end_handler;

#if CONFIG_GENERAL_DMA
	uvc_intf.dma_rx_handler = uvc_intf_node_rx_handler;
	uvc_intf.dma_channel = UVCDMA_CHANNEL;
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

void fuvc_notify_uvc_configed_callback(void)
{
	//process_post(&fuvc_test, PROCESS_EVENT_MSG, NULL);
}

void fuvc_fiddle_rx_vs_callback(void)
{
	ddev_control(uvc_hdl, UCMD_UVC_RECEIVE_VSTREAM, 0);
}

void fuvc_get_packet_rx_vs_callback(uint8_t *arg, uint32_t count)
{
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
}

bk_err_t bk_uvc_register_frame_end_callback(void *cb)
{
	if (uvc_hdl == DD_HANDLE_UNVALID)
		return BK_ERR_NOT_INIT;
	uvc_intf.end_frame_handler = cb;

	return BK_OK;
}

bk_err_t bk_uvc_set_ppi_fps(uint16_t ppi, uint8_t fps)
{
	int status = kNoErr;
	uint32_t param;
	switch (ppi) {
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
		/*case 272:
			ppi = UVC_FRAME_480_272;
			break;*/
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

	param = UVC_MUX_PARAM(ppi, fps);
	status = ddev_control(uvc_hdl, UCMD_UVC_SET_PARAM, &param);
	if (status != kNoErr) {
		os_printf("Set uvc param error!\r\n");
		status = kOptionErr;
	}
	return status;
}

bk_err_t bk_uvc_set_start(void)
{
	int status = kNoErr;
	s_uvc_start = 1;
	status = ddev_control(uvc_hdl, UCMD_UVC_START_STREAM, NULL);
	if (status != 1) {
		os_printf("start uvc error!\r\n");
		status = kOptionErr;
	} else
		status = kNoErr;

	return status;
}

bk_err_t bk_uvc_set_stop(void)
{
	int status = kNoErr;
	status = ddev_control(uvc_hdl, UCMD_UVC_STOP_STREAM, NULL);
	if (status != 1) {
		os_printf("stop uvc error!\r\n");
		status = kOptionErr;
	} else
		status = kNoErr;

	s_uvc_start = 0;
	return status;
}

bk_err_t bk_uvc_init(void)
{
	uint32_t param;
	UINT32 status;
	uint32_t mode = 0x00054043;
	void *parameter;
	int err = kNoErr;

	// step 1: init psram
	err = bk_psram_init(mode);
	if (err != kNoErr) {
		os_printf("psram init error\n");
		err = -1;
		goto init_error;
	}

	// step 2: init isr_func
	uvc_intfer_config_desc();

	// step 3: init dma
	uvc_dma_config();

	// step 4: init uvc
	uvc_hdl = ddev_open(DD_DEV_TYPE_USB, &status, 0);
	if (DD_HANDLE_UNVALID == uvc_hdl)
		goto init_error;

	parameter = (void *)fuvc_notify_uvc_configed_callback;
	err = ddev_control(uvc_hdl, UCMD_UVC_REGISTER_CONFIG_NOTIFY_CB, parameter);
	if (err != kNoErr) {
		os_printf("register uvc config callback error!\r\n");
		err = kOptionErr;
		goto init_error;
	}

	parameter = (void *)fuvc_fiddle_rx_vs_callback;
	err = ddev_control(uvc_hdl, UCMD_UVC_REGISTER_RX_VSTREAM_CB, parameter);
	if (err != kNoErr) {
		os_printf("register uvc rx video stream callback error!\r\n");
		err = kOptionErr;
		goto init_error;
	}

	parameter = (void *)UVC_DATA_ADDR;
	err = ddev_control(uvc_hdl, UCMD_UVC_REGISTER_RX_VSTREAM_BUF_PTR, parameter);
	if (err != kNoErr) {
		os_printf("uvc set rx video stream buf addr error!\r\n");
		err = kOptionErr;
		goto init_error;
	}

	param = UVC_DATA_LENGTH;
	err = ddev_control(uvc_hdl, UCMD_UVC_REGISTER_RX_VSTREAM_BUF_LEN, &param);
	if (err != kNoErr) {
		os_printf("uvc set rx video stream buf length error!\r\n");
		err = kOptionErr;
		goto init_error;
	}

	parameter = (void *)fuvc_get_packet_rx_vs_callback;
	err = ddev_control(uvc_hdl, UCMD_UVC_REGISTER_RX_PACKET_CB, parameter);
	if (err != kNoErr) {
		os_printf("register uvc rx every packet callback error!\r\n");
		err = kOptionErr;
		goto init_error;
	}
/*
	param = LinkType;
	ddev_control(usb_hdl, UCMD_UVC_REGISTER_LINK, &param);*/

#ifdef UVC_DEMO_SUPPORT100
	param = UVC_MUX_PARAM(U1_FRAME_640_480, FPS_30);
	err = ddev_control(uvc_hdl, UCMD_UVC_SET_PARAM, &param);
	if (err != kNoErr) {
		os_printf("uvc set ppi and fps error!\r\n");
		err = kOptionErr;
		goto init_error;
	}
#endif

	return err;

init_error:
	return err;
}

bk_err_t bk_uvc_deinit(void)
{
	// uvc deinit
	int status = 0;
	status = ddev_close(uvc_hdl);
	if (status != kNoErr) {
		os_printf("uvc close error!\r\n");
		status = kOptionErr;
		return status;
	}

	delay(2000);
	bk_psram_deinit();
	if (status != kNoErr) {
		os_printf("uvc deinit psram error!\r\n");
		status = kOptionErr;
		return status;
	}

	os_memset(&uvc_intf, 0, sizeof(uvc_desc_t));
	os_memset(uvc_data, 0, UVC_DATA_LENGTH * 4);
	uvc_hdl = DD_HANDLE_UNVALID;
	return status;
}

static int uvc_save_to_sdcard(uint8_t file_id)
{
#if (CONFIG_SDCARD_HOST || CONFIG_USB_HOST)
	char *file_path = "bk7256_camera.txt";
	char cFileName[FF_MAX_LFN];
	FIL fp1;
	unsigned int uiTemp = 0;
	uint32_t addr = UVC_DATA_PSRAM;
	char *content = (char *)addr;

	sprintf(cFileName, "%d:/%d_%s", DISK_NUMBER_SDIO_SD, file_id, file_path);

	FRESULT fr = f_open(&fp1, cFileName, FA_OPEN_APPEND | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("can not open file:%s!\n", cFileName);
		return fr;
	}

	os_printf("%s, len:%d\r\n", __func__, frame_total_len);
	if (frame_total_len > 0)
		fr = f_write(&fp1, content, frame_total_len, &uiTemp);
	else {
		os_printf("frame length error!\n");
		return -1;
	}

	fr = f_close(&fp1);
	if (fr != FR_OK) {
		os_printf("can not close file:%s!\n", cFileName);
		return fr;
	}
#endif
	return 0;
}

bk_err_t bk_uvc_save_frame(uint8_t file_id)
{
	int ret = rtos_init_semaphore(&aready_semaphore, 1);
	if (ret != kNoErr) {
		os_printf("init semaph failed\r\n");
		return ret;
	}

	ret = rtos_get_semaphore(&aready_semaphore, 8000);
	if (ret != kNoErr) {
		os_printf("get semaph failed\r\n");
		if (aready_semaphore != NULL)
			rtos_deinit_semaphore(&aready_semaphore);
		return ret;
	}

	uvc_save_to_sdcard(file_id);

	if (aready_semaphore != NULL)
		rtos_deinit_semaphore(&aready_semaphore);

	s_uvc_save = 1;
	return ret;
}

#endif //CONFIG_USB_UVC


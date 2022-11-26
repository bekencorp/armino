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
#include <driver/media_types.h>
#include <driver/uvc_camera.h>
#include <os/mem.h>
#include "bk_usb.h"
#include <modules/usb.h>
#include <common/bk_err.h>
#include <driver/dma.h>
#include "bk_general_dma.h"

#include <driver/psram.h>
#include <driver/gpio.h>
#include "gpio_driver.h"
#include "bk_misc.h"
#include <modules/pm.h>

#define TAG "uvc_drv"

#define UVC_LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define UVC_LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define UVC_LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define UVC_LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define USB_UVC_FIFO_ADDR           (0x46002024)
#define UVC_DATA_LENGTH             512
#define USB_UVC_HEAD_LEN            12

//#define UVC_DIAG_DEBUG

#ifdef UVC_DIAG_DEBUG

#define UVC_DIAG_DEBUG_INIT()                    \
	do {                                         \
		gpio_dev_unmap(GPIO_32);                 \
		bk_gpio_disable_pull(GPIO_32);           \
		bk_gpio_enable_output(GPIO_32);          \
		bk_gpio_set_output_low(GPIO_32);         \
		\
		gpio_dev_unmap(GPIO_33);                 \
		bk_gpio_disable_pull(GPIO_33);           \
		bk_gpio_enable_output(GPIO_33);          \
		bk_gpio_set_output_low(GPIO_33);         \
		\
		gpio_dev_unmap(GPIO_34);                 \
		bk_gpio_disable_pull(GPIO_34);           \
		bk_gpio_enable_output(GPIO_34);          \
		bk_gpio_set_output_low(GPIO_34);         \
		\
		gpio_dev_unmap(GPIO_35);                 \
		bk_gpio_disable_pull(GPIO_35);           \
		bk_gpio_enable_output(GPIO_35);          \
		bk_gpio_set_output_low(GPIO_35);         \
		\
	} while (0)

#define UVC_USB_ISR_ENTRY()                 bk_gpio_set_output_high(GPIO_32)
#define UVC_USB_ISR_OUT()                   bk_gpio_set_output_low(GPIO_32)

#define UVC_SRAM_DMA_ENTRY()                bk_gpio_set_output_high(GPIO_33)
#define UVC_SRAM_DMA_OUT()                  bk_gpio_set_output_low(GPIO_33)

#define UVC_PSRAM_DMA_ENTRY()               bk_gpio_set_output_high(GPIO_34)
#define UVC_PSRAM_DMA_OUT()                 bk_gpio_set_output_low(GPIO_34)

#define UVC_JPEG_EOF_ENTRY()                bk_gpio_set_output_high(GPIO_35)
#define UVC_JPEG_EOF_OUT()                  bk_gpio_set_output_low(GPIO_35)

#else

#define UVC_DIAG_DEBUG_INIT()

#define UVC_USB_ISR_ENTRY()
#define UVC_USB_ISR_OUT()

#define UVC_SRAM_DMA_ENTRY()
#define UVC_SRAM_DMA_OUT()

#define UVC_PSRAM_DMA_ENTRY()
#define UVC_PSRAM_DMA_OUT()

#define UVC_JPEG_EOF_ENTRY()
#define UVC_JPEG_EOF_OUT()


#endif


//#define UVC_STRIP

extern void delay(int num);

void uvc_camera_memcpy_finish_callback(dma_id_t id);


beken_thread_t uvc_thread_drv_handle = NULL;
beken_queue_t uvc_drv_msg_que = NULL;
static const uvc_camera_config_t *uvc_camera_config = NULL;
static frame_buffer_t *curr_frame_buffer = NULL;
static uint8_t *uvc_rx_vstream_buffptr = NULL;
uint32_t uvc_frame_id = 0;
static uint8_t uvc_drop_frame = 0;

extern media_debug_t *media_debug;

#define FRAME_BUFFER_UVC         (1024 * 5)

typedef struct
{
	uint8_t  frame_flag;
	uint8_t  sof;
	uint8_t  eof;
	uint8_t  uvc_dma;
	uint8_t  psram_dma;
	uint32_t rxbuf_len;
	uint32_t rx_read_len;
	uint32_t uvc_transfer_len;
	uint8_t *buffer;
	frame_buffer_t *frame;
	void (*node_full_handler)(void *curptr, uint32_t newlen, uint8_t is_eof, uint32_t frame_len);
} uvc_camera_drv_t;

uvc_camera_drv_t *uvc_camera_drv = NULL;

static bk_err_t uvc_send_msg(uint8_t type, uint32_t data)
{
	bk_err_t ret;
	uvc_msg_t msg;

	if (uvc_drv_msg_que)
	{
		msg.type = type;
		msg.data = data;

		ret = rtos_push_to_queue(&uvc_drv_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
		{
			UVC_LOGE("uvc_send_msg failed, type:%d\r\n", type);
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static bk_err_t uvc_memcpy_by_chnl(void *out, const void *in, uint32_t len, dma_id_t cpy_chnls)
{
	dma_config_t dma_config = {0};

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;

	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.src.start_addr = (uint32_t)in;
	dma_config.src.end_addr = (uint32_t)(in + len);

	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.start_addr = (uint32_t)out;
	dma_config.dst.end_addr = (uint32_t)(out + len);

	BK_LOG_ON_ERR(bk_dma_init(cpy_chnls, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(cpy_chnls, len));

	BK_LOG_ON_ERR(bk_dma_register_isr(cpy_chnls, NULL, uvc_camera_memcpy_finish_callback));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(cpy_chnls));
	BK_LOG_ON_ERR(bk_dma_start(cpy_chnls));

	return BK_OK;
}

#ifdef DVP_STRIP
static void uvc_frame_strip(frame_buffer_t *frame)
{
	uint32_t i, strip = 0, strip_max = 50;

	for (i = frame->length - 1; i > 0 && strip_max > 0; i--, strip_max--)
	{
		if (frame->frame[i] == 0xD9
		    && frame->frame[i - 1] == 0xFF)
		{
			break;
		}
		else
		{
			strip--;
		}
	}

	if (strip_max)
	{
		frame->length -= strip;
	}
	else
	{
		UVC_LOGE("uvc_frame_strip error\n");
	}
}
#endif

static void uvc_process_data_packet(void *curptr, uint32_t newlen, uint8_t is_eof, uint32_t frame_len)
{
	uint8_t *data;
	uint8_t bmhead_info;
	uint32_t fack_len = frame_len;

	UVC_PSRAM_DMA_ENTRY();

	if (curr_frame_buffer == NULL
	    || curr_frame_buffer->frame == NULL)
	{
		UVC_LOGE("%s curr_frame_buffer NULL\n");
		return;
	}

	bmhead_info = *((uint8_t *)curptr + 1);
	data = (uint8_t *)curptr + USB_UVC_HEAD_LEN;

	if (bmhead_info & 0x40)  // bit6 = 1, payload error
	{
		return;
	}

	if (frame_len & 0x3)
	{
		fack_len = ((frame_len >> 2) + 1) << 2;
		//UVC_LOGI("eof %d-%d\r\n", frame_len, fack_len);
	}

	if (uvc_camera_drv->frame_flag != (bmhead_info & 0x01))   // bit0 fliped
	{
		uvc_camera_drv->frame_flag = (bmhead_info & 0x01);
		if (uvc_camera_drv->sof || uvc_camera_drv->eof)
		{
			uvc_camera_drv->sof = false;
			uvc_camera_drv->eof = false;
			curr_frame_buffer->length = 0;
		}
	}

	if (bmhead_info & 0x02)   // bit1 = 1, end frame
	{
		/*uint8_t *eof;
		eof = (uint8_t *)curptr + newlen - 2;
		os_printf("%s, %02x, %02x\r\n", __func__, eof[0], eof[1]);*/
		uvc_camera_drv->eof = true;
		uvc_camera_drv->frame = curr_frame_buffer;
		curr_frame_buffer->sequence = ++uvc_frame_id;
	}
	else
	{
		if (((data[0] == 0xff) && (data[1] == 0xd8))) // strat frame
		{
			uvc_camera_drv->sof = true;
			//UVC_LOGI("uvc start, %02x, %02x\r\n", data[0], data[1]);
		}
	}

	if (uvc_drop_frame == 0)
	{
		uvc_memcpy_by_chnl(curr_frame_buffer->frame + curr_frame_buffer->length, data, fack_len, uvc_camera_drv->psram_dma);
		curr_frame_buffer->length += frame_len;
	}
	else
	{
		if (uvc_camera_drv->eof == true)
		{
			media_debug->isr_jpeg++;
			uvc_camera_drv->eof = false;
			uvc_camera_drv->sof = false;
			uvc_drop_frame--;
		}
	}

	UVC_PSRAM_DMA_OUT();

}

void uvc_camera_memcpy_finish_callback(dma_id_t id)
{
	if (uvc_camera_drv->eof == true)
	{
		if (uvc_camera_drv->sof == false)
		{
			curr_frame_buffer->length = 0;
			return;
		}
		frame_buffer_t *frame = uvc_camera_drv->frame;

#ifdef DVP_STRIP
		uvc_frame_strip(frame);
#endif

		media_debug->isr_jpeg++;

		UVC_JPEG_EOF_ENTRY();

		uvc_camera_config->fb_complete(frame);
		uvc_camera_drv->frame = NULL;
		uvc_camera_drv->eof = false;
		uvc_camera_drv->sof = false;

		UVC_JPEG_EOF_OUT();

		curr_frame_buffer = uvc_camera_config->fb_malloc();

		if (curr_frame_buffer == NULL
		    || curr_frame_buffer->frame == NULL)
		{
			UVC_LOGE("alloc frame error\n");
			return;
		}

		curr_frame_buffer->width = uvc_camera_config->device->width;
		curr_frame_buffer->height = uvc_camera_config->device->height;
		curr_frame_buffer->fmt = PIXEL_FMT_UVC_JPEG;
	}
}

static void uvc_camera_dma_finish_callback(dma_id_t id)
{
	UVC_SRAM_DMA_ENTRY();

	uint32_t already_len = uvc_camera_drv->rx_read_len;
	uint32_t copy_len = uvc_camera_drv->uvc_transfer_len;
	uint32_t frame_len = uvc_camera_drv->uvc_transfer_len - USB_UVC_HEAD_LEN;
	if (uvc_camera_drv->node_full_handler != NULL && copy_len > 12)
	{
		uvc_camera_drv->node_full_handler(uvc_camera_drv->buffer + already_len, copy_len, 0, frame_len);
	}

	already_len += copy_len;

	if (already_len >= uvc_camera_drv->rxbuf_len)
	{
		already_len = 0;
	}

	uvc_camera_drv->rx_read_len = already_len;

	UVC_SRAM_DMA_OUT();
}

static bk_err_t uvc_dma_config(void)
{
	bk_err_t ret = kNoErr;
	curr_frame_buffer = uvc_camera_config->fb_malloc();

	if (curr_frame_buffer == NULL)
	{
		UVC_LOGE("malloc frame fail \r\n");
		ret = kNoResourcesErr;
		return ret;
	}

	curr_frame_buffer->width = uvc_camera_config->device->width;
	curr_frame_buffer->height = uvc_camera_config->device->height;
	curr_frame_buffer->fmt = PIXEL_FMT_UVC_JPEG;

	uvc_camera_drv->uvc_dma = bk_dma_alloc(DMA_DEV_USB);
	if ((uvc_camera_drv->uvc_dma < DMA_ID_0) || (uvc_camera_drv->uvc_dma >= DMA_ID_MAX))
	{
		UVC_LOGE("malloc dma fail \r\n");
		ret = kNoResourcesErr;
		return ret;
	}
	UVC_LOGI("uvc_dma id:%d\r\n", uvc_camera_drv->uvc_dma);

	dma_config_t dma_config = {0};
	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;//DMA_DEV_USB;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = USB_UVC_FIFO_ADDR;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.start_addr = (uint32_t)uvc_camera_drv->buffer;
	dma_config.dst.end_addr = (uint32_t)(uvc_camera_drv->buffer + FRAME_BUFFER_UVC * 2);

	BK_LOG_ON_ERR(bk_dma_init(uvc_camera_drv->uvc_dma, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(uvc_camera_drv->uvc_dma, FRAME_BUFFER_UVC));
	BK_LOG_ON_ERR(bk_dma_register_isr(uvc_camera_drv->uvc_dma, NULL, uvc_camera_dma_finish_callback));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(uvc_camera_drv->uvc_dma));

	return ret;
}


static void uvc_fiddle_rx_vs_callback(void)
{
	bk_uvc_receive_video_stream();
}

static void uvc_disconnect_callback(void)
{
	UVC_LOGI("uvc_disconnect\r\n");

	uvc_send_msg(UVC_EXIT, 0);

	return;
}

static void uvc_get_packet_rx_vs_callback(uint8_t *arg, uint32_t count)
{
	uint32_t left_len = 0;

	UVC_USB_ISR_ENTRY();


	bk_dma_set_src_start_addr(uvc_camera_drv->uvc_dma, (uint32_t)arg);
	uvc_camera_drv->uvc_transfer_len = count;
	left_len = uvc_camera_drv->rxbuf_len - uvc_camera_drv->rx_read_len;
	if (left_len < uvc_camera_drv->uvc_transfer_len)
	{
		uvc_camera_drv->rx_read_len = 0;
	}

	uint32_t dest_start_addr = (uint32_t)(uvc_camera_drv->buffer + uvc_camera_drv->rx_read_len);
	bk_dma_set_dest_start_addr(uvc_camera_drv->uvc_dma, dest_start_addr);
	bk_dma_set_transfer_len(uvc_camera_drv->uvc_dma, uvc_camera_drv->uvc_transfer_len);
	bk_dma_start(uvc_camera_drv->uvc_dma);

	UVC_USB_ISR_OUT();
}

static uint32_t uvc_process_ppi_fps(uvc_camera_device_t *config)
{
	uint32_t param = 0;
	uint32_t resolution_id = UVC_FRAME_640_480;
	uvc_camera_device_t support_config[8] = {0};

	bk_uvc_get_resolution_framerate((void *)support_config, 8);
	for (uint8_t i = 0; i < 8; i++)
	{
		if (support_config[i].width == 0)
		{
			break;
		}

		if (config->width == support_config[i].width && config->height == support_config[i].height)
		{
			config->fps = support_config[i].fps;
			break;
		}
	}

	UVC_LOGI("width:%d, height:%d, fps:%d\r\n", config->width, config->height, config->fps);
	if ((config->width == 160) && (config->height == 120))
	{
		resolution_id = UVC_FRAME_160_120;
	}
	else if ((config->width == 176) && (config->height == 144))
	{
		resolution_id = UVC_FRAME_176_144;
	}
	else if ((config->width == 320) && (config->height == 240))
	{
		resolution_id = UVC_FRAME_320_240;
	}
	else if ((config->width == 352) && (config->height == 288))
	{
		resolution_id = UVC_FRAME_352_288;
	}
	else if ((config->width == 480) && (config->height == 320))
	{
		resolution_id = UVC_FRAME_480_320;
	}
	else if ((config->width == 480) && (config->height == 800))
	{
		resolution_id = UVC_FRAME_480_800;
	}
	else if ((config->width == 640) && (config->height == 320))
	{
		resolution_id = UVC_FRAME_640_320;
	}
	else if ((config->width == 640) && (config->height == 360))
	{
		resolution_id = UVC_FRAME_640_360;
	}
	else if ((config->width == 640) && (config->height == 480))
	{
		resolution_id = UVC_FRAME_640_480;
	}
	else if ((config->width == 800) && (config->height == 400))
	{
		resolution_id = UVC_FRAME_800_400;
	}
	else if ((config->width == 800) && (config->height == 480))
	{
		resolution_id = UVC_FRAME_800_480;
	}
	else if ((config->width == 864) && (config->height == 480))
	{
		resolution_id = UVC_FRAME_864_480;
	}
	else if ((config->width == 800) && (config->height == 600))
	{
		resolution_id = UVC_FRAME_800_600;
	}
	else if ((config->width == 960) && (config->height == 540))
	{
		resolution_id = UVC_FRAME_960_540;
	}
	else if ((config->width == 1280) && (config->height == 720))
	{
		resolution_id = UVC_FRAME_1280_720;
	}
	else
	{
		UVC_LOGE("Not find this resolution\r\n");
		return 0;
	}

	if (config->fps < FPS_5 || config->fps > FPS_30)
	{
		UVC_LOGE("Set FPS unknow: %d\r\n", config->fps);
		return 0;
	}

	param |= (resolution_id << 16);//bit[31-16]:ppi
	param |= config->fps; // bit[15-0]:fps

	return param;
}

static void uvc_set_ppi_fps(uint32_t data)
{
	uint32_t resolution_id;
	uint32_t fps;
	bk_err_t status;
	resolution_id = data >> 16;
	fps = data & 0xFFFF;
	status = bk_uvc_set_parameter(resolution_id, fps);
	if (status != kNoErr)
	{
		UVC_LOGI("Set uvc param0 error!\r\n");
		status = kOptionErr;
	}

	if (status != kNoErr)
	{
		uvc_send_msg(UVC_EXIT, 0);
	}
}

static void uvc_set_start(uint32_t data)
{
	bk_err_t status = BK_OK;
	void *parameter;

	parameter = (void *)uvc_get_packet_rx_vs_callback;
	bk_uvc_register_VSrxed_packet_callback(parameter);

	status = bk_uvc_start();
	if (status != BK_OK)
	{
		UVC_LOGE("start uvc error!\r\n");
		uvc_send_msg(UVC_EXIT, 0);
	}
	else
	{
		UVC_LOGI("start uvc ok!\r\n");
	}
}

static void uvc_set_stop(uint32_t data)
{
	bk_uvc_unregister_VSrxed_packet_callback();
	bk_uvc_stop();
	uvc_camera_drv->rx_read_len = 0;
}

static bk_err_t uvc_camera_init(const uvc_camera_config_t *config)
{
	uint32_t param;
	void *parameter;
	bk_err_t err = BK_OK;

	media_debug->isr_jpeg = 0;

	uvc_camera_config->fb_init((uint32_t)(uvc_camera_config->device->width) << 16 | uvc_camera_config->device->height);

	// step 1: init dma, fifo to sharemem
	err = uvc_dma_config();
	if (err != BK_OK)
	{
		goto init_error;
	}

	// step 2: init dma, sharemem to frame_buffer
	uvc_camera_drv->psram_dma = bk_dma_alloc(DMA_DEV_DTCM);
	if ((uvc_camera_drv->psram_dma < DMA_ID_0) || (uvc_camera_drv->psram_dma >= DMA_ID_MAX))
	{
		UVC_LOGE("malloc uvc_camera_drv->psram_dma fail \r\n");
		err = kNoResourcesErr;
		goto init_error;
	}
	UVC_LOGI("psram_dma id:%d\r\n", uvc_camera_drv->psram_dma);

	parameter = (void *)uvc_disconnect_callback;
	bk_uvc_register_disconnect_callback(parameter);

	parameter = (void *)uvc_fiddle_rx_vs_callback;
	err = bk_uvc_register_VSrxed_callback(parameter);
	if (err != BK_OK)
	{
		UVC_LOGE("register uvc rx video stream callback error!\r\n");
		goto init_error;
	}

	/*parameter = (void *)uvc_get_packet_rx_vs_callback;
	err = bk_uvc_register_VSrxed_packet_callback(parameter);
	if (err != BK_OK)
	{
		UVC_LOGE("register uvc rx every packet callback error!\r\n");
		goto init_error;
	}*/

	uvc_rx_vstream_buffptr = (uint8_t *)os_malloc(128);
	if (!uvc_rx_vstream_buffptr)
	{
		UVC_LOGE("malloc rx video stream buf error!\r\n");
		goto init_error;
	}

	parameter = (void *)uvc_rx_vstream_buffptr;
	err = bk_uvc_register_rx_vstream_buffptr(parameter);
	if (err != BK_OK)
	{
		UVC_LOGE("uvc set rx video stream buf addr error!\r\n");
		goto init_error;
	}

	param = 128;
	err = bk_uvc_register_rx_vstream_bufflen(param);
	if (err != BK_OK)
	{
		UVC_LOGE("uvc set rx video stream buf length error!\r\n");
		goto init_error;
	}

	bk_uvc_register_link(0);
	param = uvc_process_ppi_fps(uvc_camera_config->device);
	if (param == 0)
	{
		err = BK_FAIL;
		goto init_error;
	}

	err = bk_uvc_set_parameter((param >> 16), (param & 0xFFFF));
	if (err != BK_OK)
	{
		UVC_LOGE("uvc set fps and ppi error!\r\n");
	}

init_error:

	if (err != BK_OK)
	{
		os_printf("uvc init failed!\r\n");
	}

	return err;
}

static bk_err_t uvc_camera_deinit(void)
{
	// uvc deinit
	bk_err_t status = 0;
	bk_uvc_unregister_VSrxed_packet_callback();
	bk_uvc_stop();
	usb_device_set_using_status(0, USB_UVC_DEVICE);
	bk_usb_close();

	if (uvc_camera_drv)
	{
		bk_dma_stop(uvc_camera_drv->uvc_dma);
		bk_dma_deinit(uvc_camera_drv->uvc_dma);
		bk_dma_free(DMA_DEV_USB, uvc_camera_drv->uvc_dma);

		bk_dma_stop(uvc_camera_drv->psram_dma);
		bk_dma_deinit(uvc_camera_drv->psram_dma);
		bk_dma_free(DMA_DEV_DTCM, uvc_camera_drv->psram_dma);
	}

	//uvc_camera_config->fb_deinit();

	if (curr_frame_buffer)
	{
		uvc_camera_config->fb_free(curr_frame_buffer);
		curr_frame_buffer = NULL;
	}

	uvc_frame_id = 0;

	if (uvc_rx_vstream_buffptr)
	{
		os_free(uvc_rx_vstream_buffptr);
		uvc_rx_vstream_buffptr = NULL;
	}

	uvc_drop_frame = 0;

	UVC_LOGI("uvc_camera_deinit\n");

	return status;
}

static bk_err_t uvc_camera_reset(void)
{
	int ret = BK_OK;
	uint8_t wait_count = 0;

	bk_usb_open(USB_HOST_MODE);
	usb_device_set_using_status(1, USB_UVC_DEVICE);
	while(MGC_DevGetConnectStatus() != 1) {
		if(wait_count > 100) {
			os_printf("wait usb connect timeout 1s!\r\n");
			return BK_FAIL;
			break;
		}
		rtos_delay_milliseconds(10);
		wait_count++;
	}

	return ret;
}

static void uvc_process_main(void)
{
	bk_err_t err;

	uvc_send_msg(UVC_START, 0);

	while (1)
	{
		uvc_msg_t msg;
		err = rtos_pop_from_queue(&uvc_drv_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == err)
		{
			switch (msg.type)
			{
				case UVC_SET_PARAM:
					uvc_set_ppi_fps(msg.data);
					break;
				case UVC_START:
					uvc_set_start(msg.data);
					break;
				case UVC_STOP:
					uvc_set_stop(msg.data);
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

	if (uvc_camera_drv)
	{
		if (uvc_camera_drv->buffer)
		{
			os_free(uvc_camera_drv->buffer);
			uvc_camera_drv->buffer = NULL;
		}

		os_free(uvc_camera_drv);
		uvc_camera_drv = NULL;
	}

	if (uvc_camera_config && uvc_camera_config->uvc_disconnect)
	{
		uvc_camera_config->uvc_disconnect();
	}

	uvc_camera_config = NULL;

	rtos_deinit_queue(&uvc_drv_msg_que);
	uvc_drv_msg_que = NULL;

	uvc_thread_drv_handle = NULL;
	rtos_delete_thread(NULL);
}

bk_err_t bk_uvc_camera_power_on(void)
{
#if (CONFIG_DOORBELL_DEMO1)
	gpio_dev_unmap(37);
	bk_gpio_set_capacity(37, 0);
	BK_LOG_ON_ERR(bk_gpio_disable_input(37));
	BK_LOG_ON_ERR(bk_gpio_enable_output(37));
	// pull up gpio37, enable uvc camera vol
	//bk_gpio_set_output_low(37);
	bk_gpio_set_output_high(37);
	delay_ms(5);
#endif
	return BK_OK;
}

bk_err_t bk_uvc_camera_get_config(uvc_camera_device_t *param, uint16_t count)
{
	if (param == NULL)
	{
		return BK_FAIL;
	}
	bk_uvc_get_resolution_framerate((void *)param, count);

	return BK_OK;
}

bk_err_t bk_uvc_camera_set_config(uvc_camera_device_t *config)
{
	int status = kNoErr;
	uint32_t param = 0;
	param = uvc_process_ppi_fps(config);
	if (param != 0)
		status = uvc_send_msg(UVC_SET_PARAM, param);
	else
	{
		UVC_LOGE("set param failed!\r\n");
		status = BK_FAIL;
	}

	return status;
}

bk_err_t bk_uvc_camera_driver_start(void)
{
	uvc_send_msg(UVC_START, 0);

	return BK_OK;
}

bk_err_t bk_uvc_camera_driver_stop(void)
{
	uvc_send_msg(UVC_STOP, 0);

	return BK_OK;
}

uvc_camera_device_t *bk_uvc_camera_get_device(void)
{
	if (uvc_camera_config == NULL)
	{
		return NULL;
	}

	return uvc_camera_config->device;
}

bk_err_t bk_uvc_camera_driver_init(const uvc_camera_config_t *config)
{
	int ret = kNoErr;

	BK_ASSERT(config->fb_malloc != NULL);
	BK_ASSERT(config->fb_complete != NULL);

	UVC_DIAG_DEBUG_INIT();

#if (CONFIG_PSRAM)
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PSRAM, PM_CPU_FRQ_320M);

	bk_psram_init();
#endif

	ret = uvc_camera_reset();
	if (ret != kNoErr)
	{
		goto error;
	}

	if (uvc_camera_drv == NULL)
	{
		uvc_camera_drv = (uvc_camera_drv_t *)os_malloc(sizeof(uvc_camera_drv_t));

		if (uvc_camera_drv == NULL)
		{
			UVC_LOGE("uvc_camera_drv malloc failed\n");
			ret = kNoMemoryErr;
			goto error;
		}

		os_memset(uvc_camera_drv, 0, sizeof(uvc_camera_drv));

		uvc_camera_drv->buffer = (uint8 *)os_malloc(FRAME_BUFFER_UVC * 2);

		if (uvc_camera_drv->buffer == NULL)
		{
			UVC_LOGE("uvc_camera_drv malloc failed\n");
			if (uvc_camera_drv)
			{
				os_free(uvc_camera_drv);
				uvc_camera_drv = NULL;
			}

			ret = kNoMemoryErr;
			goto error;
		}
	}

	uvc_camera_drv->rxbuf_len = FRAME_BUFFER_UVC * 2;
	uvc_camera_drv->node_full_handler = uvc_process_data_packet;
	uvc_camera_drv->frame_flag = 0x80;

	uvc_camera_config = config;

	if (uvc_camera_init(uvc_camera_config) != BK_OK)
	{
		ret = kGeneralErr;
		goto error;
	}

	if ((!uvc_thread_drv_handle) && (!uvc_drv_msg_que))
	{

		ret = rtos_init_queue(&uvc_drv_msg_que,
		                      "uvc_queue",
		                      sizeof(uvc_msg_t),
		                      30);
		if (kNoErr != ret)
		{
			goto error;
		}

		ret = rtos_create_thread(&uvc_thread_drv_handle,
		                         3,
		                         "uvc_init",
		                         (beken_thread_function_t)uvc_process_main,
		                         4 * 1024,
		                         (beken_thread_arg_t)0);

		if (ret != kNoErr)
		{

			goto error;
		}

		return kNoErr;
	}

error:

	UVC_LOGI("uvc_camera_drv init failed\n");

	if (uvc_camera_drv)
	{
		if (uvc_camera_drv->buffer)
		{
			os_free(uvc_camera_drv->buffer);
			uvc_camera_drv->buffer = NULL;
		}

		os_free(uvc_camera_drv);
		uvc_camera_drv = NULL;
	}

	uvc_camera_deinit();

	if (uvc_drv_msg_que)
	{
		rtos_deinit_queue(&uvc_drv_msg_que);
		uvc_drv_msg_que = NULL;
	}

	if (uvc_thread_drv_handle)
	{
		uvc_thread_drv_handle = NULL;
	}

	return ret;
}

bk_err_t bk_uvc_camera_driver_deinit(void)
{
	if (!uvc_thread_drv_handle)
	{
		return BK_OK;
	}

	uvc_send_msg(UVC_EXIT, 0);

	while (uvc_thread_drv_handle)
	{
		delay_us(10);
	}

	return BK_OK;
}

void bk_uvc_camera_drop_frame(uint8_t number)
{
	uvc_drop_frame = number;
}



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

#include "uvc_sensor_devices.h"

#define TAG "uvc_drv"

#define UVC_LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define UVC_LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define UVC_LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define UVC_LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

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
		gpio_dev_unmap(GPIO_36);                 \
		bk_gpio_disable_pull(GPIO_36);           \
		bk_gpio_enable_output(GPIO_36);          \
		bk_gpio_set_output_low(GPIO_36);         \
		\
		gpio_dev_unmap(GPIO_37);                 \
		bk_gpio_disable_pull(GPIO_37);           \
		bk_gpio_enable_output(GPIO_37);          \
		bk_gpio_set_output_low(GPIO_37);         \
		\
	} while (0)

#define UVC_USB_ISR_ENTRY()                 bk_gpio_set_output_high(GPIO_32)
#define UVC_USB_ISR_OUT()                   bk_gpio_set_output_low(GPIO_32)

#define UVC_SRAM_DMA_ENTRY()                bk_gpio_set_output_high(GPIO_33)
#define UVC_SRAM_DMA_OUT()                  bk_gpio_set_output_low(GPIO_33)

#define UVC_PSRAM_DMA_ENTRY()               bk_gpio_set_output_high(GPIO_34)
#define UVC_PSRAM_DMA_OUT()                 bk_gpio_set_output_low(GPIO_34)

#define UVC_PSRAM_ISR_ENTRY()                bk_gpio_set_output_high(GPIO_35)
#define UVC_PSRAM_ISR_OUT()                  bk_gpio_set_output_low(GPIO_35)

#define UVC_JPEG_SOF_ENTRY()                bk_gpio_set_output_high(GPIO_36)
#define UVC_JPEG_SOF_OUT()                  bk_gpio_set_output_low(GPIO_36)

#define UVC_JPEG_EOF_ENTRY()                bk_gpio_set_output_high(GPIO_37)
#define UVC_JPEG_EOF_OUT()                  bk_gpio_set_output_low(GPIO_37)


#else

#define UVC_DIAG_DEBUG_INIT()

#define UVC_USB_ISR_ENTRY()
#define UVC_USB_ISR_OUT()

#define UVC_SRAM_DMA_ENTRY()
#define UVC_SRAM_DMA_OUT()

#define UVC_PSRAM_DMA_ENTRY()
#define UVC_PSRAM_DMA_OUT()

#define UVC_PSRAM_ISR_ENTRY()
#define UVC_PSRAM_ISR_OUT()

#define UVC_JPEG_SOF_ENTRY()
#define UVC_JPEG_SOF_OUT()

#define UVC_JPEG_EOF_ENTRY()
#define UVC_JPEG_EOF_OUT()

#endif


//#define UVC_STRIP

void uvc_camera_memcpy_finish_callback(dma_id_t id);


beken_thread_t uvc_thread_drv_handle = NULL;
beken_queue_t uvc_drv_msg_que = NULL;
static uvc_camera_config_t *uvc_camera_config_ptr = NULL;
static frame_buffer_t *curr_frame_buffer = NULL;
uint32_t uvc_frame_id = 0;
static uint8_t uvc_drop_frame = 0;

extern media_debug_t *media_debug;

#define FRAME_BUFFER_UVC         (1024 * 5)

typedef struct
{
	uint8_t  frame_flag : 1;
	uint8_t  eof: 1;
	uint8_t  psram_dma_busy : 1;
	uint8_t  uvc_dma;
	uint8_t  psram_dma;
	uint8_t  left_len;
	uint8_t  transfer_mode;
	uint32_t rxbuf_len;
	uint32_t rx_read_len;
	uint32_t uvc_transfer_len;
	uint8_t *buffer;
	void (*node_full_handler)(void *curptr, uint32_t len);
} uvc_camera_drv_t;

typedef struct
{
	uint8_t *src_addr;
	uint32_t length;
} uvc_mem_cpy_t;

uvc_camera_drv_t *uvc_camera_drv = NULL;
uvc_mem_cpy_t uvc_mem_cpy = {0};


static bk_err_t uvc_memcpy_by_chnl(void *out, const void *in, uint32_t len, dma_id_t cpy_chnls)
{
	dma_config_t dma_config = {0};

	uvc_camera_drv->psram_dma_busy = true;

	if (len & 0x3)
	{
		len = ((len >> 2) + 1) << 2;
	}

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
#if (CONFIG_SPE)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(cpy_chnls, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(cpy_chnls, DMA_ATTR_SEC));
#endif
	BK_LOG_ON_ERR(bk_dma_start(cpy_chnls));

	return BK_OK;
}

#ifdef UVC_STRIP
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

static void uvc_packet_check(uint8_t *ptr, uint32_t len)
{
	UVC_LOGI("[%02x, %02x, %02x, %02x, %d]\r\n", ptr[0], ptr[1], ptr[len - 2], ptr[len - 1], len);
}

#endif

static void uvc_camera_eof_handle(void)
{
	// frame_buffer complete
	media_debug->isr_jpeg++;

	UVC_JPEG_EOF_ENTRY();

	uvc_camera_config_ptr->fb_complete(curr_frame_buffer);
	uvc_camera_drv->eof = false;

	UVC_JPEG_EOF_OUT();
	curr_frame_buffer = uvc_camera_config_ptr->fb_malloc();

	if (curr_frame_buffer == NULL
			|| curr_frame_buffer->frame == NULL)
	{
		UVC_LOGE("alloc frame error\n");
		return;
	}

	curr_frame_buffer->sequence = ++uvc_frame_id;

	curr_frame_buffer->width = uvc_camera_config_ptr->uvc_config->device.width;
	curr_frame_buffer->height = uvc_camera_config_ptr->uvc_config->device.height;

	if (uvc_camera_config_ptr->uvc_config->type == MEDIA_UVC_MJPEG)
	{
		curr_frame_buffer->fmt = PIXEL_FMT_UVC_JPEG;
	}
	else
	{
		curr_frame_buffer->fmt = PIXEL_FMT_UVC_H264;
	}
}

static void uvc_camera_connect_state_change_callback(uvc_state_t state)
{
	if (state == UVC_DISCONNECT_ABNORMAL)
	{
		//uvc_send_msg(UVC_EXIT, 0);
		bk_uvc_camera_driver_deinit();
	}
	else if (state == UVC_CONNECTED)
	{
		if (uvc_camera_config_ptr && uvc_camera_config_ptr->uvc_connect_state_change_cb)
		{
			uvc_camera_config_ptr->uvc_connect_state_change_cb(state);
		}
	}
}

void uvc_bulk_packet_process(void *curptr, uint32_t newlen)
{
	uint32_t length = newlen;
	uint32_t fack_length = 0;
	uint8_t *data = curptr;

	UVC_PSRAM_DMA_ENTRY();
	if (curr_frame_buffer == NULL ||
		curr_frame_buffer->frame == NULL)
	{
		UVC_LOGE("curr_frame_buffer NULL\n");
		return;
	}

	if (length > USB_UVC_HEAD_LEN)
	{
		if (data[0] == 0x0C && data[12] == 0xFF && data[13] == 0xD8)
		{
			UVC_JPEG_SOF_ENTRY();
			data = &data[12];
			length -= USB_UVC_HEAD_LEN;
			uvc_camera_drv->eof = true;

			UVC_JPEG_SOF_OUT();
		}
	}
	else
	{
		if (data[0] == 0xFF && data[1] == 0xD8)
		{
			UVC_JPEG_SOF_ENTRY();
			uvc_camera_drv->eof = true;
			UVC_JPEG_SOF_OUT();
		}
	}

	if (uvc_drop_frame)
	{
		if (uvc_camera_drv->eof)
		{
			media_debug->isr_jpeg++;
			uvc_camera_drv->eof = false;
			uvc_drop_frame--;
		}

		if (uvc_drop_frame != 0)
		{
			UVC_PSRAM_DMA_OUT();
			return;
		}
	}

	if (curr_frame_buffer->length & 0x3)
	{
		uvc_camera_drv->eof = true;
	}

	if (uvc_camera_drv->eof && curr_frame_buffer->length > 0)
	{
		uvc_camera_eof_handle();
	}

	fack_length = length;

	if (fack_length & 0x3)
	{
		fack_length = ((fack_length >> 2) + 1) << 2;
	}

	UVC_LOGD("curr_frame_buffer->frame:%x curr_frame_buffer->length:%d length:%d====== \r\n",curr_frame_buffer->frame, curr_frame_buffer->length, length);

	os_memcpy_word((uint32_t *)(curr_frame_buffer->frame + curr_frame_buffer->length), (const uint32_t *)data,
					fack_length);
	curr_frame_buffer->length += length;

	UVC_PSRAM_DMA_OUT();
}

static void uvc_process_data_packet(void *curptr, uint32_t newlen)
{
	uint8_t *data;
	uint8_t bmhead_info;
	uint8_t need_add_length = 0;
	uint32_t fack_len = newlen - USB_UVC_HEAD_LEN;
	uint32_t aligned_length = 0;

	UVC_PSRAM_DMA_ENTRY();

	if (curr_frame_buffer == NULL
		|| curr_frame_buffer->frame == NULL)
	{
		UVC_LOGE("curr_frame_buffer NULL\n");
		return;
	}

	bmhead_info = *((uint8_t *)curptr + 1);
	data = (uint8_t *)curptr + USB_UVC_HEAD_LEN;

#ifdef UVC_STRIP
	uvc_packet_check(data, newlen);
#endif

	// step 1: check psram dma is busy
	if (uvc_camera_drv->psram_dma_busy)
	{
		// step 2_1: should save this packet pointer, wait last memcpy finish
		uvc_mem_cpy.src_addr = data;
		uvc_mem_cpy.length = newlen;
		media_debug->psram_busy++;
	}
	else
	{
		uvc_mem_cpy.length = 0;

		// step 2_2: check this packet is ok
		if (bmhead_info & 0x40)  // bit6 = 1, payload error
		{
			UVC_PSRAM_DMA_OUT();
			return;
		}

		if (uvc_camera_drv->frame_flag != (bmhead_info & 0x01))   // bit0 fliped
		{
			uvc_camera_drv->frame_flag = (bmhead_info & 0x01);
			// maybe first packet will satify this condition
			if (curr_frame_buffer->length != 0 || uvc_drop_frame > 0)
				uvc_camera_drv->eof = true;
		}

		if (uvc_drop_frame)
		{
			if (uvc_camera_drv->eof == true)
			{
				media_debug->isr_jpeg++;
				uvc_camera_drv->eof = false;
				uvc_drop_frame--;
			}

			if (uvc_drop_frame != 0)
			{
				UVC_PSRAM_DMA_OUT();
				return;
			}
		}

		// step 2_3: check if current packet is new frame
		if (uvc_camera_drv->eof)
		{
			uvc_camera_eof_handle();
		}

		if (curr_frame_buffer && newlen > 12)
		{
			need_add_length = 4 - (curr_frame_buffer->length & 0x3);
			if (need_add_length < 4)
			{
				aligned_length = curr_frame_buffer->length & 0xFFFFFFFC;
				uint32_t tmp_value = *((volatile uint32_t *)(curr_frame_buffer->frame + aligned_length));
				switch (need_add_length)
				{
					case 1:
						tmp_value &= ~(0xFF << 24);
						tmp_value |= (data[0] << 24);
						break;

					case 2:
						tmp_value &= ~(0xFFFF << 16);
						tmp_value |= (data[1] << 24) | (data[0] << 16);
						break;

					case 3:
					default:
						tmp_value &= 0xFF;
						tmp_value |= (data[2] << 24) | (data[1] << 16) | (data[0] << 8);
						break;
				}

				*((volatile uint32_t *)(curr_frame_buffer->frame + aligned_length)) = tmp_value;

				curr_frame_buffer->length = aligned_length + 4;
				fack_len -= need_add_length;
			}
			else
			{
				// if need_add_length == 4
				need_add_length = 0;
			}

			uvc_memcpy_by_chnl(curr_frame_buffer->frame + curr_frame_buffer->length, data + need_add_length, fack_len, uvc_camera_drv->psram_dma);
			curr_frame_buffer->length += fack_len;
		}
	}

	UVC_PSRAM_DMA_OUT();

}

static void uvc_bulk_packet_handle(void)
{
	UVC_SRAM_DMA_ENTRY();

	GLOBAL_INT_DECLARATION();
	uint32_t already_len = uvc_camera_drv->rx_read_len;
	uint32_t copy_len = uvc_camera_drv->uvc_transfer_len;

	uvc_bulk_packet_process(uvc_camera_drv->buffer + already_len, copy_len);

	already_len += copy_len;

	if (already_len >= uvc_camera_drv->rxbuf_len)
	{
		already_len = 0;
	}

	GLOBAL_INT_DISABLE();
	uvc_camera_drv->rx_read_len = already_len;
	GLOBAL_INT_RESTORE();
	UVC_SRAM_DMA_OUT();
}

void uvc_camera_memcpy_finish_callback(dma_id_t id)
{
	UVC_PSRAM_ISR_ENTRY();

	uvc_camera_drv->psram_dma_busy = false;

	if (uvc_mem_cpy.length && uvc_camera_drv->node_full_handler)
	{
		uvc_camera_drv->node_full_handler(uvc_mem_cpy.src_addr, uvc_mem_cpy.length);
		uvc_mem_cpy.length = 0;
	}

	UVC_PSRAM_ISR_OUT();
}

static void uvc_camera_dma_finish_callback(dma_id_t id)
{
	UVC_SRAM_DMA_ENTRY();

	GLOBAL_INT_DECLARATION();

	uint32_t already_len = uvc_camera_drv->rx_read_len;
	uint32_t copy_len = uvc_camera_drv->uvc_transfer_len;
	uint8_t *sram_dma_finish_addr = uvc_camera_drv->buffer + already_len + copy_len;
	copy_len += uvc_camera_drv->left_len;

	// step 1: check if fifo to sram is complete, is not, will compy fifo data by cpu
	while (uvc_camera_drv->left_len)
	{
		*sram_dma_finish_addr++ = *((volatile uint8_t *)(USB_UVC_FIFO_ADDR));
		uvc_camera_drv->left_len--;
	};

	// step 2: process data packet
	if (uvc_camera_drv->node_full_handler != NULL)
	{
		uvc_camera_drv->node_full_handler(uvc_camera_drv->buffer + already_len, copy_len);
	}

	already_len += copy_len;

	if (already_len >= uvc_camera_drv->rxbuf_len)
	{
		already_len = 0;
	}

	GLOBAL_INT_DISABLE();
	uvc_camera_drv->rx_read_len = already_len;
	GLOBAL_INT_RESTORE();
	UVC_SRAM_DMA_OUT();
}

static bk_err_t uvc_dma_config(void)
{
	bk_err_t ret = kNoErr;
	curr_frame_buffer = uvc_camera_config_ptr->fb_malloc();

	if (curr_frame_buffer == NULL)
	{
		UVC_LOGE("malloc frame fail \r\n");
		ret = BK_UVC_NO_MEMORY;
		return ret;
	}

	curr_frame_buffer->width = uvc_camera_config_ptr->uvc_config->device.width;
	curr_frame_buffer->height = uvc_camera_config_ptr->uvc_config->device.height;
	if (uvc_camera_config_ptr->uvc_config->type == MEDIA_UVC_MJPEG)
		curr_frame_buffer->fmt = PIXEL_FMT_UVC_JPEG;
	else
		curr_frame_buffer->fmt = PIXEL_FMT_UVC_H264;

	uvc_camera_drv->uvc_dma = bk_dma_alloc(DMA_DEV_USB);
	if ((uvc_camera_drv->uvc_dma < DMA_ID_0) || (uvc_camera_drv->uvc_dma >= DMA_ID_MAX))
	{
		UVC_LOGE("malloc dma fail \r\n");
		ret = BK_UVC_NO_RESOURCE;
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
#if (CONFIG_SPE)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(uvc_camera_drv->uvc_dma, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(uvc_camera_drv->uvc_dma, DMA_ATTR_SEC));
#endif

	return ret;
}

static void uvc_bulk_transfer_mode_handle(uint8_t *arg, uint32_t count)
{
	uint32_t left_len = 0;
	uint32_t dwIndex, dwIndex32;
	uint32_t dwCount32 = (count / 4);

	uvc_camera_drv->uvc_transfer_len = count;

	if (uvc_camera_drv->rx_read_len & 0x3)
	{
		uvc_camera_drv->rx_read_len = ((uvc_camera_drv->rx_read_len >> 2) + 1 ) << 2;
	}

	left_len = uvc_camera_drv->rxbuf_len - uvc_camera_drv->rx_read_len;
	if (left_len < uvc_camera_drv->uvc_transfer_len)
	{
		uvc_camera_drv->rx_read_len = 0;
	}

	uint8_t *dest_start_addr = (uvc_camera_drv->buffer + uvc_camera_drv->rx_read_len);

	for (dwIndex = dwIndex32 = 0; dwIndex32 < dwCount32; dwIndex32++, dwIndex += 4)
		*((uint32_t *) & (dest_start_addr[dwIndex])) = *((volatile uint32_t *)(arg));

	while (dwIndex < count)
	{
		*((uint8_t *) & (dest_start_addr[dwIndex++])) = *((volatile uint8_t *)(arg));
	}

	uvc_bulk_packet_handle();
}

static void uvc_iso_transfer_mode_handle(uint8_t *arg, uint32_t count)
{
	uint32_t left_len = 0;

	uvc_camera_drv->left_len = (count & 0x3);

	uvc_camera_drv->uvc_transfer_len = count - uvc_camera_drv->left_len;

	if (uvc_camera_drv->rx_read_len & 0x3)
	{
		uvc_camera_drv->rx_read_len = ((uvc_camera_drv->rx_read_len >> 2) + 1 ) << 2;
	}

	left_len = uvc_camera_drv->rxbuf_len - uvc_camera_drv->rx_read_len;
	if (left_len < count)
	{
		uvc_camera_drv->rx_read_len = 0;
	}

	uint32_t dest_start_addr = (uint32_t)(uvc_camera_drv->buffer + uvc_camera_drv->rx_read_len);
	bk_dma_set_dest_start_addr(uvc_camera_drv->uvc_dma, dest_start_addr);
	bk_dma_set_transfer_len(uvc_camera_drv->uvc_dma, uvc_camera_drv->uvc_transfer_len);
	bk_dma_start(uvc_camera_drv->uvc_dma);
}

static void uvc_get_packet_rx_vs_callback(uint8_t *arg, uint32_t count)
{
	UVC_USB_ISR_ENTRY();

	switch (uvc_camera_drv->transfer_mode)
	{
		case USB_ENDPOINT_CONTROL_TRANSFER:
			UVC_LOGD("UVC Transfer Mode: USB_ENDPOINT_CONTROL_TRANSFER\r\n");
			break;

		case USB_ENDPOINT_ISOCH_TRANSFER:
			UVC_LOGD("UVC Transfer Mode: USB_ENDPOINT_ISOCH_TRANSFER\r\n");
			uvc_iso_transfer_mode_handle(arg, count);
			break;

		case USB_ENDPOINT_BULK_TRANSFER:
			UVC_LOGD("UVC Transfer Mode: USB_ENDPOINT_BULK_TRANSFER\r\n");
			uvc_bulk_transfer_mode_handle(arg, count);
			break;

		case USB_ENDPOINT_INT_TRANSFER:
			UVC_LOGD("UVC Transfer Mode: USB_ENDPOINT_INT_TRANSFER\r\n");
			break;
		default:
			break;
	}

	UVC_USB_ISR_OUT();
}

static bk_err_t uvc_camera_init(void)
{
	bk_err_t err = BK_OK;

	media_debug->isr_jpeg = 0;
	media_debug->psram_busy = 0;

	uvc_camera_config_ptr->fb_init((uint32_t)(uvc_camera_config_ptr->uvc_config->device.width) << 16 | uvc_camera_config_ptr->uvc_config->device.height);

	// step 1: init dma, fifo to sharemem
	err = uvc_dma_config();
	if (err != BK_OK)
	{
		err = BK_UVC_DMA_ERROR;
		return err;
	}

	// step 2: init dma, sharemem to frame_buffer
	uvc_camera_drv->psram_dma = bk_dma_alloc(DMA_DEV_DTCM);
	if ((uvc_camera_drv->psram_dma < DMA_ID_0) || (uvc_camera_drv->psram_dma >= DMA_ID_MAX))
	{
		UVC_LOGE("malloc uvc_camera_drv->psram_dma fail \r\n");
		err = BK_UVC_NO_RESOURCE;
		return err;
	}
	UVC_LOGI("psram_dma id:%d\r\n", uvc_camera_drv->psram_dma);

	err = bk_uvc_camera_init(uvc_camera_config_ptr->uvc_config);
	if (err != BK_OK)
	{
		UVC_LOGE("uvc init failed!\r\n");
		return err;
	}

	bk_uvc_get_stream_transfer_mode(&uvc_camera_drv->transfer_mode);

	return err;
}

static bk_err_t uvc_camera_deinit(void)
{
	uvc_state_t state = uvc_camera_get_state();

	UVC_LOGI("%s, state:%d\n", __func__, state);

	if (state != UVC_DISCONNECT_ABNORMAL)
	{
		bk_uvc_camera_deinit();
	}

	if (uvc_camera_drv)
	{
		if (uvc_camera_drv->uvc_dma < DMA_ID_MAX)
		{
			bk_dma_stop(uvc_camera_drv->uvc_dma);
			bk_dma_deinit(uvc_camera_drv->uvc_dma);
			bk_dma_free(DMA_DEV_USB, uvc_camera_drv->uvc_dma);
		}

		if (uvc_camera_drv->psram_dma < DMA_ID_MAX)
		{
			bk_dma_stop(uvc_camera_drv->psram_dma);
			bk_dma_deinit(uvc_camera_drv->psram_dma);
			bk_dma_free(DMA_DEV_DTCM, uvc_camera_drv->psram_dma);
		}
	}

	if (curr_frame_buffer)
	{
		uvc_camera_config_ptr->fb_free(curr_frame_buffer);
		curr_frame_buffer = NULL;
	}

	uvc_camera_config_ptr->fb_deinit();

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

	if (state == UVC_DISCONNECT_ABNORMAL)
	{
		if (uvc_camera_config_ptr && uvc_camera_config_ptr->uvc_connect_state_change_cb)
		{
			uvc_camera_config_ptr->uvc_connect_state_change_cb(state);
		}
	}

	uvc_frame_id = 0;

	uvc_drop_frame = 0;

	UVC_LOGI("%s complete\n", __func__);

	return BK_OK;
}

bk_err_t bk_uvc_camera_driver_init(uvc_camera_config_t *config)
{
	int ret = kNoErr;

	BK_ASSERT(config->fb_malloc != NULL);
	BK_ASSERT(config->fb_complete != NULL);

	UVC_DIAG_DEBUG_INIT();

#if (CONFIG_PSRAM && (!CONFIG_SLAVE_CORE))
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PSRAM, PM_CPU_FRQ_320M);
	bk_psram_init();
#endif

	if (uvc_camera_drv == NULL)
	{
		uvc_camera_drv = (uvc_camera_drv_t *)os_malloc(sizeof(uvc_camera_drv_t));

		if (uvc_camera_drv == NULL)
		{
			UVC_LOGE("uvc_camera_drv malloc failed\n");
			ret = BK_UVC_NO_MEMORY;
			goto error;
		}

		os_memset(uvc_camera_drv, 0, sizeof(uvc_camera_drv_t));

		uvc_camera_drv->buffer = (uint8 *)os_malloc(FRAME_BUFFER_UVC * 2);

		if (uvc_camera_drv->buffer == NULL)
		{
			UVC_LOGE("uvc_camera_drv malloc failed\n");
			if (uvc_camera_drv)
			{
				os_free(uvc_camera_drv);
				uvc_camera_drv = NULL;
			}

			ret = BK_UVC_NO_MEMORY;
			goto error;
		}
	}

	uvc_camera_drv->rxbuf_len = FRAME_BUFFER_UVC * 2;
	uvc_camera_drv->node_full_handler = uvc_process_data_packet;
	uvc_camera_drv->eof = false;
	uvc_camera_drv->frame_flag = false;
	uvc_camera_drv->transfer_mode = USB_ENDPOINT_INVALID_TRANSFER;
	uvc_camera_config_ptr = config;
	uvc_camera_config_ptr->uvc_config->uvc_packet_rx_cb = uvc_get_packet_rx_vs_callback;
	uvc_camera_config_ptr->uvc_config->connect_state_change_cb = uvc_camera_connect_state_change_callback;

	ret = uvc_camera_init();
	if (ret != BK_OK)
	{
		goto error;
	}

	ret = bk_uvc_camera_start();
	if (ret != BK_OK)
	{
		UVC_LOGE("start uvc error!\r\n");
		goto error;
	}

	return ret;

error:

	UVC_LOGI("uvc_camera_drv init failed, %d\n", uvc_camera_get_state());

	uvc_camera_deinit();

#if (CONFIG_PSRAM && (!CONFIG_SLAVE_CORE))
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PSRAM, PM_CPU_FRQ_DEFAULT);
#endif

	return ret;
}

bk_err_t bk_uvc_camera_driver_deinit()
{
	UVC_LOGI("%s, %d\r\n", __func__, __LINE__);

	uvc_camera_deinit();

#if (CONFIG_PSRAM && (!CONFIG_SLAVE_CORE))
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PSRAM, PM_CPU_FRQ_DEFAULT);
#endif

	return BK_OK;
}

void bk_uvc_camera_drop_frame(uint8_t number)
{
	uvc_drop_frame = number;
}



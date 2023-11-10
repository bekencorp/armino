// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//	 http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <driver/h264.h>
#include <driver/yuv_buf.h>
#include <driver/psram.h>
#include <driver/i2c.h>
#include <driver/jpeg_enc.h>
#include <driver/flash.h>
#include <driver/dma.h>
#include <driver/jpeg_dec.h>
#include <cli.h>
#include <bk_general_dma.h>
#include "media_app.h"
#include "h264_driver.h"
#include "frame_buffer.h"
#include "camera.h"
#include "gpio_driver.h"
#include "gpio_map.h"

#define H264_UVC		0
extern void delay(int num);
extern bk_err_t demo_H264_udp_init(void);
extern bk_err_t demo_H264_tcp_init(void);

#if (CONFIG_USB_DEVICE && CONFIG_USB_UVC)
#ifdef H264_UVC
#undef H264_UVC
#define H264_UVC        0
#else
#define H264_UVC        0
#endif
extern void usbd_video_h264_init();
extern bk_err_t demo_h264_usb_init(void);
extern void usbd_video_h264_test(void);
#endif

static bk_err_t h264_dma_memcpy_init(void *in, const void *out ,uint32_t len);
static void sensor_transform_to_yuyv_half1(void);
static void sensor_transform_to_yuyv_half2(void);

//h264 config
uint8_t *h264_dest = (uint8_t *)0x60200000;
//uint8_t sensor_data[40960];
uint8_t *sensor_data = (uint8_t *)0x28060000;
//uint8_t *sensor_data = (uint8_t *)0x28060000;
uint8_t h264_dma_memcpy_id = 0;
int count = 0;
int frame_target = 0;
int flash_write_size = 0;
beken_thread_t h264_err_resume_task = NULL;
beken_semaphore_t resume_start_sem;
#if H264_UVC
static int yuv_frame_remain_line = 29;
static int odd_line = 0;
static int yuv_data_length = 20480;
static int yuv_data_offset = 0;
frame_buffer_t *jpeg_frame = NULL;
frame_buffer_t *yuv_frame = NULL;
frame_buffer_t *yuv_frame_buffer = NULL;
frame_buffer_t *pre_yframe = NULL;
frame_buffer_t *h264_dest_frame = NULL;
beken_thread_t h264_fb_decoder_task = NULL;
static int jpeg_dec_end =0;
static int odd_frame = 0;
beken_semaphore_t decode_sem;
#endif

yuv_buf_config_t yuv_buf_config = {
	.work_mode = YUV_BUF_MODE_H264,
	.mclk_div = YUV_MCLK_DIV_6,
	.x_pixel = 80,
	.y_pixel = 60,
	.yuv_mode_cfg.yuv_format = YUV_FORMAT_YUYV,
};

h264_dma_config_t h264_dma_config = {
	.rx_buf_len = 0x10000,
	.dma_channel = 0,
	.transfer_len = 0x10000,
};

static void h264_print(uint8_t *buffer, int length)
{
	for (int i = 0; i < length; i++)
	{
		os_printf("%02x ", *(buffer+i));
	}
	os_printf("\r\n");
}

void h264_final_out_handler(void *param)
{
	GPIO_UP(2);
	os_printf("frame number is %d \r\n", count++);
	if (count == frame_target)
	{
		bk_h264_int_count_show();
		bk_yuv_buf_stop(YUV_BUF_MODE_H264);
		//bk_h264_driver_deinit();
		//bk_h264_dma_rx_deinit();
		//h264_print(h264_dma_config.rx_buf, 8);
	}
	GPIO_DOWN(2);
}

void h264_line_down_handler(void)
{
#if H264_UVC
	yuv_frame_remain_line--;
	if(yuv_frame_remain_line == 28) {
		rtos_set_semaphore(&decode_sem);
	}
	if(yuv_frame_remain_line == -1) {
		yuv_data_offset = 0;
		if(odd_frame == 1) {
			yuv_frame_buffer->frame = pre_yframe->frame;
		} else {
			yuv_frame_buffer->frame = yuv_frame->frame;
		}
		yuv_frame_remain_line = 29;
	}

	if(odd_line == 0) {
		dma_memcpy_by_chnl(sensor_data + yuv_data_length, yuv_frame_buffer->frame + yuv_data_offset, yuv_data_length, h264_dma_memcpy_id);
		odd_line = 1;
		yuv_data_offset += yuv_data_length;
		sensor_transform_to_yuyv_half2();
	} else {
		dma_memcpy_by_chnl(sensor_data, yuv_frame_buffer->frame + yuv_data_offset, yuv_data_length, h264_dma_memcpy_id);
		odd_line = 0;
		yuv_data_offset += yuv_data_length;
		sensor_transform_to_yuyv_half1();
	}
	delay(10);
	bk_yuv_buf_send_data_to_h264();
#endif
}

void yuv_buf_h264_err_resume(yuv_buf_unit_t id, void *param)
{
	os_printf("h2p \r\n");
	rtos_set_semaphore(&resume_start_sem);
}

static void h264_err_resume_task_entry(beken_thread_arg_t data)
{
	os_printf("enter resume tast \r\n");
	int ret = 0;
	while(1)
	{
		ret = rtos_get_semaphore(&resume_start_sem, BEKEN_NEVER_TIMEOUT);
		if (ret != BK_OK)
			{
				os_printf("%s semaphore get failed: %d\n", __func__, ret);
			}
		media_app_camera_close(APP_CAMERA_DVP_H264_WIFI_TRANSFER);
	}
}

#if H264_UVC
void h264_fb_decoder_task_entry(beken_thread_arg_t data)
{
	int ret = 0;
	while(1)
	{
		ret = rtos_get_semaphore(&decode_sem, BEKEN_NEVER_TIMEOUT);

		if(jpeg_dec_end == 1) {
			if (ret != BK_OK)
			{
				CLI_LOGD("%s semaphore get failed: %d\n", __func__, ret);
			}

			frame_buffer_fb_free(jpeg_frame, MODULE_DECODER);
			jpeg_frame = frame_buffer_fb_read(MODULE_DECODER);
			if(jpeg_frame == NULL) {
				CLI_LOGE("jpeg frame read failed!\r\n");
			}

			if(odd_frame == 0)
			{
				odd_frame = 1;
				ret = bk_jpeg_dec_dma_start(jpeg_frame->length, jpeg_frame->frame, pre_yframe->frame);
				if(ret) {
					CLI_LOGE("jpeg dec failed! \r\n");
				}
			} else {
				odd_frame = 0;
				ret = bk_jpeg_dec_dma_start(jpeg_frame->length, jpeg_frame->frame, yuv_frame->frame);
				if(ret) {
					CLI_LOGE("jpeg dec failed! \r\n");
				}
			}
			jpeg_dec_end = 0;
		}
	}
}

static void sensor_data_black(void)
{
	for (int i = 0; i < 40960/4; i++) {
		sensor_data[4*i]=16;
		sensor_data[4*i+1]=128;
		sensor_data[4*i+2]=16;
		sensor_data[4*i+3]=128;
	}
}

static void sensor_transform_to_yuyv_half1(void)
{
	for (int i = 0; i < 5120; i++) {
		uint8_t temp = sensor_data[i*4+1];
		sensor_data[i*4+1] = sensor_data[i*4+2];
		sensor_data[i*4+2] = temp;
	}
}

static void sensor_transform_to_yuyv_half2(void)
{
	for (int i = 5120; i < 10240; i++) {
		uint8_t temp = sensor_data[i*4+1];
		sensor_data[i*4+1] = sensor_data[i*4+2];
		sensor_data[i*4+2] = temp;
	}
}

static void h264_jpeg_dec(void)
{
	os_printf("jpeg decoder end \r\n");
	jpeg_dec_end = 1;
}
#endif


static void h264_test_func(int frame,int ppi_second)
{
	int err = 0;
	frame_target = frame;
	count = 0;
	static media_ppi_t media_ppi;

	switch (ppi_second)
	{
		case 272:
			media_ppi = PPI_480X272;
			yuv_buf_config.x_pixel = 60;
			yuv_buf_config.y_pixel = 34;
			break;
		case 480:
			media_ppi = PPI_640X480;
			yuv_buf_config.x_pixel = 80;
			yuv_buf_config.y_pixel = 60;
			break;
		case 720:
			media_ppi = PPI_1280X720;
			yuv_buf_config.x_pixel = 160;
			yuv_buf_config.y_pixel = 90;
			break;
		default:
			break;
	}

	/*check clk*/
	bk_h264_clk_check();
	os_printf("line %d \r\n",__LINE__);

#if (CONFIG_USB_DEVICE && CONFIG_USB_UVC)
	usbd_video_h264_init();
#else

	/*h264 dma init*/
	h264_dma_config.rx_buf = h264_dest;
	err = bk_h264_dma_rx_init(&h264_dma_config);
	os_printf("line %d \r\n",__LINE__);
	if (err)
	{
		CLI_LOGE("h264 dma init failed! err code -0x%x \r\n", -err);
	}
#endif
	/*h264 driver init*/
	err = bk_h264_driver_init();
	if (err)
	{
		CLI_LOGE("h264 driver init failed! err code -0x%x \r\n", -err);
	}

	bk_h264_init(media_ppi);
	if (err)
	{
		CLI_LOGE("h264 module init failed! err code -0x%x \r\n", -err);
	}
	bk_h264_pingpong_in_psram_clk_set();

	/*h264 param set*/
	// os_printf("line %d \r\n",__LINE__);
	// bk_h264_set_pframe_num(0x5);
	// os_printf("line %d \r\n",__LINE__);
	// bk_h264_set_qp(0x19);
	// os_printf("line %d \r\n",__LINE__);
	// bk_h264_set_quality(0x50, 0x23);
	// os_printf("line %d \r\n",__LINE__);

	/*register h264 callback*/
	bk_h264_register_isr(H264_LINE_DONE, (h264_isr_t)h264_line_down_handler, 0);
	bk_h264_register_isr(H264_FINAL_OUT, (h264_isr_t)h264_final_out_handler, 0);

	//yuv_buf driver init
	err = bk_yuv_buf_driver_init();
	if (err)
	{
		CLI_LOGE("yuv driver init failed! err code -0x%x \r\n", -err);
	}
	bk_yuv_buf_init(&yuv_buf_config);
	bk_yuv_buf_register_isr(YUV_BUF_H264_ERR,(yuv_buf_isr_t)yuv_buf_h264_err_resume, NULL);
	bk_yuv_buf_set_em_base_addr((uint32_t)sensor_data);
	err = bk_yuv_buf_start(YUV_BUF_MODE_H264);
	if (err)
	{
		CLI_LOGE("yuv start failed! err code -0x%x \r\n", -err);
	}

	/*camera init*/
	//bk_h264_camera_gpio_set();
	//frame_buffer_init();
#if (CONFIG_USB_DEVICE && CONFIG_USB_UVC)
	media_app_camera_open(APP_CAMERA_DVP_H264_USB_TRANSFER, PPI_640X480);
#else
	media_app_camera_open(APP_CAMERA_DVP_H264_LOCAL, media_ppi);
#endif

	/*encode start*/
	//bk_h264_encode_enable();
	//CLI_LOGI("h264 encode start! \r\n");
}

static void h264_demo_main(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (os_strcmp(argv[1], "encode") == 0)
	{
		int v1 = strtoul(argv[2], NULL, 10);
		int v2 = strtoul(argv[3], NULL, 10);
		uint32_t v3 = strtoul(argv[4], NULL, 16);
		sensor_data = (uint8_t *) v3;
		os_printf("sensor_data addr is %x \r\n", sensor_data);
		h264_test_func(v1,v2);
	}
	else if (os_strcmp(argv[1], "loop") == 0) {
		int v1 = strtoul(argv[2], NULL, 10);
		int v2 = strtoul(argv[3], NULL, 10);
		int v3 = strtoul(argv[3], NULL, 10);
		for (int i = 0; i < v3; i++)
		{
			h264_test_func(v1,v2);
			delay(400000);
			media_app_camera_close(APP_CAMERA_DVP_H264_WIFI_TRANSFER);
			bk_h264_dma_rx_deinit();
			h264_print(h264_dma_config.rx_buf, 8);
			delay(10000);
		}
	}
	else if (os_strcmp(argv[1], "stop") == 0) {
		media_app_camera_close(APP_CAMERA_DVP_H264_WIFI_TRANSFER);
		bk_h264_dma_rx_deinit();
		h264_print(h264_dma_config.rx_buf, 8);
	}
	else if (os_strcmp(argv[1], "pause") == 0) {
		GPIO_UP(26);
		bk_h264_encode_disable();
		GPIO_DOWN(26);
	}
	else if (os_strcmp(argv[1], "resume") == 0) {
		GPIO_UP(26);
		bk_h264_encode_enable();
		GPIO_DOWN(26);
	}
	else if (os_strcmp(argv[1], "task_create") == 0) {
		int err;
		rtos_init_semaphore(&resume_start_sem, 1);
		err = rtos_create_thread(&h264_err_resume_task,
							4,
							"h264 err",
							(beken_thread_function_t)h264_err_resume_task_entry,
							1024 * 6,
							(beken_thread_arg_t)NULL);
		if (err != kNoErr)
		{
			CLI_LOGE("Error: Failed to create fb read: %d\n", err);
		}
	}
	else if (os_strcmp(argv[1], "flash") == 0) {
		int v1 = strtoul(argv[2], NULL, 10);
		flash_write_size = v1;
		int v2 = strtoul(argv[3], NULL, 16);
		bk_flash_driver_init();

		uint8_t write_buf[1024];
		CLI_LOGI("h264 code is: \r\n");

		bk_flash_set_protect_type(FLASH_PROTECT_NONE);
		for (int k = 0; k < flash_write_size; k++)
		{
			for (int i = 0; i < 1024 / 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					write_buf[i * 4 + j] = *(h264_dest + k * 1024 + i * 4 + (3 - j));
				}
			}
			if (k % 4 == 0)
			{
				bk_flash_erase_sector(v2 + k * 1024);
			}
			bk_flash_write_bytes(v2 + k * 1024, write_buf, 1024);
		}
		CLI_LOGI("h264 flash write finish! \r\n");
		CLI_LOGI("frame head is: \r\n");
		uint8_t a[8];
		bk_flash_read_bytes(v2, a, 8);
		for (int i = 0; i < 8; i++)
		{
			os_printf("%x ", *(a + i));
		}
	}
}

static void h264_demo_wifi(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{

	int ret = 0;
	char *msg = NULL;

	if (os_strcmp(argv[1], "udp") == 0) {
		demo_H264_udp_init();
	} else if (os_strcmp(argv[1], "tcp") == 0) {
		demo_H264_tcp_init();
	} else if (os_strcmp(argv[1], "buffer") == 0) {
		media_ppi_t ppi = PPI_DEFAULT;
		if (argc >= 3)
			ppi = get_string_to_ppi(argv[2]);
		if (ppi == PPI_DEFAULT)
			ppi = PPI_640X480;

		media_app_camera_open(APP_CAMERA_DVP_H264_LOCAL, ppi);
	}

	if (ret != BK_OK) {
		msg = CLI_CMD_RSP_ERROR;
	} else {
		msg = CLI_CMD_RSP_SUCCEED;
	}
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

#if (CONFIG_USB_DEVICE && CONFIG_USB_UVC)
static void h264_demo_usb(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	char *msg = NULL;

	if (os_strcmp(argv[1], "local") == 0) {
		usbd_video_h264_test();
	} else if (os_strcmp(argv[1], "camera") == 0) {
		demo_h264_usb_init();
	}

	if (ret != BK_OK) {
		msg = CLI_CMD_RSP_ERROR;
	} else {
		msg = CLI_CMD_RSP_SUCCEED;
	}
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}
#endif

#if H264_UVC
static void h264_uvc_encode(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (os_strcmp(argv[1], "encode") == 0) {
		int err = 0;
		int v1 = strtoul(argv[2], NULL, 10);
		// int v4 = strtoul(argv[3], NULL, 10);
		// int v5 = strtoul(argv[3], NULL, 10);
		frame_target = v1;
		count = 0;

		/*camera*/
		os_printf("%s : %d \r\n",__func__,__LINE__);
		media_app_camera_open(APP_CAMERA_UVC, PPI_640X480);
		delay(2000);
		frame_buffer_fb_display_init((800<<16)|600);

		pre_yframe = frame_buffer_fb_display_malloc_wait();
		if(pre_yframe == NULL) {
			os_printf("pre_yframe malloc failed \r\n");
		}

		yuv_frame = frame_buffer_fb_display_malloc_wait();
		if(yuv_frame == NULL) {
			CLI_LOGE("yuv_frame malloc failed!\r\n");
		}

		yuv_frame_buffer = frame_buffer_fb_display_malloc_wait();
		if(yuv_frame_buffer == NULL) {
			os_printf("yuv_frame_buffer malloc failed \r\n");
		}

		h264_dest_frame = frame_buffer_fb_display_malloc_wait();
		if(h264_dest_frame == NULL) {
			os_printf("h264_dest_frame malloc failed \r\n");
		}

		frame_buffer_fb_register(MODULE_DECODER, FB_INDEX_JPEG);
		jpeg_frame = frame_buffer_fb_read(MODULE_DECODER);
		os_printf("frame info %d \r\n", jpeg_frame->length);

		/*jpeg decoder*/
		bk_jpeg_dec_driver_init();
		bk_jpeg_dec_isr_register(DEC_END_OF_FRAME,(jpeg_dec_isr_cb_t)h264_jpeg_dec);
		rtos_init_semaphore(&decode_sem, 1);

		err = bk_jpeg_dec_dma_start(jpeg_frame->length, jpeg_frame->frame, yuv_frame->frame);
		if(err) {
			CLI_LOGE("jpeg dec failed! \r\n");
		}

		err = rtos_create_thread(&h264_fb_decoder_task,
							4,
							"h264 fb read",
							(beken_thread_function_t)h264_fb_decoder_task_entry,
							1024 * 6,
							(beken_thread_arg_t)NULL);
		if (err != kNoErr)
		{
			CLI_LOGE("Error: Failed to create fb read: %d\n", err);
		}

		/*check clk*/
		bk_h264_clk_check();

		/*h264 dma init*/
		h264_dma_config.rx_buf = h264_dest_frame->frame;
		err = bk_h264_dma_rx_init(&h264_dma_config);
		if (err)
		{
			CLI_LOGE("h264 dma init failed! err code -0x%x \r\n", -err);
		}
		h264_dma_memcpy_id = bk_dma_alloc(DMA_DEV_H264);
		os_printf("dma memcpy id is %d==========================\r\n",h264_dma_memcpy_id);

		/*h264 driver init*/
		os_printf("line %d \r\n",__LINE__);
		err = bk_h264_driver_init();
		if (err)
		{
			CLI_LOGE("h264 driver init failed! err code -0x%x \r\n", -err);
		}
		err = bk_h264_init(VGA);
		if (err)
		{
			CLI_LOGE("h264 module init failed! err code -0x%x \r\n", -err);
		}

		/*register h264 callback*/
		bk_h264_register_isr(H264_LINE_DONE, (h264_isr_t)h264_line_down_handler, 0);
		bk_h264_register_isr(H264_FINAL_OUT, (h264_isr_t)h264_final_out_handler, 0);


		//yuv_buf driver init
		err = bk_yuv_buf_driver_init();
		if (err)
		{
			CLI_LOGE("yuv driver init failed! err code -0x%x \r\n", -err);
		}
		yuv_buf_config.mclk_div = 2;
		bk_yuv_buf_init(&yuv_buf_config);
		bk_yuv_buf_enable_h264_nosensor_mode();
		bk_yuv_buf_set_em_base_addr((uint32_t)sensor_data);
		err = bk_yuv_buf_start(YUV_BUF_MODE_H264);
		if (err)
		{
			CLI_LOGE("yuv start failed! err code -0x%x \r\n", -err);
		}

		/*encode start*/
		delay(3000);
		dma_memcpy_by_chnl(sensor_data, yuv_frame->frame, yuv_data_length, h264_dma_memcpy_id);
		yuv_data_offset += yuv_data_length;
		delay(3000);
		sensor_transform_to_yuyv_half1();
		yuv_frame_buffer->frame = yuv_frame->frame;
		bk_h264_encode_enable();

		bk_yuv_buf_send_data_to_h264();

		os_printf("h264 encode start! \r\n");
	}
	if (os_strcmp(argv[1], "flash") == 0)
	{
		int v1 = strtoul(argv[2], NULL, 10);
		int v2 = strtoul(argv[3], NULL, 16);
		flash_write_size = v1;
		bk_flash_driver_init();

		uint8_t write_buf[1024];
		CLI_LOGI("h264 code is: \r\n");

		bk_flash_set_protect_type(FLASH_PROTECT_NONE);
		for (int k = 0; k < flash_write_size; k++)
		{
			for (int i = 0; i < 1024 / 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					write_buf[i * 4 + j] = *(h264_dest_frame->frame + k * 1024 + i * 4 + (3 - j));
				}
			}
			if (k % 4 == 0)
			{
				bk_flash_erase_sector(v2 + k * 1024);
			}
			bk_flash_write_bytes(v2 + k * 1024, write_buf, 1024);
		}
		CLI_LOGI("h264 flash write finish! \r\n");
		CLI_LOGI("frame head is: \r\n");
		uint8_t a[8];
		bk_flash_read_bytes(v2, a, 8);
		for (int i = 0; i < 8; i++)
		{
			os_printf("%x ", *(a + i));
		}
	}
}
#endif

#define H264_CMD_CNT (sizeof(h264_commands) / sizeof(struct cli_command))
static const struct cli_command h264_commands[] = {
	{"h264_init", "h264_init {encode|flash}", h264_demo_main}, // encode (x frames) (write y frame to flash
	{"h264_wifi", "h264_wifi {tcp|udp|rtp}", h264_demo_wifi},
#if (CONFIG_USB_DEVICE && CONFIG_USB_UVC)
	{"h264_usb", "h264_usb {local|camera}", h264_demo_usb},
#endif
#if H264_UVC
	{"h264_uvc", "h264_uvc {encode|flash}", h264_uvc_encode},
#endif
};

int cli_h264_init(void)
{
	return cli_register_commands(h264_commands, H264_CMD_CNT);
}

#include "at_common.h"

// this moudule only support camera in chip, with sccb interface.
#if (CONFIG_CAMERA)
#include <components/video_transfer.h>
#include <components/dvp_camera.h>
#endif

#include <driver/jpeg_enc.h>
#include <driver/i2c.h>
#include <driver/dma.h>
#if (CONFIG_SDCARD_HOST || CONFIG_USB_HOST)
#include "ff.h"
#include "diskio.h"
#endif

#if CONFIG_PSRAM
#include <driver/psram.h>
#endif

#if CONFIG_GENERAL_DMA
#include "bk_general_dma.h"
#endif

#include "at_video_common.h"

#define EJPEG_DMA_CHANNEL              DMA_ID_4
#define EJPEG_DATA_ADDR                (0x60010000) // PSRAM

extern void delay(int num);//TODO fix me

#if (CONFIG_CAMERA && CONFIG_APP_DEMO_VIDEO_TRANSFER && (CONFIG_SDCARD_HOST || CONFIG_USB_HOST))
static video_buff_t *g_video_buff = NULL;
static uint32_t g_pkt_seq = 0;
int video_set_camera_enable_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int video_take_photo_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif // (CONFIG_CAMERA && CONFIG_APP_DEMO_VIDEO_TRANSFER)

#if CONFIG_PSRAM
int video_read_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int video_disable_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif

#if CONFIG_CAMERA
int video_set_yuv_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int video_close_yuv_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#endif //CONFIG_CAMERA
const at_command_t video_at_cmd_table[] = {
#if (CONFIG_CAMERA && CONFIG_APP_DEMO_VIDEO_TRANSFER && (CONFIG_SDCARD_HOST || CONFIG_USB_HOST))
	{0, "ENABLE", 0, "open/close camera(1/0)", video_set_camera_enable_handler},
	{1, "TAKE_PHOTO", 1, "take a picture", video_take_photo_handler},
#endif
#if CONFIG_PSRAM
	{2, "READPSRAM", 0, "psram", video_read_psram_handler},
	{3, "DEINITPSRAM", 0, "deinit psram", video_disable_psram_handler},
#endif
#if CONFIG_CAMERA
	{4, "SETYUV", 0, "set jpeg/yuv mode and to psram", video_set_yuv_psram_handler},
	{5, "CLOSEYUV", 0, "close jpeg", video_close_yuv_psram_handler},
#endif
};

int video_at_cmd_cnt(void)
{
	return sizeof(video_at_cmd_table) / sizeof(video_at_cmd_table[0]);
}

#if (CONFIG_CAMERA && CONFIG_APP_DEMO_VIDEO_TRANSFER && (CONFIG_SDCARD_HOST || CONFIG_USB_HOST))
static int video_buffer_recv_data(uint8_t * data, uint32_t len)
{
	if (g_video_buff->buf_base) {
		vbuf_header_t *hdr = (vbuf_header_t *)data;
		uint32_t org_len, left_len;
		GLOBAL_INT_DECLARATION();

		if (len < sizeof(vbuf_header_t)) {
			os_printf("unknow err!\r\n");
			return len;
		}

		org_len = len - sizeof(vbuf_header_t);
		data = data + sizeof(vbuf_header_t);

		if ((hdr->id != g_video_buff->frame_id) && (hdr->pkt_seq == 1)) {
			// start of frame;
			GLOBAL_INT_DISABLE();
			g_video_buff->frame_id = hdr->id;
			g_video_buff->frame_len = 0;
			g_video_buff->frame_pkt_cnt = 0;
			g_video_buff->buf_ptr = g_video_buff->buf_base;
			g_video_buff->receive_state = VIDEO_BUFF_COPY;
			GLOBAL_INT_RESTORE();
			//os_printf("sof:%d\r\n", g_vbuf->frame_id);
		}

		//os_printf("%d-%d: %d-%d: %d\r\n", hdr->id, g_video_buff->frame_id,
		//    hdr->pkt_seq, (g_video_buff->frame_pkt_cnt + 1), g_video_buff->start_buf);

		if ((hdr->id == g_video_buff->frame_id)
			&& ((g_video_buff->frame_pkt_cnt + 1) == hdr->pkt_seq)
			&& (g_video_buff->receive_state == VIDEO_BUFF_COPY)) {
			left_len = g_video_buff->buf_len - g_video_buff->frame_len;
			if (org_len <= left_len) {
#if CONFIG_GENERAL_DMA
				dma_memcpy(g_video_buff->buf_ptr, data, org_len);
#else
				os_memcpy(g_video_buff->buf_ptr, data, org_len);
#endif

				GLOBAL_INT_DISABLE();
				g_video_buff->frame_len += org_len;
				g_video_buff->buf_ptr += org_len;
				g_video_buff->frame_pkt_cnt += 1;
				GLOBAL_INT_RESTORE();

				if (hdr->is_eof == 1) {
					uint8_t *sof_ptr, *eof_ptr, *crc_ptr;
					uint32_t p_len, right_image = 0;

					sof_ptr = g_video_buff->buf_base;
					eof_ptr = g_video_buff->buf_base + (g_video_buff->frame_len - 7);
					crc_ptr = eof_ptr + 3;

					/*os_printf("sof:%02x, %02x\r\n", sof_ptr[0], sof_ptr[1]);
					os_printf("eof:%02x, %02x\r\n", eof_ptr[0], eof_ptr[1]);
					os_printf("crc:%02x, %02x, %02x, %02x\r\n", crc_ptr[0], crc_ptr[1], crc_ptr[2], crc_ptr[3]);
					p_len = crc_ptr[0] + (crc_ptr[1] << 8)
						+ (crc_ptr[2] << 16) + (crc_ptr[3] << 24);
					os_printf("p_len:%d, frame_len:%d\r\n", p_len, g_video_buff->frame_len);*/

					if (((sof_ptr[0] == 0xff) && (sof_ptr[1] == 0xd8)) &&
						((eof_ptr[0] == 0xff) && (eof_ptr[1] == 0xd9))) {
						p_len = crc_ptr[0] + (crc_ptr[1] << 8)
								+ (crc_ptr[2] << 16) + (crc_ptr[3] << 24);

						if (p_len == (g_video_buff->frame_len - 5))
							right_image = 1;
					}

					if (right_image) {
						GLOBAL_INT_DISABLE();
						g_video_buff->receive_state = VIDEO_BUFF_IDLE;
						GLOBAL_INT_RESTORE();

						// all frame data have received, wakeup usr thread
						rtos_set_semaphore(&g_video_buff->aready_semaphore);

						return g_video_buff->frame_len;
					}
				}
			} else {
				os_printf("vbuf full!\r\n");
				GLOBAL_INT_DISABLE();
				g_video_buff->receive_state = VIDEO_BUFF_FULL;
				GLOBAL_INT_RESTORE();
				// all frame data not have received, psram not enough
				rtos_set_semaphore(&g_video_buff->aready_semaphore);
				return 0;
			}
		}
	}

	return len;
}

static void video_buffer_add_pkt_header(video_packet_t *param)
{
	vbuf_header_t *elem_tvhdr = (vbuf_header_t *)param->ptk_ptr;

	g_pkt_seq++;
	elem_tvhdr->id = (uint8_t)param->frame_id;
	elem_tvhdr->is_eof = param->is_eof;
	elem_tvhdr->pkt_cnt = param->frame_len;
	elem_tvhdr->pkt_seq = g_pkt_seq;

	//os_printf("i:%d,%d\r\n", param->frame_id, g_pkt_seq);

	if (param->is_eof) {
		//os_printf("eof\r\n");
		g_pkt_seq = 0;
	}
}

static int video_buff_set_open(void)
{
	if (g_video_buff == NULL) {
		int ret;
		GLOBAL_INT_DECLARATION();
		video_setup_t setup;

		g_video_buff = (video_buff_t *)os_malloc(sizeof(video_buff_t));
		if (g_video_buff == NULL) {
			os_printf("vbuf init no mem\r\n");
			ret = kNoMemoryErr;
			return ret;
		}

		ret = rtos_init_semaphore(&g_video_buff->aready_semaphore, 1);
		if (ret != kNoErr) {
			os_printf("vbuf init semaph failed\r\n");
			os_free(g_video_buff);
			g_video_buff = NULL;
			return ret;
		}

		GLOBAL_INT_DISABLE();
		g_video_buff->buf_base = NULL;
		g_video_buff->buf_len = 0;

		g_video_buff->frame_len = 0;
		g_video_buff->buf_ptr = NULL;
		g_video_buff->receive_state = VIDEO_BUFF_IDLE;

		g_video_buff->frame_id = 0xffff;
		g_video_buff->frame_pkt_cnt = 0;
		GLOBAL_INT_RESTORE();

		setup.open_type = TVIDEO_OPEN_SCCB;
		setup.send_type = TVIDEO_SND_INTF;
		setup.send_func = video_buffer_recv_data;
		setup.start_cb = NULL;
		setup.end_cb = NULL;

		setup.pkt_header_size = sizeof(vbuf_header_t);
		setup.add_pkt_header = video_buffer_add_pkt_header;

		ret = bk_video_transfer_init(&setup);
		if (ret != kNoErr) {
			os_printf("video_transfer_init failed\r\n");
			rtos_deinit_semaphore(&g_video_buff->aready_semaphore);
			os_free(g_video_buff);
			g_video_buff = NULL;
		}

		os_printf("vbuf opened\r\n");
		return ret;
	}

	return kGeneralErr;
}

static int video_buff_set_close(void)
{
	if (g_video_buff) {
		int ret;
		ret = bk_video_transfer_deinit();
		if (ret != kNoErr) {
			os_printf("video_buffer_close failed\r\n");
			return ret;
		}

		if (g_video_buff->buf_base) {
			// user all video_buffer_read_frame and blocked, so wakeup it
			rtos_set_semaphore(&g_video_buff->aready_semaphore);

			// wait until clear the buf flag
			while (g_video_buff->buf_base);
		}

		rtos_deinit_semaphore(&g_video_buff->aready_semaphore);

		GLOBAL_INT_DECLARATION();

		GLOBAL_INT_DISABLE();
		g_video_buff->aready_semaphore = NULL;
		GLOBAL_INT_RESTORE();

		os_free(g_video_buff);
		g_video_buff = NULL;
		os_printf("voide close\r\n");
		return kNoErr;
	}

	os_printf("video not open\r\n");
	return kGeneralErr;
}

static int video_buff_read_image(void)
{
	uint32_t frame_len = 0;
	GLOBAL_INT_DECLARATION();

	if (g_video_buff && (g_video_buff->buf_base == NULL))
	{
		int ret;
		uint32_t timeout;

		// try to get semaphore, clear send by the previous frame
		while (rtos_get_semaphore(&g_video_buff->aready_semaphore, 0) == kNoErr);

		GLOBAL_INT_DISABLE();
		g_video_buff->buf_base = (uint8_t *)EJPEG_DATA_ADDR;
		g_video_buff->receive_state = VIDEO_BUFF_IDLE;
		g_video_buff->buf_len = 0xF000;
		GLOBAL_INT_RESTORE();

		timeout = BEKEN_WAIT_FOREVER;

		ret = rtos_get_semaphore(&g_video_buff->aready_semaphore, timeout);
		if (ret == kNoErr) {
			frame_len = g_video_buff->frame_len;
		} else {
			os_printf("read frame time out\r\n");
			frame_len = 0;
		}

		GLOBAL_INT_DISABLE();
		g_video_buff->buf_base = NULL;
		g_video_buff->buf_len = 0;
		GLOBAL_INT_RESTORE();
	}

	return frame_len;
}

int video_set_camera_enable_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	uint32_t mode = 0x00054043;

	if (argc != 1)
	{
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}

	if(!os_strcmp(argv[0], "1")) {
		err = bk_psram_init(mode);
		if (err != kNoErr) {
			os_printf("psram init failed\r\n");
			goto error;
		}

		err = video_buff_set_open();
	} else if(!os_strcmp(argv[0], "0")) {
		err = video_buff_set_close();
		if (err != kNoErr) {
			os_printf("video close failed\r\n");
			goto error;
		}

		delay(2000);
		err = bk_psram_deinit();
	} else {
		os_printf("input first param error\n");
		err = kParamErr;
		goto error;
	}

	if (err == kNoErr) {
		msg = AT_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return err;
	}

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}

int video_take_photo_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	FIL fp1;
	char *file_path = "image.jpg";
	uint8_t file_id = 0;
	char cFileName[50];
	unsigned int uiTemp = 0;
	uint32_t addr = EJPEG_DATA_ADDR;
	uint32_t frame_len = 0;

	if (argc != 1)
	{
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}

	file_id = os_strtoul(argv[0], NULL, 10) & 0xFF;

	sprintf(cFileName, "%d:/%d_%s", DISK_NUMBER_SDIO_SD, file_id, file_path);

	FRESULT fr = f_open(&fp1, cFileName, FA_OPEN_APPEND | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("can not open file:%s!\n", cFileName);
		err = fr;
		goto error;
	}

	frame_len = video_buff_read_image();
	if (frame_len == 0) {
		os_printf("read image failed!!!\n");
		goto error;
	} else {
		fr = f_write(&fp1, (char *)addr, frame_len, &uiTemp);
	}

	fr = f_close(&fp1);
	if (fr != FR_OK) {
		os_printf("can not close file:%s!\n", file_path);
		err = fr;
		goto error;
	}

	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return kNoErr;

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}
#endif //(CONFIG_CAMERA && CONFIG_APP_DEMO_VIDEO_TRANSFER)

#if CONFIG_PSRAM
int video_read_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	uint8_t i = 0;

	uint32_t psram = 0x60000000;
	uint32_t mode = 0x00054043;
	if (argc != 0) {
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}

	err = bk_psram_init(mode);
	if (err != kNoErr) {
		os_printf("psram init error\n");
		err = kParamErr;
		goto error;
	}

	os_memset((uint8_t *)psram, 0, 30);

	for (i = 0; i < 30; i++) {
		*((uint8_t *)psram + i) = i;
	}

	os_printf("data:\n");
	for (i = 0; i <30; i++) {
		os_printf("%d ", *((uint8_t *)psram + i));
	}
	os_printf("\n");

	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return kNoErr;

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}

int video_disable_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;

	if (argc != 0) {
			os_printf("input param error\n");
			err = kParamErr;
			goto error;
	}

	err = bk_psram_deinit();
	if (err != kNoErr) {
		os_printf("psram deinit error\n");
		err = kParamErr;
		goto error;
	}

	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return kNoErr;

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}
#endif // CONFIG_PSRAM

#if CONFIG_CAMERA
static void end_of_jpeg_frame(jpeg_unit_t id, void *param)
{
	//os_printf("%s, %d\n", __func__, __LINE__);
}

static void end_of_yuv_frame(jpeg_unit_t id, void *param)
{
	//os_printf("%s, %d\n", __func__, __LINE__);
}


int video_set_yuv_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	uint8_t yuv_mode = 0;
	uint16_t display_pixel = 0;
	jpeg_config_t jpeg_config = {0};
	i2c_config_t i2c_config = {0};
	uint32_t ppi, fps;
	uint32_t psram_mode = 0x00054043;

	if (argc != 4) {
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}

	yuv_mode = os_strtoul(argv[0], NULL, 16) & 0xFF;
	if (yuv_mode > 1) {
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}
	err = bk_psram_init(psram_mode);
	if (err != kNoErr) {
		os_printf("psram init error\n");
		err = kParamErr;
		goto error;
	}

#if CONFIG_SYSTEM_CTRL
	err = bk_jpeg_enc_driver_init();
	if (err != kNoErr) {
		os_printf("video init error\n");
		err = kParamErr;
		goto error;
	}
#endif

	display_pixel = os_strtoul(argv[1], NULL, 10) & 0xFFFF;
	switch (display_pixel) {
		case 240:
			jpeg_config.x_pixel = X_PIXEL_320;
			jpeg_config.y_pixel = Y_PIXEL_240;
			ppi = QVGA_320_240;
			break;
		case 272:
			jpeg_config.x_pixel = X_PIXEL_480;
			jpeg_config.y_pixel = Y_PIXEL_272;
			ppi = VGA_480_272;
			break;
		case 480:
			jpeg_config.x_pixel = X_PIXEL_640;
			jpeg_config.y_pixel = Y_PIXEL_480;
			ppi = VGA_640_480;
			break;
		case 600:
			jpeg_config.x_pixel = X_PIXEL_800;
			jpeg_config.y_pixel = Y_PIXEL_600;
			ppi = VGA_800_600;
			break;
		case 720:
			jpeg_config.x_pixel = X_PIXEL_1280;
			jpeg_config.y_pixel = Y_PIXEL_720;
			ppi = VGA_1280_720;
			break;
		default:
			os_printf("input pixel param error\n");
			err = kParamErr;
			goto error;
	}

	fps = os_strtoul(argv[2], NULL, 10) & 0xFF;
	switch (fps) {
		case 5:
			fps = TYPE_5FPS;
			break;
		case 10:
			fps = TYPE_10FPS;
			break;
		case 15:
			fps = TYPE_15FPS;
			break;
		case 20:
			fps = TYPE_20FPS;
			break;
		case 25:
			fps = TYPE_25FPS;
			break;
		case 30:
			fps = TYPE_30FPS;
			break;
		default:
			os_printf("input fps param error\n");
			err = kParamErr;
			goto error;
	}

	jpeg_config.yuv_mode = yuv_mode;

	if (ppi == VGA_1280_720) {
		jpeg_config.sys_clk_div = 3;
		jpeg_config.mclk_div = 2;
	}else {
		jpeg_config.sys_clk_div = 3;
		jpeg_config.mclk_div = 0;
	}

	err = bk_jpeg_enc_dvp_init(&jpeg_config);
	if (err != kNoErr) {
		os_printf("jpeg init error\n");
		err = kParamErr;
		goto error;
	}

	bk_jpeg_enc_register_isr(END_OF_FRAME, end_of_jpeg_frame, NULL);
	bk_jpeg_enc_register_isr(END_OF_YUV, end_of_yuv_frame, NULL);

	i2c_config.baud_rate = os_strtoul(argv[3], NULL, 10) & 0xFFFFFFFF;//100000;// 400k
	i2c_config.addr_mode = 0;
	err = bk_i2c_init(CONFIG_CAMERA_I2C_ID, &i2c_config);
	if (err != kNoErr) {
		os_printf("i2c init error\n");
		err = kParamErr;
		goto error;
	}

	err = bk_camera_set_ppi_fps(ppi, fps);
	if (err != kNoErr) {
		os_printf("set camera ppi and fps error\n");
		err = kParamErr;
		goto error;
	}

	bk_camera_sensor_config();
	os_printf("camera init ok\n");

	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));

	return kNoErr;

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}

int video_close_yuv_psram_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;

	err = bk_jpeg_enc_dvp_deinit();
	if (err != kNoErr) {
		os_printf("jpeg deinit error\n");
		err = kParamErr;
		goto error;
	}
	os_printf("jpeg deinit ok!\n");

	err = bk_i2c_deinit(CONFIG_CAMERA_I2C_ID);
	if (err != kNoErr) {
		os_printf("i2c deinit error\n");
		err = kParamErr;
		goto error;
	}
	os_printf("I2c deinit ok!\n");

#if CONFIG_SYSTEM_CTRL
	err = bk_jpeg_enc_driver_deinit();
	if (err != kNoErr) {
		os_printf("video deinit error\n");
		err = kParamErr;
		goto error;
	}
	os_printf("video deinit ok!\n");
#endif

	delay(2000);
	err = bk_psram_deinit();
	if (err != kNoErr) {
		os_printf("psram deinit error\n");
		err = kParamErr;
		goto error;
	}
	os_printf("psram deinit ok!\n");

	msg = AT_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return kNoErr;

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}
#endif // CONFIG_CAMERA


#include "dvp_capture.h"
#include <driver/psram.h>
#include <driver/dma.h>
#include <driver/jpeg_enc.h>
#include <driver/i2c.h>
#include <driver/timer.h>
#include <components/video_transfer.h>
#include <components/dvp_camera.h>

#if (CONFIG_SDCARD_HOST)
#include "ff.h"
#include "diskio.h"
#endif

#if CONFIG_GENERAL_DMA
#include "bk_general_dma.h"
#endif


#define EJPEG_DATA_ADDR                (0x60010000) // PSRAM

extern void delay(int num);//TODO fix me

//#if (CONFIG_CAMERA && CONFIG_APP_DEMO_VIDEO_TRANSFER && (CONFIG_SDCARD_HOST || CONFIG_USB_HOST))

static void dvp_help(void)
{
	os_printf("dvp_image {init|capture file_id|set_cfg|log|save_image|video_transfer|deinit}\r\n");
}

#if (CONFIG_PSRAM)
static video_buff_t *g_video_buff = NULL;
static uint32_t g_pkt_seq = 0;
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

		/*os_printf("%d-%d: %d-%d: %d\r\n", hdr->id, g_video_buff->frame_id,
			hdr->pkt_seq, (g_video_buff->frame_pkt_cnt + 1), g_video_buff->receive_state);*/

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
					os_printf("crc:%02x, %02x, %02x, %02x\r\n", crc_ptr[0], crc_ptr[1], crc_ptr[2], crc_ptr[3]);*/
					p_len = crc_ptr[0] + (crc_ptr[1] << 8)
						+ (crc_ptr[2] << 16) + (crc_ptr[3] << 24);
					os_printf("p_len:%d, frame_len:%d\r\n", p_len, g_video_buff->frame_len);

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
					}
				}
			} else {
				os_printf("vbuf full!\r\n");
				GLOBAL_INT_DISABLE();
				g_video_buff->receive_state = VIDEO_BUFF_FULL;
				GLOBAL_INT_RESTORE();
				// all frame data not have received, psram not enough
				rtos_set_semaphore(&g_video_buff->aready_semaphore);
			}
		}
		return len;
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
#endif

#if defined(CONFIG_SDCARD_HOST) && defined(CONFIG_PSRAM)
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
		g_video_buff->buf_len = 0x3F000;
		GLOBAL_INT_RESTORE();

		timeout = 2000;// 2 second

		ret = rtos_get_semaphore(&g_video_buff->aready_semaphore, timeout);
		if (ret == kNoErr) {
			frame_len = g_video_buff->frame_len;
		} else {
			os_printf("capture time out\r\n");
			frame_len = 0;
		}

		GLOBAL_INT_DISABLE();
		g_video_buff->buf_base = NULL;
		g_video_buff->buf_len = 0;
		GLOBAL_INT_RESTORE();
	}

	return frame_len;
}
#endif


void image_save_dvp(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t err;

	if (argc < 2) {
		dvp_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
#if (CONFIG_PSRAM)
		err = bk_psram_init();
		if (err != kNoErr) {
			os_printf("psram init failed\r\n");
			return;
		}
		err = video_buff_set_open();
		if (err != kNoErr) {
			os_printf("camera init failed\r\n");
			return;
		}
#else
		os_printf("Not Support, PSRAM NOT support!\r\n");
#endif
	} else if(os_strcmp(argv[1], "deinit") == 0) {
#if (CONFIG_PSRAM)
		err = video_buff_set_close();
		if (err != kNoErr) {
			os_printf("video close failed\r\n");
			return;
		}

		delay(2000);
		err = bk_psram_deinit();
		if (err != BK_OK) {
			os_printf("vpsram deinit failed\r\n");
			return;
		}
#else
		os_printf("Not Support, PSRAM NOT support!\n");
#endif
	} else if (os_strcmp(argv[1], "capture") == 0) {
#if (CONFIG_SDCARD_HOST) && (CONFIG_PSRAM)
		FIL fp1;
		char *file_path = "dvp.jpg";
		uint8_t file_id = 0;
		char cFileName[50];
		unsigned int uiTemp = 0;
		uint32_t addr = EJPEG_DATA_ADDR;
		uint32_t frame_len = 0;

		if (argc != 3)
		{
			os_printf("input param error\n");
			return;
		}

		file_id = os_strtoul(argv[2], NULL, 10) & 0xFF;

		sprintf(cFileName, "%d:/%d_%s", DISK_NUMBER_SDIO_SD, file_id, file_path);

		FRESULT fr = f_open(&fp1, cFileName, FA_CREATE_ALWAYS | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("can not open file:%s!\n", cFileName);
			return;
		}
		os_printf("open file:%s!\n", cFileName);

		frame_len = video_buff_read_image();
		if (frame_len == 0) {
			os_printf("read image failed!!!\n");
			goto error1;
		} else {
			fr = f_write(&fp1, (char *)addr, frame_len, &uiTemp);
			if (fr != FR_OK) {
				os_printf("f_write failed 1 fr = %d\r\n", fr);
				goto error1;
			}
		}

		f_close(&fp1);
		return;

error1:
		fr = f_close(&fp1);
		if (fr != FR_OK) {
			os_printf("can not close file:%s!\n", file_path);
		}
		video_buff_set_close();
#else
		os_printf("Not Support, SDcard not support!\r\n");
#endif
	} else if (os_strcmp(argv[1], "set_cfg") == 0) {
		uint32_t dev = 0;
		uint32_t camera_cfg = 0;

		if (argc != 5) {
			os_printf("input param error\n");
			return;
		}

		dev = os_strtoul(argv[2], NULL, 10);
		camera_cfg = (os_strtoul(argv[3], NULL, 10) << 16) | os_strtoul(argv[4], NULL, 10);
		err = bk_camera_set_param(dev, camera_cfg);
		if (err != kNoErr) {
			os_printf("set camera ppi and fps error\n");
			return;
		}
	} else if (os_strcmp(argv[1], "auto_encode") == 0) {
		uint8_t auto_enable = 0;
		uint32_t up_size = 0, low_size = 0;

		auto_enable = os_strtoul(argv[2], NULL, 10) & 0xF;
		if (auto_enable)
		{
			up_size = os_strtoul(argv[3], NULL, 10) * 1024;
			low_size = os_strtoul(argv[4], NULL, 10) * 1024;
		}

		bk_camera_set_config(auto_enable, up_size, low_size);
		os_printf("set OK!\r\n");

	} else {
		dvp_help();
	}
}


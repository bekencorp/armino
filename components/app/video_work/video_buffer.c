#include <common/bk_include.h>

#if (CONFIG_APP_DEMO_VIDEO_TRANSFER)
#include "video_transfer_config.h"

// this moudule only support camera in chip, with sccb interface.
#if ((CONFIG_CAMERA) && (APP_DEMO_CFG_USE_VIDEO_BUFFER))
//#include "FreeRTOS.h"
//#include "task.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>

#include "bk_uart.h"
#include <os/mem.h>
#include <os/str.h>

#include <components/video_transfer.h>

#if CONFIG_GENERAL_DMA
#include "bk_general_dma.h"
#endif

typedef struct vbuf_hdr_st {
	uint8_t id;
	uint8_t is_eof;
	uint8_t pkt_cnt;
	uint8_t pkt_seq;
} VB_HDR_ST, *VB_HDR_PTR;

#define BUF_STA_INIT        0
#define BUF_STA_COPY        1
#define BUF_STA_GET         2
#define BUF_STA_FULL        3
#define BUF_STA_ERR         4

typedef struct video_buffer_st {
	beken_semaphore_t aready_semaphore;

	uint8_t *buf_base;  // handler in usr thread
	uint32_t buf_len;

	uint32_t frame_id;
	uint32_t frame_pkt_cnt;

	uint8_t *buf_ptr;
	uint32_t frame_len;
	uint32_t start_buf;
} VBUF_ST, *VBUF_PTR;

VBUF_PTR g_vbuf = NULL;
static uint32_t g_pkt_seq = 0;

static void video_buffer_add_pkt_header(video_packet_t *param)
{
	VB_HDR_PTR elem_tvhdr = (VB_HDR_PTR)param->ptk_ptr;

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

static int video_buffer_recv_video_data(uint8_t *data, uint32_t len)
{
	if ((g_vbuf->buf_base) && ((BUF_STA_COPY == g_vbuf->start_buf) || (BUF_STA_INIT == g_vbuf->start_buf ))) {
		VB_HDR_PTR hdr = (VB_HDR_PTR)data;
		uint32_t org_len, left_len;
		GLOBAL_INT_DECLARATION();

		if (len < sizeof(VB_HDR_ST)) {
			os_printf("unknow err!\r\n");
			return len;
		}

		org_len = len - sizeof(VB_HDR_ST);
		data = data + sizeof(VB_HDR_ST);

		if ((hdr->id != g_vbuf->frame_id) && (hdr->pkt_seq == 1)) {
			// start of frame;
			GLOBAL_INT_DISABLE();
			g_vbuf->frame_id = hdr->id;
			g_vbuf->frame_len = 0;
			g_vbuf->frame_pkt_cnt = 0;
			g_vbuf->buf_ptr = g_vbuf->buf_base;
			g_vbuf->start_buf = BUF_STA_COPY;
			GLOBAL_INT_RESTORE();
			//os_printf("sof:%d\r\n", g_vbuf->frame_id);
		}

		//os_printf("%d-%d: %d-%d: %d\r\n", hdr->id, g_vbuf->frame_id,
		//    hdr->pkt_seq, (g_vbuf->frame_pkt_cnt + 1), g_vbuf->start_buf);

		if ((hdr->id == g_vbuf->frame_id)
			&& ((g_vbuf->frame_pkt_cnt + 1) == hdr->pkt_seq)
			&& (g_vbuf->start_buf == BUF_STA_COPY)) {
			left_len = g_vbuf->buf_len - g_vbuf->frame_len;
			if (org_len <= left_len) {
#if CONFIG_GENERAL_DMA
				dma_memcpy(g_vbuf->buf_ptr, data, org_len);
#else
				os_memcpy(g_vbuf->buf_ptr, data, org_len);
#endif

				GLOBAL_INT_DISABLE();
				g_vbuf->frame_len += org_len;
				g_vbuf->buf_ptr += org_len;
				g_vbuf->frame_pkt_cnt += 1;
				GLOBAL_INT_RESTORE();

				if (hdr->is_eof == 1) {
					uint8_t *sof_ptr, *eof_ptr, *crc_ptr;
					uint32_t p_len, right_image = 0;

					sof_ptr = g_vbuf->buf_base;
					eof_ptr = g_vbuf->buf_base + (g_vbuf->frame_len - 7);
					crc_ptr = eof_ptr + 3;

					if (((sof_ptr[0] == 0xff) && (sof_ptr[1] == 0xd8)) &&
						((eof_ptr[0] == 0xff) && (eof_ptr[1] == 0xd9))) {
						p_len = crc_ptr[0] + (crc_ptr[1] << 8)
								+ (crc_ptr[2] << 16) + (crc_ptr[3] << 24);

						//os_printf("vb,len:%d - %d\r\n", p_len, (g_vbuf->frame_len - 5));
						if (p_len == (g_vbuf->frame_len - 5))
							right_image = 1;
					}

					if (right_image) {
						//os_printf("set ph\r\n");
						GLOBAL_INT_DISABLE();
						g_vbuf->start_buf = BUF_STA_GET;
						GLOBAL_INT_RESTORE();
					} else {
						GLOBAL_INT_DISABLE();
						g_vbuf->start_buf = BUF_STA_ERR;
						GLOBAL_INT_RESTORE();
					}
					// all frame data have received, wakeup usr thread
					rtos_set_semaphore(&g_vbuf->aready_semaphore);

				}
			} else {
				os_printf("vbuf full!\r\n");
				GLOBAL_INT_DISABLE();
				g_vbuf->start_buf = BUF_STA_FULL;
				GLOBAL_INT_RESTORE();
				rtos_set_semaphore(&g_vbuf->aready_semaphore);
			}
		}

		return len;
	} else {
		// keep data
		//os_printf("kp\r\n");
		return len;
	}
}

bk_err_t bk_video_buffer_open(void)
{
	if (g_vbuf == NULL) {
		int ret = kNoErr;
		GLOBAL_INT_DECLARATION();
		video_setup_t setup;

		g_vbuf = (VBUF_PTR)os_malloc(sizeof(VBUF_ST));
		if (g_vbuf == NULL) {
			os_printf("vbuf init no mem\r\n");
			ret = kNoMemoryErr;
			return ret;
		}

		if (rtos_init_semaphore(&g_vbuf->aready_semaphore, 1) != kNoErr) {
			os_printf("vbuf init semaph failed\r\n");
			os_free(g_vbuf);
			g_vbuf = NULL;
			ret = kGeneralErr;
			return ret;
		}

		GLOBAL_INT_DISABLE();
		g_vbuf->buf_base = NULL;
		g_vbuf->buf_len = 0;

		g_vbuf->frame_len = 0;
		g_vbuf->buf_ptr = NULL;
		g_vbuf->start_buf = BUF_STA_INIT;

		g_vbuf->frame_id = 0xffff;
		g_vbuf->frame_pkt_cnt = 0;
		GLOBAL_INT_RESTORE();

		setup.open_type = TVIDEO_OPEN_SCCB;
		setup.send_type = TVIDEO_SND_INTF;
		setup.send_func = video_buffer_recv_video_data;
		setup.start_cb = NULL;
		setup.end_cb = NULL;

		setup.pkt_header_size = sizeof(VB_HDR_ST);
		setup.add_pkt_header = video_buffer_add_pkt_header;

		ret = bk_video_transfer_init(&setup);
		if (ret != kNoErr) {
			os_printf("video_transfer_init failed\r\n");
			rtos_deinit_semaphore(&g_vbuf->aready_semaphore);
			os_free(g_vbuf);
			g_vbuf = NULL;
			return kOpenErr;
		}

		os_printf("vbuf opened\r\n");
	}

	return kNoErr;
}

bk_err_t bk_video_buffer_close(void)
{
	if (g_vbuf) {
		int ret = kNoErr;
		os_printf("voide close\r\n");

		ret = bk_video_transfer_deinit();
		if (ret != kNoErr) {
			os_printf("video_buffer_close failed\r\n");
			return ret;
		}

		if (g_vbuf->buf_base) {
			do {
				// user all video_buffer_read_frame and blocked, so wakeup it
				rtos_set_semaphore(&g_vbuf->aready_semaphore);

				// wait until clear the buf flag
				rtos_delay_milliseconds(10);
			} while (g_vbuf->buf_base);
		}

		rtos_deinit_semaphore(&g_vbuf->aready_semaphore);

		GLOBAL_INT_DECLARATION();

		GLOBAL_INT_DISABLE();
		g_vbuf->aready_semaphore = NULL;
		GLOBAL_INT_RESTORE();

		os_free(g_vbuf);
		g_vbuf = NULL;
	}

	return kNoErr;
}

// err_code:
//  0: success,
// -1: param error, -2: buffer full, -3: frame data err, -4: timeout, -5: unknow err
uint32_t bk_video_buffer_read_frame(uint8_t *buf, uint32_t buf_len, int *err_code, uint32_t timeout)
{
	uint32_t frame_len = 0, err = -5;
	GLOBAL_INT_DECLARATION();

	if ((buf == NULL) || (buf_len == 0)) {
		if(err_code)
			*err_code = -1;
		return 0;
	}

	if (g_vbuf && (g_vbuf->buf_base == NULL)) {
		int ret;

		// try to get semaphore, clear send by the previous frame
		while (rtos_get_semaphore(&g_vbuf->aready_semaphore, 0) == kNoErr);

		GLOBAL_INT_DISABLE();
		g_vbuf->buf_base = buf;
		g_vbuf->buf_len = buf_len;
		GLOBAL_INT_RESTORE();

		ret = rtos_get_semaphore(&g_vbuf->aready_semaphore, timeout);
		if(ret == kNoErr)  {
			if(BUF_STA_GET == g_vbuf->start_buf) {
				frame_len = g_vbuf->frame_len;
				err = 0;
			} else if(BUF_STA_FULL == g_vbuf->start_buf) {
				os_printf("read frame full\r\n");
				err = -2;
			} else if(BUF_STA_ERR == g_vbuf->start_buf) {
				os_printf("read frame data err\r\n");
				err = -3;
			} else {
				os_printf("read frame unknow err\r\n");
				err = -4;
			}
		} else {
			os_printf("read frame timeout :%d\r\n", timeout);
			err = -5;
		}

		*err_code = err;

		GLOBAL_INT_DISABLE();
		g_vbuf->start_buf = BUF_STA_INIT;
		g_vbuf->buf_base = NULL;
		g_vbuf->buf_len = 0;
		GLOBAL_INT_RESTORE();
	}

	return frame_len;
}

#define VIDEO_BUFFER_CMD        1

#if VIDEO_BUFFER_CMD
#include "string.h"
#include "stdlib.h"

void video_buffer(int argc, char **argv)
{
	if (strcmp(argv[1], "open") == 0)
		bk_video_buffer_open();
	else if (strcmp(argv[1], "read") == 0)

	{
		uint8_t *mybuf;
		uint32_t my_len, get_len = 0;
		int get_ret = 0;

		my_len = atoi(argv[2]);
		mybuf = os_malloc(my_len);

		if (mybuf == NULL) {
			os_printf("vbuf test no buff\r\n");
			return;
		}

		get_len = bk_video_buffer_read_frame(mybuf, my_len, &get_ret, BEKEN_WAIT_FOREVER);
		os_printf("get frame ret: %d, len:%d\r\n", get_ret, get_len);

		if(get_ret != 0) {
			for (int i = 0; i < get_len; i++) {
				os_printf("%02x,", mybuf[i]);
				if ((i + 1) % 32 == 0)
					os_printf("\r\n");
			}
		} else if((get_ret == -2) || (get_ret == -3)) {
			os_printf("full or data err, retry? \r\n");
		}
		os_free(mybuf);
	} else if (strcmp(argv[1], "close") == 0) {
		bk_video_buffer_close();
	} else
		os_printf("vbuf open/read len/close/\r\n");
}

void video_buffer_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	video_buffer(argc, argv);
}
#if CONFIG_RTT
FINSH_FUNCTION_EXPORT_ALIAS(video_buffer, __cmd_video_buffer, video buffer);
#endif
#else // VIDEO_BUFFER_CMD
void video_buffer_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{

}
#endif // VIDEO_BUFFER_CMD

#endif // ((CONFIG_CAMERA) && (APP_DEMO_CFG_USE_VIDEO_BUFFER))
#endif //#if (CONFIG_APP_DEMO_VIDEO_TRANSFER)


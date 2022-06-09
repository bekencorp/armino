/*------------------------------------------------------------------------/
/  The Main Development Bench of TJpgDec Module 
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2011, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/

#include "tjpgd.h"
#include <components/jpeg_decode.h>

#define MODE	1	/* Test mode: 0:Show decmpression status, 1:and output in BMP */
#define SCALE	0	/* Output scaling 0:1/1, 1:1/2, 2:1/4 or 3:1/8 */
#define WORK_AREA_SIZE 4096

/*---------------------------------*/
/* User defined session identifier */
/*---------------------------------*/

typedef struct {
	uint8_t scale_ratio;/* Output scaling 0:1/1, 1:1/2, 2:1/4*/
	uint16_t width;
	uint16_t height;
	uint16_t line_wbyte;
	uint32_t rd_ptr;
	uint32_t jpg_file_size;//jpg file size
	uint8_t *outputbuf;//jpg_dec data
	uint8_t *inputbuf;//jpg data
	uint8_t *workbuf;
	void (*jpg_dec_end_isr)(void);
}jpeg_dec_t;


static jpeg_dec_t jpeg_dec_st = {0};


/*-----------------------------*/
/* User defined input function */
/*-----------------------------*/

uint16_t input_func (	/* Returns number of bytes read (zero on error) */
	JDEC* jd,		/* Decompression object */
	uint8_t* buff,	/* Pointer to the read buffer (null to remove data) */
	uint16_t ndata	/* Number of bytes to read/skip */
)
{
	if (jpeg_dec_st.rd_ptr > jpeg_dec_st.jpg_file_size)
	{
		return 0xffff;
	}
	if (NULL != buff)
	{
		os_memcpy(buff, jpeg_dec_st.inputbuf + jpeg_dec_st.rd_ptr,ndata);
	}

	jpeg_dec_st.rd_ptr += ndata;
	return ndata;

}

/*------------------------------*/
/* User defined output function */
/*------------------------------*/

int output_func (	/* 1:Ok, 0:Aborted */
	JDEC* jd,		/* Decompression object */
	void* bitmap,	/* Bitmap data to be output */
	JRECT* rect		/* Rectangular region to output */
)
{
	uint32_t nx, ny, xc, wd;
	uint8_t *src, *dst;
	uint8_t unit = 0;

	if (JD_FORMAT == 0) {
		unit = 3;		/* Byte width of a pixel */
	}
	else if (JD_FORMAT == 1 || JD_FORMAT == 3) {
		unit = 2;		/* Byte width of a pixel */
	}
	else if (JD_FORMAT == 2) {
		unit = 1;
	}
	else {
		os_printf("JD_FORMAT error!\r\n");
		return JDR_FMT1;
	}

	nx = rect->right - rect->left + 1;
	ny = rect->bottom - rect->top + 1;	/* Number of lines of the rectangular */
	src = (uint8_t*)bitmap;				/* RGB bitmap to be output */

	wd = jpeg_dec_st.line_wbyte;							/* Number of bytes a line of the frame buffer */
	dst = jpeg_dec_st.outputbuf + rect->top * wd + rect->left * unit;	/* Left-top of the destination rectangular in the frame buffer */

	do {	/* Copy the rectangular to the frame buffer */
		xc = nx;
		do {
			if (JD_FORMAT == 2) {	/* Grayscale output */
				*dst++ = *src++;
			} else if (JD_FORMAT == 1) {	/* RGB565 output */
				*dst++ = *src++;
				*dst++ = *src++;
			} else if (JD_FORMAT == 0) {				/* RGB888 output */
				*dst++ = *src++;
				*dst++ = *src++;
				*dst++ = *src++;
			} else if (JD_FORMAT == 3) {
				*dst++ = *src++;//Y1
				*dst++ = *src++;//Y2
				*dst++ = *src++;//U
				*dst++ = *src++;//V
				--xc;
			}
		} while (--xc);
		dst += wd - nx * unit;
	} while (--ny);

	return 1;	/* Continue to decompress */
}


/*------------------------------*/
/* Decompress a JPEG stream     */
/*------------------------------*/

bk_err_t bk_jpeg_dec_sw_init(uint8_t *src_buf, uint8_t *dst_buf)
{
	jpeg_dec_st.scale_ratio = SCALE;
	jpeg_dec_st.inputbuf = src_buf;
	jpeg_dec_st.outputbuf = dst_buf;
	jpeg_dec_st.rd_ptr = 0;
	jpeg_dec_st.jpg_file_size = 1024;
	jpeg_dec_st.workbuf = os_malloc(WORK_AREA_SIZE);
	if(NULL == jpeg_dec_st.workbuf)
	{
		os_printf("work area error!!\r\n");
		return kNoMemoryErr;
	}
	return kNoErr;
}

bk_err_t bk_jpeg_dec_sw_start(uint32_t frame_size)
{
	bk_err_t ret = BK_OK;
	uint32_t xb, xs, ys;
	JDEC jd;		/* TJpgDec decompression object */

	jpeg_dec_st.rd_ptr = 0;
	jpeg_dec_st.jpg_file_size = frame_size;
	ret = jd_prepare_sw(&jd, input_func, jpeg_dec_st.workbuf, WORK_AREA_SIZE, NULL);

	if (ret == JDR_OK) {
		//os_printf(",%u,%u", jd.width, jd.height);	/* Image dimensions */
		//os_printf(",%s", jd.ncomp == 1 ? "4:0:0" : jd.msx == 1 ? "4:4:4" : jd.msy == 1 ? "4:2:2" : "4:2:0");	/* Sampling ratio */
		//os_printf(",%u", jd.nrst);					/* Restart interval */
		//os_printf(",%u", 4096 - jd.sz_pool);	/* Used memory size */

		/* Initialize frame buffer */
		xs = jd.width >> SCALE;		/* Image size to output */
		ys = jd.height >> SCALE;

		jpeg_dec_st.width = xs;
		jpeg_dec_st.height = ys;

		if (JD_FORMAT == 0) {
			xb = (xs * 3 + 3) & ~3;		/* Byte width of the frame buffer */
		}
		else if (JD_FORMAT == 1 || JD_FORMAT == 3) {
			xb = xs * 2;		/* Byte width of the frame buffer */
		}
		else if (JD_FORMAT == 2) {
			xb = xs;
		}
		else {
			os_printf("JD_FORMAT error!\r\n");
			ret = JDR_FMT1;
			return ret;
		}
		
		jpeg_dec_st.line_wbyte = xb;

		/* Start JPEG decompression */
		ret = jd_decomp_sw(&jd, output_func, SCALE);

		if (ret == JDR_OK) {
			if (jpeg_dec_st.jpg_dec_end_isr != NULL) {
				jpeg_dec_st.jpg_dec_end_isr();
			}
		} else {
			os_printf("jpeg decode error %d\r\n", ret);
		}
	} else {	/* Error occured on prepare */
			os_printf("jpeg decode prepare error %d\r\n", ret);
	}

	return ret;
}

bk_err_t bk_jpeg_dec_sw_deinit(void)
{
	if(NULL != jpeg_dec_st.workbuf) {
		os_free(jpeg_dec_st.workbuf);
		jpeg_dec_st.workbuf = NULL;
	}
	os_memset(&jpeg_dec_st, 0, sizeof(jpeg_dec_t));
	return kNoErr;
}

void bk_jpeg_dec_sw_register_finish_callback(void *cb)
{
	jpeg_dec_st.jpg_dec_end_isr = cb;
}


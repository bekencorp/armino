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
#pragma GCC optimize ("O3")

#include "tjpgd.h"
#include <components/jpeg_decode.h>

#define MODE	1	/* Test mode: 0:Show decmpression status, 1:and output in BMP */
#define SCALE	0	/* Output scaling 0:1/1, 1:1/2, 2:1/4 or 3:1/8 */
#define WORK_AREA_SIZE 10240

/*---------------------------------*/
/* User defined session identifier */
/*---------------------------------*/
/** jpeg dec isr return value */


typedef struct {
	uint16_t scale_ratio;/* Output scaling 0:1/1, 1:1/2, 2:1/4*/
	uint16_t width;
	uint16_t height;
	uint16_t line_wbyte;
	uint32_t rd_ptr;
	uint32_t jpg_file_size;//jpg file size
	uint8_t *outputbuf;//jpg_dec data
	uint8_t *inputbuf;//jpg data
	uint8_t *workbuf;
	void (*jpg_dec_end_isr)(sw_jpeg_dec_res_t *result);
}jpeg_dec_t;


JPEG_DTCM static jpeg_dec_t jpeg_dec_st = {0};
JPEG_DTCM static uint8_t jpeg_dec_out_fomat = JDF_VYUY;
JPEG_DTCM static bool s_jpegdec_is_init = false;

#if 0
JPEG_DTCM static uint8_t jpeg_dec_work_buff[WORK_AREA_SIZE] = {0};
#endif

/*-----------------------------*/
/* User defined input function */
/*-----------------------------*/

JPEG_ITCM size_t input_func (	/* Returns number of bytes read (zero on error) */
	JDEC* jd,		/* Decompression object */
	uint8_t* buff,	/* Pointer to the read buffer (null to remove data) */
	size_t ndata	/* Number of bytes to read/skip */
)
{
	uint32_t data_len = ndata;
	uint32_t buff_end = (uint32_t)jpeg_dec_st.inputbuf + jpeg_dec_st.jpg_file_size;
	if (jpeg_dec_st.rd_ptr > buff_end)
	{
		return 0;
	}

	if (jpeg_dec_st.rd_ptr + ndata > buff_end) {
		data_len = buff_end - jpeg_dec_st.rd_ptr;
	}

	if (NULL != buff)
	{
		os_memcpy(buff, jpeg_dec_st.inputbuf + jpeg_dec_st.rd_ptr, data_len);
	} else {
//		os_printf("input_func, buff is null, data_len=%d ndata=%d.\r\n",data_len, ndata);
		if(data_len < ndata) {
			jpeg_dec_st.rd_ptr += data_len;
			return 0;
		}
	}

	jpeg_dec_st.rd_ptr += data_len;
	return data_len;

}

/*------------------------------*/
/* User defined output function */
/*------------------------------*/

JPEG_ITCM int output_func (	/* 1:Ok, 0:Aborted */
	JDEC* jd,		/* Decompression object */
	void* bitmap,	/* Bitmap data to be output */
	JRECT* rect		/* Rectangular region to output */
)
{
	uint32_t nx, ny, xc, wd;
	uint8_t unit = 0;

#if (JD_FORMAT == 1) || (JD_FORMAT == 3) || (JD_FORMAT == 4)
	uint32_t *src_w, *dst_w;
#else
	uint8_t *src, *dst;
#endif

#if (JD_FORMAT == 0)
	{
		unit = 3;		/* Byte width of a pixel */
	}
#elif (JD_FORMAT == 1) || (JD_FORMAT == 3) || (JD_FORMAT == 4)
	{
		unit = 2;		/* Byte width of a pixel */
	}
#elif  (JD_FORMAT == 2)
	{
		unit = 1;
	}
#else
	{
		os_printf("JD_FORMAT error!\r\n");
		return JDR_FMT1;
	}
#endif

	nx = rect->right - rect->left + 1;
	ny = rect->bottom - rect->top + 1;	/* Number of lines of the rectangular */

#if (JD_FORMAT == 1) || (JD_FORMAT == 3) || (JD_FORMAT == 4)
	src_w = (uint32_t*)bitmap;
#else
	src = (uint8_t*)bitmap;				/* RGB bitmap to be output */
#endif

	wd = jpeg_dec_st.line_wbyte;							/* Number of bytes a line of the frame buffer */

#if (JD_FORMAT == 1) || (JD_FORMAT == 3) || (JD_FORMAT == 4)
	dst_w = (uint32_t*)(jpeg_dec_st.outputbuf + rect->top * wd + rect->left * unit);
#else
	dst = jpeg_dec_st.outputbuf + rect->top * wd + rect->left * unit;	/* Left-top of the destination rectangular in the frame buffer */
#endif

	// os_printf("output_func[%d %d %d %d]: dst: %p.\r\n",
	// 			 rect->top, rect->left, rect->bottom, rect->right,dst);
	// os_printf("output_func[nx:%d, ny:%d, wd: %d]: dst: %p.\r\n", nx, ny, wd, dst);

	do {	/* Copy the rectangular to the frame buffer */
		xc = nx;
		do {
#if (JD_FORMAT == 0)
				*dst++ = *src++;
				*dst++ = *src++;
				*dst++ = *src++;
#elif (JD_FORMAT == 2)
				*dst++ = *src++;
#elif (JD_FORMAT == 1) || (JD_FORMAT == 3) || (JD_FORMAT == 4)
				*dst_w++ = *src_w++;
				--xc;
#endif
		} while (--xc);

#if (JD_FORMAT == 1) || (JD_FORMAT == 3) || (JD_FORMAT == 4)
		dst_w += (wd - nx * unit) >> 2;
#else
		dst += wd - nx * unit;
#endif
	} while (--ny);

	return 1;	/* Continue to decompress */
}


/*------------------------------*/
/* Decompress a JPEG stream     */
/*------------------------------*/

bk_err_t bk_jpeg_dec_sw_init(void)
{
	if (s_jpegdec_is_init) {
		os_printf("jpeg decode already init.\r\n");
		return BK_OK;
	}

	jpeg_dec_st.scale_ratio = SCALE;
	jpeg_dec_st.rd_ptr = 0;
	jpeg_dec_st.jpg_file_size = 1024;
#if 0
	jpeg_dec_st.workbuf = &jpeg_dec_work_buff[0];
#else
	if (NULL != jpeg_dec_st.workbuf)
	{
		os_free(jpeg_dec_st.workbuf);
		jpeg_dec_st.workbuf = NULL;
	}

	jpeg_dec_st.workbuf = os_malloc(WORK_AREA_SIZE);
	if(NULL == jpeg_dec_st.workbuf)
	{
		os_printf("work area error!!\r\n");
		return kNoMemoryErr;
	}
#endif
	s_jpegdec_is_init = true;

	return kNoErr;
}

JPEG_ITCM bk_err_t bk_jpeg_dec_sw_start(uint32_t frame_size, uint8_t *src_buf, uint8_t *dst_buf, sw_jpeg_dec_res_t *result)
{
	bk_err_t ret = BK_OK;
	uint32_t xb, xs, ys;
	JDEC jd;		/* TJpgDec decompression object */

	jpeg_dec_st.rd_ptr = 0;
	jpeg_dec_st.inputbuf = src_buf;
	jpeg_dec_st.outputbuf = dst_buf;
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

#if (JD_FORMAT == 0)
		{
			xb = (xs * 3 + 3) & ~3;		/* Byte width of the frame buffer */
		}
#elif (JD_FORMAT == 1) || (JD_FORMAT == 3) || (JD_FORMAT == 4)
		{
			xb = xs * 2;		/* Byte width of the frame buffer */
		}
#elif (JD_FORMAT == 2)
		{
			xb = xs;
		}
#else
		{
			os_printf("JD_FORMAT error!\r\n");
			ret = JDR_FMT1;
			return ret;
		}
#endif
		
		jpeg_dec_st.line_wbyte = xb;

		/* Start JPEG decompression */
		ret = jd_decomp_sw(&jd, output_func, SCALE);

		if (ret == JDR_OK) {
			result->pixel_x = jpeg_dec_st.width;
			result->pixel_y = jpeg_dec_st.height;
			if (jpeg_dec_st.jpg_dec_end_isr != NULL) {
				jpeg_dec_st.jpg_dec_end_isr(result);
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
	if (!s_jpegdec_is_init) {
		os_printf("jpeg decode already deinit\r\n");
		return BK_OK;
	}

	if(NULL != jpeg_dec_st.workbuf)
	{
#if !CONFIG_SLAVE_CORE
		os_free(jpeg_dec_st.workbuf);
#endif
		jpeg_dec_st.workbuf = NULL;
	}

	os_memset(&jpeg_dec_st, 0, sizeof(jpeg_dec_t));
	
	s_jpegdec_is_init = false;
	return kNoErr;
}

void bk_jpeg_dec_sw_register_finish_callback(void *cb)
{
	jpeg_dec_st.jpg_dec_end_isr = cb;
}

bk_err_t bk_jpeg_dec_sw_set_out_format(uint8_t format) {
	if (format < JDF_MAX) {
		jpeg_dec_out_fomat = format;
	} else {
		os_printf("set invalid jpeg decode output format: %d\r\n", format);
		return kGeneralErr;
	}
	return kNoErr;
}

uint8_t bk_jpeg_dec_sw_get_out_format() {
	return jpeg_dec_out_fomat;
}

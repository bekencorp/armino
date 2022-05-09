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

#include <components/jpeg_decode.h>
#include "tjpgd.h"

//#define MODE	0	/* Test mode: 0:JPEG-BMP conversion, 1:Report about each files but output */
//#define SCALE	0	/* Output scaling 0:1/1, 1:1/2, 2:1/4*/
#define WORK_AREA_SIZE 4096

typedef struct {
	uint8_t scale_ratio;/* Output scaling 0:1/1, 1:1/2, 2:1/4*/
	uint16_t width;
	uint16_t heigth;
	uint16_t line_wbyte;
	uint32_t rd_ptr;
	uint32_t jpg_file_size;//jpg file size
	uint8_t *outputbuf;//jpg_dec data
	uint8_t *inputbuf;//jpg data
	uint8_t *workbuf;
	void (*jpg_dec_end_isr)(void);
}jpeg_dec_t;

static jpeg_dec_t jpeg_dec_st;

/* User defined input function */
static uint16_t input_func (
	jpegdec_config_t* jd,		/* Decompression object */
	uint8_t* buff,	/* Pointer to the read buffer (0:skip) */
	uint16_t ndata	/* Number of bytes to read/skip */
)
{
	if(jpeg_dec_st.rd_ptr > jpeg_dec_st.jpg_file_size)
		return 0xffff;
	if(NULL != buff)
		os_memcpy(buff,jpeg_dec_st.inputbuf + jpeg_dec_st.rd_ptr, ndata);
	jpeg_dec_st.rd_ptr += ndata;
	return ndata;

}

/* User defined output function */
static uint16_t output_func (
	jpegdec_config_t* jd,		/* Decompression object */
	void* bitmap,	/* data to be output */
	JRECT* rect		/* Rectangular region to output */
)
{
	uint32_t ny, nbx, xc, ret = 0;
	uint8_t *src, *dst;

	ny = rect->bottom - rect->top + 1;	/* Number of lines of the rectangular */
	src = (uint8_t*)bitmap;
	switch (JD_FORMAT) {
		case 0:
			nbx = (rect->right - rect->left + 1) * 3;
			dst = jpeg_dec_st.outputbuf + rect->top*jpeg_dec_st.line_wbyte + rect->left * 3;
			break;
		case 1:
		case 3:
			nbx = (rect->right - rect->left + 1) * 2;
			dst = jpeg_dec_st.outputbuf + rect->top*jpeg_dec_st.line_wbyte + rect->left * 2;
			break;
		case 2:
			nbx = (rect->right - rect->left + 1);
			dst = jpeg_dec_st.outputbuf + rect->top*jpeg_dec_st.line_wbyte + rect->left;
			break;
		default:
			os_printf("--- pre-decoder error,ret = %x---\r\n",ret);
			return 0;
	}

	
	do {	/* Copy the rectangular to the frame buffer */
		xc = nbx;
		do {
				*dst++ = *src++;
		} while (--xc);
		dst += jpeg_dec_st.line_wbyte - nbx;
	} while (--ny);

	return 1;	/* Continue to decompress */
}

void bk_jpeg_dec_sw_register_finish_callback(void *cb)
{
	jpeg_dec_st.jpg_dec_end_isr = cb;
}

bk_err_t bk_jpeg_dec_sw_init(uint16_t width, uint16_t heigth, uint8_t ratio)
{
	jpeg_dec_st.width = width;
	jpeg_dec_st.heigth = heigth;
	jpeg_dec_st.scale_ratio = ratio;
	jpeg_dec_st.workbuf = os_malloc(WORK_AREA_SIZE);
	if(NULL == jpeg_dec_st.workbuf)
	{
		os_printf("work area error!!\r\n");
		return kNoMemoryErr;
	}
	return kNoErr;
}

bk_err_t bk_jpeg_dec_sw_deinit(void)
{
	if(NULL != jpeg_dec_st.workbuf)
		os_free(jpeg_dec_st.workbuf);
	os_memset(&jpeg_dec_st, 0, sizeof(jpeg_dec_t));
	return kNoErr;
}

/*
	jpg_buf - input jpeg data buffer
	jpgdec_buf - output jpgdec_buf data buffer(the buffer should be freed by caller function)
	pic_size - input jpge file size
*/
bk_err_t bk_jpeg_dec_sw_fun(uint8_t *jpg_buf,uint8_t * jpgdec_buf, uint32_t pic_size)
{
	jpegdec_config_t jdec; /* Decompression object */
	int ret;
	uint32_t xs;// ys;

	jpeg_dec_st.inputbuf = jpg_buf;
	jpeg_dec_st.rd_ptr = 0;//init rd pointer
	jpeg_dec_st.jpg_file_size = pic_size;//1024

	ret = jpeg_decode_prepare(&jdec, input_func, jpeg_dec_st.workbuf, WORK_AREA_SIZE, NULL);
	if(JDR_OK != ret)
	{
		os_printf("--- pre-decoder error,ret = %x---\r\n",ret);
		return -1;
	}
	xs = jdec.width >> jpeg_dec_st.scale_ratio;
	//ys = jdec.height >> jpeg_dec_st.scale_ratio;
	switch (JD_FORMAT) {
		case 0: // RGB888
			jpeg_dec_st.line_wbyte = (xs * 3 + 3) & ~3;
			break;
		case 1:
		case 3:
			jpeg_dec_st.line_wbyte = xs * 2;
			break;
		case 2:
			jpeg_dec_st.line_wbyte = xs;//only output Y
			break;
		default:
			os_printf("--- pre-decoder error,ret = %x---\r\n", ret);
			return kParamErr;
	}

	jpeg_dec_st.outputbuf = jpgdec_buf;
	if(NULL == jpeg_dec_st.outputbuf)
	{
		os_printf("Y buffer malloc fail\r\n");
		return kParamErr;
	}
	
	//long tmp = rt_tick_get();
	ret = jpeg_decode_comp(&jdec, output_func, jpeg_dec_st.scale_ratio); /* Start to decompress with scaling */
	//os_printf("jpg dec delta:%x\r\n",rt_tick_get()-tmp);;
	
	if(JDR_OK == ret) {
		if (jpeg_dec_st.jpg_dec_end_isr != NULL) {
			jpeg_dec_st.jpg_dec_end_isr();
		}
		return kNoErr;
	} else
		return kOptionErr;
}


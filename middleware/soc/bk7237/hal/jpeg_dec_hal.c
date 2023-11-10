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
#include "BK7256_RegList.h"
#include "arch_interrupt.h"
#include "gpio_map.h"
#include "gpio_driver.h"
#include "gpio_map.h"
#include <driver/int.h>
#include "dma_hal.h"
#include <driver/dma.h>
#include <os/mem.h>
#include "dma_driver.h"
#include "sys_driver.h"
#include "jpeg_dec_macro_def.h"
#include "jpeg_dec_ll_macro_def.h"
#include "jpeg_dec_hal.h"
#include <driver/hal/hal_jpeg_dec_types.h>
#include "driver/jpeg_dec_types.h"
#include "sys_ll_op_if.h"
#include <driver/media_types.h>


#define	LDB_WORD(ptr)		(uint16_t)(((uint16_t)*((uint8_t*)(ptr))<<8)|(uint16_t)*(uint8_t*)((ptr)+1))
#define ZIG(n)	Zig[n]
#define IPSF(n)	Ipsf[n]
static JPG_DECODER_ST jpg_dec_st;
static JDEC jdec;
uint8_t Zig[64] = {	/* Zigzag-order to raster-order conversion table */
	 0,  1,  8, 16,  9,  2,  3, 10, 17, 24, 32, 25, 18, 11,  4,  5,
	12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13,  6,  7, 14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63
};

static const uint16_t Ipsf[64] = {	/* See also aa_idct.png */
	(uint16_t)(1.00000*8192), (uint16_t)(1.38704*8192), (uint16_t)(1.30656*8192), (uint16_t)(1.17588*8192), (uint16_t)(1.00000*8192), (uint16_t)(0.78570*8192), (uint16_t)(0.54120*8192), (uint16_t)(0.27590*8192),
	(uint16_t)(1.38704*8192), (uint16_t)(1.92388*8192), (uint16_t)(1.81226*8192), (uint16_t)(1.63099*8192), (uint16_t)(1.38704*8192), (uint16_t)(1.08979*8192), (uint16_t)(0.75066*8192), (uint16_t)(0.38268*8192),
	(uint16_t)(1.30656*8192), (uint16_t)(1.81226*8192), (uint16_t)(1.70711*8192), (uint16_t)(1.53636*8192), (uint16_t)(1.30656*8192), (uint16_t)(1.02656*8192), (uint16_t)(0.70711*8192), (uint16_t)(0.36048*8192),
	(uint16_t)(1.17588*8192), (uint16_t)(1.63099*8192), (uint16_t)(1.53636*8192), (uint16_t)(1.38268*8192), (uint16_t)(1.17588*8192), (uint16_t)(0.92388*8192), (uint16_t)(0.63638*8192), (uint16_t)(0.32442*8192),
	(uint16_t)(1.00000*8192), (uint16_t)(1.38704*8192), (uint16_t)(1.30656*8192), (uint16_t)(1.17588*8192), (uint16_t)(1.00000*8192), (uint16_t)(0.78570*8192), (uint16_t)(0.54120*8192), (uint16_t)(0.27590*8192),
	(uint16_t)(0.78570*8192), (uint16_t)(1.08979*8192), (uint16_t)(1.02656*8192), (uint16_t)(0.92388*8192), (uint16_t)(0.78570*8192), (uint16_t)(0.61732*8192), (uint16_t)(0.42522*8192), (uint16_t)(0.21677*8192),
	(uint16_t)(0.54120*8192), (uint16_t)(0.75066*8192), (uint16_t)(0.70711*8192), (uint16_t)(0.63638*8192), (uint16_t)(0.54120*8192), (uint16_t)(0.42522*8192), (uint16_t)(0.29290*8192), (uint16_t)(0.14932*8192),
	(uint16_t)(0.27590*8192), (uint16_t)(0.38268*8192), (uint16_t)(0.36048*8192), (uint16_t)(0.32442*8192), (uint16_t)(0.27590*8192), (uint16_t)(0.21678*8192), (uint16_t)(0.14932*8192), (uint16_t)(0.07612*8192)
};


/** breaf   User defined input function 
 *  param1  Decompression object
 *  param2  Pointer to the read buffer (0:skip)
 *  param2  Number of bytes to read/skip
 */
static uint16_t jpeg_dec_input_func (JDEC* jd, uint8_t* buff, uint16_t ndata)
{
	if (jpg_dec_st.rd_ptr > jpg_dec_st.jpg_file_size)
	{
		//os_printf("jpg_dec_st.jpg_file_size = %x \r\n", jpg_dec_st.jpg_file_size);
		//os_printf("jpg_dec_st.rd_ptr = %x \r\n", jpg_dec_st.rd_ptr);
		return 0xffff;
	}
	if (NULL != buff)
	{
		os_memcpy(buff, jpg_dec_st.inputbuf + jpg_dec_st.rd_ptr,ndata);
	}
	//os_printf("buff = %x \r\n", buff);
	//os_printf("*buff = %x \r\n", *buff);
	//os_printf("jpg_dec_st.rd_ptr = %x \r\n", jpg_dec_st.rd_ptr);
	//os_printf("jpg_dec_st.inputbuf = %x\r\n", *(jpg_dec_st.inputbuf +jpg_dec_st.rd_ptr));

	jpg_dec_st.rd_ptr += ndata;
	return ndata;

}

/**
 * @brief    This API is hw jpeg dec init
 *
 * @param   none
 * 
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
int jpg_decoder_init(void)
{
	jpg_dec_st.scale_ratio = 0;
	jpg_dec_st.rd_ptr = 0;//init rd pointer
	jpg_dec_st.jpg_file_size = 1024;

	jpg_dec_st.workbuf = os_malloc(WORK_AREA_SIZE);
	if (NULL == jpg_dec_st.workbuf)
	{
//		uart_printf("work area error!!\r\n");
		return -1;
	}
	return 0;
}


/**
 * @brief    This API is hw jpeg dec deinit
 *
 * @param   none
 * 
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
int jpg_decoder_deinit(void)
{
	if(NULL != jpg_dec_st.workbuf) {
		os_free(jpg_dec_st.workbuf);
		jpg_dec_st.workbuf = NULL;
	}
	os_memset(&jpg_dec_st, 0, sizeof(jpg_dec_st));
	return kNoErr;
}

/**
 * @brief    Pointer to allocated memory block (NULL:no memory available)
 *  
 * @param1   Pointer to the decompressor object
 * 
 * @param2   Number of bytes to allocate 
 * @return
 *     - BK_OK: succeed
 *     - others: other errors.
 */
static void* alloc_pool(JDEC* jd, uint16_t nd)
{
	char *rp = 0;


	nd = (nd + 3) & ~3;			/* Align block size to the word boundary */

	if (jd->sz_pool >= nd) {
		jd->sz_pool -= nd;
		rp = (char*)jd->pool;			/* Get start of available memory pool */
		jd->pool = (void*)(rp + nd);	/* Allocate requierd bytes */
	}

	return (void*)rp;	/* Return allocated memory block (NULL:no memory to allocate) */
}

/**
 * @brief   Create de-quantization and prescaling tables with a DQT segment
 *  
 * @param1   jd Pointer to the decompressor object
 * @param2   * data  Pointer to the quantizer tables 
 * @param3   ndata Size of input data
 * @return
 *     - JDR_OK: succeed
 *     - others: other errors.
 */
static JRESULT create_qt_tbl(JDEC* jd, const uint8_t* data, uint16_t ndata)
{
	uint16_t i;
	uint8_t d, z;
	int32_t *pb;


	while (ndata) {	/* Process all tables in the segment */
		if (ndata < 65) return JDR_FMT1;	/* Err: table size is unaligned */
		ndata -= 65;
		d = *data++;							/* Get table property */
		if (d & 0xF0) return JDR_FMT1;			/* Err: not 8-bit resolution */
		i = d & 3;								/* Get table ID */
		pb = alloc_pool(jd, 64 * sizeof (int32_t));/* Allocate a memory block for the table */
		if (!pb) return JDR_MEM1;				/* Err: not enough memory */
		jd->qttbl[i] = pb;						/* Register the table */
		for (i = 0; i < 64; i++) {				/* Load the table */
			z = ZIG(i);							/* Zigzag-order to raster-order conversion */
			pb[z] = (int32_t)((uint32_t)*data++ * IPSF(z));	/* Apply scale factor of Arai algorithm to the de-quantizers */
		}
	}

	return JDR_OK;
}


/**
 * @brief     Create huffman code tables with a DHT segment
 *  
 * @param1   jd Pointer to the decompressor object
 * @param2   * data Pointer to the packed huffman tables 
 * @param3   ndata Size of input data
 * @return
 *     - JDR_OK: succeed
 *     - others: other errors.
 */
static JRESULT create_huffman_tbl( JDEC* jd, const uint8_t* data, uint16_t ndata)
{
	uint16_t i, j, b, np, cls, num;
	uint8_t d, *pb, *pd;
	uint16_t hc, *ph;

	while (ndata) {	/* Process all tables in the segment */
		if (ndata < 17) return JDR_FMT1;	/* Err: wrong data size */
		ndata -= 17;
		d = *data++;						/* Get table number and class */
		if (d & 0xEE) return JDR_FMT1;		/* Err: invalid class/number */
		cls = d >> 4; num = d & 0x0F;		/* class = dc(0)/ac(1), table number = 0/1 */
		pb = alloc_pool(jd, 16);			/* Allocate a memory block for the bit distribution table */
		if (!pb) return JDR_MEM1;			/* Err: not enough memory */
		jd->huffbits[num][cls] = pb;
		for (np = i = 0; i < 16; i++) {		/* Load number of patterns for 1 to 16-bit code */
			np += (pb[i] = *data++);		/* Get sum of code words for each code */
		}
		ph = alloc_pool(jd, (uint16_t)(np * sizeof (uint16_t)));/* Allocate a memory block for the code word table */
		if (!ph) return JDR_MEM1;			/* Err: not enough memory */
		jd->huffcode[num][cls] = ph;
		hc = 0;
		for (j = i = 0; i < 16; i++) {		/* Re-build huffman code word table */
			b = pb[i];
			while (b--) ph[j++] = hc++;
			hc <<= 1;
		}

		if (ndata < np) return JDR_FMT1;	/* Err: wrong data size */
		ndata -= np;
		pd = alloc_pool(jd, np);			/* Allocate a memory block for the decoded data */
		if (!pd) return JDR_MEM1;			/* Err: not enough memory */
		jd->huffdata[num][cls] = pd;
		for (i = 0; i < np; i++) {			/* Load decoded data corresponds to each code ward */
			d = *data++;
			if (!cls && d > 11) return JDR_FMT1;
			*pd++ = d;
		}
	}

	return JDR_OK;
}

/**
 * @brief    Analyze the JPEG image and Initialize decompressor object 
 *  
 * @param1   Blank decompressor object
 * @param2   JPEG strem input function 
 * @param3   Working buffer for the decompression session
 * @param4   Size of working buffer
 * @param5   I/O device identifier for the session
 * @return
 *     - JDR_OK: succeed
 *     - others: other errors.
 */
JRESULT jd_prepare (JDEC* jd, uint16_t (*infunc)(JDEC*, uint8_t*, uint16_t), void* pool,
					uint16_t sz_pool, void* dev)
{
	uint8_t *seg, b;
	uint16_t marker;
	uint32_t ofs;
	uint32_t pointer;
	uint16_t n, i, j, len;
	JRESULT rc;
	uint8_t skip_get_mark = 0;

	if (!pool)
	{
		//os_printf("1--return = %d \r\n", JDR_PAR);
		return JDR_PAR;
	}
	jd->pool = pool;		/* Work memroy */
	jd->sz_pool = sz_pool;	/* Size of given work memory */
	jd->infunc = infunc;	/* Stream input function */
	jd->device = dev;		/* I/O device identifier */
	jd->nrst = 0;			/* No restart interval (default) */

	for (i = 0; i < 2; i++)
	{	/* Nulls pointers */
		for (j = 0; j < 2; j++)
		{
			jd->huffbits[i][j] = 0;
			jd->huffcode[i][j] = 0;
			jd->huffdata[i][j] = 0;
		}
	}
	for (i = 0; i < 4; jd->qttbl[i++] = 0) ;
	jd->inbuf = seg = alloc_pool(jd, JD_SZBUF);		/* Allocate stream input buffer */
	if (!seg)
	{
		//os_printf("JDR_MEM1--return = %d \r\n", JDR_MEM1);
		return JDR_MEM1;
	}
	if (jd->infunc(jd, seg, 2) != 2)
	{
		//os_printf("JDR_INP--return = %d \r\n", JDR_INP);
		return JDR_INP;/* Check SOI marker */
	}

	//os_printf("jd->inbuf = %x \r\n", *(jd->inbuf));
	//os_printf("jd->inbuf = %x \r\n", *(jd->inbuf + 1));
	//os_printf("jd->inbuf = %x \r\n", *(jd->inbuf + 2));

	if (LDB_WORD(seg) != 0xFFD8)
	{
		//os_printf("LDB_WORD(seg) = %x \r\n", LDB_WORD(seg));
		return JDR_FMT1;	/* Err: SOI is not detected */
	}

	ofs = 2;
	pointer = 2;

	for (;;)
	{
		if(0 == skip_get_mark)
		{
			/* Get a JPEG marker */
			if (jd->infunc(jd, seg, 4) != 4)
			{
				//os_printf("6-----JDR_INP = %x \r\n", jd->infunc(jd, seg, 4));
				//os_printf("6-----JDR_INP = %x \r\n", JDR_INP);
				return JDR_INP;
			}
			marker = LDB_WORD(seg);		/* Marker */
			len = LDB_WORD(seg + 2);	/* Length field */

			if((marker& 0xFF) != 0xFF) {
				//os_printf("6-----marker& 0xFF = %x \r\n", marker& 0xFF);
				pointer += 2 + len;
			}
			if (len <= 2 || (marker >> 8) != 0xFF) {
				//os_printf("6-----JDR_INP = %x \r\n", JDR_INP);
				return JDR_FMT1;
			}
			len -= 2;		/* Content size excluding length field */
			ofs += 4 + len;	/* Number of bytes loaded */
			BASE_FFDA = pointer;
		}

		skip_get_mark = 0;

		switch (marker & 0xFF) {
		case 0xC0:	/* SOF0 (baseline JPEG) */
			/* Load segment data */

			if (len > JD_SZBUF)
				return JDR_MEM2;
			if (jd->infunc(jd, seg, len) != len)
			{
				return JDR_INP;
			}

			jd->width = LDB_WORD(seg+3);		/* Image width in unit of pixel */
			jd->height = LDB_WORD(seg+1);		/* Image height in unit of pixel */

			if (seg[5] != 3)
			{
				os_printf("7--JDR_FMT3-- = %x \r\n", JDR_FMT3);
				return JDR_FMT3;	/* Err: Supports only Y/Cb/Cr format */
			}

			/* Check three image components */
			for (i = 0; i < 3; i++)
			{
				b = seg[7 + 3 * i];	/* Get sampling factor */
				if (!i)
				{	/* Y component */
					if (b != 0x11 && b != 0x22 && b != 0x21)
					{	/* Check sampling factor */
						return JDR_FMT3;/* Err: Supports only 4:4:4, 4:2:0 or 4:2:2 */
					}
					jd->msx = b >> 4; jd->msy = b & 15;/* Size of MCU [blocks] */
				}
				else
				{	/* Cb/Cr component */
					if (b != 0x11)
					{
						return JDR_FMT3;/* Err: Sampling factor of Cr/Cb must be 1 */
					}
				}
				b = seg[8 + 3 * i];	/* Get dequantizer table ID for this component */
				if (b > 3)
				{
					return JDR_FMT3;/* Err: Invalid ID */
				}
				jd->qtid[i] = b;
			}
			break;

		case 0xDD:	/* DRI */
			/* Load segment data */
			if (len > JD_SZBUF) return JDR_MEM2;
			if (jd->infunc(jd, seg, len) != len)
			{
				return JDR_INP;
			}

			/* Get restart interval (MCUs) */
			jd->nrst = LDB_WORD(seg);
			break;

		case 0xC4:	/* DHT */
			/* Load segment data */
			if (len > JD_SZBUF)
				return JDR_MEM2;
			if (jd->infunc(jd, seg, len) != len)
			{
				return JDR_INP;
			}

			/* Create huffman tables */
			rc = create_huffman_tbl(jd, seg, len);
			//os_printf("0xC4-----JPEGDEC_INTEN = %d \r\n", JPEGDEC_INTEN);
			if (rc)
				return rc;
			break;

		case 0xDB:	/* DQT */
			/* Load segment data */
			if (len > JD_SZBUF)
				return JDR_MEM2;
			if (jd->infunc(jd, seg, len) != len)
				return JDR_INP;

			/* Create de-quantizer tables */
			rc = create_qt_tbl(jd, seg, len);
			if (rc)
				return rc;
			break;

		case 0xDA:	/* SOS */
			/* Load segment data */
			if (len > JD_SZBUF)
				return JDR_MEM2;
			if (jd->infunc(jd, seg, len) != len)
				return JDR_INP;

			if (!jd->width || !jd->height)
				return JDR_FMT1;	/* Err: Invalid image size */

			if (seg[0] != 3)
				return JDR_FMT3;				/* Err: Supports only three color components format */

			/* Check if all tables corresponding to each components have been loaded */
			for (i = 0; i < 3; i++)
			{
				b = seg[2 + 2 * i];	/* Get huffman table ID */
				if (b != 0x00 && b != 0x11)	return JDR_FMT3;	/* Err: Different table number for DC/AC element */
				b = i ? 1 : 0;
				if (!jd->huffbits[b][0] || !jd->huffbits[b][1])
				{	/* Check dc/ac huffman table for this component */
					return JDR_FMT1;					/* Err: Nnot loaded */
				}
				if (!jd->qttbl[jd->qtid[i]])
				{			/* Check dequantizer table for this component */
					return JDR_FMT1;					/* Err: Not loaded */
				}
			}

			/* Allocate working buffer for MCU and RGB */
			n = jd->msy * jd->msx;						/* Number of Y blocks in the MCU */
			if (!n) return JDR_FMT1;					/* Err: SOF0 has not been loaded */
			len = n * 64 * 2 + 64;						/* Allocate buffer for IDCT and RGB output */
			if (len < 256) len = 256;					/* but at least 256 byte is required for IDCT */
			jd->workbuf = alloc_pool(jd, len);			/* and it may occupy a part of following MCU working buffer for RGB output */
			if (!jd->workbuf) return JDR_MEM1;			/* Err: not enough memory */
			jd->mcubuf = (uint8_t*)alloc_pool(jd, (uint16_t)((n + 2) * 64));	/* Allocate MCU working buffer */
			if (!jd->mcubuf) return JDR_MEM1;			/* Err: not enough memory */

			/* Pre-load the JPEG data to extract it from the bit stream */
			jd->dptr = seg; jd->dctr = 0; jd->dmsk = 0;	/* Prepare to read bit stream */
			if (ofs %= JD_SZBUF)
			{
			/* Align read offset to JD_SZBUF */
//				jd->dctr = jd->infunc(jd, seg + ofs, (uint16_t)(JD_SZBUF - ofs));
//				jd->dptr = seg + ofs - 1;
			}

			return JDR_OK;		/* Initialization succeeded. Ready to decompress the JPEG image. */

		case 0xC1:	/* SOF1 */
		case 0xC2:	/* SOF2 */
		case 0xC3:	/* SOF3 */
		case 0xC5:	/* SOF5 */
		case 0xC6:	/* SOF6 */
		case 0xC7:	/* SOF7 */
		case 0xC9:	/* SOF9 */
		case 0xCA:	/* SOF10 */
		case 0xCB:	/* SOF11 */
		case 0xCD:	/* SOF13 */
		case 0xCE:	/* SOF14 */
		case 0xCF:	/* SOF15 */
		case 0xD9:	/* EOI */
			return JDR_FMT3;	/* Unsuppoted JPEG standard (may be progressive JPEG) */

		/*skip the first maker flag(0xff),treat the following 0xff as the flag*/
		case 0xFF:
			if(0xff != seg[2])
			{
				pointer += 3;
				BASE_FFDA = pointer;				
				marker = seg[2];
				if(jd->infunc(jd, &seg[4], 1) != 1)
				{
					return JDR_INP;
				}
				len = LDB_WORD(&seg[3]);
				if (len <= 2 )
					return JDR_FMT1;
			}
			else
			{
				if(0xff != seg[3])
				{
					pointer += 4;
					BASE_FFDA = pointer;
					marker = seg[3];
					if(jd->infunc(jd, seg, 2) != 2)
						return JDR_INP;
				}
				else
				{
					pointer += 4;
					while(1)
					{
						if(jd->infunc(jd, seg, 1) != 1)
						{
							return JDR_INP;
						}
						if(0xff == seg[0])
						{				
							pointer += 1;
							BASE_FFDA = pointer;
							continue;
						}
						marker = seg[0];
						pointer += 1;
						BASE_FFDA = pointer;						
						if(jd->infunc(jd, seg, 2) != 2)
							return JDR_INP;
						break;
					}
				}

				len = LDB_WORD(seg);
				pointer += len;
				BASE_FFDA = pointer;
				if (len <= 2 )
					return JDR_FMT1;
			}
			len -= 2;
			skip_get_mark = 1;
			break;

		default:	/* Unknown segment (comment, exif or etc..) */
			/* Skip segment data */
			if (jd->infunc(jd, 0, len) != len)
			{	/* Null pointer specifies to skip bytes of stream */
					return JDR_INP;
			}
		}
	}
}

int jpg_dec_config(uint16_t xpixel, uint16_t ypixel, uint32_t length, unsigned char *input_buf, unsigned char * output_buf)
{
	jpeg_dec_ll_set_reg0x58_value((uint32_t)input_buf);
	jpeg_dec_ll_set_reg0x59_value((uint32_t)output_buf);
	jpg_dec_st.inputbuf = input_buf;

	jpeg_dec_ll_set_reg0x5b_value(xpixel*ypixel*2);
	jpeg_dec_ll_set_reg0xf_value(xpixel*ypixel*2/64 - 1);
#if(1)
		jpeg_dec_ll_set_reg0x5a_value(length);
#else
			switch(xpixel)
		{
			case PIXEL_320:
				jpeg_dec_ll_set_reg0x5a_value(HVGA_RD_LEN);
				break;
			case PIXEL_480:
				jpeg_dec_ll_set_reg0x5a_value(V720P_RD_LEN);
				break;
			case PIXEL_640:
				jpeg_dec_ll_set_reg0x5a_value(VGA_RD_LEN);
				break;
			case PIXEL_1280:
				jpeg_dec_ll_set_reg0x5a_value(V720P_RD_LEN);
				break;
			case JPEGDEC_X_PIXEL_1920:
				jpeg_dec_ll_set_reg0x5a_value(V1080P_RD_LEN);
				break;
			default:
				jpeg_dec_ll_set_reg0x5a_value(V720P_RD_LEN);
				break;
		}
#endif

	return 0;
}
void jpeg_dec_block_int_en(bool auto_int_en)
{
	jpeg_dec_ll_set_reg0x2_jpeg_dec_auto(0);
//	jpeg_dec_ll_set_reg0x5e_dec_frame_int(auto_int_en);
	jpeg_dec_ll_set_reg0x5e_dec_totalbusy_int(1);
}

void jpeg_dec_auto_frame_end_int_en(bool auto_int_en)
{
	jpeg_dec_ll_set_reg0x2_jpeg_dec_auto(auto_int_en);
	jpeg_dec_ll_set_reg0x5e_dec_frame_int(auto_int_en);
	jpeg_dec_ll_set_reg0x2_jpeg_dec_linen(0);
	jpeg_dec_ll_set_reg0x2_jpeg_line_num(0);
}

void jpeg_dec_auto_line_num_int_en(bool line_int_en, uint16_t line_num)
{
	jpeg_dec_ll_set_reg0x2_jpeg_dec_auto(line_int_en);
	jpeg_dec_ll_set_reg0x2_jpeg_dec_linen(line_int_en);
	jpeg_dec_ll_set_reg0x2_jpeg_line_num(line_num);
}


JRESULT JpegdecInit(void)
{
	int ret;
	uint32_t xs;
	uint32_t bits_num = 0, i;
	volatile unsigned long * huf_pointer;
	volatile unsigned long * zig_pointer;
	volatile unsigned long * dqt_pointer;

	jpg_dec_st.rd_ptr = 0;//init rd pointer
	jpg_dec_st.jpg_file_size = 1024;

	ret = jd_prepare(&jdec, jpeg_dec_input_func, jpg_dec_st.workbuf, WORK_AREA_SIZE, NULL);
	if(ret != JDR_OK) {
		os_printf("jd prepare error return %x \r\n", ret);
		return ret;
	}

	huf_pointer = &REG_JPEG_TMP0;
	for(i = 0; i < 64; i = i + 1)
		*(huf_pointer + i) = 0;

	REG_JPEG_BITS_DC00 = *((jdec).huffbits[0][0] + 0);
	REG_JPEG_BITS_DC01 = *((jdec).huffbits[0][0] + 1);
	REG_JPEG_BITS_DC02 = *((jdec).huffbits[0][0] + 2);
	REG_JPEG_BITS_DC03 = *((jdec).huffbits[0][0] + 3);
	REG_JPEG_BITS_DC04 = *((jdec).huffbits[0][0] + 4);
	REG_JPEG_BITS_DC05 = *((jdec).huffbits[0][0] + 5);
	REG_JPEG_BITS_DC06 = *((jdec).huffbits[0][0] + 6);
	REG_JPEG_BITS_DC07 = *((jdec).huffbits[0][0] + 7);
	REG_JPEG_BITS_DC08 = *((jdec).huffbits[0][0] + 8);
	REG_JPEG_BITS_DC09 = *((jdec).huffbits[0][0] + 9);
	REG_JPEG_BITS_DC0A = *((jdec).huffbits[0][0] + 10);
	REG_JPEG_BITS_DC0B = *((jdec).huffbits[0][0] + 11);
	REG_JPEG_BITS_DC0C = *((jdec).huffbits[0][0] + 12);
	REG_JPEG_BITS_DC0D = *((jdec).huffbits[0][0] + 13);
	REG_JPEG_BITS_DC0E = *((jdec).huffbits[0][0] + 14);
	REG_JPEG_BITS_DC0F = *((jdec).huffbits[0][0] + 15);	

	for(i = 0; i < 16; i = i + 1)
	{
		bits_num = bits_num + *((jdec).huffbits[0][0] + i);
	}
	//os_printf("13-----bits_num = %d \r\n", bits_num);

	huf_pointer = &REG_JPEG_HUF_TABLE00;
	for(i = 0; i < bits_num; i = i + 1)
		*(huf_pointer + i) = (*((jdec).huffcode[0][0] + i) << 8) + *((jdec).huffdata[0][0] + i);

	//ind 10
	REG_JPEG_BITS_DC10 = *((jdec).huffbits[1][0] + 0); 
	REG_JPEG_BITS_DC11 = *((jdec).huffbits[1][0] + 1); 
	REG_JPEG_BITS_DC12 = *((jdec).huffbits[1][0] + 2); 
	REG_JPEG_BITS_DC13 = *((jdec).huffbits[1][0] + 3); 
	REG_JPEG_BITS_DC14 = *((jdec).huffbits[1][0] + 4); 
	REG_JPEG_BITS_DC15 = *((jdec).huffbits[1][0] + 5); 
	REG_JPEG_BITS_DC16 = *((jdec).huffbits[1][0] + 6); 
	REG_JPEG_BITS_DC17 = *((jdec).huffbits[1][0] + 7); 
	REG_JPEG_BITS_DC18 = *((jdec).huffbits[1][0] + 8); 
	REG_JPEG_BITS_DC19 = *((jdec).huffbits[1][0] + 9); 
	REG_JPEG_BITS_DC1A = *((jdec).huffbits[1][0] + 10);
	REG_JPEG_BITS_DC1B = *((jdec).huffbits[1][0] + 11);
	REG_JPEG_BITS_DC1C = *((jdec).huffbits[1][0] + 12);
	REG_JPEG_BITS_DC1D = *((jdec).huffbits[1][0] + 13);
	REG_JPEG_BITS_DC1E = *((jdec).huffbits[1][0] + 14);
	REG_JPEG_BITS_DC1F = *((jdec).huffbits[1][0] + 15);

	bits_num = 0;	
	for(i = 0; i < 16; i = i + 1)
	{
		bits_num = bits_num + *((jdec).huffbits[1][0] + i);
		//addJPEG_DEC_Reg0x1 = bits_num;
	}

	huf_pointer = &REG_JPEG_HUF_TABLE10;
	for(i = 0; i < bits_num; i = i + 1)
		*(huf_pointer + i) = (*((jdec).huffcode[1][0] + i) << 8) + *((jdec).huffdata[1][0] + i);

	//ind 01
	REG_JPEG_BITS_AC00 = *((jdec).huffbits[0][1] + 0);
	REG_JPEG_BITS_AC01 = *((jdec).huffbits[0][1] + 1);
	REG_JPEG_BITS_AC02 = *((jdec).huffbits[0][1] + 2);
	REG_JPEG_BITS_AC03 = *((jdec).huffbits[0][1] + 3);
	REG_JPEG_BITS_AC04 = *((jdec).huffbits[0][1] + 4);
	REG_JPEG_BITS_AC05 = *((jdec).huffbits[0][1] + 5);
	REG_JPEG_BITS_AC06 = *((jdec).huffbits[0][1] + 6);
	REG_JPEG_BITS_AC07 = *((jdec).huffbits[0][1] + 7);
	REG_JPEG_BITS_AC08 = *((jdec).huffbits[0][1] + 8);
	REG_JPEG_BITS_AC09 = *((jdec).huffbits[0][1] + 9);
	REG_JPEG_BITS_AC0A = *((jdec).huffbits[0][1] + 10);
	REG_JPEG_BITS_AC0B = *((jdec).huffbits[0][1] + 11);
	REG_JPEG_BITS_AC0C = *((jdec).huffbits[0][1] + 12);
	REG_JPEG_BITS_AC0D = *((jdec).huffbits[0][1] + 13);
	REG_JPEG_BITS_AC0E = *((jdec).huffbits[0][1] + 14);
	REG_JPEG_BITS_AC0F = *((jdec).huffbits[0][1] + 15);
	bits_num = 0; 
	for(i = 0; i < 16; i = i + 1)
	{
		bits_num = bits_num + *((jdec).huffbits[0][1] + i);
	}

	huf_pointer = &REG_JPEG_HUF_TABLE01;
	for(i = 0; i < bits_num; i = i + 1)
		*(huf_pointer + i) = (*((jdec).huffcode[0][1] + i) << 8) + *((jdec).huffdata[0][1] + i);

	//ind 11
	REG_JPEG_BITS_AC10 = *((jdec).huffbits[1][1] + 0);
	REG_JPEG_BITS_AC11 = *((jdec).huffbits[1][1] + 1);
	REG_JPEG_BITS_AC12 = *((jdec).huffbits[1][1] + 2);
	REG_JPEG_BITS_AC13 = *((jdec).huffbits[1][1] + 3);
	REG_JPEG_BITS_AC14 = *((jdec).huffbits[1][1] + 4);
	REG_JPEG_BITS_AC15 = *((jdec).huffbits[1][1] + 5);
	REG_JPEG_BITS_AC16 = *((jdec).huffbits[1][1] + 6);
	REG_JPEG_BITS_AC17 = *((jdec).huffbits[1][1] + 7);
	REG_JPEG_BITS_AC18 = *((jdec).huffbits[1][1] + 8);
	REG_JPEG_BITS_AC19 = *((jdec).huffbits[1][1] + 9);
	REG_JPEG_BITS_AC1A = *((jdec).huffbits[1][1] + 10);
	REG_JPEG_BITS_AC1B = *((jdec).huffbits[1][1] + 11);
	REG_JPEG_BITS_AC1C = *((jdec).huffbits[1][1] + 12);
	REG_JPEG_BITS_AC1D = *((jdec).huffbits[1][1] + 13);
	REG_JPEG_BITS_AC1E = *((jdec).huffbits[1][1] + 14);
	REG_JPEG_BITS_AC1F = *((jdec).huffbits[1][1] + 15);
	bits_num = 0;
	for(i = 0; i < 16; i = i + 1)
	{
	  bits_num = bits_num + *((jdec).huffbits[1][1] + i);
	}

	huf_pointer = &REG_JPEG_HUF_TABLE11;
	for(i = 0; i < bits_num; i = i + 1)
		*(huf_pointer + i) = (*((jdec).huffcode[1][1] + i) << 8) + *((jdec).huffdata[1][1] + i);

	dqt_pointer	= &REG_JPEG_DQT_TABLE0;
	for(i = 0; i < 64; i = i + 1)
		*(dqt_pointer + i) = (*((jdec).qttbl[0] + i));
	
	dqt_pointer	= &REG_JPEG_DQT_TABLE1;
	for(i = 0; i < 64; i = i + 1)
		*(dqt_pointer + i) = (*((jdec).qttbl[1] + i));	

	zig_pointer	= &REG_JPEG_Zig;
	for(i = 0; i < 64; i = i + 1)
		*(zig_pointer + i) = (unsigned long)Zig[i];

	xs = (jdec).width >> jpg_dec_st.scale_ratio;
	//ys = (*jdec).height >> jpg_dec_st.scale_ratio;
	jpg_dec_st.width = (jdec).width;
	jpg_dec_st.heigth = (jdec).height;
	jpg_dec_st.line_wbyte = xs;//only output Y
	return JDR_OK;
}


/**
 * @brief	 Start to decompress the JPEG picture  
 *	
 * @param1	 jd Initialized decompression object
 * 
 * @param2	 size of bytes to allocate 
 * @return
 *	   - BK_OK: succeed
 *	   - others: other errors.
 */
JRESULT jd_decomp(void)
{
	//uint16_t	mx, my;
	JRESULT rc;

	jpeg_dec_ll_set_reg0x0_jpeg_dec_en(1);
	jpeg_dec_ll_set_reg0x5_mcu_x(0);
	jpeg_dec_ll_set_reg0x6_mcu_y(0);
	//mx = jd->msx * 8; my = jd->msy * 8;			/* Size of the MCU (pixel) */
	jpeg_dec_ll_set_reg0xa_value(jdec.width);  //pixel number X
	jpeg_dec_ll_set_reg0x8_dec_cmd(JPEGDEC_DC_CLEAR);
	jdec.dcv[2] = jdec.dcv[1] = jdec.dcv[0] = 0;	/* Initialize DC values */
	rc = JDR_OK;
	jpeg_dec_ll_set_reg0x8_dec_cmd(JPEGDEC_START);
	return rc;
}

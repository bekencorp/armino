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

#pragma once

#include <soc/soc.h>
#include "driver/jpeg_dec_types.h"



#ifdef __cplusplus
extern "C" {
#endif

#define addJPEG_data_st                                         (addJPEG_Reg0x9 & 0x100)
#define addJPEG_ycount                                          (addJPEG_Reg0x9 & 0xff)
#define WORK_AREA_SIZE 4096

#define	JD_SZBUF		1024	/* Size of stream input buffer */
#define JD_FORMAT		0	/* Output pixel format 0:RGB888 (3 BYTE/pix), 1:RGB565 (1 WORD/pix) */
#define	JD_USE_SCALE	1	/* Use descaling feature for output */
#define JD_TBLCLIP		1	/* Use table for saturation (might be a bit faster but increases 1K bytes of code size) */

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Rectangular structure */
typedef struct {
	uint16_t left, right, top, bottom;
} JRECT;


/* Decompressor object structure */
typedef struct JDEC JDEC;
struct JDEC {
	uint16_t dctr;				/* Number of bytes available in the input buffer */
	uint8_t* dptr;				/* Current data read ptr */
	uint8_t* inbuf;				/* Bit stream input buffer */
	uint8_t dmsk;				/* Current bit in the current read byte */
	uint8_t scale;				/* Output scaling ratio */
	uint8_t msx, msy;			/* MCU size in unit of block (width, height) */
	uint8_t qtid[3];			/* Quantization table ID of each component */
	int16_t dcv[3];				/* Previous DC element of each component */
	uint16_t nrst;				/* Restart inverval */
	uint16_t width, height;		/* Size of the input image (pixel) */
	uint8_t* huffbits[2][2];	/* Huffman bit distribution tables [id][dcac] */
	uint16_t* huffcode[2][2];	/* Huffman code word tables [id][dcac] */
	uint8_t* huffdata[2][2];	/* Huffman decoded data tables [id][dcac] */
	int32_t* qttbl[4];			/* Dequantizer tables [id] */
	void* workbuf;				/* Working buffer for IDCT and RGB output */
	uint8_t* mcubuf;			/* Working buffer for the MCU */
	void* pool;					/* Pointer to available memory pool */
	uint16_t sz_pool;			/* Size of momory pool (bytes available) */
	uint16_t (*infunc)(JDEC*, uint8_t*, uint16_t);/* Pointer to jpeg stream input function */
	void* device;				/* Pointer to I/O device identifiler for the session */
};



//static 	JDEC jdec; /* Decompression object */

int jpg_dec_config(uint16_t xpixel, uint16_t ypixel, uint32_t length, unsigned char *input_buf, unsigned char * output_buf);
void jpeg_dec_block_int_en(bool auto_int_en);
void jpeg_dec_auto_frame_end_int_en(bool auto_int_en);
void jpeg_dec_auto_line_num_int_en(bool line_int_en, uint16_t line_num);

JRESULT JpegdecInit(void);
JRESULT jd_decomp(void);

int jpg_decoder_init(void);

int jpg_decoder_deinit(void);

#ifdef __cplusplus
}
#endif










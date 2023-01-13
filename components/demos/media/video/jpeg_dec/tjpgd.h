/*----------------------------------------------------------------------------/
/ TJpgDec - Tiny JPEG Decompressor R0.03 include file         (C)ChaN, 2021
/----------------------------------------------------------------------------*/
#ifndef DEF_TJPGDEC
#define DEF_TJPGDEC

#ifdef __cplusplus
extern "C" {
#endif

#include "tjpgdcnf.h"
//#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <common/bk_include.h>
#include <components/log.h>
#include <os/mem.h>
#include <os/str.h>


#if JD_FASTDECODE >= 1
typedef int16_t jd_yuv_t;
#else
typedef uint8_t jd_yuv_t;
#endif

/*Specifies output pixel format.*/
typedef enum {
	JDF_RGB888 = 0, /* 0: RGB888 (24-bit/pix) */
	JDF_RGB565, 	/* 1: RGB565 (16-bit/pix) */
	JDF_Grayscale,  /* 2: Grayscale (8-bit/pix) */
	JDF_VYUY,		/* 3: VYUY (16-bit/pix) */
	JDF_YUYV, 		/* 4: YUYV (16-bit/pix) */
	JDF_MAX,
}JFORMAT;

#if 0
#define JPEG_DTCM  __attribute__((section(".dtcm_sec_data")))
#define JPEG_ITCM  __attribute__((section(".itcm_sec_code")))
#else
#define JPEG_DTCM
#define JPEG_ITCM
#endif

/* Error code */
typedef enum {
	JDR_OK = 0,	/* 0: Succeeded */
	JDR_INTR,	/* 1: Interrupted by output function */	
	JDR_INP,	/* 2: Device error or wrong termination of input stream */
	JDR_MEM1,	/* 3: Insufficient memory pool for the image */
	JDR_MEM2,	/* 4: Insufficient stream input buffer */
	JDR_PAR,	/* 5: Parameter error */
	JDR_FMT1,	/* 6: Data format error (may be broken data) */
	JDR_FMT2,	/* 7: Right format but not supported */
	JDR_FMT3	/* 8: Not supported JPEG standard */
} JRESULT;



/* Rectangular region in the output image */
typedef struct {
	uint16_t left;		/* Left end */
	uint16_t right;		/* Right end */
	uint16_t top;		/* Top end */
	uint16_t bottom;	/* Bottom end */
} JRECT;



/* Decompressor object structure */
typedef struct JDEC JDEC;
struct JDEC {
	size_t dctr;				/* Number of bytes available in the input buffer */
	uint8_t* dptr;				/* Current data read ptr */
	uint8_t* inbuf;				/* Bit stream input buffer */
	uint8_t dbit;				/* Number of bits availavble in wreg or reading bit mask */
	uint8_t scale;				/* Output scaling ratio */
	uint8_t msx, msy;			/* MCU size in unit of block (width, height) */
	uint8_t qtid[3];			/* Quantization table ID of each component, Y, Cb, Cr */
	uint8_t ncomp;				/* Number of color components 1:grayscale, 3:color */
	int16_t dcv[3];				/* Previous DC element of each component */
	uint16_t nrst;				/* Restart inverval */
	uint16_t width, height;		/* Size of the input image (pixel) */
	uint8_t* huffbits[2][2];	/* Huffman bit distribution tables [id][dcac] */
	uint16_t* huffcode[2][2];	/* Huffman code word tables [id][dcac] */
	uint8_t* huffdata[2][2];	/* Huffman decoded data tables [id][dcac] */
	int32_t* qttbl[4];			/* Dequantizer tables [id] */
#if JD_FASTDECODE >= 1
	uint32_t wreg;				/* Working shift register */
	uint8_t marker;				/* Detected marker (0:None) */
#if JD_FASTDECODE == 2
	uint8_t longofs[2][2];		/* Table offset of long code [id][dcac] */
	uint16_t* hufflut_ac[2];	/* Fast huffman decode tables for AC short code [id] */
	uint8_t* hufflut_dc[2];		/* Fast huffman decode tables for DC short code [id] */
#endif
#endif
	void* workbuf;				/* Working buffer for IDCT and RGB output */
	jd_yuv_t* mcubuf;			/* Working buffer for the MCU */
	void* pool;					/* Pointer to available memory pool */
	size_t sz_pool;				/* Size of momory pool (bytes available) */
	size_t (*infunc)(JDEC*, uint8_t*, size_t);	/* Pointer to jpeg stream input function */
	void* device;				/* Pointer to I/O device identifiler for the session */
};


/* TJpgDec API functions */
JPEG_ITCM JRESULT jd_prepare_sw (JDEC* jd, size_t (*infunc)(JDEC*,uint8_t*,size_t), void* pool, size_t sz_pool, void* dev);
JPEG_ITCM JRESULT jd_decomp_sw (JDEC* jd, int (*outfunc)(JDEC*,void*,JRECT*), uint8_t scale);
void jd_set_JD_FORMAT(JD_FORMAT_OUTPUT format);
JD_FORMAT_OUTPUT jd_get_JD_FORMAT(void);


#ifdef __cplusplus
}
#endif

#endif /* _TJPGDEC */

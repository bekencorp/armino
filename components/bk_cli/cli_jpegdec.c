#include <stdlib.h>
#include "cli.h"
#include <os/os.h>
#include <components/system.h>
#include "bk_rtos_debug.h"

#if (CONFIG_FATFS)
#include "ff.h"
#include "diskio.h"
#endif

#if (CONFIG_JPEGDEC_SW)
#include <components/jpeg_decode.h>
#endif

#define JPEG_DEC_CNT (sizeof(s_jpegdec_commands) / sizeof(struct cli_command))

#if CONFIG_SLAVE_CORE
#define JPEG_DTCM __attribute__((section(".dtcm_sec_data")))
#define JPEG_ITCM __attribute__((section(".itcm_sec_code ")))
#else
#define JPEG_DTCM
#define JPEG_ITCM
#endif

#define RANGE_LIMIT(x) (x > 255 ? 255 : (x < 0 ? 0 : x))
#define write_data(addr,val)                 *((volatile uint32_t *)(addr)) = val
#define read_data(addr,val)                  val = *((volatile uint32_t *)(addr))
#define get_addr_data(addr)                  *((volatile uint32_t *)(addr))

extern JPEG_ITCM uint64_t riscv_get_cycle(void);
extern JPEG_ITCM uint64_t riscv_get_mtimer(void);
extern JPEG_ITCM uint64_t riscv_get_instruct_cnt(void);

void bk_mem_dump_ex(const char *title, unsigned char *data, uint32_t data_len);

static JPEG_DTCM uint64_t volatile	saved_time = 0;
static JPEG_DTCM uint64_t volatile	cur_time = 0;
static JPEG_DTCM uint64_t volatile	saved_sycle_cnt = 0;
static JPEG_DTCM uint64_t volatile	cur_sycle_cnt = 0;
static JPEG_DTCM uint64_t volatile	saved_inst_cnt = 0;
static JPEG_DTCM uint64_t volatile	cur_inst_cnt = 0;

#define jpeg_perfmon(flag) \
{ \
	cur_inst_cnt = riscv_get_instruct_cnt(); \
	cur_sycle_cnt = riscv_get_cycle(); \
	cur_time = riscv_get_mtimer(); \
 \
	saved_time = (cur_time - saved_time) / 26; \
	saved_sycle_cnt = cur_sycle_cnt - saved_sycle_cnt; \
	saved_inst_cnt = cur_inst_cnt - saved_inst_cnt; \
 \
	if (1 == (flag)) { \
		os_printf("=====elapse time(%d us).\r\n", (u32)((saved_time) & 0xFFFFFFFF)); \
		os_printf("=====diff inst_cnt: %d:%d\r\n", (u32)(saved_inst_cnt >> 32), (u32)(saved_inst_cnt & 0xFFFFFFFF)); \
		os_printf("=====diff sycle_cnt: %d:%d\r\n", (u32)(saved_sycle_cnt >> 32), (u32)(saved_sycle_cnt & 0xFFFFFFFF)); \
	} \
 \
	saved_time = riscv_get_mtimer(); \
	saved_sycle_cnt = riscv_get_cycle(); \
	saved_inst_cnt = riscv_get_instruct_cnt(); \
}


//Note the count n is the word count n == sizeof(buf)/sizeof(word)
JPEG_ITCM void memcpy32(uint32_t *out, const uint32_t *in, uint32_t n)
{
	for(int i = 0; i < n; i++)
	{
		write_data((out+i), get_addr_data(in+i));
	}
}

#if 0 // CONFIG_SLAVE_CORE


#define YUV_WIDTH_MAX (320)
#define YUV_HEIGHT_MAX (480)

#define RGB_WIDTH_MAX (480)
#define RGB_HEIGHT_MAX (320)

#define BUF_WIDTH_PIXEL (80)
#define BUF_HEIGHT_PIXEL (60)

#define BUF_WIDTH_WORD  (BUF_WIDTH_PIXEL/2)
#define BUF_HEIGHT_WORD (BUF_HEIGHT_PIXEL/2)
#define YUV_BUF_SIZE   (80*60*2)
#define YUV_BUF_SIZE_WORD  (YUV_BUF_SIZE/4)

#define PSRAM_SRC_ADDR (0x60000000)
#define PSRAM_DEST_ADDR (0x60100000)

#define PSRAM_SRC_CACHE_ADDR (0x64000000)
#define PSRAM_DEST_CACHE_ADDR (0x64100000)



uint32_t g_yuv_line_word_count = YUV_WIDTH_MAX/2;
uint32_t g_rgb_line_word_count = RGB_WIDTH_MAX/2;



//Note the count n is the word count n == sizeof(buf)/sizeof(word)
JPEG_ITCM int memcmp32(uint32_t *out, const uint32_t *in, uint32_t n)
{
	// os_printf("Start Check memroy size(%d), src addr(0x%x), dest addr(0x%x)\r\n", n, in, out);
       for(int i = 0; i < n; i++)
       {
          if((get_addr_data(out+i) != get_addr_data(in+i))) {
          	 os_printf("****************Faild, src addr(0x%x). dest addr(0x%x)\r\n",in+i, out+i);
          	 // bk_mem_dump_ex("src data", (unsigned char *)in, 64);
          	 // bk_mem_dump_ex("dst data", (unsigned char *)out, 64);
          	 return -1;
          }
       }
       return 0;
}


// #define memcpy32_from_psram memcpy32
JPEG_ITCM void memcpy32_from_psram(uint32_t *out, const uint32_t *in, uint32_t n)
{
	int psram_offset = 0, buf_offset = 0;
	for(int i = 0; i < BUF_HEIGHT_PIXEL; i++) {
		memcpy32(out + buf_offset, in + psram_offset, BUF_WIDTH_WORD);
		psram_offset += g_yuv_line_word_count;
		buf_offset += BUF_WIDTH_WORD;
	}
}

// #define memcpy32_to_psram memcpy32
JPEG_ITCM void memcpy32_to_psram(uint32_t *out, const uint32_t *in, uint32_t n)
{
	int psram_offset = 0, buf_offset = 0;
	for(int i = 0; i < BUF_HEIGHT_PIXEL; i++) {
		memcpy32(out + psram_offset, in + buf_offset, BUF_WIDTH_WORD);
		psram_offset += g_yuv_line_word_count;
		buf_offset += BUF_WIDTH_WORD;
	}
}

JPEG_DTCM static int s_rows, s_cols;
JPEG_DTCM static int s_y, s_u, s_v, s_r, s_g, s_b;
JPEG_DTCM static unsigned char *s_yuv_buf;
JPEG_DTCM static unsigned short *s_rgb_buf;
JPEG_DTCM static int s_y_pos,s_u_pos,s_v_pos;
JPEG_DTCM static int s_r_tmp=0,s_g_tmp=0,s_b_tmp=0, s_tmp=0;

JPEG_ITCM \
void YUV422ToRGB565(const void* inbuf, void* outbuf, int width, int height)
{
	//int index = RGB_ROTATE_90_CONST(width,height);

	s_yuv_buf = (unsigned char *)inbuf;
	s_rgb_buf = (unsigned short *)outbuf;

	s_y_pos = 0;
	s_u_pos = 1;
	s_v_pos = 3;

	s_r_tmp=0;
	s_g_tmp=0;
	s_b_tmp=0;
	s_tmp = 0;

	for (s_rows = 0; s_rows < height; s_rows++) {
		for (s_cols = 0; s_cols < width; s_cols++) {
			s_y = s_yuv_buf[s_y_pos];
			s_u = s_yuv_buf[s_u_pos] - 128;
			s_v = s_yuv_buf[s_v_pos] - 128;

			if ((s_cols & 0x01) == 0) {
				s_r_tmp = s_v + ((s_v * 103) >> 8);
				s_g_tmp = ((s_u * 88) >> 8) + ((s_v * 183) >> 8);
				s_b_tmp = s_u + ((s_u * 198) >> 8);
			}

			// R = Y + 1.402*(V-128)
			// G = Y - 0.34414*(U-128)
			// B = Y + 1.772*(U-128)
			s_r = RANGE_LIMIT(s_y + s_r_tmp);
			s_g = RANGE_LIMIT(s_y - s_g_tmp);
			s_b = RANGE_LIMIT(s_y + s_b_tmp);

			// index += RGB_ROTATE_90_OFSSET(rows,cols,width,height);
			// rgb_buf[index] = (((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3));
			*s_rgb_buf++ = (((s_r & 0xf8) << 8) | ((s_g & 0xfc) << 3) | ((s_b & 0xf8) >> 3));

			s_y_pos += 2;

			if (s_cols & 0x01) {
				s_u_pos += 4;
				s_v_pos += 4;
			}
		}
	}
}

// #define RGB_ROTATE_90(rows,cols,height,width)  ((width)*(height-cols-1)+(rows)-1)
#define RGB_ROTATE_90(rows,cols,height,width)  (width*height-width-width*cols+rows-1)
#define RGB_ROTATE_90_CONST(height,width)  (width*height-width-1)
#define RGB_ROTATE_90_OFSSET(rows,cols,height,width) (rows-width*cols)

JPEG_ITCM \
void YUV422ToRGB565_Rotate90(const void* inbuf, void* outbuf, int width, int height)
{
	//int index = RGB_ROTATE_90_CONST(width,height);
	// jpeg_perfmon(1);
	int rows, cols;
	int y, u, v, r, g, b;
	unsigned char *yuv_buf;
	unsigned short *rgb_buf;
	int y_pos,u_pos,v_pos;
	int r_tmp=0,g_tmp=0,b_tmp=0;

	yuv_buf = (unsigned char *)inbuf;
	rgb_buf = (unsigned short *)outbuf;

	y_pos = 0;
	u_pos = 1;
	v_pos = 3;

	jpeg_perfmon(1);

	for (rows = 0; rows < height; rows++) {
		for (cols = 0; cols < width; cols++) {
			y = yuv_buf[y_pos];
			u = yuv_buf[u_pos] - 128;
			v = yuv_buf[v_pos] - 128;

			if ((cols & 0x01) == 0) {
				r_tmp = v + ((v * 103) >> 8);
				g_tmp = ((u * 88) >> 8) + ((v * 183) >> 8);
				b_tmp = u + ((u * 198) >> 8);
				// tmp = (v << 5) + (v << 3) - v;
				// r_tmp = v + (((v << 6) + (v << 5) + (v << 3) - v) >> 8);
				// g_tmp = (((u << 6) + (u << 4) + (u << 3)) >> 8) + (((v << 7) + (v << 5) + (v << 3) - v) >> 8);
				// b_tmp = u + (((u << 7) + (u << 6)) >> 8);
			}

			// R = Y + 1.402*(V-128)
			// G = Y - 0.34414*(U-128)
			// B = Y + 1.772*(U-128)
			r = RANGE_LIMIT(y + r_tmp);
			g = RANGE_LIMIT(y - g_tmp);
			b = RANGE_LIMIT(y + b_tmp);

			// index += RGB_ROTATE_90_OFSSET(rows,cols,width,height);
			// rgb_buf[index] = (((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3));
			*rgb_buf++ = (((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3));

			y_pos += 2;

			if (cols & 0x01) {
				u_pos += 4;
				v_pos += 4;
			}
		}
	}
	jpeg_perfmon(1);
}


JPEG_DTCM uint8_t g_in_buff[YUV_BUF_SIZE];
JPEG_DTCM uint8_t g_out_buff[YUV_BUF_SIZE];

JPEG_ITCM \
void test_yuv_rotate(uint32_t *src_psram,
			uint32_t *dest_psram,
			int width,
			int height,
			int test_count) {
	int i = 0;
	// int j = 0, k = 0, offset = 0;

	int src_Len = YUV_BUF_SIZE * test_count;

	void *inbuf = NULL;
	void *outbuf = NULL;

	inbuf = (void *)&g_in_buff[0];
	outbuf = (void *)&g_out_buff[0];


	do {
		// inbuf = os_malloc(YUV_BUF_SIZE);
		// if(NULL == inbuf)
		// {
		// 	os_printf("Faild, malloc inbuf mem oom.\r\n");
		// 	break;
		// }
		// outbuf = os_malloc(YUV_BUF_SIZE);
		// if(NULL == outbuf)
		// {
		// 	os_printf("Faild, malloc outbuf mem oom.\r\n");
		// 	break;
		// }

		if(width > BUF_WIDTH_PIXEL || height > BUF_HEIGHT_PIXEL)
		{
			os_printf("Invalid paramter.width(%d),height(%d).\r\n", width,height);
			return;
		}

		memset((void *)outbuf, 0, YUV_BUF_SIZE);
		for(i = 0; i < src_Len/4; i++) {
			*((uint32_t *)src_psram + i) = i+0x77777777;
		}

		memcpy32(inbuf, src_psram, YUV_BUF_SIZE_WORD);

		// os_printf("YUB test begin. psram->dtcm===========\r\n");
		// jpeg_perfmon(0);
		// for(i = 0; i < test_count; i++) {
		// 	memcpy32_from_psram(inbuf, src_psram + i * YUV_BUF_SIZE_WORD, YUV_BUF_SIZE_WORD);
		// }
		// jpeg_perfmon(1);
		// os_printf("YUB test end.psram->dtcm===========\r\n");

		// os_printf("YUB test move data full begin.===========\r\n");
		// jpeg_perfmon(0);
		// offset = 0;
		// for(k = 0; k < test_count/32; k++){
		// 	for(i = 0; i < 8; i++) {
		// 		for(j = 0; j < 4; j++) {
		// 			offset =  YUV_BUF_SIZE_WORD*i*4 + BUF_WIDTH_WORD*j;
		// 			memcpy32_from_psram(inbuf, src_psram + offset, YUV_BUF_SIZE_WORD);
		// 			memcpy32(outbuf, inbuf, YUV_BUF_SIZE_WORD);
		// 			memcpy32_to_psram(dest_psram + offset, outbuf, YUV_BUF_SIZE_WORD);
		// 		}
		// 	}
		// }
		// jpeg_perfmon(1);
		// memcmp32(dest_psram, src_psram, YUV_BUF_SIZE_WORD*32);
		// os_printf("YUB test move data end.===========\r\n");

		os_printf("YUB test begin.===========\r\n");
		jpeg_perfmon(0);
		for(i = 0; i < test_count; i++) {
			YUV422ToRGB565(inbuf, outbuf, width, height);
		}
		jpeg_perfmon(1);
		os_printf("YUB test end.===========\r\n");

		os_printf("YUB test rotate begin.===========\r\n");
		jpeg_perfmon(0);
		// for(i = 0; i < test_count; i++) {
			YUV422ToRGB565_Rotate90(inbuf, outbuf, width, height);
		// }
		jpeg_perfmon(1);
		os_printf("YUB test rotate end.===========\r\n");

		// os_printf("YUB test begin. dtcm->psram. ==========\r\n");
		// jpeg_perfmon(0);
		// for(i = 0; i < test_count; i++) {
		// 	memcpy32_to_psram(dest_psram + i * YUV_BUF_SIZE_WORD, outbuf, YUV_BUF_SIZE_WORD);
		// }
		// jpeg_perfmon(1);
		// os_printf("YUB test end. dtcm->psram.===========\r\n");

		// os_printf("YUB test rotate full begin.===========\r\n");
		// jpeg_perfmon(0);
		// offset = 0;
		// for(k = 0; k < test_count/32; k++){
		// 	for(i = 0; i < 8; i++) {
		// 		for(j = 0; j < 4; j++) {
		// 			offset =  YUV_BUF_SIZE_WORD*i*4 + BUF_WIDTH_WORD*j;
		// 			memcpy32_from_psram(inbuf, src_psram + offset, YUV_BUF_SIZE_WORD);
		// 			YUV422ToRGB565_Rotate90(inbuf, outbuf, width, height);
		// 			memcpy32_to_psram(dest_psram + offset, outbuf, YUV_BUF_SIZE_WORD);
		// 		}
		// 	}
		// }
		// jpeg_perfmon(1);
		// os_printf("YUB test rotate full end.===========\r\n");


	} while(0);

	// if(NULL != inbuf) os_free(inbuf);
	// if(NULL != outbuf) os_free(outbuf);

}

void cli_yuv_rotate_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int width;
	int height;
	int test_count;
	uint32_t *src_psram = (uint32_t *)PSRAM_SRC_ADDR;
	uint32_t *dest_psram = (uint32_t *)PSRAM_DEST_ADDR;

	if (argc != 4) {
		cmd_printf("Usage: yuvtest <width> <height> <test_count>.\r\n");
		return;
	}

	width = strtoul(argv[1], NULL, 10);
	height = strtoul(argv[2], NULL, 10);
	test_count = strtoul(argv[3], NULL, 10);

	os_printf("Yuvtest start: ========= width(%d),height(%d),test_count(%d).\r\n", width,height,test_count);
	os_printf("g_yuv_line_word_count(%d),YUV_BUF_SIZE_WORD(%d).\r\n", g_yuv_line_word_count,YUV_BUF_SIZE_WORD);

	test_yuv_rotate(src_psram, dest_psram, width, height, test_count);

	// os_printf("Yuvtest start:=========== Enable psram dcache.\r\n");
	// src_psram = (uint32_t *)PSRAM_SRC_CACHE_ADDR;
	// dest_psram = (uint32_t *)PSRAM_DEST_CACHE_ADDR;
	// test_yuv_rotate(src_psram, dest_psram, width, height, test_count);

}
#endif

void jpeg_dec_read_sdcard_to_psram(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if (CONFIG_FATFS)
	int ret = 0;
	char *filename;
	uint32_t srcaddr;
	// uint32_t dstaddr;
	uint32_t total_size = 0;
	uint32_t total_word_size = 0;

	filename = argv[1]; //saved file name
	os_printf("filename  = %s \r\n", filename);
	srcaddr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
	os_printf("image p_srcaddr  = %X \r\n", srcaddr);
	// dstaddr = os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;
	// os_printf("image p_dstaddr  = %X \r\n", dstaddr);
	// uint8_t * p_srcaddr = (uint8_t *)srcaddr;
	// uint8_t * p_dstaddr = (uint8_t *)dstaddr;

	char cFileName[FF_MAX_LFN];
	FIL file;
	FRESULT fr;
	FSIZE_t size_64bit = 0;
	unsigned int uiTemp = 0;

	// step 1: read picture from sd to psram
	sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);
	char *ucRdTemp = NULL; //(char *)p_srcaddr;
	os_printf("write to psram addr:  %x \r\n", srcaddr);

	/*open jpeg file*/
	fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", filename);
		return;
	}

	do {
		size_64bit = f_size(&file);
		total_size = (uint32_t)size_64bit;// total byte
		os_printf("read file total_size = %d.\r\n", total_size);

		ucRdTemp = os_malloc(total_size + 4);
		if(NULL == ucRdTemp) {
			os_printf("os malloc fail. oom \r\n");
			ret = -1;
			break;
		}
		fr = f_read(&file, ucRdTemp, total_size, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read file fail.\r\n");
			ret = -2;
			break;
		}

		total_word_size = (total_size >> 2) + (total_size&0x3);

		memcpy32((uint32_t *)srcaddr, (uint32_t *)ucRdTemp, total_word_size);
	} while(0);

	fr = f_close(&file);
	if (fr != FR_OK) {
		os_printf("close %s fail!\r\n", filename);
	}

	if(NULL != ucRdTemp) {
		os_free(ucRdTemp);
		ucRdTemp = NULL;
	}

	if(0 == ret) {
		os_printf("file read ok\r\n");
	}

#else
	os_printf("Not support\r\n");
#endif

}


void jpeg_sw_dec_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_JPEGDEC_SW
	int err = 0;
	uint32_t srcaddr;
	uint32_t dstaddr;
	uint32_t total_size = 0;
	uint8_t  format = 3;

	srcaddr = os_strtoul(argv[1], NULL, 16) & 0xFFFFFFFF;
	os_printf("image p_srcaddr  = 0x%X \r\n", srcaddr);
	dstaddr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
	os_printf("image p_dstaddr  = 0x%X \r\n", dstaddr);
	total_size = os_strtoul(argv[3], NULL, 10) & 0xFFFFFFFF;
	os_printf("image total_size  = %d \r\n", total_size);

	if (argc > 4) {
		format = os_strtoul(argv[4], NULL, 10) & 0xFFFFFFFF;
		os_printf("image out format  = %d \r\n", format);
	}

	uint8_t * p_srcaddr = (uint8_t *)srcaddr;
	uint8_t * p_dstaddr = (uint8_t *)dstaddr;

	// init jpeg_dec
	err = bk_jpeg_dec_sw_init();
	if (err != kNoErr) {
		os_printf("init jpeg_decoder failed\r\n");
		return;
	}

	// set jpeg_dec out format
//	bk_jpeg_dec_sw_set_out_format(format);

	// start jpeg_dec
	os_printf("start jpeg_dec.\r\n");
	jpeg_perfmon(0);

	bk_jpeg_dec_sw_register_finish_callback(NULL);
	err = bk_jpeg_dec_sw_start(total_size, p_srcaddr, p_dstaddr, NULL);
	if (err != kNoErr) {
		os_printf("jpeg_decoder failed\r\n");
		return;
	}

	jpeg_perfmon(1);
	os_printf("jpeg_dec ok.\r\n");

	bk_jpeg_dec_sw_deinit();
#else
	os_printf("Not support\r\n");
#endif


}


void jpeg_dec_write_psram_to_sdcard(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if (CONFIG_FATFS)
	int ret = 0;
	char *filename = NULL;
	char cFileName[FF_MAX_LFN];
	FIL file;
	FRESULT fr;
	unsigned int uiTemp = 0;
	uint32_t total_size = 0;
	// uint32_t total_word_size = 0;
	char *ucRdTemp = NULL;

	filename = argv[1]; //saved file name
	os_printf("filename  = %s \r\n", filename);

	total_size = os_strtoul(argv[2], NULL, 10) & 0xFFFFFFFF;
	os_printf("image total_size	= %d \r\n", total_size);

	uint32_t paddr = os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;
	os_printf("read from psram addr = %x \r\n", paddr);

	//	save data to sdcard
	sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);

	fr = f_open(&file, cFileName, FA_OPEN_APPEND | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", filename);
		return;
	}

	do {
		ucRdTemp = (char *)paddr;//os_malloc(total_size + 4);
		// if(NULL == ucRdTemp) {
		// 	os_printf("os malloc fail. oom \r\n");
		// 	ret = -1;
		// 	break;
		// }
		// total_word_size = (total_size >> 2) + (total_size&0x3);
		// memcpy32((uint32_t *)ucRdTemp, (uint32_t *)paddr, total_word_size);

		fr = f_write(&file, (char *)ucRdTemp, total_size , &uiTemp);
		if (fr != FR_OK) {
			os_printf("write %s fail.\r\n", filename);
			ret = -1;
			break;
		}
		os_printf("\n");
	}while(0);


	fr = f_close(&file);
	if (fr != FR_OK) {
		os_printf("close %s fail!\r\n", filename);
	}

	// if(NULL != ucRdTemp) {
	// 	os_free(ucRdTemp);
	// 	ucRdTemp = NULL;
	// }

	if(0 == ret) {
		os_printf("sd card write data to file successful\r\n");
	}

#else
	os_printf("Not support\r\n");
#endif
}


static const struct cli_command s_jpegdec_commands[] = {
#if CONFIG_SLAVE_CORE
	// {"yuvtest", "<width> <height> <test_count> Test yuv rotate", cli_yuv_rotate_cmd},
#else
	{"jpegread", "<filename> <src_addr> Read data from sdcard to psram", jpeg_dec_read_sdcard_to_psram},
	{"jpegwrite", "<filename> <size> <dest_addr> Write data from psram to sdcard", jpeg_dec_write_psram_to_sdcard},
#endif
#if CONFIG_JPEGDEC_SW
	{"jpegdec", "<filename> <src_addr> <dest_addr> Test yuv rotate", jpeg_sw_dec_test},
#endif
};

int cli_jpegdec_init(void)
{
	return cli_register_commands(s_jpegdec_commands, JPEG_DEC_CNT);
}


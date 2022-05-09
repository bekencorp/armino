#ifndef _JPEG_DEC_HAL_H_
#define _JPEG_DEC_HAL_H_

int jpg_decoder_init();
void jpg_decoder_deinit(void);
int jpg_decoder_fun(unsigned char *jpg_buf, unsigned char ** Y_buf, int pic_size);
void jpg_get_pic_size(int *width, int *heigth);

extern void jpg_dec_test(void);
typedef struct _JPG_DECODER_ST
{
	int width;
	int heigth;
	int scale_ratio;//0,1
	int line_wbyte;
	int rd_ptr;
	int jpg_file_size;//jpg file size
	unsigned char *outputbuf;//y data
	unsigned char *inputbuf;//jpg data
	unsigned char *workbuf;

}JPG_DECODER_ST;




#define REG_JPEG_DEC_BASE_ADDR              0x48040000


#define REG_JPEG_ACC_REG0               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x00 * 4)))
#define REG_JPEG_PIPO_REG0              (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x01 * 4)))
#define REG_JPEG_PIPO_REG1              (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x02 * 4)))
#define REG_JPEG_DCUV                   (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x03 * 4)))
#define REG_JPEG_CUR_MASK               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x04 * 4)))
#define REG_JPEG_MCUX                  (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x05 * 4)))
#define REG_JPEG_MCUY                   (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x06 * 4)))
#define REG_JPEG_CURBYTE                (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x07 * 4)))


#define REG_JPEG_CURRUN                 (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x09 * 4)))
#define REG_JPEG_XPIXEL                 (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0a * 4)))
#define REG_JPEG_HUF_DATA               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0b * 4)))

#define REG_JPEG_STATE                   (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0d * 4)))

#define REG_JPED_DEC_RDY                 (REG_JPEG_STATE & 0x1)
#define REG_JPED_RRLOAD                  (REG_JPEG_STATE & 0x2)
#define REG_JPED_DEC_RELOAD              (REG_JPEG_STATE & 0x4)
#define REG_JPED_DEC_SEARCH              (REG_JPEG_STATE & 0x8)
#define REG_JPED_EXT_RELOAD              (REG_JPEG_STATE & 0x10)
#define REG_JPED_EXT_SEARCH              (REG_JPEG_STATE & 0x20)
#define REG_IDCT_BUSY                    (REG_JPEG_STATE & 0x40)
#define TMP_MEM_CLR_BUSY                 (REG_JPEG_STATE & 0x80)
#define REG_JPED_DEC_RDY2                (REG_JPEG_STATE & 0x100)

#define REG_CUR_NBIT                     (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0c * 4)))
#define REG_JPEG_EXT_DATA                (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0e * 4)))
#define REG_JPEG_BLK                     (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0f * 4)))

#define REG_DEC_CMD                     (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x08 * 4)))

#define REG_DEC_START                   (REG_DEC_CMD = 0x01);
#define REG_DEC_TMP_CLR                 (REG_DEC_CMD = 0x02);
#define REG_DC_CLR                      (REG_DEC_CMD = 0x04);
#define BASE_LOAD                       (REG_DEC_CMD = 0x08);

#define REG_JPEG_BITS_DC00               addJPEG_DEC_Reg0x10
#define REG_JPEG_BITS_DC01               addJPEG_DEC_Reg0x11
#define REG_JPEG_BITS_DC02               addJPEG_DEC_Reg0x12
#define REG_JPEG_BITS_DC03               addJPEG_DEC_Reg0x13
#define REG_JPEG_BITS_DC04               addJPEG_DEC_Reg0x14
#define REG_JPEG_BITS_DC05               addJPEG_DEC_Reg0x15
#define REG_JPEG_BITS_DC06               addJPEG_DEC_Reg0x16
#define REG_JPEG_BITS_DC07               addJPEG_DEC_Reg0x17
#define REG_JPEG_BITS_DC08               addJPEG_DEC_Reg0x18
#define REG_JPEG_BITS_DC09               addJPEG_DEC_Reg0x19
#define REG_JPEG_BITS_DC0A               addJPEG_DEC_Reg0x1a
#define REG_JPEG_BITS_DC0B               addJPEG_DEC_Reg0x1b
#define REG_JPEG_BITS_DC0C               addJPEG_DEC_Reg0x1c
#define REG_JPEG_BITS_DC0D               addJPEG_DEC_Reg0x1d
#define REG_JPEG_BITS_DC0E               addJPEG_DEC_Reg0x1e
#define REG_JPEG_BITS_DC0F               addJPEG_DEC_Reg0x1f

#define REG_JPEG_BITS_DC10               addJPEG_DEC_Reg0x20
#define REG_JPEG_BITS_DC11               addJPEG_DEC_Reg0x21
#define REG_JPEG_BITS_DC12               addJPEG_DEC_Reg0x22
#define REG_JPEG_BITS_DC13               addJPEG_DEC_Reg0x23
#define REG_JPEG_BITS_DC14               addJPEG_DEC_Reg0x24
#define REG_JPEG_BITS_DC15               addJPEG_DEC_Reg0x25
#define REG_JPEG_BITS_DC16               addJPEG_DEC_Reg0x26
#define REG_JPEG_BITS_DC17               addJPEG_DEC_Reg0x27
#define REG_JPEG_BITS_DC18               addJPEG_DEC_Reg0x28
#define REG_JPEG_BITS_DC19               addJPEG_DEC_Reg0x29
#define REG_JPEG_BITS_DC1A               addJPEG_DEC_Reg0x2a
#define REG_JPEG_BITS_DC1B               addJPEG_DEC_Reg0x2b
#define REG_JPEG_BITS_DC1C               addJPEG_DEC_Reg0x2c
#define REG_JPEG_BITS_DC1D               addJPEG_DEC_Reg0x2d
#define REG_JPEG_BITS_DC1E               addJPEG_DEC_Reg0x2e
#define REG_JPEG_BITS_DC1F               addJPEG_DEC_Reg0x2f



#define REG_JPEG_BITS_AC00              addJPEG_DEC_Reg0x30 
#define REG_JPEG_BITS_AC01              addJPEG_DEC_Reg0x31 
#define REG_JPEG_BITS_AC02              addJPEG_DEC_Reg0x32 
#define REG_JPEG_BITS_AC03              addJPEG_DEC_Reg0x33 
#define REG_JPEG_BITS_AC04              addJPEG_DEC_Reg0x34 
#define REG_JPEG_BITS_AC05              addJPEG_DEC_Reg0x35 
#define REG_JPEG_BITS_AC06              addJPEG_DEC_Reg0x36 
#define REG_JPEG_BITS_AC07              addJPEG_DEC_Reg0x37 
#define REG_JPEG_BITS_AC08              addJPEG_DEC_Reg0x38 
#define REG_JPEG_BITS_AC09              addJPEG_DEC_Reg0x39 
#define REG_JPEG_BITS_AC0A              addJPEG_DEC_Reg0x3a 
#define REG_JPEG_BITS_AC0B              addJPEG_DEC_Reg0x3b 
#define REG_JPEG_BITS_AC0C              addJPEG_DEC_Reg0x3c 
#define REG_JPEG_BITS_AC0D              addJPEG_DEC_Reg0x3d 
#define REG_JPEG_BITS_AC0E              addJPEG_DEC_Reg0x3e 
#define REG_JPEG_BITS_AC0F              addJPEG_DEC_Reg0x3f 

#define REG_JPEG_BITS_AC10              addJPEG_DEC_Reg0x40 
#define REG_JPEG_BITS_AC11              addJPEG_DEC_Reg0x41 
#define REG_JPEG_BITS_AC12              addJPEG_DEC_Reg0x42 
#define REG_JPEG_BITS_AC13              addJPEG_DEC_Reg0x43 
#define REG_JPEG_BITS_AC14              addJPEG_DEC_Reg0x44 
#define REG_JPEG_BITS_AC15              addJPEG_DEC_Reg0x45 
#define REG_JPEG_BITS_AC16              addJPEG_DEC_Reg0x46 
#define REG_JPEG_BITS_AC17              addJPEG_DEC_Reg0x47 
#define REG_JPEG_BITS_AC18              addJPEG_DEC_Reg0x48 
#define REG_JPEG_BITS_AC19              addJPEG_DEC_Reg0x49 
#define REG_JPEG_BITS_AC1A              addJPEG_DEC_Reg0x4a 
#define REG_JPEG_BITS_AC1B              addJPEG_DEC_Reg0x4b 
#define REG_JPEG_BITS_AC1C              addJPEG_DEC_Reg0x4c 
#define REG_JPEG_BITS_AC1D              addJPEG_DEC_Reg0x4d 
#define REG_JPEG_BITS_AC1E              addJPEG_DEC_Reg0x4e 
#define REG_JPEG_BITS_AC1F              addJPEG_DEC_Reg0x4f 

#define TIME_STATE                       (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x50 * 4)))
#define DEBUG0                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x51 * 4)))
#define DEBUG1                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x52 * 4)))
#define DEBUG2                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x53 * 4)))
#define DEBUG3                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x54 * 4)))
#define DEBUG4                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x55 * 4)))
#define DEBUG5                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x56 * 4)))
#define BASE_RADDR                       (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x58 * 4)))
#define BASE_WADDR                       (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x59 * 4)))
#define BASE_RD_LEN                      (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x5a * 4)))
#define BASE_WR_LEN                      (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x5b * 4)))
#define BASE_FFDA                        (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x5c * 4)))

#define JPEGDEC_INTEN                    (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x5e * 4)))
#define JPEGDEC_INTSTAT                  (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x5f * 4)))

#define dec_busy_clr                     (JPEGDEC_INTSTAT = 0x01);
#define rrload_clr                       (JPEGDEC_INTSTAT = 0x02);
#define reload_clr                       (JPEGDEC_INTSTAT = 0x04);
#define search_clr                       (JPEGDEC_INTSTAT = 0x08);
#define ext_reload_clr                   (JPEGDEC_INTSTAT = 0x10);
#define ext_bit_clr                      (JPEGDEC_INTSTAT = 0x20);
#define dec_busy2_clr                    (JPEGDEC_INTSTAT = 0x40);
#define mcu_finish_clr                   (JPEGDEC_INTSTAT = 0x80);


#define REG_JPEG_HUF_TABLE00            *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x80 * 4))
#define REG_JPEG_HUF_TABLE10            *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0xc0 * 4))
#define REG_JPEG_HUF_TABLE01            *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x100 * 4))
#define REG_JPEG_HUF_TABLE11            *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x200 * 4))
#define REG_JPEG_Zig                    *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x300 * 4))
#define REG_JPEG_TMP0                   *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x340 * 4))
#define REG_JPEG_DQT_TABLE0             *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x380 * 4))
#define REG_JPEG_DQT_TABLE1             *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3c0 * 4))

/*
#define REG_JPEG_DEC_BASE_ADDR              (0x0080E000)

#define REG_JPEG_BUF_POINT              (*((volatile unsigned long *)(REG_JPEG_DEC_BUF_ADDR + 0x00 * 4)))

#define REG_JPEG_ACC_REG0               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x00 * 4)))
#define REG_JPEG_PIPO_REG0              (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x01 * 4)))
#define REG_JPEG_PIPO_REG1              (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x02 * 4)))
#define REG_JPEG_DCUV                   (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x03 * 4)))
#define REG_JPEG_CUR_MASK               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x04 * 4)))
#define REG_JPEG_TEST0                  (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x05 * 4)))
#define REG_JPEG_TEST1                  (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x06 * 4)))	
#define REG_JPEG_CURBYTE                (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x07 * 4)))	
#define REG_DEC_CMD                     (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x08 * 4)))

#define REG_DEC_START                   (REG_DEC_CMD = 0x01);
#define REG_DEC_TMP_CLR                 (REG_DEC_CMD = 0x02);
#define REG_DC_CLR                      (REG_DEC_CMD = 0x04);
#define BASE_LOAD                       (REG_DEC_CMD = 0x08);

#define REG_JPEG_CURRUN                 (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x09 * 4)))	
#define REG_JPEG_XPIXEL                 (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0a * 4)))
#define REG_JPEG_HUF_DATA               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0b * 4)))

#define REG_JPEG_STATE                   (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0d * 4)))
	
#define REG_JPED_DEC_RDY                 (REG_JPEG_STATE & 0x1)
#define REG_JPED_RRLOAD                  (REG_JPEG_STATE & 0x2)
#define REG_JPED_DEC_RELOAD              (REG_JPEG_STATE & 0x4)
#define REG_JPED_DEC_SEARCH              (REG_JPEG_STATE & 0x8)
#define REG_JPED_EXT_RELOAD              (REG_JPEG_STATE & 0x10)
#define REG_JPED_EXT_SEARCH              (REG_JPEG_STATE & 0x20)
#define REG_IDCT_BUSY                    (REG_JPEG_STATE & 0x40)
#define TMP_MEM_CLR_BUSY                 (REG_JPEG_STATE & 0x80)

#define REG_CUR_NBIT                     (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0c * 4)))
#define REG_JPEG_EXT_DATA                (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0e * 4)))
#define REG_JPEG_BLK                     (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x0f * 4)))
//#define REG_JPEG_PIPO_EMPT0              REG_JPEG_PIPO_EMPT & 0x1;
//#define REG_JPEG_PIPO_EMPT1              REG_JPEG_PIPO_EMPT & 0x2;


#define REG_JPEG_BITS_DC00               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x10 * 4)))
#define REG_JPEG_BITS_DC01               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x11 * 4)))
#define REG_JPEG_BITS_DC02               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x12 * 4)))
#define REG_JPEG_BITS_DC03               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x13 * 4)))
#define REG_JPEG_BITS_DC04               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x14 * 4)))
#define REG_JPEG_BITS_DC05               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x15 * 4)))
#define REG_JPEG_BITS_DC06               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x16 * 4)))
#define REG_JPEG_BITS_DC07               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x17 * 4)))
#define REG_JPEG_BITS_DC08               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x18 * 4)))
#define REG_JPEG_BITS_DC09               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x19 * 4)))
#define REG_JPEG_BITS_DC0A               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1a * 4)))
#define REG_JPEG_BITS_DC0B               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1b * 4)))
#define REG_JPEG_BITS_DC0C               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1c * 4)))
#define REG_JPEG_BITS_DC0D               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1d * 4)))
#define REG_JPEG_BITS_DC0E               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1e * 4)))
#define REG_JPEG_BITS_DC0F               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1f * 4)))

#define REG_JPEG_BITS_DC10               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x20 * 4)))
#define REG_JPEG_BITS_DC11               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x21 * 4)))
#define REG_JPEG_BITS_DC12               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x22 * 4)))
#define REG_JPEG_BITS_DC13               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x23 * 4)))
#define REG_JPEG_BITS_DC14               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x24 * 4)))
#define REG_JPEG_BITS_DC15               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x25 * 4)))
#define REG_JPEG_BITS_DC16               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x26 * 4)))
#define REG_JPEG_BITS_DC17               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x27 * 4)))
#define REG_JPEG_BITS_DC18               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x28 * 4)))
#define REG_JPEG_BITS_DC19               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x29 * 4)))
#define REG_JPEG_BITS_DC1A               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2a * 4)))
#define REG_JPEG_BITS_DC1B               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2b * 4)))
#define REG_JPEG_BITS_DC1C               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2c * 4)))
#define REG_JPEG_BITS_DC1D               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2d * 4)))
#define REG_JPEG_BITS_DC1E               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2e * 4)))
#define REG_JPEG_BITS_DC1F               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2f * 4)))



#define REG_JPEG_BITS_AC00               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x30 * 4)))
#define REG_JPEG_BITS_AC01               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x31 * 4)))
#define REG_JPEG_BITS_AC02               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x32 * 4)))
#define REG_JPEG_BITS_AC03               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x33 * 4)))
#define REG_JPEG_BITS_AC04               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x34 * 4)))
#define REG_JPEG_BITS_AC05               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x35 * 4)))
#define REG_JPEG_BITS_AC06               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x36 * 4)))
#define REG_JPEG_BITS_AC07               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x37 * 4)))
#define REG_JPEG_BITS_AC08               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x38 * 4)))
#define REG_JPEG_BITS_AC09               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x39 * 4)))
#define REG_JPEG_BITS_AC0A               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3a * 4)))
#define REG_JPEG_BITS_AC0B               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3b * 4)))
#define REG_JPEG_BITS_AC0C               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3c * 4)))
#define REG_JPEG_BITS_AC0D               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3d * 4)))
#define REG_JPEG_BITS_AC0E               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3e * 4)))
#define REG_JPEG_BITS_AC0F               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3f * 4)))

#define REG_JPEG_BITS_AC10               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x40 * 4)))
#define REG_JPEG_BITS_AC11               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x41 * 4)))
#define REG_JPEG_BITS_AC12               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x42 * 4)))
#define REG_JPEG_BITS_AC13               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x43 * 4)))
#define REG_JPEG_BITS_AC14               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x44 * 4)))
#define REG_JPEG_BITS_AC15               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x45 * 4)))
#define REG_JPEG_BITS_AC16               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x46 * 4)))
#define REG_JPEG_BITS_AC17               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x47 * 4)))
#define REG_JPEG_BITS_AC18               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x48 * 4)))
#define REG_JPEG_BITS_AC19               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x49 * 4)))
#define REG_JPEG_BITS_AC1A               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4a * 4)))
#define REG_JPEG_BITS_AC1B               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4b * 4)))
#define REG_JPEG_BITS_AC1C               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4c * 4)))
#define REG_JPEG_BITS_AC1D               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4d * 4)))
#define REG_JPEG_BITS_AC1E               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4e * 4)))
#define REG_JPEG_BITS_AC1F               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4f * 4)))
	
#define TIME_STATE                       (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x50 * 4)))
#define DEBUG0                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x51 * 4)))
#define DEBUG1                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x52 * 4)))
#define DEBUG2                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x53 * 4)))
#define DEBUG3                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x54 * 4)))
#define DEBUG4                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x55 * 4)))
#define DEBUG5                           (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x56 * 4)))
#define BASE_RADDR                       (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x58 * 4)))
#define BASE_WADDR                       (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x59 * 4)))
#define BASE_RD_LEN                      (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x5a * 4)))
#define BASE_WR_LEN                      (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x5b * 4)))	
#define BASE_FFDA                        (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x5c * 4)))	

#define REG_JPEG_HUF_TABLE00            *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x80 * 4))
#define REG_JPEG_HUF_TABLE10            *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0xc0 * 4))
#define REG_JPEG_HUF_TABLE01            *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x100 * 4))
#define REG_JPEG_HUF_TABLE11            *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x200 * 4))
#define REG_JPEG_Zig                    *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x300 * 4))
#define REG_JPEG_TMP0                   *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x340 * 4))
#define REG_JPEG_DQT_TABLE0             *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x380 * 4))	
#define REG_JPEG_DQT_TABLE1             *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3c0 * 4))
//#define REG_JPEG_TMP1                   *((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3c0 * 4))		
*/


#endif

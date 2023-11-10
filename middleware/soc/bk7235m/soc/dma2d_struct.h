// Copyright 2020-2021 Beken 
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


#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {

    /* REG_0x00 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tran_start                     : 1;  //0x0[       0],dma2d transfer start.,0,RW
           volatile uint32_t  tran_suspend                   : 1;  //0x0[       1],dma2d transfer start.,0,RW
           volatile uint32_t  tran_abort                     : 1;  //0x0[       2],dma2d transfer start.,0,RW
           volatile uint32_t  reserved0                       : 3;  //0x0[  5:  3],NC,0,RW
           volatile uint32_t  line_offset_mode               : 1;  //0x0[       6],line's offset mode sel: 0:in pixel express; 1: in bytes express.,0,RW
           volatile uint32_t  reserved1                       : 1;  //0x0[       7],NC,0,RW
           volatile uint32_t  error_int_en                   : 1;  //0x0[       8],trabsfer error int ena.,0,RW
           volatile uint32_t  complete_int_en                : 1;  //0x0[       9],transfer complete int ena.,0,RW
           volatile uint32_t  waterm_int_en                  : 1;  //0x0[     10],transfer watermark int ena.,0,RW
           volatile uint32_t  clut_error_int_en              : 1;  //0x0[     11],clut transfer error int ena.,0,RW
           volatile uint32_t  clut_cmplt_int_en              : 1;  //0x0[     12],clut transfer complete int ena.,0,RW
           volatile uint32_t  config_error_int_en            : 1;  //0x0[     13],config error int ena,,0,RW
           volatile uint32_t  reserved2                       : 2;  //0x0[15:14],NC,0,R
           volatile uint32_t  mode                           : 3;  //0x0[18:16],DMA2D mode sel: 000:m2m; 001:m2m pixel convert with fg; 010:m2m blend; 011:r2m.only with output; 100: m2m blend fix fg; 101:m2m blend fix bg;,0,RW
		   volatile uint32_t  master_tran_length			 :2;  //0x0[20:19],RW max transfer length:00:256bytes; 01:192 bytes;  10:128bytes;	11:64bytes;
		   volatile uint32_t  out_byte_revese				 :1;  //0x0[21],RW output rgb888 formart, reverse data byte by byte.
		   volatile uint32_t  reserved						 :13;  //0x0[31:21],NC,0,R
        }; 
        uint32_t v;
    }dma2d_control_reg; 

    /* REG_0x01 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  error_int                      : 1;  //0x1[       0],transfer error int flag.,0,R
           volatile uint32_t  complete_int                   : 1;  //0x1[       1],transfer complete int flag.,0,R
           volatile uint32_t  waterm_int                     : 1;  //0x1[       2],transfer watermark intt flag.,0,R
           volatile uint32_t  clut_error_int                 : 1;  //0x1[       3],clut transfer error intt flag.,0,R
           volatile uint32_t  clut_cmplt_int                 : 1;  //0x1[       4],clut transfer complete intt flag.,0,R
           volatile uint32_t  config_error_int               : 1;  //0x1[       5],config error int t flag.,0,R
           volatile uint32_t  reserved                       :26;  //0x1[31:  6],NC,0,R
        }; 
        uint32_t v; 
    }dma2d_int_status; 

    /* REG_0x02 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  clr_error_int                  : 1;  //0x2[       0],clear transfer error int flag.,0,W
           volatile uint32_t  clr_complete_int               : 1;  //0x2[       1],clear transfer complete int flag.,0,W
           volatile uint32_t  clr_waterm_int                 : 1;  //0x2[       2],clear transfer watermark intt flag.,0,W
           volatile uint32_t  clr_clut_error_int             : 1;  //0x2[       3],clear clut transfer error intt flag.,0,W
           volatile uint32_t  clr_clut_cmplt_int             : 1;  //0x2[       4],clear clut transfer complete intt flag.,0,W
           volatile uint32_t  clr_config_error_int           : 1;  //0x2[       5],clear config error int t flag.,0,W
           volatile uint32_t  reserved                       :26;  //0x2[31:  6],NC,0,W
        }; 
        uint32_t v; 
    }dma2d_int_clear; 

    /* REG_0x03 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  fg_address                     :32;  //0x3[31:  0],foreground mem address, written when transfer disable.,0,RW
        }; 
        uint32_t v; 
    }dma2d_fg_address; 

    /* REG_0x04 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  fg_line_offset                 :16;  //0x4[15:  0],line offset for fg, offset In pixel or bytes.,0,RW
           volatile uint32_t  reserved                       :16;  //0x4[31:16],NC,0,RW
        }; 
        uint32_t v; 
    }dma2d_fg_offset; 

    /* REG_0x05 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  bg_address                     :32;  //0x5[31:  0],background mem address, written when transfer disable.,0,RW
        }; 
        uint32_t v; 
    }dma2d_bg_address; 

    /* REG_0x06 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  bg_line_offset                 :16;  //0x6[15:  0],line offset for bg, offset In pixel or bytes.,0,RW
           volatile uint32_t  reserved                       :16;  //0x6[31:16],NC,0,RW
        }; 
        uint32_t v; 
    }dma2d_bg_offset; 

    /* REG_0x07 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  fg_color_mode                  : 4;  //0x7[  3:  0],foreground image color mode.; 0000:ARGB888;0001:RGB888;0010:RGB565;0011:ARGB1555;0100:ARGB4444; 0101:L8;0110:AL44; 0111:AL88; 1000:L4; 1001:A8;1010:A4;,0,RW
           volatile uint32_t  fg_clut_color_mode             : 1;  //0x7[       4],color mode for clut. 0:argb8888; 1:rgb888.,0,RW
           volatile uint32_t  fg_start_clut                  : 1;  //0x7[       5],automatic load the clut. Automatic reset.,0,RW
           volatile uint32_t  reserved0                       : 2;  //0x7[  7:  6],NC,0,RW
           volatile uint32_t  fg_clut_size                   : 8;  //0x7[15:  8],the size of clut used for foreground image. Size = fg_clut_size + 1;,0,RW
           volatile uint32_t  fg_alpha_mode                  : 2;  //0x7[17:16],alpha value use for fg image. 00: nochange; 01:replace orginal, 10: alpha[7:0] multipied with orginal value.,0,RW
           volatile uint32_t  reserved1                       : 2;  //0x7[19:18],NC,0,RW
           volatile uint32_t  alpha_invert                   : 1;  //0x7[     20],invert alpha value.,0,RW
           volatile uint32_t  fg_rb_swap                     : 1;  //0x7[     21],red blue swap to support rgb or argb. 0: regular mode.1:swap_mode.,0,RW
           volatile uint32_t  reserved                       : 2;  //0x7[23:22],NC,0,RW
           volatile uint32_t  fg_alpha                       : 8;  //0x7[31:24],fg alpha value set. Use with fg_alpha_mode.,0,RW
        }; 
        uint32_t v; 
    }dma2d_fg_pfc_ctrl; 

    /* REG_0x08 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  blue_value                     : 8;  //0x8[  7:  0],blue_value in a4 or a8 mode of fg,,0,RW
           volatile uint32_t  green_value                    : 8;  //0x8[15:  8],green_value in a4 or a8 mode of fg,,0,RW
           volatile uint32_t  red_value                      : 8;  //0x8[23:16],red_value in a4 or a8 mode of fg,,0,RW
           volatile uint32_t  reserved                       : 8;  //0x8[31:24],NC,0,RW
        }; 
        uint32_t v; 
    }dma2d_fg_color_reg; 

    /* REG_0x09 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  bg_color_mode                  : 4;  //0x9[  3:  0],background image color mode.; 0000:ARGB888;0001:RGB888;0010:RGB565;0011:ARGB1555;0100:ARGB4444; 0101:L8;0110:AL44; 0111:AL88; 1000:L4; 1001:A8;1010:A4;,0,RW
           volatile uint32_t  bg_clut_color_mode             : 1;  //0x9[       4],color mode for clut. 0:argb8888; 1:rgb888.,0,RW
           volatile uint32_t  bg_start_clut                  : 1;  //0x9[       5],automatic load the clut. Automatic reset.,0,RW
           volatile uint32_t  reserved0                       : 2;  //0x9[  7:  6],NC,0,RW
           volatile uint32_t  bg_clut_size                   : 8;  //0x9[15:  8],the size of clut used for background image. Size = fg_clut_size + 1;,0,RW
           volatile uint32_t  bg_alpha_mode                  : 2;  //0x9[17:16],alpha value use for fg image. 00: nochange; 01:replace orginal, 10: alpha[7:0] multipied with orginal value.,0,RW
           volatile uint32_t  reserved1                       : 2;  //0x9[19:18],NC,0,RW
           volatile uint32_t  alpha_invert                   : 1;  //0x9[     20],invert alpha value.,0,RW
           volatile uint32_t  bg_rb_swap                     : 1;  //0x9[     21],red blue swap to support rgb or argb. 0: regular mode.1:swap_mode.,0,RW
           volatile uint32_t  reserved                       : 2;  //0x9[23:22],NC,0,RW
           volatile uint32_t  bg_alpha                       : 8;  //0x9[31:24],bg alpha value set. Use withbg_alpha_mode.,0,RW
        }; 
        uint32_t v; 
    }dma2d_bg_pfc_ctrl; 

    /* REG_0x0a */
    union 
    { 
        struct 
        { 
           volatile uint32_t  blue_value                     : 8;  //0xa[  7:  0],blue_value in a4 or a8 mode of bg,,0,RW
           volatile uint32_t  green_value                    : 8;  //0xa[15:  8],green_value in a4 or a8 mode of bg,,0,RW
           volatile uint32_t  red_value                      : 8;  //0xa[23:16],red_value in a4 or a8 mode of bg,,0,RW
           volatile uint32_t  reserved                       : 8;  //0xa[31:24],NC,0,RW
        }; 
        uint32_t v; 
    }dma2d_bg_color_reg; 

    /* REG_0x0b */
    union 
    { 
        struct 
        { 
           volatile uint32_t  fg_clut_address                :32;  //0xb[31: 0],clut data dedicated to the fg image.,0,RW
        }; 
        uint32_t v; 
    }fg_clut_mem_address; 

    /* REG_0x0c */
    union 
    { 
        struct 
        { 
           volatile uint32_t  bg_clut_address                :32;  //0xc[31:  0],clut data dedicated to the bg image.,0,RW
        }; 
        uint32_t v; 
    }bg_clut_mem_address; 

    /* REG_0x0d */
    union 
    { 
        struct 
        { 
           volatile uint32_t  out_color_mode                 : 3;  //0xd[  2:  0],format of output image.0:argb888; 1:rgb888; 010:rgb565; 011:argb1555; 100:argb444,0,RW
           volatile uint32_t  reserved0                       : 6;  //0xd[  8:  3],NC,0,RW
           volatile uint32_t  out_swap_bytes                 : 1;  //0xd[       9],0:bytes in regular order. 1:bytes swaped two by two in output fifo.,0,RW
           volatile uint32_t  reserved1                       :10;  //0xd[19:10],NC,0,RW
           volatile uint32_t  out_alpha_invert              :1;  //0xd[     20],invert alpha value.,0,RW
           volatile uint32_t  out_rb_swap                    : 1;  //0xd[     21],ed blue swap to support rgb or argb. 0: regular mode.1:swap_mode.,0,RW
           volatile uint32_t  reserved                       :10;  //0xd[31:22],NC,0,RW
        }; 
        uint32_t v; 
    }out_pfc_contrl; 

    /* REG_0x0e */
    union 
    { 
        struct 
        { 
           volatile uint32_t  output_clor_reg                :32;  //0xe[31:  0],output reg, in different color mode. Output is different.; Argb888 or rgb888: alpha[31:24];red[23:16]; green[15:8];blue[7:0].; Rgb565:red[15:11]; green[12:5]; blue[4:0];; argb1555:a[15];red[14:10]; green[4:0]; blue[4:0];; argb4444:alpha[15:12];red[11:8];green[7:4];blue[3;0].,0,RW
        }; 
        uint32_t v; 
    }out_color_reg; 

    /* REG_0x0f */
    union 
    { 
        struct 
        { 
           volatile uint32_t  out_mem_address                :32;  //0xf[31:  0],address of data output fifo.,0,RW
        }; 
        uint32_t v; 
    }dma2d_out_mem_address; 

    /* REG_0x10 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  out_line_offset                :16;  //0x10[15:  0],output line offset. Offfset with bytes or pixel.in pixel[15:14] ignored.,0,RW
           volatile uint32_t  reserved                       :16;  //0x10[31:16],NC,0,R
        }; 
        uint32_t v; 
    }output_offset; 

    /* REG_0x11 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  number_line                    :16;  //0x11[15:  0],X PIXEL.,0,RW
           volatile uint32_t  pixel_line                     :14;  //0x11[29:16],Y_PIXEL.,None,RW
           volatile uint32_t  reserved                       : 2;  //0x11[31:30],NC,0,RW
        }; 
        uint32_t v; 
    }dma2d_number_of_line; 

    /* REG_0x12 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  line_watermark                 :16;  //0x12[15:  0],config the line watermark int generation, transfer reach the watermark, int hold on.,0,RW
           volatile uint32_t  reserved                       :16;  //0x12[31:16],NC,0,RW
        }; 
        uint32_t v;
    }dma2d_line_watermark; 

    /* REG_0x13 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  master_time_en                : 1;  //0x18[       0],enable dead time function.,0,RW
           volatile uint32_t  reserved0                       : 7;  //0x18[  7:  1],NC,0,RW
           volatile uint32_t  dead_time                      : 8;  //0x18[15:  8],dead time value in ahb clock cycle inserted between two consecutive accesses on ahb master.,0,RW
           volatile uint32_t  reserved                       :16;  //0x18[31:16],NC,0,RW
        };
        uint32_t v; 
    }dma2d_master_time_config; 

    /* REG_0x14 ~0xff  offset 0x50 ~0x3fc  reserved*/
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x14_reserved;

    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x15_reserved; 

    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x16_reserved; 

    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x17_reserved; 

    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x18_reserved; 


    /* REG_0x19*/
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x19_reserved; 

    /* REG_0x1a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a_reserved; 

    /* REG_0x1b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b_reserved; 

    /* REG_0x1c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c_reserved; 

    /* REG_0x1d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d_reserved; 

    /* REG_0x1e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e_reserved; 

    /* REG_0x1f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f_reserved; 

    /* REG_0x20 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x20_reserved; 

    /* REG_0x21 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x21_reserved; 

    /* REG_0x22 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x22_reserved; 

    /* REG_0x23 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x23_reserved; 

    /* REG_0x24 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x24_reserved; 

    /* REG_0x25 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x25_reserved; 

    /* REG_0x26 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x26_reserved; 

    /* REG_0x27 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x27_reserved; 

    /* REG_0x28 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x28_reserved; 

    /* REG_0x29 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x29_reserved; 

    /* REG_0x2a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2a_reserved; 

    /* REG_0x2b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2b_reserved; 

    /* REG_0x2c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2c_reserved; 

    /* REG_0x2d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2d_reserved; 

    /* REG_0x2e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2e_reserved; 

    /* REG_0x2f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x2f_reserved; 

    /* REG_0x30 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x30_reserved; 

    /* REG_0x31 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x31_reserved; 

    /* REG_0x32 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x32_reserved; 

    /* REG_0x33 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x33_reserved; 

    /* REG_0x34 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x34_reserved; 

    /* REG_0x35 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x35_reserved; 

    /* REG_0x36 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x36_reserved; 

    /* REG_0x37 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x37_reserved; 

    /* REG_0x38 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x38_reserved; 

    /* REG_0x39 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x39_reserved; 

    /* REG_0x3a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x3a_reserved; 

    /* REG_0x3b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x3b_reserved; 

    /* REG_0x3c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x3c_reserved; 

    /* REG_0x3d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x3d_reserved; 

    /* REG_0x3e */
    union 
    {
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v;
    }reg_0x3e_reserved; 

    /* REG_0x3f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x3f_reserved; 

    /* REG_0x40 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x40_reserved; 

    /* REG_0x41 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x41_reserved; 

    /* REG_0x42 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x42_reserved; 

    /* REG_0x43 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x43_reserved; 

    /* REG_0x44 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x44_reserved; 

    /* REG_0x45 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x45_reserved; 

    /* REG_0x46 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x46_reserved; 

    /* REG_0x47 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x47_reserved; 

    /* REG_0x48 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x48_reserved; 

    /* REG_0x49 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x49_reserved; 

    /* REG_0x4a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4a_reserved; 

    /* REG_0x4b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4b_reserved; 

    /* REG_0x4c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4c_reserved; 

    /* REG_0x4d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4d_reserved; 

    /* REG_0x4e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4e_reserved; 

    /* REG_0x4f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x4f_reserved; 

    /* REG_0x50 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x50_reserved; 

    /* REG_0x51 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x51_reserved; 

    /* REG_0x52 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x52_reserved; 

    /* REG_0x53 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x53_reserved; 

    /* REG_0x54 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x54_reserved; 

    /* REG_0x55 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x55_reserved; 

    /* REG_0x56 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x56_reserved; 

    /* REG_0x57 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x57_reserved; 

    /* REG_0x58 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x58_reserved; 

    /* REG_0x59 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x59_reserved; 

    /* REG_0x5a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5a_reserved; 

    /* REG_0x5b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5b_reserved; 

    /* REG_0x5c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5c_reserved; 

    /* REG_0x5d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5d_reserved; 

    /* REG_0x5e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5e_reserved; 

    /* REG_0x5f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x5f_reserved; 

    /* REG_0x60 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x60_reserved; 

    /* REG_0x61 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x61_reserved; 

    /* REG_0x62 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x62_reserved; 

    /* REG_0x63 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x63_reserved; 

    /* REG_0x64 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x64_reserved; 

    /* REG_0x65 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x65_reserved; 

    /* REG_0x66 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x66_reserved; 

    /* REG_0x67 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x67_reserved; 

    /* REG_0x68 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x68_reserved; 

    /* REG_0x69 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x69_reserved; 

    /* REG_0x6a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6a_reserved; 

    /* REG_0x6b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6b_reserved; 

    /* REG_0x6c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6c_reserved; 

    /* REG_0x6d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6d_reserved; 

    /* REG_0x6e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6e_reserved; 

    /* REG_0x6f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x6f_reserved; 

    /* REG_0x70 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x70_reserved; 

    /* REG_0x71 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x71_reserved; 

    /* REG_0x72 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x72_reserved; 

    /* REG_0x73 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x73_reserved; 

    /* REG_0x74 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x74_reserved; 

    /* REG_0x75 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x75_reserved; 

    /* REG_0x76 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x76_reserved; 

    /* REG_0x77 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x77_reserved; 

    /* REG_0x78 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x78_reserved; 

    /* REG_0x79 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x79_reserved; 

    /* REG_0x7a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x7a_reserved; 

    /* REG_0x7b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x7b_reserved; 

    /* REG_0x7c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x7c_reserved; 

    /* REG_0x7d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x7d_reserved; 

    /* REG_0x7e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x7e_reserved; 

    /* REG_0x7f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x7f_reserved; 

    /* REG_0x80 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x80_reserved; 

    /* REG_0x81 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x81_reserved; 

    /* REG_0x82 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x82_reserved; 

    /* REG_0x83 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x83_reserved; 

    /* REG_0x84 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x84_reserved; 

    /* REG_0x85 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x85_reserved; 

    /* REG_0x86 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x86_reserved; 

    /* REG_0x87 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x87_reserved; 

    /* REG_0x88 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x88_reserved; 

    /* REG_0x89 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x89_reserved; 

    /* REG_0x8a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x8a_reserved; 

    /* REG_0x8b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x8b_reserved; 

    /* REG_0x8c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x8c_reserved; 

    /* REG_0x8d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x8d_reserved; 

    /* REG_0x8e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x8e_reserved; 

    /* REG_0x8f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x8f_reserved; 

    /* REG_0x90 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x90_reserved; 

    /* REG_0x91 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x91_reserved; 

    /* REG_0x92 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x92_reserved; 

    /* REG_0x93 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x93_reserved; 

    /* REG_0x94 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x94_reserved; 

    /* REG_0x95 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x95_reserved; 

    /* REG_0x96 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x96_reserved; 

    /* REG_0x97 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x97_reserved; 

    /* REG_0x98 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x98_reserved; 

    /* REG_0x99 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x99_reserved; 

    /* REG_0x9a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x9a_reserved; 

    /* REG_0x9b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x9b_reserved; 

    /* REG_0x9c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x9c_reserved; 

    /* REG_0x9d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x9d_reserved; 

    /* REG_0x9e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x9e_reserved; 

    /* REG_0x9f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x9f_reserved; 

    /* REG_0xa0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xa0_reserved; 

    /* REG_0xa1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xa1_reserved; 

    /* REG_0xa2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xa2_reserved; 

    /* REG_0xa3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xa3_reserved; 

    /* REG_0xa4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xa4_reserved; 

    /* REG_0xa5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xa5_reserved; 

    /* REG_0xa6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xa6_reserved; 

    /* REG_0xa7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xa7_reserved; 

    /* REG_0xa8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xa8_reserved; 

    /* REG_0xa9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xa9_reserved; 

    /* REG_0xaa */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xaa_reserved; 

    /* REG_0xab */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xab_reserved; 

    /* REG_0xac */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xac_reserved; 

    /* REG_0xad */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xad_reserved; 

    /* REG_0xae */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xae_reserved; 

    /* REG_0xaf */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xaf_reserved; 

    /* REG_0xb0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xb0_reserved; 

    /* REG_0xb1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xb1_reserved; 

    /* REG_0xb2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xb2_reserved; 

    /* REG_0xb3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xb3_reserved; 

    /* REG_0xb4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xb4_reserved; 

    /* REG_0xb5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xb5_reserved; 

    /* REG_0xb6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xb6_reserved; 

    /* REG_0xb7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xb7_reserved; 

    /* REG_0xb8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xb8_reserved; 

    /* REG_0xb9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xb9_reserved; 

    /* REG_0xba */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xba_reserved; 

    /* REG_0xbb */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xbb_reserved; 

    /* REG_0xbc */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xbc_reserved; 

    /* REG_0xbd */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xbd_reserved; 

    /* REG_0xbe */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xbe_reserved; 

    /* REG_0xbf */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xbf_reserved; 

    /* REG_0xc0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xc0_reserved; 

    /* REG_0xc1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xc1_reserved; 

    /* REG_0xc2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xc2_reserved; 

    /* REG_0xc3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xc3_reserved; 

    /* REG_0xc4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xc4_reserved; 

    /* REG_0xc5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xc5_reserved; 

    /* REG_0xc6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xc6_reserved; 

    /* REG_0xc7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xc7_reserved; 

    /* REG_0xc8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xc8_reserved; 

    /* REG_0xc9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xc9_reserved; 

    /* REG_0xca */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xca_reserved; 

    /* REG_0xcb */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xcb_reserved; 

    /* REG_0xcc */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xcc_reserved; 

    /* REG_0xcd */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xcd_reserved; 

    /* REG_0xce */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xce_reserved; 

    /* REG_0xcf */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xcf_reserved; 

    /* REG_0xd0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xd0_reserved; 

    /* REG_0xd1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xd1_reserved; 

    /* REG_0xd2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xd2_reserved; 

    /* REG_0xd3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xd3_reserved; 

    /* REG_0xd4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xd4_reserved; 

    /* REG_0xd5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xd5_reserved; 

    /* REG_0xd6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xd6_reserved; 

    /* REG_0xd7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xd7_reserved; 

    /* REG_0xd8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xd8_reserved; 

    /* REG_0xd9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xd9_reserved; 

    /* REG_0xda */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xda_reserved; 

    /* REG_0xdb */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xdb_reserved; 

    /* REG_0xdc */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xdc_reserved; 

    /* REG_0xdd */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xdd_reserved; 

    /* REG_0xde */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xde_reserved; 

    /* REG_0xdf */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xdf_reserved; 

    /* REG_0xe0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe0_reserved; 

    /* REG_0xe1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe1_reserved; 

    /* REG_0xe2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe2_reserved; 

    /* REG_0xe3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe3_reserved; 

    /* REG_0xe4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe4_reserved; 

    /* REG_0xe5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe5_reserved; 

    /* REG_0xe6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe6_reserved; 

    /* REG_0xe7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe7_reserved; 

    /* REG_0xe8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe8_reserved; 

    /* REG_0xe9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xe9_reserved; 

    /* REG_0xea */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xea_reserved; 

    /* REG_0xeb */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xeb_reserved; 

    /* REG_0xec */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xec_reserved; 

    /* REG_0xed */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xed_reserved; 

    /* REG_0xee */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xee_reserved; 

    /* REG_0xef */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xef_reserved; 

    /* REG_0xf0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf0_reserved; 

    /* REG_0xf1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf1_reserved; 

    /* REG_0xf2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf2_reserved; 

    /* REG_0xf3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf3_reserved; 

    /* REG_0xf4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf4_reserved; 

    /* REG_0xf5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf5_reserved; 

    /* REG_0xf6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf6_reserved; 

    /* REG_0xf7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf7_reserved; 

    /* REG_0xf8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf8_reserved; 

    /* REG_0xf9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xf9_reserved; 

    /* REG_0xfa */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xfa_reserved; 

    /* REG_0xfb */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xfb_reserved; 

    /* REG_0xfc */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xfc_reserved; 

    /* REG_0xfd */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xfd_reserved; 

    /* REG_0xfe */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xfe_reserved; 

    /* REG_0xff */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0xff_reserved; 

    /* REG_0x100 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  blue                           : 1;  //0x100[  7:  0],blue value foe index<y>for the fg.,0,RW
           volatile uint32_t  green                          : 1;  //0x100[15:  8],green value foe index<y>for the fg.,0,RW
           volatile uint32_t  red                            : 1;  //0x100[23:16],red value foe index<y>for the fg.,0,RW
           volatile uint32_t  alpha                          : 1;  //0x100[31:24],alpha value foe index<y>for the fg.,0,RW
        }; 
        uint32_t v; 
    }dma2d_fg_clut0; 

    /* REG_0x101 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x101_reserved; 

    /* REG_0x102 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x102_reserved; 

    /* REG_0x103 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x103_reserved; 

    /* REG_0x104 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x104_reserved; 

    /* REG_0x105 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x105_reserved; 

    /* REG_0x106 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x106_reserved; 

    /* REG_0x107 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x107_reserved; 

    /* REG_0x108 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x108_reserved; 

    /* REG_0x109 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x109_reserved; 

    /* REG_0x10a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x10a_reserved; 

    /* REG_0x10b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x10b_reserved; 

    /* REG_0x10c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x10c_reserved; 

    /* REG_0x10d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x10d_reserved; 

    /* REG_0x10e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x10e_reserved; 

    /* REG_0x10f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x10f_reserved; 

    /* REG_0x110 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x110_reserved; 

    /* REG_0x111 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x111_reserved; 

    /* REG_0x112 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x112_reserved; 

    /* REG_0x113 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x113_reserved; 

    /* REG_0x114 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x114_reserved; 

    /* REG_0x115 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x115_reserved; 

    /* REG_0x116 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x116_reserved; 

    /* REG_0x117 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x117_reserved; 

    /* REG_0x118 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x118_reserved; 

    /* REG_0x119 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x119_reserved; 

    /* REG_0x11a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x11a_reserved; 

    /* REG_0x11b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x11b_reserved; 

    /* REG_0x11c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x11c_reserved; 

    /* REG_0x11d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x11d_reserved; 

    /* REG_0x11e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x11e_reserved; 

    /* REG_0x11f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x11f_reserved; 

    /* REG_0x120 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x120_reserved; 

    /* REG_0x121 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x121_reserved; 

    /* REG_0x122 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x122_reserved; 

    /* REG_0x123 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x123_reserved; 

    /* REG_0x124 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x124_reserved; 

    /* REG_0x125 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x125_reserved; 

    /* REG_0x126 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x126_reserved; 

    /* REG_0x127 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x127_reserved; 

    /* REG_0x128 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x128_reserved; 

    /* REG_0x129 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x129_reserved; 

    /* REG_0x12a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x12a_reserved; 

    /* REG_0x12b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x12b_reserved; 

    /* REG_0x12c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x12c_reserved; 

    /* REG_0x12d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x12d_reserved; 

    /* REG_0x12e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x12e_reserved; 

    /* REG_0x12f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x12f_reserved; 

    /* REG_0x130 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x130_reserved; 

    /* REG_0x131 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x131_reserved; 

    /* REG_0x132 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x132_reserved; 

    /* REG_0x133 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x133_reserved; 

    /* REG_0x134 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x134_reserved; 

    /* REG_0x135 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x135_reserved; 

    /* REG_0x136 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x136_reserved; 

    /* REG_0x137 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x137_reserved; 

    /* REG_0x138 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x138_reserved; 

    /* REG_0x139 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x139_reserved; 

    /* REG_0x13a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x13a_reserved; 

    /* REG_0x13b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x13b_reserved; 

    /* REG_0x13c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x13c_reserved; 

    /* REG_0x13d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x13d_reserved; 

    /* REG_0x13e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x13e_reserved; 

    /* REG_0x13f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x13f_reserved; 

    /* REG_0x140 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x140_reserved; 

    /* REG_0x141 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x141_reserved; 

    /* REG_0x142 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x142_reserved; 

    /* REG_0x143 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x143_reserved; 

    /* REG_0x144 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x144_reserved; 

    /* REG_0x145 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x145_reserved; 

    /* REG_0x146 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x146_reserved; 

    /* REG_0x147 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x147_reserved; 

    /* REG_0x148 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x148_reserved; 

    /* REG_0x149 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x149_reserved; 

    /* REG_0x14a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x14a_reserved; 

    /* REG_0x14b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x14b_reserved; 

    /* REG_0x14c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x14c_reserved; 

    /* REG_0x14d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x14d_reserved; 

    /* REG_0x14e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x14e_reserved; 

    /* REG_0x14f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x14f_reserved; 

    /* REG_0x150 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x150_reserved; 

    /* REG_0x151 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x151_reserved; 

    /* REG_0x152 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x152_reserved; 

    /* REG_0x153 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x153_reserved; 

    /* REG_0x154 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x154_reserved; 

    /* REG_0x155 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x155_reserved; 

    /* REG_0x156 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x156_reserved; 

    /* REG_0x157 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x157_reserved; 

    /* REG_0x158 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x158_reserved; 

    /* REG_0x159 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x159_reserved; 

    /* REG_0x15a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x15a_reserved; 

    /* REG_0x15b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x15b_reserved; 

    /* REG_0x15c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x15c_reserved; 

    /* REG_0x15d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x15d_reserved; 

    /* REG_0x15e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x15e_reserved; 

    /* REG_0x15f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x15f_reserved; 

    /* REG_0x160 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x160_reserved; 

    /* REG_0x161 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x161_reserved; 

    /* REG_0x162 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x162_reserved; 

    /* REG_0x163 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x163_reserved; 

    /* REG_0x164 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x164_reserved; 

    /* REG_0x165 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x165_reserved; 

    /* REG_0x166 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x166_reserved; 

    /* REG_0x167 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x167_reserved; 

    /* REG_0x168 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x168_reserved; 

    /* REG_0x169 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x169_reserved; 

    /* REG_0x16a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x16a_reserved; 

    /* REG_0x16b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x16b_reserved; 

    /* REG_0x16c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x16c_reserved; 

    /* REG_0x16d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x16d_reserved; 

    /* REG_0x16e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x16e_reserved; 

    /* REG_0x16f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x16f_reserved; 

    /* REG_0x170 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x170_reserved; 

    /* REG_0x171 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x171_reserved; 

    /* REG_0x172 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x172_reserved; 

    /* REG_0x173 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x173_reserved; 

    /* REG_0x174 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x174_reserved; 

    /* REG_0x175 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x175_reserved; 

    /* REG_0x176 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x176_reserved; 

    /* REG_0x177 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x177_reserved; 

    /* REG_0x178 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x178_reserved; 

    /* REG_0x179 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x179_reserved; 

    /* REG_0x17a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x17a_reserved; 

    /* REG_0x17b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x17b_reserved; 

    /* REG_0x17c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x17c_reserved; 

    /* REG_0x17d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x17d_reserved; 

    /* REG_0x17e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x17e_reserved; 

    /* REG_0x17f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x17f_reserved; 

    /* REG_0x180 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x180_reserved; 

    /* REG_0x181 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x181_reserved; 

    /* REG_0x182 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x182_reserved; 

    /* REG_0x183 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x183_reserved; 

    /* REG_0x184 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x184_reserved; 

    /* REG_0x185 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x185_reserved; 

    /* REG_0x186 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x186_reserved; 

    /* REG_0x187 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x187_reserved; 

    /* REG_0x188 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x188_reserved; 

    /* REG_0x189 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x189_reserved; 

    /* REG_0x18a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x18a_reserved; 

    /* REG_0x18b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x18b_reserved; 

    /* REG_0x18c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x18c_reserved; 

    /* REG_0x18d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x18d_reserved; 

    /* REG_0x18e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x18e_reserved; 

    /* REG_0x18f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x18f_reserved; 

    /* REG_0x190 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x190_reserved; 

    /* REG_0x191 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x191_reserved; 

    /* REG_0x192 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x192_reserved; 

    /* REG_0x193 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x193_reserved; 

    /* REG_0x194 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x194_reserved; 

    /* REG_0x195 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x195_reserved; 

    /* REG_0x196 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x196_reserved; 

    /* REG_0x197 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x197_reserved; 

    /* REG_0x198 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x198_reserved; 

    /* REG_0x199 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x199_reserved; 

    /* REG_0x19a */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x19a_reserved; 

    /* REG_0x19b */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x19b_reserved; 

    /* REG_0x19c */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x19c_reserved; 

    /* REG_0x19d */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x19d_reserved; 

    /* REG_0x19e */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x19e_reserved; 

    /* REG_0x19f */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x19f_reserved; 

    /* REG_0x1a0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a0_reserved; 

    /* REG_0x1a1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a1_reserved; 

    /* REG_0x1a2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a2_reserved; 

    /* REG_0x1a3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a3_reserved; 

    /* REG_0x1a4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a4_reserved; 

    /* REG_0x1a5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a5_reserved; 

    /* REG_0x1a6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a6_reserved; 

    /* REG_0x1a7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a7_reserved; 

    /* REG_0x1a8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a8_reserved; 

    /* REG_0x1a9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1a9_reserved; 

    /* REG_0x1aa */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1aa_reserved; 

    /* REG_0x1ab */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ab_reserved; 

    /* REG_0x1ac */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ac_reserved; 

    /* REG_0x1ad */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ad_reserved; 

    /* REG_0x1ae */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ae_reserved; 

    /* REG_0x1af */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1af_reserved; 

    /* REG_0x1b0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b0_reserved; 

    /* REG_0x1b1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b1_reserved; 

    /* REG_0x1b2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b2_reserved; 

    /* REG_0x1b3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b3_reserved; 

    /* REG_0x1b4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b4_reserved; 

    /* REG_0x1b5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b5_reserved; 

    /* REG_0x1b6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b6_reserved; 

    /* REG_0x1b7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b7_reserved; 

    /* REG_0x1b8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b8_reserved; 

    /* REG_0x1b9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1b9_reserved; 

    /* REG_0x1ba */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ba_reserved; 

    /* REG_0x1bb */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1bb_reserved; 

    /* REG_0x1bc */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1bc_reserved; 

    /* REG_0x1bd */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1bd_reserved; 

    /* REG_0x1be */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1be_reserved; 

    /* REG_0x1bf */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1bf_reserved; 

    /* REG_0x1c0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c0_reserved; 

    /* REG_0x1c1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c1_reserved; 

    /* REG_0x1c2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c2_reserved; 

    /* REG_0x1c3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c3_reserved; 

    /* REG_0x1c4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c4_reserved; 

    /* REG_0x1c5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c5_reserved; 

    /* REG_0x1c6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c6_reserved; 

    /* REG_0x1c7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c7_reserved; 

    /* REG_0x1c8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c8_reserved; 

    /* REG_0x1c9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1c9_reserved; 

    /* REG_0x1ca */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ca_reserved; 

    /* REG_0x1cb */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1cb_reserved; 

    /* REG_0x1cc */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1cc_reserved; 

    /* REG_0x1cd */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1cd_reserved; 

    /* REG_0x1ce */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ce_reserved; 

    /* REG_0x1cf */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1cf_reserved; 

    /* REG_0x1d0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d0_reserved; 

    /* REG_0x1d1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d1_reserved; 

    /* REG_0x1d2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d2_reserved; 

    /* REG_0x1d3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d3_reserved; 

    /* REG_0x1d4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d4_reserved; 

    /* REG_0x1d5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d5_reserved; 

    /* REG_0x1d6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d6_reserved; 

    /* REG_0x1d7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d7_reserved; 

    /* REG_0x1d8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d8_reserved; 

    /* REG_0x1d9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1d9_reserved; 

    /* REG_0x1da */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1da_reserved; 

    /* REG_0x1db */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1db_reserved; 

    /* REG_0x1dc */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1dc_reserved; 

    /* REG_0x1dd */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1dd_reserved; 

    /* REG_0x1de */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1de_reserved; 

    /* REG_0x1df */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1df_reserved; 

    /* REG_0x1e0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e0_reserved; 

    /* REG_0x1e1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e1_reserved; 

    /* REG_0x1e2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e2_reserved; 

    /* REG_0x1e3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e3_reserved; 

    /* REG_0x1e4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e4_reserved; 

    /* REG_0x1e5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e5_reserved; 

    /* REG_0x1e6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e6_reserved; 

    /* REG_0x1e7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e7_reserved; 

    /* REG_0x1e8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e8_reserved; 

    /* REG_0x1e9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1e9_reserved; 

    /* REG_0x1ea */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ea_reserved; 

    /* REG_0x1eb */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1eb_reserved; 

    /* REG_0x1ec */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ec_reserved; 

    /* REG_0x1ed */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ed_reserved; 

    /* REG_0x1ee */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ee_reserved; 

    /* REG_0x1ef */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ef_reserved; 

    /* REG_0x1f0 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f0_reserved; 

    /* REG_0x1f1 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f1_reserved; 

    /* REG_0x1f2 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f2_reserved; 

    /* REG_0x1f3 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f3_reserved; 

    /* REG_0x1f4 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f4_reserved; 

    /* REG_0x1f5 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f5_reserved; 

    /* REG_0x1f6 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f6_reserved; 

    /* REG_0x1f7 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f7_reserved; 

    /* REG_0x1f8 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f8_reserved; 

    /* REG_0x1f9 */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1f9_reserved; 

    /* REG_0x1fa */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1fa_reserved; 

    /* REG_0x1fb */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1fb_reserved; 

    /* REG_0x1fc */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1fc_reserved; 

    /* REG_0x1fd */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1fd_reserved; 

    /* REG_0x1fe */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1fe_reserved; 

    /* REG_0x1ff */
    union 
    { 
        struct 
        { 
           volatile uint32_t reserved:32;  
        }; 
        uint32_t v; 
    }reg_0x1ff_reserved; 

    /* REG_0x200 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  blue                           : 1;  //0x200[  7:  0],blue value foe index<y>for the bg.,0,RW
           volatile uint32_t  green                          : 1;  //0x200[15:  8],green value foe index<y>for the bg.,0,RW
           volatile uint32_t  red                            : 1;  //0x200[23:16],red value foe index<y>for the bg.,0,RW
           volatile uint32_t  alpha                          : 1;  //0x200[31:24],alpha value foe index<y>for the bg.,0,RW
        }; 
        uint32_t v; 
    }dma2d_bg_clut0; 
} dma2d_hw_t;

#ifdef __cplusplus 
}
#endif


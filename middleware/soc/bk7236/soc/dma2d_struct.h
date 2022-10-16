// Copyright 2020-2022 Beken
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
	volatile uint32_t device_id;    //Device ID:ASCII Code "DMAD",0x00415544,R

    /* REG_0x01 */
	volatile uint32_t version_id;    //Version ID :V1.1,0x00010001,R

    /* REG_0x02 */
    union
    {
        struct
        {
           volatile uint32_t  soft_reset                     :1;  //0x2[       0],software write 1 to reset DISP modulation,need software clear up.,0,RW
           volatile uint32_t  clk_gate                       :1;  //0x2[       1],0x0:enable ahb clock gate.,0,RW
           volatile uint32_t  reserved                       :30;

        };
        uint32_t v;
    }module_control;

    /* REG_0x03 */
	volatile uint32_t global_status;    //global status,0,R

    /* REG_0x04 */
    union
    {
        struct
        {
           volatile uint32_t  tran_start                     : 1;  //0x0[       0],dma2d transfer start.,0,RW
           volatile uint32_t  tran_suspend                   : 1;  //0x0[       1],dma2d transfer start.,0,RW
           volatile uint32_t  tran_abort                     : 1;  //0x0[       2],dma2d transfer start.,0,RW
           volatile uint32_t  reserved0                      : 3;  //0x0[  5:  3],NC,0,RW
           volatile uint32_t  line_offset_mode               : 1;  //0x0[       6],line's offset mode sel: 0:in pixel express; 1: in bytes express.,0,RW
           volatile uint32_t  reserved1                      : 1;  //0x0[       7],NC,0,RW
           volatile uint32_t  error_int_en                   : 1;  //0x0[       8],trabsfer error int ena.,0,RW
           volatile uint32_t  complete_int_en                : 1;  //0x0[       9],transfer complete int ena.,0,RW
           volatile uint32_t  waterm_int_en                  : 1;  //0x0[     10],transfer watermark int ena.,0,RW
           volatile uint32_t  clut_error_int_en              : 1;  //0x0[     11],clut transfer error int ena.,0,RW
           volatile uint32_t  clut_cmplt_int_en              : 1;  //0x0[     12],clut transfer complete int ena.,0,RW
           volatile uint32_t  config_error_int_en            : 1;  //0x0[     13],config error int ena,,0,RW
           volatile uint32_t  reserved2                      : 2;  //0x0[15:14],NC,0,R
           volatile uint32_t  mode                           : 3;  //0x0[18:16],DMA2D mode sel: 000:m2m; 001:m2m pixel convert with fg; 010:m2m blend; 011:r2m.only with output; 100: m2m blend fix fg; 101:m2m blend fix bg;,0,RW
           volatile uint32_t  master_tran_length             :2;  //0x0[20:19],RW max transfer length:00:256bytes; 01:192 bytes;  10:128bytes;  11:64bytes;
           volatile uint32_t  out_byte_revese                :1;  //0x0[21],RW output rgb888 formart, reverse data byte by byte.
           volatile uint32_t  reserved                       :13;  //0x0[31:21],NC,0,R
        };
        uint32_t v;
    }dma2d_control_reg;

    /* REG_0x05 */
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

    /* REG_0x06 */
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

    /* REG_0x07 */
    union
    {
        struct
        {
           volatile uint32_t  fg_address                     :32;  //0x3[31:  0],foreground mem address, written when transfer disable.,0,RW
        };
        uint32_t v;
    }dma2d_fg_address;

    /* REG_0x08 */
    union
    {
        struct
        {
           volatile uint32_t  fg_line_offset                 :16;  //0x4[15:  0],line offset for fg, offset In pixel or bytes.,0,RW
           volatile uint32_t  reserved                       :16;  //0x4[31:16],NC,0,RW
        };
        uint32_t v;
    }dma2d_fg_offset;

    /* REG_0x09 */
    union
    {
        struct
        {
           volatile uint32_t  bg_address                     :32;  //0x5[31:  0],background mem address, written when transfer disable.,0,RW
        };
        uint32_t v;
    }dma2d_bg_address;

    /* REG_0x0a */
    union
    {
        struct
        {
           volatile uint32_t  bg_line_offset                 :16;  //0x6[15:  0],line offset for bg, offset In pixel or bytes.,0,RW
           volatile uint32_t  reserved                       :16;  //0x6[31:16],NC,0,RW
        };
        uint32_t v;
    }dma2d_bg_offset;

    /* REG_0x0b */
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

    /* REG_0x0c */
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

    /* REG_0x0d */
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

    /* REG_0x0e */
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

    /* REG_0x0f */
    union
    {
        struct
        {
           volatile uint32_t  fg_clut_address                :32;  //0xb[31: 0],clut data dedicated to the fg image.,0,RW
        };
        uint32_t v;
    }fg_clut_mem_address;

    /* REG_0x10 */
    union
    {
        struct
        {
           volatile uint32_t  bg_clut_address                :32;  //0xc[31:  0],clut data dedicated to the bg image.,0,RW
        };
        uint32_t v;
    }bg_clut_mem_address;

    /* REG_0x11 */
    union
    {
        struct
        {
           volatile uint32_t  out_color_mode                 : 3;  //0xd[  2:  0],format of output image.0:argb888; 1:rgb888; 010:rgb565; 011:argb1555; 100:argb444,0,RW
           volatile uint32_t  reserved0                       : 17;  //0xd[  19:  3],NC,0,RW
           volatile uint32_t  out_alpha_invert              :1;  //0xd[     20],invert alpha value.,0,RW
           volatile uint32_t  out_rb_swap                    : 1;  //0xd[     21],ed blue swap to support rgb or argb. 0: regular mode.1:swap_mode.,0,RW
           volatile uint32_t  reserved                       :10;  //0xd[31:22],NC,0,RW
        };
        uint32_t v;
    }out_pfc_contrl;

    /* REG_0x12 */
    union
    {
        struct
        {
           volatile uint32_t  output_clor_reg                :32;  //0xe[31:  0],output reg, in different color mode. Output is different.; Argb888 or rgb888: alpha[31:24];red[23:16]; green[15:8];blue[7:0].; Rgb565:red[15:11]; green[12:5]; blue[4:0];; argb1555:a[15];red[14:10]; green[4:0]; blue[4:0];; argb4444:alpha[15:12];red[11:8];green[7:4];blue[3;0].,0,RW
        };
        uint32_t v;
    }out_color_reg;

    /* REG_0x13 */
    union
    {
        struct
        {
           volatile uint32_t  out_mem_address                :32;  //0xf[31:  0],address of data output fifo.,0,RW
        };
        uint32_t v;
    }dma2d_out_mem_address;

    /* REG_0x14 */
    union
    {
        struct
        {
           volatile uint32_t  out_line_offset                :16;  //0x10[15:  0],output line offset. Offfset with bytes or pixel.in pixel[15:14] ignored.,0,RW
           volatile uint32_t  reserved                       :16;  //0x10[31:16],NC,0,R
        };
        uint32_t v;
    }output_offset;

    /* REG_0x15 */
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

    /* REG_0x16 */
    union
    {
        struct
        {
           volatile uint32_t  line_watermark                 :16;  //0x12[15:0],config the line watermark int generation, transfer reach the watermark, int hold on.,0,RW
           volatile uint32_t  reserved                       :16;  //0x12[31:16],NC,0,RW
        };
        uint32_t v;
    }dma2d_line_watermark;

    union
    {
        struct
        {
           volatile uint32_t  blue                           : 8;  //0x200[7:0],blue value foe index<y>for the bg.,0,RW
           volatile uint32_t  green                          : 8;  //0x200[15:8],green value foe index<y>for the bg.,0,RW
           volatile uint32_t  red                            : 8;  //0x200[23:16],red value foe index<y>for the bg.,0,RW
           volatile uint32_t  alpha                          : 8;  //0x200[31:24],alpha value foe index<y>for the bg.,0,RW
        };
        uint32_t v;
    }dma2d_fg_clut[256];


    union
    {
        struct
        {
           volatile uint32_t  blue                           : 8;  //0x200[7:0],blue value foe index<y>for the bg.,0,RW
           volatile uint32_t  green                          : 8;  //0x200[15:8],green value foe index<y>for the bg.,0,RW
           volatile uint32_t  red                            : 8;  //0x200[23:16],red value foe index<y>for the bg.,0,RW
           volatile uint32_t  alpha                          : 8;  //0x200[31:24],alpha value foe index<y>for the bg.,0,RW
        };
        uint32_t v;
    }dma2d_bg_clut[256];

} dma2d_hw_t;

#ifdef __cplusplus
}
#endif


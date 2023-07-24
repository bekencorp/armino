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
		union {
			struct {
				uint32_t rgb_int_en:  2;    /**<bit[0-1] rgb display interrupt enable */
				uint32_t i8080_int_en:   2; /**< i8080 display interrupt enable */
				uint32_t rgb_sof: 1; /**<  */
				uint32_t rgb_eof:    1; /**<  */
				uint32_t i8080_sof: 1; /**< 8080 display output start of frame */
				uint32_t i8080_eof: 1; /**< 8080 display output end of frame */
				uint32_t wr_thrd:       10; /**rgb fifo wr thrd */
				uint32_t rd_thrd:  10; /**< rgb fifo rd thrd */
				uint32_t disconti_mode:  1; /**<  open in 20mhz, defult 1  */
				uint32_t reserved:      3;
			};
			uint32_t v;
		} display_int;

		/* REG_0x01 */
		union {
			struct {
				uint32_t x_pixel:            11; /**< bit[0:10] */
				uint32_t dclk_rev:          1;
				uint32_t y_pixel:   11; /**<  bit[12:22]  */
				uint32_t str_fifo_clr:    1; /**<  bit[23]  */
				uint32_t rgb_disp_on :   1;  /**<  bit[24] rgb modle enable */
				uint32_t rgb_on:       1; /**<  bit[25]  */
				uint32_t lcd_display_on:  1;	/**<   bit[26] Rgb output signal. */
				uint32_t rgb_clk_div:	 5; /**<  bit[31:27] rgb_clk_div + 1 */
			};
			uint32_t v;
		} status;

		/* REG_0x02 */
		union {
			struct {
				uint32_t rgb_dat:       16;/**< bit[0:15] */
				uint32_t reserved:      16;
			};
			uint32_t v;
		} rgb_fifo;

		/* REG_0x03 */
		union {
			struct {
				uint32_t hsync_back_porch:  8; /**< bit[0:7] */
				uint32_t hsync_front_porch: 7;
				uint32_t vsync_back_porch:   5;
				uint32_t vsync_front_porch: 7;
				uint32_t reserved: 1;
				uint32_t yuv_sel:          3;
			};
			uint32_t v;
		} hsync_vsync;

		/* REG_0x04 */
		union {
			struct {
				uint32_t i8080_disp_en: 1; /**< bit[0] */
				uint32_t i8080_dat_on:  1; /**< bit[1] */
				uint32_t i8080_fifo_mode:   1; /**< bit[2] 0:when i8080_disp_en=0, invalid write fifo, 1:valid whenever time*/
				uint32_t i8080_fifo_clr:      1; /**< bit[3] */
				uint32_t i8080_cmdfifo_clr:   1; /**< bit[4] */
				uint32_t reset_sleep_in:          1; /**< bit[5] */
				uint32_t no_use1:           2; /**< bit[6:7] */
				uint32_t tik_cnt:                  2; /**<bit[8:9]0：8clk；1：6clk；2：4clk；3：2clk */
				uint32_t no_use2:                 2;
				uint32_t i8080_1ms_count:           9; /**< bit[12:20] */
				uint32_t reservsed:           11;
			};
			uint32_t v;
		} i8080_config;

		/* REG_0x05 */
		union {
			struct {
				uint32_t i8080_cmd_fifo: 16; /**< bit[15: 0] i8080 command fifo */
				uint32_t reserved:           16;
			};
			uint32_t v;
		} i8080_cmd_fifo;

		/* REG_0x06 */
		union {
			struct {
				uint32_t i8080_dat_fifo:  16; /**bit[15: 0]   */
				uint32_t reserved:           16; /**< i8080 data fifo */
			};
			uint32_t v;
		} i8080_dat_fifo;

		/* REG_0x07 */
		union {
			struct {
				uint32_t dat_wr_thrd:             8; /**< bit[0:7 i8080 rgb fifo wr thrd */
				uint32_t cmd_wr_thrd:         8; /**< i8080 cmd fifo wr thrd */
				uint32_t dat_rd_thrd:          8; /**<i8080 dat fifo rd thrd */
				uint32_t cmd_rd_thrd:          8; /**< i8080 cmd fifo rd thrd */
			};
			uint32_t v;
		} i8080thrd;
} lcd_disp_hw_t;


#ifdef __cplusplus
}
#endif


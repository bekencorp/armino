// Copyright 2021-2022 Beken
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
	uint32_t devide_id;

	/* REG_0x01 */
	uint32_t version_id;

	/* REG_0x02 */
	union {
		struct {
			uint32_t soft_reset:    1;
			uint32_t clk_gate:      1;
			uint32_t reserved:      30;
		};
		uint32_t v;
	} module_control;

	/* REG_0x03 */
	uint32_t global_status;

	/* REG_0x04 */
	union {
		struct {
			uint32_t rgb_int_en:     2;    /**<bit[0-1] rgb display interrupt enable */
			uint32_t i8080_int_en:   2; /**< i8080 display interrupt enable */
			uint32_t rgb_sof:        1; /**<  */
			uint32_t rgb_eof:        1; /**<  */
			uint32_t i8080_sof:      1; /**< 8080 display output start of frame */
			uint32_t i8080_eof:      1; /**< 8080 display output end of frame */
			uint32_t int_de:         1;
			uint32_t reserved:       18;
			uint32_t de_int_en:      1;
			uint32_t disconti_mode:  1; /**<  open in 20mhz, defult 1  */
			uint32_t reserved:       3;
		};
		uint32_t v;
	} display_int;

	/* REG_0x05 */
	union {
		struct {
			uint32_t x_pixel:         11;
			uint32_t dclk_rev:        1;
			uint32_t y_pixel:         11;
			uint32_t reserved:        1;
			uint32_t rgb_disp_on :    1;
			uint32_t rgb_on:          1;
			uint32_t lcd_display_on:  1;
			uint32_t reserved:	      5;
		};
		uint32_t v;
	} rgb_cfg;

	/* REG_0x06 */
	uint32_t rgb_fifo;

	/* REG_0x07 */
	union {
		struct {
			uint32_t hsync_back_porch:   8; /**< bit[0:7] */
			uint32_t hsync_front_porch:  7;
			uint32_t vsync_back_porch:   5;
			uint32_t vsync_front_porch:  7;
			uint32_t reserved:           1;
			uint32_t yuv_sel:            3;
			uint32_t reserved:           1;
		};
		uint32_t v;
	} sync_cfg;

	/* REG_0x08 */
	union {
		struct {
			uint32_t i8080_disp_en:      1;
			uint32_t i8080_dat_on:       1;
			uint32_t i8080_fifo_mode:    1;
			uint32_t i8080_fifo_clr:     1;
			uint32_t i8080_cmd_fifo_clr:  1;
			uint32_t reset_sleep_in:     1;
			uint32_t i8080_ena:          1;
			uint32_t reserved:           1;
			uint32_t tik_cnt:            2;
			uint32_t reserved:           2;
			uint32_t i8080_1ms_count:    9;
			uint32_t reservsed:          11;
		};
		uint32_t v;
	} i8080_config;

	/* REG_0x09 */
	union {
		struct {
			uint32_t i8080_cmd_fifo:     16; /**< bit[15: 0] i8080 command fifo */
			uint32_t reserved:           16;
		};
		uint32_t v;
	} i8080_cmd_fifo;

	/* REG_0x0a */
	uint32_t i8080_dat_fifo;

	/* REG_0x0b */
	union {
		struct {
			uint32_t reserved:             8; /**< bit[0:7 i8080 rgb fifo wr thrd */
			uint32_t cmd_wr_thrd:          8; /**< i8080 cmd fifo wr thrd */
			uint32_t reserved:             8; /**<i8080 dat fifo rd thrd */
			uint32_t cmd_rd_thrd:          8; /**< i8080 cmd fifo rd thrd */
		};
		uint32_t v;
	} i8080thrd;

	/* REG_0x0c */
	union {
		struct {
			uint32_t rgb_ver_cnt:          11;
			uint32_t i8080_cmd_cfg_done:   1;
			uint32_t i8080_ver_cnt:        11;
			uint32_t disp_fifo_empty:      1;
			uint32_t disp_fifo_near_full:  1;
			uint32_t reserved:             7;
		};
		uint32_t v;
	} disp_status;

	/* REG_0x0d */
	union {
		struct {
			uint32_t hsync_back_low:       6;
			uint32_t reserved:             2;
			uint32_t vsync_back_low:       6;
			uint32_t reserved:             2;
			uint32_t pfc_pixel_reve:       1;
			uint32_t reserved:             15;
		};
		uint32_t v;
	} rgb_sync_low;

	/* REG_0x0e */
	union {
		struct {
			uint32_t partial_offset_clum_l:  11;
			uint32_t reserved:               5;
			uint32_t partial_offset_clum_r:  11;
			uint32_t reserved:               1;
			uint32_t partial_area_ena:       1;
			uint32_t reserved:               3;
		};
		uint32_t v;
	} rgb_clum_offset;

	/* REG_0x0f */
	union {
		struct {
			uint32_t partial_offset_clum_l:  11;
			uint32_t reserved:               5;
			uint32_t partial_offset_clum_r:  11;
			uint32_t reserved:               5;
		};
		uint32_t v;
	} rgb_line_offset;

	/* REG_0x10 */
	union {
		struct {
			uint32_t reserved:               8;
			uint32_t dat_wr_thrd:            9;
			uint32_t reserved:               3;
			uint32_t dat_rd_thrd:            9;
			uint32_t reserved:               3;
		};
		uint32_t v;
	} dat_fifo_thrd;

	/* REG_0x11 */
	uint32_t mater_rd_base_addr;

	uint32_t reserved;

	/* REG_0x13 */
	union {
		struct {
			uint32_t i8080_cmd_para_count:   22;
			uint32_t reserved:               10;
		};
		uint32_t v;
	} cmd_count;

} lcd_disp_hw_t;


#ifdef __cplusplus
}
#endif


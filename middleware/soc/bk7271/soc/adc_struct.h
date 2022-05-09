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
			uint32_t adc_mode:	2; /**< bit[0:1] adc mode:0:power down; 1:single step; 2:software control 3: continus modes*/
			uint32_t adc_en:	1; /**< bit[2] adc enable/disable,  */
			uint32_t adc_channel:   4; /**< bit[3-6] adc channel; 0-5 : digital channel, others :  analog channel*/
			uint32_t adc_setting:	1; /**< bit[7] waiting for sample cycle: 0:4 cycles 1:8 cycyles*/
			uint32_t adc_int_clear:	1; /**< bit[8] clear interrupt flag bit*/
			uint32_t adc_div:	6; /**< bit[9:14] sample clock division, adc_clk = clk/[2*(adc_div +1)]*/
			uint32_t adc_32m_mode:	1; /**< bit[15] adc sample clk set 32M, adc_div is ineffective*/
			uint32_t adc_samp_rate:	6; /**< bit[16:21] adc sample rate only effective in continuous mode, every period
							period=(16+adc_samp_rate)*adc_clk */
			uint32_t adc_filter:	7; /**< bit[22:28] 1.continuous mode:adc cumulative num of adc data: if adc_filter>64, the result of accumulative /2 
							will be output, others, the result output directly.2.output sample rate = period/(adc_filter+1)*/
			uint32_t adc_busy:	1; /**< bit[29] adc busy flag */
			uint32_t fifo_empty:	1; /**< bit[30] fifo empty flag: 1:fifo empty; 0:not empty */
			uint32_t fifo_full:	1; /**< bit[31] fifo full flag: 1:fifo full; 0:not full*/
		};
		uint32_t v;
	} ctrl;

	/* REG_0x01 */
	uint32_t adc_raw_data;			/**original adc output data*/

	/* REG_0x02 */
	union {
		struct {
			uint32_t fifo_level:	5; /**< bit[0:4] only effective in continuous mode, when fifo data num > fifo_level, generate int */
			uint32_t steady_ctrl:	3; /**< bit[5:7] when adc power on wait (steady_ctrl +1)*8 adc clk to send valid*/

			uint32_t reserved:      24; /**< bit[8:31] */
		};

		uint32_t v;
	} steady_ctrl;

	/* REG_0x03 */
	union {
		struct {
			uint32_t sat_ctrl:	2; /**< bit[0:1] adc saturation processing:
						10 bit:
							00: sel [15:0]
							01: sel [16:0]
							10/11: ineffective
						12bit:
							00: sel [18:3] + round[2:0]
							01: sel [17:2]+ round[1:0]
							10: sel [16:1]+ round[0]
							11: sel [15:0]*/
			uint32_t sat_enable:	1; /**< bit[2] enable saturation*/
			uint32_t over_flow:	1; /**< bit[3] adc over flag*/

			uint32_t reserved:      28; /**< bit[4:31] */
		};

		uint32_t v;
	} sat_ctrl;

	/* REG_0x04 */
	uint32_t adc_data;			/**adc output data after satutate processs*/

}adc_hw_t;

#ifdef __cplusplus
}
#endif


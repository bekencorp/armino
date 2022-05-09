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

//TODO add full comments according to reg spec
typedef volatile struct {
	struct {
		union {
			struct {
				/* PWM mode
				 * 000: idle
				 * 001: PWM mode
				 * 010: Timer mode
				 * 011: Counter mode
				 * 100: Capture mode
				 * 101: Counter Neg
				 */
				uint32_t chan0_en:         1; /**< bit[0] pwm0_mode, R/W */
				uint32_t chan0_int_en:     1; /**< bit[1] pwm0_int_enable, R/W */
				uint32_t chan0_mode:       3; /**< bit[2:4] */

				uint32_t chan1_en:         1; /**< bit[5] pwm0_mode, R/W */
				uint32_t chan1_int_en:     1; /**< bit[6] pwm0_int_enable, R/W */
				uint32_t chan1_mode:       3; /**< bit[7:9] */

				uint32_t chan2_en:         1; /**< bit[10] pwm0_mode, R/W */
				uint32_t chan2_int_en:     1; /**< bit[11] pwm0_int_enable, R/W */
				uint32_t chan2_mode:       3; /**< bit[12:14] */

				uint32_t rfu:              1; /**< bit[15] */
				uint32_t pre_div:          4; /**< bit[16:19] */
				uint32_t reserved:         12;/**< bit[20:31] */
			};
			uint32_t v;
		} ctrl;

		union {
			struct {
				uint32_t chan0_int_st:     1; /**< bit[0] pwm0 interrupt status, write 1 to clear*/
				uint32_t chan1_int_st:     1; /**< bit[1] pwm0 interrupt status, write 1 to clear*/
				uint32_t chan2_int_st:     1; /**< bit[2] pwm0 interrupt status, write 1 to clear*/
				uint32_t reserved:         29;/**< bit[3:31] */
			};
			uint32_t v;
		} int_st;

		struct {
			uint32_t counter_end;
			uint32_t duty_cycle;
			uint32_t capture_out;
		} end_dc_cap_out[SOC_PWM_CHAN_NUM_PER_GROUP];

		union {
			struct {
				uint32_t chan_update: 1;  /**< bit[0], W, Update imm ;  it can be used when you want to update pwm_end_value  during  pwm working*/
				uint32_t reserved:    31; /**< bit[1:31] */
			};
			uint32_t v;
		} config_update[SOC_PWM_CHAN_NUM_PER_GROUP];

		union {
			struct {
				uint32_t read_en:  1;  /**< bit[0], W, Software write '1', and hardware clear it after finish read*/
				uint32_t chan_id:  3;  /**< bit[1:3] */
				uint32_t reserved: 28; /**< bit[4:31] */
			};
			uint32_t v;
		} counter_read_ctrl;

		uint32_t cnt_read;
	} group[SOC_PWM_GROUP_NUM];
} pwm_hw_t;

#ifdef __cplusplus
}
#endif

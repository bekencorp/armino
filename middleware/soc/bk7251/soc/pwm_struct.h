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
	union {
		struct {
			uint32_t chan0_en:     1; /**< bit[0] pwm0_en, R/W, channel enable/disable, 0-disable, 1-enable */
			uint32_t chan0_int_en: 1; /**< bit[1] pwm0_int_en, R/W, interrupt enable/disable,  */
			uint32_t chan0_mode:   2; /**< bit[2:3] pwm0_mode, R/W, 00-PWM mode, 01-Timer, 10 - Capture Posedge, 11 - Capture Negedge */

			uint32_t chan1_en:     1; /**< bit[4] pwm1_en */
			uint32_t chan1_int_en: 1; /**< bit[5] pwm1_int_en */
			uint32_t chan1_mode:   2; /**< bit[6:7] pwm1_mode */

			uint32_t chan2_en:     1; /**< bit[8] pwm2_en */
			uint32_t chan2_int_en: 1; /**< bit[9] pwm2_int_en */
			uint32_t chan2_mode:   2; /**< bit[10:11] pwm2_mode */

			uint32_t chan3_en:     1; /**< bit[12] pwm3_en */
			uint32_t chan3_int_en: 1; /**< bit[13] pwm3_int_en */
			uint32_t chan3_mode:   2; /**< bit[14:15] pwm3_mode */

			uint32_t chan4_en:     1; /**< bit[16] pwm4_en */
			uint32_t chan4_int_en: 1; /**< bit[17] pwm4_int_en */
			uint32_t chan4_mode:   2; /**< bit[18:19] pwm4_mode */

			uint32_t chan5_en:     1; /**< bit[20] pwm5_en */
			uint32_t chan5_int_en: 1; /**< bit[21] pwm5_int_en */
			uint32_t chan5_mode:   2; /**< bit[22:23] pwm5_mode */

			uint32_t reserved:     8; /**< bit[24:31] */
		};
		uint32_t v;
	} ctrl;

	union {
		struct {
			uint32_t chan0_int_status:   1; /**< bit[0] pwm0_int, R/W1C */
			uint32_t chan1_int_status:   1; /**< bit[1] pwm1_int, R/W1C */
			uint32_t chan2_int_status:   1; /**< bit[2] pwm2_int, R/W1C */
			uint32_t chan3_int_status:   1; /**< bit[3] pwm3_int, R/W1C */
			uint32_t chan4_int_status:   1; /**< bit[4] pwm4_int, R/W1C */
			uint32_t chan5_int_status:   1; /**< bit[5] pwm5_int, R/W1C */

			uint32_t reserved:           26; /**< bit[6:31] */
		};

		uint32_t v;
	} int_status;

	struct {
		uint32_t period_cycle; /**< pwmX_end, period cycle, unit is (1/clock_freq) */
		uint32_t duty_cycle; /**< pwmX_dc, duty cycle, unit is (1/clock_freq) */
		uint32_t capture_out; /**< pwmX_cap_out */
	} freq_duty_capture_out[SOC_PWM_CHAN_NUM_PER_UNIT];
} pwm_hw_t;

#ifdef __cplusplus
}
#endif

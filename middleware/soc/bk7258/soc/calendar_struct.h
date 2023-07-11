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
	/* REG_0x0 */
	uint32_t init_val;

	/* REG_0x1 */
	union {
		struct {
			uint32_t en:       1;  /**< bit[0] calendar enable */
			uint32_t reserved: 31; /**< bit[1:31] */
		};
		uint32_t v;
	} enable;

	/* REG_0x2 */
	uint32_t cnt_val_s; /*< calendar counter value. Unit is 1s */

	/* REG_0x3 */
	union {
		struct {
			uint32_t cnt_31_25_us_val: 15; /**< bit[0:14] calendar counter value. Unit is 31.25us */
			uint32_t reserved:         17; /**< bit[15:31] reserved */
		};
		uint32_t v;
	} cnt_val_us;
} calendar_hw_t;

#ifdef __cplusplus
}
#endif


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
			uint32_t en:        1; /**< bit[0] efuse read operate enable, sw set 1, hw clear it after read operate finished*/
			uint32_t reserved0: 7; /**< bit[1:7] */
			uint32_t addr:      5; /**< bit[8:12] efuse read operate address */
			uint32_t reserved1: 19; /**< bit[13:31] */
		};
		uint32_t v;
	} ctrl;

	union {
		struct {
			uint32_t rd_data:       8; /**< bit[0:7] efuse read operate data */
			uint32_t rd_data_valid: 1; /**< bit[8] efuse read data valid indication */
			uint32_t reserved:      23; /**< bit[9:31] */
		};
		uint32_t v;
	} optr;
} efuse_hw_t;

#ifdef __cplusplus
}
#endif



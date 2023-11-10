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
			uint32_t period:   16; /**< bit[0:15] */
			uint32_t key:      8;  /**< bit[16:23] */
			uint32_t reserved: 8;  /**< bit[24:31] */
		};
		uint32_t v;
	} ctrl;
} wdt_hw_t;

#ifdef __cplusplus
}
#endif


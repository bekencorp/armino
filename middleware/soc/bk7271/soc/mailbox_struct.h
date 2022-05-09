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

			uint32_t box0_param0;
			uint32_t box0_param1;
			uint32_t box0_param2;
			uint32_t box0_param3;
			uint32_t box1_param0;
			uint32_t box1_param1;
			uint32_t box1_param2;
			uint32_t box1_param3;
	union {
			struct {
				uint32_t box0_ready: 1;  /**< bit[0]*/
				uint32_t box1_ready: 1;  /**< bit[1]*/
				uint32_t reserved:    30; /**< bit[2:31] */
			};
			uint32_t value;
		} box_ready;

			UINT32 invalid0;		//reg9
			UINT32 invalid1;		//reg10
			UINT32 invalid2;		//reg11

	union {
			struct {
				uint32_t box0_clear: 1;  /**< bit[0]*/
				uint32_t box1_clear: 1;  /**< bit[1]*/
				uint32_t reserved:    30; /**< bit[2:31] */
			};
			uint32_t value;
		} box_clear;

} s_mailbox_t;

#ifdef __cplusplus
}
#endif

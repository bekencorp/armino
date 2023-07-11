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
	uint32_t smb_devid; /**< reg0*/
	uint32_t smb_verid; /**< reg1*/

	union {
			struct {
				uint32_t soft_reset:  1;  /**< reg2[0]*/
				uint32_t clkg_bypass: 1;  /**< reg2[1]*/
				uint32_t reserved:    30; /**< reg2[2:31] */
			};
			uint32_t value;
	} smb_clkrst;

	uint32_t smb_state; /**< reg3*/

	union {
			struct {
				uint32_t box0_ready: 1;  /**< reg4[0]*/
				uint32_t box1_ready: 1;  /**< reg4[0]*/
				uint32_t reserved:   30; /**< reg4[2:31] */
			};
			uint32_t value;
		} box_ready;

	union {
			struct {
				uint32_t box0_clear: 1;  /**< reg5[0]*/
				uint32_t box1_clear: 1;  /**< reg5[0]*/
				uint32_t reserved:    30; /**< reg5[2:31] */
			};
			uint32_t value;
		} box_clear;

	union {
			struct {
				uint32_t cpu0_sender: 1;  /**< reg6[0]*/
				uint32_t cpu1_sender: 1;  /**< reg6[0]*/
				uint32_t reserved:    30; /**< reg6[2:31] */
			};
			uint32_t value;
		} sender;

	union {
			struct {
				uint32_t cpu0_receiver: 1;  /**< reg7[0]*/
				uint32_t cpu1_receiver: 1;  /**< reg7[0]*/
				uint32_t reserved:     30;  /**< reg7[2:31] */
			};
			uint32_t value;
		} receiver;

	uint32_t box0_param0; /**< reg8*/
	uint32_t box0_param1; /**< reg9*/
	uint32_t box0_param2; /**< rega*/
	uint32_t box0_param3; /**< regb*/
	uint32_t box1_param0; /**< regc*/
	uint32_t box1_param1; /**< regd*/
	uint32_t box1_param2; /**< rege*/
	uint32_t box1_param3; /**< regf*/

} s_mailbox_t;

#ifdef __cplusplus
}
#endif

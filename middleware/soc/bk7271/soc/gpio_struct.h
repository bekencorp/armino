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

#include <soc/soc.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
	struct {
		union {
			struct {
				uint32_t gpio_input:		1; /**< bit[0] gpio_input_bit, R*/
				uint32_t gpio_output:		1; /**< bit[1] gpio_output_bit, R/W */
				uint32_t gpio_input_en:		1; /**< bit[2] gpio_input_en_bit, R/W */
				uint32_t gpio_output_en:	1; /**< bit[3] gpio_output_en_bit, R/W */
				uint32_t gpio_pull_mode:	1; /**< bit[4] gpio_pull_mode_bit: 1:pull_up 0:pull_down*/
				uint32_t gpio_pull_mode_en:	1; /**< bit[5] gpio_pull_mode_en_bit, defult:1R/W */
				uint32_t gpio_2_func_en:	1; /**< bit[6] gpio_2_func_en_bit, R/W */
				uint32_t gpio_input_monitor:	1; /**< bit[7] view gpio input values, R/W */
				uint32_t gpio_capacity:		2; /**< bit[8:9] gpio driver capacity,R/W */

				uint32_t reserved:		22;
			};
			uint32_t v;
		} cfg;
	}gpio_num[SOC_GPIO_NUM];

	uint32_t reserved_2[8];

	//reg0x30 int_type1
	union {
		struct {
			uint32_t gpio0_int_type:	2; /**< bit[0:1]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio1_int_type:	2; /**< bit[2:3]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio2_int_type:	2; /**< bit[4:5]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio3_int_type:	2; /**< bit[6:7]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio4_int_type:	2; /**< bit[8:9]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio5_int_type:	2; /**< bit[10:11]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio6_int_type:	2; /**< bit[12:13]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio7_int_type:	2; /**< bit[14:15]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio8_int_type:	2; /**< bit[16:17]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio9_int_type:	2; /**< bit[18:19]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio10_int_type:	2; /**< bit[20:21]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio11_int_type:	2; /**< bit[22:23]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio12_int_type:	2; /**< bit[24:25]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio13_int_type:	2; /**< bit[26:27]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio14_int_type:	2; /**< bit[28:29]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio15_int_type:	2; /**< bit[30:31]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
		};
		uint32_t v;
	} gpio_0_15_int_type;

	//reg0x31 int_type2
	union {
		struct {
			uint32_t gpio16_int_type:	2; /**< bit[0:1]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio17_int_type:	2; /**< bit[2:3]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio18_int_type:	2; /**< bit[4:5]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio19_int_type:	2; /**< bit[6:7]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio20_int_type:	2; /**< bit[8:9]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio21_int_type:	2; /**< bit[10:11]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio22_int_type:	2; /**< bit[12:13]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio23_int_type:	2; /**< bit[14:15]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio24_int_type:	2; /**< bit[16:17]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio25_int_type:	2; /**< bit[18:19]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio26_int_type:	2; /**< bit[20:21]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio27_int_type:	2; /**< bit[22:23]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio28_int_type:	2; /**< bit[24:25]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio29_int_type:	2; /**< bit[26:27]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio30_int_type:	2; /**< bit[28:29]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio31_int_type:	2; /**< bit[30:31]: 0:low_level; 1:high_level; 2:rising edge; 3:falling edge R/W */
		};
		uint32_t v;
	} gpio_16_31_int_type;

	//reg0x32 int_type3
	union {
		struct {
			uint32_t gpio32_int_type:	2; /**< bit[0:1]: 0:low_level; 1:high_levle; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio33_int_type:	2; /**< bit[2:3]: 0:low_level; 1:high_levle; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio34_int_type:	2; /**< bit[4:5]: 0:low_level; 1:high_levle; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio35_int_type:	2; /**< bit[6:7]: 0:low_level; 1:high_levle; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio36_int_type:	2; /**< bit[8:9]: 0:low_level; 1:high_levle; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio37_int_type:	2; /**< bit[10:11]: 0:low_level; 1:high_levle; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio38_int_type:	2; /**< bit[12:13]: 0:low_level; 1:high_levle; 2:rising edge; 3:falling edge R/W */
			uint32_t gpio39_int_type:	2; /**< bit[14:15]: 0:low_level; 1:high_levle; 2:rising edge; 3:falling edge R/W */

			uint32_t reserved:		16;
		};
		uint32_t v;
	} gpio_32_39_int_type;


	//reg0x33 int_enable1
	union {
		struct {
			uint32_t gpio0_int_enable:	1; /**< bit[0]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio1_int_enable:	1; /**< bit[1]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio2_int_enable:	1; /**< bit[2]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio3_int_enable:	1; /**< bit[3]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio4_int_enable:	1; /**< bit[4]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio5_int_enable:	1; /**< bit[5]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio6_int_enable:	1; /**< bit[6]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio7_int_enable:	1; /**< bit[7]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio8_int_enable:	1; /**< bit[8]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio9_int_enable:	1; /**< bit[9]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio10_int_enable:	1; /**< bit[10]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio11_int_enable:	1; /**< bit[11]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio12_int_enable:	1; /**< bit[12]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio13_int_enable:	1; /**< bit[13]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio14_int_enable:	1; /**< bit[14]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio15_int_enable:	1; /**< bit[15]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio16_int_enable:	1; /**< bit[16]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio17_int_enable:	1; /**< bit[17]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio18_int_enable:	1; /**< bit[18]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio19_int_enable:	1; /**< bit[19]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio20_int_enable:	1; /**< bit[20]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio21_int_enable:	1; /**< bit[21]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio22_int_enable:	1; /**< bit[22]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio23_int_enable:	1; /**< bit[23]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio24_int_enable:	1; /**< bit[24]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio25_int_enable:	1; /**< bit[25]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio26_int_enable:	1; /**< bit[26]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio27_int_enable:	1; /**< bit[27]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio28_int_enable:	1; /**< bit[28]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio29_int_enable:	1; /**< bit[29]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio30_int_enable:	1; /**< bit[30]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio31_int_enable:	1; /**< bit[31]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
		};
		uint32_t v;
	} gpio_0_31_int_enable;


	//reg0x34 int_enable2
	union {
		struct {
			uint32_t gpio32_int_enable:	1; /**< bit[0]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio33_int_enable:	1; /**< bit[1]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio34_int_enable:	1; /**< bit[2]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio35_int_enable:	1; /**< bit[3]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio36_int_enable:	1; /**< bit[4]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio37_int_enable:	1; /**< bit[5]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio38_int_enable:	1; /**< bit[6]:gpio_int_enable_bit: 0:disable; 1:enable R/W */
			uint32_t gpio39_int_enable:	1; /**< bit[7]:gpio_int_enable_bit: 0:disable; 1:enable R/W */

			uint32_t reserved:		24;
		};
		uint32_t v;
	} gpio_32_39_int_enable;


	//reg0x35 int_status1
	union {
		struct {
			uint32_t gpio0_int_st:	   1; /**< bit[0] interrupt status, write 1 to clear*/
			uint32_t gpio1_int_st:	   1; /**< bit[1] Interrupt status, write 1 to clear*/
			uint32_t gpio2_int_st:	   1; /**< bit[2] Interrupt status, write 1 to clear*/
			uint32_t gpio3_int_st:	   1; /**< bit[3] interrupt status, write 1 to clear*/
			uint32_t gpio4_int_st:	   1; /**< bit[4] Interrupt status, write 1 to clear*/
			uint32_t gpio5_int_st:	   1; /**< bit[5] Interrupt status, write 1 to clear*/
			uint32_t gpio6_int_st:	   1; /**< bit[6] interrupt status, write 1 to clear*/
			uint32_t gpio7_int_st:	   1; /**< bit[7] Interrupt status, write 1 to clear*/
			uint32_t gpio8_int_st:	   1; /**< bit[8] Interrupt status, write 1 to clear*/
			uint32_t gpio9_int_st:	   1; /**< bit[9] interrupt status, write 1 to clear*/
			uint32_t gpio10_int_st:	   1; /**< bit[10] Interrupt status, write 1 to clear*/
			uint32_t gpio11_int_st:	   1; /**< bit[11] Interrupt status, write 1 to clear*/
			uint32_t gpio12_int_st:	   1; /**< bit[12] interrupt status, write 1 to clear*/
			uint32_t gpio13_int_st:	   1; /**< bit[13] Interrupt status, write 1 to clear*/
			uint32_t gpio14_int_st:	   1; /**< bit[14] Interrupt status, write 1 to clear*/
			uint32_t gpio15_int_st:	   1; /**< bit[15] interrupt status, write 1 to clear*/
			uint32_t gpio16_int_st:	   1; /**< bit[16] Interrupt status, write 1 to clear*/
			uint32_t gpio17_int_st:	   1; /**< bit[17] Interrupt status, write 1 to clear*/
			uint32_t gpio18_int_st:	   1; /**< bit[18] Interrupt status, write 1 to clear*/
			uint32_t gpio19_int_st:	   1; /**< bit[19] Interrupt status, write 1 to clear*/
			uint32_t gpio20_int_st:    1; /**< bit[20] Interrupt status, write 1 to clear*/
			uint32_t gpio21_int_st:    1; /**< bit[21] Interrupt status, write 1 to clear*/
			uint32_t gpio22_int_st:    1; /**< bit[22] interrupt status, write 1 to clear*/
			uint32_t gpio23_int_st:    1; /**< bit[23] Interrupt status, write 1 to clear*/
			uint32_t gpio24_int_st:    1; /**< bit[24] Interrupt status, write 1 to clear*/
			uint32_t gpio25_int_st:    1; /**< bit[25] interrupt status, write 1 to clear*/
			uint32_t gpio26_int_st:    1; /**< bit[26] Interrupt status, write 1 to clear*/
			uint32_t gpio27_int_st:    1; /**< bit[27] Interrupt status, write 1 to clear*/
			uint32_t gpio28_int_st:    1; /**< bit[28] Interrupt status, write 1 to clear*/
			uint32_t gpio29_int_st:    1; /**< bit[29] Interrupt status, write 1 to clear*/
			uint32_t gpio30_int_st:	   1; /**< bit[30] interrupt status, write 1 to clear*/
			uint32_t gpio31_int_st:	   1; /**< bit[31] Interrupt status, write 1 to clear*/
		};
		uint32_t v;
	}gpio_0_31_int_st;

	//reg0x36 int_status2
	union {
		struct {
			uint32_t gpio32_int_st:		1; /**< bit[0] interrupt status, write 1 to clear*/
			uint32_t gpio33_int_st:		1; /**< bit[1] Interrupt status, write 1 to clear*/
			uint32_t gpio34_int_st:		1; /**< bit[2] Interrupt status, write 1 to clear*/
			uint32_t gpio35_int_st:		1; /**< bit[3] interrupt status, write 1 to clear*/
			uint32_t gpio36_int_st:		1; /**< bit[4] Interrupt status, write 1 to clear*/
			uint32_t gpi037_int_st:		1; /**< bit[5] Interrupt status, write 1 to clear*/
			uint32_t gpio38_int_st:		1; /**< bit[6] interrupt status, write 1 to clear*/
			uint32_t gpio39_int_st:		1; /**< bit[7] Interrupt status, write 1 to clear*/

			uint32_t reserved:		24;

		};
		uint32_t v;
	}gpio_32_39_int_st;
} gpio_hw_t;

#ifdef __cplusplus
}
#endif

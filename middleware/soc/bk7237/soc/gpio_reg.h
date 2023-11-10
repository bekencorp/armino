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

//TODO place all auto-generated Registers here!!!

#define GPIO_V_MODE_SECOND_FUNC					0x48
#define GPIO_V_MODE_SECOND_FUNC_PULL_UP			0x78
#define GPIO_V_MODE_SECOND_FUNC_PULL_DOWN		0x68
#define GPIO_V_MODE_INPUT_PULL_UP				0x3C
#define GPIO_V_MODE_INPUT_PULL_DOWN				0x2C
#define GPIO_V_MODE_INPUT						0x0C
#define GPIO_V_MODE_OUTPUT_PULL_UP				0x30
#define GPIO_V_MODE_OUTPUT_PULL_DOWN			0x20
#define GPIO_V_MODE_OUTPUT						0x00
#define GPIO_V_MODE_HIGH_IMPENDANCE				0x08
#define GPIO_V_MODE_DEFAULT						0x28

#define GPIO_F_IO_MODE_EN					2
#define GPIO_F_IO_MODE_SET(mode)			mode << 2
#define GPIO_F_IO_MODE_EN_M					0x03 << 2

#define GPIO_F_PULL_EN						4
#define GPIO_F_PULL_SET(mode)				mode << 4
#define GPIO_F_PULL_EN_M					0x03 << 4

#define GPIO_F_FUNC_EN						(BIT(6))
#define GPIO_F_FUNC_EN_S					0x6
#define GPIO_F_FUNC_EN_V					0x1

#define GPIO_F_PERIAL_MODE					0xF
#define GPIO_F_PERIAL_MODE_V				0xF	//mask value for valid bits. (2 power GPIO_F_PERIAL_MODE_S) - 1.
#define GPIO_F_PERIAL_MODE_S				0x4	//each channel shift bits number.
#define GPIO_F_PERIAL_MODE_MS(_ch)			( 4 * (_ch))
#define GPIO_F_PERIAL_MODE_M				(GPIO_F_PERIAL_MODE_V<<GPIO_F_PERIAL_MODE_S)

#define GPIO_V_PERIAL_MODE1					0x00
#define GPIO_V_PERIAL_MODE2					0x01
#define GPIO_V_PERIAL_MODE3					0x02
#define GPIO_V_PERIAL_MODE4					0x03
#define GPIO_V_PERIAL_MODE5					0x04
#define GPIO_V_PERIAL_MODE6					0x05

#define GPIO_F_INT_EN						0x01
#define GPIO_F_INT_EN_V						0x01
#define GPIO_F_INT_EN_S						0x01
#define GPIO_F_INT_EN_MS(_ch)				( _ch)
#define GPIO_F_INT_EN_M						(GPIO_F_INT_EN_V<<GPIO_F_INT_EN_S)


#define GPIO_V_TYPE_MODE_LOW_VOL			0x00
#define GPIO_V_TYPE_MODE_HIGH_VOL			0x01
#define GPIO_V_TYPE_MODE_RISING_EDGE		0x02
#define GPIO_V_TYPE_MODE_FALLING_EDGE		0x03

#define GPIO_F_INT_TYPE_MODE				0x03
#define GPIO_F_INT_TYPE_MODE_V				0x03
#define GPIO_F_INT_TYPE_MODE_S				0x02
#define GPIO_F_INT_TYPE_MODE_MS(_ch)		( 2 * (_ch))
#define GPIO_F_INT_TYPE_MODE_M				(GPIO_F_INT_TYPE_MODE<<GPIO_F_INT_TYPE_MODE_S)


#ifdef __cplusplus
}
#endif


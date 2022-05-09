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
#include <stdio.h>
#include <soc/soc.h>
#include <driver/hal/hal_adc_types.h>
#include <driver/hal/hal_gpio_types.h>

#define INVALID	0

#define ADC_DEV_MAP  \
{\
	{0, GPIO_11, GPIO_DEV_ADC0},\
	{ADC_1, GPIO_18, GPIO_DEV_ADC1},\
	{ADC_2, GPIO_19, GPIO_DEV_ADC2},\
	{ADC_3, GPIO_20, GPIO_DEV_ADC3},\
	{ADC_4, GPIO_21, GPIO_DEV_ADC4},\
	{ADC_5, GPIO_22, GPIO_DEV_ADC5},\
	{ADC_6, GPIO_23, GPIO_DEV_ADC6},\
	{ADC_7, GPIO_31, GPIO_DEV_ADC7},\
	{ADC_8, GPIO_32, GPIO_DEV_ADC8},\
	{ADC_9, GPIO_34, GPIO_DEV_ADC9},\
	{ADC_10, GPIO_36, GPIO_DEV_ADC10},\
	{ADC_11, GPIO_37, GPIO_DEV_ADC11},\
	{ADC_12, GPIO_38, GPIO_DEV_ADC12},\
	{ADC_13, GPIO_39, GPIO_DEV_ADC13},\
}

#ifdef __cplusplus
}
#endif

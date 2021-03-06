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
#include <driver/hal/hal_gpio_types.h>
#include <soc/soc.h>

#define BIT_64(i)	((1L) << (i))
#define GPIO_DEV_MAP  \
{\
	{GPIO_0, {GPIO_DEV_UART2_TXD, GPIO_DEV_I2C1_SCL, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_1, {GPIO_DEV_UART2_RXD, GPIO_DEV_I2C1_SDA, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_2, {GPIO_DEV_I2S1_CLK, GPIO_DEV_ADC5, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_3, {GPIO_DEV_I2S1_SYNC, GPIO_DEV_ADC4, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_4, {GPIO_DEV_I2S1_DIN, GPIO_DEV_ADC1, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_5, {GPIO_DEV_I2S1_DOUT, GPIO_DEV_ADC2, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_6, {GPIO_DEV_CLK13M, GPIO_DEV_PWM0, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_7, {GPIO_DEV_WIFI_ACTIVE, GPIO_DEV_PWM1, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_8, {GPIO_DEV_BT_ACTIVE, GPIO_DEV_PWM2, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_9, {GPIO_DEV_BT_PRIORITY, GPIO_DEV_PWM3, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_10,{GPIO_DEV_UART1_RXD, GPIO_DEV_INVALID, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_11, {GPIO_DEV_UART1_TXD, GPIO_DEV_INVALID, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_12, {GPIO_DEV_UART1_CTS, GPIO_DEV_ADC6, GPIO_DEV_PGA_INP, GPIO_DEV_INVALID}},\
	{GPIO_13, {GPIO_DEV_UART1_RTS, GPIO_DEV_ADC7, GPIO_DEV_PGA_INN, GPIO_DEV_INVALID}},\
	{GPIO_14, {GPIO_DEV_SDIO_HOST_CLK, GPIO_DEV_SPI0_SCK, GPIO_DEV_QSPI_FLASH_CLK, GPIO_DEV_INVALID}},\
	{GPIO_15, {GPIO_DEV_SDIO_HOST_DATA3, GPIO_DEV_SPI0_CSN, GPIO_DEV_QSPI_FLASH_CSN, GPIO_DEV_INVALID}},\
	{GPIO_16, {GPIO_DEV_SDIO_HOST_CMD, GPIO_DEV_SPI0_MOSI, GPIO_DEV_QSPI_IO0, GPIO_DEV_INVALID}},\
	{GPIO_17, {GPIO_DEV_SDIO_HOST_DATA0, GPIO_DEV_SPI0_MISO, GPIO_DEV_QSPI_IO1, GPIO_DEV_INVALID}},\
	{GPIO_18, {GPIO_DEV_SDIO_HOST_DATA2, GPIO_DEV_QSPI_IO2, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_19, {GPIO_DEV_SDIO_HOST_DATA1, GPIO_DEV_QSPI_IO3, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_20, {GPIO_DEV_I2C0_SCL, GPIO_DEV_JTAG_TCK, GPIO_DEV_QSPI_IO3, GPIO_DEV_INVALID}},\
	{GPIO_21, {GPIO_DEV_I2C0_SDA, GPIO_DEV_JTAG_TMS, GPIO_DEV_QSPI_IO2, GPIO_DEV_INVALID}},\
	{GPIO_22, {GPIO_DEV_CLK26M, GPIO_DEV_JTAG_TDI, GPIO_DEV_QSPI_IO1, GPIO_DEV_INVALID}},\
	{GPIO_23, {GPIO_DEV_ADC3, GPIO_DEV_JTAG_TDO, GPIO_DEV_QSPI_IO0, GPIO_DEV_INVALID}},\
	{GPIO_24, {GPIO_DEV_LPO_CLK, GPIO_DEV_PWM4, GPIO_DEV_QSPI_RAM_CLK, GPIO_DEV_INVALID}},\
	{GPIO_25, {GPIO_DEV_TXEN, GPIO_DEV_USB0_DP, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_26, {GPIO_DEV_IRDA, GPIO_DEV_PWM5, GPIO_DEV_QSPI_RAM_CSN, GPIO_DEV_INVALID}},\
	{GPIO_27, {GPIO_DEV_JPEG_MCLK, GPIO_DEV_INVALID, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_28, {GPIO_DEV_RXEN, GPIO_DEV_USB0_DN, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_29, {GPIO_DEV_JPEG_PCLK, GPIO_DEV_INVALID, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_30, {GPIO_DEV_JPEG_HSYNC, GPIO_DEV_SPI0_SCK, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_31, {GPIO_DEV_JPEG_VSYNC, GPIO_DEV_SPI0_CSN, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_32, {GPIO_DEV_JPEG_PXDATA0, GPIO_DEV_SPI0_MOSI, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_33, {GPIO_DEV_JPEG_PXDATA1, GPIO_DEV_SPI0_MISO, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_34, {GPIO_DEV_JPEG_PXDATA2, GPIO_DEV_SDIO_HOST_CLK, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_35, {GPIO_DEV_JPEG_PXDATA3, GPIO_DEV_SDIO_HOST_CMD, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_36, {GPIO_DEV_JPEG_PXDATA4, GPIO_DEV_SDIO_HOST_DATA0, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_37, {GPIO_DEV_JPEG_PXDATA5, GPIO_DEV_SDIO_HOST_DATA1, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_38, {GPIO_DEV_JPEG_PXDATA6, GPIO_DEV_SDIO_HOST_DATA2, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
	{GPIO_39, {GPIO_DEV_JPEG_PXDATA7, GPIO_DEV_SDIO_HOST_DATA3, GPIO_DEV_INVALID, GPIO_DEV_INVALID}},\
}

#define GPIO_MAP_TABLE(DEV_NUM, MODE_NUM, table) \
					 struct {\
						 uint64_t gpio_bits;\
						 uint8_t devs[(DEV_NUM)];\
					 } table[(MODE_NUM)]

#define GPIO_SDIO_MAP_TABLE \
				 {\
					 {BIT(14)|BIT(15)|BIT(16)|BIT(17)|BIT(18)|BIT(19), {GPIO_DEV_SDIO_HOST_CLK, GPIO_DEV_SDIO_HOST_DATA3, GPIO_DEV_SDIO_HOST_CMD,GPIO_DEV_SDIO_HOST_DATA0,GPIO_DEV_SDIO_HOST_DATA2,GPIO_DEV_SDIO_HOST_DATA1}},\
					 {BIT(14)|BIT(15)|BIT(16)|BIT(17)|BIT(18)|BIT(19), {GPIO_DEV_SDIO_HOST_CLK, GPIO_DEV_SDIO_HOST_DATA3, GPIO_DEV_SDIO_HOST_CMD,GPIO_DEV_SDIO_HOST_DATA0,GPIO_DEV_SDIO_HOST_DATA2,GPIO_DEV_SDIO_HOST_DATA1}},\
					 {BIT64(34)|BIT64(35)|BIT64(36)|BIT64(37)|BIT64(38)|BIT64(39), {GPIO_DEV_SDIO_HOST_CLK, GPIO_DEV_SDIO_HOST_CMD, GPIO_DEV_SDIO_HOST_DATA0,GPIO_DEV_SDIO_HOST_DATA1,GPIO_DEV_SDIO_HOST_DATA2,GPIO_DEV_SDIO_HOST_DATA3}},\
					 {BIT64(34)|BIT64(35)|BIT64(36)|BIT64(37)|BIT64(38)|BIT64(39), {GPIO_DEV_SDIO_HOST_CLK, GPIO_DEV_SDIO_HOST_CMD, GPIO_DEV_SDIO_HOST_DATA0,GPIO_DEV_SDIO_HOST_DATA1,GPIO_DEV_SDIO_HOST_DATA2,GPIO_DEV_SDIO_HOST_DATA3}},\
				}

#define GPIO_SDIO_USED_GPIO_NUM 6

#define GPIO_SPI_MAP_TABLE \
				 {\
					{BIT(14)|BIT(15)|BIT(16)|BIT(17), {GPIO_DEV_SPI0_SCK, GPIO_DEV_SPI0_CSN, GPIO_DEV_SPI0_MOSI, GPIO_DEV_SPI0_MISO}},\
					{BIT(14)|BIT(15)|BIT(16)|BIT(17), {GPIO_DEV_SPI0_SCK, GPIO_DEV_SPI0_CSN, GPIO_DEV_SPI0_MOSI, GPIO_DEV_SPI0_MISO}},\
					{BIT(30)|BIT(31)|BIT64(32)|BIT64(33), {GPIO_DEV_SPI0_SCK, GPIO_DEV_SPI0_CSN, GPIO_DEV_SPI0_MOSI, GPIO_DEV_SPI0_MISO}},\
					{BIT(30)|BIT(31)|BIT64(32)|BIT64(33), {GPIO_DEV_SPI0_SCK, GPIO_DEV_SPI0_CSN, GPIO_DEV_SPI0_MOSI, GPIO_DEV_SPI0_MISO}},\
					}

#define GPIO_SPI_USED_GPIO_NUM 4


#ifdef __cplusplus
}
#endif

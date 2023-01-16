// Copyright 2022-2023 Beken
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

typedef struct {
	uint8_t dev;
	int8_t cfg_bit;
	int8_t reg;
	int8_t bit;
} prro_map_t;

#define PRRO_INVALID_REG -1
#define PRRO_AP_MAP(ap_map) \
prro_map_t ap_map[] = \
{\
	{PRRO_DEV_AHB_AON,        0, 4,  0 },\
	{PRRO_DEV_AHB_SYSTEM,     1, 4,  1 },\
	{PRRO_DEV_AHB_FLASH,      2, 4,  2 },\
	{PRRO_DEV_AHB_GDMA,       3, 4,  3 },\
	{PRRO_DEV_AHB_LA,         4, 4,  4 },\
	{PRRO_DEV_AHB_USB,        5, 4,  5 },\
	{PRRO_DEV_AHB_CAN,        6, 4,  6 },\
	{PRRO_DEV_AHB_QSPI0,      7, 4,  7 },\
	{PRRO_DEV_AHB_QSPI1,      8, 4,  8 },\
	{PRRO_DEV_AHB_PSRAM,      9, 4,  9 },\
	{PRRO_DEV_AHB_FFT,        10, 4, 10},\
	{PRRO_DEV_AHB_SBC,        11, 4, 11},\
	{PRRO_DEV_AHB_JPEG_ENC,   12, 4, 12},\
	{PRRO_DEV_AHB_JPEG_DEC,   13, 4, 14},\
	{PRRO_DEV_AHB_YUV,        14, 4, 13},\
	{PRRO_DEV_AHB_LCD_DISP,   15, 4, 15},\
	{PRRO_DEV_AHB_DMA2D,      16, 4, 16},\
	{PRRO_DEV_AHB_ROTT,       17, 4, 17},\
	{PRRO_DEV_AHB_WIFI_MAC,   18, 4, 18},\
	{PRRO_DEV_AHB_WIFI_MODEM, 19, 4, 19},\
	{PRRO_DEV_AHB_BTDM_AHB,   20, 4, 20},\
	{PRRO_DEV_AHB_MBOX0,      21, 4, 21},\
	{PRRO_DEV_AHB_MBOX1,      22, 4, 22},\
	{PRRO_DEV_AHB_DMA1,       23, 4, 23},\
	{PRRO_DEV_AHB_MAX,        -1, PRRO_INVALID_REG, -1},\
\
	{PRRO_DEV_APB_WDT,        32, 7, 0 },\
	{PRRO_DEV_APB_EFUSE,      33, 7, 4 },\
	{PRRO_DEV_APB_MBIC,       34, 7, 5 },\
	{PRRO_DEV_APB_SDMADC,     35, 7, 7 },\
	{PRRO_DEV_APB_TIMER0,     36, 7, 1 },\
	{PRRO_DEV_APB_TIMER1,     37, 7, 6 },\
	{PRRO_DEV_APB_PWM0,       38, 7, 14 },\
	{PRRO_DEV_APB_PWM1,       39, 7, 19 },\
	{PRRO_DEV_APB_UART0,      40, 7, 2 },\
	{PRRO_DEV_APB_UART1,      41, 7, 8 },\
	{PRRO_DEV_APB_UART2,      42, 7, 9 },\
	{PRRO_DEV_APB_SPI0,       43, 7, 3},\
	{PRRO_DEV_APB_SPI1,       44, 7, 12},\
	{PRRO_DEV_APB_I2C0,       45, 7, 10},\
	{PRRO_DEV_APB_I2C1,       46, 7, 11},\
	{PRRO_DEV_APB_SARADC,     47, 7, 13},\
	{PRRO_DEV_APB_IRDA,       48, 7, 15},\
	{PRRO_DEV_APB_TRNG,       49, 7, 16},\
	{PRRO_DEV_APB_SDIO,       50, 7, 17},\
	{PRRO_DEV_APB_SLCD,       51, 7, 18},\
	{PRRO_DEV_APB_AUDIO,      52, 7, 20},\
	{PRRO_DEV_APB_I2S0,       53, 7, 21},\
	{PRRO_DEV_APB_I2S1,       54, 7, 22},\
	{PRRO_DEV_APB_I2S2,       55, 7, 23},\
	{PRRO_DEV_APB_VIDP,       56, 7, 24},\
	{PRRO_DEV_APB_BTDM_APB,   57, 7, 25},\
	{PRRO_DEV_APB_MAX,        -1,  PRRO_INVALID_REG, -1}\
}

#define PRRO_SEC_MAP(sec_map)\
prro_map_t sec_map[] = \
{\
	{PRRO_DEV_AHB_AON,        0, 5,  0 },\
	{PRRO_DEV_AHB_SYSTEM,     1, 5,  1 },\
	{PRRO_DEV_AHB_FLASH,      2, 5,  2 },\
	{PRRO_DEV_AHB_GDMA,       3, 5,  3 },\
	{PRRO_DEV_AHB_LA,         4, 5,  4 },\
	{PRRO_DEV_AHB_USB,        5, 5,  5 },\
	{PRRO_DEV_AHB_CAN,        6, 5,  6 },\
	{PRRO_DEV_AHB_QSPI0,      7, 5,  7 },\
	{PRRO_DEV_AHB_QSPI1,      8, 5,  8 },\
	{PRRO_DEV_AHB_PSRAM,      9, 5,  9 },\
	{PRRO_DEV_AHB_FFT,        10, 5, 10},\
	{PRRO_DEV_AHB_SBC,        11, 5, 11},\
	{PRRO_DEV_AHB_JPEG_ENC,   12, 5, 12},\
	{PRRO_DEV_AHB_JPEG_DEC,   13, 5, 14},\
	{PRRO_DEV_AHB_YUV,        14, 5, 13},\
	{PRRO_DEV_AHB_LCD_DISP,   15, 5, 15},\
	{PRRO_DEV_AHB_DMA2D,      16, 5, 16},\
	{PRRO_DEV_AHB_ROTT,       17, 5, 17},\
	{PRRO_DEV_AHB_WIFI_MAC,   18, 5, 18},\
	{PRRO_DEV_AHB_WIFI_MODEM, 19, 5, 19},\
	{PRRO_DEV_AHB_BTDM_AHB,   20, 5, 20},\
	{PRRO_DEV_AHB_MBOX0,      21, 5, 21},\
	{PRRO_DEV_AHB_MBOX1,      22, 5, 22},\
	{PRRO_DEV_AHB_DMA1,       23, 5, 23},\
	{PRRO_DEV_AHB_MAX,        -1,  PRRO_INVALID_REG, -1},\
\
	{PRRO_DEV_APB_WDT,        0, 8, 0 },\
	{PRRO_DEV_APB_EFUSE,      1, 8, 4 },\
	{PRRO_DEV_APB_MBIC,       2, 8, 5 },\
	{PRRO_DEV_APB_SDMADC,     3, 8, 7 },\
	{PRRO_DEV_APB_TIMER0,     4, 8, 1 },\
	{PRRO_DEV_APB_TIMER1,     5, 8, 6 },\
	{PRRO_DEV_APB_PWM0,       6, 8, 14 },\
	{PRRO_DEV_APB_PWM1,       7, 8, 19 },\
	{PRRO_DEV_APB_UART0,      8, 8, 2 },\
	{PRRO_DEV_APB_UART1,      9, 8, 8 },\
	{PRRO_DEV_APB_UART2,      10, 8, 9 },\
	{PRRO_DEV_APB_SPI0,       11, 8, 3},\
	{PRRO_DEV_APB_SPI1,       12, 8, 12},\
	{PRRO_DEV_APB_I2C0,       13, 8, 10},\
	{PRRO_DEV_APB_I2C1,       14, 8, 11},\
	{PRRO_DEV_APB_SARADC,     15, 8, 13},\
	{PRRO_DEV_APB_IRDA,       16, 8, 15},\
	{PRRO_DEV_APB_TRNG,       17, 8, 16},\
	{PRRO_DEV_APB_SDIO,       18, 8, 17},\
	{PRRO_DEV_APB_SLCD,       19, 8, 18},\
	{PRRO_DEV_APB_AUDIO,      20, 8, 20},\
	{PRRO_DEV_APB_I2S0,       21, 8, 21},\
	{PRRO_DEV_APB_I2S1,       22, 8, 22},\
	{PRRO_DEV_APB_I2S2,       23, 8, 23},\
	{PRRO_DEV_APB_VIDP,       24, 8, 24},\
	{PRRO_DEV_APB_BTDM_APB,   25, 8, 25},\
	{PRRO_DEV_APB_MAX,        -1,  PRRO_INVALID_REG, -1},\
\
	{PRRO_DEV_JPEC_ENC_HNONSEC, 0, 0xd, 0},\
	{PRRO_DEV_JPEC_DEC_HNONSEC, 1, 0xe, 0},\
	{PRRO_DEV_DMA2D_HNONSEC,    2, 0xf, 0},\
	{PRRO_DEV_WIFI_MAC_HNONSEC, 3, 0x10, 0},\
	{PRRO_DEV_HSU_HNONSEC,      4, 0x11, 0},\
	{PRRO_DEV_BTDM_HNONSEC,     5, 0x12, 0},\
	{PRRO_DEV_LA_HNONSEC,       6, 0x13, 0},\
	{PRRO_DEV_LCD_DISP_HNONSEC, 7, 0x14, 0},\
	{PRRO_DEV_ENC_HNONSEC,      8, 0x15, 0},\
	{PRRO_DEV_MAX_HNONSEC,      -1, PRRO_INVALID_REG,    -1}\
}


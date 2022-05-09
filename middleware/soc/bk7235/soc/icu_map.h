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
#include <driver/int_types.h>
#include <soc/soc.h>

#define GROUP0	0
#define GROUP1	1
#define INVALID	0

/*
 * 1.NOTICE: help confirm this array sequence is the same as icu_int_src_t,
 *   because the code in API of "bk_int_isr_register" uses the value as index of INT_SRC.
 * 2.The PRIOURITY in this array is inherit from ARM(IRQ/FIQ), not valid in RISC-V.
 *   so here uses the default value which is 1.We'll modify it after the priourity
 *   feature is finish.
 * 3.There aren't exist some IRQ numbers: 28,42~47,51,56~63
 */
#define ICU_DEV_MAP	\
{\
	{INT_SRC_UART1,   4, IQR_PRI_DEFAULT,GROUP0}, \
	{INT_SRC_UART2,   15, IQR_PRI_DEFAULT,GROUP0}, \
	{INT_SRC_I2C0,  6, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_IRDA,  9, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_I2S,    24, IQR_PRI_DEFAULT,GROUP0}, \
	{INT_SRC_I2C1,  14, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_SPI,    7, IQR_PRI_DEFAULT,GROUP0}, \
	{INT_SRC_GPIO,  55, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_TIMER,   3, IQR_PRI_DEFAULT,GROUP0}, \
	{INT_SRC_PWM,    5, IQR_PRI_DEFAULT,GROUP0}, \
	{INT_SRC_AUDIO,   23, IQR_PRI_DEFAULT,    GROUP0}, \
	{INT_SRC_SARADC,  8, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_SDIO,  10, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_USB,    19, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_FFT,	    21, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_GDMA,  11, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_MODEM,   29, IQR_PRI_DEFAULT,    GROUP0}, \
	{INT_SRC_MAC_TXRX_TIMER,  31, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_MAC_TXRX_MISC,   32, IQR_PRI_DEFAULT,    GROUP1}, \
	{INT_SRC_MAC_RX_TRIGGER,  33, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_MAC_TX_TRIGGER,  34, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_MAC_PROT_TRIGGER,  35, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_MAC_GENERAL,  36, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_SDIO_DMA,  INVALID, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_USB_PLUG_INOUT,  53, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_SECURITY,  INVALID, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_MAC_WAKEUP,  38, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_HSSPI_SLAVE,  INVALID, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_PLL_UNLOCK,  INVALID, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_JPEG_DEC,  26, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_BLE,    40, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_PSRAM,   INVALID, IQR_PRI_DEFAULT,    GROUP0}, \
	{INT_SRC_LA,    12, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_BTDM,  39, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_BT,    41, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_UART3,   16, IQR_PRI_DEFAULT,    GROUP0}, \
	{INT_SRC_I2C2,  INVALID, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_SPI2,  17, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_SPI3,  INVALID, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_PWM2,  5, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_USB2,  INVALID, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_MAILBOX0,  48, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_MAILBOX1,  49, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_BT_WDT,  INVALID, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_DSP_WDT,  INVALID, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_RTC,    54, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_TOUCH,   52, IQR_PRI_DEFAULT,    GROUP1}, \
	{INT_SRC_CEC,    INVALID, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_MODEM_RC,  30, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_MAC_HSU,  37, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_TIMER1,   13, ICU_PRI_IRQ_TIMER,GROUP0}, \
	{INT_SRC_MAC_INTN_PHY,  INVALID, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_MAC_INT_GEN,  INVALID, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_MAC_INT_RESERVED0, INVALID, IQR_PRI_DEFAULT,   GROUP1}, \
	{INT_SRC_JPEG_ENC,25, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_EIP130_SEC,2, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_EIP130,  1, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_LCD,    27, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_QSPI,  20, ICU_PRI_IRQ_QSPI,   GROUP0}, \
	{INT_SRC_CAN,    18, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_SBC,    22, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_BMC32,  0, IQR_PRI_DEFAULT,   GROUP0}, \
	{INT_SRC_BMC64,  50, IQR_PRI_DEFAULT,   GROUP1}, \
}

#ifdef __cplusplus
}
#endif

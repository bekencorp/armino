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

#include <driver/int_types.h>
#include <soc/soc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GROUP0     0
#define GROUP1     1
#define INVALID    0

#define ICU_DEV_MAP  \
{\
	{INT_SRC_UART1, 0, ICU_PRI_IRQ_UART1 , GROUP0}, \
	{INT_SRC_UART2, 1, ICU_PRI_IRQ_UART2 , GROUP0}, \
	{INT_SRC_I2C0, 2, ICU_PRI_IRQ_I2C0 , GROUP0}, \
	{INT_SRC_IRDA, 3, ICU_PRI_IRQ_IRDA , GROUP0}, \
	{INT_SRC_I2S, INVALID, INVALID , INVALID}, \
	{INT_SRC_I2C1, 5, ICU_PRI_IRQ_I2C1 , GROUP0}, \
	{INT_SRC_SPI, 6, ICU_PRI_IRQ_SPI , GROUP0}, \
	{INT_SRC_GPIO, 7, ICU_PRI_IRQ_GPIO , GROUP0}, \
	{INT_SRC_TIMER, 8, ICU_PRI_IRQ_TIMER , GROUP0}, \
	{INT_SRC_PWM, 9, ICU_PRI_IRQ_PWM , GROUP0}, \
	{INT_SRC_AUDIO, INVALID, INVALID , INVALID}, \
	{INT_SRC_SARADC, 11, ICU_PRI_IRQ_UART1 , GROUP0}, \
	{INT_SRC_SDIO, 12, ICU_PRI_IRQ_SDIO , GROUP0}, \
	{INT_SRC_USB, INVALID, INVALID , INVALID}, \
	{INT_SRC_FFT, INVALID, INVALID , INVALID}, \
	{INT_SRC_GDMA, 15, ICU_PRI_IRQ_GENERDMA , GROUP0}, \
	{INT_SRC_MODEM, 16, ICU_PRI_FIQ_MODEM , GROUP1}, \
	{INT_SRC_MAC_TXRX_TIMER, 17, ICU_PRI_FIQ_MAC_TX_RX_TIMER , GROUP1}, \
	{INT_SRC_MAC_TXRX_MISC, 18, ICU_PRI_FIQ_MAC_TX_RX_MISC , GROUP1}, \
	{INT_SRC_MAC_RX_TRIGGER, 19, ICU_PRI_FIQ_MAC_RX_TRIGGER , GROUP1}, \
	{INT_SRC_MAC_TX_TRIGGER, 20, ICU_PRI_FIQ_MAC_TX_TRIGGER , GROUP1}, \
	{INT_SRC_MAC_PROT_TRIGGER, 21, ICU_PRI_FIQ_MAC_PROT_TRIGGER , GROUP1}, \
	{INT_SRC_MAC_GENERAL, 22, ICU_PRI_FIQ_MAC_GENERAL , GROUP1}, \
	{INT_SRC_SDIO_DMA, INVALID, INVALID , INVALID}, \
	{INT_SRC_USB_PLUG_INOUT, INVALID, INVALID , INVALID}, \
	{INT_SRC_SECURITY, 13, ICU_PRI_FIQ_SECURITY , GROUP1}, \
	{INT_SRC_MAC_WAKEUP, 26, ICU_PRI_FIQ_MAC_WAKEUP , GROUP1}, \
	{INT_SRC_HSSPI_SLAVE, INVALID, INVALID , INVALID}, \
	{INT_SRC_PLL_UNLOCK, 28, ICU_PRI_FIQ_DPLL_UNLOCK , GROUP1}, \
	{INT_SRC_JPEG, INVALID, INVALID , INVALID}, \
	{INT_SRC_BLE, 30, ICU_PRI_FIQ_BLE , GROUP1}, \
	{INT_SRC_PSRAM, INVALID, INVALID , INVALID}, \
	{INT_SRC_LA, 14, ICU_PRI_IRQ_LA , GROUP0}, \
	{INT_SRC_BTDM, 29, ICU_PRI_FIQ_BTDM , GROUP1}, \
	{INT_SRC_BT, 31, ICU_PRI_FIQ_BT , GROUP1}, \
}

#ifdef __cplusplus
}
#endif


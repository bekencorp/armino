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

#include <soc/soc.h>

//TODO place all auto-generated Registers here!!!

#define ICU_R_BASE                     (SOC_ICU_REG_BASE)

#define ICU_R_CLK_MUX                  (ICU_R_BASE + 4 * 0)

#define ICU_F_PWMS_CLK_MUX             (BIT(7))
#define ICU_F_PWMS_CLK_MUX_M           (BIT(7))
#define ICU_F_PWMS_CLK_MUX_V           0x1
#define ICU_F_PWMS_CLK_MUX_S           7
#define ICU_V_PWMS_CLK_MUX_26M         1
#define ICU_V_PWMS_CLK_MUX_DCO         0

#define ICU_R_PWM_CHAN_CLK_MUX         (ICU_R_BASE + 4 * 1)

#define ICU_F_PWM_CHAN_CLK_MUX         (BIT(0))
#define ICU_F_PWM_CHAN_CLK_MUX_M       (BIT(0))
#define ICU_F_PWM_CHAN_CLK_MUX_V       0x1
#define ICU_F_PWM_CHAN_CLK_MUX_S       0
#define ICU_F_PWM_CHAN_CLK_MUX_MS(_ch) (ICU_F_PWM_CHAN_CLK_MUX_S + 1 * (_ch))
#define ICU_V_PWM_CHAN_CLK_MUX_26M     1
#define ICU_V_PWM_CHAN_CLK_MUX_GLOBAL  0

#define ICU_R_PWM_CHAN_PWR             (ICU_R_BASE + 4 * 2)

#define ICU_F_PWM_CHAN_PWR             (BIT(14))
#define ICU_F_PWM_CHAN_PWR_M           (BIT(14))
#define ICU_F_PWM_CHAN_PWR_V           0x1
#define ICU_F_PWM_CHAN_PWR_S           14
#define ICU_F_PWM_CHAN_PWR_MS(_ch)     (ICU_F_PWM_CHAN_PWR_S + 1 * (_ch))
#define ICU_V_PWM_CHAN_PWR_UP          0
#define ICU_V_PWM_CHAN_PWR_DOWN        1

#define ICU_R_TIMER_CHAN_PWR           (ICU_R_BASE + 4 * 2)

#define ICU_F_TIMER_CHAN_PWR           (BIT(12))
#define ICU_F_TIMER_CHAN_PWR_M         (BIT(12))
#define ICU_F_TIMER_CHAN_PWR_V         0x1
#define ICU_F_TIMER_CHAN_PWR_S         12
#define ICU_F_TIMER_CHAN_PWR_MS(_ch)   (ICU_F_TIMER_CHAN_PWR_S + (_ch) / 3)
#define ICU_V_TIMER_CHAN_PWR_UP        0
#define ICU_V_TIMER_CHAN_PWR_DOWN      1

#define ICU_F_I2C_CHAN_CLK_S		4
#define ICU_F_SPI_CHAN_CLK_S		7

#define ICU_F_USB_CHAN_PWR_S		0
#define ICU_F_I2C_CHAN_PWR_S		3
#define ICU_F_SPI_CHAN_PWR_S		8

#define ICU_F_I2C_CHAN_INT_S		3
#define ICU_F_SPI_CHAN_INT_S		8

#define ICU_GPIO_F_PERIAL_MODE			0x03
#define ICU_GPIO_F_PERIAL_MODE_V		0x03
#define ICU_GPIO_F_PERIAL_MODE_S		0x02
#define ICU_GPIO_F_PERIAL_MODE_MS(_ch)		( 2 * (_ch))
#define ICU_GPIO_F_PERIAL_MODE_M		(ICU_GPIO_F_PERIAL_MODE_V<<ICU_GPIO_F_PERIAL_MODE_S)

#define ICU_V_CLK_MUX_26M              1
#define ICU_V_CLK_MUX_DCO              0

#define	ICU_R_FUNC_CLK_PWD             (ICU_R_BASE + 4 * 0x3)

#define PWD_SDIO_DMA_CLK               (1 << 6)
#define PWD_SECURITY_CLK               (1 << 5)
#define PWD_QSPI_CLK                   (1 << 4)
#define PWD_JEPG_CLK                   (1 << 3)
#define PWD_USB2_CLK                   (1 << 2)
#define PWD_USB1_CLK                   (1 << 1)
#define PWD_ARM_WATCHDOG_CLK           (1 << 0)

#define	ICU_R_JTAG_SELECT              (ICU_R_BASE + 4 * 0x5)
#define ICU_V_JTAG_SEL_WR_ARM          (0x7271F00F) /* JTAG_SEL_WR_WIFI actually */
#define ICU_V_JTAG_SEL_WR_BT           (0x7271B00B)
#define ICU_V_JTAG_SEL_WR_TL4          (0x7271D00D) /* JTAG_SEL_WR_DSP actually */
#define ICU_V_JTAG_SEL_RD_WIFI         (0x00000000)
#define ICU_V_JTAG_SEL_RD_BT           (0x00000001)
#define ICU_V_JTAG_SEL_RD_DSP          (0x00000002)

#define	ICU_R_INT_EN                   (ICU_R_BASE + 4 * 0x10)
#define	ICU_R_FIQ_ENABLE               (ICU_R_BASE + 4 * 0x11)
#define	ICU_R_GLOBAL_INT_EN            (ICU_R_BASE + 4 * 0x12)
#define ICU_R_INT_RAW_STATUS           (ICU_R_BASE + 4 * 0x13)
#define	ICU_R_INT_STATUS               (ICU_R_BASE + 4 * 0x15)
#define	ICU_R_FIQ_STATUS               (ICU_R_BASE + 4 * 0x16)
#define ICU_R_ARM_WAKEUP_EN            (ICU_R_BASE + 4 * 0x17)

#define MAX_INT_GROUP_NUM			2
#define FIQ_MAX_COUNT				8
#define IRQ_MAX_COUNT				32
#define FIQ_STATIS_COUNT			FIQ_MAX_COUNT
#define IRQ_STATIS_COUNT			IRQ_MAX_COUNT
#define FIQ_START_COUNT_BIT			0
#define INTC_MAX_COUNT                    (IRQ_MAX_COUNT + FIQ_MAX_COUNT)

#define ICU_PRI_IRQ_MAC_WAKEUP                   (7)
#define ICU_PRI_IRQ_MAC_GENERAL                  (1)
#define ICU_PRI_IRQ_MAC_PROT_TRIGGER             (6)
#define ICU_PRI_IRQ_MAC_TX_TRIGGER               (3)
#define ICU_PRI_IRQ_MAC_RX_TRIGGER               (4)
#define ICU_PRI_IRQ_MAC_TX_RX_MISC               (5)
#define ICU_PRI_IRQ_MAC_TX_RX_TIMER              (2)
#define ICU_PRI_IRQ_MODEM                        (8)
#define ICU_PRI_IRQ_SECURITY                     (22)
#define ICU_PRI_IRQ_MAILBOX1                     (12)	//IRQ_MAILBOX_BT
#define ICU_PRI_IRQ_MAILBOX0                     (11)	//IRQ_MAILBOX_DSP
#define ICU_PRI_IRQ_GENERDMA                     (28)
#define ICU_PRI_IRQ_SDIO_DMA                     (31)
#define ICU_PRI_IRQ_SPI2                         (29)
#define ICU_PRI_IRQ_SPI3                         (30)

#define ICU_PRI_IRQ_USB2                         (14)
#define ICU_PRI_IRQ_USB1                         (13)
#define ICU_PRI_IRQ_QSPI                         (27)
#define ICU_PRI_IRQ_SDIO_HOST                    (15)
#define ICU_PRI_IRQ_SARADC                       (16)
#define ICU_PRI_IRQ_PWM                          (17)
#define ICU_PRI_IRQ_TIMER                        (18)
#define ICU_PRI_IRQ_GPIO                         (19)
#define ICU_PRI_IRQ_SPI                          (20)
#define ICU_PRI_IRQ_I2C2                         (21)
#define ICU_PRI_IRQ_IRDA                         (23)
#define ICU_PRI_IRQ_I2C1                         (24)
#define ICU_PRI_IRQ_I2C0                         (27)
#define ICU_PRI_IRQ_UART3                        (32)
#define ICU_PRI_IRQ_UART2                        (25)
#define ICU_PRI_IRQ_UART1                        (26)

#define ICU_PRI_FIQ_PLL_UNLOCK                   (5)
#define ICU_PRI_FIQ_USB_PLUG_INOUT               (4)
#define ICU_PRI_FIQ_BT_WDT                       (1)
#define ICU_PRI_FIQ_DSP_WDT                      (0)
#define ICU_PRI_FIQ_RTC                          (2)
#define ICU_PRI_FIQ_TOUCH                        (3)
#define ICU_PRI_FIQ_CEC                          (6)
#define ICU_PRI_FIQ_JPEG                         (7)

#ifdef __cplusplus
}
#endif


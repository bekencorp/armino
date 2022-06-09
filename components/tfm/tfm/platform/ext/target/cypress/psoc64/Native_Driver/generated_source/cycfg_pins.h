/*******************************************************************************
* File Name: cycfg_pins.h
*
* Description:
* Pin configuration
* This file was automatically generated and should not be modified.
* Tools Package 2.2.0.2801
* latest-v2.X 2.0.0.6211
* personalities 3.0.0.0
* udd 3.0.0.562
*
********************************************************************************
* Copyright 2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
********************************************************************************/

#if !defined(CYCFG_PINS_H)
#define CYCFG_PINS_H

#include "cycfg_notices.h"
#include "cy_gpio.h"
#if defined (CY_USING_HAL)
	#include "cyhal_hwmgr.h"
#endif //defined (CY_USING_HAL)
#include "cycfg_routing.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CYBSP_WCO_IN_ENABLED 1U
#define CYBSP_WCO_IN_PORT GPIO_PRT0
#define CYBSP_WCO_IN_PORT_NUM 0U
#define CYBSP_WCO_IN_PIN 0U
#define CYBSP_WCO_IN_NUM 0U
#define CYBSP_WCO_IN_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_WCO_IN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_0_HSIOM
	#define ioss_0_port_0_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WCO_IN_HSIOM ioss_0_port_0_pin_0_HSIOM
#define CYBSP_WCO_IN_IRQ ioss_interrupts_gpio_0_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_IN_HAL_PORT_PIN P0_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_IN P0_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_IN_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_IN_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_IN_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_WCO_OUT_ENABLED 1U
#define CYBSP_WCO_OUT_PORT GPIO_PRT0
#define CYBSP_WCO_OUT_PORT_NUM 0U
#define CYBSP_WCO_OUT_PIN 1U
#define CYBSP_WCO_OUT_NUM 1U
#define CYBSP_WCO_OUT_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_WCO_OUT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_1_HSIOM
	#define ioss_0_port_0_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WCO_OUT_HSIOM ioss_0_port_0_pin_1_HSIOM
#define CYBSP_WCO_OUT_IRQ ioss_interrupts_gpio_0_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_OUT_HAL_PORT_PIN P0_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_OUT P0_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_OUT_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_OUT_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_OUT_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SW2 (P0_4)
	#define CYBSP_USER_BTN1 CYBSP_SW2
	#define CYBSP_USER_BTN CYBSP_SW2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_LED_RGB_GREEN (P0_5)
	#define CYBSP_USER_LED4 CYBSP_LED_RGB_GREEN
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_A0 (P10_0)
	#define CYBSP_J2_1 CYBSP_A0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_A1 (P10_1)
	#define CYBSP_J2_3 CYBSP_A1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_A2 (P10_2)
	#define CYBSP_J2_5 CYBSP_A2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_A3 (P10_3)
	#define CYBSP_J2_7 CYBSP_A3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_A4 (P10_4)
	#define CYBSP_J2_9 CYBSP_A4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_A5 (P10_5)
	#define CYBSP_J2_11 CYBSP_A5
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_J2_13 (P10_6)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_J2_15 (P10_7)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_LED9 (P11_1)
	#define CYBSP_USER_LED2 CYBSP_LED9
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SS (P11_2)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D3 (P11_3)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D2 (P11_4)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D1 (P11_5)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D0 (P11_6)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SCK (P11_7)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SPI_MOSI (P12_0)
	#define CYBSP_D11 CYBSP_SPI_MOSI
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SPI_MISO (P12_1)
	#define CYBSP_D12 CYBSP_SPI_MISO
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SPI_CLK (P12_2)
	#define CYBSP_D13 CYBSP_SPI_CLK
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SPI_CS (P12_3)
	#define CYBSP_D10 CYBSP_SPI_CS
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SDHC_CMD (P12_4)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SDHC_CLK (P12_5)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SDHC_IO0 (P13_0)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SDHC_IO1 (P13_1)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SDHC_IO2 (P13_2)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SDHC_IO3 (P13_3)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SDHC_DETECT (P13_7)
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_RX_ENABLED 1U
#define CYBSP_CSD_RX_PORT GPIO_PRT1
#define CYBSP_CSD_RX_PORT_NUM 1U
#define CYBSP_CSD_RX_PIN 0U
#define CYBSP_CSD_RX_NUM 0U
#define CYBSP_CSD_RX_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_0_HSIOM
	#define ioss_0_port_1_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_RX_HSIOM ioss_0_port_1_pin_0_HSIOM
#define CYBSP_CSD_RX_IRQ ioss_interrupts_gpio_1_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_RX_HAL_PORT_PIN P1_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_RX P1_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_RX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_RX_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_RX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_LED_RGB_RED (P1_1)
	#define CYBSP_USER_LED3 CYBSP_LED_RGB_RED
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SW4 (P1_4)
	#define CYBSP_USER_BTN2 CYBSP_SW4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_LED8 (P1_5)
	#define CYBSP_USER_LED1 CYBSP_LED8
	#define CYBSP_USER_LED CYBSP_LED8
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_SDIO_D0 (P2_0)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_SDIO_D1 (P2_1)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_SDIO_D2 (P2_2)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_SDIO_D3 (P2_3)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_SDIO_CMD (P2_4)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_SDIO_CLK (P2_5)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_WL_REG_ON (P2_6)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_RX (P3_0)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_TX (P3_1)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_RTS (P3_2)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_CTS (P3_3)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_POWER (P3_4)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_DEVICE_WAKE (P3_5)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_HOST_WAKE (P4_0)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_HOST_WAKE (P4_1)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_DEBUG_UART_RX (P5_0)
	#define CYBSP_D0 CYBSP_DEBUG_UART_RX
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_DEBUG_UART_TX (P5_1)
	#define CYBSP_D1 CYBSP_DEBUG_UART_TX
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_DEBUG_UART_RTS (P5_2)
	#define CYBSP_D2 CYBSP_DEBUG_UART_RTS
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_DEBUG_UART_CTS (P5_3)
	#define CYBSP_D3 CYBSP_DEBUG_UART_CTS
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_D4 (P5_4)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_D5 (P5_5)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_D6 (P5_6)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_D7 (P5_7)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_I2C_SCL (P6_0)
	#define CYBSP_D15 CYBSP_I2C_SCL
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_I2C_SDA (P6_1)
	#define CYBSP_D14 CYBSP_I2C_SDA
#endif //defined (CY_USING_HAL)
#define CYBSP_SWO_ENABLED 1U
#define CYBSP_SWO_PORT GPIO_PRT6
#define CYBSP_SWO_PORT_NUM 6U
#define CYBSP_SWO_PIN 4U
#define CYBSP_SWO_NUM 4U
#define CYBSP_SWO_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_SWO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_4_HSIOM
	#define ioss_0_port_6_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWO_HSIOM ioss_0_port_6_pin_4_HSIOM
#define CYBSP_SWO_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_SWO_HAL_PORT_PIN P6_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWO P6_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWO_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWO_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWO_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_SWDIO_ENABLED 1U
#define CYBSP_SWDIO_PORT GPIO_PRT6
#define CYBSP_SWDIO_PORT_NUM 6U
#define CYBSP_SWDIO_PIN 6U
#define CYBSP_SWDIO_NUM 6U
#define CYBSP_SWDIO_DRIVEMODE CY_GPIO_DM_PULLUP
#define CYBSP_SWDIO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_6_HSIOM
	#define ioss_0_port_6_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWDIO_HSIOM ioss_0_port_6_pin_6_HSIOM
#define CYBSP_SWDIO_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_SWDIO_HAL_PORT_PIN P6_6
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDIO P6_6
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDIO_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDIO_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDIO_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_PULLUP
#endif //defined (CY_USING_HAL)
#define CYBSP_SWDCK_ENABLED 1U
#define CYBSP_SWDCK_PORT GPIO_PRT6
#define CYBSP_SWDCK_PORT_NUM 6U
#define CYBSP_SWDCK_PIN 7U
#define CYBSP_SWDCK_NUM 7U
#define CYBSP_SWDCK_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define CYBSP_SWDCK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_7_HSIOM
	#define ioss_0_port_6_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWDCK_HSIOM ioss_0_port_6_pin_7_HSIOM
#define CYBSP_SWDCK_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_SWDCK_HAL_PORT_PIN P6_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDCK P6_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDCK_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDCK_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDCK_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_PULLDOWN
#endif //defined (CY_USING_HAL)
#define CYBSP_CINA_ENABLED 1U
#define CYBSP_CINA_PORT GPIO_PRT7
#define CYBSP_CINA_PORT_NUM 7U
#define CYBSP_CINA_PIN 1U
#define CYBSP_CINA_NUM 1U
#define CYBSP_CINA_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CINA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_1_HSIOM
	#define ioss_0_port_7_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CINA_HSIOM ioss_0_port_7_pin_1_HSIOM
#define CYBSP_CINA_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CINA_HAL_PORT_PIN P7_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINA P7_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINA_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINA_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINA_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CINB_ENABLED 1U
#define CYBSP_CINB_PORT GPIO_PRT7
#define CYBSP_CINB_PORT_NUM 7U
#define CYBSP_CINB_PIN 2U
#define CYBSP_CINB_NUM 2U
#define CYBSP_CINB_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CINB_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_2_HSIOM
	#define ioss_0_port_7_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CINB_HSIOM ioss_0_port_7_pin_2_HSIOM
#define CYBSP_CINB_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CINB_HAL_PORT_PIN P7_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINB P7_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINB_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINB_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINB_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_LED_RGB_BLUE (P7_3)
	#define CYBSP_USER_LED5 CYBSP_LED_RGB_BLUE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_D8 (P7_5)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_D9 (P7_6)
#endif //defined (CY_USING_HAL)
#define CYBSP_CMOD_ENABLED 1U
#define CYBSP_CMOD_PORT GPIO_PRT7
#define CYBSP_CMOD_PORT_NUM 7U
#define CYBSP_CMOD_PIN 7U
#define CYBSP_CMOD_NUM 7U
#define CYBSP_CMOD_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CMOD_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_7_HSIOM
	#define ioss_0_port_7_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CMOD_HSIOM ioss_0_port_7_pin_7_HSIOM
#define CYBSP_CMOD_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CMOD_HAL_PORT_PIN P7_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CMOD P7_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CMOD_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CMOD_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CMOD_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_BTN0_ENABLED 1U
#define CYBSP_CSD_BTN0_PORT GPIO_PRT8
#define CYBSP_CSD_BTN0_PORT_NUM 8U
#define CYBSP_CSD_BTN0_PIN 1U
#define CYBSP_CSD_BTN0_NUM 1U
#define CYBSP_CSD_BTN0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_BTN0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_1_HSIOM
	#define ioss_0_port_8_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_BTN0_HSIOM ioss_0_port_8_pin_1_HSIOM
#define CYBSP_CSD_BTN0_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN0_HAL_PORT_PIN P8_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN0 P8_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN0_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_BTN1_ENABLED 1U
#define CYBSP_CSD_BTN1_PORT GPIO_PRT8
#define CYBSP_CSD_BTN1_PORT_NUM 8U
#define CYBSP_CSD_BTN1_PIN 2U
#define CYBSP_CSD_BTN1_NUM 2U
#define CYBSP_CSD_BTN1_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_BTN1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_2_HSIOM
	#define ioss_0_port_8_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_BTN1_HSIOM ioss_0_port_8_pin_2_HSIOM
#define CYBSP_CSD_BTN1_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN1_HAL_PORT_PIN P8_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN1 P8_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN1_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_SLD0_ENABLED 1U
#define CYBSP_CSD_SLD0_PORT GPIO_PRT8
#define CYBSP_CSD_SLD0_PORT_NUM 8U
#define CYBSP_CSD_SLD0_PIN 3U
#define CYBSP_CSD_SLD0_NUM 3U
#define CYBSP_CSD_SLD0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_3_HSIOM
	#define ioss_0_port_8_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD0_HSIOM ioss_0_port_8_pin_3_HSIOM
#define CYBSP_CSD_SLD0_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD0_HAL_PORT_PIN P8_3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD0 P8_3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD0_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_SLD1_ENABLED 1U
#define CYBSP_CSD_SLD1_PORT GPIO_PRT8
#define CYBSP_CSD_SLD1_PORT_NUM 8U
#define CYBSP_CSD_SLD1_PIN 4U
#define CYBSP_CSD_SLD1_NUM 4U
#define CYBSP_CSD_SLD1_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_4_HSIOM
	#define ioss_0_port_8_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD1_HSIOM ioss_0_port_8_pin_4_HSIOM
#define CYBSP_CSD_SLD1_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD1_HAL_PORT_PIN P8_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD1 P8_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD1_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_SLD2_ENABLED 1U
#define CYBSP_CSD_SLD2_PORT GPIO_PRT8
#define CYBSP_CSD_SLD2_PORT_NUM 8U
#define CYBSP_CSD_SLD2_PIN 5U
#define CYBSP_CSD_SLD2_NUM 5U
#define CYBSP_CSD_SLD2_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_5_HSIOM
	#define ioss_0_port_8_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD2_HSIOM ioss_0_port_8_pin_5_HSIOM
#define CYBSP_CSD_SLD2_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD2_HAL_PORT_PIN P8_5
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD2 P8_5
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD2_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD2_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD2_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_SLD3_ENABLED 1U
#define CYBSP_CSD_SLD3_PORT GPIO_PRT8
#define CYBSP_CSD_SLD3_PORT_NUM 8U
#define CYBSP_CSD_SLD3_PIN 6U
#define CYBSP_CSD_SLD3_NUM 6U
#define CYBSP_CSD_SLD3_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_6_HSIOM
	#define ioss_0_port_8_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD3_HSIOM ioss_0_port_8_pin_6_HSIOM
#define CYBSP_CSD_SLD3_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD3_HAL_PORT_PIN P8_6
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD3 P8_6
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD3_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD3_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD3_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_SLD4_ENABLED 1U
#define CYBSP_CSD_SLD4_PORT GPIO_PRT8
#define CYBSP_CSD_SLD4_PORT_NUM 8U
#define CYBSP_CSD_SLD4_PIN 7U
#define CYBSP_CSD_SLD4_NUM 7U
#define CYBSP_CSD_SLD4_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_7_HSIOM
	#define ioss_0_port_8_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD4_HSIOM ioss_0_port_8_pin_7_HSIOM
#define CYBSP_CSD_SLD4_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD4_HAL_PORT_PIN P8_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD4 P8_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD4_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD4_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD4_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_J2_2 (P9_0)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_J2_4 (P9_1)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_J2_6 (P9_2)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_J2_8 (P9_3)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_J2_10 (P9_4)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_J2_12 (P9_5)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_J2_14 (P9_6)
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_J2_16 (P9_7)
#endif //defined (CY_USING_HAL)

extern const cy_stc_gpio_pin_config_t CYBSP_WCO_IN_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WCO_IN_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WCO_OUT_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WCO_OUT_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_RX_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_RX_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_SWO_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_SWO_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_SWDIO_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_SWDIO_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_SWDCK_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_SWDCK_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CINA_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CINA_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CINB_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CINB_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CMOD_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CMOD_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_BTN0_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_BTN0_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_BTN1_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_BTN1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD0_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_SLD0_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD1_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_SLD1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD2_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_SLD2_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD3_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_SLD3_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD4_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_SLD4_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */

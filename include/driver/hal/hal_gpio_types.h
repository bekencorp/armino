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

#include <common/bk_err.h>

/**
 * @brief GPIO defines
 * @defgroup bk_api_gpio_defs macos
 * @ingroup bk_api_gpio
 * @{
 */

/**
 * @brief default GPIO pins number
 */
typedef enum {
	GPIO_0 = 0,
	GPIO_1,
	GPIO_2,
	GPIO_3,
	GPIO_4,
	GPIO_5,
	GPIO_6,
	GPIO_7,
	GPIO_8,
	GPIO_9,
	GPIO_10,
	GPIO_11,
	GPIO_12,
	GPIO_13,
	GPIO_14,
	GPIO_15,
	GPIO_16,
	GPIO_17,
	GPIO_18,
	GPIO_19,
	GPIO_20,
	GPIO_21,
	GPIO_22,
	GPIO_23,
	GPIO_24,
	GPIO_25,
	GPIO_26,
	GPIO_27,
	GPIO_28,
	GPIO_29,
	GPIO_30,
	GPIO_31,
	GPIO_32,
	GPIO_33,
	GPIO_34,
	GPIO_35,
	GPIO_36,
	GPIO_37,
	GPIO_38,
	GPIO_39,
	GPIO_40,
	GPIO_41,
	GPIO_42,
	GPIO_43,
	GPIO_44,
	GPIO_45,
	GPIO_46,
	GPIO_47,
	GPIO_NUM,
} gpio_id_t;


/**
 * @brief GPIO input/output enable
 */
typedef enum {
	GPIO_IO_DISABLE = 0,		/**<disable gpio output and input mode which is high impendence state */
	GPIO_OUTPUT_ENABLE = 1,		/**<set gpio output mode */
	GPIO_INPUT_ENABLE = 2,		/**<set gpio input mode */
	GPIO_IO_INVALID = 3,		/**<gpio invalid mode */
} gpio_io_mode_t;


/**
 * @brief GPIO pull up/down set
 */
typedef enum {
	GPIO_PULL_DISABLE = 0,		/**<disbale gpio pull mode */
	GPIO_PULL_DOWN_EN = 1,		/**<set gpio as pull down mode */
	GPIO_PULL_UP_EN = 2,		/**<set gpio as pull up mode */
	GPIO_PULL_INVALID = 3,		/**<gpio invalid pull mode */
} gpio_pull_mode_t;

/**
 * @brief GPIO second function disable/enable
 */
typedef enum {
	GPIO_SECOND_FUNC_DISABLE = 0, /**< disbale gpio second function */
	GPIO_SECOND_FUNC_ENABLE,      /**< enbale gpio second function */
} gpio_func_mode_t;

/**
 * @brief GPIO mode config
 */
typedef struct {
	gpio_io_mode_t io_mode;		/**< set gpio output or input mode */
	gpio_pull_mode_t pull_mode;	/**< set gpio pull mode */
	gpio_func_mode_t func_mode; /**< set gpio func mode */
} gpio_config_t;

/**
 * @brief GPIO intterrupt type
 */
typedef enum {
	GPIO_INT_TYPE_LOW_LEVEL = 0,		/**<set gpio as low level intterrupt type */
	GPIO_INT_TYPE_HIGH_LEVEL,		/**<set gpio as high level intterrupt type */
	GPIO_INT_TYPE_RISING_EDGE,		/**<set gpio as rising edge intterrupt type */
	GPIO_INT_TYPE_FALLING_EDGE,		/**<set gpio as falling edge intterrupt type */
	GPIO_INT_TYPE_MAX,			/**< Invalid intterrupt type mode */
} gpio_int_type_t;


/**
 * @brief GPIOs device number
 */
typedef enum {
	GPIO_DEV_NONE = 0, /**< The GPIO doesn't map to any device */
	GPIO_DEV_PWM0= 0x01,
	GPIO_DEV_PWM1,
	GPIO_DEV_PWM2,
	GPIO_DEV_PWM3,
	GPIO_DEV_PWM4,
	GPIO_DEV_PWM5,
	GPIO_DEV_PWM6,
	GPIO_DEV_PWM7,
	GPIO_DEV_PWM8,
	GPIO_DEV_PWM9,
	GPIO_DEV_PWM10,
	GPIO_DEV_PWM11,

	GPIO_DEV_ADC0 = 0x0F,
	GPIO_DEV_ADC1,
	GPIO_DEV_ADC2,
	GPIO_DEV_ADC3,
	GPIO_DEV_ADC4,
	GPIO_DEV_ADC5,
	GPIO_DEV_ADC6,
	GPIO_DEV_ADC7,
	GPIO_DEV_ADC8,
	GPIO_DEV_ADC9,
	GPIO_DEV_ADC10,
	GPIO_DEV_ADC11,
	GPIO_DEV_ADC12,
	GPIO_DEV_ADC13,

	GPIO_DEV_UART1_TXD= 0x1E,
	GPIO_DEV_UART1_RXD,
	GPIO_DEV_UART2_TXD,
	GPIO_DEV_UART2_RXD,
	GPIO_DEV_UART3_TXD,
	GPIO_DEV_UART3_RXD,

	GPIO_DEV_I2C0_SCL = 0x24,
	GPIO_DEV_I2C0_SDA,
	GPIO_DEV_I2C1_SCL,
	GPIO_DEV_I2C1_SDA,
	GPIO_DEV_I2C2_SCL,
	GPIO_DEV_I2C2_SDA,


	GPIO_DEV_SPI0_SCK = 0x2A,
	GPIO_DEV_SPI0_CSN,
	GPIO_DEV_SPI0_MOSI,
	GPIO_DEV_SPI0_MISO,
	GPIO_DEV_SPI1_SCK,
	GPIO_DEV_SPI1_CSN,
	GPIO_DEV_SPI1_MOSI,
	GPIO_DEV_SPI1_MISO,
	GPIO_DEV_SPI2_SCK,
	GPIO_DEV_SPI2_CSN,
	GPIO_DEV_SPI2_MOSI,
	GPIO_DEV_SPI2_MISO,

	GPIO_DEV_I2S1_CLK = 0x37,
	GPIO_DEV_I2S1_SYNC,
	GPIO_DEV_I2S1_DIN,
	GPIO_DEV_I2S1_DOUT,
	GPIO_DEV_I2S2_CLK,
	GPIO_DEV_I2S2_SYNC,
	GPIO_DEV_I2S2_DIN,
	GPIO_DEV_I2S2_DOUT,
	GPIO_DEV_I2S3_CLK,
	GPIO_DEV_I2S3_SYNC,
	GPIO_DEV_I2S3_DIN,
	GPIO_DEV_I2S3_DOUT,

	GPIO_DEV_JTAG_TCK,
	GPIO_DEV_JTAG_TMS,
	GPIO_DEV_JTAG_TDI,
	GPIO_DEV_JTAG_TDO,

	GPIO_DEV_SDIO_HOST_CLK = 0x48,
	GPIO_DEV_SDIO_HOST_CMD,
	GPIO_DEV_SDIO_HOST_DATA0,
	GPIO_DEV_SDIO_HOST_DATA1,
	GPIO_DEV_SDIO_HOST_DATA2,
	GPIO_DEV_SDIO_HOST_DATA3,

	GPIO_DEV_USB0_DP,
	GPIO_DEV_USB0_DN,

	GPIO_DEV_USB1_DP = 0x50,
	GPIO_DEV_USB1_DN,

	GPIO_DEV_UART1_CTS,
	GPIO_DEV_UART1_RTS,

	GPIO_DEV_JPEG_MCLK= 0x54,
	GPIO_DEV_JPEG_PCLK,
	GPIO_DEV_JPEG_HSYNC,
	GPIO_DEV_JPEG_VSYNC,
	GPIO_DEV_JPEG_PXDATA0,
	GPIO_DEV_JPEG_PXDATA1,
	GPIO_DEV_JPEG_PXDATA2,
	GPIO_DEV_JPEG_PXDATA3,
	GPIO_DEV_JPEG_PXDATA4,
	GPIO_DEV_JPEG_PXDATA5,
	GPIO_DEV_JPEG_PXDATA6,
	GPIO_DEV_JPEG_PXDATA7,

	GPIO_DEV_QSPI_RAM_CLK = 0x60,
	GPIO_DEV_QSPI_RAM_CSN,
	GPIO_DEV_QSPI_FLASH_CLK,
	GPIO_DEV_QSPI_FLASH_CSN,
	GPIO_DEV_QSPI_IO0,
	GPIO_DEV_QSPI_IO1,
	GPIO_DEV_QSPI_IO2,
	GPIO_DEV_QSPI_IO3,

	GPIO_DEV_IRDA,
	GPIO_DEV_CLK13M,
	GPIO_DEV_CLK26M,
	GPIO_DEV_LPO_CLK,
	GPIO_DEV_WIFI_ACTIVE,
	GPIO_DEV_BT_ACTIVE,
	GPIO_DEV_BT_PRIORITY,

	GPIO_DEV_TXEN,
	GPIO_DEV_RXEN = 0x70,

	GPIO_DEV_PGA_INP,
	GPIO_DEV_PGA_INN,

	GPIO_DEV_SPDIF1,
	GPIO_DEV_SPDIF2,
	GPIO_DEV_SPDIF3,
	GPIO_DEV_HDMI_CEC,

	GPIO_DEV_I2S1_DOUT2,
	GPIO_DEV_I2S1_DOUT3,
	GPIO_DEV_I2S1_MCLK,

	GPIO_DEV_DMIC1_CLK,
	GPIO_DEV_DMIC1_DAT,
	GPIO_DEV_DMIC2_CLK,
	GPIO_DEV_DMIC2_DAT,

	GPIO_DEV_DIG_CLKOUT1,
	GPIO_DEV_DIG_CLKOUT2,

	GPIO_DEV_TOUCH0 = 0x80,
	GPIO_DEV_TOUCH1,
	GPIO_DEV_TOUCH2,
	GPIO_DEV_TOUCH3,
	GPIO_DEV_TOUCH4,
	GPIO_DEV_TOUCH5,
	GPIO_DEV_TOUCH6,
	GPIO_DEV_TOUCH7,
	GPIO_DEV_TOUCH8,
	GPIO_DEV_TOUCH9,
	GPIO_DEV_TOUCH10,
	GPIO_DEV_TOUCH11,
	GPIO_DEV_TOUCH12,
	GPIO_DEV_TOUCH13,
	GPIO_DEV_TOUCH14,
	GPIO_DEV_TOUCH15,

	GPIO_DEV_LCD_RGB0 = 0x90,
	GPIO_DEV_LCD_RGB1,
	GPIO_DEV_LCD_RGB2,
	GPIO_DEV_LCD_RGB3,
	GPIO_DEV_LCD_RGB4,
	GPIO_DEV_LCD_RGB5,
	GPIO_DEV_LCD_RGB6,
	GPIO_DEV_LCD_RGB7,
	GPIO_DEV_LCD_RGB8,
	GPIO_DEV_LCD_RGB9,
	GPIO_DEV_LCD_RGB10,
	GPIO_DEV_LCD_RGB11,
	GPIO_DEV_LCD_RGB12,
	GPIO_DEV_LCD_RGB13,
	GPIO_DEV_LCD_RGB14,
	GPIO_DEV_LCD_RGB15,
	GPIO_DEV_LCD_RGB16,
	GPIO_DEV_LCD_RGB17,
	GPIO_DEV_LCD_RGB18,
	GPIO_DEV_LCD_RGB19,
	GPIO_DEV_LCD_RGB20 = 0xA4,

	GPIO_DEV_QSPI_CLK = 0xA5,
	GPIO_DEV_QSPI_CSN,
	GPIO_DEV_BT_ANT0 = 0xA7,
	GPIO_DEV_BT_ANT1,
	GPIO_DEV_BT_ANT2,
	GPIO_DEV_BT_ANT3,
	GPIO_DEV_CAN_TX = 0xAB,
	GPIO_DEV_CAN_RX,
	GPIO_DEV_CAN_STANDBY,
	GPIO_DEV_CLK_AUXS = 0xAE,

	GPIO_DEV_DEBUG0 = 0xB0,
	GPIO_DEV_DEBUG1,
	GPIO_DEV_DEBUG2,
	GPIO_DEV_DEBUG3,
	GPIO_DEV_DEBUG4,
	GPIO_DEV_DEBUG5,
	GPIO_DEV_DEBUG6,
	GPIO_DEV_DEBUG7,
	GPIO_DEV_DEBUG8,
	GPIO_DEV_DEBUG9,
	GPIO_DEV_DEBUG10,
	GPIO_DEV_DEBUG11,
	GPIO_DEV_DEBUG12,
	GPIO_DEV_DEBUG13,
	GPIO_DEV_DEBUG14,
	GPIO_DEV_DEBUG15,
	GPIO_DEV_DEBUG16 = 0xC0,
	GPIO_DEV_DEBUG17,
	GPIO_DEV_DEBUG18,
	GPIO_DEV_DEBUG19,
	GPIO_DEV_DEBUG20,
	GPIO_DEV_DEBUG21,
	GPIO_DEV_DEBUG22,
	GPIO_DEV_DEBUG23,
	GPIO_DEV_DEBUG24,
	GPIO_DEV_DEBUG25,
	GPIO_DEV_DEBUG26,
	GPIO_DEV_DEBUG27,
	GPIO_DEV_DEBUG28,
	GPIO_DEV_DEBUG29,
	GPIO_DEV_DEBUG30,
	GPIO_DEV_DEBUG31,

	GPIO_DEV_INVALID = 0xFF,
} gpio_dev_t;


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

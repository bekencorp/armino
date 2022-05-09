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

typedef volatile struct {
	//reg00
	union {
		struct {
			uint32_t uart1: 1; /**< bit[0], UART1 clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t uart2: 1; /**< bit[1], UART2 clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t uart3:  1; /**< bit[2], UART3 clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t irda:  1; /**< bit[3], IRDA clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t fm_i2c:  1; /**< bit[4], I2C2 clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t i2c1: 1; /**< bit[5], I2C1 clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t i2c2: 1; /**< bit[6], I2C2 clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t spi1:   1; /**< bit[7], SPI1 clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t spi2:  1; /**< bit[8], SPI2 clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t spi3:  1; /**< bit[9], SPI3 clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t pwms: 1; /**< bit[10], PWMs clock select Audio PLL, if saradc is 0 */
			uint32_t sdio: 1; /**< bit[11], SDIO host clock select Audio PLL, if saradc is 0 */
			uint32_t efuse: 1; /**< bit[12], EFUSE host clock select Audio PLL, if saradc is 0 */
			uint32_t cec: 1; /**< bit[13], CEC clock select Audio PLL, if saradc is 0 */

			uint32_t reserved_1: 2;   /**< bit[14:15] */

			uint32_t qspi: 2;  /**< bit[16:17] QSPI clock division, 0: 1-div, 1: 2-div, 2: 4-div, 3: 8-div */
			uint32_t dco_clk_div: 2;  /**< bit[18:19] DCO clock division, 0: 1-div, 1: 2-div, 2: 4-div, 3: 8-div */
			uint32_t clk26m_div_factor: 2;  /**< bit[20:21] 26M clock division, 0: 1-div, 1: 2-div, 2: 4-div, 3: 8-div */
			uint32_t reserved_2: 2;  /**< bit[22:23] */
			uint32_t i2s_mclk_div: 3;  /**< bit[24:26], I2S MCLK division */
			uint32_t reserved_3: 5;  /**< bit[27:31] */
		};
		uint32_t v;
	} peri_clk_mux;

	//reg01 Clock select of pwms
	union {
		struct {
			uint32_t pwm_group0: 1; /**< bit[0], PWM group0 clock select, 1: LPO_CLK, 0: use clk_mux_pwms */
			uint32_t pwm_group1: 1; /**< bit[1], PWM group1 clock select, 1: LPO_CLK, 0: use clk_mux_pwms */
			uint32_t pwm_group2: 1; /**< bit[2], PWM group2 clock select, 1: LPO_CLK, 0: use clk_mux_pwms */
			uint32_t pwm_group3: 1; /**< bit[3], PWM group3 clock select, 1: LPO_CLK, 0: use clk_mux_pwms */
			uint32_t reserved_1: 12; /**< bit[4:15] */
			uint32_t saradc: 2; /**< bit[16:17], SarADC clock select, 1: LPO_CLK, 0: use clk_mux_pwms */
			uint32_t reserved_2: 14; /**< bit[18:31] */
		};
		uint32_t v;
	} pwm_group_saradc_clk_mux;

	//reg02 clock power down for periphral unit
	union {
		struct {
			uint32_t uart1: 1;           /**< bit[0], UART1 */
			uint32_t uart2: 1;           /**< bit[1], UART2 */
			uint32_t uart3: 1;           /**< bit[2], UART3 */
			uint32_t fm_i2c:  1;         /**< bit[3], FM_I2C1 */
			uint32_t i2c1:  1;           /**< bit[4], I2C1*/
			uint32_t i2c2:  1;           /**< bit[5], I2C2*/
			uint32_t sdio_host:  1;      /**< bit[6], SDIO host*/
			uint32_t reserved_1:  1;     /**< bit[7] */
			uint32_t spi1:   1;          /**< bit[8], SPI1 */
			uint32_t spi2:   1;          /**< bit[9], SPI2 */
			uint32_t spi3:   1;          /**< bit[10], SPI3 */
			uint32_t irda:   1;          /**< bit[11], IRDA */
			uint32_t timers: 2;          /**< bit[12:13], timers*/
			uint32_t pwm_group0:1;       /**< bit[14], pwm group0[0/1/2]*/
			uint32_t pwm_group1:1;       /**< bit[15], pwm group1[3/4/5]*/
			uint32_t pwm_group2:1;       /**< bit[16], pwm group2[6/7/8]*/
			uint32_t pwm_group3:1;       /**< bit[17], pwm group3[9/10/11]*/
			uint32_t saradc: 1;          /**< bit[18], SARADC */
			uint32_t cec:  1;            /**< bit[19], CEC */
			uint32_t reserved_2:  12;     /**< bit[20:31] */
		};
		uint32_t v;
	} clk_pwr_down;

	//reg03 Func clk power down
	union {
		struct {
			uint32_t arm_watchdog: 1;       /**< bit[0] */
			uint32_t usb1:         1;       /**< bit[1] */
			uint32_t usb2:         1;       /**< bit[2] */
			uint32_t jpeg:         1;       /**< bit[3] */
			uint32_t reserved_1:   1;       /**< bit[4] */
			uint32_t security:     1;       /**< bit[5] */
			uint32_t sdio_dma:     1;       /**< bit[6] */
			uint32_t reserved_2:   25;      /**< bit[7:31] */
		};
		uint32_t v;
	} func_clk_pwr_down;

	//reg04 AHB/APB clock gating disable for periphral unit
	union {
		struct {
			uint32_t icu: 1;          /**< bit[0], ICU apb clock gate disable */
			uint32_t uart: 1;         /**< bit[1], UART apb clock gate disable */
			uint32_t i2c:  1;         /**< bit[2], I2C apb clock gate disable */
			uint32_t sdio_host:  1;   /**< bit[3], SDIO host clock gate disable */
			uint32_t reserved_1:  1;  /**< bit[4], reserved clock gate disable */
			uint32_t spi:  1;         /**< bit[5], SPI apb clock gate disable */
			uint32_t gpio:   1;       /**< bit[6], GPIO apb clock gate disable */
			uint32_t watchdog: 1;     /**< bit[7], watchdog apb clock gate disable */
			uint32_t timers:  1;      /**< bit[8], Timers apb clock gate disable */
			uint32_t pwms:  1;        /**< bit[9], PWMS apb clock gate disable */
			uint32_t saradc:  1;      /**< bit[10], SARADC apb clock gate disable */
			uint32_t cec:  1;         /**< bit[11], CEC apb clock gate disable */
			uint32_t reserved_2:  4;  /**< bit[12:15] */

			uint32_t modem:  1;       /**< bit[16], MODEM apb clock gate disable */
			uint32_t mac:  1;         /**< bit[17], MAC apb clock gate disable */
			uint32_t usb:  1;         /**< bit[18], USB apb clock gate disable */
			uint32_t flash:  1;       /**< bit[19], FLASH apb clock gate disable */
			uint32_t qspi:  1;        /**< bit[20], QSPI apb clock gate disable */
			uint32_t dcache:  1;      /**< bit[21], DCACHE apb clock gate disable */
			uint32_t ahb_bus_memory: 1; /**< bit[22], bus memory apb clock gate disable */
			uint32_t sys_ctrl:  1;    /**< bit[23], sys_ctrl apb clock gate disable */
			uint32_t sdio_dma:  1;    /**< bit[24], SDIO_DMA apb clock gate disable */
			uint32_t gdma:  1;        /**< bit[25], GDMA apb clock gate disable */
			uint32_t logic_analyze:  1;/**< bit[26], Logic Analyze apb clock gate disable */

			uint32_t reserved_3:  5;/**< bit[27:31] */
		};
		uint32_t v;
	} peri_clk_gate_disable;

	//reg05
	uint32_t jtag_sel;

	//reg06~reg15
	uint32_t reserved_1[16 - 5 - 1];

	//reg16 IRQ interrupt enable
	union {
		struct {
			uint32_t uart1: 1;  /**< bit[0] UART1 interrupt */
			uint32_t uart2: 1;  /**< bit[1] UART2 interrupt */
			uint32_t uart3: 1;  /**< bit[2] UART3 interrupt */
			uint32_t fm_i2c: 1; /**< bit[3] FM_I2C interrupt */
			uint32_t i2c1: 1;  /**< bit[4] I2C1 interrupt */
			uint32_t i2c2: 1;  /**< bit[5] I2C2 interrupt */
			uint32_t sdio: 1;  /**< bit[6] SDIO interrupt */
			uint32_t usb1: 1;  /**< bit[7] USB1 interrupt */

			uint32_t spi1: 1;  /**< bit[8] SPI1 interrupt */
			uint32_t spi2: 1;  /**< bit[9] SPI2 interrupt */
			uint32_t spi3: 1;  /**< bit[10] SPI3 interrupt */
			uint32_t gpio: 1;  /**< bit[11] GPIOinterrupt */
			uint32_t irda: 1;  /**< bit[12] IRDA interrupt */
			uint32_t timer: 1;  /**< bit[13] TIMER interrupt */
			uint32_t pwm1: 1;  /**< bit[14] PWM interrupt */
			uint32_t pwm2: 1;  /**< bit[15] PWM interrupt */

			uint32_t saradc: 1;  /**< bit[16] SARADC interrupt */
			uint32_t qspi: 1;  /**< bit[17] QSPI interrupt */
			uint32_t usb2: 1;  /**< bit[18] USB2 interrupt */
			uint32_t sdio_dma: 1;  /**< bit[19] SDIO DMA interrupt */
			uint32_t gdma: 1;  /**< bit[20] GDMA interrupt */
			uint32_t mbox1: 1;  /**< bit[21] Mail box1 interrupt */
			uint32_t mbox2: 1;  /**< bit[22] Mail box2 interrupt */
			uint32_t security: 1;  /**< bit[23] Security interrupt */

			uint32_t modem: 1;  /**< bit[24] Security interrupt */
			uint32_t mac_txrx_timer: 1;  /**< bit[25] Security interrupt */
			uint32_t mac_txrx_misc: 1;  /**< bit[26] Security interrupt */
			uint32_t mac_rx_trigger: 1;  /**< bit[27] Security interrupt */
			uint32_t mac_tx_trigger: 1;  /**< bit[28] Security interrupt */
			uint32_t mac_prot_trigger: 1;  /**< bit[29] Security interrupt */
			uint32_t mac_general: 1;  /**< bit[30] Security interrupt */
			uint32_t mac_wakeup: 1;  /**< bit[31] Security interrupt */
		};
		uint32_t v;
	} irq_en;

	//reg17
	union {
		struct {
			uint32_t pll_unlock: 1;  /**< bit[0] PLL unlock enable */
			uint32_t usb_plug:   1;  /**< bit[1] USB plug in/out enable */
			uint32_t bt_wdt:     1;  /**< bit[2] BT watchdog enable */
			uint32_t dsp_wdt:    1;  /**< bit[3] DSP watchdog enable */
			uint32_t rtc:        1;  /**< bit[4] RTC enable */
			uint32_t touch:      1;  /**< bit[5] TOUCH enable */
			uint32_t cec:        1;  /**< bit[6] CEC enable */
			uint32_t jpeg:       1;  /**< bit[7] JPEG enable */
			uint32_t reserved:   24; /**< bit[8:31] */
		};
		uint32_t v;
	} fiq_en;

	//reg18
	union {
		struct {
			uint32_t irq: 1; /**< bit[0] */
			uint32_t fiq: 1; /**< bit[1] */
			uint32_t reserved: 30; /**< bit[2:31] */
		};
		uint32_t v;
	} int_global_en;

	//reg19 IRQ interrupt raw status
	union {
		struct {
			uint32_t uart1: 1;  /**< bit[0] UART1 interrupt */
			uint32_t uart2: 1;  /**< bit[1] UART2 interrupt */
			uint32_t uart3: 1;  /**< bit[2] UART3 interrupt */
			uint32_t fm_i2c: 1; /**< bit[3] FM_I2C interrupt */
			uint32_t i2c1: 1;  /**< bit[4] I2C1 interrupt */
			uint32_t i2c2: 1;  /**< bit[5] I2C2 interrupt */
			uint32_t sdio: 1;  /**< bit[6] SDIO interrupt */
			uint32_t usb1: 1;  /**< bit[7] USB1 interrupt */

			uint32_t spi1: 1;  /**< bit[8] SPI1 interrupt */
			uint32_t spi2: 1;  /**< bit[9] SPI2 interrupt */
			uint32_t spi3: 1;  /**< bit[10] SPI3 interrupt */
			uint32_t gpio: 1;  /**< bit[11] SPI3 interrupt */
			uint32_t irda: 1;  /**< bit[12] IRDA interrupt */
			uint32_t timer: 1;  /**< bit[13] TIMER interrupt */
			uint32_t pwm1: 1;  /**< bit[14] PWM interrupt */
			uint32_t pwm2: 1;  /**< bit[15] PWM interrupt */

			uint32_t saradc: 1;  /**< bit[16] SARADC interrupt */
			uint32_t qspi: 1;  /**< bit[17] QSPI interrupt */
			uint32_t usb2: 1;  /**< bit[18] USB2 interrupt */
			uint32_t sdio_dma: 1;  /**< bit[19] SDIO DMA interrupt */
			uint32_t gdma: 1;  /**< bit[20] GDMA interrupt */
			uint32_t mbox1: 1;  /**< bit[21] Mail box1 interrupt */
			uint32_t mbox2: 1;  /**< bit[22] Mail box2 interrupt */
			uint32_t security: 1;  /**< bit[23] Security interrupt */

			uint32_t modem: 1;  /**< bit[24] Security interrupt */
			uint32_t mac_tx_rx_timer: 1;  /**< bit[25] Security interrupt */
			uint32_t mac_tx_rx_misc: 1;  /**< bit[26] Security interrupt */
			uint32_t mac_rx_trigger: 1;  /**< bit[27] Security interrupt */
			uint32_t mac_tx_trigger: 1;  /**< bit[28] Security interrupt */
			uint32_t mac_prot_trigger: 1;  /**< bit[29] Security interrupt */
			uint32_t mac_general: 1;  /**< bit[30] Security interrupt */
			uint32_t mac_wakeup: 1;  /**< bit[31] Security interrupt */
		};
		uint32_t v;
	} irq_raw_st;

	//reg20
	union {
		struct {
			uint32_t pll_unlock:   1;  /**< bit[0] PLL unlock enable */
			uint32_t usb:          1;  /**< bit[1] USB plug in/out enable */
			uint32_t bt_watchdog:  1;  /**< bit[2] BT watchdog enable */
			uint32_t dsp_watchdog: 1;  /**< bit[3] DSP watchdog enable */
			uint32_t rtc:          1;  /**< bit[4] RTC enable */
			uint32_t touch:        1;  /**< bit[5] TOUCH enable */
			uint32_t cec:          1;  /**< bit[6] CEC enable */
			uint32_t jpeg:         1;  /**< bit[7] JPEG enable */
			uint32_t reserved:     24; /**< bit[8:31] */
		};
		uint32_t v;
	} fiq_raw_st;

	//reg21 IRQ interrupt status
	union {
		struct {
			uint32_t uart1: 1;  /**< bit[0] UART1 interrupt */
			uint32_t uart2: 1;  /**< bit[1] UART2 interrupt */
			uint32_t uart3: 1;  /**< bit[2] UART3 interrupt */
			uint32_t fm_i2c: 1; /**< bit[3] FM_I2C interrupt */
			uint32_t i2c1: 1;  /**< bit[4] I2C1 interrupt */
			uint32_t i2c2: 1;  /**< bit[5] I2C2 interrupt */
			uint32_t sdio: 1;  /**< bit[6] SDIO interrupt */
			uint32_t usb1: 1;  /**< bit[7] USB1 interrupt */

			uint32_t spi1: 1;  /**< bit[8] SPI1 interrupt */
			uint32_t spi2: 1;  /**< bit[9] SPI2 interrupt */
			uint32_t spi3: 1;  /**< bit[10] SPI3 interrupt */
			uint32_t gpio: 1;  /**< bit[11] SPI3 interrupt */
			uint32_t irda: 1;  /**< bit[12] IRDA interrupt */
			uint32_t timer: 1;  /**< bit[13] TIMER interrupt */
			uint32_t pwm1: 1;  /**< bit[14] PWM interrupt */
			uint32_t pwm2: 1;  /**< bit[15] PWM interrupt */

			uint32_t saradc: 1;  /**< bit[16] SARADC interrupt */
			uint32_t qspi: 1;  /**< bit[17] QSPI interrupt */
			uint32_t usb2: 1;  /**< bit[18] USB2 interrupt */
			uint32_t sdio_dma: 1;  /**< bit[19] SDIO DMA interrupt */
			uint32_t gdma: 1;  /**< bit[20] GDMA interrupt */
			uint32_t mbox1: 1;  /**< bit[21] Mail box1 interrupt */
			uint32_t mbox2: 1;  /**< bit[22] Mail box2 interrupt */
			uint32_t security: 1;  /**< bit[23] Security interrupt */

			uint32_t modem: 1;  /**< bit[24] Security interrupt */
			uint32_t mac_tx_rx_timer: 1;  /**< bit[25] Security interrupt */
			uint32_t mac_tx_rx_misc: 1;  /**< bit[26] Security interrupt */
			uint32_t mac_rx_trigger: 1;  /**< bit[27] Security interrupt */
			uint32_t mac_tx_trigger: 1;  /**< bit[28] Security interrupt */
			uint32_t mac_prot_trigger: 1;  /**< bit[29] Security interrupt */
			uint32_t mac_general: 1;  /**< bit[30] Security interrupt */
			uint32_t mac_wakeup: 1;  /**< bit[31] Security interrupt */
		};
		uint32_t v;
	} irq_st;

	//reg22
	union {
		struct {
			uint32_t pll_unlock:   1;  /**< bit[0] PLL unlock enable */
			uint32_t usb:          1;  /**< bit[1] USB plug in/out enable */
			uint32_t bt_watchdog:  1;  /**< bit[2] BT watchdog enable */
			uint32_t dsp_watchdog: 1;  /**< bit[3] DSP watchdog enable */
			uint32_t rtc:          1;  /**< bit[4] RTC enable */
			uint32_t touch:        1;  /**< bit[5] TOUCH enable */
			uint32_t cec:          1;  /**< bit[6] CEC enable */
			uint32_t jpeg:         1;  /**< bit[7] jpeg enable */
			uint32_t reserved:     24; /**< bit[8:31] */
		};
		uint32_t v;
	} fiq_st;

	//reg23 ARM wakeup enable
	union {
		struct {
			uint32_t uart1: 1;   /**<bit[0], UART1 wakeup */
			uint32_t uart2: 1;   /**<bit[1], UART2 wakeup */
			uint32_t uart3: 1;   /**<bit[2], UART2 wakeup */
			uint32_t fm_i2c: 1;  /**<bit[3], FM_I2C wakeup */
			uint32_t i2c1: 1;    /**<bit[4], I2C1 wakeup */
			uint32_t i2c2: 1;    /**<bit[5], I2C2 */
			uint32_t sdio_host: 1; /**<bit[6], SDIO host wakeup */
			uint32_t usb1: 1; /**<bit[7], USB1 wakeup */

			uint32_t spi1: 1; /**<bit[8], GPIO wakeup */
			uint32_t spi2: 1; /**<bit[9], TIMER wakeup */
			uint32_t spi3: 1; /**<bit[10], PWM wakeup */
			uint32_t gpio: 1; /**<bit[11] GPIO wakeup */
			uint32_t irda: 1; /**<bit[12], IRDA wakeup */
			uint32_t timer: 1; /**<bit[13], TIMER wakeup */
			uint32_t pwm1: 1; /**<bit[14], PWM1 wakeup */
			uint32_t pwm2: 1; /**<bit[15], PWM2 wakeup */

			uint32_t saradc: 1; /**<bit[16], SARADC wakeup */
			uint32_t qspi: 1; /**<bit[17], QSPI wakeup */
			uint32_t usb2: 1; /**<bit[18], USB2 wakeup */
			uint32_t sdio_dma: 1; /**<bit[19], SDIO DMA wakeup */
			uint32_t gdma: 1; /**<bit[20], GDMA wakeup */
			uint32_t mbox1: 1; /**<bit[21], mailbox1 wakeup */
			uint32_t mbox2: 1; /**<bit[22], mailbox2 wakeup */
			uint32_t security: 1; /**<bit[23], Security wakeup */

			uint32_t modem: 1; /**<bit[24], modem wakeup */
			uint32_t mac_txrx_timer: 1; /**<bit[25], MAC tx&rx timer wakeup */
			uint32_t mac_txrx_misc: 1; /**<bit[26], MAC tx&rx misc wakeup */
			uint32_t mac_rx_trigger: 1; /**<bit[27], MAC rx trigger wakeup */
			uint32_t mac_tx_trigger: 1; /**<bit[28], MAC TX trigger wakeup */
			uint32_t mac_prot_trigger: 1; /**<bit[29], MAC prot trigger wakeup */
			uint32_t mac_general: 1; /**<bit[30], MAC general wakeup */
			uint32_t mac_wakeup: 1; /**<bit[31], MAC wakeup */
		};
		uint32_t v;
	} arm_wakeup_en;

	//Reg 0x18
	union {
		struct {
			uint32_t pll_unlock: 1;  /**< bit[0] */
			uint32_t usb_plug:   1;  /**< bit[1] */
			uint32_t bt_wdt:     1;  /**< bit[2] */
			uint32_t dsp_wdt:    1;  /**< bit[3] */
			uint32_t rtc:        1;  /**< bit[4] */
			uint32_t touch:      1;  /**< bit[5] */
			uint32_t cec:        1;  /**< bit[6] */
			uint32_t jpeg:       1;  /**< bit[7] */
			uint32_t reserved:   24; /**< bit[8:31] */
		};
		uint32_t v;
	} arm_wakeup_en2;

	uint32_t reserved_2[32 - 24 - 1];

	//reg 0x20
	uint32_t gpio_peri_mode_cfg[SOC_PWM_CHAN_NUM_PER_GROUP];

	/* REG_0x23 */
	union {
		struct {
			uint32_t uart2:  1; /**< bit[0] */
			uint32_t spi3:  1; /**< bit[1] */
			uint32_t sdio:  1; /**< bit[2] */
			uint32_t sdio_host: 1; /**< bit[3] */
			uint32_t pwm6:  2; /**< bit[4:5] */
			uint32_t pwm7:  2; /**< bit[6:7] */
			uint32_t pwm8:  2; /**< bit[8:9] */
			uint32_t pwm9:  2; /**< bit[10:11] */
			uint32_t spdif: 2; /**< bit[12:13] */
			uint32_t reserved: 18; /**< bit[14:31] */
		};
		uint32_t v;
	} gpio_map_cfg;

	/* REG_0x24 */
	union {
		struct {
			uint32_t gpio_debug_cfg: 16; /**< bit[0:15] */
			uint32_t reserved:       16; /**< bit[16:31] */
		};
		uint32_t v;
	} debug_cfg;

	/* REG_0x25 */
	union {
		struct {
			uint32_t mux: 3; /**< bit[0:2] */
			uint32_t reserved: 29; /**< bit[3:31] */
		};
		uint32_t v;
	} debug_mux;

	/* REG_0x26 */
	uint32_t debug_msg;

	/* REG_0x27 */
	uint32_t debug_report;

	/* REG_0x28 */
	union {
		struct {
			uint32_t en: 1; /**< bit[0] */
			uint32_t reserved: 15; /**< bit[1:15] */
			uint32_t st: 16; /**< bit[16:31] */
		};
		uint32_t v;
	} touch_ctrl;

	/* REG_0x29 */
	union {
		struct {
			uint32_t gpio_pcfg4: 24; /**< bit[0:23] */
			uint32_t reserved:   8;  /**< bit[24:31] */
		};
		uint32_t v;
	} gpio_peri_mode_cfg4;

	/* REG_0x2A */
	union {
		struct {
			uint32_t gpio_pcfg5: 16; /**< bit[0:15] */
			uint32_t reserved:   16; /**< bit[16:31] */
		};
		uint32_t v;
	} gpio_peri_mode_cfg5;
} icu_hw_t;

#ifdef __cplusplus
}
#endif

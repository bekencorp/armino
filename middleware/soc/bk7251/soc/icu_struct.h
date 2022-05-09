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
			uint32_t i2c1:  1; /**< bit[2], I2C1 clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t irda:  1; /**< bit[3], IRDA clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t i2c2:  1; /**< bit[4], I2C2 clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t saradc: 1; /**< bit[5], SARADC clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t spi:   1; /**< bit[6], SPI clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t pwms:  1; /**< bit[7], PWMS clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t sdio:  1; /**< bit[8], SDIO clock select, 1-CLK_26M, 0-DCO Divided clk_mux */
			uint32_t saradc_audio_pll: 1; /**< bit[9], SARADC clock select Audio PLL, if saradc is 0 */

			uint32_t reserved_1: 6;   /**< bit[15:10] */

			uint32_t qspi: 2;         /**< bit[16:17] QSPI clock select, 0-DCO, 1-26M, 2-120M (DPLL division), 3-reserved */
			uint32_t dco_clk_div: 2;  /**< bit[18:19] DCO clock division, 0: 1-div, 1: 2-div, 2: 4-div, 3: 8-div */
			uint32_t reserved_2: 12;  /**< bit[20:31] */
		};
		uint32_t v;
	} peri_clk_mux;

	//reg01 Clock select of pwms
	union {
		struct {
			uint32_t chan0: 1; /**< bit[0], PWM chan0 clock select, 1: LPO_CLK, 0: use clk_mux_pwms */
			uint32_t chan1: 1; /**< bit[1], PWM chan0 clock select, 1: LPO_CLK, 0: use clk_mux_pwms */
			uint32_t chan2: 1; /**< bit[2], PWM chan0 clock select, 1: LPO_CLK, 0: use clk_mux_pwms */
			uint32_t chan3: 1; /**< bit[3], PWM chan0 clock select, 1: LPO_CLK, 0: use clk_mux_pwms */
			uint32_t chan4: 1; /**< bit[4], PWM chan0 clock select, 1: LPO_CLK, 0: use clk_mux_pwms */
			uint32_t chan5: 1; /**< bit[5], PWM chan0 clock select, 1: LPO_CLK, 0: use clk_mux_pwms */
			uint32_t reserved: 26; /**< bit[6:21] */
		};
		uint32_t v;
	} pwm_chan_clk_mux;

	//reg02 clock power down for periphral unit
	union {
		struct {
			uint32_t uart1: 1;           /**< bit[0], UART1 */
			uint32_t uart2: 1;           /**< bit[1], UART2 */
			uint32_t i2c1:  1;           /**< bit[2], I2C1 */
			uint32_t irda:  1;           /**< bit[3], IRDA */
			uint32_t i2s_pcm:  1;        /**< bit[4], I2C2 */
			uint32_t i2c2:  1;           /**< bit[5], I2C2 */
			uint32_t spi:   1;           /**< bit[6], SPI */
			uint32_t saradc: 1;          /**< bit[7], SARADC */
			uint32_t arm_watchdog: 1;    /**< bit[8], ARM watchdog */
			uint32_t pwm0:  1;           /**< bit[9], PWM 0 */
			uint32_t pwm1:  1;           /**< bit[10], PWM 1 */
			uint32_t pwm2:  1;           /**< bit[11], PWM 2 */
			uint32_t pwm3:  1;           /**< bit[12], PWM 3 */
			uint32_t pwm4:  1;           /**< bit[13], PWM 4 */
			uint32_t pwm5:  1;           /**< bit[14], PWM 5 */
			uint32_t audio:  1;          /**< bit[15], AUDIO */
			uint32_t tl410_watchdog:  1; /**< bit[16], TL410 */
			uint32_t sdio:  1;           /**< bit[17], SDIO */
			uint32_t usb:  1;            /**< bit[18], USB */
			uint32_t fft:  1;            /**< bit[19], FFT */
			uint32_t timers:  2;         /**< bit[20:21], timers */
			uint32_t jpeg:  1;           /**< bit[22], SDIO */
			uint32_t qspi:  1;           /**< bit[23], SDIO */

			uint32_t reserved: 8; /**< bit[24:31] */
		};
		uint32_t v;
	} clk_pwr_down;

	//reg03 AHB/APB clock gating disable for periphral unit
	union {
		struct {
			uint32_t icu: 1;           /**< bit[0], ICU apb clock gate disable */
			uint32_t uart1: 1;         /**< bit[1], UART1 apb clock gate disable */
			uint32_t uart2: 1;         /**< bit[2], UART2 apb clock gate disable */
			uint32_t i2c1:  1;         /**< bit[3], I2C1 apb clock gate disable */
			uint32_t irda:  1;         /**< bit[4], IRDA apb clock gate disable */
			uint32_t i2s_pcm:  1;      /**< bit[5], I2S/PCM apb clock gate disable */
			uint32_t i2c2:  1;         /**< bit[6], I2C2 apb clock gate disable */
			uint32_t spi:   1;         /**< bit[7], SPI apb clock gate disable */
			uint32_t gpio:   1;        /**< bit[8], GPIO apb clock gate disable */
			uint32_t watchdog: 1;      /**< bit[9], watchdog apb clock gate disable */
			uint32_t pwms_timers:  1;  /**< bit[10], PWMS & Timers apb clock gate disable */
			uint32_t audio:  1;        /**< bit[11], AUDIO apb clock gate disable */
			uint32_t saradc:  1;       /**< bit[12], SARADC apb clock gate disable */
			uint32_t sdio:  1;         /**< bit[13], SDIO apb clock gate disable */
			uint32_t usb:  1;          /**< bit[14], USB apb clock gate disable */
			uint32_t fft:  1;          /**< bit[15], FFT apb clock gate disable */
			uint32_t mac:  1;          /**< bit[16], MAC apb clock gate disable */

			uint32_t reserved: 15;     /**< bit[17:31] */
		};
		uint32_t v;
	} peri_clk_gate_disable;

	//reg04
	union {
		struct {
			/**< Note: software write '1', and wait interrupt wakeup tl410,
				then hardware clear it automatically */
			uint32_t tl410: 1; /**< bit[0] TL410 clock power down */
			uint32_t ble: 1;   /**< bit[1] BLE clock power down */
			uint32_t reserved: 30;
		};
		uint32_t v;
	} clk_pwr_down_2;

	//reg05
	union {
		struct {
			uint32_t factor: 2; /**< bit[0:1] 26M clock division factor, 0: 1 division, 1: 2 division, 2: 4 division, 3: 8 division */
			uint32_t reserved: 30;
		};
		uint32_t v;
	} clk26m_div_factor;

	//reg06
	union {
		/**< write: 0x7111_e968 to select arm; 0x7111_e410 to select tl410
		         *   read:  0x0: arm is selected 0x1: tl410 is selected
		         */
		uint32_t v; /**< bit[0:31] JTAG select */
	} jtag_sel;

	//reg07~reg15
	uint32_t reserved[16 - 6 - 1];

	//reg16
	union {
		struct {

			//IRQ interrupt enable
			uint32_t irq_uart1: 1;  /**< bit[0] UART1 interrupt */
			uint32_t irq_uart2: 1;  /**< bit[1] UART2 interrupt */
			uint32_t irq_i2c1: 1;  /**< bit[2] I2C1 interrupt */
			uint32_t irq_irda: 1;  /**< bit[3] IRDA interrupt */
			uint32_t irq_i2s_pcm: 1;  /**< bit[4] I2S PCM interrupt */
			uint32_t irq_i2c2: 1;  /**< bit[5] I2C2 interrupt */
			uint32_t irq_spi: 1;  /**< bit[6] GPIO interrupt */
			uint32_t irq_gpio: 1;  /**< bit[7] GPIO interrupt */
			uint32_t irq_timer: 1;  /**< bit[8] TIMER interrupt */
			uint32_t irq_pwm: 1;  /**< bit[9] PWM interrupt */
			uint32_t irq_audio: 1;  /**< bit[10] AUDIO interrupt */
			uint32_t irq_saradc: 1;  /**< bit[11] SARADC interrupt */
			uint32_t irq_sdio: 1;  /**< bit[12] SDIO interrupt */
			uint32_t irq_usb: 1;  /**< bit[13] USB interrupt */
			uint32_t irq_fft: 1;  /**< bit[14] FFT interrupt */
			uint32_t irq_general_dma: 1;  /**< bit[15] general DMA interrupt */

			//FIQ interrupt enable
			uint32_t fiq_modem: 1; /**< bit[16] MODEM interrupt */
			uint32_t fiq_mac_txrx_timer: 1; /**< bit[17] MAC tx&rx timer interrupt */
			uint32_t fiq_mac_txrx_misc: 1; /**< bit[18] MAC tx&rx misc interrupt */
			uint32_t fiq_mac_rx_trigger: 1; /**< bit[19] MAC rx trigger interrupt */
			uint32_t fiq_mac_tx_trigger: 1; /**< bit[20] MAC tx trigger interrupt */
			uint32_t fiq_mac_prot_trigger: 1; /**< bit[21] MAC prot tigger interrupt */
			uint32_t fiq_mac_general: 1; /**< bit[22] MAC general interrupt */
			uint32_t fiq_sdio_dma: 1; /**< bit[23] SDIO DMA interrupt */
			uint32_t fiq_usb_plug_in_out: 1; /**< bit[24] USB plug in/out interrupt */
			uint32_t fiq_security: 1; /**< bit[25] security interrupt */
			uint32_t fiq_mac_wakeup: 1; /**< bit[26] MAC wakeup interrupt */
			uint32_t fiq_hsspi_slave: 1; /**< bit[27] HSSPI slave interrupt */
			uint32_t fiq_pll_unlock: 1; /**< bit[28] PLL unlock interrupt */
			uint32_t fiq_jpeg: 1; /**< bit[29] JPEG interrupt */
			uint32_t fiq_ble: 1; /**< bit[30] BLE interrupt */
			uint32_t fiq_psram: 1; /**< bit[31] PSRAM interrupt */
		};
		uint32_t v;
	} int_en;

	//reg17
	union {
		struct {
			uint32_t irq_en: 1;  /**< bit[0] IRQ global enable */
			uint32_t fiq_en: 1;  /**< bit[1] FIQ global enable */

			uint32_t reserved: 30;
		};
		uint32_t v;
	} int_global_en;

	//reg18 interrupt status (before mask)
	union {
		struct {

			//IRQ interrupt enable
			uint32_t irq_uart1: 1;  /**< bit[0] UART1 interrupt */
			uint32_t irq_uart2: 1;  /**< bit[1] UART2 interrupt */
			uint32_t irq_i2c1: 1;  /**< bit[2] I2C1 interrupt */
			uint32_t irq_irda: 1;  /**< bit[3] IRDA interrupt */
			uint32_t irq_i2s_pcm: 1;  /**< bit[4] I2S PCM interrupt */
			uint32_t irq_i2c2: 1;  /**< bit[5] I2C2 interrupt */
			uint32_t irq_spi: 1;  /**< bit[6] GPIO interrupt */
			uint32_t irq_gpio: 1;  /**< bit[7] GPIO interrupt */
			uint32_t irq_timer: 1;  /**< bit[8] TIMER interrupt */
			uint32_t irq_pwm: 1;  /**< bit[9] PWM interrupt */
			uint32_t irq_audio: 1;  /**< bit[10] AUDIO interrupt */
			uint32_t irq_saradc: 1;  /**< bit[11] SARADC interrupt */
			uint32_t irq_sdio: 1;  /**< bit[12] SDIO interrupt */
			uint32_t irq_usb: 1;  /**< bit[13] USB interrupt */
			uint32_t irq_fft: 1;  /**< bit[14] FFT interrupt */
			uint32_t irq_general_dma: 1;  /**< bit[15] general DMA interrupt */

			//FIQ interrupt enable
			uint32_t fiq_modem: 1; /**< bit[16] MODEM interrupt */
			uint32_t fiq_mac_txrx_timer: 1; /**< bit[17] MAC tx&rx timer interrupt */
			uint32_t fiq_mac_txrx_misc: 1; /**< bit[18] MAC tx&rx misc interrupt */
			uint32_t fiq_mac_rx_trigger: 1; /**< bit[19] MAC rx trigger interrupt */
			uint32_t fiq_mac_tx_trigger: 1; /**< bit[20] MAC tx trigger interrupt */
			uint32_t fiq_mac_prot_trigger: 1; /**< bit[21] MAC prot tigger interrupt */
			uint32_t fiq_mac_general: 1; /**< bit[22] MAC general interrupt */
			uint32_t fiq_sdio_dma: 1; /**< bit[23] SDIO DMA interrupt */
			uint32_t fiq_usb_plug_in_out: 1; /**< bit[24] USB plug in/out interrupt */
			uint32_t fiq_security: 1; /**< bit[25] security interrupt */
			uint32_t fiq_mac_wakeup: 1; /**< bit[26] MAC wakeup interrupt */
			uint32_t fiq_hsspi_slave: 1; /**< bit[27] HSSPI slave interrupt */
			uint32_t fiq_pll_unlock: 1; /**< bit[28] PLL unlock interrupt */
			uint32_t fiq_jpeg: 1; /**< bit[29] JPEG interrupt */
			uint32_t fiq_ble: 1; /**< bit[30] BLE interrupt */
			uint32_t fiq_psram: 1; /**< bit[31] PSRAM interrupt */
		};

		uint32_t v;
	} int_raw_status;

	//reg19 interrupt status (after mask)
	union {
		struct {

			//IRQ interrupt enable
			uint32_t irq_uart1: 1;  /**< bit[0] UART1 interrupt */
			uint32_t irq_uart2: 1;  /**< bit[1] UART2 interrupt */
			uint32_t irq_i2c1: 1;  /**< bit[2] I2C1 interrupt */
			uint32_t irq_irda: 1;  /**< bit[3] IRDA interrupt */
			uint32_t irq_i2s_pcm: 1;  /**< bit[4] I2S PCM interrupt */
			uint32_t irq_i2c2: 1;  /**< bit[5] I2C2 interrupt */
			uint32_t irq_spi: 1;  /**< bit[6] GPIO interrupt */
			uint32_t irq_gpio: 1;  /**< bit[7] GPIO interrupt */
			uint32_t irq_timer: 1;  /**< bit[8] TIMER interrupt */
			uint32_t irq_pwm: 1;  /**< bit[9] PWM interrupt */
			uint32_t irq_audio: 1;  /**< bit[10] AUDIO interrupt */
			uint32_t irq_saradc: 1;  /**< bit[11] SARADC interrupt */
			uint32_t irq_sdio: 1;  /**< bit[12] SDIO interrupt */
			uint32_t irq_usb: 1;  /**< bit[13] USB interrupt */
			uint32_t irq_fft: 1;  /**< bit[14] FFT interrupt */
			uint32_t irq_general_dma: 1;  /**< bit[15] general DMA interrupt */

			//FIQ interrupt enable
			uint32_t fiq_modem: 1; /**< bit[16] MODEM interrupt */
			uint32_t fiq_mac_txrx_timer: 1; /**< bit[17] MAC tx&rx timer interrupt */
			uint32_t fiq_mac_txrx_misc: 1; /**< bit[18] MAC tx&rx misc interrupt */
			uint32_t fiq_mac_rx_trigger: 1; /**< bit[19] MAC rx trigger interrupt */
			uint32_t fiq_mac_tx_trigger: 1; /**< bit[20] MAC tx trigger interrupt */
			uint32_t fiq_mac_prot_trigger: 1; /**< bit[21] MAC prot tigger interrupt */
			uint32_t fiq_mac_general: 1; /**< bit[22] MAC general interrupt */
			uint32_t fiq_sdio_dma: 1; /**< bit[23] SDIO DMA interrupt */
			uint32_t fiq_usb_plug_in_out: 1; /**< bit[24] USB plug in/out interrupt */
			uint32_t fiq_security: 1; /**< bit[25] security interrupt */
			uint32_t fiq_mac_wakeup: 1; /**< bit[26] MAC wakeup interrupt */
			uint32_t fiq_hsspi_slave: 1; /**< bit[27] HSSPI slave interrupt */
			uint32_t fiq_pll_unlock: 1; /**< bit[28] PLL unlock interrupt */
			uint32_t fiq_jpeg: 1; /**< bit[29] JPEG interrupt */
			uint32_t fiq_ble: 1; /**< bit[30] BLE interrupt */
			uint32_t fiq_psram: 1; /**< bit[31] PSRAM interrupt */
		};

		uint32_t v;
	} int_status;

	//reg20 ARM wakeup enable
	union {
		struct {
			uint32_t uart1: 1; /**<bit[0], UART1 wakeup */
			uint32_t uart2: 1; /**<bit[1], UART2 wakeup */
			uint32_t i2c1: 1; /**<bit[2], I2C1 wakeup */
			uint32_t irda: 1; /**<bit[3], IRDA wakeup */
			uint32_t i2s_pcm: 1; /**<bit[4], I2S/PCM wakeup */
			uint32_t i2c2: 1; /**<bit[5], I2C2 wakeup */
			uint32_t spi: 1; /**<bit[6], SPI wakeup */
			uint32_t gpio: 1; /**<bit[7], GPIO wakeup */
			uint32_t timer: 1; /**<bit[8], TIMER wakeup */
			uint32_t pwm: 1; /**<bit[9], PWM wakeup */
			uint32_t audio: 1; /**<bit[10], AUDIOwakeup */
			uint32_t saradc: 1; /**<bit[11], SARADC wakeup */
			uint32_t sdio: 1; /**<bit[12], SDIO wakeup */
			uint32_t usb: 1; /**<bit[13], USB wakeup */
			uint32_t fft: 1; /**<bit[14], FFT wakeup */
			uint32_t general_dma: 1; /**<bit[15], general DMA wakeup */
			uint32_t modem: 1; /**<bit[16], modem wakeup */
			uint32_t mac_tx_rx_timer: 1; /**<bit[17], MAC tx&rx timer wakeup */
			uint32_t mac_tx_rx_misc: 1; /**<bit[18], MAC tx&rx misc wakeup */
			uint32_t mac_rx_trigger: 1; /**<bit[19], MAC rx trigger wakeup */
			uint32_t mac_tx_trigger: 1; /**<bit[20], MAC TX trigger wakeup */
			uint32_t mac_prot_trigger: 1; /**<bit[21], MAC prot trigger wakeup */
			uint32_t mac_general: 1; /**<bit[22], MAC general wakeup */
			uint32_t sdio_dma: 1; /**<bit[23], SDIO DMA wakeup */
			uint32_t usb_plug_in_out: 1; /**<bit[24],usb plug in out wakeup */
			uint32_t security:1; /**<bit[25],security wakeup */
			uint32_t mac_wakeup: 1; /**<bit[26], MAC wakeup */
			uint32_t hsspi_slave: 1; /**<bit[27], MAC general wakeup */
			uint32_t reserved: 1; /**<bit[28], reserved */
			uint32_t jpeg: 1; /**<bit[29], JPEG general wakeup */
			uint32_t ble: 1; /**<bit[30], BLE wakeup */
			uint32_t psram: 1; /**<bit[31], PSRAM wakeup */
		};
		uint32_t v;
	} arm_wakeup_en;

	//reg21~reg31
	uint32_t reserved2[32 - 21];

	//reg32 TL410 interrupt enable
	union {
		union {
			uint32_t uart1: 1; /**< bit[0] UART1 interrupt */
			uint32_t uart2: 1; /**< bit[1] UART2 interrupt */
			uint32_t i2c1: 1; /**< bit[2] I2C1 interrupt */
			uint32_t irda: 1; /**< bit[3] IRDA interrupt */
			uint32_t i2s_pcm: 1; /**< bit[4] I2S/PCM interrupt */
			uint32_t i2c2: 1; /**< bit[5] I2C2 interrupt */
			uint32_t spi: 1; /**< bit[6] SPI interrupt */
			uint32_t gpio: 1; /**< bit[7] GPIO interrupt */
			uint32_t reserved: 1; /**< bit[8] */
			uint32_t pwm: 1; /**< bit[9] PWM interrupt */
			uint32_t audio: 1; /**< bit[10] AUDIO interrupt */
			uint32_t saradc: 1; /**< bit[11] SARADC interrupt */
			uint32_t sdio: 1; /**< bit[12] SDIO interrupt */
			uint32_t usb: 1; /**< bit[13] USB interrupt */
			uint32_t fft: 1; /**< bit[14] FFT interrupt */
			uint32_t reserved1: 1; /**< bit[15] */
			uint32_t mailbox: 2; /**< bit[16:17] mailbox interrupt */

			uint32_t reserved2: 14; /**< bit[18:31] */
		};
		uint32_t v;
	} tl410_int_en;


	//reg33 TL410 interrupt raw status befor mask
	union {
		union {
			uint32_t uart1: 1; /**< bit[0] UART1  */
			uint32_t uart2: 1; /**< bit[1] UART2  */
			uint32_t i2c1: 1; /**< bit[2] I2C1  */
			uint32_t irda: 1; /**< bit[3] IRDA  */
			uint32_t i2s_pcm: 1; /**< bit[4] I2S/PCM  */
			uint32_t i2c2: 1; /**< bit[5] I2C2  */
			uint32_t spi: 1; /**< bit[6] SPI  */
			uint32_t gpio: 1; /**< bit[7] GPIO  */
			uint32_t reserved: 1; /**< bit[8] */
			uint32_t pwm: 1; /**< bit[9] PWM  */
			uint32_t audio: 1; /**< bit[10] AUDIO  */
			uint32_t saradc: 1; /**< bit[11] SARADC  */
			uint32_t sdio: 1; /**< bit[12] SDIO  */
			uint32_t usb: 1; /**< bit[13] USB  */
			uint32_t fft: 1; /**< bit[14] FFT  */
			uint32_t reserved1: 1; /**< bit[15] */
			uint32_t mailbox: 2; /**< bit[16:17] mailbox  */

			uint32_t reserved2: 14; /**< bit[18:31] */
		};

		uint32_t v;
	} tl410_int_raw_status;

	//reg34 TL410 interrupt status after mask
	union {
		union {
			uint32_t uart1: 1; /**< bit[0] UART1  */
			uint32_t uart2: 1; /**< bit[1] UART2  */
			uint32_t i2c1: 1; /**< bit[2] I2C1  */
			uint32_t irda: 1; /**< bit[3] IRDA  */
			uint32_t i2s_pcm: 1; /**< bit[4] I2S/PCM  */
			uint32_t i2c2: 1; /**< bit[5] I2C2  */
			uint32_t spi: 1; /**< bit[6] SPI  */
			uint32_t gpio: 1; /**< bit[7] GPIO  */
			uint32_t reserved: 1; /**< bit[8] */
			uint32_t pwm: 1; /**< bit[9] PWM  */
			uint32_t audio: 1; /**< bit[10] AUDIO  */
			uint32_t saradc: 1; /**< bit[11] SARADC  */
			uint32_t sdio: 1; /**< bit[12] SDIO  */
			uint32_t usb: 1; /**< bit[13] USB  */
			uint32_t fft: 1; /**< bit[14] FFT  */
			uint32_t reserved1: 1; /**< bit[15] */
			uint32_t mailbox: 2; /**< bit[16:17] mailbox  */

			uint32_t reserved2: 14; /**< bit[18:31] */
		};

		uint32_t v;
	} tl410_int_status;

	//reg35 TL410 wakeup enable
	union {
		union {
			uint32_t uart1: 1; /**< bit[0] UART1 wakeup enable */
			uint32_t uart2: 1; /**< bit[1] UART2 wakeup enable */
			uint32_t i2c1: 1; /**< bit[2] I2C1 wakeup enable */
			uint32_t irda: 1; /**< bit[3] IRDA wakeup enable */
			uint32_t i2s_pcm: 1; /**< bit[4] I2S/PCM wakeup enable */
			uint32_t i2c2: 1; /**< bit[5] I2C2 wakeup enable */
			uint32_t spi: 1; /**< bit[6] SPI wakeup enable */
			uint32_t gpio: 1; /**< bit[7] GPIO wakeup enable */
			uint32_t reserved: 1; /**< bit[8] */
			uint32_t pwm: 1; /**< bit[9] PWM wakeup enable */
			uint32_t audio: 1; /**< bit[10] AUDIO wakeup enable */
			uint32_t saradc: 1; /**< bit[11] SARADC wakeup enable */
			uint32_t sdio: 1; /**< bit[12] SDIO wakeup enable */
			uint32_t usb: 1; /**< bit[13] USB wakeup enable */
			uint32_t fft: 1; /**< bit[14] FFT wakeup enable */
			uint32_t reserved2: 1; /**< bit[15] */
			uint32_t mailbox: 2; /**< bit[16:17] mailbox wakeup enable */
			uint32_t reserved3: 14; /**< bit[18:31] */
		};
		uint32_t v;
	} tl410_wakeup_en;

} icu_hw_t;

#ifdef __cplusplus
}
#endif

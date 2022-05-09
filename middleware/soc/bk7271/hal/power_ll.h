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

#include <soc/soc.h>
#include "icu_hw.h"
#include "pwm_ll.h"


#ifdef __cplusplus
extern "C" {
#endif

#define ICU_LL_REG_BASE() (SOC_ICU_REG_BASE)

#define CHAN_BITS_MASK ((1 << SOC_PWM_CHAN_NUM_PER_GROUP) - 1)
static uint16_t s_chan_pwr_up_bits = 0;


static bool power_ll_pwm_is_group_all_chan_pwr_down(uint32_t group)
{
	uint32_t pwr_up_bits_of_group = (s_chan_pwr_up_bits >> (group * SOC_PWM_CHAN_NUM_PER_GROUP)) & CHAN_BITS_MASK;

	return (pwr_up_bits_of_group == 0);
}

/* In BK7271, all channels in a PWM group share the same Power Down bit, the
 * software control the actual down of PWM group power.
 * */
static inline void power_ll_pwm_pwr_up(icu_hw_t *hw, uint32_t chan)
{
	uint32_t group = PWM_LL_GROUP(chan);

	s_chan_pwr_up_bits |= BIT(chan);
	REG_MCHAN_SET_FIELD(group, ICU_R_PWM_CHAN_PWR, ICU_F_PWM_CHAN_PWR, ICU_V_PWM_CHAN_PWR_UP);
}

static inline void power_ll_pwm_pwr_down(icu_hw_t *hw, uint32_t chan)
{
	uint32_t group = PWM_LL_GROUP(chan);

	s_chan_pwr_up_bits &= ~(BIT(chan));
	if (power_ll_pwm_is_group_all_chan_pwr_down(chan))
		REG_MCHAN_SET_FIELD(group, ICU_R_PWM_CHAN_PWR, ICU_F_PWM_CHAN_PWR, ICU_V_PWM_CHAN_PWR_UP);
}


static inline void power_ll_pwr_up_timer(icu_hw_t *hw, uint32_t timer_id)
{
	REG_MCHAN_SET_FIELD(timer_id, ICU_R_TIMER_CHAN_PWR, ICU_F_TIMER_CHAN_PWR, ICU_V_TIMER_CHAN_PWR_UP);
}

static inline void power_ll_pwr_down_timer(icu_hw_t *hw, uint32_t timer_id)
{
	REG_MCHAN_SET_FIELD(timer_id, ICU_R_TIMER_CHAN_PWR, ICU_F_TIMER_CHAN_PWR, ICU_V_TIMER_CHAN_PWR_DOWN);
}

static inline void power_ll_pwr_up_qspi(icu_hw_t *hw)
{
}

static inline void power_ll_pwr_down_qspi(icu_hw_t *hw)
{
}

static inline void power_ll_pwr_up_jpeg(icu_hw_t *hw)
{
	hw->func_clk_pwr_down.jpeg= 0;
}

static inline void power_ll_pwr_down_jpeg(icu_hw_t *hw)
{
	hw->func_clk_pwr_down.jpeg= 1;
}

static inline void power_ll_pwr_up_fft(icu_hw_t *hw)
{
}
static inline void power_ll_pwr_down_fft(icu_hw_t *hw)
{
}

static inline void power_ll_pwr_up_sdio(icu_hw_t *hw)
{
	hw->clk_pwr_down.sdio_host= 0;
}
static inline void power_ll_pwr_down_sdio(icu_hw_t *hw)
{
	hw->clk_pwr_down.sdio_host= 1;
}

static inline void power_ll_pwr_up_tl410_wdt(icu_hw_t *hw)
{
}
static inline void power_ll_pwr_down_tl410_wdt(icu_hw_t *hw)
{
}

static inline void power_ll_pwr_up_audio(icu_hw_t *hw)
{
}
static inline void power_ll_pwr_down_audio(icu_hw_t *hw)
{
}

static inline void power_ll_pwr_up_arm_wdt(icu_hw_t *hw)
{
	hw->func_clk_pwr_down.arm_watchdog= 0;
}
static inline void power_ll_pwr_down_arm_wdt(icu_hw_t *hw)
{
	hw->func_clk_pwr_down.arm_watchdog= 1;
}

static inline void power_ll_pwr_up_saradc(icu_hw_t *hw)
{
	hw->clk_pwr_down.saradc= 0;
}
static inline void power_ll_pwr_down_saradc(icu_hw_t *hw)
{
	hw->clk_pwr_down.saradc= 1;
}

static inline void power_ll_pwr_up_spi(icu_hw_t *hw, uint32 id)
{
	REG_CLR_BIT(&hw->clk_pwr_down, 1 << (id + ICU_F_SPI_CHAN_PWR_S)) ;
}
static inline void power_ll_pwr_down_spi(icu_hw_t *hw, uint32 id)
{
	REG_SET_BIT(&hw->clk_pwr_down, 1 << (id + ICU_F_SPI_CHAN_PWR_S) );
}

static inline void power_ll_pwr_up_i2s_pcm(icu_hw_t *hw)
{
}
static inline void power_ll_pwr_down_i2s_pcm(icu_hw_t *hw)
{
}

static inline void power_ll_pwr_up_irda(icu_hw_t *hw)
{
	hw->clk_pwr_down.irda= 0;
}

static inline void power_ll_pwr_down_irda(icu_hw_t *hw)
{
	hw->clk_pwr_down.irda= 1;
}

static inline void power_ll_pwr_up_i2c(icu_hw_t *hw, uint32 id)
{
	if (id == 0) {
		hw->clk_pwr_down.fm_i2c= 0;
	} else if (id == 1) {
		hw->clk_pwr_down.i2c1= 0;
	} else {
		hw->clk_pwr_down.i2c2= 0;
	}
}

static inline void power_ll_pwr_down_i2c(icu_hw_t *hw, uint32 id)
{
	if (id == 0) {
		hw->clk_pwr_down.fm_i2c= 1;
	} else if (id == 1) {
		hw->clk_pwr_down.i2c1= 1;
	} else {
		hw->clk_pwr_down.i2c2= 1;
	}
}

static inline void power_ll_pwr_up_uart(icu_hw_t *hw, uint32_t id)
{
	REG_CLR_BIT(&hw->clk_pwr_down, 1 << id) ;
}

static inline void power_ll_pwr_down_uart(icu_hw_t *hw, uint32_t id)
{
	REG_SET_BIT(&hw->clk_pwr_down, 1 << id );
}


static inline void power_ll_pwr_up_ble(icu_hw_t *hw)
{
}
static inline void power_ll_pwr_down_ble(icu_hw_t *hw)
{
}

static inline void power_ll_pwr_up_tl410(icu_hw_t *hw)
{
}
static inline void power_ll_pwr_down_tl410(icu_hw_t *hw)
{
}

static inline void power_ll_pwr_up_security(icu_hw_t *hw)
{
	hw->func_clk_pwr_down.security = 0;
}
static inline void power_ll_pwr_down_security(icu_hw_t *hw)
{
	hw->func_clk_pwr_down.security = 1;
}

static inline void power_ll_pwr_up_cec(icu_hw_t *hw)
{
	hw->clk_pwr_down.cec = 0;
}
static inline void power_ll_pwr_down_cec(icu_hw_t *hw)
{
	hw->clk_pwr_down.cec = 1;
}


static inline void power_ll_pwr_up_sdio_dma(icu_hw_t *hw)
{
	hw->func_clk_pwr_down.sdio_dma = 0;
}
static inline void power_ll_pwr_down_sdio_dma(icu_hw_t *hw)
{
	hw->func_clk_pwr_down.sdio_dma = 1;
}

static inline void power_ll_pwr_up_usb(icu_hw_t *hw, uint32 id)
{
	REG_CLR_BIT(&hw->func_clk_pwr_down, 1 << (id + ICU_F_USB_CHAN_PWR_S)) ;
}
static inline void power_ll_pwr_down_usb(icu_hw_t *hw, uint32 id)
{
	REG_SET_BIT(&hw->func_clk_pwr_down, 1 << (id + ICU_F_USB_CHAN_PWR_S) );
}

static inline void power_ll_pwr_up_wdt(icu_hw_t *hw)
{
	hw->func_clk_pwr_down.arm_watchdog = 0;
}

static inline void power_ll_pwr_down_wdt(icu_hw_t *hw)
{
	hw->func_clk_pwr_down.arm_watchdog = 1;
}

#ifdef __cplusplus
}
#endif

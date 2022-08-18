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


#ifdef __cplusplus
extern "C" {
#endif

#define ICU_LL_REG_BASE() (SOC_ICU_REG_BASE)

static inline void clock_ll_set_dco_clk_1_div(icu_hw_t *hw)
{
	hw->peri_clk_mux.dco_clk_div = 0;
}

static inline void clock_ll_set_dco_clk_2_div(icu_hw_t *hw)
{
	hw->peri_clk_mux.dco_clk_div = 1;
}

static inline void clock_ll_set_dco_clk_4_div(icu_hw_t *hw)
{
	hw->peri_clk_mux.dco_clk_div = 2;
}

static inline void clock_ll_set_dco_clk_8_div(icu_hw_t *hw)
{
	hw->peri_clk_mux.dco_clk_div = 3;
}

static inline void clock_ll_set_qspi_clk_26m(icu_hw_t *hw)
{
}

static inline void clock_ll_set_qspi_clk_dco(icu_hw_t *hw)
{
}

static inline void clock_ll_set_qspi_clk_120m(icu_hw_t *hw)
{
}
static inline void clock_ll_set_qspi_clk_80m(icu_hw_t *hw)
{

}


static inline void clock_ll_set_pwms_clk_26m(icu_hw_t *hw)
{
	hw->peri_clk_mux.pwms = ICU_V_PWMS_CLK_MUX_26M;
}

static inline void clock_ll_set_pwms_clk_dco(icu_hw_t *hw)
{
	hw->peri_clk_mux.pwms = ICU_V_PWMS_CLK_MUX_DCO;
}

static inline void clock_ll_enable_pwm_lpo_clk(icu_hw_t *hw, uint32_t chan)
{
	REG_MCHAN_SET_FIELD(chan, ICU_R_PWM_CHAN_CLK_MUX, ICU_F_PWM_CHAN_CLK_MUX, ICU_V_PWM_CHAN_CLK_MUX_26M);
}

static inline void clock_ll_disable_pwm_lpo_clk(icu_hw_t *hw, uint32_t chan)
{
	REG_MCHAN_SET_FIELD(chan, ICU_R_PWM_CHAN_CLK_MUX, ICU_F_PWM_CHAN_CLK_MUX, ICU_V_PWM_CHAN_CLK_MUX_GLOBAL);
}


static inline void clock_ll_set_saradc_enable_audio_pll(icu_hw_t *hw)
{
}

static inline void clock_ll_set_saradc_disable_audio_pll(icu_hw_t *hw)
{
}

static inline void clock_ll_set_efuse_clk_dco(icu_hw_t *hw)
{
	hw->peri_clk_mux.efuse = 0;
}

static inline void clock_ll_set_efuse_clk_26m(icu_hw_t *hw)
{
	hw->peri_clk_mux.efuse = 1;
}

static inline void clock_ll_set_sdio_clk_dco(icu_hw_t *hw)
{
	hw->peri_clk_mux.sdio = 0;
}

static inline void clock_ll_set_sdio_clk_26m(icu_hw_t *hw)
{
	hw->peri_clk_mux.sdio = 1;
}

static inline void clock_ll_set_saradc_clk_dco(icu_hw_t *hw)
{
	hw->peri_clk_mux.saradc = 0;
}

static inline void clock_ll_set_saradc_clk_26m(icu_hw_t *hw)
{
	hw->peri_clk_mux.saradc = 1;
}

static inline void clock_ll_set_spi_clk_dco(icu_hw_t *hw, uint32_t id)
{
	hw->peri_clk_mux.spi = 0;
}

static inline void clock_ll_set_spi_clk_26m(icu_hw_t *hw, uint32_t id)
{
	hw->peri_clk_mux.spi = 1;
}

static inline void clock_ll_set_irda_clk_dco(icu_hw_t *hw)
{
	hw->peri_clk_mux.irda = 0;
}

static inline void clock_ll_set_irda_clk_26m(icu_hw_t *hw)
{
	hw->peri_clk_mux.irda = 1;
}

static inline void clock_ll_set_i2c_clk_dco(icu_hw_t *hw, uint32 id)
{
	if(id == 0)
		hw->peri_clk_mux.i2c1 = 0;
	else
		hw->peri_clk_mux.i2c2 = 0;
}

static inline void clock_ll_set_i2c_clk_26m(icu_hw_t *hw, uint32 id)
{
	if(id == 0)
		hw->peri_clk_mux.i2c1 = 1;
	else
		hw->peri_clk_mux.i2c2 = 1;
}

static inline void clock_ll_set_uart_clk_26m(icu_hw_t *hw, uint32_t id)
{
	REG_SET_BIT(&hw->peri_clk_mux, 1 << (id & 0x1));
}

static inline void clock_ll_set_uart_clk_dco(icu_hw_t *hw, uint32_t id)
{
	REG_CLR_BIT(&hw->peri_clk_mux, 1 << (id & 0x1));
}


static inline void clock_ll_set_26m_clk_1_div(icu_hw_t *hw)
{
	hw->clk26m_div_factor.factor = 0;
}

static inline void clock_ll_set_26m_clk_2_div(icu_hw_t *hw)
{
	hw->clk26m_div_factor.factor = 1;
}

static inline void clock_ll_set_26m_clk_4_div(icu_hw_t *hw)
{
	hw->clk26m_div_factor.factor = 2;
}

static inline void clock_ll_set_26m_clk_8_div(icu_hw_t *hw)
{
	hw->clk26m_div_factor.factor = 3;
}

static inline void clock_ll_set_i2s_mclk_1_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2s_mclk_2_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2s_mclk_4_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2s_mclk_8_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2s_mclk_16_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2s_mclk_32_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_cec_clk_dco(icu_hw_t *hw)
{
}

static inline void clock_ll_set_cec_clk_26m(icu_hw_t *hw)
{
}

static inline uint32_t clock_ll_get_uart_clk(icu_hw_t *hw, uint32_t id)
{
	return (hw->peri_clk_mux.v & BIT(id)) ? ICU_V_CLK_MUX_26M : ICU_V_CLK_MUX_DCO;
}

#if CONFIG_SOC_BK7236A
static inline void icu_ll_jpeg_set_96m(icu_hw_t *hw)
{
	hw->peri_clk_mux.jpeg = 0;
}
static inline void icu_ll_jpeg_set_120m(icu_hw_t *hw)
{
	hw->peri_clk_mux.jpeg = 1;
}
static inline void icu_ll_jpeg_set_160m(icu_hw_t *hw)
{
	hw->peri_clk_mux.jpeg = 2;
}
static inline void icu_ll_jpeg_set_240m(icu_hw_t *hw)
{
	hw->peri_clk_mux.jpeg = 3;
}
#endif

#ifdef __cplusplus
}
#endif

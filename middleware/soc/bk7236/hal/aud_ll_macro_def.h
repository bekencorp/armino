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

/***********************************************************************************************************************************
* This file is generated from BK7256_ADDR Mapping_20211224_format_change_highlight_20220113_update.xlsm automatically
* Modify it manually is not recommended
* CHIP ID:BK7256,GENARATE TIME:2022-01-18 11:24:40
************************************************************************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <soc/soc.h>
#include "aud_reg.h"

static inline uint32_t aud_ll_get_audio_config_value(void)
{
	return REG_READ(AUD_CONFIG);
}

static inline void aud_ll_set_audio_config_value(uint32_t value)
{
	REG_WRITE(AUD_CONFIG,value);
}

static inline uint32_t aud_ll_get_audio_config_samp_rate_adc(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value = ((reg_value >> SAMPLE_RATE_ADC_POSI) & SAMPLE_RATE_ADC_MASK);
	return reg_value;
}

static inline void aud_ll_set_audio_config_samp_rate_adc(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value &= ~(SAMPLE_RATE_ADC_MASK << SAMPLE_RATE_ADC_POSI);
	reg_value |= ((value & SAMPLE_RATE_ADC_MASK) << SAMPLE_RATE_ADC_POSI);
	REG_WRITE(AUD_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_audio_config_dac_enable(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value = ((reg_value >> DAC_ENABLE_POSI) & DAC_ENABLE_MASK);
	return reg_value;
}

static inline void aud_ll_set_audio_config_dac_enable(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value &= ~(DAC_ENABLE_MASK << DAC_ENABLE_POSI);
	reg_value |= ((value & DAC_ENABLE_MASK) << DAC_ENABLE_POSI);
	REG_WRITE(AUD_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_audio_config_adc_enable(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value = ((reg_value >> ADC_ENABLE_POSI) & ADC_ENABLE_MASK);
	return reg_value;
}

static inline void aud_ll_set_audio_config_adc_enable(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value &= ~(ADC_ENABLE_MASK << ADC_ENABLE_POSI);
	reg_value |= ((value & ADC_ENABLE_MASK) << ADC_ENABLE_POSI);
	REG_WRITE(AUD_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_audio_config_dtmf_enable(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value = ((reg_value >> DTMF_ENABLE_POSI) & DTMF_ENABLE_MASK);
	return reg_value;
}

static inline void aud_ll_set_audio_config_dtmf_enable(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value &= ~(DTMF_ENABLE_MASK << DTMF_ENABLE_POSI);
	reg_value |= ((value & DTMF_ENABLE_MASK) << DTMF_ENABLE_POSI);
	REG_WRITE(AUD_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_audio_config_line_enable(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value = ((reg_value >> LINEIN_ENABLE_POSI) & LINEIN_ENABLE_MASK);
	return reg_value;
}

static inline void aud_ll_set_audio_config_line_enable(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value &= ~(LINEIN_ENABLE_MASK << LINEIN_ENABLE_POSI);
	reg_value |= ((value & LINEIN_ENABLE_MASK) << LINEIN_ENABLE_POSI);
	REG_WRITE(AUD_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_audio_config_samp_rate_dac(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value = ((reg_value >> SAMPLE_RATE_DAC_POSI) & SAMPLE_RATE_DAC_MASK);
	return reg_value;
}

static inline void aud_ll_set_audio_config_samp_rate_dac(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_CONFIG);
	reg_value &= ~(SAMPLE_RATE_DAC_MASK << SAMPLE_RATE_DAC_POSI);
	reg_value |= ((value & SAMPLE_RATE_DAC_MASK) << SAMPLE_RATE_DAC_POSI);
	REG_WRITE(AUD_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_dtmf_config0_value(void)
{
	return REG_READ(AUD_DTMF_CONFIG_0);
}

static inline void aud_ll_set_dtmf_config0_value(uint32_t value)
{
	REG_WRITE(AUD_DTMF_CONFIG_0,value);
}

static inline uint32_t aud_ll_get_dtmf_config0_tone_pattern(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_0);
	reg_value = ((reg_value >> TONE_PATTERN_POSI) & TONE_PATTERN_MASK);
	return reg_value;
}

static inline void aud_ll_set_dtmf_config0_tone_pattern(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_0);
	reg_value &= ~(TONE_PATTERN_MASK << TONE_PATTERN_POSI);
	reg_value |= ((value & TONE_PATTERN_MASK) << TONE_PATTERN_POSI);
	REG_WRITE(AUD_DTMF_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_dtmf_config0_tone_mode(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_0);
	reg_value = ((reg_value >> TONE_MODE_POSI) & TONE_MODE_MASK);
	return reg_value;
}

static inline void aud_ll_set_dtmf_config0_tone_mode(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_0);
	reg_value &= ~(TONE_MODE_MASK << TONE_MODE_POSI);
	reg_value |= ((value & TONE_MODE_MASK) << TONE_MODE_POSI);
	REG_WRITE(AUD_DTMF_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_dtmf_config0_tone_pause_time(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_0);
	reg_value = ((reg_value >> TONE_PAUSE_TIME_POSI) & TONE_PAUSE_TIME_MASK);
	return reg_value;
}

static inline void aud_ll_set_dtmf_config0_tone_pause_time(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_0);
	reg_value &= ~(TONE_PAUSE_TIME_MASK << TONE_PAUSE_TIME_POSI);
	reg_value |= ((value & TONE_PAUSE_TIME_MASK) << TONE_PAUSE_TIME_POSI);
	REG_WRITE(AUD_DTMF_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_dtmf_config0_tone_active_time(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_0);
	reg_value = ((reg_value >> TONE_ACTIVE_TIME_POSI) & TONE_ACTIVE_TIME_MASK);
	return reg_value;
}

static inline void aud_ll_set_dtmf_config0_tone_active_time(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_0);
	reg_value &= ~(TONE_ACTIVE_TIME_MASK << TONE_ACTIVE_TIME_POSI);
	reg_value |= ((value & TONE_ACTIVE_TIME_MASK) << TONE_ACTIVE_TIME_POSI);
	REG_WRITE(AUD_DTMF_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_dtmf_config1_value(void)
{
	return REG_READ(AUD_DTMF_CONFIG_1);
}

static inline void aud_ll_set_dtmf_config1_value(uint32_t value)
{
	REG_WRITE(AUD_DTMF_CONFIG_1,value);
}

static inline uint32_t aud_ll_get_dtmf_config1_tone1_step(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_1);
	reg_value = ((reg_value >> TONE_STEP_POSI) & TONE_STEP_MASK);
	return reg_value;
}

static inline void aud_ll_set_dtmf_config1_tone1_step(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_1);
	reg_value &= ~(TONE_STEP_MASK << TONE_STEP_POSI);
	reg_value |= ((value & TONE_STEP_MASK) << TONE_STEP_POSI);
	REG_WRITE(AUD_DTMF_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_dtmf_config1_tone1_attu(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_1);
	reg_value = ((reg_value >> TONE_ATTU_POSI) & TONE_ATTU_MASK);
	return reg_value;
}

static inline void aud_ll_set_dtmf_config1_tone1_attu(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_1);
	reg_value &= ~(TONE_ATTU_MASK << TONE_ATTU_POSI);
	reg_value |= ((value & TONE_ATTU_MASK) << TONE_ATTU_POSI);
	REG_WRITE(AUD_DTMF_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_dtmf_config1_tone1_enable(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_1);
	reg_value = ((reg_value >> TONE_ENABLE_POSI) & TONE_ENABLE_MASK);
	return reg_value;
}

static inline void aud_ll_set_dtmf_config1_tone1_enable(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_1);
	reg_value &= ~(TONE_ENABLE_MASK << TONE_ENABLE_POSI);
	reg_value |= ((value & TONE_ENABLE_MASK) << TONE_ENABLE_POSI);
	REG_WRITE(AUD_DTMF_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_dtmf_config2_value(void)
{
	return REG_READ(AUD_DTMF_CONFIG_2);
}

static inline void aud_ll_set_dtmf_config2_value(uint32_t value)
{
	REG_WRITE(AUD_DTMF_CONFIG_2,value);
}

static inline uint32_t aud_ll_get_dtmf_config2_tone2_step(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_2);
	reg_value = ((reg_value >> TONE_STEP_POSI) & TONE_STEP_MASK);
	return reg_value;
}

static inline void aud_ll_set_dtmf_config2_tone2_step(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_2);
	reg_value &= ~(TONE_STEP_MASK << TONE_STEP_POSI);
	reg_value |= ((value & TONE_STEP_MASK) << TONE_STEP_POSI);
	REG_WRITE(AUD_DTMF_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_dtmf_config2_tone2_attu(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_2);
	reg_value = ((reg_value >> TONE_ATTU_POSI) & TONE_ATTU_MASK);
	return reg_value;
}

static inline void aud_ll_set_dtmf_config2_tone2_attu(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_2);
	reg_value &= ~(TONE_ATTU_MASK << TONE_ATTU_POSI);
	reg_value |= ((value & TONE_ATTU_MASK) << TONE_ATTU_POSI);
	REG_WRITE(AUD_DTMF_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_dtmf_config2_tone2_enable(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_2);
	reg_value = ((reg_value >> TONE_ENABLE_POSI) & TONE_ENABLE_MASK);
	return reg_value;
}

static inline void aud_ll_set_dtmf_config2_tone2_enable(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_CONFIG_2);
	reg_value &= ~(TONE_ENABLE_MASK << TONE_ENABLE_POSI);
	reg_value |= ((value & TONE_ENABLE_MASK) << TONE_ENABLE_POSI);
	REG_WRITE(AUD_DTMF_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_adc_config0_value(void)
{
	return REG_READ(AUD_ADC_CONFIG_0);
}

static inline void aud_ll_set_adc_config0_value(uint32_t value)
{
	REG_WRITE(AUD_ADC_CONFIG_0,value);
}

static inline uint32_t aud_ll_get_adc_config0_adc_hpf2_coef_b2(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_0);
	reg_value = ((reg_value >> ADC_HPF2_COEF_B2_POSI) & ADC_HPF2_COEF_B2_MASK);
	return reg_value;
}

static inline void aud_ll_set_adc_config0_adc_hpf2_coef_b2(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_0);
	reg_value &= ~(ADC_HPF2_COEF_B2_MASK << ADC_HPF2_COEF_B2_POSI);
	reg_value |= ((value & ADC_HPF2_COEF_B2_MASK) << ADC_HPF2_COEF_B2_POSI);
	REG_WRITE(AUD_ADC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_adc_config0_adc_hpf2_bypass(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_0);
	reg_value = ((reg_value >> ADC_HPF2_BYPASS_POSI) & ADC_HPF2_BYPASS_MASK);
	return reg_value;
}

static inline void aud_ll_set_adc_config0_adc_hpf2_bypass(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_0);
	reg_value &= ~(ADC_HPF2_BYPASS_MASK << ADC_HPF2_BYPASS_POSI);
	reg_value |= ((value & ADC_HPF2_BYPASS_MASK) << ADC_HPF2_BYPASS_POSI);
	REG_WRITE(AUD_ADC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_adc_config0_adc_hpf1_bypass(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_0);
	reg_value = ((reg_value >> ADC_HPF1_BYPASS_POSI) & ADC_HPF1_BYPASS_MASK);
	return reg_value;
}

static inline void aud_ll_set_adc_config0_adc_hpf1_bypass(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_0);
	reg_value &= ~(ADC_HPF1_BYPASS_MASK << ADC_HPF1_BYPASS_POSI);
	reg_value |= ((value & ADC_HPF1_BYPASS_MASK) << ADC_HPF1_BYPASS_POSI);
	REG_WRITE(AUD_ADC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_adc_config0_adc_set_gain(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_0);
	reg_value = ((reg_value >> ADC_SET_GAIN_POSI) & ADC_SET_GAIN_MASK);
	return reg_value;
}

static inline void aud_ll_set_adc_config0_adc_set_gain(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_0);
	reg_value &= ~(ADC_SET_GAIN_MASK << ADC_SET_GAIN_POSI);
	reg_value |= ((value & ADC_SET_GAIN_MASK) << ADC_SET_GAIN_POSI);
	REG_WRITE(AUD_ADC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_adc_config0_adc_sampe_edge(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_0);
	reg_value = ((reg_value >> ADC_SAMPLE_EDGE_POSI) & ADC_SAMPLE_EDGE_MASK);
	return reg_value;
}

static inline void aud_ll_set_adc_config0_adc_sampe_edge(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_0);
	reg_value &= ~(ADC_SAMPLE_EDGE_MASK << ADC_SAMPLE_EDGE_POSI);
	reg_value |= ((value & ADC_SAMPLE_EDGE_MASK) << ADC_SAMPLE_EDGE_POSI);
	REG_WRITE(AUD_ADC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_adc_config1_value(void)
{
	return REG_READ(AUD_ADC_CONFIG_1);
}

static inline void aud_ll_set_adc_config1_value(uint32_t value)
{
	REG_WRITE(AUD_ADC_CONFIG_1,value);
}

static inline uint32_t aud_ll_get_adc_config1_adc_hpf2_coef_b0(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_1);
	reg_value = ((reg_value >> ADC_HPF2_COEF_B0_POSI) & ADC_HPF2_COEF_B0_MASK);
	return reg_value;
}

static inline void aud_ll_set_adc_config1_adc_hpf2_coef_b0(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_1);
	reg_value &= ~(ADC_HPF2_COEF_B0_MASK << ADC_HPF2_COEF_B0_POSI);
	reg_value |= ((value & ADC_HPF2_COEF_B0_MASK) << ADC_HPF2_COEF_B0_POSI);
	REG_WRITE(AUD_ADC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_adc_config1_adc_hpf2_coef_b1(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_1);
	reg_value = ((reg_value >> ADC_HPF2_COEF_B1_POSI) & ADC_HPF2_COEF_B1_MASK);
	return reg_value;
}

static inline void aud_ll_set_adc_config1_adc_hpf2_coef_b1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_1);
	reg_value &= ~(ADC_HPF2_COEF_B1_MASK << ADC_HPF2_COEF_B1_POSI);
	reg_value |= ((value & ADC_HPF2_COEF_B1_MASK) << ADC_HPF2_COEF_B1_POSI);
	REG_WRITE(AUD_ADC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_adc_config2_value(void)
{
	return REG_READ(AUD_ADC_CONFIG_2);
}

static inline void aud_ll_set_adc_config2_value(uint32_t value)
{
	REG_WRITE(AUD_ADC_CONFIG_2,value);
}

static inline uint32_t aud_ll_get_adc_config2_adc_hpf2_coef_a0(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_2);
	reg_value = ((reg_value >> ADC_HPF2_COEF_A0_POSI) & ADC_HPF2_COEF_A0_MASK);
	return reg_value;
}

static inline void aud_ll_set_adc_config2_adc_hpf2_coef_a0(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_2);
	reg_value &= ~(ADC_HPF2_COEF_A0_MASK << ADC_HPF2_COEF_A0_POSI);
	reg_value |= ((value & ADC_HPF2_COEF_A0_MASK) << ADC_HPF2_COEF_A0_POSI);
	REG_WRITE(AUD_ADC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_adc_config2_adc_hpf2_coef_a1(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_2);
	reg_value = ((reg_value >> ADC_HPF2_COEF_A1_POSI) & ADC_HPF2_COEF_A1_MASK);
	return reg_value;
}

static inline void aud_ll_set_adc_config2_adc_hpf2_coef_a1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_CONFIG_2);
	reg_value &= ~(ADC_HPF2_COEF_A1_MASK << ADC_HPF2_COEF_A1_POSI);
	reg_value |= ((value & ADC_HPF2_COEF_A1_MASK) << ADC_HPF2_COEF_A1_POSI);
	REG_WRITE(AUD_ADC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_dac_config0_value(void)
{
	return REG_READ(AUD_DAC_CONFIG_0);
}

static inline void aud_ll_set_dac_config0_value(uint32_t value)
{
	REG_WRITE(AUD_DAC_CONFIG_0,value);
}

static inline uint32_t aud_ll_get_dac_config0_dac_hpf2_coef_b2(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_0);
	reg_value = ((reg_value >> DAC_HPF2_COEF_B2_POSI) & DAC_HPF2_COEF_B2_MASK);
	return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_hpf2_coef_b2(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_0);
	reg_value &= ~(DAC_HPF2_COEF_B2_MASK << DAC_HPF2_COEF_B2_POSI);
	reg_value |= ((value & DAC_HPF2_COEF_B2_MASK) << DAC_HPF2_COEF_B2_POSI);
	REG_WRITE(AUD_DAC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_dac_config0_dac_hpf2_bypass(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_0);
	reg_value = ((reg_value >> DAC_HPF2_BYPASS_POSI) & DAC_HPF2_BYPASS_MASK);
	return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_hpf2_bypass(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_0);
	reg_value &= ~(DAC_HPF2_BYPASS_MASK << DAC_HPF2_BYPASS_POSI);
	reg_value |= ((value & DAC_HPF2_BYPASS_MASK) << DAC_HPF2_BYPASS_POSI);
	REG_WRITE(AUD_DAC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_dac_config0_dac_hpf1_bypass(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_0);
	reg_value = ((reg_value >> DAC_HPF1_BYPASS_POSI) & DAC_HPF1_BYPASS_MASK);
	return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_hpf1_bypass(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_0);
	reg_value &= ~(DAC_HPF1_BYPASS_MASK << DAC_HPF1_BYPASS_POSI);
	reg_value |= ((value & DAC_HPF1_BYPASS_MASK) << DAC_HPF1_BYPASS_POSI);
	REG_WRITE(AUD_DAC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_dac_config0_dac_set_gain(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_0);
	reg_value = ((reg_value >> DAC_SET_GAIN_POSI) & DAC_SET_GAIN_MASK);
	return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_set_gain(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_0);
	reg_value &= ~(DAC_SET_GAIN_MASK << DAC_SET_GAIN_POSI);
	reg_value |= ((value & DAC_SET_GAIN_MASK) << DAC_SET_GAIN_POSI);
	REG_WRITE(AUD_DAC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_dac_config0_dac_clk_invert(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_0);
	reg_value = ((reg_value >> DAC_CLK_INVERT_POSI) & DAC_CLK_INVERT_MASK);
	return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_clk_invert(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_0);
	reg_value &= ~(DAC_CLK_INVERT_MASK << DAC_CLK_INVERT_POSI);
	reg_value |= ((value & DAC_CLK_INVERT_MASK) << DAC_CLK_INVERT_POSI);
	REG_WRITE(AUD_DAC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_dac_config1_value(void)
{
	return REG_READ(AUD_DAC_CONFIG_1);
}

static inline void aud_ll_set_dac_config1_value(uint32_t value)
{
	REG_WRITE(AUD_DAC_CONFIG_1,value);
}

static inline uint32_t aud_ll_get_dac_config1_dac_hpf2_coef_b0(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_1);
	reg_value = ((reg_value >> DAC_HPF2_COEF_B0_POSI) & DAC_HPF2_COEF_B0_MASK);
	return reg_value;
}

static inline void aud_ll_set_dac_config1_dac_hpf2_coef_b0(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_1);
	reg_value &= ~(DAC_HPF2_COEF_B0_MASK << DAC_HPF2_COEF_B0_POSI);
	reg_value |= ((value & DAC_HPF2_COEF_B0_MASK) << DAC_HPF2_COEF_B0_POSI);
	REG_WRITE(AUD_DAC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_dac_config1_dac_hpf2_coef_b1(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_1);
	reg_value = ((reg_value >> DAC_HPF2_COEF_B1_POSI) & DAC_HPF2_COEF_B1_MASK);
	return reg_value;
}

static inline void aud_ll_set_dac_config1_dac_hpf2_coef_b1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_1);
	reg_value &= ~(DAC_HPF2_COEF_B1_MASK << DAC_HPF2_COEF_B1_POSI);
	reg_value |= ((value & DAC_HPF2_COEF_B1_MASK) << DAC_HPF2_COEF_B1_POSI);
	REG_WRITE(AUD_DAC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_dac_config2_value(void)
{
	return REG_READ(AUD_DAC_CONFIG_2);
}

static inline void aud_ll_set_dac_config2_value(uint32_t value)
{
	REG_WRITE(AUD_DAC_CONFIG_2,value);
}

static inline uint32_t aud_ll_get_dac_config2_dac_hpf2_coef_a1(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_2);
	reg_value = ((reg_value >> DAC_HPF2_COEF_A1_POSI) & DAC_HPF2_COEF_A1_MASK);
	return reg_value;
}

static inline void aud_ll_set_dac_config2_dac_hpf2_coef_a1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_2);
	reg_value &= ~(DAC_HPF2_COEF_A1_MASK << DAC_HPF2_COEF_A1_POSI);
	reg_value |= ((value & DAC_HPF2_COEF_A1_MASK) << DAC_HPF2_COEF_A1_POSI);
	REG_WRITE(AUD_DAC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_dac_config2_dac_hpf2_coef_a2(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_2);
	reg_value = ((reg_value >> DAC_HPF2_COEF_A2_POSI) & DAC_HPF2_COEF_A2_MASK);
	return reg_value;
}

static inline void aud_ll_set_dac_config2_dac_hpf2_coef_a2(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DAC_CONFIG_2);
	reg_value &= ~(DAC_HPF2_COEF_A2_MASK << DAC_HPF2_COEF_A2_POSI);
	reg_value |= ((value & DAC_HPF2_COEF_A2_MASK) << DAC_HPF2_COEF_A2_POSI);
	REG_WRITE(AUD_DAC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_fifo_config_value(void)
{
	return REG_READ(AUD_FIFO_CONFIG);
}

static inline void aud_ll_set_fifo_config_value(uint32_t value)
{
	REG_WRITE(AUD_FIFO_CONFIG,value);
}

static inline uint32_t aud_ll_get_fifo_config_dacr_rd_threshold(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value = ((reg_value >> DAC_R_RD_THRED_POSI) & DAC_R_RD_THRED_MASK);
	return reg_value;
}

static inline void aud_ll_set_fifo_config_dacr_rd_threshold(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value &= ~(DAC_R_RD_THRED_MASK << DAC_R_RD_THRED_POSI);
	reg_value |= ((value & DAC_R_RD_THRED_MASK) << DAC_R_RD_THRED_POSI);
	REG_WRITE(AUD_FIFO_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_fifo_config_dacl_rd_threshold(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value = ((reg_value >> DAC_L_RD_THRED_POSI) & DAC_L_RD_THRED_MASK);
	return reg_value;
}

static inline void aud_ll_set_fifo_config_dacl_rd_threshold(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value &= ~(DAC_L_RD_THRED_MASK << DAC_L_RD_THRED_POSI);
	reg_value |= ((value & DAC_L_RD_THRED_MASK) << DAC_L_RD_THRED_POSI);
	REG_WRITE(AUD_FIFO_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_fifo_config_dtmf_wr_threshold(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value = ((reg_value >> DTMF_WR_THRED_POSI) & DTMF_WR_THRED_MASK);
	return reg_value;
}

static inline void aud_ll_set_fifo_config_dtmf_wr_threshold(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value &= ~(DTMF_WR_THRED_MASK << DTMF_WR_THRED_POSI);
	reg_value |= ((value & DTMF_WR_THRED_MASK) << DTMF_WR_THRED_POSI);
	REG_WRITE(AUD_FIFO_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_fifo_config_adcl_wr_threshold(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value = ((reg_value >> ADC_WR_THRED_POSI) & ADC_WR_THRED_MASK);
	return reg_value;
}

static inline void aud_ll_set_fifo_config_adcl_wr_threshold(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value &= ~(ADC_WR_THRED_MASK << ADC_WR_THRED_POSI);
	reg_value |= ((value & ADC_WR_THRED_MASK) << ADC_WR_THRED_POSI);
	REG_WRITE(AUD_FIFO_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_fifo_config_dacr_int_en(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value = ((reg_value >> DAC_R_INT_EN_POSI) & DAC_R_INT_EN_MASK);
	return reg_value;
}

static inline void aud_ll_set_fifo_config_dacr_int_en(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value &= ~(DAC_R_INT_EN_MASK << DAC_R_INT_EN_POSI);
	reg_value |= ((value & DAC_R_INT_EN_MASK) << DAC_R_INT_EN_POSI);
	REG_WRITE(AUD_FIFO_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_fifo_config_dacl_int_en(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value = ((reg_value >> DAC_L_INT_EN_POSI) & DAC_L_INT_EN_MASK);
	return reg_value;
}

static inline void aud_ll_set_fifo_config_dacl_int_en(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value &= ~(DAC_L_INT_EN_MASK << DAC_L_INT_EN_POSI);
	reg_value |= ((value & DAC_L_INT_EN_MASK) << DAC_L_INT_EN_POSI);
	REG_WRITE(AUD_FIFO_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_fifo_config_dtmf_int_en(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value = ((reg_value >> DTMF_INT_EN_POSI) & DTMF_INT_EN_MASK);
	return reg_value;
}

static inline void aud_ll_set_fifo_config_dtmf_int_en(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value &= ~(DTMF_INT_EN_MASK << DTMF_INT_EN_POSI);
	reg_value |= ((value & DTMF_INT_EN_MASK) << DTMF_INT_EN_POSI);
	REG_WRITE(AUD_FIFO_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_fifo_config_adcl_int_en(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value = ((reg_value >> ADC_INT_EN_POSI) & ADC_INT_EN_MASK);
	return reg_value;
}

static inline void aud_ll_set_fifo_config_adcl_int_en(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value &= ~(ADC_INT_EN_MASK << ADC_INT_EN_POSI);
	reg_value |= ((value & ADC_INT_EN_MASK) << ADC_INT_EN_POSI);
	REG_WRITE(AUD_FIFO_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_fifo_config_loop_ton2dac(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value = ((reg_value >> LOOP_TON2DAC_POSI) & LOOP_TON2DAC_MASK);
	return reg_value;
}

static inline void aud_ll_set_fifo_config_loop_ton2dac(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value &= ~(LOOP_TON2DAC_MASK << LOOP_TON2DAC_POSI);
	reg_value |= ((value & LOOP_TON2DAC_MASK) << LOOP_TON2DAC_POSI);
	REG_WRITE(AUD_FIFO_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_fifo_config_loop_adc2dac(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value = ((reg_value >> LOOP_ADC2DAC_POSI) & LOOP_ADC2DAC_MASK);
	return reg_value;
}

static inline void aud_ll_set_fifo_config_loop_adc2dac(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_CONFIG);
	reg_value &= ~(LOOP_ADC2DAC_MASK << LOOP_ADC2DAC_POSI);
	reg_value |= ((value & LOOP_ADC2DAC_MASK) << LOOP_ADC2DAC_POSI);
	REG_WRITE(AUD_FIFO_CONFIG,reg_value);
}

static inline uint32_t aud_ll_get_agc_config0_value(void)
{
	return REG_READ(AUD_AGC_CONFIG_0);
}

static inline void aud_ll_set_agc_config0_value(uint32_t value)
{
	REG_WRITE(AUD_AGC_CONFIG_0,value);
}

static inline uint32_t aud_ll_get_agc_config0_agc_noise_thrd(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_0);
	reg_value = ((reg_value >> AGC_NOISE_THRED_POSI) & AGC_NOISE_THRED_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config0_agc_noise_thrd(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_0);
	reg_value &= ~(AGC_NOISE_THRED_MASK << AGC_NOISE_THRED_POSI);
	reg_value |= ((value & AGC_NOISE_THRED_MASK) << AGC_NOISE_THRED_POSI);
	REG_WRITE(AUD_AGC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_agc_config0_agc_noise_high(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_0);
	reg_value = ((reg_value >> AGC_NOISE_HIGH_POSI) & AGC_NOISE_HIGH_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config0_agc_noise_high(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG_0);
    reg_value &= ~(AGC_NOISE_HIGH_MASK << AGC_NOISE_HIGH_POSI);
    reg_value |= ((value & AGC_NOISE_HIGH_MASK) << AGC_NOISE_HIGH_POSI);
    REG_WRITE(AUD_AGC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_agc_config0_agc_noise_low(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_0);
	reg_value = ((reg_value >> AGC_NOISE_LOW_POSI) & AGC_NOISE_LOW_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config0_agc_noise_low(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_0);
	reg_value &= ~(AGC_NOISE_LOW_MASK << AGC_NOISE_LOW_POSI);
	reg_value |= ((value & AGC_NOISE_LOW_MASK) << AGC_NOISE_LOW_POSI);
	REG_WRITE(AUD_AGC_CONFIG_0,reg_value);
}

static inline uint32_t aud_ll_get_agc_config1_value(void)
{
	return REG_READ(AUD_AGC_CONFIG_1);
}

static inline void aud_ll_set_agc_config1_value(uint32_t value)
{
	REG_WRITE(AUD_AGC_CONFIG_1,value);
}

static inline uint32_t aud_ll_get_agc_config1_agc_noise_min(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value = ((reg_value >> AGC_NOISE_MIN_POSI) & AGC_NOISE_MIN_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_noise_min(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value &= ~(AGC_NOISE_MIN_MASK << AGC_NOISE_MIN_POSI);
	reg_value |= ((value & AGC_NOISE_MIN_MASK) << AGC_NOISE_MIN_POSI);
	REG_WRITE(AUD_AGC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_agc_config1_agc_noise_tout(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value = ((reg_value >> AGC_NOISE_TOUT_POSI) & AGC_NOISE_TOUT_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_noise_tout(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value &= ~(AGC_NOISE_TOUT_MASK << AGC_NOISE_TOUT_POSI);
	reg_value |= ((value & AGC_NOISE_TOUT_MASK) << AGC_NOISE_TOUT_POSI);
	REG_WRITE(AUD_AGC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_agc_config1_agc_high_dur(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value = ((reg_value >> AGC_HIGH_DUR_POSI) & AGC_HIGH_DUR_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_high_dur(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value &= ~(AGC_HIGH_DUR_MASK << AGC_HIGH_DUR_POSI);
	reg_value |= ((value & AGC_HIGH_DUR_MASK) << AGC_HIGH_DUR_POSI);
	REG_WRITE(AUD_AGC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_agc_config1_agc_low_dur(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value = ((reg_value >> AGC_LOW_DUR_POSI) & AGC_LOW_DUR_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_low_dur(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value &= ~(AGC_LOW_DUR_MASK << AGC_LOW_DUR_POSI);
	reg_value |= ((value & AGC_LOW_DUR_MASK) << AGC_LOW_DUR_POSI);
	REG_WRITE(AUD_AGC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_agc_config1_agc_min(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value = ((reg_value >> AGC_MIN_POSI) & AGC_MIN_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_min(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value &= ~(AGC_MIN_MASK << AGC_MIN_POSI);
	reg_value |= ((value & AGC_MIN_MASK) << AGC_MIN_POSI);
	REG_WRITE(AUD_AGC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_agc_config1_agc_max(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value = ((reg_value >> AGC_MAX_POSI) & AGC_MAX_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_max(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value &= ~(AGC_MAX_MASK << AGC_MAX_POSI);
	reg_value |= ((value & AGC_MAX_MASK) << AGC_MAX_POSI);
	REG_WRITE(AUD_AGC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_agc_config1_agc_ng_method(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value = ((reg_value >> AGC_NG_METHOD_POSI) & AGC_NG_METHOD_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_ng_method(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value &= ~(AGC_NG_METHOD_MASK << AGC_NG_METHOD_POSI);
	reg_value |= ((value & AGC_NG_METHOD_MASK) << AGC_NG_METHOD_POSI);
	REG_WRITE(AUD_AGC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_agc_config1_agc_ng_enable(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value = ((reg_value >> AGC_NG_ENABLE_POSI) & AGC_NG_ENABLE_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_ng_enable(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_1);
	reg_value &= ~(AGC_NG_ENABLE_MASK << AGC_NG_ENABLE_POSI);
	reg_value |= ((value & AGC_NG_ENABLE_MASK) << AGC_NG_ENABLE_POSI);
	REG_WRITE(AUD_AGC_CONFIG_1,reg_value);
}

static inline uint32_t aud_ll_get_agc_config2_value(void)
{
	return REG_READ(AUD_AGC_CONFIG_2);
}

static inline void aud_ll_set_agc_config2_value(uint32_t value)
{
	REG_WRITE(AUD_AGC_CONFIG_2,value);
}

static inline uint32_t aud_ll_get_agc_config2_agc_decay_time(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value = ((reg_value >> AGC_DECAY_TIME_POSI) & AGC_DECAY_TIME_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_decay_time(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value &= ~(AGC_DECAY_TIME_MASK << AGC_DECAY_TIME_POSI);
	reg_value |= ((value & AGC_DECAY_TIME_MASK) << AGC_DECAY_TIME_POSI);
	REG_WRITE(AUD_AGC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_agc_config2_agc_attack_time(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value = ((reg_value >> AGC_ATTACK_TIME_POSI) & AGC_ATTACK_TIME_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_attack_time(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value &= ~(AGC_ATTACK_TIME_MASK << AGC_ATTACK_TIME_POSI);
	reg_value |= ((value & AGC_ATTACK_TIME_MASK) << AGC_ATTACK_TIME_POSI);
	REG_WRITE(AUD_AGC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_agc_config2_agc_high_thrd(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value = ((reg_value >> AGC_HIGH_THRD_POSI) & AGC_HIGH_THRD_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_high_thrd(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value &= ~(AGC_HIGH_THRD_MASK << AGC_HIGH_THRD_POSI);
	reg_value |= ((value & AGC_HIGH_THRD_MASK) << AGC_HIGH_THRD_POSI);
	REG_WRITE(AUD_AGC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_agc_config2_agc_low_thrd(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value = ((reg_value >> AGC_LOW_THRED_POSI) & AGC_LOW_THRED_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_low_thrd(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value &= ~(AGC_LOW_THRED_MASK << AGC_LOW_THRED_POSI);
	reg_value |= ((value & AGC_LOW_THRED_MASK) << AGC_LOW_THRED_POSI);
	REG_WRITE(AUD_AGC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_agc_config2_agc_iir_coef(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value = ((reg_value >> AGC_IIR_COEF_POSI) & AGC_IIR_COEF_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_iir_coef(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value &= ~(AGC_IIR_COEF_MASK << AGC_IIR_COEF_POSI);
	reg_value |= ((value & AGC_IIR_COEF_MASK) << AGC_IIR_COEF_POSI);
	REG_WRITE(AUD_AGC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_agc_config2_agc_enable(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value = ((reg_value >> AGC_ENABLE_POSI) & AGC_ENABLE_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_enable(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value &= ~(AGC_ENABLE_MASK << AGC_ENABLE_POSI);
	reg_value |= ((value & AGC_ENABLE_MASK) << AGC_ENABLE_POSI);
	REG_WRITE(AUD_AGC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_agc_config2_manual_pga_value(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value = ((reg_value >> MANUAL_PGA_VAL_POSI) & MANUAL_PGA_VAL_MASK);
	return reg_value;
}

static inline void aud_ll_set_agc_config2_manual_pga_value(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value &= ~(MANUAL_PGA_VAL_MASK << MANUAL_PGA_VAL_POSI);
	reg_value |= ((value & MANUAL_PGA_VAL_MASK) << MANUAL_PGA_VAL_POSI);
	REG_WRITE(AUD_AGC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_agc_config2_manual_pga(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG_2);
    reg_value = ((reg_value >> MANUAL_PGA_MODE_POSI) & MANUAL_PGA_MODE_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config2_manual_pga(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_CONFIG_2);
	reg_value &= ~(MANUAL_PGA_MODE_MASK << MANUAL_PGA_MODE_POSI);
	reg_value |= ((value & MANUAL_PGA_MODE_MASK) << MANUAL_PGA_MODE_POSI);
	REG_WRITE(AUD_AGC_CONFIG_2,reg_value);
}

static inline uint32_t aud_ll_get_fifo_status_value(void)
{
	return REG_READ(AUD_FIFO_STATUS);
}

static inline uint32_t aud_ll_get_fifo_status_dacr_near_full(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DAC_R_NEAR_FULL_POSI) & DAC_R_NEAR_FULL_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dacl_near_full(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DAC_L_NEAR_FULL_POSI) & DAC_L_NEAR_FULL_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_adcl_near_full(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> ADC_NEAR_FULL_POSI) & ADC_NEAR_FULL_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dtmf_near_full(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DTMF_NEAR_FULL_POSI) & DTMF_NEAR_FULL_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dacr_near_empty(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DAC_R_NEAR_EMPTY_POSI) & DAC_R_NEAR_EMPTY_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dacl_near_empty(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DAC_L_NEAR_EMPTY_POSI) & DAC_L_NEAR_EMPTY_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_adcl_near_empty(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> ADC_NEAR_EMPTY_POSI) & ADC_NEAR_EMPTY_POSI);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dtmf_near_empty(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DTMF_NEAR_EMPTY_POSI) & DTMF_NEAR_EMPTY_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dacr_fifo_full(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DAC_R_FIFO_FULL_POSI) & DAC_R_FIFO_FULL_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dacl_fifo_full(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DAC_L_FIFO_FULL_POSI) & DAC_L_FIFO_FULL_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_adcl_fifo_full(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> ADC_FIFO_FULL_POSI) & ADC_FIFO_FULL_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dtmf_fifo_full(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DTMF_FIFO_FULL_POSI) & DTMF_FIFO_FULL_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dacr_fifo_empty(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DAC_R_FIFO_EMPTY_POSI) & DAC_R_FIFO_EMPTY_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dacl_fifo_empty(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DAC_L_FIFO_EMPTY_POSI) & DAC_L_FIFO_EMPTY_POSI);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_adcl_fifo_empty(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> ADC_FIFO_EMPTY_POSI) & ADC_FIFO_EMPTY_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dtmf_fifo_empty(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DTMF_FIFO_EMPTY_POSI) & DTMF_FIFO_EMPTY_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dacr_int_flag(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DAC_R_INT_FLAG_POSI) & DAC_R_INT_FLAG_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dacl_int_flag(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DAC_L_INT_FLAG_POSI) & DAC_L_INT_FLAG_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_adcl_int_flag(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> ADC_INT_FLAG_POSI) & ADC_INT_FLAG_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_fifo_status_dtmf_int_flag(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FIFO_STATUS);
	reg_value = ((reg_value >> DTMF_INT_FLAG_POSI) & DTMF_INT_FLAG_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_agc_status_value(void)
{
	return REG_READ(AUD_AGC_STATUS);
}

static inline uint32_t aud_ll_get_agc_status_rssi(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_STATUS);
	reg_value = ((reg_value >> AGC_RSSI_POSI) & AGC_RSSI_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_agc_status_mic_pga(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_STATUS);
	reg_value = ((reg_value >> AGC_MIC_PGA_POSI) & AGC_MIC_PGA_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_agc_status_mic_rssi(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_AGC_STATUS);
	reg_value = ((reg_value >> AGC_MIC_RSSI_POSI) & AGC_MIC_RSSI_POSI);
	return reg_value;
}

static inline uint32_t aud_ll_get_dtmf_fport_value(void)
{
	return REG_READ(AUD_DTMF_FIFO_PORT);
}

static inline uint32_t aud_ll_get_dtmf_fport_dtmf_port(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_DTMF_FIFO_PORT);
	reg_value = ((reg_value >> AD_DTMF_FIFO_POSI) & AD_DTMF_FIFO_MASK);
	return reg_value;
}

static inline uint32_t aud_ll_get_adc_fport_value(void)
{
	return REG_READ(AUD_ADC_FIFO_PORT);
}

static inline uint32_t aud_ll_get_adc_fport_adc_port(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_ADC_FIFO_PORT);
	reg_value = ((reg_value >> AD_ADC_L_FIFO_POSI) & AD_ADC_L_FIFO_MASK);
	return reg_value;
}

static inline void aud_ll_set_dacl_fport_value(uint32_t value)
{
	REG_WRITE(AUD_DAC_FIFO_PORT,value);
}

static inline uint32_t aud_ll_get_extend_cfg_value(void)
{
	return REG_READ(AUD_EXTEND_CFG);
}

static inline void aud_ll_set_extend_cfg_value(uint32_t value)
{
	REG_WRITE(AUD_EXTEND_CFG,value);
}

static inline uint32_t aud_ll_get_extend_cfg_dac_fracmod_manual(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_EXTEND_CFG);
	reg_value = ((reg_value >> DAC_FRACMOD_MANUAL_POSI) & DAC_FRACMOD_MANUAL_MASK);
	return reg_value;
}

static inline void aud_ll_set_extend_cfg_dac_fracmod_manual(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_EXTEND_CFG);
	reg_value &= ~(DAC_FRACMOD_MANUAL_MASK << DAC_FRACMOD_MANUAL_POSI);
	reg_value |= ((value & DAC_FRACMOD_MANUAL_MASK) << DAC_FRACMOD_MANUAL_POSI);
	REG_WRITE(AUD_EXTEND_CFG,reg_value);
}

static inline uint32_t aud_ll_get_extend_cfg_adc_fracmod_manual(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_EXTEND_CFG);
	reg_value = ((reg_value >> ADC_FRACMOD_MANUAL_POSI) & ADC_FRACMOD_MANUAL_MASK);
	return reg_value;
}

static inline void aud_ll_set_extend_cfg_adc_fracmod_manual(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_EXTEND_CFG);
	reg_value &= ~(ADC_FRACMOD_MANUAL_MASK << ADC_FRACMOD_MANUAL_POSI);
	reg_value |= ((value & ADC_FRACMOD_MANUAL_MASK) << ADC_FRACMOD_MANUAL_POSI);
	REG_WRITE(AUD_EXTEND_CFG,reg_value);
}

static inline uint32_t aud_ll_get_extend_cfg_filt_enable(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_EXTEND_CFG);
	reg_value = ((reg_value >> FILT_ENABLE_POSI) & FILT_ENABLE_MASK);
	return reg_value;
}

static inline void aud_ll_set_extend_cfg_filt_enable(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_EXTEND_CFG);
	reg_value &= ~(FILT_ENABLE_MASK << FILT_ENABLE_POSI);
	reg_value |= ((value & FILT_ENABLE_MASK) << FILT_ENABLE_POSI);
	REG_WRITE(AUD_EXTEND_CFG,reg_value);
}

static inline uint32_t aud_ll_get_dac_fracmod_value(void)
{
	return REG_READ(AUD_DAC_FRACMOD);
}

static inline void aud_ll_set_dac_fracmod_value(uint32_t value)
{
	REG_WRITE(AUD_DAC_FRACMOD,value);
}

static inline uint32_t aud_ll_get_dac_fracmod_dac_fraccoef(void)
{
	return REG_READ(AUD_DAC_FRACMOD);
}

static inline void aud_ll_set_dac_fracmod_dac_fraccoef(uint32_t value)
{
	REG_WRITE(AUD_DAC_FRACMOD,value);
}

static inline uint32_t aud_ll_get_adc_fracmod_value(void)
{
	return REG_READ(AUD_ADC_FRACMOD);
}

static inline void aud_ll_set_adc_fracmod_value(uint32_t value)
{
	REG_WRITE(AUD_ADC_FRACMOD,value);
}

static inline uint32_t aud_ll_get_adc_fracmod_adc_fraccoef(void)
{
	return REG_READ(AUD_ADC_FRACMOD);
}

static inline void aud_ll_set_adc_fracmod_adc_fraccoef(uint32_t value)
{
	REG_WRITE(AUD_ADC_FRACMOD,value);
}

static inline uint32_t aud_ll_get_hpf2_ext_coef_value(void)
{
	return REG_READ(AUD_HPF2_EXT_COEF);
}

static inline void aud_ll_set_hpf2_ext_coef_value(uint32_t value)
{
	REG_WRITE(AUD_HPF2_EXT_COEF,value);
}

static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_a1_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_HPF2_EXT_COEF);
	reg_value = ((reg_value >> HPF2_A1_L_6BIT_POSI) & HPF2_A1_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_a1_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_HPF2_EXT_COEF);
	reg_value &= ~(HPF2_A1_L_6BIT_MASK << HPF2_A1_L_6BIT_POSI);
	reg_value |= ((value & HPF2_A1_L_6BIT_MASK) << HPF2_A1_L_6BIT_POSI);
	REG_WRITE(AUD_HPF2_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_a2_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_HPF2_EXT_COEF);
	reg_value = ((reg_value >> HPF2_A2_L_6BIT_POSI) & HPF2_A2_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_a2_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_HPF2_EXT_COEF);
	reg_value &= ~(HPF2_A2_L_6BIT_MASK << HPF2_A2_L_6BIT_POSI);
	reg_value |= ((value & HPF2_A2_L_6BIT_MASK) << HPF2_A2_L_6BIT_POSI);
	REG_WRITE(AUD_HPF2_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_b0_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_HPF2_EXT_COEF);
	reg_value = ((reg_value >> HPF2_B0_L_6BIT_POSI) & HPF2_B0_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_b0_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_HPF2_EXT_COEF);
	reg_value &= ~(HPF2_B0_L_6BIT_MASK << HPF2_B0_L_6BIT_POSI);
	reg_value |= ((value & HPF2_B0_L_6BIT_MASK) << HPF2_B0_L_6BIT_POSI);
	REG_WRITE(AUD_HPF2_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_b1_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_HPF2_EXT_COEF);
	reg_value = ((reg_value >> HPF2_B1_L_6BIT_POSI) & HPF2_B1_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_b1_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_HPF2_EXT_COEF);
	reg_value &= ~(HPF2_B1_L_6BIT_MASK << HPF2_B1_L_6BIT_POSI);
	reg_value |= ((value & HPF2_B1_L_6BIT_MASK) << HPF2_B1_L_6BIT_POSI);
	REG_WRITE(AUD_HPF2_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_b2_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_HPF2_EXT_COEF);
	reg_value = ((reg_value >> HPF2_B2_L_6BIT_POSI) & HPF2_B2_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_b2_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_HPF2_EXT_COEF);
	reg_value &= ~(HPF2_B2_L_6BIT_MASK << HPF2_B2_L_6BIT_POSI);
	reg_value |= ((value & HPF2_B2_L_6BIT_MASK) << HPF2_B2_L_6BIT_POSI);
	REG_WRITE(AUD_HPF2_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt0_coef_a1a2_value(void)
{
	return REG_READ(AUD_FLT_0_COEF_1);
}

static inline void aud_ll_set_flt0_coef_a1a2_value(uint32_t value)
{
	REG_WRITE(AUD_FLT_0_COEF_1,value);
}

static inline uint32_t aud_ll_get_flt0_coef_a1a2_flt0_a1(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_COEF_1);
	reg_value = ((reg_value >> FLT_0_A1_POSI) & FLT_0_A1_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt0_coef_a1a2_flt0_a1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_COEF_1);
	reg_value &= ~(FLT_0_A1_MASK << FLT_0_A1_POSI);
	reg_value |= ((value & FLT_0_A1_MASK) << FLT_0_A1_POSI);
	REG_WRITE(AUD_FLT_0_COEF_1,reg_value);
}

static inline uint32_t aud_ll_get_flt0_coef_a1a2_flt0_a2(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_COEF_1);
	reg_value = ((reg_value >> FLT_0_A2_POSI) & FLT_0_A2_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt0_coef_a1a2_flt0_a2(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_COEF_1);
	reg_value &= ~(FLT_0_A2_MASK << FLT_0_A2_POSI);
	reg_value |= ((value & FLT_0_A2_MASK) << FLT_0_A2_POSI);
	REG_WRITE(AUD_FLT_0_COEF_1,reg_value);
}

static inline uint32_t aud_ll_get_flt0_coef_b0b1_value(void)
{
	return REG_READ(AUD_FLT_0_COEF_2);
}

static inline void aud_ll_set_flt0_coef_b0b1_value(uint32_t value)
{
	REG_WRITE(AUD_FLT_0_COEF_2,value);
}

static inline uint32_t aud_ll_get_flt0_coef_b0b1_flt0_b0(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_COEF_2);
	reg_value = ((reg_value >> FLT_0_B0_POSI) & FLT_0_B0_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt0_coef_b0b1_flt0_b0(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_COEF_2);
	reg_value &= ~(FLT_0_B0_MASK << FLT_0_B0_POSI);
	reg_value |= ((value & FLT_0_B0_MASK) << FLT_0_B0_POSI);
	REG_WRITE(AUD_FLT_0_COEF_2,reg_value);
}

static inline uint32_t aud_ll_get_flt0_coef_b0b1_flt0_b1(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_COEF_2);
	reg_value = ((reg_value >> FLT_0_B1_POSI) & FLT_0_B1_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt0_coef_b0b1_flt0_b1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_COEF_2);
	reg_value &= ~(FLT_0_B1_MASK << FLT_0_B1_POSI);
	reg_value |= ((value & FLT_0_B1_MASK) << FLT_0_B1_POSI);
	REG_WRITE(AUD_FLT_0_COEF_2,reg_value);
}

static inline uint32_t aud_ll_get_flt0_coef_b2_value(void)
{
	return REG_READ(AUD_FLT_0_COEF_3);
}

static inline void aud_ll_set_flt0_coef_b2_value(uint32_t value)
{
	REG_WRITE(AUD_FLT_0_COEF_3,value);
}

static inline uint32_t aud_ll_get_flt0_coef_b2_flt0_b2(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_COEF_3);
	reg_value = ((reg_value >> FLT_0_B2_POSI) & FLT_0_B2_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt0_coef_b2_flt0_b2(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_COEF_3);
	reg_value &= ~(FLT_0_B2_MASK << FLT_0_B2_POSI);
	reg_value |= ((value & FLT_0_B2_MASK) << FLT_0_B2_POSI);
	REG_WRITE(AUD_FLT_0_COEF_3,reg_value);
}

static inline uint32_t aud_ll_get_flt1_coef_a1a2_value(void)
{
	return REG_READ(AUD_FLT_1_COEF_1);
}

static inline void aud_ll_set_flt1_coef_a1a2_value(uint32_t value)
{
	REG_WRITE(AUD_FLT_1_COEF_1,value);
}

static inline uint32_t aud_ll_get_flt1_coef_a1a2_flt1_a1(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_COEF_1);
	reg_value = ((reg_value >> FLT_1_A1_POSI) & FLT_1_A1_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt1_coef_a1a2_flt1_a1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_COEF_1);
	reg_value &= ~(FLT_1_A1_MASK << FLT_1_A1_POSI);
	reg_value |= ((value & FLT_1_A1_MASK) << FLT_1_A1_POSI);
	REG_WRITE(AUD_FLT_1_COEF_1,reg_value);
}

static inline uint32_t aud_ll_get_flt1_coef_a1a2_flt1_a2(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_COEF_1);
	reg_value = ((reg_value >> FLT_1_A2_POSI) & FLT_1_A2_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt1_coef_a1a2_flt1_a2(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_COEF_1);
	reg_value &= ~(FLT_1_A2_MASK << FLT_1_A2_POSI);
	reg_value |= ((value & FLT_1_A2_MASK) << FLT_1_A2_POSI);
	REG_WRITE(AUD_FLT_1_COEF_1,reg_value);
}

static inline uint32_t aud_ll_get_flt1_coef_b0b1_value(void)
{
	return REG_READ(AD_FLT_1_COEF_2);
}

static inline void aud_ll_set_flt1_coef_b0b1_value(uint32_t value)
{
	REG_WRITE(AD_FLT_1_COEF_2,value);
}

static inline uint32_t aud_ll_get_flt1_coef_b0b1_flt1_b0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AD_FLT_1_COEF_2);
    reg_value = ((reg_value >> FLT_1_B0_POSI) & FLT_1_B0_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt1_coef_b0b1_flt1_b0(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_1_COEF_2);
	reg_value &= ~(FLT_1_B0_MASK << FLT_1_B0_POSI);
	reg_value |= ((value & FLT_1_B0_MASK) << FLT_1_B0_POSI);
	REG_WRITE(AD_FLT_1_COEF_2,reg_value);
}

static inline uint32_t aud_ll_get_flt1_coef_b0b1_flt1_b1(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_1_COEF_2);
	reg_value = ((reg_value >> FLT_1_B1_POSI) & FLT_1_B1_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt1_coef_b0b1_flt1_b1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_1_COEF_2);
	reg_value &= ~(FLT_1_B1_MASK << FLT_1_B1_POSI);
	reg_value |= ((value & FLT_1_B1_MASK) << FLT_1_B1_POSI);
	REG_WRITE(AD_FLT_1_COEF_2,reg_value);
}

static inline uint32_t aud_ll_get_flt1_coef_b2_value(void)
{
	return REG_READ(AD_FLT_1_COEF_3);
}

static inline void aud_ll_set_flt1_coef_b2_value(uint32_t value)
{
	REG_WRITE(AD_FLT_1_COEF_3,value);
}

static inline uint32_t aud_ll_get_flt1_coef_b2_flt1_b2(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_1_COEF_3);
	reg_value = ((reg_value >> FLT_1_B2_POSI) & FLT_1_B2_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt1_coef_b2_flt1_b2(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_1_COEF_3);
	reg_value &= ~(FLT_1_B2_MASK << FLT_1_B2_POSI);
	reg_value |= ((value & FLT_1_B2_MASK) << FLT_1_B2_POSI);
	REG_WRITE(AD_FLT_1_COEF_3,reg_value);
}

static inline uint32_t aud_ll_get_flt2_coef_a1a2_value(void)
{
	return REG_READ(AUD_FLT_2_COEF_1);
}

static inline void aud_ll_set_flt2_coef_a1a2_value(uint32_t value)
{
	REG_WRITE(AUD_FLT_2_COEF_1,value);
}

static inline uint32_t aud_ll_get_flt2_coef_a1a2_flt2_a1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT_2_COEF_1);
    reg_value = ((reg_value >> FLT_2_A1_POSI) & FLT_2_A1_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt2_coef_a1a2_flt2_a1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_COEF_1);
	reg_value &= ~(FLT_2_A1_MASK << FLT_2_A1_POSI);
	reg_value |= ((value & FLT_2_A1_MASK) << FLT_2_A1_POSI);
	REG_WRITE(AUD_FLT_2_COEF_1,reg_value);
}

static inline uint32_t aud_ll_get_flt2_coef_a1a2_flt2_a2(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_COEF_1);
	reg_value = ((reg_value >> FLT_2_A2_POSI) & FLT_2_A2_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt2_coef_a1a2_flt2_a2(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_COEF_1);
	reg_value &= ~(FLT_2_A2_MASK << FLT_2_A2_POSI);
	reg_value |= ((value & FLT_2_A2_MASK) << FLT_2_A2_POSI);
	REG_WRITE(AUD_FLT_2_COEF_1,reg_value);
}

static inline uint32_t aud_ll_get_flt2_coef_b0b1_value(void)
{
	return REG_READ(AD_FLT_2_COEF_2);
}

static inline void aud_ll_set_flt2_coef_b0b1_value(uint32_t value)
{
	REG_WRITE(AD_FLT_2_COEF_2,value);
}

static inline uint32_t aud_ll_get_flt2_coef_b0b1_flt2_b0(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_2_COEF_2);
	reg_value = ((reg_value >> FLT_2_B0_POSI) & FLT_2_B0_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt2_coef_b0b1_flt2_b0(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_2_COEF_2);
	reg_value &= ~(FLT_2_B0_MASK << FLT_2_B0_POSI);
	reg_value |= ((value & FLT_2_B0_MASK) << FLT_2_B0_POSI);
	REG_WRITE(AD_FLT_2_COEF_2,reg_value);
}

static inline uint32_t aud_ll_get_flt2_coef_b0b1_flt2_b1(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_2_COEF_2);
	reg_value = ((reg_value >> FLT_2_B1_POSI) & FLT_2_B1_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt2_coef_b0b1_flt2_b1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_2_COEF_2);
	reg_value &= ~(FLT_2_B1_MASK << FLT_2_B1_POSI);
	reg_value |= ((value & FLT_2_B1_MASK) << FLT_2_B1_POSI);
	REG_WRITE(AD_FLT_2_COEF_2,reg_value);
}

static inline uint32_t aud_ll_get_flt2_coef_b2_value(void)
{
	return REG_READ(AD_FLT_2_COEF_3);
}

static inline void aud_ll_set_flt2_coef_b2_value(uint32_t value)
{
	REG_WRITE(AD_FLT_2_COEF_3,value);
}

static inline uint32_t aud_ll_get_flt2_coef_b2_flt2_b2(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_2_COEF_3);
	reg_value = ((reg_value >> FLT_2_B2_POSI) & FLT_2_B2_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt2_coef_b2_flt2_b2(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_2_COEF_3);
	reg_value &= ~(FLT_2_B2_MASK << FLT_2_B2_POSI);
	reg_value |= ((value & FLT_2_B2_MASK) << FLT_2_B2_POSI);
	REG_WRITE(AD_FLT_2_COEF_3,reg_value);
}

static inline uint32_t aud_ll_get_flt3_coef_a1a2_value(void)
{
	return REG_READ(AUD_FLT_3_COEF_1);
}

static inline void aud_ll_set_flt3_coef_a1a2_value(uint32_t value)
{
	REG_WRITE(AUD_FLT_3_COEF_1,value);
}

static inline uint32_t aud_ll_get_flt3_coef_a1a2_flt3_a1(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_COEF_1);
	reg_value = ((reg_value >> FLT_3_A1_POSI) & FLT_3_A1_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt3_coef_a1a2_flt3_a1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_COEF_1);
	reg_value &= ~(FLT_3_A1_MASK << FLT_3_A1_POSI);
	reg_value |= ((value & FLT_3_A1_MASK) << FLT_3_A1_POSI);
	REG_WRITE(AUD_FLT_3_COEF_1,reg_value);
}

static inline uint32_t aud_ll_get_flt3_coef_a1a2_flt3_a2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT_3_COEF_1);
    reg_value = ((reg_value >> FLT_3_A2_POSI) & FLT_3_A2_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt3_coef_a1a2_flt3_a2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT_3_COEF_1);
    reg_value &= ~(FLT_3_A2_MASK << FLT_3_A2_POSI);
    reg_value |= ((value & FLT_3_A2_MASK) << FLT_3_A2_POSI);
    REG_WRITE(AUD_FLT_3_COEF_1,reg_value);
}

static inline uint32_t aud_ll_get_flt3_coef_b0b1_value(void)
{
	return REG_READ(AD_FLT_3_COEF_2);
}

static inline void aud_ll_set_flt3_coef_b0b1_value(uint32_t value)
{
	REG_WRITE(AD_FLT_3_COEF_2,value);
}

static inline uint32_t aud_ll_get_flt3_coef_b0b1_flt3_b0(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_3_COEF_2);
	reg_value = ((reg_value >> FLT_3_B0_POSI) & FLT_3_B0_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt3_coef_b0b1_flt3_b0(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_3_COEF_2);
	reg_value &= ~(FLT_3_B0_MASK << FLT_3_B0_POSI);
	reg_value |= ((value & FLT_3_B0_MASK) << FLT_3_B0_POSI);
	REG_WRITE(AD_FLT_3_COEF_2,reg_value);
}

static inline uint32_t aud_ll_get_flt3_coef_b0b1_flt3_b1(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_3_COEF_2);
	reg_value = ((reg_value >> FLT_3_B1_POSI) & FLT_3_B1_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt3_coef_b0b1_flt3_b1(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_3_COEF_2);
	reg_value &= ~(FLT_3_B1_MASK << FLT_3_B1_POSI);
	reg_value |= ((value & FLT_3_B1_MASK) << FLT_3_B1_POSI);
	REG_WRITE(AD_FLT_3_COEF_2,reg_value);
}

static inline uint32_t aud_ll_get_flt3_coef_b2_value(void)
{
	return REG_READ(AD_FLT_3_COEF_3);
}

static inline void aud_ll_set_flt3_coef_b2_value(uint32_t value)
{
	REG_WRITE(AD_FLT_3_COEF_3,value);
}

static inline uint32_t aud_ll_get_flt3_coef_b2_flt3_b2(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_3_COEF_3);
	reg_value = ((reg_value >> FLT_3_B2_POSI) & FLT_3_B2_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt3_coef_b2_flt3_b2(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AD_FLT_3_COEF_3);
	reg_value &= ~(FLT_3_B2_MASK << FLT_3_B2_POSI);
	reg_value |= ((value & FLT_3_B2_MASK) << FLT_3_B2_POSI);
	REG_WRITE(AD_FLT_3_COEF_3,reg_value);
}

static inline uint32_t aud_ll_get_flt0_ext_coef_value(void)
{
	return REG_READ(AUD_FLT_0_EXT_COEF);
}

static inline void aud_ll_set_flt0_ext_coef_value(uint32_t value)
{
	REG_WRITE(AUD_FLT_0_EXT_COEF,value);
}

static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_a1_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_EXT_COEF);
	reg_value = ((reg_value >> FLT_A1_L_6BIT_POSI) & FLT_A1_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_a1_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_EXT_COEF);
	reg_value &= ~(FLT_A1_L_6BIT_MASK << FLT_A1_L_6BIT_POSI);
	reg_value |= ((value & FLT_A1_L_6BIT_MASK) << FLT_A1_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_0_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_a2_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_EXT_COEF);
	reg_value = ((reg_value >> FLT_A2_L_6BIT_POSI) & FLT_A2_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_a2_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_EXT_COEF);
	reg_value &= ~(FLT_A2_L_6BIT_MASK << FLT_A2_L_6BIT_POSI);
	reg_value |= ((value & FLT_A2_L_6BIT_MASK) << FLT_A2_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_0_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_b0_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_EXT_COEF);
	reg_value = ((reg_value >> FLT_B0_L_6BIT_POSI) & FLT_B0_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_b0_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_EXT_COEF);
	reg_value &= ~(FLT_B0_L_6BIT_MASK << FLT_B0_L_6BIT_POSI);
	reg_value |= ((value & FLT_B0_L_6BIT_MASK) << FLT_B0_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_0_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_b1_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_EXT_COEF);
	reg_value = ((reg_value >> FLT_B1_L_6BIT_POSI) & FLT_B1_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_b1_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_EXT_COEF);
	reg_value &= ~(FLT_B1_L_6BIT_MASK << FLT_B1_L_6BIT_POSI);
	reg_value |= ((value & FLT_B1_L_6BIT_MASK) << FLT_B1_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_0_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_b2_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_EXT_COEF);
	reg_value = ((reg_value >> FLT_B2_L_6BIT_POSI) & FLT_B2_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_b2_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_0_EXT_COEF);
	reg_value &= ~(FLT_B2_L_6BIT_MASK << FLT_B2_L_6BIT_POSI);
	reg_value |= ((value & FLT_B2_L_6BIT_MASK) << FLT_B2_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_0_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt1_ext_coef_value(void)
{
	return REG_READ(AUD_FLT_1_EXT_COEF);
}

static inline void aud_ll_set_flt1_ext_coef_value(uint32_t value)
{
	REG_WRITE(AUD_FLT_1_EXT_COEF,value);
}

static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_a1_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_EXT_COEF);
	reg_value = ((reg_value >> FLT_A1_L_6BIT_POSI) & FLT_A1_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_a1_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_EXT_COEF);
	reg_value &= ~(FLT_A1_L_6BIT_MASK << FLT_A1_L_6BIT_POSI);
	reg_value |= ((value & FLT_A1_L_6BIT_MASK) << FLT_A1_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_1_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_a2_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_EXT_COEF);
	reg_value = ((reg_value >> FLT_A2_L_6BIT_POSI) & FLT_A2_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_a2_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_EXT_COEF);
	reg_value &= ~(FLT_A2_L_6BIT_MASK << FLT_A2_L_6BIT_POSI);
	reg_value |= ((value & FLT_A2_L_6BIT_MASK) << FLT_A2_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_1_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_b0_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_EXT_COEF);
	reg_value = ((reg_value >> FLT_B0_L_6BIT_POSI) & FLT_B0_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_b0_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_EXT_COEF);
	reg_value &= ~(FLT_B0_L_6BIT_MASK << FLT_B0_L_6BIT_POSI);
	reg_value |= ((value & FLT_B0_L_6BIT_MASK) << FLT_B0_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_1_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_b1_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_EXT_COEF);
	reg_value = ((reg_value >> FLT_B1_L_6BIT_POSI) & FLT_B1_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_b1_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_EXT_COEF);
	reg_value &= ~(FLT_B1_L_6BIT_MASK << FLT_B1_L_6BIT_POSI);
	reg_value |= ((value & FLT_B1_L_6BIT_MASK) << FLT_B1_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_1_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_b2_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_EXT_COEF);
	reg_value = ((reg_value >> FLT_B2_L_6BIT_POSI) & FLT_B2_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_b2_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_1_EXT_COEF);
	reg_value &= ~(FLT_B2_L_6BIT_MASK << FLT_B2_L_6BIT_POSI);
	reg_value |= ((value & FLT_B2_L_6BIT_MASK) << FLT_B2_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_1_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt2_ext_coef_value(void)
{
	return REG_READ(AUD_FLT_2_EXT_COEF);
}

static inline void aud_ll_set_flt2_ext_coef_value(uint32_t value)
{
	REG_WRITE(AUD_FLT_2_EXT_COEF,value);
}

static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_a1_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_EXT_COEF);
	reg_value = ((reg_value >> FLT_A1_L_6BIT_POSI) & FLT_A1_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_a1_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_EXT_COEF);
	reg_value &= ~(FLT_A1_L_6BIT_MASK << FLT_A1_L_6BIT_POSI);
	reg_value |= ((value & FLT_A1_L_6BIT_MASK) << FLT_A1_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_2_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_a2_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_EXT_COEF);
	reg_value = ((reg_value >> FLT_A2_L_6BIT_POSI) & FLT_A2_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_a2_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_EXT_COEF);
	reg_value &= ~(FLT_A2_L_6BIT_MASK << FLT_A2_L_6BIT_POSI);
	reg_value |= ((value & FLT_A2_L_6BIT_MASK) << FLT_A2_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_2_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_b0_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_EXT_COEF);
	reg_value = ((reg_value >> FLT_B0_L_6BIT_POSI) & FLT_B0_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_b0_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_EXT_COEF);
	reg_value &= ~(FLT_B0_L_6BIT_MASK << FLT_B0_L_6BIT_POSI);
	reg_value |= ((value & FLT_B0_L_6BIT_MASK) << FLT_B0_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_2_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_b1_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_EXT_COEF);
	reg_value = ((reg_value >> FLT_B1_L_6BIT_POSI) & FLT_B1_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_b1_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_EXT_COEF);
	reg_value &= ~(FLT_B1_L_6BIT_MASK << FLT_B1_L_6BIT_POSI);
	reg_value |= ((value & FLT_B1_L_6BIT_MASK) << FLT_B1_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_2_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_b2_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_EXT_COEF);
	reg_value = ((reg_value >> FLT_B2_L_6BIT_POSI) & FLT_B2_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_b2_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_2_EXT_COEF);
	reg_value &= ~(FLT_B2_L_6BIT_MASK << FLT_B2_L_6BIT_POSI);
	reg_value |= ((value & FLT_B2_L_6BIT_MASK) << FLT_B2_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_2_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt3_ext_coef_value(void)
{
	return REG_READ(AUD_FLT_3_EXT_COEF);
}

static inline void aud_ll_set_flt3_ext_coef_value(uint32_t value)
{
	REG_WRITE(AUD_FLT_3_EXT_COEF,value);
}

static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_a1_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_EXT_COEF);
	reg_value = ((reg_value >> FLT_A1_L_6BIT_POSI) & FLT_A1_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_a1_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_EXT_COEF);
	reg_value &= ~(FLT_A1_L_6BIT_MASK << FLT_A1_L_6BIT_POSI);
	reg_value |= ((value & FLT_A1_L_6BIT_MASK) << FLT_A1_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_3_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_a2_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_EXT_COEF);
	reg_value = ((reg_value >> FLT_A2_L_6BIT_POSI) & FLT_A2_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_a2_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_EXT_COEF);
	reg_value &= ~(FLT_A2_L_6BIT_MASK << FLT_A2_L_6BIT_POSI);
	reg_value |= ((value & FLT_A2_L_6BIT_MASK) << FLT_A2_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_3_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_b0_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_EXT_COEF);
	reg_value = ((reg_value >> FLT_B0_L_6BIT_POSI) & FLT_B0_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_b0_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_EXT_COEF);
	reg_value &= ~(FLT_B0_L_6BIT_MASK << FLT_B0_L_6BIT_POSI);
	reg_value |= ((value & FLT_B0_L_6BIT_MASK) << FLT_B0_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_3_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_b1_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_EXT_COEF);
	reg_value = ((reg_value >> FLT_B1_L_6BIT_POSI) & FLT_B1_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_b1_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_EXT_COEF);
	reg_value &= ~(FLT_B1_L_6BIT_MASK << FLT_B1_L_6BIT_POSI);
	reg_value |= ((value & FLT_B1_L_6BIT_MASK) << FLT_B1_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_3_EXT_COEF,reg_value);
}

static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_b2_l6bit(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_EXT_COEF);
	reg_value = ((reg_value >> FLT_B2_L_6BIT_POSI) & FLT_B2_L_6BIT_MASK);
	return reg_value;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_b2_l6bit(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AUD_FLT_3_EXT_COEF);
	reg_value &= ~(FLT_B2_L_6BIT_MASK << FLT_B2_L_6BIT_POSI);
	reg_value |= ((value & FLT_B2_L_6BIT_MASK) << FLT_B2_L_6BIT_POSI);
	REG_WRITE(AUD_FLT_3_EXT_COEF,reg_value);
}

#ifdef __cplusplus
}
#endif

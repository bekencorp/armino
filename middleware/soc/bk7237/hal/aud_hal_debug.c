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

#include "hal_config.h"
#include "aud_hw.h"
#include "aud_hal.h"
#include "aud_ll_macro_def.h"

#if CFG_HAL_DEBUG_AUD

void aud_struct_dump(void)
{
	SOC_LOGI("base=%x\r\n", (uint32_t)AUD_LL_REG_BASE);

	/* REG_0x00 */
	SOC_LOGI("  audio_config=0x%x value=0x%x\n", AUD_AUDIO_CONFIG_ADDR, aud_ll_get_audio_config_value());
	SOC_LOGI("    samp_rate_adc: %x\n", aud_ll_get_audio_config_samp_rate_adc());
	SOC_LOGI("    dac_enable:    %x\n", aud_ll_get_audio_config_dac_enable());
	SOC_LOGI("    adc_enable:    %x\n", aud_ll_get_audio_config_adc_enable());
	SOC_LOGI("    dtmf_enable:   %x\n", aud_ll_get_audio_config_dtmf_enable());
	SOC_LOGI("    line_enable:   %x\n", aud_ll_get_audio_config_line_enable());
	SOC_LOGI("    samp_rate_dac: %x\n", aud_ll_get_audio_config_samp_rate_dac());

	/* REG_0x01 */
	SOC_LOGI("\n");
	SOC_LOGI("  dtmf_config0=0x%x value=0x%x\n", AUD_DTMF_CONFIG0_ADDR, aud_ll_get_dtmf_config0_value());
	SOC_LOGI("    tone_pattern:     %x\n", aud_ll_get_dtmf_config0_tone_pattern());
	SOC_LOGI("    tone_mode:        %x\n", aud_ll_get_dtmf_config0_tone_mode());
	SOC_LOGI("    tone_pause_time:  %x\n", aud_ll_get_dtmf_config0_tone_pause_time());
	SOC_LOGI("    tone_active_time: %x\n", aud_ll_get_dtmf_config0_tone_active_time());

	/* REG_0x02 */
	SOC_LOGI("\n");
	SOC_LOGI("  dtmf_config1=0x%x value=0x%x\n", AUD_DTMF_CONFIG1_ADDR, aud_ll_get_dtmf_config1_value());
	SOC_LOGI("    tone1_step:   %x\n", aud_ll_get_dtmf_config1_tone1_step());
	SOC_LOGI("    tone1_attu:   %x\n", aud_ll_get_dtmf_config1_tone1_attu());
	SOC_LOGI("    tone1_enable: %x\n", aud_ll_get_dtmf_config1_tone1_enable());

	/* REG_0x03 */
	SOC_LOGI("\n");
	SOC_LOGI("  dtmf_config2=0x%x value=0x%x\n", AUD_DTMF_CONFIG2_ADDR, aud_ll_get_dtmf_config2_value());
	SOC_LOGI("    tone2_step:   %x\n", aud_ll_get_dtmf_config2_tone2_step());
	SOC_LOGI("    tone2_attu:   %x\n", aud_ll_get_dtmf_config2_tone2_attu());
	SOC_LOGI("    tone2_enable: %x\n", aud_ll_get_dtmf_config2_tone2_enable());

	/* REG_0x04 */
	SOC_LOGI("\n");
	SOC_LOGI("  adc_config0=0x%x value=0x%x\n", AUD_ADC_CONFIG0_ADDR, aud_ll_get_adc_config0_value());
	SOC_LOGI("    adc_hpf2_coef_b2: %x\n", aud_ll_get_adc_config0_adc_hpf2_coef_b2());
	SOC_LOGI("    adc_hpf2_bypass:  %x\n", aud_ll_get_adc_config0_adc_hpf2_bypass());
	SOC_LOGI("    adc_hpf1_bypass:  %x\n", aud_ll_get_adc_config0_adc_hpf1_bypass());
	SOC_LOGI("    adc_set_gain:     %x\n", aud_ll_get_adc_config0_adc_set_gain());
	SOC_LOGI("    adc_sampe_edge:   %x\n", aud_ll_get_adc_config0_adc_sampe_edge());

	/* REG_0x05 */
	SOC_LOGI("\n");
	SOC_LOGI("  adc_config1=0x%x value=0x%x\n", AUD_ADC_CONFIG1_ADDR, aud_ll_get_adc_config1_value());
	SOC_LOGI("    adc_hpf2_coef_b0: %x\n", aud_ll_get_adc_config1_adc_hpf2_coef_b0());
	SOC_LOGI("    adc_hpf2_coef_b1: %x\n", aud_ll_get_adc_config1_adc_hpf2_coef_b1());

	/* REG_0x06 */
	SOC_LOGI("\n");
	SOC_LOGI("  adc_config2=0x%x value=0x%x\n", AUD_ADC_CONFIG2_ADDR, aud_ll_get_adc_config2_value());
	SOC_LOGI("    adc_hpf2_coef_a0: %x\n", aud_ll_get_adc_config2_adc_hpf2_coef_a0());
	SOC_LOGI("    adc_hpf2_coef_a1: %x\n", aud_ll_get_adc_config2_adc_hpf2_coef_a1());

	/* REG_0x07 */
	SOC_LOGI("\n");
	SOC_LOGI("  dac_config0=0x%x value=0x%x\n", AUD_DAC_CONFIG0_ADDR, aud_ll_get_dac_config0_value());
	SOC_LOGI("    dac_hpf2_coef_b2: %x\n", aud_ll_get_dac_config0_dac_hpf2_coef_b2());
	SOC_LOGI("    dac_hpf2_bypass:  %x\n", aud_ll_get_dac_config0_dac_hpf2_bypass());
	SOC_LOGI("    dac_hpf1_bypass:  %x\n", aud_ll_get_dac_config0_dac_hpf1_bypass());
	SOC_LOGI("    dac_set_gain:     %x\n", aud_ll_get_dac_config0_dac_set_gain());
	SOC_LOGI("    dac_clk_invert:   %x\n", aud_ll_get_dac_config0_dac_clk_invert());

	/* REG_0x08 */
	SOC_LOGI("\n");
	SOC_LOGI("  dac_config1=0x%x value=0x%x\n", AUD_DAC_CONFIG1_ADDR, aud_ll_get_dac_config1_value());
	SOC_LOGI("    dac_hpf2_coef_b0: %x\n", aud_ll_get_dac_config1_dac_hpf2_coef_b0());
	SOC_LOGI("    dac_hpf2_coef_b1: %x\n", aud_ll_get_dac_config1_dac_hpf2_coef_b1());

	/* REG_0x09 */
	SOC_LOGI("\n");
	SOC_LOGI("  dac_config2=0x%x value=0x%x\n", AUD_DAC_CONFIG2_ADDR, aud_ll_get_dac_config2_value());
	SOC_LOGI("    dac_hpf2_coef_a1: %x\n", aud_ll_get_dac_config2_dac_hpf2_coef_a1());
	SOC_LOGI("    dac_hpf2_coef_a2: %x\n", aud_ll_get_dac_config2_dac_hpf2_coef_a2());

	/* REG_0x0a */
	SOC_LOGI("  fifo_config=0x%x value=0x%x\n", AUD_FIFO_CONFIG_ADDR, aud_ll_get_fifo_config_value());
	SOC_LOGI("    dacr_rd_threshold: %x\n", aud_ll_get_fifo_config_dacr_rd_threshold());
	SOC_LOGI("    dacl_rd_threshold: %x\n", aud_ll_get_fifo_config_dacl_rd_threshold());
	SOC_LOGI("    dtmf_wr_threshold: %x\n", aud_ll_get_fifo_config_dtmf_wr_threshold());
	SOC_LOGI("    adcl_wr_threshold: %x\n", aud_ll_get_fifo_config_adcl_wr_threshold());
	SOC_LOGI("    dacr_int_en:       %x\n", aud_ll_get_fifo_config_dacr_int_en());
	SOC_LOGI("    dacl_int_en:       %x\n", aud_ll_get_fifo_config_dacl_int_en());
	SOC_LOGI("    dtmf_int_en:       %x\n", aud_ll_get_fifo_config_dtmf_int_en());
	SOC_LOGI("    adcl_int_en:       %x\n", aud_ll_get_fifo_config_adcl_int_en());
	SOC_LOGI("    loop_ton2dac:      %x\n", aud_ll_get_fifo_config_loop_ton2dac());
	SOC_LOGI("    loop_adc2dac:      %x\n", aud_ll_get_fifo_config_loop_adc2dac());

	/* REG_0x0b */
	SOC_LOGI("\n");
	SOC_LOGI("  agc_config0=0x%x value=0x%x\n", AUD_AGC_CONFIG0_ADDR, aud_ll_get_agc_config0_value());
	SOC_LOGI("    agc_noise_thrd: %x\n", aud_ll_get_agc_config0_agc_noise_thrd());
	SOC_LOGI("    agc_noise_high: %x\n", aud_ll_get_agc_config0_agc_noise_high());
	SOC_LOGI("    agc_noise_low:  %x\n", aud_ll_get_agc_config0_agc_noise_low());
	SOC_LOGI("    agc_step:       %x\n", aud_ll_get_agc_config0_agc_step());

	/* REG_0x0c */
	SOC_LOGI("  agc_config1=0x%x value=0x%x\n", AUD_AGC_CONFIG1_ADDR, aud_ll_get_agc_config1_value());
	SOC_LOGI("    agc_noise_min:  %x\n", aud_ll_get_agc_config1_agc_noise_min());
	SOC_LOGI("    agc_noise_tout: %x\n", aud_ll_get_agc_config1_agc_noise_tout());
	SOC_LOGI("    agc_high_dur:   %x\n", aud_ll_get_agc_config1_agc_high_dur());
	SOC_LOGI("    agc_low_dur:    %x\n", aud_ll_get_agc_config1_agc_low_dur());
	SOC_LOGI("    agc_min:        %x\n", aud_ll_get_agc_config1_agc_min());
	SOC_LOGI("    agc_max:        %x\n", aud_ll_get_agc_config1_agc_max());
	SOC_LOGI("    agc_ng_method:  %x\n", aud_ll_get_agc_config1_agc_ng_method());
	SOC_LOGI("    agc_ng_enable:  %x\n", aud_ll_get_agc_config1_agc_ng_enable());

	/* REG_0x0d */
	SOC_LOGI("  agc_config2=0x%x value=0x%x\n", AUD_AGC_CONFIG2_ADDR, aud_ll_get_agc_config2_value());
	SOC_LOGI("    agc_decay_time:   %x\n", aud_ll_get_agc_config2_agc_decay_time());
	SOC_LOGI("    agc_attack_time:  %x\n", aud_ll_get_agc_config2_agc_attack_time());
	SOC_LOGI("    agc_high_thrd:    %x\n", aud_ll_get_agc_config2_agc_high_thrd());
	SOC_LOGI("    agc_low_thrd:     %x\n", aud_ll_get_agc_config2_agc_low_thrd());
	SOC_LOGI("    agc_iir_coef:     %x\n", aud_ll_get_agc_config2_agc_iir_coef());
	SOC_LOGI("    agc_enable:       %x\n", aud_ll_get_agc_config2_agc_enable());
	SOC_LOGI("    manual_pga_value: %x\n", aud_ll_get_agc_config2_manual_pga_value());
	SOC_LOGI("    manual_pga:       %x\n", aud_ll_get_agc_config2_manual_pga());

	/* REG_0x0e */
	SOC_LOGI("  fifo_status=0x%x value=0x%x\n", AUD_FIFO_STATUS_ADDR, aud_ll_get_fifo_status_value());
	SOC_LOGI("    dacr_near_full:  %x\n", aud_ll_get_fifo_status_dacr_near_full());
	SOC_LOGI("    dacl_near_full:  %x\n", aud_ll_get_fifo_status_dacl_near_full());
	SOC_LOGI("    adcl_near_full:  %x\n", aud_ll_get_fifo_status_adcl_near_full());
	SOC_LOGI("    dtmf_near_full:  %x\n", aud_ll_get_fifo_status_dtmf_near_full());
	SOC_LOGI("    dacr_near_empty: %x\n", aud_ll_get_fifo_status_dacr_near_empty());
	SOC_LOGI("    dacl_near_empty: %x\n", aud_ll_get_fifo_status_dacl_near_empty());
	SOC_LOGI("    adcl_near_empty: %x\n", aud_ll_get_fifo_status_adcl_near_empty());
	SOC_LOGI("    dtmf_near_empty: %x\n", aud_ll_get_fifo_status_dtmf_near_empty());
	SOC_LOGI("    dacr_fifo_full:  %x\n", aud_ll_get_fifo_status_dacr_fifo_full());
	SOC_LOGI("    dacl_fifo_full:  %x\n", aud_ll_get_fifo_status_dacl_fifo_full());
	SOC_LOGI("    adcl_fifo_full:  %x\n", aud_ll_get_fifo_status_adcl_fifo_full());
	SOC_LOGI("    dtmf_fifo_full:  %x\n", aud_ll_get_fifo_status_dtmf_fifo_full());
	SOC_LOGI("    dacr_fifo_empty: %x\n", aud_ll_get_fifo_status_dacr_fifo_empty());
	SOC_LOGI("    dacl_fifo_empty: %x\n", aud_ll_get_fifo_status_dacl_fifo_empty());
	SOC_LOGI("    adcl_fifo_empty: %x\n", aud_ll_get_fifo_status_adcl_fifo_empty());
	SOC_LOGI("    dtmf_fifo_empty: %x\n", aud_ll_get_fifo_status_dtmf_fifo_empty());
	SOC_LOGI("    dacr_int_flag:   %x\n", aud_ll_get_fifo_status_dacr_int_flag());
	SOC_LOGI("    dacl_int_flag:   %x\n", aud_ll_get_fifo_status_dacl_int_flag());
	SOC_LOGI("    adcl_int_flag:   %x\n", aud_ll_get_fifo_status_adcl_int_flag());
	SOC_LOGI("    dtmf_int_flag:   %x\n", aud_ll_get_fifo_status_dtmf_int_flag());

	/* REG_0x0f */
	SOC_LOGI("\n");
	SOC_LOGI("  agc_status=0x%x value=0x%x\n", AUD_AGC_STATUS_ADDR, aud_ll_get_agc_status_value());
	SOC_LOGI("    rssi:     %x\n", aud_ll_get_agc_status_rssi());
	SOC_LOGI("    mic_pga:  %x\n", aud_ll_get_agc_status_mic_pga());
	SOC_LOGI("    mic_rssi: %x\n", aud_ll_get_agc_status_mic_rssi());

	/* REG_0x10 */
	SOC_LOGI("\n");
	SOC_LOGI("  dtmf_fport=0x%x value=0x%x\n", AUD_DTMF_FPORT_ADDR, aud_ll_get_dtmf_fport_value());
	SOC_LOGI("    dtmf_port: %x\n", aud_ll_get_dtmf_fport_dtmf_port());

	/* REG_0x11 */
	SOC_LOGI("\n");
	SOC_LOGI("  adc_fport=0x%x value=0x%x\n", AUD_ADC_FPORT_ADDR, aud_ll_get_adc_fport_value());
	SOC_LOGI("    adc_port: %x\n", aud_ll_get_adc_fport_adc_port());

	/* REG_0x12 */
	/*
	SOC_LOGI("\n");
	SOC_LOGI("  dac_fport=0x%x value=0x%x\n", AUD_DACL_FPORT_ADDR, hw->dac_fport.v);
	SOC_LOGI("    dacl_port: %x\n", hw->dac_fport.dacl_port);
	SOC_LOGI("    dacr_port: %x\n", hw->dac_fport.dacr_port);
	*/

	/* REG_0x18 */
	SOC_LOGI("\n");
	SOC_LOGI("  extend_cfg=0x%x value=0x%x\n", AUD_EXTEND_CFG_ADDR, aud_ll_get_extend_cfg_value());
	SOC_LOGI("    dac_fracmod_manual: %x\n", aud_ll_get_extend_cfg_dac_fracmod_manual());
	SOC_LOGI("    adc_fracmod_manual: %x\n", aud_ll_get_extend_cfg_adc_fracmod_manual());
	SOC_LOGI("    filt_enable:        %x\n", aud_ll_get_extend_cfg_filt_enable());

	/* REG_0x19 */
	SOC_LOGI("\n");
	SOC_LOGI("  dac_fracmod=0x%x value=0x%x\n", AUD_DAC_FRACMOD_ADDR, aud_ll_get_dac_fracmod_value());
	SOC_LOGI("    dac_fraccoef: %x\n", aud_ll_get_dac_fracmod_dac_fraccoef());

	/* REG_0x1a */
	SOC_LOGI("\n");
	SOC_LOGI("  adc_fracmod=0x%x value=0x%x\n", AUD_ADC_FRACMOD_ADDR, aud_ll_get_adc_fracmod_value());
	SOC_LOGI("    adc_fraccoef: %x\n", aud_ll_get_adc_fracmod_adc_fraccoef());

	/* REG_0x1f */
	SOC_LOGI("  hpf2_ext_coef=0x%x value=0x%x\n", AUD_HPF2_EXT_COEF_ADDR, aud_ll_get_hpf2_ext_coef_value());
	SOC_LOGI("    hpf2_a1_l6bit: %x\n", aud_ll_get_hpf2_ext_coef_hpf2_a1_l6bit());
	SOC_LOGI("    hpf2_a2_l6bit: %x\n", aud_ll_get_hpf2_ext_coef_hpf2_a2_l6bit());
	SOC_LOGI("    hpf2_b0_l6bit: %x\n", aud_ll_get_hpf2_ext_coef_hpf2_b0_l6bit());
	SOC_LOGI("    hpf2_b1_l6bit: %x\n", aud_ll_get_hpf2_ext_coef_hpf2_b1_l6bit());
	SOC_LOGI("    hpf2_b2_l6bit: %x\n", aud_ll_get_hpf2_ext_coef_hpf2_b2_l6bit());

	/* REG_0x20 */
	SOC_LOGI("\n");
	SOC_LOGI("  flt0_coef_a1a2=0x%x value=0x%x\n", AUD_FLT0_COEF_A1A2_ADDR, aud_ll_get_flt0_coef_a1a2_value());
	SOC_LOGI("    flt0_a1: %x\n", aud_ll_get_flt0_coef_a1a2_flt0_a1());
	SOC_LOGI("    flt0_a2: %x\n", aud_ll_get_flt0_coef_a1a2_flt0_a2());

	/* REG_0x21 */
	SOC_LOGI("\n");
	SOC_LOGI("  flt0_coef_b0b1=0x%x value=0x%x\n", AUD_FLT0_COEF_B0B1_ADDR, aud_ll_get_flt0_coef_b0b1_value());
	SOC_LOGI("    flt0_b0: %x\n", aud_ll_get_flt0_coef_b0b1_flt0_b0());
	SOC_LOGI("    flt0_b1: %x\n", aud_ll_get_flt0_coef_b0b1_flt0_b1());

	/* REG_0x22 */
	SOC_LOGI("\n");
	SOC_LOGI("  flt0_coef_b2=0x%x value=0x%x\n", AUD_FLT0_COEF_B2_ADDR, aud_ll_get_flt0_coef_b2_value());
	SOC_LOGI("    flt0_b2: %x\n", aud_ll_get_flt0_coef_b2_flt0_b2());

	/* REG_0x23 */
	SOC_LOGI("\n");
	SOC_LOGI("  flt1_coef_a1a2=0x%x value=0x%x\n", AUD_FLT1_COEF_A1A2_ADDR, aud_ll_get_flt1_coef_a1a2_value());
	SOC_LOGI("    flt1_a1: %x\n", aud_ll_get_flt1_coef_a1a2_flt1_a1());
	SOC_LOGI("    flt1_a2: %x\n", aud_ll_get_flt1_coef_a1a2_flt1_a2());

	/* REG_0x24 */
	SOC_LOGI("\n");
	SOC_LOGI("  flt1_coef_b0b1=0x%x value=0x%x\n", AUD_FLT1_COEF_B0B1_ADDR, aud_ll_get_flt1_coef_b0b1_value());
	SOC_LOGI("    flt1_b0: %x\n", aud_ll_get_flt1_coef_b0b1_flt1_b0());
	SOC_LOGI("    flt1_b1: %x\n", aud_ll_get_flt1_coef_b0b1_flt1_b1());

	/* REG_0x25 */
	SOC_LOGI("\n");
	SOC_LOGI("  flt1_coef_b2=0x%x value=0x%x\n", AUD_FLT1_COEF_B2_ADDR, aud_ll_get_flt1_coef_b2_value());
	SOC_LOGI("    flt1_b2: %x\n", aud_ll_get_flt1_coef_b2_flt1_b2());

	/* REG_0x26 */
	SOC_LOGI("\n");
	SOC_LOGI("  flt2_coef_a1a2=0x%x value=0x%x\n", AUD_FLT2_COEF_A1A2_ADDR, aud_ll_get_flt2_coef_a1a2_value());
	SOC_LOGI("    flt2_a1: %x\n", aud_ll_get_flt2_coef_a1a2_flt2_a1());
	SOC_LOGI("    flt2_a2: %x\n", aud_ll_get_flt2_coef_a1a2_flt2_a2());

	/* REG_0x27 */
	SOC_LOGI("\n");
	SOC_LOGI("  flt2_coef_b0b1=0x%x value=0x%x\n", AUD_FLT2_COEF_B0B1_ADDR, aud_ll_get_flt2_coef_b0b1_value());
	SOC_LOGI("    flt2_b0: %x\n", aud_ll_get_flt2_coef_b0b1_flt2_b0());
	SOC_LOGI("    flt2_b1: %x\n", aud_ll_get_flt2_coef_b0b1_flt2_b1());

	/* REG_0x28 */
	SOC_LOGI("\n");
	SOC_LOGI("  flt2_coef_b2=0x%x value=0x%x\n", AUD_FLT2_COEF_B2_ADDR, aud_ll_get_flt2_coef_b2_value());
	SOC_LOGI("    flt2_b2: %x\n", aud_ll_get_flt2_coef_b2_flt2_b2());

	/* REG_0x29 */
	SOC_LOGI("\n");
	SOC_LOGI("  flt3_coef_a1a2=0x%x value=0x%x\n", AUD_FLT3_COEF_A1A2_ADDR, aud_ll_get_flt3_coef_a1a2_value());
	SOC_LOGI("    flt3_a1: %x\n", aud_ll_get_flt3_coef_a1a2_flt3_a1());
	SOC_LOGI("    flt3_a2: %x\n", aud_ll_get_flt3_coef_a1a2_flt3_a2());

	/* REG_0x2a */
	SOC_LOGI("\n");
	SOC_LOGI("  flt3_coef_b0b1=0x%x value=0x%x\n", AUD_FLT3_COEF_B0B1_ADDR, aud_ll_get_flt3_coef_b0b1_value());
	SOC_LOGI("    flt3_b0: %x\n", aud_ll_get_flt3_coef_b0b1_flt3_b0());
	SOC_LOGI("    flt3_b1: %x\n", aud_ll_get_flt3_coef_b0b1_flt3_b1());

	/* REG_0x2b */
	SOC_LOGI("\n");
	SOC_LOGI("  flt3_coef_b2=0x%x value=0x%x\n", AUD_FLT3_COEF_B2_ADDR, aud_ll_get_flt3_coef_b2_value());
	SOC_LOGI("    flt3_b2: %x\n", aud_ll_get_flt3_coef_b2_flt3_b2());

	/* REG_0x2c */
	SOC_LOGI("  flt0_ext_coef=0x%x value=0x%x\n", AUD_FLT0_EXT_COEF_ADDR, aud_ll_get_flt0_ext_coef_value());
	SOC_LOGI("    flt0_a1_l6bit: %x\n", aud_ll_get_flt0_ext_coef_flt0_a1_l6bit());
	SOC_LOGI("    flt0_a2_l6bit: %x\n", aud_ll_get_flt0_ext_coef_flt0_a2_l6bit());
	SOC_LOGI("    flt0_b0_l6bit: %x\n", aud_ll_get_flt0_ext_coef_flt0_b0_l6bit());
	SOC_LOGI("    flt0_b1_l6bit: %x\n", aud_ll_get_flt0_ext_coef_flt0_b1_l6bit());
	SOC_LOGI("    flt0_b2_l6bit: %x\n", aud_ll_get_flt0_ext_coef_flt0_b2_l6bit());

	/* REG_0x2d */
	SOC_LOGI("  flt1_ext_coef=0x%x value=0x%x\n", AUD_FLT1_EXT_COEF_ADDR, aud_ll_get_flt1_ext_coef_value());
	SOC_LOGI("    flt1_a1_l6bit: %x\n", aud_ll_get_flt1_ext_coef_flt1_a1_l6bit());
	SOC_LOGI("    flt1_a2_l6bit: %x\n", aud_ll_get_flt1_ext_coef_flt1_a2_l6bit());
	SOC_LOGI("    flt1_b0_l6bit: %x\n", aud_ll_get_flt1_ext_coef_flt1_b0_l6bit());
	SOC_LOGI("    flt1_b1_l6bit: %x\n", aud_ll_get_flt1_ext_coef_flt1_b1_l6bit());
	SOC_LOGI("    flt1_b2_l6bit: %x\n", aud_ll_get_flt1_ext_coef_flt1_b2_l6bit());

	/* REG_0x2e */
	SOC_LOGI("  flt2_ext_coef=0x%x value=0x%x\n", AUD_FLT2_EXT_COEF_ADDR, aud_ll_get_flt2_ext_coef_value());
	SOC_LOGI("    flt2_a1_l6bit: %x\n", aud_ll_get_flt2_ext_coef_flt2_a1_l6bit());
	SOC_LOGI("    flt2_a2_l6bit: %x\n", aud_ll_get_flt2_ext_coef_flt2_a2_l6bit());
	SOC_LOGI("    flt2_b0_l6bit: %x\n", aud_ll_get_flt2_ext_coef_flt2_b0_l6bit());
	SOC_LOGI("    flt2_b1_l6bit: %x\n", aud_ll_get_flt2_ext_coef_flt2_b1_l6bit());
	SOC_LOGI("    flt2_b2_l6bit: %x\n", aud_ll_get_flt2_ext_coef_flt2_b2_l6bit());

	/* REG_0x2f */
	SOC_LOGI("  flt3_ext_coef=0x%x value=0x%x\n", AUD_FLT3_EXT_COEF_ADDR, aud_ll_get_flt3_ext_coef_value());
	SOC_LOGI("    flt3_a1_l6bit: %x\n", aud_ll_get_flt3_ext_coef_flt3_a1_l6bit());
	SOC_LOGI("    flt3_a2_l6bit: %x\n", aud_ll_get_flt3_ext_coef_flt3_a2_l6bit());
	SOC_LOGI("    flt3_b0_l6bit: %x\n", aud_ll_get_flt3_ext_coef_flt3_b0_l6bit());
	SOC_LOGI("    flt3_b1_l6bit: %x\n", aud_ll_get_flt3_ext_coef_flt3_b1_l6bit());
	SOC_LOGI("    flt3_b2_l6bit: %x\n", aud_ll_get_flt3_ext_coef_flt3_b2_l6bit());

}

#endif


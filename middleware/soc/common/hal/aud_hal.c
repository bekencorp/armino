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


#include <common/bk_include.h>
#include "aud_hal.h"
#include "aud_ll_macro_def.h"
#include <driver/aud_types.h>


/* 1. config dtmf registers
   2. set interrupt
   3. set enable
 */
bk_err_t aud_hal_dtmf_config(const aud_dtmf_config_t *config)
{
	/* audio_config */
	aud_ll_set_audio_config_adc_enable(config->adc_enable);
	aud_ll_set_audio_config_line_enable(config->dtmf_enable);
	aud_ll_set_audio_config_dtmf_enable(config->dtmf_enable);

	/* dtmf_config0 */
	aud_ll_set_dtmf_config0_tone_pattern(config->tone_pattern);
	aud_ll_set_dtmf_config0_tone_mode(config->tone_mode);
	aud_ll_set_dtmf_config0_tone_pause_time(config->tone_pause_time);
	aud_ll_set_dtmf_config0_tone_active_time(config->tone_active_time);

	/* dtmf_config1 */
	aud_ll_set_dtmf_config1_tone1_step(config->tone1_step);
	aud_ll_set_dtmf_config1_tone1_attu(config->tone1_attu);
	aud_ll_set_dtmf_config1_tone1_enable(config->tone1_enable);

	/* dtmf_config2 */
	aud_ll_set_dtmf_config2_tone2_step(config->tone2_step);
	aud_ll_set_dtmf_config2_tone2_attu(config->tone2_attu);
	aud_ll_set_dtmf_config2_tone2_enable(config->tone2_enable);

	/* fifo_config */
	aud_ll_set_fifo_config_dtmf_wr_threshold(config->dtmf_wr_threshold);
	aud_ll_set_fifo_config_dtmf_int_en(config->dtmf_int_enable);
	aud_ll_set_fifo_config_loop_ton2dac(config->loop_ton2dac);

	aud_ll_set_audio_config_adc_enable(config->adc_enable);
	aud_ll_set_audio_config_line_enable(config->dtmf_enable);
	aud_ll_set_audio_config_dtmf_enable(config->dtmf_enable);

	/* dtmf_config0 */
	aud_ll_set_dtmf_config0_tone_pattern(config->tone_pattern);
	aud_ll_set_dtmf_config0_tone_mode(config->tone_mode);
	aud_ll_set_dtmf_config0_tone_pause_time(config->tone_pause_time);
	aud_ll_set_dtmf_config0_tone_active_time(config->tone_active_time);

	/* dtmf_config1 */
	aud_ll_set_dtmf_config1_tone1_step(config->tone1_step);
	aud_ll_set_dtmf_config1_tone1_attu(config->tone1_attu);
	aud_ll_set_dtmf_config1_tone1_enable(config->tone1_enable);

	/* dtmf_config2 */
	aud_ll_set_dtmf_config2_tone2_step(config->tone2_step);
	aud_ll_set_dtmf_config2_tone2_attu(config->tone2_attu);
	aud_ll_set_dtmf_config2_tone2_enable(config->tone2_enable);

	/* fifo_config */
	aud_ll_set_fifo_config_dtmf_wr_threshold(config->dtmf_wr_threshold);
	aud_ll_set_fifo_config_dtmf_int_en(config->dtmf_int_enable);
	aud_ll_set_fifo_config_loop_ton2dac(config->loop_ton2dac);

	return BK_OK;
}

/* 1. config adc registers
   2. set interrupt
   3. set enable
 */
bk_err_t aud_hal_adc_config(const aud_adc_config_t *config)
{
	/* audio_config */
	aud_ll_set_audio_config_samp_rate_adc(config->samp_rate);
	aud_ll_set_audio_config_adc_enable(config->adc_enable);
	aud_ll_set_audio_config_line_enable(config->dtmf_enable);
	aud_ll_set_audio_config_dtmf_enable(config->dtmf_enable);

	/* adc_config0 */
	aud_ll_set_adc_config0_adc_hpf2_coef_b2(config->adc_hpf2_coef_B2);
	aud_ll_set_adc_config0_adc_hpf2_bypass(config->adc_hpf2_bypass_enable);
	aud_ll_set_adc_config0_adc_hpf1_bypass(config->adc_hpf1_bypass_enable);
	aud_ll_set_adc_config0_adc_set_gain(config->adc_set_gain);
	aud_ll_set_adc_config0_adc_sampe_edge(config->adc_samp_edge);

	/* adc_config1 */
	aud_ll_set_adc_config1_adc_hpf2_coef_b0(config->adc_hpf2_coef_B0);
	aud_ll_set_adc_config1_adc_hpf2_coef_b1(config->adc_hpf2_coef_B1);

	/* adc_config2 */
	aud_ll_set_adc_config2_adc_hpf2_coef_a0(config->adc_hpf2_coef_A0);
	aud_ll_set_adc_config2_adc_hpf2_coef_a1(config->adc_hpf2_coef_A1);

	/* fifo_config */
	aud_ll_set_fifo_config_dtmf_wr_threshold(config->dtmf_wr_threshold);
	aud_ll_set_fifo_config_adcl_wr_threshold(config->adcl_wr_threshold);
	aud_ll_set_fifo_config_dtmf_int_en(config->dtmf_int_enable);
	aud_ll_set_fifo_config_adcl_int_en(config->adcl_int_enable);
	aud_ll_set_fifo_config_loop_adc2dac(config->loop_adc2dac);

	/* agc_config0 */
	aud_ll_set_agc_config0_agc_noise_thrd(config->agc_noise_thrd);
	aud_ll_set_agc_config0_agc_noise_high(config->agc_noise_high);
	aud_ll_set_agc_config0_agc_noise_low(config->agc_noise_low);

	/* agc_config1 */
	aud_ll_set_agc_config1_agc_noise_min(config->agc_noise_min);
	aud_ll_set_agc_config1_agc_noise_tout(config->agc_noise_tout);
	aud_ll_set_agc_config1_agc_high_dur(config->agc_high_dur);
	aud_ll_set_agc_config1_agc_low_dur(config->agc_low_dur);
	aud_ll_set_agc_config1_agc_min(config->agc_min);
	aud_ll_set_agc_config1_agc_max(config->agc_max);
	aud_ll_set_agc_config1_agc_ng_method(config->agc_ng_method);
	aud_ll_set_agc_config1_agc_ng_enable(config->agc_ng_enable);

	/* agc_config2 */
	aud_ll_set_agc_config2_agc_decay_time(config->agc_decay_time);
	aud_ll_set_agc_config2_agc_attack_time(config->agc_attack_time);
	aud_ll_set_agc_config2_agc_high_thrd(config->agc_high_thrd);
	aud_ll_set_agc_config2_agc_low_thrd(config->agc_low_thrd);
	aud_ll_set_agc_config2_agc_iir_coef(config->agc_iir_coef);
	aud_ll_set_agc_config2_agc_enable(config->agc_enable);
	aud_ll_set_agc_config2_manual_pga_value(config->manual_pga_value);
	aud_ll_set_agc_config2_manual_pga(config->manual_pga_enable);

	/* extend_config */
	aud_ll_set_extend_cfg_adc_fracmod_manual(config->adc_fracmod_manual);
	aud_ll_set_adc_fracmod_value(config->adc_fracmod);

	aud_ll_set_audio_config_samp_rate_adc(config->samp_rate);
	aud_ll_set_audio_config_adc_enable(config->adc_enable);
	aud_ll_set_audio_config_line_enable(config->dtmf_enable);
	aud_ll_set_audio_config_dtmf_enable(config->dtmf_enable);

	/* adc_config0 */
	aud_ll_set_adc_config0_adc_hpf2_coef_b2(config->adc_hpf2_coef_B2);
	aud_ll_set_adc_config0_adc_hpf2_bypass(config->adc_hpf2_bypass_enable);
	aud_ll_set_adc_config0_adc_hpf1_bypass(config->adc_hpf1_bypass_enable);
	aud_ll_set_adc_config0_adc_set_gain(config->adc_set_gain);
	aud_ll_set_adc_config0_adc_sampe_edge(config->adc_samp_edge);

	/* adc_config1 */
	aud_ll_set_adc_config1_adc_hpf2_coef_b0(config->adc_hpf2_coef_B0);
	aud_ll_set_adc_config1_adc_hpf2_coef_b1(config->adc_hpf2_coef_B1);

	/* adc_config2 */
	aud_ll_set_adc_config2_adc_hpf2_coef_a0(config->adc_hpf2_coef_A0);
	aud_ll_set_adc_config2_adc_hpf2_coef_a1(config->adc_hpf2_coef_A1);

	/* fifo_config */
	aud_ll_set_fifo_config_dtmf_wr_threshold(config->dtmf_wr_threshold);
	aud_ll_set_fifo_config_adcl_wr_threshold(config->adcl_wr_threshold);
	aud_ll_set_fifo_config_dtmf_int_en(config->dtmf_int_enable);
	aud_ll_set_fifo_config_adcl_int_en(config->adcl_int_enable);
	aud_ll_set_fifo_config_loop_adc2dac(config->loop_adc2dac);

	/* agc_config0 */
	aud_ll_set_agc_config0_agc_noise_thrd(config->agc_noise_thrd);
	aud_ll_set_agc_config0_agc_noise_high(config->agc_noise_high);
	aud_ll_set_agc_config0_agc_noise_low(config->agc_noise_low);

	/* agc_config1 */
	aud_ll_set_agc_config1_agc_noise_min(config->agc_noise_min);
	aud_ll_set_agc_config1_agc_noise_tout(config->agc_noise_tout);
	aud_ll_set_agc_config1_agc_high_dur(config->agc_high_dur);
	aud_ll_set_agc_config1_agc_low_dur(config->agc_low_dur);
	aud_ll_set_agc_config1_agc_min(config->agc_min);
	aud_ll_set_agc_config1_agc_max(config->agc_max);
	aud_ll_set_agc_config1_agc_ng_method(config->agc_ng_method);
	aud_ll_set_agc_config1_agc_ng_enable(config->agc_ng_enable);

	/* agc_config2 */
	aud_ll_set_agc_config2_agc_decay_time(config->agc_decay_time);
	aud_ll_set_agc_config2_agc_attack_time(config->agc_attack_time);
	aud_ll_set_agc_config2_agc_high_thrd(config->agc_high_thrd);
	aud_ll_set_agc_config2_agc_low_thrd(config->agc_low_thrd);
	aud_ll_set_agc_config2_agc_iir_coef(config->agc_iir_coef);
	aud_ll_set_agc_config2_agc_enable(config->agc_enable);
	aud_ll_set_agc_config2_manual_pga_value(config->manual_pga_value);
	aud_ll_set_agc_config2_manual_pga(config->manual_pga_enable);

	/* extend_config */
	aud_ll_set_extend_cfg_adc_fracmod_manual(config->adc_fracmod_manual);
	aud_ll_set_adc_fracmod_value(config->adc_fracmod);

	return BK_OK;
}

/* enable adc */
bk_err_t aud_hal_adc_enable(void)
{
	aud_ll_set_audio_config_dtmf_enable(0);
	aud_ll_set_audio_config_adc_enable(1);
	aud_ll_set_audio_config_line_enable(1);

	return BK_OK;
}

/* disable adc */
bk_err_t aud_hal_adc_disable(void)
{
	aud_ll_set_audio_config_dtmf_enable(0);
	aud_ll_set_audio_config_adc_enable(0);
	aud_ll_set_audio_config_line_enable(0);

	return BK_OK;
}

/* enable dtmf */
bk_err_t aud_hal_dtmf_enable(void)
{
	aud_ll_set_audio_config_adc_enable(0);
	aud_ll_set_audio_config_line_enable(0);
	aud_ll_set_audio_config_dtmf_enable(1);

	aud_ll_set_dtmf_config1_tone1_enable(1);
	aud_ll_set_dtmf_config2_tone2_enable(1);

	return BK_OK;
}

/* disable dtmf */
bk_err_t aud_hal_dtmf_disable(void)
{
	aud_ll_set_audio_config_dtmf_enable(0);
	aud_ll_set_audio_config_adc_enable(0);
	aud_ll_set_audio_config_line_enable(0);

	aud_ll_set_dtmf_config1_tone1_enable(0);
	aud_ll_set_dtmf_config2_tone2_enable(0);

	return BK_OK;
}

/* enable adc interrupt */
bk_err_t aud_hal_adc_int_enable(void)
{
	aud_ll_set_fifo_config_dtmf_int_en(1);
	aud_ll_set_fifo_config_adcl_int_en(1);

	return BK_OK;
}

/* disable adc interrupt */
bk_err_t aud_hal_adc_int_disable(void)
{
	aud_ll_set_fifo_config_dtmf_int_en(0);
	aud_ll_set_fifo_config_adcl_int_en(0);

	return BK_OK;
}

bk_err_t aud_hal_dac_config(const aud_dac_config_t *config)
{
	/* audio_config */
	aud_ll_set_audio_config_dac_enable(config->dac_enable);
	aud_ll_set_audio_config_samp_rate_dac(config->samp_rate);

	/* dac_config0 */
	aud_ll_set_dac_config0_dac_hpf2_coef_b2(config->dac_hpf2_coef_B2);
	aud_ll_set_dac_config0_dac_hpf2_bypass(config->dac_hpf2_bypass_enable);
	aud_ll_set_dac_config0_dac_hpf1_bypass(config->dac_hpf1_bypass_enable);
	aud_ll_set_dac_config0_dac_set_gain(config->dac_set_gain);
	aud_ll_set_dac_config0_dac_clk_invert(config->dac_clk_invert);

	/* dac_config1 */
	aud_ll_set_dac_config1_dac_hpf2_coef_b0(config->dac_hpf2_coef_B0);
	aud_ll_set_dac_config1_dac_hpf2_coef_b1(config->dac_hpf2_coef_B1);

	/* dac_config2 */
	aud_ll_set_dac_config2_dac_hpf2_coef_a1(config->dac_hpf2_coef_A1);
	aud_ll_set_dac_config2_dac_hpf2_coef_a2(config->dac_hpf2_coef_A2);
	
	/* fifo_config */
	aud_ll_set_fifo_config_dacl_rd_threshold(config->dacl_rd_threshold);
	aud_ll_set_fifo_config_dacr_rd_threshold(config->dacr_rd_threshold);
	aud_ll_set_fifo_config_dacl_int_en(config->dacl_int_enable);
	aud_ll_set_fifo_config_dacr_int_en(config->dacr_int_enable);
	
	/* extend_config */
	aud_ll_set_extend_cfg_dac_fracmod_manual(config->dac_fracmod_manual_enable);
	aud_ll_set_extend_cfg_filt_enable(config->dac_filt_enable);
	aud_ll_set_dac_fracmod_value(config->dac_fracmode_value);
	
	return BK_OK;
}

bk_err_t aud_hal_dac_set_sample_rate(aud_dac_samp_rate_t sample_rate)
{
	/* disable dac handset bit again, to make sure this bit unset */
	aud_ll_set_extend_cfg_dac_fracmod_manual(0x0);

	switch(sample_rate)
	{
		case AUD_DAC_SAMP_RATE_8K:
			aud_ll_set_audio_config_samp_rate_dac(0x0);
			break;

		case AUD_DAC_SAMP_RATE_11_025K:
			aud_ll_set_extend_cfg_dac_fracmod_manual(0x1);
			aud_ll_set_dac_fracmod_value(CONST_DIV_44_1K << 2);
			aud_ll_set_audio_config_samp_rate_dac(0x2);
			break;

		case AUD_DAC_SAMP_RATE_12K:
			aud_ll_set_extend_cfg_dac_fracmod_manual(0x1);
			aud_ll_set_dac_fracmod_value(CONST_DIV_48K << 2);
			aud_ll_set_audio_config_samp_rate_dac(0x3);
			break;

		case AUD_DAC_SAMP_RATE_16K:
			aud_ll_set_audio_config_samp_rate_dac(0x1);
			break;

		case AUD_DAC_SAMP_RATE_22_05K:
			aud_ll_set_extend_cfg_dac_fracmod_manual(0x1);
			aud_ll_set_dac_fracmod_value(CONST_DIV_44_1K << 1);
			aud_ll_set_audio_config_samp_rate_dac(0x2);
			break;

		case AUD_DAC_SAMP_RATE_24K:
			aud_ll_set_extend_cfg_dac_fracmod_manual(0x1);
			aud_ll_set_dac_fracmod_value(CONST_DIV_48K << 1);
			aud_ll_set_audio_config_samp_rate_dac(0x3);
			break;

		case AUD_DAC_SAMP_RATE_32K:
			aud_ll_set_extend_cfg_dac_fracmod_manual(0x1);
			aud_ll_set_dac_fracmod_value(CONST_DIV_16K >> 1);
			aud_ll_set_audio_config_samp_rate_dac(0x1);
			break;

		case AUD_DAC_SAMP_RATE_44_1K:
			aud_ll_set_audio_config_samp_rate_dac(0x2);
			break;

		case AUD_DAC_SAMP_RATE_48K:
			aud_ll_set_audio_config_samp_rate_dac(0x3);
			break;

		default:
			break;
	}

	return BK_OK;
}

bk_err_t aud_hal_dac_int_enable(void)
{
	aud_ll_set_fifo_config_dacl_int_en(0x1);
	aud_ll_set_fifo_config_dacr_int_en(0x1);

	return BK_OK;
}

bk_err_t aud_hal_dac_int_disable(void)
{
	aud_ll_set_fifo_config_dacl_int_en(0x0);
	aud_ll_set_fifo_config_dacr_int_en(0x0);

	return BK_OK;
}

bk_err_t aud_hal_dac_write(uint32_t pcm_value)
{
	//aud_ll_set_dac_fport_dacl_port(pcm_value & AUD_DAC_FPORT_DACL_PORT_MASK);
	//aud_ll_set_dac_fport_dacr_port((pcm_value >> 16) & AUD_DAC_FPORT_DACR_PORT_MASK);
	aud_hal_set_dac_fport_value(pcm_value);
	return BK_OK;
}

bk_err_t aud_hal_eq_config(const aud_eq_config_t *config)
{
	aud_ll_set_flt0_coef_a1a2_flt0_a1((config->flt0_A1 >> 6) & 0xFFFF);
	aud_ll_set_flt0_ext_coef_flt0_a1_l6bit((config->flt0_A1) & 0x3F);
	aud_ll_set_flt0_coef_a1a2_flt0_a2((config->flt0_A2 >> 6) & 0xFFFF);
	aud_ll_set_flt0_ext_coef_flt0_a2_l6bit((config->flt0_A2) & 0x3F);
	aud_ll_set_flt0_coef_b0b1_flt0_b0((config->flt0_B0 >> 6) & 0xFFFF);
	aud_ll_set_flt0_ext_coef_flt0_b0_l6bit((config->flt0_B0) & 0x3F);
	aud_ll_set_flt0_coef_b0b1_flt0_b1((config->flt0_B1 >> 6) & 0xFFFF);
	aud_ll_set_flt0_ext_coef_flt0_b1_l6bit((config->flt0_B1) & 0x3F);
	aud_ll_set_flt0_coef_b2_flt0_b2((config->flt0_B2 >> 6) & 0xFFFF);
	aud_ll_set_flt0_ext_coef_flt0_b2_l6bit((config->flt0_B2) & 0x3F);

	aud_ll_set_flt1_coef_a1a2_flt1_a1((config->flt1_A1 >> 6) & 0xFFFF);
	aud_ll_set_flt1_ext_coef_flt1_a1_l6bit((config->flt1_A1) & 0x3F);
	aud_ll_set_flt1_coef_a1a2_flt1_a2((config->flt1_A2 >> 6) & 0xFFFF);
	aud_ll_set_flt1_ext_coef_flt1_a2_l6bit((config->flt1_A2) & 0x3F);
	aud_ll_set_flt1_coef_b0b1_flt1_b0((config->flt1_B0 >> 6) & 0xFFFF);
	aud_ll_set_flt1_ext_coef_flt1_b0_l6bit((config->flt1_B0) & 0x3F);
	aud_ll_set_flt1_coef_b0b1_flt1_b1((config->flt1_B1 >> 6) & 0xFFFF);
	aud_ll_set_flt1_ext_coef_flt1_b1_l6bit((config->flt1_B1) & 0x3F);
	aud_ll_set_flt1_coef_b2_flt1_b2((config->flt1_B2 >> 6) & 0xFFFF);
	aud_ll_set_flt1_ext_coef_flt1_b2_l6bit((config->flt1_B2) & 0x3F);

	aud_ll_set_flt2_coef_a1a2_flt2_a1((config->flt2_A1 >> 6) & 0xFFFF);
	aud_ll_set_flt2_ext_coef_flt2_a1_l6bit((config->flt2_A1) & 0x3F);
	aud_ll_set_flt2_coef_a1a2_flt2_a2((config->flt2_A2 >> 6) & 0xFFFF);
	aud_ll_set_flt2_ext_coef_flt2_a2_l6bit((config->flt2_A2) & 0x3F);
	aud_ll_set_flt2_coef_b0b1_flt2_b0((config->flt2_B0 >> 6) & 0xFFFF);
	aud_ll_set_flt2_ext_coef_flt2_b0_l6bit((config->flt2_B0) & 0x3F);
	aud_ll_set_flt2_coef_b0b1_flt2_b1((config->flt2_B1 >> 6) & 0xFFFF);
	aud_ll_set_flt2_ext_coef_flt2_b1_l6bit((config->flt2_B1) & 0x3F);
	aud_ll_set_flt2_coef_b2_flt2_b2((config->flt2_B2 >> 6) & 0xFFFF);
	aud_ll_set_flt2_ext_coef_flt2_b2_l6bit((config->flt2_B2) & 0x3F);

	aud_ll_set_flt3_coef_a1a2_flt3_a1((config->flt3_A1 >> 6) & 0xFFFF);
	aud_ll_set_flt3_ext_coef_flt3_a1_l6bit((config->flt3_A1) & 0x3F);
	aud_ll_set_flt3_coef_a1a2_flt3_a2((config->flt3_A2 >> 6) & 0xFFFF);
	aud_ll_set_flt3_ext_coef_flt3_a2_l6bit((config->flt3_A2) & 0x3F);
	aud_ll_set_flt3_coef_b0b1_flt3_b0((config->flt3_B0 >> 6) & 0xFFFF);
	aud_ll_set_flt3_ext_coef_flt3_b0_l6bit((config->flt3_B0) & 0x3F);
	aud_ll_set_flt3_coef_b0b1_flt3_b1((config->flt3_B1 >> 6) & 0xFFFF);
	aud_ll_set_flt3_ext_coef_flt3_b1_l6bit((config->flt3_B1) & 0x3F);
	aud_ll_set_flt3_coef_b2_flt3_b2((config->flt3_B2 >> 6) & 0xFFFF);
	aud_ll_set_flt3_ext_coef_flt3_b2_l6bit((config->flt3_B2) & 0x3F);

	return BK_OK;
}



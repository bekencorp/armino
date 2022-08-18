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

#include <os/os.h>
#include <components/log.h>

#include "media_core.h"
#include "aud_act.h"

#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>
#include <driver/aud.h>
#include <driver/aud_types.h>
#include <driver/dma.h>
#include "aud_hal.h"
#include "sys_driver.h"
#include "aud_driver.h"
#include <driver/psram.h>
#include <modules/aec.h>
#include <modules/audio_ring_buff.h>
#include <modules/g711.h>

#include "gpio_driver.h"
#include <driver/gpio.h>
#include <soc/mapping.h>

#include "adc_ccb.h"

#define TAG "adc_ccb"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define AUD_8K_FRAME_SAMP_SIZE           160*2

dma_id_t aud_adc_dma_id = DMA_ID_MAX;


static bk_err_t audio_adc_config()
{
	bk_err_t ret = BK_OK;
	aud_adc_config_t adc_config;

	adc_config.mic_config = AUD_MIC_MIC1_ENABLE;
	adc_config.samp_rate = AUD_ADC_SAMP_RATE_8K;
	adc_config.adc_enable = AUD_ADC_DISABLE;
	adc_config.line_enable = AUD_ADC_LINE_DISABLE;
	adc_config.dtmf_enable = AUD_DTMF_DISABLE;
	adc_config.adc_hpf2_coef_B2 = 0;
	adc_config.adc_hpf2_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
	adc_config.adc_hpf1_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
	adc_config.adc_set_gain = 0x2d;
	adc_config.adc_samp_edge = AUD_ADC_SAMP_EDGE_RISING;
	adc_config.adc_hpf2_coef_B0 = 0;
	adc_config.adc_hpf2_coef_B1 = 0;
	adc_config.adc_hpf2_coef_A0 = 0;
	adc_config.adc_hpf2_coef_A1 = 0;
	adc_config.dtmf_wr_threshold = 8;
	adc_config.adcl_wr_threshold = 8;
	adc_config.dtmf_int_enable = AUD_DTMF_INT_DISABLE;
	adc_config.adcl_int_enable = AUD_ADCL_INT_DISABLE;
	adc_config.loop_adc2dac = AUD_LOOP_ADC2DAC_DISABLE;
	adc_config.agc_noise_thrd = 101;
	adc_config.agc_noise_high = 101;
	adc_config.agc_noise_low = 160;
	adc_config.agc_noise_min = 1;
	adc_config.agc_noise_tout = 0;
	adc_config.agc_high_dur = 3;
	adc_config.agc_low_dur = 3;
	adc_config.agc_min = 1;
	adc_config.agc_max = 4;
	adc_config.agc_ng_method = AUD_AGC_NG_METHOD_MUTE;
	adc_config.agc_ng_enable = AUD_AGC_NG_DISABLE;
	adc_config.agc_decay_time = AUD_AGC_DECAY_TIME_128;
	adc_config.agc_attack_time = AUD_AGC_ATTACK_TIME_128;
	adc_config.agc_high_thrd = 18;
	adc_config.agc_low_thrd = 0;
	adc_config.agc_iir_coef = AUD_AGC_IIR_COEF_1_1024;
	adc_config.agc_enable = AUD_AGC_DISABLE;
	adc_config.manual_pga_value = 0;
	adc_config.manual_pga_enable = AUD_GAC_MANUAL_PGA_DISABLE;
	adc_config.adc_fracmod_manual = AUD_ADC_TRACMOD_MANUAL_DISABLE;
	adc_config.adc_fracmod = 0;

	/* init audio driver and config adc */
	ret = bk_aud_driver_init();
	if (ret != BK_OK)
	{
		os_printf("cp1: init audio driver fail \r\n");
		goto aud_adc_exit;
	}

	ret = bk_aud_adc_init(AUD_ADC_WORK_MODE_ADC, &adc_config, NULL);
	if (ret != BK_OK)
	{
		os_printf("cp1: init audio adc fail \r\n");
		goto aud_adc_exit;
	}

	return BK_OK;

aud_adc_exit:
	os_printf("cp1: audio adc config fail \r\n");
	bk_aud_driver_deinit();
	return BK_FAIL;
}


static void audio_dma_adc_finish_isr(void)
{
	bk_gpio_pull_up(GPIO_2);



	bk_gpio_pull_down(GPIO_2);
}

static bk_err_t audio_adc_dma_config(dma_id_t dma_id, int32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t adc_port_addr;

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 1;
	dma_config.src.dev = DMA_DEV_AUDIO;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_16BITS;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;

	/* get adc fifo address */
	if (bk_aud_get_adc_fifo_addr(&adc_port_addr) != BK_OK)
	{
		os_printf("get adc fifo address failed\r\n");
		return BK_FAIL;
	}
	else
	{
		dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dma_config.src.start_addr = adc_port_addr;
		dma_config.src.end_addr = adc_port_addr + 4;
	}

	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
	dma_config.dst.start_addr = (uint32_t)ring_buff_addr;
	dma_config.dst.end_addr = (uint32_t)ring_buff_addr + ring_buff_size;

	/* init dma channel */
	ret = bk_dma_init(dma_id, &dma_config);
	if (ret != BK_OK)
	{
		os_printf("cp1: audio adc dma channel init fail \r\n");
		return BK_FAIL;
	}

	/* set dma transfer length */
	bk_dma_set_transfer_len(dma_id, transfer_len);

	//register isr
	bk_dma_register_isr(dma_id, NULL, (void *)audio_dma_adc_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);

	//bk_dma_set_transfer_len(dma_id, 0x20);
	//bk_dma_start(dma_id);

	return BK_OK;
}


void audio_adc_start(void)
{
	int ret;

	LOGI("%s\n", __func__);

	ret = audio_adc_config();

	if (ret != BK_OK)
	{
		os_printf("cp1: audio adc init fail \r\n");
		return;
	}

	/* allocate free DMA channel */
	aud_adc_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((aud_adc_dma_id < DMA_ID_0) || (aud_adc_dma_id >= DMA_ID_MAX))
	{
		os_printf("malloc dma fail \r\n");
		return;
	}

#ifdef CONFIG_PSRAM
	ret = audio_adc_dma_config(aud_adc_dma_id, (int32_t *)psram_map->aud_adc, AUD_8K_FRAME_SAMP_SIZE * 2, AUD_8K_FRAME_SAMP_SIZE);
#else
	ret = audio_adc_dma_config(aud_adc_dma_id, (int32_t *)NULL, AUD_8K_FRAME_SAMP_SIZE * 2, AUD_8K_FRAME_SAMP_SIZE);
#endif
	ret = bk_dma_start(aud_adc_dma_id);
	if (ret != BK_OK)
	{
		os_printf("cp1: start dac dma fail \r\n");
		return;
	}

	/* enable adc */
	/* wait receive data and then open adc */
	bk_aud_start_adc();
}


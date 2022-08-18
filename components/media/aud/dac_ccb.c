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

#include "dac_ccb.h"

#define TAG "dac_ccb"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define AUD_8K_FRAME_SAMP_SIZE           160*2

dma_id_t aud_dac_dma_id = DMA_ID_MAX;


static bk_err_t audio_dac_config(void)
{
	bk_err_t ret = BK_OK;
	aud_dac_config_t dac_config;

	dac_config.dac_enable = AUD_DAC_DISABLE;
	dac_config.samp_rate = AUD_DAC_SAMP_RATE_SOURCE_8K;
	dac_config.dac_hpf2_coef_B2 = 0x3A22;
	dac_config.dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
	dac_config.dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
	dac_config.dac_set_gain = 0x2D;    //default 2D  3F  15
	dac_config.dac_clk_invert = AUD_DAC_CLK_INVERT_RISING;

	dac_config.dac_hpf2_coef_B0 = 0x3A22;
	dac_config.dac_hpf2_coef_B1 = 0x8BBF;
	dac_config.dac_hpf2_coef_A1 = 0x751C;
	dac_config.dac_hpf2_coef_A2 = 0xC9E6;
	dac_config.dacr_rd_threshold = 0x4;
	dac_config.dacl_rd_threshold = 0x4;
	dac_config.dacr_int_enable = 0x0;
	dac_config.dacl_int_enable = 0x0;
	dac_config.dac_filt_enable = AUD_DAC_FILT_DISABLE;
	dac_config.dac_fracmod_manual_enable = AUD_DAC_FRACMOD_MANUAL_DISABLE;
	dac_config.dac_fracmode_value = 0x0;

	/* init audio driver and config dac */
	ret = bk_aud_driver_init();
	if (ret != BK_OK)
	{
		os_printf("cp1: init audio driver fail \r\n");
		goto aud_dac_exit;
	}

	ret = bk_aud_dac_init(&dac_config);
	if (ret != BK_OK)
	{
		os_printf("cp1: init audio dac fail \r\n");
		goto aud_dac_exit;
	}

	return BK_OK;

aud_dac_exit:
	os_printf("cp1: audio dac config fail \r\n");
	bk_aud_driver_deinit();
	return BK_FAIL;
}



static void audio_dma_dac_finish_isr(void)
{
	bk_gpio_pull_up(GPIO_3);

	bk_gpio_pull_down(GPIO_3);
}

static bk_err_t audio_dac_dma_config(dma_id_t dma_id, int32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t dac_port_addr;

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 1;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.dst.dev = DMA_DEV_AUDIO;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	/* get dac fifo address */
	if (bk_aud_get_dac_fifo_addr(&dac_port_addr) != BK_OK)
	{
		os_printf("get dac fifo address failed\r\n");
		return BK_FAIL;
	}
	else
	{
		dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
		dma_config.dst.start_addr = dac_port_addr;
		dma_config.dst.end_addr = dac_port_addr + 4;
	}
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	//dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
	dma_config.src.start_addr = (uint32_t)ring_buff_addr;
	dma_config.src.end_addr = (uint32_t)(ring_buff_addr) + ring_buff_size;

	/* init dma channel */
	ret = bk_dma_init(dma_id, &dma_config);
	if (ret != BK_OK)
	{
		os_printf("cp1: audio dac dma channel init fail \r\n");
		return BK_FAIL;
	}

	/* set dma transfer length */
	bk_dma_set_transfer_len(dma_id, transfer_len);

	//register isr
	bk_dma_register_isr(dma_id, NULL, (void *)audio_dma_dac_finish_isr);
	bk_dma_enable_finish_interrupt(dma_id);

	//bk_dma_set_transfer_len(dma_id, 0x20);
	//bk_dma_start(dma_id);

	return BK_OK;
}

void audio_dac_start(void)
{
	int ret;

	LOGI("%s\n", __func__);

	ret = audio_dac_config();

	if (ret != BK_OK)
	{
		os_printf("cp1: audio dac init fail \r\n");
		return;
	}

	/* allocate free DMA channel */
	aud_dac_dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
	if ((aud_dac_dma_id < DMA_ID_0) || (aud_dac_dma_id >= DMA_ID_MAX))
	{
		os_printf("malloc dma fail \r\n");
		return;
	}

#ifdef CONFIG_PSRAM
	ret = audio_dac_dma_config(aud_dac_dma_id, (int32_t *)psram_map->aud_dac, AUD_8K_FRAME_SAMP_SIZE, AUD_8K_FRAME_SAMP_SIZE);
#else
	ret = audio_dac_dma_config(aud_dac_dma_id, (int32_t *)NULL, AUD_8K_FRAME_SAMP_SIZE, AUD_8K_FRAME_SAMP_SIZE);
#endif

	ret = bk_dma_start(aud_dac_dma_id);
	if (ret != BK_OK)
	{
		os_printf("cp1: start dac dma fail \r\n");
		return;
	}

	/* enable dac */
	/* wait receive data and then open dac */
	bk_aud_start_dac();

}


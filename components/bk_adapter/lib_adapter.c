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

//TODO optimize it, divide this file to smaller component

#include <stdint.h>
#include <components/log.h>
#include "bk_mcu_ps.h"
#include <os/str.h>
#if CONFIG_FLASH_ORIGIN_API
#include "BkDriverFlash.h"
#include "bk_flash.h"
#else
#include "driver/flash.h"
#endif
#include <os/os.h>
#include <components/system.h>
#include "modules/wifi_types.h"

int mcu_suppress_and_sleep(uint32_t sleep_ticks)
{
#if CONFIG_MCU_PS
#if (CONFIG_FREERTOS)
        GLOBAL_INT_DECLARATION();
        GLOBAL_INT_DISABLE();
        uint32_t missed_ticks = 0;
        missed_ticks = mcu_power_save(sleep_ticks);
        bk_update_tick(missed_ticks);
        GLOBAL_INT_RESTORE();
#endif
#endif
	return BK_OK;
}

//Called by bkreg_tx()
int bkreg_tx_get_uart_port(void)
{
	int port = bk_get_printf_port();

#if (CONFIG_RTT)
#include "rtconfig.h"
        if (os_strcmp(RT_CONSOLE_DEVICE_NAME, "uart1") == 0) {
                port = 0;
	}
#endif
	return port;
}

//Called by write_cal_result_to_flash() only
void write_cal_result_to_flash_secure_op1(void)
{
        #if CONFIG_ALIOS
        hal_flash_dis_secure(0, 0, 0);
        #else
#if CONFIG_FLASH_ORIGIN_API
        bk_flash_enable_security(FLASH_PROTECT_NONE);
#else
        bk_flash_set_protect_type(FLASH_PROTECT_NONE);
#endif
        #endif
}

//Called by write_cal_result_to_flash() only
void write_cal_result_to_flash_secure_op2(void)
{
        #if CONFIG_ALIOS
        hal_flash_enable_secure(0, 0, 0);
        #else
#if CONFIG_FLASH_ORIGIN_API
        bk_flash_enable_security(FLASH_UNPROTECT_LAST_BLOCK);
#else
        bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
#endif
        #endif
}

void connection_fail_cb(wifi_linkstate_reason_t info)
{
#if (CONFIG_ALIOS)
	connect_fail_cb(info);
#endif
}

void bk_audio_intf_dac_pause(void)
{
#if ((CONFIG_AUDIO_DAC) && (CONFIG_SOC_BK7251))
	extern void audio_intf_dac_pause(void);
	audio_intf_dac_pause();
#endif
}

void bk_audio_intf_adc_pause(void)
{
#if ((CONFIG_AUDIO_ADC) && (CONFIG_SOC_BK7251))
	extern void audio_intf_adc_pause(void);
	audio_intf_adc_pause();
#endif
}

void bk_audio_intf_dac_play(void)
{
#if ((CONFIG_AUDIO_DAC) && (CONFIG_SOC_BK7251))
	extern void audio_intf_dac_play(void);
	audio_intf_dac_play();
#endif
}

void bk_audio_intf_adc_play(void)
{
#if ((CONFIG_AUDIO_ADC) && (CONFIG_SOC_BK7251))
	extern void audio_intf_adc_play(void);
	audio_intf_adc_play();
#endif
}

void bk_audio_intf_uninit(void)
{
#if ((CONFIG_AUDIO) && (CONFIG_SOC_BK7251))
	extern void audio_intf_uninit(void);
	audio_intf_uninit();
#endif
}

void bk_audio_intf_init(void)
{
#if ((CONFIG_AUDIO) && (CONFIG_SOC_BK7251))
	extern UINT32 audio_intf_init(void);
	audio_intf_init();
#endif
}

void bk_audio_intf_dac_set_volume(void)
{
#if ((CONFIG_AUDIO_DAC) && (CONFIG_SOC_BK7251))
	extern void audio_intf_dac_set_volume(void);
	audio_intf_dac_set_volume();
#endif
}

void bk_audio_intf_dac_set_sample_rate(void)
{
#if ((CONFIG_AUDIO_DAC) && (CONFIG_SOC_BK7251))
	extern void audio_intf_set_sample_rate(void);
	audio_intf_set_sample_rate();
#endif
}

bool bk_is_audio_en(void)
{
#if CONFIG_AUDIO
	return true;
#else
	return false;
#endif
}

bool bk_is_audio_adc_en(void)
{
#if CONFIG_AUDIO_ADC
	return true;
#else
	return false;
#endif
}

bool bk_is_audio_dac_en(void)
{
#if CONFIG_AUDIO_DAC
	return true;
#else
	return false;
#endif
}

#if CONFIG_RTT
//TODO optimize it, ARMINO components should be OS-independent!!!
#include "rtdef.h"
#include "rthw.h"

extern int wifi_demo(int argc, char **argv);
extern int rfcali_cfg_tssi_b(int argc, char **argv);
extern int rfcali_cfg_mode(int argc, char **argv);
extern int rfcali_cfg_rate_dist(int argc, char **argv);
extern int rfcali_cfg_tssi_g(int argc, char **argv);
extern int rfcali_show_data(int argc, char **argv);
extern void pwrtbl(int argc, char **argv);

MSH_CMD_EXPORT(wifi_demo, wifi_demo command);

#if !CONFIG_SOC_BK7271 //TODO fix it
FINSH_FUNCTION_EXPORT_ALIAS(rfcali_cfg_tssi_b, __cmd_rfcali_cfg_tssi_b, rfcali cfg tssi);
FINSH_FUNCTION_EXPORT_ALIAS(rfcali_cfg_mode, __cmd_rfcali_cfg_mode, rfcali cfg mode);
FINSH_FUNCTION_EXPORT_ALIAS(rfcali_cfg_rate_dist, __cmd_rfcali_cfg_rate_dist, rfcali cfg rate_dist);
FINSH_FUNCTION_EXPORT_ALIAS(rfcali_cfg_tssi_g, __cmd_rfcali_cfg_tssi_g, rfcali cfg tssi);
FINSH_FUNCTION_EXPORT_ALIAS(rfcali_show_data, __cmd_rfcali_show_data, rfcali show data);
#endif

FINSH_FUNCTION_EXPORT_ALIAS(pwrtbl, __cmd_pwrtbl, pwrtbl test);
#endif


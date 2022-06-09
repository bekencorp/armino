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

#include <stdlib.h>
#include <string.h>
#include <common/bk_include.h>
#include "cli_config.h"
#include "param_config.h"
#include "bk_cli.h"
#include <os/str.h>
#include <os/mem.h>
#include <os/os.h>
#include <components/log.h>
#include <common/sys_config.h>
#include <driver/uart.h>
#include "bk_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLI_TAG "cli"

#define CLI_LOGI(...) BK_LOGI(CLI_TAG, ##__VA_ARGS__)
#define CLI_LOGW(...) BK_LOGW(CLI_TAG, ##__VA_ARGS__)
#define CLI_LOGE(...) BK_LOGE(CLI_TAG, ##__VA_ARGS__)
#define CLI_LOGD(...) BK_LOGD(CLI_TAG, ##__VA_ARGS__)

#define CLI_RET_ON_INVALID_ARGC(_actual_argc, _expect_minimum_argc) do {\
		if ((_actual_argc) < (_expect_minimum_argc)) {\
			CLI_LOGE("invalid argc, expect %d actual %d\n", (_expect_minimum_argc), (_actual_argc));\
			return;\
		}\
	}while(0)

int cli_wifi_init(void);
int cli_ble_init(void);
int cli_netif_init(void);
int cli_misc_init(void);
int cli_mem_init(void);
int cli_airkiss_init(void);
int cli_phy_init(void);

#if CONFIG_LWIP
int cli_lwip_init(void);
#endif

int cli_iperf_init(void);
int cli_pwr_init(void);
int cli_timer_init(void);
int cli_wdt_init(void);
int cli_trng_init(void);
int cli_efuse_init(void);
int cli_gpio_init(void);
int cli_os_init(void);
int cli_ota_init(void);
int cli_flash_init(void);
int cli_flash_test_init(void);
int cli_keyVaule_init(void);
int cli_matter_init(void);

int cli_uart_init(void);
int cli_adc_init(void);
int cli_spi_init(void);
int cli_qspi_init(void);
int cli_i2c_init(void);
int cli_aon_rtc_init(void);
int cli_sd_init(void);
int cli_fatfs_init(void);
int cli_temp_detect_init(void);
int cli_security_init(void);
int cli_mico_init(void);
int cli_peri_init(void);
int cli_event_init(void);
int cli_pwm_init(void);
int cli_reg_init(void);
int cli_dma_init(void);
int cli_exception_init(void);
int cli_icu_init(void);
int cli_at_init(void);
#if CONFIG_SECURITYIP
int cli_vault_init(void);
#endif
int cli_aud_init(void);
int cli_fft_init(void);
int cli_sbc_init(void);
int cli_touch_init(void);
int cli_jpeg_init(void);
int cli_lcd_init(void);
int cli_dma2d_init(void);
int cli_i2s_init(void);
int cli_uvc_init(void);
int cli_aud_cp0_init(void);
int cli_calendar_init(void);
int cli_jpegdec_init(void);
int cli_aec_init(void);
int cli_g711_init(void);
int cli_dvp_init(void);
int cli_doorbell_init(void);

extern int hexstr2bin(const char *hex, u8 *buf, size_t len);

#ifdef __cplusplus
}
#endif

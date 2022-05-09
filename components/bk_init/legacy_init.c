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

#include <components/log.h>
#include <modules/wifi.h>
#include <components/netif.h>
#include <components/event.h>
#include <driver/uart.h>

#if CONFIG_BLE
#include "modules/ble.h"
#include "ble_api_5_x.h"
#endif

#if ((CONFIG_FREERTOS) || (CONFIG_LITEOS_M) || (CONFIG_LITEOS_M_V3)) && (CONFIG_CLI)
#include "bk_api_cli.h"
#endif

#define TAG "app_init"

volatile const uint8_t build_version[] = __DATE__ " " __TIME__;

static int app_wifi_init(void)
{
#if (CONFIG_WIFI_ENABLE)
	wifi_init_config_t wifi_config = WIFI_DEFAULT_INIT_CONFIG();
	BK_LOG_ON_ERR(bk_event_init());
	BK_LOG_ON_ERR(bk_netif_init());
	BK_LOG_ON_ERR(bk_wifi_init(&wifi_config));
#endif
	return BK_OK;
}

static int app_ble_init(void)
{
#if CONFIG_BLE
	BK_LOG_ON_ERR(bk_ble_init());
#endif
	return BK_OK;
}

static int app_bt_init(void)
{
#if (CONFIG_SOC_BK7271)
#if (CONFIG_BT)
	BK_LOGI(TAG, "BT active\r\n");
#if 0//TODO
	if (!ate_is_enabled())
		bt_activate(NULL);
#endif
#endif
#endif
	return BK_OK;
}

static int app_key_init(void)
{
#if CONFIG_BUTTON
	key_initialization();
#endif
	return BK_OK;
}

static int app_mp3_player_init(void)
{
#if (CONFIG_MP3PLAYER)
	key_init();
	media_thread_init();
#endif
	return BK_OK;
}

int app_sdio_init(void)
{
#if CONFIG_SDIO
	BK_LOGI(TAG, "sdio intf init\r\n");
	sdio_intf_init();
#endif

#if CONFIG_SDIO_TRANS
	BK_LOGI(TAG, "sdio trans init\r\n");
	sdio_trans_init();
#endif
	return BK_OK;
}

int app_usb_init(void)
{
#if CONFIG_USB
	BK_LOGI(TAG, "fusb init\r\n");
#if 0//TODO
	if (!ate_is_enabled())
		fusb_init();
#endif
#endif
	return BK_OK;
}

static int app_cli_init(void)
{
#if ((CONFIG_FREERTOS) || (CONFIG_LITEOS_M) || (CONFIG_LITEOS_M_V3)) && (CONFIG_CLI)
#if !CONFIG_FULLY_HOSTED
	bk_cli_init();
#endif
#endif
	return BK_OK;
}

static int app_usb_charge_init(void)
{
#if (CONFIG_SOC_BK7251)
#if CONFIG_USB_CHARGE
	extern void usb_plug_func_open(void);
	usb_plug_func_open();
#endif
#endif
	return BK_OK;
}

static int app_uart_debug_init_todo(void)
{
#if CONFIG_UART_DEBUG
#ifndef KEIL_SIMULATOR
	BK_LOGI(TAG, "uart debug init\r\n");
	uart_debug_init();
#endif
#endif
	return BK_OK;
}
int legacy_init1(void)
{
	BK_LOGI(TAG, "bk378 test init!!!!\r\n");
	app_sdio_init();
	app_key_init();
	app_usb_charge_init();
	app_wifi_init();
	app_bt_init();
	app_ble_init();
	app_mp3_player_init();
	app_uart_debug_init_todo();
	app_cli_init();

#if (CONFIG_FREERTOS)
#if CONFIG_SEMI_HOSTED
	semi_host_init();
#endif
#endif

#if CONFIG_UDISK_MP3
	um_init();
#endif
	return 0;
}

int legacy_init(void)
{
	BK_LOGI(TAG, "armino app init: %s\n", build_version);

#ifdef APP_VERSION
	BK_LOGI(TAG, "APP Verion: %s\n", APP_VERSION);
#endif

#if (CONFIG_SOC_BK7256XX)
	app_wifi_init();
#if (CONFIG_BLUETOOTH)
	app_ble_init();
#endif
#elif (CONFIG_SOC_BK7256_CP1)

#else
	app_sdio_init();
	app_key_init();
	app_usb_charge_init();
	app_wifi_init();
#if (CONFIG_BLUETOOTH)
	app_bt_init();
	app_ble_init();
#endif
	app_mp3_player_init();
	app_uart_debug_init_todo();
#endif

	app_cli_init();

#if (CONFIG_FREERTOS)
#if CONFIG_SEMI_HOSTED
	semi_host_init();
#endif
#endif

#if CONFIG_UDISK_MP3
	um_init();
#endif
	return 0;
}

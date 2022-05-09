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
#include <components/log.h>
#include "param_config.h"
#include "bk_drv_model.h"
#include "bk_uart.h"
#include "bk_wifi_private.h"
#include "bk_net_param.h" //TODO should finally remove this include
#include "bk_sys_ctrl.h"
#include <driver/efuse.h>
#include <os/mem.h>
#include "bk_phy.h"
#include <components/system.h>
#include <components/log.h>
#if (CONFIG_RANDOM_MAC_ADDR)
#include <driver/trng.h>
#endif

#define TAG "mac"

#if ((CONFIG_SOC_BK7231) && (CONFIG_BASE_MAC_FROM_EFUSE))
#error "BK7231 not support efuse!"
#endif

#define DEFAULT_MAC_ADDR "\xC8\x47\x8C\x00\x00\x18"
static uint8_t s_base_mac[] = DEFAULT_MAC_ADDR;
static bool s_mac_inited = false;

#if (CONFIG_BASE_MAC_FROM_EFUSE)
static int write_base_mac_to_efuse(const uint8_t *mac)
{
#if 0
	uint8_t efuse_addr = 0;
	uint8_t efuse_data = 0;
	int i = 0, ret;

	if (!mac)
		return BK_ERR_PARAM;

	for (i = 0; i < EFUSE_MAC_LEN; i++) {
		efuse_addr = EFUSE_MAC_START_ADDR + i;
		efuse_data = mac[i];

		if (i == 0) {
			// ensure mac[0]-bit0 in efuse not '1'
			efuse_data &= ~(0x01);
		}

		ret = bk_efuse_write_byte(efuse_addr, efuse_data);
		if (ret != BK_OK) {
			BK_LOGI(TAG, "efuse set mac failed(%x)\r\n", ret);
			return ret;
		}
	}

	BK_LOGI(TAG, "efuse set mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(mac));
	return BK_OK;
#else
	BK_LOGI(TAG, "write mac to eufse stub");
	return BK_OK;
#endif
}

static int read_base_mac_from_efuse(uint8_t *mac)
{
	uint8_t efuse_addr = 0;
	uint8_t efuse_data = 0;
	int i = 0, ret;

	if (!mac)
		return BK_ERR_PARAM;

	for (i = 0; i < BK_MAC_ADDR_LEN; i++) {
		efuse_addr = EFUSE_MAC_START_ADDR + i;
		efuse_data = 0;

		ret = bk_efuse_read_byte(efuse_addr, &efuse_data);
		if (ret == BK_OK)
			mac[i] = efuse_data;
		else {
			os_memset(mac, 0, BK_MAC_ADDR_LEN);
			mac[i] = 0;
			BK_LOGE(TAG, "efuse get mac failed(%x)\n", ret);
			return ret;
		}
	}

	BK_LOGI(TAG, "efuse get mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(mac));

	if (BK_IS_ZERO_MAC(mac)) {
		BK_LOGE(TAG, "efuse MAC all zero, see as error\r\n");
		return BK_ERR_ZERO_MAC;
	}

	return BK_OK;
}
#endif

#if (CONFIG_BASE_MAC_FROM_RF_OTP_FLASH)
static int read_base_mac_from_rf_otp_flash(uint8_t *mac)
{
        if (manual_cal_get_macaddr_from_flash((uint8_t *)mac))
		return BK_OK;
	else
		return BK_FAIL;
}

static int write_base_mac_to_rf_otp_flash(const uint8_t *mac)
{
	if (manual_cal_write_macaddr_to_flash((uint8_t *)s_base_mac))
		return BK_OK;
	else
		return BK_FAIL;
}

#endif

#if (CONFIG_BASE_MAC_FROM_NVS)
static int read_base_mac_from_nvs(uint8_t *mac)
{
        uint8_t tmp_mac[8] = {0};
        if (get_info_item(WIFI_MAC_ITEM, (uint8_t *)tmp_mac, NULL, NULL)) {
                os_memcpy(mac, tmp_mac, BK_MAC_ADDR_LEN);
		return BK_OK;
	} else {
		return BK_FAIL;
	}
}

static int write_base_mac_to_nvs(const uint8_t *mac)
{
	if (save_info_item(WIFI_MAC_ITEM, (uint8_t *)s_base_mac, NULL, NULL))
		return BK_OK;
	else
		return BK_FAIL;
}
#endif

#if (CONFIG_RANDOM_MAC_ADDR)
static void random_mac_address(u8 *mac)
{
	int i = 0;

	mac[3] = bk_rand() & 0xff;
	mac[4] = bk_rand() & 0xff;
	mac[5] = bk_rand() & 0xff;

	os_printf("mac:");
	for (i = 0; i < 6; i++)
	    os_printf("%02X ", mac[i]);
	os_printf("\n");
}
#endif

static int mac_init(void)
{
	int ret = BK_FAIL;

#if (CONFIG_BASE_MAC_FROM_EFUSE)
        ret = read_base_mac_from_efuse(s_base_mac);
#elif (CONFIG_BASE_MAC_FROM_RF_OTP_FLASH)
        ret = read_base_mac_from_rf_otp_flash(s_base_mac);
#elif (CONFIG_BASE_MAC_FROM_NVS)
	ret = read_base_mac_from_nvs(s_base_mac);
#endif

#if (CONFIG_RANDOM_MAC_ADDR)
	if ((BK_OK != ret) ||BK_IS_GROUP_MAC(s_base_mac)) {
		os_memcpy(s_base_mac, DEFAULT_MAC_ADDR, BK_MAC_ADDR_LEN);
		random_mac_address(s_base_mac);
#if (CONFIG_BASE_MAC_FROM_EFUSE)
		ret = write_base_mac_to_efuse(s_base_mac);
#elif (CONFIG_BASE_MAC_FROM_RF_OTP_FLASH)
		ret = write_base_mac_to_rf_otp_flash(s_base_mac);
#elif (CONFIG_BASE_MAC_FROM_NVS)
		ret = write_base_mac_to_nvs(s_base_mac);
#endif
		BK_LOGI(TAG, "use random mac "BK_MAC_FORMAT" as base mac\n", BK_MAC_STR(s_base_mac));
	}
#else
	if (BK_OK != ret) {
		os_memcpy(s_base_mac, DEFAULT_MAC_ADDR, BK_MAC_ADDR_LEN);
		if (BK_IS_GROUP_MAC(s_base_mac)) {
                        BK_LOGE(TAG, "base mac is group mac"BK_MAC_FORMAT"\n", BK_MAC_STR(s_base_mac));
			return BK_ERR_GROUP_MAC;
		} else {
			BK_LOGI(TAG, "use default mac "BK_MAC_FORMAT" as base mac\n", BK_MAC_STR(s_base_mac));
		}
        } else {
		BK_LOGI(TAG, "base mac "BK_MAC_FORMAT"\n", BK_MAC_STR(s_base_mac));
	}
#endif

	return BK_OK;
}

bk_err_t bk_get_mac(uint8_t *mac, mac_type_t type)
{
	uint8_t mac_mask = (0xff & (2/*NX_VIRT_DEV_MAX*/ - 1));
	uint8_t mac_low;


	if (s_mac_inited == false) {
		mac_init();
		s_mac_inited = true;
	}

	switch (type) {
	case MAC_TYPE_BASE:
		os_memcpy(mac, s_base_mac, BK_MAC_ADDR_LEN);
		break;

	case MAC_TYPE_AP:
		mac_mask = (0xff & (2/*NX_VIRT_DEV_MAX*/ - 1));

		os_memcpy(mac, s_base_mac, BK_MAC_ADDR_LEN);
		mac_low = mac[5];

		// if  NX_VIRT_DEV_MAX == 4.
		// if support AP+STA, mac addr should be equal with each other in byte0-4 & byte5[7:2],
		// byte5[1:0] can be different
		// ie: mac[5]= 0xf7,  so mac[5] can be 0xf4, f5, f6. here wre chose 0xf4
		mac[5] &= ~mac_mask;
		mac_low = ((mac_low & mac_mask) ^ mac_mask);
		mac[5] |= mac_low;
		break;

	case MAC_TYPE_STA:
		os_memcpy(mac, s_base_mac, BK_MAC_ADDR_LEN);
		break;

	default:
		return BK_ERR_INVALID_MAC_TYPE;
	}

	return BK_OK;
}

bk_err_t bk_set_base_mac(const uint8_t *mac)
{
	int ret = BK_FAIL;

	if (!mac)
		return BK_ERR_NULL_PARAM;

	if (BK_IS_GROUP_MAC(mac)) {
		BK_LOGE(TAG, "set failed, cann't be a bc/mc address\r\n");
		return BK_ERR_GROUP_MAC;
	}

	os_memcpy(s_base_mac, mac, BK_MAC_ADDR_LEN);

#if (CONFIG_BASE_MAC_FROM_EFUSE)
	ret = write_base_mac_to_efuse(mac);
#elif (CONFIG_BASE_MAC_FROM_RF_OTP_FLASH)
	ret = write_base_mac_to_rf_otp_flash(mac);
#elif (CONFIG_BASE_MAC_FROM_NVS)
	ret = write_base_mac_to_nvs(mac);
#endif

	if (ret != BK_OK)
		BK_LOGE(TAG, "failed to write base mac, ret(%x)\n", ret);

	return ret;
}


/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * Development of the code in this file was sponsored by Microbric Pty Ltd
 * and Mnemote Pty Ltd
 *
 * The MIT License (MIT)
 *
 * Copyright 2020-2023 Beken
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <string.h>

#include "py/mphal.h"
#include "py/runtime.h"
#include "py/mperrno.h"
#include "shared/netutils/netutils.h"
#include "modnetwork.h"

#if MICROPY_PY_NETWORK_WLAN || MICROPY_PY_NETWORK_LAN || MICROPY_PY_NETWORK_PPP
#include <modules/wifi.h>
#include <components/netif.h>

STATIC mp_obj_t netif_initialize() {
	//bk_netif_init();

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(netif_init_obj, netif_initialize);

STATIC mp_obj_t bk_ifconfig(size_t n_args, const mp_obj_t *args) {
    wlan_if_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    netif_ip4_config_t config;
    int ret;

    bk_netif_get_ip4_config(self->if_id, &config);
    if (n_args == 1) {
        // get
        mp_obj_t tuple[4];
        tuple[0] = mp_obj_new_str(config.ip, strlen(config.ip));
        tuple[1] = mp_obj_new_str(config.mask, strlen(config.mask));
        tuple[2] = mp_obj_new_str(config.gateway, strlen(config.gateway));
        tuple[3] = mp_obj_new_str(config.dns, strlen(config.dns));
        return mp_obj_new_tuple(4, tuple);
    } else if (mp_obj_is_type(args[1], &mp_type_tuple) || mp_obj_is_type(args[1], &mp_type_list)) {
        // set
        mp_obj_t *items;
        mp_obj_get_array_fixed_n(args[1], 4, &items);
        strncpy(config.ip, mp_obj_str_get_str(items[0]), NETIF_IP4_STR_LEN);
        strncpy(config.mask, mp_obj_str_get_str(items[1]), NETIF_IP4_STR_LEN);
        strncpy(config.gateway, mp_obj_str_get_str(items[2]), NETIF_IP4_STR_LEN);
        strncpy(config.dns, mp_obj_str_get_str(items[3]), NETIF_IP4_STR_LEN);
        ret = bk_netif_set_ip4_config(self->if_id, &config);
		if (ret != BK_OK) {
	        mp_hal_exceptions(ret);
		}
    } else {
        // check for the correct string
        const char *mode = mp_obj_str_get_str(args[1]);
        if (strcmp("dhcp", mode)) {
            mp_raise_ValueError(MP_ERROR_TEXT("invalid arguments"));
        }
        ret = bk_netif_dhcpc_start(self->if_id);
		if (ret != BK_OK) {
	        mp_hal_exceptions(ret);
		}
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bk_ifconfig_obj, 1, 2, bk_ifconfig);

STATIC mp_obj_t bk_phy_mode(size_t n_args, const mp_obj_t *args) {
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bk_phy_mode_obj, 0, 1, bk_phy_mode);

STATIC const mp_rom_map_elem_t mp_module_network_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_network) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&netif_init_obj) },

    #if MICROPY_PY_NETWORK_WLAN
    { MP_ROM_QSTR(MP_QSTR_WLAN), MP_ROM_PTR(&get_wlan_obj) },
    #endif

    #if MICROPY_PY_NETWORK_LAN
    { MP_ROM_QSTR(MP_QSTR_LAN), MP_ROM_PTR(&get_lan_obj) },
    #endif
    #if MICROPY_PY_NETWORK_PPP
    { MP_ROM_QSTR(MP_QSTR_PPP), MP_ROM_PTR(&ppp_make_new_obj) },
    #endif
    { MP_ROM_QSTR(MP_QSTR_phy_mode), MP_ROM_PTR(&bk_phy_mode_obj) },
    { MP_ROM_QSTR(MP_QSTR_ifconfig), MP_ROM_PTR(&bk_ifconfig_obj) },

    #if MICROPY_PY_NETWORK_WLAN
    { MP_ROM_QSTR(MP_QSTR_STA_IF), MP_ROM_INT(NETIF_IF_STA)},
    { MP_ROM_QSTR(MP_QSTR_AP_IF), MP_ROM_INT(NETIF_IF_AP)},

    { MP_ROM_QSTR(MP_QSTR_MODE_11B), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_MODE_11G), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_MODE_11N), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_MODE_11AX), MP_ROM_INT(0) },

    { MP_ROM_QSTR(MP_QSTR_AUTH_OPEN), MP_ROM_INT(WIFI_SECURITY_NONE) },
    { MP_ROM_QSTR(MP_QSTR_AUTH_WEP), MP_ROM_INT(WIFI_SECURITY_WEP) },
    { MP_ROM_QSTR(MP_QSTR_AUTH_WPA_PSK), MP_ROM_INT(WIFI_SECURITY_WPA_TKIP) },
    { MP_ROM_QSTR(MP_QSTR_AUTH_WPA2_PSK), MP_ROM_INT(WIFI_SECURITY_WPA2_AES) },
    { MP_ROM_QSTR(MP_QSTR_AUTH_WPA_WPA2_PSK), MP_ROM_INT(WIFI_SECURITY_WPA2_MIXED) },
    { MP_ROM_QSTR(MP_QSTR_AUTH_WPA3_PSK), MP_ROM_INT(WIFI_SECURITY_WPA3_SAE) },
    { MP_ROM_QSTR(MP_QSTR_AUTH_WPA2_WPA3_PSK), MP_ROM_INT(WIFI_SECURITY_WPA3_WPA2_MIXED) },
    { MP_ROM_QSTR(MP_QSTR_AUTH_MAX), MP_ROM_INT(WIFI_SECURITY_AUTO) },
    #endif

    #if MICROPY_PY_NETWORK_LAN
    { MP_ROM_QSTR(MP_QSTR_PHY_LAN8710), MP_ROM_INT(PHY_LAN8710) },
    { MP_ROM_QSTR(MP_QSTR_PHY_LAN8720), MP_ROM_INT(PHY_LAN8720) },
    { MP_ROM_QSTR(MP_QSTR_PHY_IP101), MP_ROM_INT(PHY_IP101) },
    { MP_ROM_QSTR(MP_QSTR_PHY_RTL8201), MP_ROM_INT(PHY_RTL8201) },
    { MP_ROM_QSTR(MP_QSTR_PHY_DP83848), MP_ROM_INT(PHY_DP83848) },

    #if CONFIG_ETH_SPI_ETHERNET_KSZ8851SNL
    { MP_ROM_QSTR(MP_QSTR_PHY_KSZ8851SNL), MP_ROM_INT(PHY_KSZ8851SNL) },
    #endif
    #if CONFIG_ETH_SPI_ETHERNET_DM9051
    { MP_ROM_QSTR(MP_QSTR_PHY_DM9051), MP_ROM_INT(PHY_DM9051) },
    #endif
    #if CONFIG_ETH_SPI_ETHERNET_W5500
    { MP_ROM_QSTR(MP_QSTR_PHY_W5500), MP_ROM_INT(PHY_W5500) },
    #endif

    { MP_ROM_QSTR(MP_QSTR_ETH_INITIALIZED), MP_ROM_INT(ETH_INITIALIZED)},
    { MP_ROM_QSTR(MP_QSTR_ETH_STARTED), MP_ROM_INT(ETH_STARTED)},
    { MP_ROM_QSTR(MP_QSTR_ETH_STOPPED), MP_ROM_INT(ETH_STOPPED)},
    { MP_ROM_QSTR(MP_QSTR_ETH_CONNECTED), MP_ROM_INT(ETH_CONNECTED)},
    { MP_ROM_QSTR(MP_QSTR_ETH_DISCONNECTED), MP_ROM_INT(ETH_DISCONNECTED)},
    { MP_ROM_QSTR(MP_QSTR_ETH_GOT_IP), MP_ROM_INT(ETH_GOT_IP)},
    #endif

    { MP_ROM_QSTR(MP_QSTR_STAT_IDLE), MP_ROM_INT(WIFI_LINKSTATE_STA_IDLE)},
    { MP_ROM_QSTR(MP_QSTR_STAT_CONNECTING), MP_ROM_INT(WIFI_LINKSTATE_STA_CONNECTING)},
    { MP_ROM_QSTR(MP_QSTR_STAT_DISCONNECTED), MP_ROM_INT(WIFI_LINKSTATE_STA_DISCONNECTED)},
    { MP_ROM_QSTR(MP_QSTR_STAT_CONNECTED), MP_ROM_INT(WIFI_LINKSTATE_STA_CONNECTED)},
    { MP_ROM_QSTR(MP_QSTR_STAT_CONNECT_FAILED), MP_ROM_INT(WIFI_LINKSTATE_STA_CONNECT_FAILED)},
    { MP_ROM_QSTR(MP_QSTR_STAT_GOT_IP), MP_ROM_INT(WIFI_LINKSTATE_STA_GOT_IP)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_network_globals, mp_module_network_globals_table);

const mp_obj_module_t mp_module_network = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_network_globals,
};

// Note: This port doesn't define MICROPY_PY_NETWORK so this will not conflict
// with the common implementation provided by extmod/modnetwork.c.
MP_REGISTER_MODULE(MP_QSTR_network, mp_module_network);
#endif

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

#include "py/objlist.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "modnetwork.h"

#if MICROPY_PY_NETWORK_WLAN
#include "os/os.h"
#include "modules/wifi.h"
#include "components/event.h"
#include "components/netif_types.h"

#define BK_SCAN_TIMEOUT_MS              (4000)

STATIC const wlan_if_obj_t wlan_sta_obj;
STATIC const wlan_if_obj_t wlan_ap_obj;

// Set to "true" if bk_wifi_sta_start() was called
static bool wifi_started = false;

// Set to "true" if the STA interface is requested to be connected by the
// user, used for automatic reassociation.
static bool wifi_sta_connect_requested = false;

// Set to "true" if the STA interface is connected to wifi and has IP address.
static bool wifi_sta_connected = false;

// Store the current status. 0 means None here, safe to do so as first enum value is WIFI_REASON_UNSPECIFIED=1.
static uint8_t wifi_sta_disconn_reason = 0;

static uint8_t conf_wifi_sta_reconnects = 0;
static uint8_t wifi_sta_reconnects;

// This function is called by the system-event task and so runs in a different
// thread to the main MicroPython task.  It must not raise any Python exceptions.

STATIC void require_if(mp_obj_t wlan_if, int if_no) {
    wlan_if_obj_t *self = MP_OBJ_TO_PTR(wlan_if);
    if (self->if_id != if_no) {
        mp_raise_msg(&mp_type_OSError, if_no == NETIF_IF_STA ? MP_ERROR_TEXT("STA required") : MP_ERROR_TEXT("AP required"));
    }
}

STATIC mp_obj_t get_wlan(size_t n_args, const mp_obj_t *args) {
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
        wifi_started = 1;
    }

    int idx = (n_args > 0) ? mp_obj_get_int(args[0]) : NETIF_IF_STA;
    if (idx == NETIF_IF_STA) {
        return MP_OBJ_FROM_PTR(&wlan_sta_obj);
    } else if (idx == NETIF_IF_AP) {
        return MP_OBJ_FROM_PTR(&wlan_ap_obj);
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid WLAN interface identifier"));
    }
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(get_wlan_obj, 0, 1, get_wlan);

STATIC mp_obj_t network_wlan_active(size_t n_args, const mp_obj_t *args) {
    wlan_if_obj_t *self = MP_OBJ_TO_PTR(args[0]);

    return (self->if_id) ? mp_const_true : mp_const_false;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(network_wlan_active_obj, 1, 2, network_wlan_active);

STATIC mp_obj_t network_wlan_connect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_ssid, ARG_key, ARG_bssid };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_, MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_bssid, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_const_none} },
    };
    wifi_sta_config_t wifi_sta_config;
    wlan_if_obj_t *self = (wlan_if_obj_t *)MP_OBJ_TO_PTR(pos_args[0]);

    require_if(self, NETIF_IF_STA);

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    memset(&wifi_sta_config, 0, sizeof(wifi_sta_config));

    // configure any parameters that are given
    if (n_args > 1) {
        size_t len;
        const char *p;
        if (args[ARG_ssid].u_obj != mp_const_none) {
            p = mp_obj_str_get_data(args[ARG_ssid].u_obj, &len);
            memcpy(wifi_sta_config.ssid, p, MIN(len, sizeof(wifi_sta_config.ssid)));
        }
        if (args[ARG_key].u_obj != mp_const_none) {
            p = mp_obj_str_get_data(args[ARG_key].u_obj, &len);
            memcpy(wifi_sta_config.password, p, MIN(len, sizeof(wifi_sta_config.password)));
        }
        if (args[ARG_bssid].u_obj != mp_const_none) {
            p = mp_obj_str_get_data(args[ARG_bssid].u_obj, &len);
            if (len != sizeof(wifi_sta_config.bssid)) {
                mp_raise_ValueError(NULL);
            }
            memcpy(wifi_sta_config.bssid, p, sizeof(wifi_sta_config.bssid));
        }
        mp_hal_exceptions(bk_wifi_sta_set_config(&wifi_sta_config));
    }

    wifi_sta_reconnects = 0;
    // connect to the WiFi AP
    MP_THREAD_GIL_EXIT();
    mp_hal_exceptions(bk_wifi_sta_start());
    MP_THREAD_GIL_ENTER();
    wifi_sta_connect_requested = true;

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(network_wlan_connect_obj, 1, network_wlan_connect);

STATIC mp_obj_t network_wlan_disconnect(mp_obj_t self_in) {
    wifi_sta_connect_requested = false;
    mp_hal_exceptions(bk_wifi_sta_stop());
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(network_wlan_disconnect_obj, network_wlan_disconnect);

STATIC mp_obj_t network_wlan_status(size_t n_args, const mp_obj_t *args) {
    wlan_if_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if (n_args == 1) {
        if (self->if_id == NETIF_IF_STA) {
            // Case of no arg is only for the STA interface
            if (wifi_sta_connected) {
                // Happy path, connected with IP
                return MP_OBJ_NEW_SMALL_INT(STAT_GOT_IP);
            } else if (wifi_sta_connect_requested
                       && (conf_wifi_sta_reconnects == 0
                           || wifi_sta_reconnects < conf_wifi_sta_reconnects)) {
                // No connection or error, but is requested = Still connecting
                return MP_OBJ_NEW_SMALL_INT(STAT_CONNECTING);
            } else if (wifi_sta_disconn_reason == 0) {
                // No activity, No error = Idle
                return MP_OBJ_NEW_SMALL_INT(STAT_IDLE);
            } else {
                // Simply pass the error through from SDK
                return MP_OBJ_NEW_SMALL_INT(wifi_sta_disconn_reason);
            }
        }
        return mp_const_none;
    }

    // one argument: return status based on query parameter
    switch ((uintptr_t)args[1]) {
        case (uintptr_t)MP_OBJ_NEW_QSTR(MP_QSTR_stations): {
            // return list of connected stations, only if in soft-AP mode
            require_if(args[0], NETIF_IF_AP);

            mp_obj_t list = mp_obj_new_list(0, NULL);
            for (int i = 0; i < 0; ++i) {
                mp_obj_tuple_t *t = mp_obj_new_tuple(1, NULL);
                //t->items[0] = mp_obj_new_bytes(stations[i].mac, sizeof(stations[i].mac));
                mp_obj_list_append(list, t);
            }
            return list;
        }
        case (uintptr_t)MP_OBJ_NEW_QSTR(MP_QSTR_rssi): {
            // return signal of AP, only in STA mode
            require_if(args[0], NETIF_IF_STA);

            return MP_OBJ_NEW_SMALL_INT(-1);
        }
        default:
            mp_raise_ValueError(MP_ERROR_TEXT("unknown status param"));
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(network_wlan_status_obj, 1, 2, network_wlan_status);

static int wlan_scan_done_handler(void *arg, event_module_t event_module,
								  int event_id, void *event_data)
{
	if (arg) {
		rtos_set_semaphore((beken_semaphore_t *)arg);
	}

	return BK_OK;
}

STATIC mp_obj_t network_wlan_scan(mp_obj_t self_in) {
	static beken_semaphore_t scan_sema = NULL;
	wifi_scan_result_t scan_result = {0};
    mp_obj_t list = mp_obj_new_list(0, NULL);
    bk_err_t err;
	wifi_scan_ap_info_t *ap_info;

    MP_THREAD_GIL_EXIT();
	if( scan_sema == NULL ) {
		err = rtos_init_semaphore( &scan_sema, 1 );
		if(err != BK_OK){
			mp_hal_exceptions(err);
		}
	}
    bk_event_register_cb(EVENT_MOD_WIFI, EVENT_WIFI_SCAN_DONE, wlan_scan_done_handler, (void *)&scan_sema);
    err = bk_wifi_scan_start(NULL);
    MP_THREAD_GIL_ENTER();
	err = rtos_get_semaphore(&scan_sema, BK_SCAN_TIMEOUT_MS);
	if(err != BK_OK) {
	    return list;
	}

	mp_hal_exceptions(bk_wifi_scan_get_result(&scan_result));
	for (ap_info = scan_result.aps; ap_info < scan_result.aps + scan_result.ap_num; ap_info++) {
        mp_obj_tuple_t *t = mp_obj_new_tuple(6, NULL);
        int ssid_len = strlen(ap_info->ssid);
        t->items[0] = mp_obj_new_bytes((const byte *)ap_info->ssid, ssid_len);
        t->items[1] = mp_obj_new_bytes(ap_info->bssid, sizeof(ap_info->bssid));
        t->items[2] = MP_OBJ_NEW_SMALL_INT(ap_info->channel);
        t->items[3] = MP_OBJ_NEW_SMALL_INT(0);
        t->items[4] = MP_OBJ_NEW_SMALL_INT(ap_info->security);
        t->items[5] = 0 ? mp_const_true : mp_const_false; // XXX hidden?
        mp_obj_list_append(list, MP_OBJ_FROM_PTR(t));
	}
	bk_wifi_scan_free_result(&scan_result);

    return list;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(network_wlan_scan_obj, network_wlan_scan);

STATIC mp_obj_t network_wlan_isconnected(mp_obj_t self_in) {
    wlan_if_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->if_id == NETIF_IF_STA) {
        return mp_obj_new_bool(wifi_sta_connected);
    } else {
        return mp_obj_new_bool(0 != 0);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(network_wlan_isconnected_obj, network_wlan_isconnected);

STATIC mp_obj_t network_wlan_config_ap(wlan_if_obj_t *self, size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    if (n_args != 1 && kwargs->used != 0) {
        mp_raise_TypeError(MP_ERROR_TEXT("either pos or kw args are allowed"));
    }

    wifi_ap_config_t ap_cfg;
    mp_hal_exceptions(bk_wifi_ap_get_config(&ap_cfg));

    if (kwargs->used != 0) {
        for (size_t i = 0; i < kwargs->alloc; i++) {
            if (!mp_map_slot_is_filled(kwargs, i)) {
                continue;
            }

            switch (mp_obj_str_get_qstr(kwargs->table[i].key)) {
                case MP_QSTR_mac: {
                    mp_buffer_info_t bufinfo;
                    mp_get_buffer_raise(kwargs->table[i].value, &bufinfo, MP_BUFFER_READ);
                    if (bufinfo.len != 6) {
                        mp_raise_ValueError(MP_ERROR_TEXT("invalid buffer length"));
                    }
                    mp_hal_exceptions(bk_wifi_set_mac_address(bufinfo.buf));
                    break;
                }
                case MP_QSTR_ssid:
                case MP_QSTR_essid: {
                    size_t len;
                    const char *s = mp_obj_str_get_data(kwargs->table[i].value, &len);
                    len = MIN(len, sizeof(ap_cfg.ssid));
                    memcpy(ap_cfg.ssid, s, len);
                    break;
                }
                case MP_QSTR_hidden: {
                    ap_cfg.hidden = mp_obj_is_true(kwargs->table[i].value);
                    break;
                }
                case MP_QSTR_security:
                case MP_QSTR_authmode: {
                    ap_cfg.security = mp_obj_get_int(kwargs->table[i].value);
                    break;
                }
                case MP_QSTR_key:
                case MP_QSTR_password: {
                    size_t len;
                    const char *s = mp_obj_str_get_data(kwargs->table[i].value, &len);
                    len = MIN(len, sizeof(ap_cfg.password) - 1);
                    memcpy(ap_cfg.password, s, len);
                    ap_cfg.password[len] = 0;
                    break;
                }
                case MP_QSTR_channel: {
                    // Get the current value of secondary
                    ap_cfg.channel = mp_obj_get_int(kwargs->table[i].value);
                    break;
                }
                case MP_QSTR_hostname:
                case MP_QSTR_dhcp_hostname: {
                    //const char *s = mp_obj_str_get_str(kwargs->table[i].value);
                    //mp_hal_exceptions(tcpip_adapter_set_hostname(self->if_id, s));
                    break;
                }
                case MP_QSTR_max_clients: {
                    ap_cfg.max_con = mp_obj_get_int(kwargs->table[i].value);
                    break;
                }
                case MP_QSTR_txpower: {
                    //int8_t power = (mp_obj_get_float(kwargs->table[i].value) * 4);
                    //mp_hal_exceptions(bk_wifi_set_max_tx_power(power));
                    break;
                }
                case MP_QSTR_protocol: {
                    //mp_hal_exceptions(bk_wifi_set_protocol(self->if_id, mp_obj_get_int(kwargs->table[i].value)));
                    break;
                }
                default:
                    goto unknown;
            }
        }

        mp_hal_exceptions(bk_wifi_ap_set_config(&ap_cfg));

        return mp_const_none;
    }

    // Get config

    if (n_args != 2) {
        mp_raise_TypeError(MP_ERROR_TEXT("can query only one param"));
    }

    mp_obj_t val = mp_const_none;

    switch (mp_obj_str_get_qstr(args[1])) {
        case MP_QSTR_mac: {
            uint8_t mac[6];
            mp_hal_exceptions(bk_wifi_sta_get_mac(mac));
            return mp_obj_new_bytes(mac, sizeof(mac));
        }
        case MP_QSTR_ssid:
        case MP_QSTR_essid:
            val = mp_obj_new_str((char *)ap_cfg.ssid, strlen((char *)ap_cfg.ssid));
            break;
        case MP_QSTR_hidden:
            val = mp_obj_new_bool(ap_cfg.hidden);
            break;
        case MP_QSTR_security:
        case MP_QSTR_authmode:
            val = MP_OBJ_NEW_SMALL_INT(ap_cfg.security);
            break;
        case MP_QSTR_channel: {
            val = MP_OBJ_NEW_SMALL_INT(ap_cfg.channel);
            break;
        }
        case MP_QSTR_hostname:
        case MP_QSTR_dhcp_hostname: {
            //const char *s;
            //mp_hal_exceptions(tcpip_adapter_get_hostname(self->if_id, &s));
            //val = mp_obj_new_str(s, strlen(s));
            break;
        }
        case MP_QSTR_max_clients: {
            val = MP_OBJ_NEW_SMALL_INT(ap_cfg.max_con);
            break;
        }
        case MP_QSTR_txpower: {
            int8_t power = 0;
            //mp_hal_exceptions(bk_wifi_get_max_tx_power(&power));
            val = mp_obj_new_float(power * 0.25);
            break;
        }
        case MP_QSTR_protocol: {
            uint8_t protocol_bitmap = 0;
            //mp_hal_exceptions(bk_wifi_get_protocol(self->if_id, &protocol_bitmap));
            val = MP_OBJ_NEW_SMALL_INT(protocol_bitmap);
            break;
        }
        default:
            goto unknown;
    }

    return val;

unknown:
    mp_raise_msg_varg(&mp_type_TypeError, MP_ERROR_TEXT("unknown config param(%s)"), mp_obj_str_get_str(args[1]));
}

STATIC mp_obj_t network_wlan_config_sta(wlan_if_obj_t *self, size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    if (n_args != 1 && kwargs->used != 0) {
        mp_raise_TypeError(MP_ERROR_TEXT("either pos or kw args are allowed"));
    }

    wifi_sta_config_t sta_cfg;
    bk_wifi_sta_get_config(&sta_cfg);

    if (kwargs->used != 0) {
        for (size_t i = 0; i < kwargs->alloc; i++) {
            if (!mp_map_slot_is_filled(kwargs, i)) {
                continue;
            }

            switch (mp_obj_str_get_qstr(kwargs->table[i].key)) {
                case MP_QSTR_mac: {
                    mp_buffer_info_t bufinfo;
                    mp_get_buffer_raise(kwargs->table[i].value, &bufinfo, MP_BUFFER_READ);
                    if (bufinfo.len != 6) {
                        mp_raise_ValueError(MP_ERROR_TEXT("invalid buffer length"));
                    }
                    mp_hal_exceptions(bk_wifi_set_mac_address(bufinfo.buf));
                    break;
                }
                case MP_QSTR_ssid:
                case MP_QSTR_essid: {
                    size_t len;
                    const char *s = mp_obj_str_get_data(kwargs->table[i].value, &len);
                    len = MIN(len, sizeof(sta_cfg.ssid));
                    memcpy(sta_cfg.ssid, s, len);
                    break;
                }
                case MP_QSTR_security:
                case MP_QSTR_authmode: {
                    sta_cfg.security = mp_obj_get_int(kwargs->table[i].value);
                    break;
                }
                case MP_QSTR_key:
                case MP_QSTR_password: {
                    size_t len;
                    const char *s = mp_obj_str_get_data(kwargs->table[i].value, &len);
                    len = MIN(len, sizeof(sta_cfg.password) - 1);
                    memcpy(sta_cfg.password, s, len);
                    sta_cfg.password[len] = 0;
                    break;
                }
                case MP_QSTR_channel: {
                    // Get the current value of secondary
                    sta_cfg.channel = mp_obj_get_int(kwargs->table[i].value);
                    break;
                }
                case MP_QSTR_hostname:
                case MP_QSTR_dhcp_hostname: {
                    //const char *s = mp_obj_str_get_str(kwargs->table[i].value);
                    //mp_hal_exceptions(tcpip_adapter_set_hostname(self->if_id, s));
                    break;
                }
                case MP_QSTR_reconnects: {
                    int reconnects = mp_obj_get_int(kwargs->table[i].value);
                    // parameter reconnects == -1 means to retry forever.
                    // here means conf_wifi_sta_reconnects == 0 to retry forever.
                    conf_wifi_sta_reconnects = (reconnects == -1) ? 0 : reconnects + 1;
                    break;
                }
                case MP_QSTR_txpower: {
                    //int8_t power = (mp_obj_get_float(kwargs->table[i].value) * 4);
                    //mp_hal_exceptions(bk_wifi_set_max_tx_power(power));
                    break;
                }
                case MP_QSTR_protocol: {
                    //mp_hal_exceptions(bk_wifi_set_protocol(self->if_id, mp_obj_get_int(kwargs->table[i].value)));
                    break;
                }
                default:
                    goto unknown;
            }
        }

        mp_hal_exceptions(bk_wifi_sta_set_config(&sta_cfg));

        return mp_const_none;
    }

    // Get config
    if (n_args != 2) {
        mp_raise_TypeError(MP_ERROR_TEXT("can query only one param"));
    }

    mp_obj_t val = mp_const_none;

    switch (mp_obj_str_get_qstr(args[1])) {
        case MP_QSTR_mac: {
            uint8_t mac[6];
            mp_hal_exceptions(bk_wifi_sta_get_mac(mac));
            return mp_obj_new_bytes(mac, sizeof(mac));
        }
        case MP_QSTR_ssid:
        case MP_QSTR_essid:
            val = mp_obj_new_str((char *)sta_cfg.ssid, strlen((char *)sta_cfg.ssid));
            break;
        case MP_QSTR_security:
        case MP_QSTR_authmode:
            val = MP_OBJ_NEW_SMALL_INT(sta_cfg.security);
            break;
        case MP_QSTR_channel: {
            val = MP_OBJ_NEW_SMALL_INT(sta_cfg.channel);
            break;
        }
        case MP_QSTR_hostname:
        case MP_QSTR_dhcp_hostname: {
            //const char *s;
            //mp_hal_exceptions(tcpip_adapter_get_hostname(self->if_id, &s));
            //val = mp_obj_new_str(s, strlen(s));
            break;
        }
        case MP_QSTR_reconnects: {
            int rec = conf_wifi_sta_reconnects - 1;
            val = MP_OBJ_NEW_SMALL_INT(rec);
            break;
        }
        case MP_QSTR_txpower: {
            int8_t power = 0;
            //mp_hal_exceptions(bk_wifi_get_max_tx_power(&power));
            val = mp_obj_new_float(power * 0.25);
            break;
        }
        case MP_QSTR_protocol: {
            uint8_t protocol_bitmap = 0;
            //mp_hal_exceptions(bk_wifi_get_protocol(self->if_id, &protocol_bitmap));
            val = MP_OBJ_NEW_SMALL_INT(protocol_bitmap);
            break;
        }
        default:
            goto unknown;
    }

    return val;

unknown:
    mp_raise_msg_varg(&mp_type_TypeError, MP_ERROR_TEXT("unknown config param(%s)"), mp_obj_str_get_str(args[1]));
}

STATIC mp_obj_t network_wlan_config(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    if (n_args != 1 && kwargs->used != 0) {
        mp_raise_TypeError(MP_ERROR_TEXT("either pos or kw args are allowed"));
    }

    wlan_if_obj_t *self = MP_OBJ_TO_PTR(args[0]);

    if (self->if_id == NETIF_IF_STA) {
        return network_wlan_config_sta(self, n_args, args, kwargs);
    } else if (self->if_id == NETIF_IF_AP) {
        return network_wlan_config_ap(self, n_args, args, kwargs);
    } else {
        goto unknown;
    }

unknown:
    mp_raise_ValueError(MP_ERROR_TEXT("unknown config param"));
}
MP_DEFINE_CONST_FUN_OBJ_KW(network_wlan_config_obj, 1, network_wlan_config);

STATIC const mp_rom_map_elem_t wlan_if_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_active), MP_ROM_PTR(&network_wlan_active_obj) },
    { MP_ROM_QSTR(MP_QSTR_connect), MP_ROM_PTR(&network_wlan_connect_obj) },
    { MP_ROM_QSTR(MP_QSTR_disconnect), MP_ROM_PTR(&network_wlan_disconnect_obj) },
    { MP_ROM_QSTR(MP_QSTR_status), MP_ROM_PTR(&network_wlan_status_obj) },
    { MP_ROM_QSTR(MP_QSTR_scan), MP_ROM_PTR(&network_wlan_scan_obj) },
    { MP_ROM_QSTR(MP_QSTR_isconnected), MP_ROM_PTR(&network_wlan_isconnected_obj) },
    { MP_ROM_QSTR(MP_QSTR_config), MP_ROM_PTR(&network_wlan_config_obj) },
    { MP_ROM_QSTR(MP_QSTR_ifconfig), MP_ROM_PTR(&network_wlan_config_obj) },
};
STATIC MP_DEFINE_CONST_DICT(wlan_if_locals_dict, wlan_if_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    wlan_if_type,
    MP_QSTR_WLAN,
    MP_TYPE_FLAG_NONE,
    locals_dict, &wlan_if_locals_dict
    );

STATIC const wlan_if_obj_t wlan_sta_obj = {{&wlan_if_type}, NETIF_IF_STA};
STATIC const wlan_if_obj_t wlan_ap_obj = {{&wlan_if_type}, NETIF_IF_AP};

#endif // MICROPY_PY_NETWORK_WLAN

#include "bk_feature.h"
#include "sdkconfig.h"


int bk_feature_bssid_connect_enable(void) {
#if CONFIG_BSSID_CONNECT
	return 1;
#else
	return 0;
#endif
}

int bk_feature_fast_connect_enable(void) {
#if CONFIG_WIFI_FAST_CONNECT
	return 1;
#else
	return 0;
#endif
}

int bk_feature_fast_dhcp_enable(void) {
#if CONFIG_WIFI_FAST_DHCP
	return 1;
#else
	return 0;
#endif
}

int bk_feature_sta_vsie_enable(void) {
#if CONFIG_COMPONENTS_STA_VSIE
	return 1;
#else
	return 0;
#endif
}

int bk_feature_ap_statype_limit_enable(void) {
#if CONFIG_AP_STATYPE_LIMIT
	return 1;
#else
	return 0;
#endif
}

int bk_feature_temp_detect_enable(void) {
#if CONFIG_TEMP_DETECT
    return 1;
#else
    return 0;
#endif
}

int bk_feature_lvgl_use_psram_enable(void) {
#if CONFIG_LVGL_USE_PSRAM
    return 1;
#else
    return 0;
#endif
}

int bk_feature_lvgl_use_direct_mode_enable(void) {
#if CONFIG_LVGL_USE_DIRECT_MODE
    return 1;
#else
    return 0;
#endif
}


int bk_feature_not_check_ssid_enable(void) {
#if CONFIG_NOT_CHECK_SSID_CHANGE
    return 1;
#else
    return 0;
#endif
}


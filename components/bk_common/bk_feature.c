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

int bk_feature_get_cpu_id(void) {
#if CONFIG_SYS_CPU0
    return BK_CPU0;
#elif CONFIG_SYS_CPU1
    return BK_CPU1;
#elif CONFIG_SYS_CPU2
    return BK_CPU2;
#else
    return 0;
#endif
}


int bk_feature_get_cpu_cnt(void) {
#if CONFIG_CPU_CNT
    return CONFIG_CPU_CNT;
#else
    return 1;
#endif
}

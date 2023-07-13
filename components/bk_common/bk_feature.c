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


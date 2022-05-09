#include <common/sys_config.h>
#include <components/log.h>
#include <modules/wifi.h>
#include <components/event.h>
#include <components/netif.h>
#include <string.h>

#define TAG "example"

int example_wifi_event_cb(void *arg, event_module_t event_module,
					  int event_id, void *event_data)
{
	wifi_event_ap_disconnected_t *ap_disconnected;
	wifi_event_ap_connected_t *ap_connected;

	switch (event_id) {
	case EVENT_WIFI_AP_CONNECTED:
		ap_connected = (wifi_event_ap_connected_t *)event_data;
		BK_LOGI(TAG, BK_MAC_FORMAT" connected to AP\n", BK_MAC_STR(ap_connected->mac));
		break;

	case EVENT_WIFI_AP_DISCONNECTED:
		ap_disconnected = (wifi_event_ap_disconnected_t *)event_data;
		BK_LOGI(TAG, BK_MAC_FORMAT" disconnected from AP\n", BK_MAC_STR(ap_disconnected->mac));
		break;


	default:
		BK_LOGI(TAG, "rx event <%d %d>\n", event_module, event_id);
		break;
	}

	return BK_OK;
}

static int example_wifi_init(void)
{
	wifi_init_config_t wifi_config = WIFI_DEFAULT_INIT_CONFIG();
	BK_LOG_ON_ERR(bk_event_init());
	BK_LOG_ON_ERR(bk_netif_init());
	BK_LOG_ON_ERR(bk_wifi_init(&wifi_config));
	return BK_OK;
}

static void example_event_handler_init(void)
{
	BK_LOG_ON_ERR(bk_event_register_cb(EVENT_MOD_WIFI, EVENT_ID_ALL, example_wifi_event_cb, NULL));
}

static void example_ap_init(void)
{
	wifi_ap_config_t ap_config = WIFI_DEFAULT_AP_CONFIG();
	netif_ip4_config_t ip4_config = {0};

	strncpy(ip4_config.ip, CONFIG_EXAMPLE_IP, NETIF_IP4_STR_LEN);
	strncpy(ip4_config.mask, CONFIG_EXAMPLE_MASK, NETIF_IP4_STR_LEN);
	strncpy(ip4_config.gateway, CONFIG_EXAMPLE_GW, NETIF_IP4_STR_LEN);
	strncpy(ip4_config.dns, CONFIG_EXAMPLE_DNS, NETIF_IP4_STR_LEN);
	BK_LOG_ON_ERR(bk_netif_set_ip4_config(NETIF_IF_AP, &ip4_config));

	strncpy(ap_config.ssid, CONFIG_EXAMPLE_WIFI_SSID, WIFI_SSID_STR_LEN);
	strncpy(ap_config.password, CONFIG_EXAMPLE_WIFI_PASSWORD, WIFI_PASSWORD_LEN);

	BK_LOGI(TAG, "ssid:%s  key:%s\r\n", ap_config.ssid, ap_config.password);
	BK_LOG_ON_ERR(bk_wifi_ap_set_config(&ap_config));
	BK_LOG_ON_ERR(bk_wifi_ap_start());
}

int main(void)
{
	example_wifi_init();
	example_event_handler_init();
	example_ap_init();
	return 0;
}

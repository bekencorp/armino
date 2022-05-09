#include <components/log.h>
#include <modules/wifi.h>
#include <components/netif.h>
#include <components/event.h>
#include <string.h>

#define TAG "example"

int example_netif_event_cb(void *arg, event_module_t event_module,
					   int event_id, void *event_data)
{
	netif_event_got_ip4_t *got_ip;

	switch (event_id) {
	case EVENT_NETIF_GOT_IP4:
		got_ip = (netif_event_got_ip4_t *)event_data;
		BK_LOGI(TAG, "%s got ip\n", got_ip->netif_if == NETIF_IF_STA ? "STA" : "unknown netif");
		break;
	default:
		BK_LOGI(TAG, "rx event <%d %d>\n", event_module, event_id);
		break;
	}

	return BK_OK;
}

int example_wifi_event_cb(void *arg, event_module_t event_module,
					  int event_id, void *event_data)
{
	wifi_event_sta_disconnected_t *sta_disconnected;
	wifi_event_sta_connected_t *sta_connected;

	switch (event_id) {
	case EVENT_WIFI_STA_CONNECTED:
		sta_connected = (wifi_event_sta_connected_t *)event_data;
		BK_LOGI(TAG, "STA connected to %s\n", sta_connected->ssid);
		break;

	case EVENT_WIFI_STA_DISCONNECTED:
		sta_disconnected = (wifi_event_sta_disconnected_t *)event_data;
		BK_LOGI(TAG, "STA disconnected, reason(%d)\n", sta_disconnected->disconnect_reason);
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
	BK_LOG_ON_ERR(bk_event_register_cb(EVENT_MOD_NETIF, EVENT_ID_ALL, example_netif_event_cb, NULL));
}

static void example_sta_connect(void)
{
	wifi_sta_config_t sta_config = WIFI_DEFAULT_STA_CONFIG();

	strncpy(sta_config.ssid, CONFIG_EXAMPLE_WIFI_SSID, WIFI_SSID_STR_LEN);
	strncpy(sta_config.password, CONFIG_EXAMPLE_WIFI_PASSWORD, WIFI_PASSWORD_LEN);

	BK_LOGI(TAG, "ssid:%s password:%s\n", sta_config.ssid, sta_config.password);
	BK_LOG_ON_ERR(bk_wifi_sta_set_config(&sta_config));
	BK_LOG_ON_ERR(bk_wifi_sta_start());
}

int main(void)
{
	example_wifi_init();
	example_event_handler_init();
	example_sta_connect();
	return 0;
}

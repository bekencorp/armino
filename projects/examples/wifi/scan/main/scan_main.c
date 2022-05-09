#include <common/sys_config.h>
#include <components/log.h>
#include <modules/wifi.h>
#include <components/event.h>
#include <components/netif.h>
#include <string.h>

#define TAG "example"

static int example_wifi_event_cb(void *arg, event_module_t event_module,
								  int event_id, void *event_data)
{
	wifi_scan_result_t scan_result = {0};

	BK_LOG_ON_ERR(bk_wifi_scan_get_result(&scan_result));
	BK_LOG_ON_ERR(bk_wifi_scan_dump_result(&scan_result));
	bk_wifi_scan_free_result(&scan_result);

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
	BK_LOG_ON_ERR(bk_event_register_cb(EVENT_MOD_WIFI, EVENT_WIFI_SCAN_DONE,
							   example_wifi_event_cb, NULL));
}

static void example_scan_init(void)
{

#if CONFIG_EXAMPLE_SCAN_ALL_SSID
	BK_LOG_ON_ERR(bk_wifi_scan_start(NULL));
#else
	wifi_scan_config_t scan_config = {0};
	strncpy(scan_config.ssid, CONFIG_EXAMPLE_WIFI_SSID, WIFI_SSID_STR_LEN);
	BK_LOG_ON_ERR(bk_wifi_scan_start(&scan_config));
#endif
}

int main(void)
{
	example_wifi_init();
	example_event_handler_init();
	example_scan_init();
	return 0;
}

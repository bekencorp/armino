#include "bk_private/bk_init.h"
#include <components/system.h>
#include <os/os.h>
#if (!CONFIG_SLAVE_CORE)
#include <components/shell_task.h>
#include "bk_private/bk_wifi.h"
#include "bk_private/bk_wifi_wrapper.h"

#include "av_audio.h"
#include "doorbell.h"

extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

void user_app_main(void){
	char *camera_param[4] = {"uvc", "800X480", "480X272"};
	os_printf("start av client \r\n");
	demo_sta_app_init("av_demo", "");
	os_printf("connect ap: av_demo \r\n");
#if CONFIG_AV_DEMO_MODE_TCP
	av_voice_tcp_client_init();
	demo_doorbell_tcp_client_init(3, (char **)&camera_param);
#else
	av_voice_udp_client_init();
	demo_doorbell_udp_client_init(3, (char **)&camera_param);
#endif
	bk_wifi_capa_config(WIFI_CAPA_ID_TX_AMPDU_EN, 0);
	os_printf("av client run \r\n");
}
#endif

int main(void)
{
#if (!CONFIG_SLAVE_CORE)
	rtos_set_user_app_entry((beken_thread_function_t)user_app_main);
	// bk_set_printf_sync(true);
	// shell_set_log_level(BK_LOG_WARN);
#endif
	bk_init();

	return 0;
}
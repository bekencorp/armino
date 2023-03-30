#include "bk_private/legacy_init.h"
#include <components/system.h>
#include <os/os.h>

#if (!CONFIG_SLAVE_CORE)
#include <components/shell_task.h>

#include "bk_private/bk_wifi_wrapper.h"
#include "av_audio.h"
#include "doorbell.h"

extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

void user_app_main(void){
	char *lcd_param[4] = {"800X480", "480X272"};
	os_printf("start av server \r\n");
	demo_softap_app_init("av_demo", "", "13");
	os_printf("start ap: av_demo \r\n");
#if CONFIG_AV_DEMO_MODE_TCP
	av_voice_tcp_server_init();
	demo_doorbell_tcp_server_init(2, (char **)&lcd_param);
#else
	av_voice_udp_server_init();
	demo_doorbell_udp_server_init(2, (char **)&lcd_param);
#endif
	os_printf("av server run \r\n");
}
#endif

int main(void)
{
#if (!CONFIG_SLAVE_CORE)
	rtos_set_user_app_entry((beken_thread_function_t)user_app_main);
	// bk_set_printf_sync(true);
	// shell_set_log_level(BK_LOG_WARN);
#endif
	legacy_init();

	return 0;
}

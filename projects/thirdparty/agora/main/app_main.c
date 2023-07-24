#include "bk_private/legacy_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>
#include "cli.h"

extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

#if CONFIG_SLAVE_CORE
void user_app_main(void){

}
#else
#define AGORA_RTC_CMD_CNT   (sizeof(s_agora_rtc_commands) / sizeof(struct cli_command))

extern void cli_agora_rtc_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

static const struct cli_command s_agora_rtc_commands[] =
{
	{"agora_test", "agora_test {audio start|stop appid audio_type sample_rate aec_en}", cli_agora_rtc_test_cmd},
	{"agora_test", "agora_test {video start|stop appid video_type ppi}", cli_agora_rtc_test_cmd},
	{"agora_test", "agora_test {both start|stop appid audio_type sample_rate video_type ppi aec_en lcd_ppi lcd_name}", cli_agora_rtc_test_cmd},
};

int agora_rtc__cli_init(void)
{
	return cli_register_commands(s_agora_rtc_commands, AGORA_RTC_CMD_CNT);
}
void user_app_main(void){
	agora_rtc__cli_init();
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
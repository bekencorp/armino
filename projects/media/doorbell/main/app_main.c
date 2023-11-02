#include "bk_private/legacy_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>

#include "doorbell_comm.h"

extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

void user_app_main(void)
{
    rtos_delay_milliseconds(1000);
	doorbell_core_init();
}

int main(void)
{
#if (!CONFIG_SLAVE_CORE)
    rtos_set_user_app_entry((beken_thread_function_t)user_app_main);
//    bk_set_printf_sync(false);
    // shell_set_log_level(BK_LOG_WARN);
#endif
    legacy_init();

	return 0;
}
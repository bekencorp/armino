#include "bk_private/bk_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>

#include "cli.h"

//#include "dm_ble_demo.h"

extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

#ifdef CONFIG_CACHE_CUSTOM_SRAM_MAPPING
const unsigned int g_sram_addr_map[4] =
{
    0x38000000,
    0x30020000,
    0x38020000,
    0x30000000
};
#endif


void user_app_main(void)
{
}

int main(void)
{
#if (!CONFIG_SLAVE_CORE)
    rtos_set_user_app_entry((beken_thread_function_t)user_app_main);
    // bk_set_printf_sync(true);
    // shell_set_log_level(BK_LOG_WARN);
#endif
    bk_init();


    rtos_delay_milliseconds(500);

#if CONFIG_BLE

    //dm_ble_demo_main();

#endif

    return 0;
}

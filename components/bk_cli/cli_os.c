#include "cli.h"
#include <os/os.h>
#include "bk_rtos_debug.h"

static void cli_task_list_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	rtos_dump_task_list();
}

static void cli_task_cpuload_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_FREERTOS
	rtos_dump_task_runtime_stats();
#endif
}

static void cli_task_backtrace_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	rtos_dump_backtrace();
}

static void cli_os_info_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	rtos_dump_backtrace();
	rtos_dump_task_list();

#if CONFIG_FREERTOS
	rtos_dump_task_runtime_stats();
#endif

#if CONFIG_FREERTOS && CONFIG_MEM_DEBUG
	os_dump_memory_stats(0, 0, NULL);
#endif

#if CONFIG_FREERTOS_V10
	extern void port_check_isr_stack(void);
	port_check_isr_stack();
#endif
}

static void cli_assert_dump_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	BK_ASSERT(false);
}

#if CONFIG_DEBUG_FIRMWARE
void os_test_stack_guard(uint32_t override_len)
{
	unsigned char test_buf[32] = {0};
	volatile unsigned char *p_test = (volatile unsigned char *)test_buf;
	for (int i = 0; i < 32 + override_len; ++i)
	{
		/* code */
		*p_test = 0xff - i;
		p_test++;
	}

	return;
}
#endif

static void cli_test_stack_guard_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_DEBUG_FIRMWARE
    uint32_t override_len = 16;
    os_printf("Test stack guard beging.\r\n");
    if (argc >= 2) {
        override_len = strtoll(argv[1], NULL, 16);
        os_printf("Test stack guard, override_len: %d.\r\n", override_len);

        os_test_stack_guard(override_len);
    } else {
        os_printf("stackguard <override_len>\r\n");
    }
#endif
}

#define OS_CMD_CNT (sizeof(s_os_commands) / sizeof(struct cli_command))
static const struct cli_command s_os_commands[] = {
	{"tasklist", "list tasks", cli_task_list_cmd},
	{"cpuload", "show task cpu load", cli_task_cpuload_cmd},
	{"backtrace", "show task backtrace", cli_task_backtrace_cmd},
	{"osinfo", "show os runtime information", cli_os_info_cmd},
	{"assert", "asset and dump system information", cli_assert_dump_cmd},
	{"stackguard", "stackguard <override_len>", cli_test_stack_guard_cmd},
};

int cli_os_init(void)
{
	return cli_register_commands(s_os_commands, OS_CMD_CNT);
}

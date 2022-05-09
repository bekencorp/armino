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

#define OS_CMD_CNT (sizeof(s_os_commands) / sizeof(struct cli_command))
static const struct cli_command s_os_commands[] = {
	{"tasklist", "list tasks", cli_task_list_cmd},
	{"cpuload", "show task cpu load", cli_task_cpuload_cmd},
	{"backtrace", "show task backtrace", cli_task_backtrace_cmd},
	{"osinfo", "show os runtime information", cli_os_info_cmd},
	{"assert", "asset and dump system information", cli_assert_dump_cmd},
};

int cli_os_init(void)
{
	return cli_register_commands(s_os_commands, OS_CMD_CNT);
}

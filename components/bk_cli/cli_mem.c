#include <stdlib.h>
#include "cli.h"
#include <os/os.h>
#include <components/system.h>
#include "bk_rtos_debug.h"
#if CONFIG_PSRAM
#include <driver/psram.h>
#endif

void cli_memory_free_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	cmd_printf("free memory %d\r\n", rtos_get_free_heap_size());
}

void cli_memory_dump_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i;
	uint8_t *pstart;
	uint32_t start, length;

	if (argc != 3) {
		cmd_printf("Usage: memdump <addr> <length>.\r\n");
		return;
	}

	start = strtoul(argv[1], NULL, 0);
	length = strtoul(argv[2], NULL, 0);
	pstart = (uint8_t *)start;

	for (i = 0; i < length; i++) {
		cmd_printf("%02x ", pstart[i]);
		if (i % 0x10 == 0x0F)
			cmd_printf("\r\n");

	}
}

void cli_memory_set_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	os_printf("cli_memory_set_cmd\r\n");
}

void cli_memory_stack_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_FREERTOS
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	rtos_dump_stack_memory_usage();
	GLOBAL_INT_RESTORE();
#endif
}

#if CONFIG_MEM_DEBUG && CONFIG_FREERTOS
static void cli_memory_leak_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t start_tick = 0;
	uint32_t ticks_since_malloc = 0;
	uint32_t seconds_since_malloc = 0;
	char *task_name = NULL;

	if (argc >= 2) {
		seconds_since_malloc = os_strtoul(argv[1], NULL, 10);
		ticks_since_malloc = bk_get_ticks_per_second() * seconds_since_malloc;
	}

	if (argc >= 3)
		start_tick = os_strtoul(argv[2], NULL, 10);

	if (argc >= 4)
		task_name = argv[3];

	os_dump_memory_stats(start_tick, ticks_since_malloc, task_name);
}
#endif

#if CONFIG_PSRAM_AS_SYS_MEMORY && CONFIG_FREERTOS
void cli_psram_malloc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t *pstart;
	uint32_t length;

	if (argc != 2) {
		cmd_printf("Usage: psram_malloc <length>.\r\n");
		return;
	}

	length = strtoul(argv[1], NULL, 0);

	pstart = (uint8_t *)psram_malloc(length);

	cmd_printf("psram_malloc ret(%p).\r\n", pstart);
}

void cli_psram_free_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t *pstart;
	uint32_t start;

	if (argc != 2) {
		cmd_printf("Usage: psram_free <addr>.\r\n");
		return;
	}

	start = strtoul(argv[1], NULL, 0);
	pstart = (uint8_t *)start;
	cmd_printf("psram_free addr(%p).\r\n", pstart);
	os_free(pstart);

}
#endif

#define MEM_CMD_CNT (sizeof(s_mem_commands) / sizeof(struct cli_command))
static const struct cli_command s_mem_commands[] = {
	{"memstack", "show stack memory usage", cli_memory_stack_cmd},
	{"memshow", "show free heap", cli_memory_free_cmd},
	{"memdump", "<addr> <length>", cli_memory_dump_cmd},
	{"memset", "<addr> <value 1> [<value 2> ... <value n>]", cli_memory_set_cmd},
#if CONFIG_MEM_DEBUG && CONFIG_FREERTOS
	{"memleak", "[show memleak", cli_memory_leak_cmd},
#endif
#if CONFIG_PSRAM_AS_SYS_MEMORY && CONFIG_FREERTOS
	{"psram_malloc", "psram_malloc <length>", cli_psram_malloc_cmd},
	{"psram_free", "psram_free <addr>", cli_psram_free_cmd},
#endif
};

int cli_mem_init(void)
{
	return cli_register_commands(s_mem_commands, MEM_CMD_CNT);
}

#include <common/bk_include.h>
#include "lwip/opt.h"

#include "lwip/memp.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "cli.h"
#include <os/str.h>
#include <os/os.h>
#include "bk_cli.h"
#include <lwip/sockets.h>
#include <stdlib.h>

void cli_lwip_mem_info(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i;
	struct memp_desc *tmp;

	if (argc > 2) {
		CLI_LOGI("invalid lwip command paramters\n");
		return;
	}

	if ((argc == 2) && (os_strcmp(argv[1], "-r") == 0))
	{
		CLI_LOGI("reset lwip memory err count and max used\n");

		for (i = 0; i < MEMP_MAX; i++) {
			lwip_stats.memp[i]->max = 0,
			lwip_stats.memp[i]->err = 0;
		}

		lwip_stats.mem.max = 0;
		lwip_stats.mem.err = 0;
	}

	SYS_ARCH_DECL_PROTECT(old_level);
	SYS_ARCH_PROTECT(old_level);

	os_printf("%-16s total used addr       size  err\r\n", "Name");
	os_printf("----------------------------------------------------\r\n");
	for (i = 0; i < MEMP_MAX; i++) {
		tmp = (struct memp_desc *)memp_pools[i];
		os_printf("%-16s %-5d %-4d 0x%08x %-4d  %d\r\n",
			tmp->desc, tmp->num, tmp->stats->used,
			(unsigned int)tmp->base, tmp->size, lwip_stats.memp[i]->err);
	}

	os_printf("===== MEMP_PUBF_POOL ======\r\n");
	os_printf("avail %d, used %d, max %d, err %d\r\n",
			lwip_stats.memp[MEMP_PBUF_POOL]->avail,
			lwip_stats.memp[MEMP_PBUF_POOL]->used,
			lwip_stats.memp[MEMP_PBUF_POOL]->max,
			lwip_stats.memp[MEMP_PBUF_POOL]->err);

	os_printf("========== MEM ============\r\n");
	os_printf("avail %d, used %d, max %d, err %d\r\n",
			lwip_stats.mem.avail,
			lwip_stats.mem.used,
			lwip_stats.mem.max,
			lwip_stats.mem.err);

	SYS_ARCH_UNPROTECT(old_level);
}
void cli_lwip_stats(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if LWIP_STATS_DISPLAY
	if (argc > 2) {
		CLI_LOGI("invalid lwip command paramters\n");
		return;
	}

	if ((argc == 2) && (os_strcmp(argv[1], "-r") == 0))
	{
		CLI_LOGI("reset lwip status\n");
		stats_reset_short();
	}

	stats_display_short();
#endif
}
extern void iperf(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define IPERF_CMD_CNT (sizeof(s_lwip_commands) / sizeof(struct cli_command))
static const struct cli_command s_lwip_commands[] = {
#if CONFIG_LWIP
	{"lwip_mem", "print lwip memory information", cli_lwip_mem_info},
	{"lwip_stats", "print lwip protocal statistics", cli_lwip_stats},
#endif
};

int cli_lwip_init(void)
{
	return cli_register_commands(s_lwip_commands, IPERF_CMD_CNT);
}
// eof


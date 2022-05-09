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

void memp_dump_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i;
	struct memp_desc *tmp;

	SYS_ARCH_DECL_PROTECT(old_level);
	SYS_ARCH_PROTECT(old_level);

	cmd_printf("%-16s total used addr       size\r\n", "Name");
	cmd_printf("----------------------------------------------------\r\n");
	for (i = 0; i < MEMP_MAX; i++) {
		tmp = (struct memp_desc *)memp_pools[i];
		cmd_printf("%-16s %-5d %-4d 0x%08x %-4d\r\n",
				   tmp->desc, tmp->num, tmp->stats->used,
				   (unsigned int)tmp->base, tmp->size);
	}

	cmd_printf("===== MEM ======\r\n");
	cmd_printf("avail %d, used %d, max %d\r\n",
			   lwip_stats.mem.avail,
			   lwip_stats.mem.used,
			   lwip_stats.mem.max);

	SYS_ARCH_UNPROTECT(old_level);
}

extern void iperf(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define IPERF_CMD_CNT (sizeof(s_lwip_commands) / sizeof(struct cli_command))
static const struct cli_command s_lwip_commands[] = {
#if CONFIG_LWIP
	{"memp", "print memp list", memp_dump_command},
#endif
};

int cli_lwip_init(void)
{
	return cli_register_commands(s_lwip_commands, IPERF_CMD_CNT);
}
// eof


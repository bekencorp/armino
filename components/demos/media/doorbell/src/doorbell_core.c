#include <common/bk_include.h>
#include "cli.h"
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <driver/int.h>
#include <common/bk_err.h>

#include "doorbell.h"

#if 0
#define CMD_CONTAIN(value) doorbell_cmd_contain(argc, argv, value)

static bool doorbell_cmd_contain(int argc, char **argv, char *string)
{
	int i;
	bool ret = false;

	for (i = 0; i < argc; i++)
	{
		if (os_strcmp(argv[i], string) == 0)
		{
			ret = true;
		}
	}

	return ret;
}
#endif

void cli_doorbell_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int ret = BK_FAIL;

	if (os_strcmp(argv[1], "tcp") == 0)
	{
		ret = demo_doorbell_tcp_init();
	}
#ifdef CONFIG_AV_DEMO
	else if (os_strcmp(argv[1], "udp_c") == 0)
	{
		ret = demo_doorbell_udp_client_init(argc - 2, &argv[2]);
	}
	else if (os_strcmp(argv[1], "udp_s") == 0)
	{
		ret = demo_doorbell_udp_server_init(argc - 2, &argv[2]);
	}
#endif
	else
	{
		ret = demo_doorbell_udp_init();
	}

	if (ret != BK_OK)
	{
		msg = CLI_CMD_RSP_ERROR;
	}
	else
	{
		msg = CLI_CMD_RSP_SUCCEED;
	}

	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}


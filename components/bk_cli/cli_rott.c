#include "cli.h"
extern void rott_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);


#define ROTT_CNT (sizeof(s_rott_commands) / sizeof(struct cli_command))

static const struct cli_command s_rott_commands[] = {
	{"rott", "{init} {bypass,yuyv|vuyy} {rotate r90|270 yuyv|vuyy} ", rott_test},
};

int cli_rott_init(void)
{
	return cli_register_commands(s_rott_commands, ROTT_CNT);
}









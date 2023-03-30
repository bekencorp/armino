#include "cli.h"
extern void rott_only_yuv2rgb565_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void rott_rotate_yuv2rgb565_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);


#define ROTT_CNT (sizeof(s_rott_commands) / sizeof(struct cli_command))

static const struct cli_command s_rott_commands[] = {
	{"rott_bypass", "intput_fmt", rott_only_yuv2rgb565_test},
	{"rott_rotate", "intput_fmt ", rott_rotate_yuv2rgb565_test},
};

int cli_rott_init(void)
{
	return cli_register_commands(s_rott_commands, ROTT_CNT);
}









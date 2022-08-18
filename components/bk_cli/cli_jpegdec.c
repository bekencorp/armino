#include "cli.h"

#define JPEG_DEC_CNT (sizeof(s_jpegdec_commands) / sizeof(struct cli_command))
static const struct cli_command s_jpegdec_commands[] = {};

int cli_jpegdec_init(void)
{
	return cli_register_commands(s_jpegdec_commands, JPEG_DEC_CNT);
}


#include "cli.h"

extern void lcd_jpeg_dec_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define JPEG_DEC_CNT (sizeof(s_jpegdec_commands) / sizeof(struct cli_command))
static const struct cli_command s_jpegdec_commands[] = {
	{"lcd_jpegdec", "jpeg-jpegdec-lcd", lcd_jpeg_dec_cmd},
};

int cli_jpegdec_init(void)
{
	return cli_register_commands(s_jpegdec_commands, JPEG_DEC_CNT);
}


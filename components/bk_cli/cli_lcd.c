#include "cli.h"

#if CONFIG_LCD_TEST

extern void lcd_video_jpeg_dec(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_video(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_8080_init(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_close(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_help(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_rgb_color(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define LCD_CNT (sizeof(s_lcd_commands) / sizeof(struct cli_command))
static const struct cli_command s_lcd_commands[] = {
	{"lcd", "lcd help", lcd_help },
	{"lcd_8080_init", "lcd_8080_init {start|stop}\r\n", lcd_8080_init},
	{"lcd_rgb_clolor", "lcd_rgb_clolor='color'", lcd_rgb_color},
	{"lcd_video", "lcd_video=96M,8,25", lcd_video},
	{"lcd_video_jpeg_dec", "lcd_video_jpeg_dec = 96M,8,25,4", lcd_video_jpeg_dec},
	{"lcd_close", "lcd_close", lcd_close},
};

int cli_lcd_init(void)
{
	return cli_register_commands(s_lcd_commands, LCD_CNT);
}
#endif









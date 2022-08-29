#include "cli.h"
extern void lcd_8080_display_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_8080_display_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_8080_display_480p_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_rgb_display_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_rgb_display_jpeg(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_rgb_display_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_rgb_close(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void sdcard_read_to_psram(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void lcd_rgb_gc9503v_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void jpeg_dec_display_demo(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void sdcard_write_from_mem(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);


#define LCD_CNT (sizeof(s_lcd_commands) / sizeof(struct cli_command))
static const struct cli_command s_lcd_commands[] = {
	{"lcd_8080", "init|fram_disp", lcd_8080_display_test},
	{"lcd_8080_yuv", "lcd_8080_yuv=4", lcd_8080_display_yuv},
	{"lcd_8080_yuv_partical", "lcd_8080_yuv_partical=4", lcd_8080_display_480p_yuv},
	{"lcd_rgb", "lcd_rgb=rgb565_display,1,display_partical", lcd_rgb_display_test},
	{"lcd_rgb_yuv", "lcd_rgb=480p,1,display_partical", lcd_rgb_display_yuv},
	{"lcd_rgb_jpeg", "lcd_rgb_jpeg =480p ,25", lcd_rgb_display_jpeg},
	{"lcd_close", "lcd_close=yuv|jpeg,", lcd_rgb_close},
	{"lcd_device_init_handler", " init , gc9503v ,display", lcd_rgb_gc9503v_test},
	{"sdcard_write_from_mem", "file_name|pixelx|pixely|addr", sdcard_write_from_mem},
	{"sdcard_read_to_mem", "file_name,addr", sdcard_read_to_psram},
	{"lcd", " file_display,file name", jpeg_dec_display_demo},
};

int cli_lcd_init(void)
{
	return cli_register_commands(s_lcd_commands, LCD_CNT);
}









#include "cli.h"

#if CONFIG_DMA2D_TEST

extern void lcd_fill(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void dma2d_memcpy_display(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void bk_example_alpha_blend(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void bk_example_fill_2p_alpha_blend(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void bk_example_dma2d_rgb565_to_argb8888pixel(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void bk_example_dma2d_argb8888_to_rgb565pixel(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void bk_example_dma2d_deinit(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define DMA2D_CNT (sizeof(s_dma2d_commands) / sizeof(struct cli_command))
static const struct cli_command s_dma2d_commands[] = {
	{"lcd_fill", "lcd_dma2d_blending", lcd_fill},
	{"dma2d_memcpy_display", "dma2d_mem_to_mem \r\n", dma2d_memcpy_display},
	{"dma2d_565_to_8888", "dma2d_pfc = RedBlueSwapConfig", bk_example_dma2d_rgb565_to_argb8888pixel},
	{"dma2d_8888_to_565", "dma2d_pfc = RedBlueSwapConfig", bk_example_dma2d_argb8888_to_rgb565pixel},
	{"alpha_patrtial_blend", "bk_example_alpha_blend", bk_example_alpha_blend},
	{"alpha_fill_blend_p2", "bk_example_alpha_blend", bk_example_fill_2p_alpha_blend},
	{"dma2d_deinit", "bk_example_alpha_blend", bk_example_dma2d_deinit},
};

int cli_dma2d_init(void)
{
	return cli_register_commands(s_dma2d_commands, DMA2D_CNT);
}

#endif




















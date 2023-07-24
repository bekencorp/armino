#include <common/bk_include.h>
#include <driver/lcd.h>
#include "st7796s.h"

void st7796s_set_display_mem_area(uint16 xs, uint16 xe, uint16 ys, uint16 ye)
{
	uint16 xs_l, xs_h, xe_l, xe_h;
	uint16 ys_l, ys_h, ye_l, ye_h; 

	xs_h = xs >> 8;
	xs_l = xs & 0xff;

	xe_h = xe >> 8;
	xe_l = xe & 0xff;

	ys_h = ys >> 8;
	ys_l = ys & 0xff;

	ye_h = ye >> 8;
	ye_l = ye & 0xff;

	uint32_t param_clumn[4] = {xs_h, xs_l, xe_h, xe_l};
	uint32_t param_row[4]= {ys_h, ys_l, ye_h, ye_l};

	bk_lcd_8080_send_cmd(4, 0x2a, param_clumn);
	bk_lcd_8080_send_cmd(4, 0x2b, param_row	);
}

#include <common/bk_include.h>
#include <driver/lcd.h>
#include "st7796s.h"

extern void  delay_ms(INT32 ms_count);

uint32_t param_sleep_out[1]  = {0x0 };
uint32_t param_command1[1]   = {0xc3};
uint32_t param_command2[1]   = {0x96};
uint32_t param_memacess[1]   = {0x48};
uint32_t param_color_md[1]   = {0x05};
uint32_t param_polar_cv[1]   = {0x01};
uint32_t param_disp_out[8]   = {0x40, 0x8a, 0x00, 0x00, 0x29, 0x19, 0xa5, 0x33};
uint32_t param_power_1[1]    = {0x06};
uint32_t param_power_2[1]    = {0xa7};
uint32_t param_vcom_ctl[1]   = {0x18};
uint32_t param_cath_ctl1[14] = {0xf0, 0x09, 0x0b, 0x06, 0x04, 0x15, 0x2f, 0x54, 0x42, 0x3c, 0x17, 0x14, 0x18, 0x1b};
uint32_t param_cath_ctl2[14] = {0xf0, 0x09, 0x0b, 0x06, 0x04, 0x03, 0x2d, 0x43, 0x42, 0x3b, 0x16, 0x14, 0x17, 0x1b};
uint32_t param_command12[1]  = {0x3c};
uint32_t param_command22[1]  = {0x69};
uint32_t param_display_on[1] = {0x0 };


/**
 * @brief  lcd_st7796s_init
 * @param  none
 * @return BK_OK:succeed; others: other errors.
 */
bk_err_t st7796s_init(void)
{
	delay_ms(131);
	delay_ms(10);

	bk_lcd_8080_send_cmd(0, SLEEP_OUT, param_sleep_out);
	delay_ms(120);

	bk_lcd_8080_send_cmd(1, COMMAND_1, param_command1);
	bk_lcd_8080_send_cmd(1, COMMAND_2, param_command2);
	bk_lcd_8080_send_cmd(1, MEM_ACCESS_CTRL, param_memacess);
	bk_lcd_8080_send_cmd(1, COLOR_MODE, param_color_md );
	bk_lcd_8080_send_cmd(1, PLOAR_CONVERT, param_polar_cv);
	bk_lcd_8080_send_cmd(8, DISP_OUT_CTRL, param_disp_out );
	bk_lcd_8080_send_cmd(1, POWER_CTRL1, param_power_1  );
	bk_lcd_8080_send_cmd(1, POWER_CTRL2, param_power_2  );
	bk_lcd_8080_send_cmd(1, VCOM_CTRL, param_vcom_ctl );
	bk_lcd_8080_send_cmd(14, CATHODE_CTRL, param_cath_ctl1);
	bk_lcd_8080_send_cmd(14, ANODE_CTRL, param_cath_ctl2);
	bk_lcd_8080_send_cmd(1, COMMAND_1, param_command12);
	bk_lcd_8080_send_cmd(1, COMMAND_2, param_command22);

	delay_ms(120);
	bk_lcd_8080_send_cmd(0, DISPLAY_ON, param_display_on);

	return BK_OK;
}

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

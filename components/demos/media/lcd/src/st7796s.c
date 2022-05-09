#include <common/bk_include.h>
#include <driver/lcd.h>
#include "st7796s.h"

extern void delay(INT32 num);

/**
 * @brief  lcd_st7796s_init
 * @param  none
 * @return BK_OK:succeed; others: other errors.
 */
bk_err_t st7796s_init(void)
{
	delay(16470);
	//delay(16470);

	//*************LCD Driver Initial *****//
	bk_lcd_8080_write_cmd(SLEEP_OUT);
	delay(16470);//120ms
	//delay(16470);//120ms

	bk_lcd_8080_write_cmd(COMMAND_1);

	bk_lcd_8080_write_data(0xc3);
	bk_lcd_8080_write_cmd(COMMAND_2);
	bk_lcd_8080_write_data(0x96);
	bk_lcd_8080_write_cmd(MEM_ACCESS_CTRL);
	bk_lcd_8080_write_data(0x48);
	bk_lcd_8080_write_cmd(COLOR_MODE);
	bk_lcd_8080_write_data(0x5);
	bk_lcd_8080_write_cmd(PLOAR_CONVERT);
	bk_lcd_8080_write_data(0x1);
	bk_lcd_8080_write_cmd(DISP_OUT_CTRL);
	bk_lcd_8080_write_data(0x40);
	bk_lcd_8080_write_data(0x8a);
	bk_lcd_8080_write_data(0x00);
	bk_lcd_8080_write_data(0x00);
	bk_lcd_8080_write_data(0x29);
	bk_lcd_8080_write_data(0x19);
	bk_lcd_8080_write_data(0xa5);
	bk_lcd_8080_write_data(0x33);
	bk_lcd_8080_write_cmd(POWER_CTRL1);//no
	bk_lcd_8080_write_data(0x06);
	bk_lcd_8080_write_cmd(POWER_CTRL2);
	bk_lcd_8080_write_data(0xa7);
	bk_lcd_8080_write_cmd(VCOM_CTRL);
	bk_lcd_8080_write_data(0x18);
	bk_lcd_8080_write_cmd(CATHODE_CTRL); //Positive Voltage Gamma Control
	bk_lcd_8080_write_data(0xf0);
	bk_lcd_8080_write_data(0x09);
	bk_lcd_8080_write_data(0x0b);
	bk_lcd_8080_write_data(0x06);
	bk_lcd_8080_write_data(0x04);
	bk_lcd_8080_write_data(0x15);
	bk_lcd_8080_write_data(0x2f);
	bk_lcd_8080_write_data(0x54);
	bk_lcd_8080_write_data(0x42);
	bk_lcd_8080_write_data(0x3c);
	bk_lcd_8080_write_data(0x17);
	bk_lcd_8080_write_data(0x14);
	bk_lcd_8080_write_data(0x18);
	bk_lcd_8080_write_data(0x1b);
	bk_lcd_8080_write_cmd(ANODE_CTRL); //Negative Voltage Gamma Control
	bk_lcd_8080_write_data(0xf0);
	bk_lcd_8080_write_data(0x09);
	bk_lcd_8080_write_data(0x0b);
	bk_lcd_8080_write_data(0x06);
	bk_lcd_8080_write_data(0x04);
	bk_lcd_8080_write_data(0x03);
	bk_lcd_8080_write_data(0x2d);
	bk_lcd_8080_write_data(0x43);
	bk_lcd_8080_write_data(0x42);
	bk_lcd_8080_write_data(0x3b);
	bk_lcd_8080_write_data(0x16);
	bk_lcd_8080_write_data(0x14);
	bk_lcd_8080_write_data(0x17);
	bk_lcd_8080_write_data(0x1b);

	bk_lcd_8080_write_cmd(COMMAND_1);
	bk_lcd_8080_write_data(0x3c);
	bk_lcd_8080_write_cmd(COMMAND_1);
	bk_lcd_8080_write_data(0x69);
	
	//delay(16470);
	delay(16470);//120ms
	bk_lcd_8080_write_cmd(DISPLAY_ON); //Display ON
	return BK_OK;
}


void st7796s_set_display_mem_area(uint16 column, uint16 row)
{
	/*
	This function need set in initial.
	Can't set again in int handle.
	Will cause imag dislocaled.
	*/

	uint16 column_num1,column_num2,row_num1,row_num2;

	column_num1 = column >> 8;
	column_num2 = column & 0xff;

	row_num1 = row >> 8;
	row_num2 = row & 0xff;

	bk_lcd_8080_write_cmd(COLUMN_SET);
	bk_lcd_8080_write_data(0x0);
	bk_lcd_8080_write_data(0x0);
	bk_lcd_8080_write_data(column_num1);
	bk_lcd_8080_write_data(column_num2);
	
	bk_lcd_8080_write_cmd(ROW_SET);
	bk_lcd_8080_write_data(0x0);
	bk_lcd_8080_write_data(0x0);
	bk_lcd_8080_write_data(row_num1);
	bk_lcd_8080_write_data(row_num2);
}



// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <driver/gpio.h>
#include <driver/media_types.h>
#include <driver/lcd_types.h>
#include <driver/lcd_spi.h>
#include "bk_misc.h"
#include "lcd_devices.h"
#include "gpio_driver.h"
#include <driver/lcd.h>


#if CONFIG_PWM
#include <driver/pwm.h>
#endif


static const lcd_rgb_t lcd_rgb =
{
	.clk = LCD_20M,
	.data_out_clk_edge = NEGEDGE_OUTPUT,

	.hsync_pulse_width = 10,
	.vsync_pulse_width = 2,
	.hsync_back_porch = 60,
	.hsync_front_porch = 30, 
	.vsync_back_porch = 35,
	.vsync_front_porch = 18,
};

static void lcd_st7701s_config(void)
{
	#define Delay delay_ms
	#define SPI_WriteComm lcd_spi_write_cmd
	#define SPI_WriteData lcd_spi_write_data

	bk_gpio_set_output_high(LCD_SPI_RST);
	delay(10);
	bk_gpio_set_output_low(LCD_SPI_RST);
	delay(800);
	bk_gpio_set_output_high(LCD_SPI_RST);
	delay(800);
#if 1
	//GP_COMMAD_PA(1);
	SPI_WriteComm(0x11);
	//------------------------------------------Bank0 Setting----------------------------------------------------//
	//------------------------------------Display Control setting----------------------------------------------//
	Delay(200);
//	//GP_COMMAD_PA(6);
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);

//	//GP_COMMAD_PA(3);
	SPI_WriteComm(0xC0);
	SPI_WriteData(0x63);
	SPI_WriteData(0x00);

//	//GP_COMMAD_PA(3);
	SPI_WriteComm(0xC1);
	SPI_WriteData(0x11);
	SPI_WriteData(0x02);

//	//GP_COMMAD_PA(3);
	SPI_WriteComm(0xC2);
	SPI_WriteData(0x31);
	SPI_WriteData(0x08);
	//-------------------------------------Gamma Cluster Setting-------------------------------------------//
//	//GP_COMMAD_PA(17);
	SPI_WriteComm(0xB0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x14);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x10);
	SPI_WriteData(0x05);
	SPI_WriteData(0x02);
	SPI_WriteData(0x08);
	SPI_WriteData(0x08);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x05);
	SPI_WriteData(0x13);
	SPI_WriteData(0x11);
	SPI_WriteData(0xA3);
	SPI_WriteData(0x29);
	SPI_WriteData(0x18); 

//	//GP_COMMAD_PA(17);
	SPI_WriteComm(0xB1);
	SPI_WriteData(0x40);
	SPI_WriteData(0x02);
	SPI_WriteData(0x86);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x13);
	SPI_WriteData(0x09);
	SPI_WriteData(0x05);
	SPI_WriteData(0x09);
	SPI_WriteData(0x09);
	SPI_WriteData(0x1F);
	SPI_WriteData(0x07);
	SPI_WriteData(0x15);
	SPI_WriteData(0x12);
	SPI_WriteData(0x15);
	SPI_WriteData(0x19);
	SPI_WriteData(0x08);
	//---------------------------------------End Gamma Setting----------------------------------------------//
	//------------------------------------End Display Control setting----------------------------------------//
	//-----------------------------------------Bank0 Setting End---------------------------------------------//
	//-------------------------------------------Bank1 Setting---------------------------------------------------//
	//-------------------------------- Power Control Registers Initial --------------------------------------//
//	//GP_COMMAD_PA(6);
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x11);

	//GP_COMMAD_PA(2);
	SPI_WriteComm(0xB0);
	SPI_WriteData(0x50);
	//-------------------------------------------Vcom Setting---------------------------------------------------//
	//GP_COMMAD_PA(2);
	SPI_WriteComm(0xB1);
	SPI_WriteData(0x68);
	//-----------------------------------------End Vcom Setting-----------------------------------------------//
	//GP_COMMAD_PA(2);
	SPI_WriteComm(0xB2);
	SPI_WriteData(0x07);

	//GP_COMMAD_PA(2);
	SPI_WriteComm(0xB3);
	SPI_WriteData(0x80);

	//GP_COMMAD_PA(2);
	SPI_WriteComm(0xB5);
	SPI_WriteData(0x47);

	//GP_COMMAD_PA(2);
	SPI_WriteComm(0xB7);
	SPI_WriteData(0x8A);

	//GP_COMMAD_PA(2);
	SPI_WriteComm(0xB8);
	SPI_WriteData(0x21);

	//GP_COMMAD_PA(2);
	SPI_WriteComm(0xC1);
	SPI_WriteData(0x78);

	//GP_COMMAD_PA(2);
	SPI_WriteComm(0xC2);
	SPI_WriteData(0x78);

	//GP_COMMAD_PA(2);
	SPI_WriteComm(0xD0);
	SPI_WriteData(0x88);
	//---------------------------------End Power Control Registers Initial -------------------------------//
	Delay(100);
	//---------------------------------------------GIP Setting----------------------------------------------------//
	//GP_COMMAD_PA(4);
	SPI_WriteComm(0xE0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x02);

	//GP_COMMAD_PA(12);
	SPI_WriteComm(0xE1);
	SPI_WriteData(0x08);
	SPI_WriteData(0x00);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x00);
	SPI_WriteData(0x07);
	SPI_WriteData(0x00);
	SPI_WriteData(0x09);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x33);
	SPI_WriteData(0x33);

	//GP_COMMAD_PA(14);
	SPI_WriteComm(0xE2);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	//GP_COMMAD_PA(5);
	SPI_WriteComm(0xE3);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x33);
	SPI_WriteData(0x33);

	//GP_COMMAD_PA(3);
	SPI_WriteComm(0xE4);
	SPI_WriteData(0x44);
	SPI_WriteData(0x44);

	//GP_COMMAD_PA(17);
	SPI_WriteComm(0xE5);
	SPI_WriteData(0x0E);
	SPI_WriteData(0x2D);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x10);
	SPI_WriteData(0x2D);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x2D);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x0C);
	SPI_WriteData(0x2D);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);

	//GP_COMMAD_PA(5);
	SPI_WriteComm(0xE6);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x33);
	SPI_WriteData(0x33);

	//GP_COMMAD_PA(3);
	SPI_WriteComm(0xE7);
	SPI_WriteData(0x44);
	SPI_WriteData(0x44);

	//GP_COMMAD_PA(17);
	SPI_WriteComm(0xE8);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x2D);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x2D);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x09);
	SPI_WriteData(0x2D);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x0B);
	SPI_WriteData(0x2D);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xA0);

	//GP_COMMAD_PA(8);
	SPI_WriteComm(0xEB);
	SPI_WriteData(0x02);
	SPI_WriteData(0x01);
	SPI_WriteData(0xE4);
	SPI_WriteData(0xE4);
	SPI_WriteData(0x44);
	SPI_WriteData(0x00);
	SPI_WriteData(0x40);

	//GP_COMMAD_PA(3);
	SPI_WriteComm(0xEC);
	SPI_WriteData(0x02);
	SPI_WriteData(0x01);

	//GP_COMMAD_PA(17);
	SPI_WriteComm(0xED);
	SPI_WriteData(0xAB);
	SPI_WriteData(0x89);
	SPI_WriteData(0x76);
	SPI_WriteData(0x54);
	SPI_WriteData(0x01);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0x10);
	SPI_WriteData(0x45);
	SPI_WriteData(0x67);
	SPI_WriteData(0x98);
	SPI_WriteData(0xBA);
	//---------------------------------------------End GIP Setting-----------------------------------------------//
	//------------------------------ Power Control Registers Initial End-----------------------------------//
	//------------------------------------------Bank1 Setting----------------------------------------------------//
	//GP_COMMAD_PA(6);
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0x3A);
	SPI_WriteData(0x50);

	Delay(120);

#if 0
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x12);

	SPI_WriteComm(0xD1);
	SPI_WriteData(0x81);
	SPI_WriteComm(0xD2);
	SPI_WriteData(0x08);
#endif
	////GP_COMMAD_PA(1);
	SPI_WriteComm(0x29);
#else
	
	Delay(20);
	 /*
	SPI_WriteComm(0xB0);
	SPI_WriteData(0x04);
	
	SPI_WriteComm(0x6F);
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	
	SPI_WriteComm(0xB3);//Frame Memory Access and Interface Setting
	SPI_WriteData(0x12);//02(MCU)
	SPI_WriteData(0x00);
	
	SPI_WriteComm(0xB6);//DSI Control
	SPI_WriteData(0x51);
	SPI_WriteData(0x83);
	
	SPI_WriteComm(0xB7);//MDDI Control
	SPI_WriteData(0x00);
	SPI_WriteData(0x80);
	SPI_WriteData(0x15);
	SPI_WriteData(0x25);
	
	SPI_WriteComm(0xBD);//Resizing Control
	SPI_WriteData(0x00);
	
	SPI_WriteComm(0xC0);//Panel Driving Setting 1
	SPI_WriteData(0x02);
	SPI_WriteData(0x87);//PCDIVH,L
	
	SPI_WriteComm(0xC1);//Panel Driving Setting 2
	SPI_WriteData(0x43);//SS 23
	SPI_WriteData(0x31);//NL
	SPI_WriteData(0x99);//GIPMODE
	SPI_WriteData(0x21);//GSPF
	SPI_WriteData(0x20);//GSPS
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);//DIVI
	SPI_WriteData(0x28);//RTN
	SPI_WriteData(0x0C);//BP
	SPI_WriteData(0x0A);//FP
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x21);
	SPI_WriteData(0x01);
	
	SPI_WriteComm(0xC2);//Display V-Timing Setting
	SPI_WriteData(0x00);//LINEINV
	SPI_WriteData(0x06);//SEQGND
	SPI_WriteData(0x06);//SEQVCI
	SPI_WriteData(0x01);//GEQ1W
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	
	SPI_WriteComm(0xC4);//Panel Driving Setting 3
	SPI_WriteData(0x00);
	SPI_WriteData(0x01);
	
	SPI_WriteComm(0xC6);//Outline Sharpening Control
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	
	SPI_WriteComm(0xC7);//Panel Driving Setting 4
	SPI_WriteData(0x11);
	SPI_WriteData(0x8D);
	SPI_WriteData(0xA0);
	SPI_WriteData(0xF5);
	SPI_WriteData(0x27);
	
	SPI_WriteComm(0xC8);
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	
	SPI_WriteComm(0xC9);
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	
	SPI_WriteComm(0xCA);
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	
	SPI_WriteComm(0xD0);//Power Setting (Charge Pump Setting)
	SPI_WriteData(0x99);//DC
	SPI_WriteData(0x03);
	SPI_WriteData(0xCE);//VLMT
	SPI_WriteData(0xA6);//DC, VLMT
	SPI_WriteData(0x0C);//00 不要MOS
	SPI_WriteData(0x43);//VC3, VC2
	SPI_WriteData(0x20);
	SPI_WriteData(0x10);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x01);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x03);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	
	SPI_WriteComm(0xD1);//Power Setting (Switching Regulator Setting)
	SPI_WriteData(0x18);
	SPI_WriteData(0x0C);
	SPI_WriteData(0x23);
	SPI_WriteData(0x03);
	SPI_WriteData(0x75);
	SPI_WriteData(0x02);
	SPI_WriteData(0x50);
	
	SPI_WriteComm(0xD3);//Power Setting for Internal Mode
	SPI_WriteData(0x33);
	
	SPI_WriteComm(0xD5);//VPLVL/VNLVL Setting
	SPI_WriteData(0x2A);//VPLVL
	SPI_WriteData(0x2A);//VNLVL
	
	SPI_WriteComm(0xD6);
	SPI_WriteData(0x28);
	
	SPI_WriteComm(0xDA);
	SPI_WriteData(0x01);
	
	SPI_WriteComm(0xDE);
	SPI_WriteData(0x01);
	SPI_WriteData(0x4F);//VDC
	
	SPI_WriteComm(0xE6);
	SPI_WriteData(0x4F);//VDC2
	
	SPI_WriteComm(0xFA);//VDC_SEL Setting
	SPI_WriteData(0x03);
	 */
	
	
	   
	   //SSD2805_Generic_packetsize_set(2);
	SPI_WriteComm(0xB0);//B0
	SPI_WriteData(0x04);
	
	//SSD2805_Generic_packetsize_set(3);
	SPI_WriteComm(0x6f);//B0
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	
	//SSD2805_Generic_packetsize_set(3);
	SPI_WriteComm(0xB3);//B3
	SPI_WriteData(0x12);//12
	SPI_WriteData(0x00);
	
	//SSD2805_Generic_packetsize_set(3);
	SPI_WriteComm(0xB6);//B6
	SPI_WriteData(0x51);
	SPI_WriteData(0x83);
	
	//SSD2805_Generic_packetsize_set(2);
	SPI_WriteComm(0xBd);//B6
	SPI_WriteData(0x00);
	
	//SSD2805_Generic_packetsize_set(3);
	SPI_WriteComm(0xc0);//B6
	SPI_WriteData(0x02);
	SPI_WriteData(0x87);
	//*********************************************
	//SSD2805_Generic_packetsize_set(16);
	SPI_WriteComm(0xC1);//C1
	SPI_WriteData(0x43);//SS
	SPI_WriteData(0x3F);//NL   31  800	3f	854
	SPI_WriteData(0x99);//GIPMODE
	SPI_WriteData(0x21);//GSPF
	SPI_WriteData(0x20);//GSPS
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);//DIVI
	SPI_WriteData(0x28);//RTN
	SPI_WriteData(0x0C);//BP
	SPI_WriteData(0x0A);//FP
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x21);
	SPI_WriteData(0x01);
	
	//SSD2805_Generic_packetsize_set(7);
	SPI_WriteComm(0xC2);//C2
	SPI_WriteData(0x00);//LINEINV
	SPI_WriteData(0x06);//SEQGND
	SPI_WriteData(0x06);//SEQVCI
	SPI_WriteData(0x01);//GEQ1W
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	
	//SSD2805_Generic_packetsize_set(3);
	SPI_WriteComm(0xC4);//C2
	SPI_WriteData(0x00);//LINEINV
	SPI_WriteData(0x01);//SEQGND
	
	//SSD2805_Generic_packetsize_set(3);
	SPI_WriteComm(0xC6);//C2
	SPI_WriteData(0x00);//LINEINV
	SPI_WriteData(0x00);//SEQGND
	
	//SSD2805_Generic_packetsize_set(6);
	SPI_WriteComm(0xC7);//C2
	SPI_WriteData(0x11);//LINEINV
	SPI_WriteData(0x8d);//SEQGND
	SPI_WriteData(0xa0);//SEQVCI
	SPI_WriteData(0xf5);//GEQ1W
	SPI_WriteData(0x27);
	
	
	//SSD2805_Generic_packetsize_set(25);
	SPI_WriteComm(0xC8);//C8
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	
	//SSD2805_Generic_packetsize_set(25);
	SPI_WriteComm(0xC9);//C9
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	
	//SSD2805_Generic_packetsize_set(25);
	SPI_WriteComm(0xCA);//CA
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	SPI_WriteData(0x01);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x12);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x2B);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x29);
	SPI_WriteData(0x17);
	SPI_WriteData(0x13);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x04);
	
	//SSD2805_Generic_packetsize_set(17);
	SPI_WriteComm(0xD0);//D0
	SPI_WriteData(0x99);//DC
	SPI_WriteData(0x03);
	SPI_WriteData(0xCE);//VLMT
	SPI_WriteData(0xA6);//DC, VLMT
	SPI_WriteData(0x00);//...........
	SPI_WriteData(0x43);//VC3, VC2
	SPI_WriteData(0x20);//
	SPI_WriteData(0x10);//
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x01);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x03);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	
	//SSD2805_Generic_packetsize_set(8);
	SPI_WriteComm(0xD1);//D1
	SPI_WriteData(0x18);
	SPI_WriteData(0x0C);
	SPI_WriteData(0x23);
	SPI_WriteData(0x03);
	SPI_WriteData(0x75);
	SPI_WriteData(0x02);
	SPI_WriteData(0x50);
	
	//SSD2805_Generic_packetsize_set(2);
	SPI_WriteComm(0xD3);//D3
	SPI_WriteData(0x33);
	
	//SSD2805_Generic_packetsize_set(3);
	SPI_WriteComm(0xD5);//D5
	SPI_WriteData(0x2A);
	SPI_WriteData(0x2A);
	
	//SSD2805_Generic_packetsize_set(2);
	SPI_WriteComm(0xD6);//D3
	SPI_WriteData(0x28);
	
	//SSD2805_Generic_packetsize_set(2);
	SPI_WriteComm(0xDa);//D3
	SPI_WriteData(0x01);
	
	//SSD2805_Generic_packetsize_set(3);
	SPI_WriteComm(0xDE);//DE
	SPI_WriteData(0x01);
	SPI_WriteData(0x4f);//59
	
	//SSD2805_Generic_packetsize_set(2);
	SPI_WriteComm(0xe6);//D3
	SPI_WriteData(0x4f);
	
	//SSD2805_Generic_packetsize_set(2);
	SPI_WriteComm(0xFA);//FA
	SPI_WriteData(0x03);
	
	
	SPI_WriteComm(0xB0);
	SPI_WriteData(0x03);
	
	SPI_WriteComm(0x2A);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x01);
	SPI_WriteData(0xDF);
	
	SPI_WriteComm(0x2B);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x03);
	SPI_WriteData(0x1F);
	
	SPI_WriteComm(0x36);
	SPI_WriteData(0x08);
	
	SPI_WriteComm(0x3A);
	SPI_WriteData(0x55);
	
	SPI_WriteComm(0x11);
	Delay(200);
	
	SPI_WriteComm(0x29);
	Delay(20);
	
	SPI_WriteComm(0x2C);
#endif
}

static void lcd_st7701s_init(void)
{
#if 1
    os_printf("lcd_st7701s: init.\r\n");
#endif
	lcd_spi_init_gpio();
	lcd_st7701s_config();
}

static void st7701s_lcd_backlight_open(void)
{
	os_printf("lcd_st7701s: backlight open.\r\n");
#if CONFIG_PWM
	lcd_driver_backlight_init(LCD_RGB_PWM_BACKLIGHT, 100);
#endif
}

static void lcd_set_backlight(uint8_t percent)
{
#if CONFIG_PWM
	pwm_period_duty_config_t config = {0};

	if (percent > 100)
	{
		percent  = 100;
	}

	config.period_cycle = 100;
	config.duty_cycle = percent;

	bk_pwm_set_period_duty(LCD_RGB_PWM_BACKLIGHT, &config);
#endif
}

static void st7701s_lcd_backlight_close(void)
{
	os_printf("lcd_st7701s: backlight close.\r\n");
#if CONFIG_PWM
	lcd_driver_backlight_deinit(LCD_RGB_PWM_BACKLIGHT);
#endif
}

const lcd_device_t lcd_device_st7701s_ly =
{
	.id = LCD_DEVICE_ST7701S_LY,
	.name = "st7701s_ly",
	.type = LCD_TYPE_RGB565,
	.ppi = PPI_480X800,
	.rgb = &lcd_rgb,
	.backlight_open = st7701s_lcd_backlight_open,
	.backlight_set = lcd_set_backlight,
	.init = lcd_st7701s_init,
	.backlight_close = st7701s_lcd_backlight_close,
	.lcd_off = NULL,
};


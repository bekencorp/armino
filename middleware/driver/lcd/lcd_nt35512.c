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

static const lcd_rgb_t lcd_rgb =
{
	.clk = LCD_20M,
	.data_out_clk_edge = NEGEDGE_OUTPUT,
	.hsync_pulse_width = 2,
	.hsync_back_porch = 2,  //3
	.hsync_front_porch = 10,  //2
	
	.vsync_pulse_width = 2,
	.vsync_back_porch = 5,   //
	.vsync_front_porch = 9,  //1
};


static void lcd_nt35512_config(void)
{
	bk_gpio_set_output_high(LCD_SPI_RST);
	delay_us(10);
	bk_gpio_set_output_low(LCD_SPI_RST);
	delay_us(800);
	bk_gpio_set_output_high(LCD_SPI_RST);
	delay_us(800);

	lcd_spi_write_hf_word_cmd(0xF000); lcd_spi_write_hf_word_data(0x0055);
    lcd_spi_write_hf_word_cmd(0xF001); lcd_spi_write_hf_word_data(0x00aa);
    lcd_spi_write_hf_word_cmd(0xF002); lcd_spi_write_hf_word_data(0x0052);
    lcd_spi_write_hf_word_cmd(0xF003); lcd_spi_write_hf_word_data(0x0008);
    lcd_spi_write_hf_word_cmd(0xF004); lcd_spi_write_hf_word_data(0x0001);
  
    lcd_spi_write_hf_word_cmd(0xb600); lcd_spi_write_hf_word_data(0x0034);
    lcd_spi_write_hf_word_cmd(0xb601); lcd_spi_write_hf_word_data(0x0034);
    lcd_spi_write_hf_word_cmd(0xb602); lcd_spi_write_hf_word_data(0x0034);
  
    lcd_spi_write_hf_word_cmd(0xB000); lcd_spi_write_hf_word_data(0x0009); // RGB Interface Signals Control
    lcd_spi_write_hf_word_cmd(0xB001); lcd_spi_write_hf_word_data(0x0009);
    lcd_spi_write_hf_word_cmd(0xB002); lcd_spi_write_hf_word_data(0x0009);
  
    lcd_spi_write_hf_word_cmd(0xb700); lcd_spi_write_hf_word_data(0x0024);
    lcd_spi_write_hf_word_cmd(0xb701); lcd_spi_write_hf_word_data(0x0024);
    lcd_spi_write_hf_word_cmd(0xb702); lcd_spi_write_hf_word_data(0x0024);
  
    lcd_spi_write_hf_word_cmd(0xB100); lcd_spi_write_hf_word_data(0x0006); // Display Option Control
    lcd_spi_write_hf_word_cmd(0xB101); lcd_spi_write_hf_word_data(0x0006);
    lcd_spi_write_hf_word_cmd(0xB102); lcd_spi_write_hf_word_data(0x0006);
  
    lcd_spi_write_hf_word_cmd(0xB800); lcd_spi_write_hf_word_data(0x0034);
    lcd_spi_write_hf_word_cmd(0xB200); lcd_spi_write_hf_word_data(0x0000);
  
    lcd_spi_write_hf_word_cmd(0xB900); lcd_spi_write_hf_word_data(0x0024); // Display Option Control
    lcd_spi_write_hf_word_cmd(0xB901); lcd_spi_write_hf_word_data(0x0024);
    lcd_spi_write_hf_word_cmd(0xB902); lcd_spi_write_hf_word_data(0x0024);
  
    lcd_spi_write_hf_word_cmd(0xB300); lcd_spi_write_hf_word_data(0x0005); // Display Option Control
    lcd_spi_write_hf_word_cmd(0xB301); lcd_spi_write_hf_word_data(0x0005);
    lcd_spi_write_hf_word_cmd(0xB302); lcd_spi_write_hf_word_data(0x0005);
  
    lcd_spi_write_hf_word_cmd(0xBf00); lcd_spi_write_hf_word_data(0x0001);
  
    lcd_spi_write_hf_word_cmd(0xBa00); lcd_spi_write_hf_word_data(0x0024); // Display Option Control
    lcd_spi_write_hf_word_cmd(0xBa01); lcd_spi_write_hf_word_data(0x0024);
    lcd_spi_write_hf_word_cmd(0xBa02); lcd_spi_write_hf_word_data(0x0024);
  
    lcd_spi_write_hf_word_cmd(0xB500); lcd_spi_write_hf_word_data(0x000B); // Display Option Control
    lcd_spi_write_hf_word_cmd(0xB501); lcd_spi_write_hf_word_data(0x000B);
    lcd_spi_write_hf_word_cmd(0xB502); lcd_spi_write_hf_word_data(0x000B);
  
    lcd_spi_write_hf_word_cmd(0xBc00); lcd_spi_write_hf_word_data(0x0000); // Display Option Control
    lcd_spi_write_hf_word_cmd(0xBc01); lcd_spi_write_hf_word_data(0x0090);
    lcd_spi_write_hf_word_cmd(0xBc02); lcd_spi_write_hf_word_data(0x0000);
  
    lcd_spi_write_hf_word_cmd(0xBd00); lcd_spi_write_hf_word_data(0x0000); // Display Option Control
    lcd_spi_write_hf_word_cmd(0xBd01); lcd_spi_write_hf_word_data(0x0090);
    lcd_spi_write_hf_word_cmd(0xBd02); lcd_spi_write_hf_word_data(0x0000);
  
    lcd_spi_write_hf_word_cmd(0xBe00); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xBe01); lcd_spi_write_hf_word_data(0x004b);
  
  //Positive Gamma for RED
    lcd_spi_write_hf_word_cmd(0xD100); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD101); lcd_spi_write_hf_word_data(0x0037);
    lcd_spi_write_hf_word_cmd(0xD102); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD103); lcd_spi_write_hf_word_data(0x0052);
    lcd_spi_write_hf_word_cmd(0xD104); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD105); lcd_spi_write_hf_word_data(0x007B);
    lcd_spi_write_hf_word_cmd(0xD106); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD107); lcd_spi_write_hf_word_data(0x0099);
    lcd_spi_write_hf_word_cmd(0xD108); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD109); lcd_spi_write_hf_word_data(0x00B1);
    lcd_spi_write_hf_word_cmd(0xD10A); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD10B); lcd_spi_write_hf_word_data(0x00d2);
    lcd_spi_write_hf_word_cmd(0xD10C); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD10D); lcd_spi_write_hf_word_data(0x00f6);
    lcd_spi_write_hf_word_cmd(0xD10E); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD10F); lcd_spi_write_hf_word_data(0x0027);
    lcd_spi_write_hf_word_cmd(0xD110); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD111); lcd_spi_write_hf_word_data(0x004E);
    lcd_spi_write_hf_word_cmd(0xD112); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD113); lcd_spi_write_hf_word_data(0x008C);
    lcd_spi_write_hf_word_cmd(0xD114); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD115); lcd_spi_write_hf_word_data(0x00BE);
    lcd_spi_write_hf_word_cmd(0xD116); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD117); lcd_spi_write_hf_word_data(0x000B);
    lcd_spi_write_hf_word_cmd(0xD118); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD119); lcd_spi_write_hf_word_data(0x0048);
    lcd_spi_write_hf_word_cmd(0xD11A); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD11B); lcd_spi_write_hf_word_data(0x004A);
    lcd_spi_write_hf_word_cmd(0xD11C); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD11D); lcd_spi_write_hf_word_data(0x007E);
    lcd_spi_write_hf_word_cmd(0xD11E); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD11F); lcd_spi_write_hf_word_data(0x00BC);
    lcd_spi_write_hf_word_cmd(0xD120); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD121); lcd_spi_write_hf_word_data(0x00e1);
    lcd_spi_write_hf_word_cmd(0xD122); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD123); lcd_spi_write_hf_word_data(0x0010);
    lcd_spi_write_hf_word_cmd(0xD124); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD125); lcd_spi_write_hf_word_data(0x0031);
    lcd_spi_write_hf_word_cmd(0xD126); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD127); lcd_spi_write_hf_word_data(0x005a);
    lcd_spi_write_hf_word_cmd(0xD128); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD129); lcd_spi_write_hf_word_data(0x0073);
    lcd_spi_write_hf_word_cmd(0xD12A); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD12B); lcd_spi_write_hf_word_data(0x0094);
    lcd_spi_write_hf_word_cmd(0xD12C); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD12D); lcd_spi_write_hf_word_data(0x009f);
    lcd_spi_write_hf_word_cmd(0xD12E); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD12F); lcd_spi_write_hf_word_data(0x00b3);
    lcd_spi_write_hf_word_cmd(0xD130); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD131); lcd_spi_write_hf_word_data(0x00b9);
    lcd_spi_write_hf_word_cmd(0xD132); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD133); lcd_spi_write_hf_word_data(0x00c1);
  
  
  //Positive Gamma for GREEN
    lcd_spi_write_hf_word_cmd(0xD200); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD201); lcd_spi_write_hf_word_data(0x0037);
    lcd_spi_write_hf_word_cmd(0xD202); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD203); lcd_spi_write_hf_word_data(0x0052);
    lcd_spi_write_hf_word_cmd(0xD204); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD205); lcd_spi_write_hf_word_data(0x007b);
    lcd_spi_write_hf_word_cmd(0xD206); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD207); lcd_spi_write_hf_word_data(0x0099);
    lcd_spi_write_hf_word_cmd(0xD208); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD209); lcd_spi_write_hf_word_data(0x00b1);
    lcd_spi_write_hf_word_cmd(0xD20A); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD20B); lcd_spi_write_hf_word_data(0x00d2);
    lcd_spi_write_hf_word_cmd(0xD20C); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD20D); lcd_spi_write_hf_word_data(0x00f6);
    lcd_spi_write_hf_word_cmd(0xD20E); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD20F); lcd_spi_write_hf_word_data(0x0027);
    lcd_spi_write_hf_word_cmd(0xD210); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD211); lcd_spi_write_hf_word_data(0x004e);
    lcd_spi_write_hf_word_cmd(0xD212); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD213); lcd_spi_write_hf_word_data(0x008c);
    lcd_spi_write_hf_word_cmd(0xD214); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD215); lcd_spi_write_hf_word_data(0x00be);
    lcd_spi_write_hf_word_cmd(0xD216); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD217); lcd_spi_write_hf_word_data(0x000b);
    lcd_spi_write_hf_word_cmd(0xD218); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD219); lcd_spi_write_hf_word_data(0x0048);
    lcd_spi_write_hf_word_cmd(0xD21A); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD21B); lcd_spi_write_hf_word_data(0x004a);
    lcd_spi_write_hf_word_cmd(0xD21C); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD21D); lcd_spi_write_hf_word_data(0x007e);
    lcd_spi_write_hf_word_cmd(0xD21E); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD21F); lcd_spi_write_hf_word_data(0x00bc);
    lcd_spi_write_hf_word_cmd(0xD220); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD221); lcd_spi_write_hf_word_data(0x00e1);
    lcd_spi_write_hf_word_cmd(0xD222); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD223); lcd_spi_write_hf_word_data(0x0010);
    lcd_spi_write_hf_word_cmd(0xD224); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD225); lcd_spi_write_hf_word_data(0x0031);
    lcd_spi_write_hf_word_cmd(0xD226); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD227); lcd_spi_write_hf_word_data(0x005a);
    lcd_spi_write_hf_word_cmd(0xD228); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD229); lcd_spi_write_hf_word_data(0x0073);
    lcd_spi_write_hf_word_cmd(0xD22A); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD22B); lcd_spi_write_hf_word_data(0x0094);
    lcd_spi_write_hf_word_cmd(0xD22C); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD22D); lcd_spi_write_hf_word_data(0x009f);
    lcd_spi_write_hf_word_cmd(0xD22E); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD22F); lcd_spi_write_hf_word_data(0x00b3);
    lcd_spi_write_hf_word_cmd(0xD230); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD231); lcd_spi_write_hf_word_data(0x00b9);
    lcd_spi_write_hf_word_cmd(0xD232); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD233); lcd_spi_write_hf_word_data(0x00c1);
  
  //Positive Gamma for Blue
   lcd_spi_write_hf_word_cmd(0xD300); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD301); lcd_spi_write_hf_word_data(0x0037);
    lcd_spi_write_hf_word_cmd(0xD302); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD303); lcd_spi_write_hf_word_data(0x0052);
    lcd_spi_write_hf_word_cmd(0xD304); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD305); lcd_spi_write_hf_word_data(0x007b);
    lcd_spi_write_hf_word_cmd(0xD306); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD307); lcd_spi_write_hf_word_data(0x0099);
    lcd_spi_write_hf_word_cmd(0xD308); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD309); lcd_spi_write_hf_word_data(0x00b1);
    lcd_spi_write_hf_word_cmd(0xD30A); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD30B); lcd_spi_write_hf_word_data(0x00d2);
    lcd_spi_write_hf_word_cmd(0xD30C); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD30D); lcd_spi_write_hf_word_data(0x00f6);
    lcd_spi_write_hf_word_cmd(0xD30E); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD30F); lcd_spi_write_hf_word_data(0x0027);
    lcd_spi_write_hf_word_cmd(0xD310); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD311); lcd_spi_write_hf_word_data(0x004e);
    lcd_spi_write_hf_word_cmd(0xD312); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD313); lcd_spi_write_hf_word_data(0x008c);
    lcd_spi_write_hf_word_cmd(0xD314); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD315); lcd_spi_write_hf_word_data(0x00be);
    lcd_spi_write_hf_word_cmd(0xD316); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD317); lcd_spi_write_hf_word_data(0x000b);
    lcd_spi_write_hf_word_cmd(0xD318); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD319); lcd_spi_write_hf_word_data(0x0048);
    lcd_spi_write_hf_word_cmd(0xD31A); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD31B); lcd_spi_write_hf_word_data(0x004a);
    lcd_spi_write_hf_word_cmd(0xD31C); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD31D); lcd_spi_write_hf_word_data(0x007e);
    lcd_spi_write_hf_word_cmd(0xD31E); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD31F); lcd_spi_write_hf_word_data(0x00bc);
    lcd_spi_write_hf_word_cmd(0xD320); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD321); lcd_spi_write_hf_word_data(0x00e1);
    lcd_spi_write_hf_word_cmd(0xD322); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD323); lcd_spi_write_hf_word_data(0x0010);
    lcd_spi_write_hf_word_cmd(0xD324); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD325); lcd_spi_write_hf_word_data(0x0031);
    lcd_spi_write_hf_word_cmd(0xD326); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD327); lcd_spi_write_hf_word_data(0x005a);
    lcd_spi_write_hf_word_cmd(0xD328); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD329); lcd_spi_write_hf_word_data(0x0073);
    lcd_spi_write_hf_word_cmd(0xD32A); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD32B); lcd_spi_write_hf_word_data(0x0094);
    lcd_spi_write_hf_word_cmd(0xD32C); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD32D); lcd_spi_write_hf_word_data(0x009f);
    lcd_spi_write_hf_word_cmd(0xD32E); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD32F); lcd_spi_write_hf_word_data(0x00b3);
    lcd_spi_write_hf_word_cmd(0xD330); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD331); lcd_spi_write_hf_word_data(0x00b9);
    lcd_spi_write_hf_word_cmd(0xD332); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD333); lcd_spi_write_hf_word_data(0x00c1);
  
  
  //Negative Gamma for RED
    lcd_spi_write_hf_word_cmd(0xD400); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD401); lcd_spi_write_hf_word_data(0x0037);
    lcd_spi_write_hf_word_cmd(0xD402); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD403); lcd_spi_write_hf_word_data(0x0052);
    lcd_spi_write_hf_word_cmd(0xD404); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD405); lcd_spi_write_hf_word_data(0x007b);
    lcd_spi_write_hf_word_cmd(0xD406); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD407); lcd_spi_write_hf_word_data(0x0099);
    lcd_spi_write_hf_word_cmd(0xD408); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD409); lcd_spi_write_hf_word_data(0x00b1);
    lcd_spi_write_hf_word_cmd(0xD40A); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD40B); lcd_spi_write_hf_word_data(0x00d2);
    lcd_spi_write_hf_word_cmd(0xD40C); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD40D); lcd_spi_write_hf_word_data(0x00f6);
    lcd_spi_write_hf_word_cmd(0xD40E); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD40F); lcd_spi_write_hf_word_data(0x0027);
    lcd_spi_write_hf_word_cmd(0xD410); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD411); lcd_spi_write_hf_word_data(0x004e);
    lcd_spi_write_hf_word_cmd(0xD412); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD413); lcd_spi_write_hf_word_data(0x008c);
    lcd_spi_write_hf_word_cmd(0xD414); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD415); lcd_spi_write_hf_word_data(0x00be);
    lcd_spi_write_hf_word_cmd(0xD416); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD417); lcd_spi_write_hf_word_data(0x000b);
    lcd_spi_write_hf_word_cmd(0xD418); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD419); lcd_spi_write_hf_word_data(0x0048);
    lcd_spi_write_hf_word_cmd(0xD41A); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD41B); lcd_spi_write_hf_word_data(0x004a);
    lcd_spi_write_hf_word_cmd(0xD41C); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD41D); lcd_spi_write_hf_word_data(0x007e);
    lcd_spi_write_hf_word_cmd(0xD41E); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD41F); lcd_spi_write_hf_word_data(0x00bc);
    lcd_spi_write_hf_word_cmd(0xD420); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD421); lcd_spi_write_hf_word_data(0x00e1);
    lcd_spi_write_hf_word_cmd(0xD422); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD423); lcd_spi_write_hf_word_data(0x0010);
    lcd_spi_write_hf_word_cmd(0xD424); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD425); lcd_spi_write_hf_word_data(0x0031);
    lcd_spi_write_hf_word_cmd(0xD426); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD427); lcd_spi_write_hf_word_data(0x005a);
    lcd_spi_write_hf_word_cmd(0xD428); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD429); lcd_spi_write_hf_word_data(0x0073);
    lcd_spi_write_hf_word_cmd(0xD42A); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD42B); lcd_spi_write_hf_word_data(0x0094);
    lcd_spi_write_hf_word_cmd(0xD42C); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD42D); lcd_spi_write_hf_word_data(0x009f);
    lcd_spi_write_hf_word_cmd(0xD42E); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD42F); lcd_spi_write_hf_word_data(0x00b3);
    lcd_spi_write_hf_word_cmd(0xD430); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD431); lcd_spi_write_hf_word_data(0x00b9);
    lcd_spi_write_hf_word_cmd(0xD432); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD433); lcd_spi_write_hf_word_data(0x00c1);
			
   //Negative Gamma for Green
    lcd_spi_write_hf_word_cmd(0xD500); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD501); lcd_spi_write_hf_word_data(0x0037);
    lcd_spi_write_hf_word_cmd(0xD502); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD503); lcd_spi_write_hf_word_data(0x0052);
    lcd_spi_write_hf_word_cmd(0xD504); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD505); lcd_spi_write_hf_word_data(0x007b);
    lcd_spi_write_hf_word_cmd(0xD506); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD507); lcd_spi_write_hf_word_data(0x0099);
    lcd_spi_write_hf_word_cmd(0xD508); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD509); lcd_spi_write_hf_word_data(0x00b1);
    lcd_spi_write_hf_word_cmd(0xD50A); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD50B); lcd_spi_write_hf_word_data(0x00d2);
    lcd_spi_write_hf_word_cmd(0xD50C); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD50D); lcd_spi_write_hf_word_data(0x00f6);
    lcd_spi_write_hf_word_cmd(0xD50E); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD50F); lcd_spi_write_hf_word_data(0x0027);
    lcd_spi_write_hf_word_cmd(0xD510); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD511); lcd_spi_write_hf_word_data(0x004e);
    lcd_spi_write_hf_word_cmd(0xD512); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD513); lcd_spi_write_hf_word_data(0x008c);
    lcd_spi_write_hf_word_cmd(0xD514); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD515); lcd_spi_write_hf_word_data(0x00be);
    lcd_spi_write_hf_word_cmd(0xD516); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD517); lcd_spi_write_hf_word_data(0x000b);
    lcd_spi_write_hf_word_cmd(0xD518); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD519); lcd_spi_write_hf_word_data(0x0048);
    lcd_spi_write_hf_word_cmd(0xD51A); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD51B); lcd_spi_write_hf_word_data(0x004a);
    lcd_spi_write_hf_word_cmd(0xD51C); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD51D); lcd_spi_write_hf_word_data(0x007e);
    lcd_spi_write_hf_word_cmd(0xD51E); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD51F); lcd_spi_write_hf_word_data(0x00bc);
    lcd_spi_write_hf_word_cmd(0xD520); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD521); lcd_spi_write_hf_word_data(0x00e1);
    lcd_spi_write_hf_word_cmd(0xD522); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD523); lcd_spi_write_hf_word_data(0x0010);
    lcd_spi_write_hf_word_cmd(0xD524); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD525); lcd_spi_write_hf_word_data(0x0031);
    lcd_spi_write_hf_word_cmd(0xD526); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD527); lcd_spi_write_hf_word_data(0x005a);
    lcd_spi_write_hf_word_cmd(0xD528); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD529); lcd_spi_write_hf_word_data(0x0073);
    lcd_spi_write_hf_word_cmd(0xD52A); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD52B); lcd_spi_write_hf_word_data(0x0094);
    lcd_spi_write_hf_word_cmd(0xD52C); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD52D); lcd_spi_write_hf_word_data(0x009f);
    lcd_spi_write_hf_word_cmd(0xD52E); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD52F); lcd_spi_write_hf_word_data(0x00b3);
    lcd_spi_write_hf_word_cmd(0xD530); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD531); lcd_spi_write_hf_word_data(0x00b9);
    lcd_spi_write_hf_word_cmd(0xD532); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD533); lcd_spi_write_hf_word_data(0x00c1);

    //Negative Gamma for Blue
    lcd_spi_write_hf_word_cmd(0xD600); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD601); lcd_spi_write_hf_word_data(0x0037);
    lcd_spi_write_hf_word_cmd(0xD602); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD603); lcd_spi_write_hf_word_data(0x0052);
    lcd_spi_write_hf_word_cmd(0xD604); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD605); lcd_spi_write_hf_word_data(0x007b);
    lcd_spi_write_hf_word_cmd(0xD606); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD607); lcd_spi_write_hf_word_data(0x0099);
    lcd_spi_write_hf_word_cmd(0xD608); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD609); lcd_spi_write_hf_word_data(0x00b1);
    lcd_spi_write_hf_word_cmd(0xD60A); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD60B); lcd_spi_write_hf_word_data(0x00d2);
    lcd_spi_write_hf_word_cmd(0xD60C); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xD60D); lcd_spi_write_hf_word_data(0x00f6);
    lcd_spi_write_hf_word_cmd(0xD60E); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD60F); lcd_spi_write_hf_word_data(0x0027);
    lcd_spi_write_hf_word_cmd(0xD610); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD611); lcd_spi_write_hf_word_data(0x004e);
    lcd_spi_write_hf_word_cmd(0xD612); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD613); lcd_spi_write_hf_word_data(0x008c);
    lcd_spi_write_hf_word_cmd(0xD614); lcd_spi_write_hf_word_data(0x0001);
    lcd_spi_write_hf_word_cmd(0xD615); lcd_spi_write_hf_word_data(0x00be);
    lcd_spi_write_hf_word_cmd(0xD616); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD617); lcd_spi_write_hf_word_data(0x000b);
    lcd_spi_write_hf_word_cmd(0xD618); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD619); lcd_spi_write_hf_word_data(0x0048);
    lcd_spi_write_hf_word_cmd(0xD61A); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD61B); lcd_spi_write_hf_word_data(0x004a);
    lcd_spi_write_hf_word_cmd(0xD61C); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD61D); lcd_spi_write_hf_word_data(0x007e);
    lcd_spi_write_hf_word_cmd(0xD61E); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD61F); lcd_spi_write_hf_word_data(0x00bc);
    lcd_spi_write_hf_word_cmd(0xD620); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xD621); lcd_spi_write_hf_word_data(0x00e1);
    lcd_spi_write_hf_word_cmd(0xD622); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD623); lcd_spi_write_hf_word_data(0x0010);
    lcd_spi_write_hf_word_cmd(0xD624); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD625); lcd_spi_write_hf_word_data(0x0031);
    lcd_spi_write_hf_word_cmd(0xD626); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD627); lcd_spi_write_hf_word_data(0x005a);
    lcd_spi_write_hf_word_cmd(0xD628); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD629); lcd_spi_write_hf_word_data(0x0073);
    lcd_spi_write_hf_word_cmd(0xD62A); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD62B); lcd_spi_write_hf_word_data(0x0094);
    lcd_spi_write_hf_word_cmd(0xD62C); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD62D); lcd_spi_write_hf_word_data(0x009f);
    lcd_spi_write_hf_word_cmd(0xD62E); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD62F); lcd_spi_write_hf_word_data(0x00b3);
    lcd_spi_write_hf_word_cmd(0xD630); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD631); lcd_spi_write_hf_word_data(0x00b9);
    lcd_spi_write_hf_word_cmd(0xD632); lcd_spi_write_hf_word_data(0x0003);
    lcd_spi_write_hf_word_cmd(0xD633); lcd_spi_write_hf_word_data(0x00c1);
  			
    lcd_spi_write_hf_word_cmd(0xf000); lcd_spi_write_hf_word_data(0x0055);
    lcd_spi_write_hf_word_cmd(0xf001); lcd_spi_write_hf_word_data(0x00aa);
    lcd_spi_write_hf_word_cmd(0xf002); lcd_spi_write_hf_word_data(0x0052);
    lcd_spi_write_hf_word_cmd(0xf003); lcd_spi_write_hf_word_data(0x0008);
    lcd_spi_write_hf_word_cmd(0xf004); lcd_spi_write_hf_word_data(0x0000);
  
    lcd_spi_write_hf_word_cmd(0xB000); lcd_spi_write_hf_word_data(0x0000); // Setting AVDD Voltage 0008 0009
    lcd_spi_write_hf_word_cmd(0xB001); lcd_spi_write_hf_word_data(0x0005);
    lcd_spi_write_hf_word_cmd(0xB002); lcd_spi_write_hf_word_data(0x0002);
    lcd_spi_write_hf_word_cmd(0xB003); lcd_spi_write_hf_word_data(0x0005);
    lcd_spi_write_hf_word_cmd(0xB004); lcd_spi_write_hf_word_data(0x0002);
  
    lcd_spi_write_hf_word_cmd(0xb600); lcd_spi_write_hf_word_data(0x000A);
  
    lcd_spi_write_hf_word_cmd(0xB700); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xB701); lcd_spi_write_hf_word_data(0x0070);
  
    lcd_spi_write_hf_word_cmd(0xB800); lcd_spi_write_hf_word_data(0x0001); // Power Control for VCL
    lcd_spi_write_hf_word_cmd(0xB801); lcd_spi_write_hf_word_data(0x0005);
    lcd_spi_write_hf_word_cmd(0xB802); lcd_spi_write_hf_word_data(0x0005);
    lcd_spi_write_hf_word_cmd(0xB803); lcd_spi_write_hf_word_data(0x0005);
  
  
    lcd_spi_write_hf_word_cmd(0xBC00); lcd_spi_write_hf_word_data(0x0000); // Setting VGMP and VGSP Voltage
    lcd_spi_write_hf_word_cmd(0xBC01); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xBC02); lcd_spi_write_hf_word_data(0x0000);
  
    lcd_spi_write_hf_word_cmd(0xcC00); lcd_spi_write_hf_word_data(0x0003); 
    lcd_spi_write_hf_word_cmd(0xcC01); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0xcC02); lcd_spi_write_hf_word_data(0x0000);
  
    lcd_spi_write_hf_word_cmd(0xBD00); lcd_spi_write_hf_word_data(0x0001); // Setting VGMN and VGSN Voltage
    lcd_spi_write_hf_word_cmd(0xBD01); lcd_spi_write_hf_word_data(0x0084);
    lcd_spi_write_hf_word_cmd(0xBD02); lcd_spi_write_hf_word_data(0x0007);
    lcd_spi_write_hf_word_cmd(0xBD03); lcd_spi_write_hf_word_data(0x0031);
    lcd_spi_write_hf_word_cmd(0xBD04); lcd_spi_write_hf_word_data(0x0000);
  
    lcd_spi_write_hf_word_cmd(0xBA00); lcd_spi_write_hf_word_data(0x0001);
//  
//    lcd_spi_write_hf_word_cmd(0xff00); lcd_spi_write_hf_word_data(0x00aa);
//    lcd_spi_write_hf_word_cmd(0xff01); lcd_spi_write_hf_word_data(0x0055);
//    lcd_spi_write_hf_word_cmd(0xff02); lcd_spi_write_hf_word_data(0x0025);
//    lcd_spi_write_hf_word_cmd(0xff03); lcd_spi_write_hf_word_data(0x0001);
//  
//    lcd_spi_write_hf_word_cmd(0xf304); lcd_spi_write_hf_word_data(0x0011);
//    lcd_spi_write_hf_word_cmd(0xf306); lcd_spi_write_hf_word_data(0x0010);
//    lcd_spi_write_hf_word_cmd(0xf408); lcd_spi_write_hf_word_data(0x0000);
  
    lcd_spi_write_hf_word_cmd(0X3500); lcd_spi_write_hf_word_data(0x0000);
//    lcd_spi_write_hf_word_cmd(0X3600); lcd_spi_write_hf_word_data(0x0000);
    lcd_spi_write_hf_word_cmd(0x3a00); lcd_spi_write_hf_word_data(0x0055); //55:16ǋ   66:18ǋ   77:24ǋ
    //lcd_spi_write_hf_word_cmd(0x4a00); lcd_spi_write_hf_word_data(0x0000);
  
   
    lcd_spi_write_hf_word_cmd(0x1100);
    delay_ms(120);
    lcd_spi_write_hf_word_cmd(0x2900);
}

static void lcd_nt35512_backlight_io_init(void)
{
	gpio_dev_unmap(34);
	bk_gpio_set_capacity(34, 0);
	BK_LOG_ON_ERR(bk_gpio_enable_output(34));
	BK_LOG_ON_ERR(bk_gpio_pull_down(34));
}
static void lcd_nt35512_init(void)
{
	lcd_nt35512_backlight_io_init();
	lcd_spi_init_gpio();
	lcd_nt35512_config();
}


static void nt35512_lcd_backlight_open(void)
{
	BK_LOG_ON_ERR(bk_gpio_pull_up(34));
	// pull up gpio34, enable lcd backlight control
	bk_gpio_set_output_high(34);
}

static void nt35512_lcd_backlight_close(void)
{
	bk_gpio_set_output_low(34);
}

static bk_err_t nt35512_lcd_off(void)
{
	lcd_spi_write_hf_word_cmd(0x2800);
	delay_ms(100);

	lcd_spi_write_hf_word_cmd(0x1100);
	delay_ms(100);

	return BK_OK;
}

const lcd_device_t lcd_device_nt35512 =
{
	.id = LCD_DEVICE_NT35512,
	.name = "nt35512",
	.type = LCD_TYPE_RGB565,
	.ppi = PPI_480X800,
	.rgb = &lcd_rgb,
	.backlight_open = nt35512_lcd_backlight_open,
	.backlight_set = NULL,
	.init = lcd_nt35512_init,
	.backlight_close = nt35512_lcd_backlight_close,
	.lcd_off = nt35512_lcd_off,
};



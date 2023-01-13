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
#include "lcd_disp_hal.h"
//#include "include/bk_lcd_commands.h"

//#if CONFIG_LCD_nt35510_mcu

#define TAG "nt35510_mcu"
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)

//uint8_t madctl_val= 0x48;


static void nt35510_mcu_send_data(uint8_t param_count, uint16_t command, uint16_t param)
{
    uint8_t cmd_h = (command & 0xff00) >> 8;
    uint8_t cmd_l = (command & 0x00ff);
    uint32_t param_32[2];
    uint8_t param_h;
    uint8_t param_l;

    if(param_count > 1)
    {
        LOGE("[%s][%d] param count over the max value:%d\r\n", param_count);
        return ;
    }

    if(param_count > 0)
    {
        param_h = (param & 0xff00) >> 8;
        param_l = (param & 0x00ff);
        param_32[0] = param_h;
        param_32[1] = param_l;
    }
    
    lcd_hal_8080_cmd_send(0, cmd_h, 0);
    lcd_hal_8080_cmd_send(param_count*2, cmd_l, param_32);
}


static bk_err_t lcd_nt35510_mcu_swap_xy(bool swap_axes)
{
    return BK_OK;
    #if 0
    if (swap_axes) {
        madctl_val |= (1 << 5);
    } else {
        madctl_val &= ~(1 << 5);
    }
    
    uint32_t madct[1] = {madctl_val};
    lcd_hal_8080_cmd_send(1, MEM_ACCESS_CTRL, (uint32_t *)madct);  //MV=1: 36h 0x28 or 0x68 or 0xB8

    return BK_OK;
#endif
}

static bk_err_t lcd_nt35510_mcu_mirror( bool mirror_x, bool mirror_y)
{
    return BK_OK;
#if 0
    if (mirror_x) {
        madctl_val |= (1 << 6); // MX=1 36h 0x48 
    } else {
        madctl_val &= ~(1 << 6);
    }
    if (mirror_y) {
        madctl_val |= (1 << 7); // MY=1 36h 0x88 
    } else {
        madctl_val &= ~(1 << 7);
    }
    uint32_t madctl[1] = {madctl_val};
    lcd_hal_8080_cmd_send(1, MEM_ACCESS_CTRL, (uint32_t *)madctl);
    return BK_OK;
#endif
}

bk_err_t nt35510_mcu_lcd_on(void)
{
    nt35510_mcu_send_data(0, 0x2900, 0);
    return BK_OK;
}

static bk_err_t nt35510_mcu_lcd_off(void)
{
    nt35510_mcu_send_data(0, 0x2800, 0);
    return BK_OK;
}

bk_err_t nt35510_mcu_swreset(void)
{
    lcd_hal_8080_cmd_send(0, 0x0100, 0);
    rtos_delay_milliseconds(10);
    return BK_OK;
}

void lcd_nt35510_mcu_init(void)
{
    LOGI("%s\n", __func__);

    rtos_delay_milliseconds(131);
    rtos_delay_milliseconds(10);

    nt35510_mcu_send_data(1, 0xF000, 0x0055);
    nt35510_mcu_send_data(1, 0xF001, 0x00AA);
    nt35510_mcu_send_data(1, 0xF002, 0x0052);
    nt35510_mcu_send_data(1, 0xF003, 0x0008);
    nt35510_mcu_send_data(1, 0xF004, 0x0001);
    nt35510_mcu_send_data(1, 0xB600, 0x0034);
    nt35510_mcu_send_data(1, 0xB601, 0x0034);
    nt35510_mcu_send_data(1, 0xB602, 0x0034);

    //Set AVDD 5.2V
    nt35510_mcu_send_data(1, 0xB000, 0x0009);
    nt35510_mcu_send_data(1, 0xB001, 0x0009);
    nt35510_mcu_send_data(1, 0xB002, 0x0009);


    //Set AVEE Ratio
    nt35510_mcu_send_data(1, 0xB700, 0x0024);
    nt35510_mcu_send_data(1, 0xB701, 0x0024);
    nt35510_mcu_send_data(1, 0xB702, 0x0024);

    nt35510_mcu_send_data(1, 0xB100, 0x0009);
    nt35510_mcu_send_data(1, 0xB101, 0x0009);
    nt35510_mcu_send_data(1, 0xB102, 0x0009);

    nt35510_mcu_send_data(1, 0xB800, 0x0034);

    nt35510_mcu_send_data(1, 0xB200, 0x0000);

    //VGH Ratio
    nt35510_mcu_send_data(1, 0xB900, 0x0024);
    nt35510_mcu_send_data(1, 0xB901, 0x0024);
    nt35510_mcu_send_data(1, 0xB902, 0x0024);

    //VGH=15V(1V/step)    Free pump
    nt35510_mcu_send_data(1, 0xB300, 0x0005);     //08
    nt35510_mcu_send_data(1, 0xB301, 0x0005);     //08
    nt35510_mcu_send_data(1, 0xB302, 0x0005);     //08

    nt35510_mcu_send_data(1, 0xBF00, 0x0001); //VGH:7~18V

    nt35510_mcu_send_data(1, 0xBA00, 0x0034);
    nt35510_mcu_send_data(1, 0xBA01, 0x0034);
    nt35510_mcu_send_data(1, 0xBA02, 0x0034);

    nt35510_mcu_send_data(1, 0xB500, 0x000B);
    nt35510_mcu_send_data(1, 0xB501, 0x000B);
    nt35510_mcu_send_data(1, 0xB502, 0x000B);

    //VGMP/VGSP=4.5V/0V
    nt35510_mcu_send_data(1, 0xBC00, 0x0000);     //00
    nt35510_mcu_send_data(1, 0xBC01, 0x00A3);     //C8 =5.5V/90=4.8V
    nt35510_mcu_send_data(1, 0xBC02, 0x0000);     //01


    //VGMN/VGSN=-4.5V/0V
    nt35510_mcu_send_data(1, 0xBD00, 0x0000); //00
    nt35510_mcu_send_data(1, 0xBD01, 0x00A3); //90
    nt35510_mcu_send_data(1, 0xBD02, 0x0000);

    nt35510_mcu_send_data(1, 0xBE00, 0x0000);  //VCOM
    nt35510_mcu_send_data(1, 0xBE01, 0x0050); //HSD:64;Novatek:50=-1.0V, 80  5f
                /////////////////////////


    //Gamma (R+)
    nt35510_mcu_send_data(1, 0xD100, 0x0000);
    nt35510_mcu_send_data(1, 0xD101, 0x0037);
    nt35510_mcu_send_data(1, 0xD102, 0x0000);
    nt35510_mcu_send_data(1, 0xD103, 0x0052);
    nt35510_mcu_send_data(1, 0xD104, 0x0000);
    nt35510_mcu_send_data(1, 0xD105, 0x007B);
    nt35510_mcu_send_data(1, 0xD106, 0x0000);
    nt35510_mcu_send_data(1, 0xD107, 0x0099);
    nt35510_mcu_send_data(1, 0xD108, 0x0000);
    nt35510_mcu_send_data(1, 0xD109, 0x00B1);
    nt35510_mcu_send_data(1, 0xD10A, 0x0000);
    nt35510_mcu_send_data(1, 0xD10B, 0x00D2);
    nt35510_mcu_send_data(1, 0xD10C, 0x0000);
    nt35510_mcu_send_data(1, 0xD10D, 0x00F6);
    nt35510_mcu_send_data(1, 0xD10E, 0x0001);
    nt35510_mcu_send_data(1, 0xD10F, 0x0027);
    nt35510_mcu_send_data(1, 0xD110, 0x0001);
    nt35510_mcu_send_data(1, 0xD111, 0x004E);
    nt35510_mcu_send_data(1, 0xD112, 0x0001);
    nt35510_mcu_send_data(1, 0xD113, 0x008C);
    nt35510_mcu_send_data(1, 0xD114, 0x0001);
    nt35510_mcu_send_data(1, 0xD115, 0x00BE);
    nt35510_mcu_send_data(1, 0xD116, 0x0002);
    nt35510_mcu_send_data(1, 0xD117, 0x000B);
    nt35510_mcu_send_data(1, 0xD118, 0x0002);
    nt35510_mcu_send_data(1, 0xD119, 0x0048);
    nt35510_mcu_send_data(1, 0xD11A, 0x0002);
    nt35510_mcu_send_data(1, 0xD11B, 0x004A);
    nt35510_mcu_send_data(1, 0xD11C, 0x0002);
    nt35510_mcu_send_data(1, 0xD11D, 0x007E);
    nt35510_mcu_send_data(1, 0xD11E, 0x0002);
    nt35510_mcu_send_data(1, 0xD11F, 0x00BC);
    nt35510_mcu_send_data(1, 0xD120, 0x0002);
    nt35510_mcu_send_data(1, 0xD121, 0x00E1);
    nt35510_mcu_send_data(1, 0xD122, 0x0003);
    nt35510_mcu_send_data(1, 0xD123, 0x0010);
    nt35510_mcu_send_data(1, 0xD124, 0x0003);
    nt35510_mcu_send_data(1, 0xD125, 0x0031);
    nt35510_mcu_send_data(1, 0xD126, 0x0003);
    nt35510_mcu_send_data(1, 0xD127, 0x005A);
    nt35510_mcu_send_data(1, 0xD128, 0x0003);
    nt35510_mcu_send_data(1, 0xD129, 0x0073);
    nt35510_mcu_send_data(1, 0xD12A, 0x0003);
    nt35510_mcu_send_data(1, 0xD12B, 0x0094);
    nt35510_mcu_send_data(1, 0xD12C, 0x0003);
    nt35510_mcu_send_data(1, 0xD12D, 0x009F);
    nt35510_mcu_send_data(1, 0xD12E, 0x0003);
    nt35510_mcu_send_data(1, 0xD12F, 0x00B3);
    nt35510_mcu_send_data(1, 0xD130, 0x0003);
    nt35510_mcu_send_data(1, 0xD131, 0x00B9);
    nt35510_mcu_send_data(1, 0xD132, 0x0003);
    nt35510_mcu_send_data(1, 0xD133, 0x00C1);

    //Gamma (G+)
    nt35510_mcu_send_data(1, 0xD200, 0x0000);
    nt35510_mcu_send_data(1, 0xD201, 0x0037);
    nt35510_mcu_send_data(1, 0xD202, 0x0000);
    nt35510_mcu_send_data(1, 0xD203, 0x0052);
    nt35510_mcu_send_data(1, 0xD204, 0x0000);
    nt35510_mcu_send_data(1, 0xD205, 0x007B);
    nt35510_mcu_send_data(1, 0xD206, 0x0000);
    nt35510_mcu_send_data(1, 0xD207, 0x0099);
    nt35510_mcu_send_data(1, 0xD208, 0x0000);
    nt35510_mcu_send_data(1, 0xD209, 0x00B1);
    nt35510_mcu_send_data(1, 0xD20A, 0x0000);
    nt35510_mcu_send_data(1, 0xD20B, 0x00D2);
    nt35510_mcu_send_data(1, 0xD20C, 0x0000);
    nt35510_mcu_send_data(1, 0xD20D, 0x00F6);
    nt35510_mcu_send_data(1, 0xD20E, 0x0001);
    nt35510_mcu_send_data(1, 0xD20F, 0x0027);
    nt35510_mcu_send_data(1, 0xD210, 0x0001);
    nt35510_mcu_send_data(1, 0xD211, 0x004E);
    nt35510_mcu_send_data(1, 0xD212, 0x0001);
    nt35510_mcu_send_data(1, 0xD213, 0x008C);
    nt35510_mcu_send_data(1, 0xD214, 0x0001);
    nt35510_mcu_send_data(1, 0xD215, 0x00BE);
    nt35510_mcu_send_data(1, 0xD216, 0x0002);
    nt35510_mcu_send_data(1, 0xD217, 0x000B);
    nt35510_mcu_send_data(1, 0xD218, 0x0002);
    nt35510_mcu_send_data(1, 0xD219, 0x0048);
    nt35510_mcu_send_data(1, 0xD21A, 0x0002);
    nt35510_mcu_send_data(1, 0xD21B, 0x004A);
    nt35510_mcu_send_data(1, 0xD21C, 0x0002);
    nt35510_mcu_send_data(1, 0xD21D, 0x007E);
    nt35510_mcu_send_data(1, 0xD21E, 0x0002);
    nt35510_mcu_send_data(1, 0xD21F, 0x00BC);
    nt35510_mcu_send_data(1, 0xD220, 0x0002);
    nt35510_mcu_send_data(1, 0xD221, 0x00E1);
    nt35510_mcu_send_data(1, 0xD222, 0x0003);
    nt35510_mcu_send_data(1, 0xD223, 0x0010);
    nt35510_mcu_send_data(1, 0xD224, 0x0003);
    nt35510_mcu_send_data(1, 0xD225, 0x0031);
    nt35510_mcu_send_data(1, 0xD226, 0x0003);
    nt35510_mcu_send_data(1, 0xD227, 0x005A);
    nt35510_mcu_send_data(1, 0xD228, 0x0003);
    nt35510_mcu_send_data(1, 0xD229, 0x0073);
    nt35510_mcu_send_data(1, 0xD22A, 0x0003);
    nt35510_mcu_send_data(1, 0xD22B, 0x0094);
    nt35510_mcu_send_data(1, 0xD22C, 0x0003);
    nt35510_mcu_send_data(1, 0xD22D, 0x009F);
    nt35510_mcu_send_data(1, 0xD22E, 0x0003);
    nt35510_mcu_send_data(1, 0xD22F, 0x00B3);
    nt35510_mcu_send_data(1, 0xD230, 0x0003);
    nt35510_mcu_send_data(1, 0xD231, 0x00B9);
    nt35510_mcu_send_data(1, 0xD232, 0x0003);
    nt35510_mcu_send_data(1, 0xD233, 0x00C1);

    //Gamma (B+)
    nt35510_mcu_send_data(1, 0xD300, 0x0000);
    nt35510_mcu_send_data(1, 0xD301, 0x0037);
    nt35510_mcu_send_data(1, 0xD302, 0x0000);
    nt35510_mcu_send_data(1, 0xD303, 0x0052);
    nt35510_mcu_send_data(1, 0xD304, 0x0000);
    nt35510_mcu_send_data(1, 0xD305, 0x007B);
    nt35510_mcu_send_data(1, 0xD306, 0x0000);
    nt35510_mcu_send_data(1, 0xD307, 0x0099);
    nt35510_mcu_send_data(1, 0xD308, 0x0000);
    nt35510_mcu_send_data(1, 0xD309, 0x00B1);
    nt35510_mcu_send_data(1, 0xD30A, 0x0000);
    nt35510_mcu_send_data(1, 0xD30B, 0x00D2);
    nt35510_mcu_send_data(1, 0xD30C, 0x0000);
    nt35510_mcu_send_data(1, 0xD30D, 0x00F6);
    nt35510_mcu_send_data(1, 0xD30E, 0x0001);
    nt35510_mcu_send_data(1, 0xD30F, 0x0027);
    nt35510_mcu_send_data(1, 0xD310, 0x0001);
    nt35510_mcu_send_data(1, 0xD311, 0x004E);
    nt35510_mcu_send_data(1, 0xD312, 0x0001);
    nt35510_mcu_send_data(1, 0xD313, 0x008C);
    nt35510_mcu_send_data(1, 0xD314, 0x0001);
    nt35510_mcu_send_data(1, 0xD315, 0x00BE);
    nt35510_mcu_send_data(1, 0xD316, 0x0002);
    nt35510_mcu_send_data(1, 0xD317, 0x000B);
    nt35510_mcu_send_data(1, 0xD318, 0x0002);
    nt35510_mcu_send_data(1, 0xD319, 0x0048);
    nt35510_mcu_send_data(1, 0xD31A, 0x0002);
    nt35510_mcu_send_data(1, 0xD31B, 0x004A);
    nt35510_mcu_send_data(1, 0xD31C, 0x0002);
    nt35510_mcu_send_data(1, 0xD31D, 0x007E);
    nt35510_mcu_send_data(1, 0xD31E, 0x0002);
    nt35510_mcu_send_data(1, 0xD31F, 0x00BC);
    nt35510_mcu_send_data(1, 0xD320, 0x0002);
    nt35510_mcu_send_data(1, 0xD321, 0x00E1);
    nt35510_mcu_send_data(1, 0xD322, 0x0003);
    nt35510_mcu_send_data(1, 0xD323, 0x0010);
    nt35510_mcu_send_data(1, 0xD324, 0x0003);
    nt35510_mcu_send_data(1, 0xD325, 0x0031);
    nt35510_mcu_send_data(1, 0xD326, 0x0003);
    nt35510_mcu_send_data(1, 0xD327, 0x005A);
    nt35510_mcu_send_data(1, 0xD328, 0x0003);
    nt35510_mcu_send_data(1, 0xD329, 0x0073);
    nt35510_mcu_send_data(1, 0xD32A, 0x0003);
    nt35510_mcu_send_data(1, 0xD32B, 0x0094);
    nt35510_mcu_send_data(1, 0xD32C, 0x0003);
    nt35510_mcu_send_data(1, 0xD32D, 0x009F);
    nt35510_mcu_send_data(1, 0xD32E, 0x0003);
    nt35510_mcu_send_data(1, 0xD32F, 0x00B3);
    nt35510_mcu_send_data(1, 0xD330, 0x0003);
    nt35510_mcu_send_data(1, 0xD331, 0x00B9);
    nt35510_mcu_send_data(1, 0xD332, 0x0003);
    nt35510_mcu_send_data(1, 0xD333, 0x00C1);
    //Gamma (R-)
    nt35510_mcu_send_data(1, 0xD400, 0x0000);
    nt35510_mcu_send_data(1, 0xD401, 0x0037);
    nt35510_mcu_send_data(1, 0xD402, 0x0000);
    nt35510_mcu_send_data(1, 0xD403, 0x0052);
    nt35510_mcu_send_data(1, 0xD404, 0x0000);
    nt35510_mcu_send_data(1, 0xD405, 0x007B);
    nt35510_mcu_send_data(1, 0xD406, 0x0000);
    nt35510_mcu_send_data(1, 0xD407, 0x0099);
    nt35510_mcu_send_data(1, 0xD408, 0x0000);
    nt35510_mcu_send_data(1, 0xD409, 0x00B1);
    nt35510_mcu_send_data(1, 0xD40A, 0x0000);
    nt35510_mcu_send_data(1, 0xD40B, 0x00D2);
    nt35510_mcu_send_data(1, 0xD40C, 0x0000);
    nt35510_mcu_send_data(1, 0xD40D, 0x00F6);
    nt35510_mcu_send_data(1, 0xD40E, 0x0001);
    nt35510_mcu_send_data(1, 0xD40F, 0x0027);
    nt35510_mcu_send_data(1, 0xD410, 0x0001);
    nt35510_mcu_send_data(1, 0xD411, 0x004E);
    nt35510_mcu_send_data(1, 0xD412, 0x0001);
    nt35510_mcu_send_data(1, 0xD413, 0x008C);
    nt35510_mcu_send_data(1, 0xD414, 0x0001);
    nt35510_mcu_send_data(1, 0xD415, 0x00BE);
    nt35510_mcu_send_data(1, 0xD416, 0x0002);
    nt35510_mcu_send_data(1, 0xD417, 0x000B);
    nt35510_mcu_send_data(1, 0xD418, 0x0002);
    nt35510_mcu_send_data(1, 0xD419, 0x0048);
    nt35510_mcu_send_data(1, 0xD41A, 0x0002);
    nt35510_mcu_send_data(1, 0xD41B, 0x004A);
    nt35510_mcu_send_data(1, 0xD41C, 0x0002);
    nt35510_mcu_send_data(1, 0xD41D, 0x007E);
    nt35510_mcu_send_data(1, 0xD41E, 0x0002);
    nt35510_mcu_send_data(1, 0xD41F, 0x00BC);
    nt35510_mcu_send_data(1, 0xD420, 0x0002);
    nt35510_mcu_send_data(1, 0xD421, 0x00E1);
    nt35510_mcu_send_data(1, 0xD422, 0x0003);
    nt35510_mcu_send_data(1, 0xD423, 0x0010);
    nt35510_mcu_send_data(1, 0xD424, 0x0003);
    nt35510_mcu_send_data(1, 0xD425, 0x0031);
    nt35510_mcu_send_data(1, 0xD426, 0x0003);
    nt35510_mcu_send_data(1, 0xD427, 0x005A);
    nt35510_mcu_send_data(1, 0xD428, 0x0003);
    nt35510_mcu_send_data(1, 0xD429, 0x0073);
    nt35510_mcu_send_data(1, 0xD42A, 0x0003);
    nt35510_mcu_send_data(1, 0xD42B, 0x0094);
    nt35510_mcu_send_data(1, 0xD42C, 0x0003);
    nt35510_mcu_send_data(1, 0xD42D, 0x009F);
    nt35510_mcu_send_data(1, 0xD42E, 0x0003);
    nt35510_mcu_send_data(1, 0xD42F, 0x00B3);
    nt35510_mcu_send_data(1, 0xD430, 0x0003);
    nt35510_mcu_send_data(1, 0xD431, 0x00B9);
    nt35510_mcu_send_data(1, 0xD432, 0x0003);
    nt35510_mcu_send_data(1, 0xD433, 0x00C1);
    //Gamma (G-)
    nt35510_mcu_send_data(1, 0xD500, 0x0000);
    nt35510_mcu_send_data(1, 0xD501, 0x0037);
    nt35510_mcu_send_data(1, 0xD502, 0x0000);
    nt35510_mcu_send_data(1, 0xD503, 0x0052);
    nt35510_mcu_send_data(1, 0xD504, 0x0000);
    nt35510_mcu_send_data(1, 0xD505, 0x007B);
    nt35510_mcu_send_data(1, 0xD506, 0x0000);
    nt35510_mcu_send_data(1, 0xD507, 0x0099);
    nt35510_mcu_send_data(1, 0xD508, 0x0000);
    nt35510_mcu_send_data(1, 0xD509, 0x00B1);
    nt35510_mcu_send_data(1, 0xD50A, 0x0000);
    nt35510_mcu_send_data(1, 0xD50B, 0x00D2);
    nt35510_mcu_send_data(1, 0xD50C, 0x0000);
    nt35510_mcu_send_data(1, 0xD50D, 0x00F6);
    nt35510_mcu_send_data(1, 0xD50E, 0x0001);
    nt35510_mcu_send_data(1, 0xD50F, 0x0027);
    nt35510_mcu_send_data(1, 0xD510, 0x0001);
    nt35510_mcu_send_data(1, 0xD511, 0x004E);
    nt35510_mcu_send_data(1, 0xD512, 0x0001);
    nt35510_mcu_send_data(1, 0xD513, 0x008C);
    nt35510_mcu_send_data(1, 0xD514, 0x0001);
    nt35510_mcu_send_data(1, 0xD515, 0x00BE);
    nt35510_mcu_send_data(1, 0xD516, 0x0002);
    nt35510_mcu_send_data(1, 0xD517, 0x000B);
    nt35510_mcu_send_data(1, 0xD518, 0x0002);
    nt35510_mcu_send_data(1, 0xD519, 0x0048);
    nt35510_mcu_send_data(1, 0xD51A, 0x0002);
    nt35510_mcu_send_data(1, 0xD51B, 0x004A);
    nt35510_mcu_send_data(1, 0xD51C, 0x0002);
    nt35510_mcu_send_data(1, 0xD51D, 0x007E);
    nt35510_mcu_send_data(1, 0xD51E, 0x0002);
    nt35510_mcu_send_data(1, 0xD51F, 0x00BC);
    nt35510_mcu_send_data(1, 0xD520, 0x0002);
    nt35510_mcu_send_data(1, 0xD521, 0x00E1);
    nt35510_mcu_send_data(1, 0xD522, 0x0003);
    nt35510_mcu_send_data(1, 0xD523, 0x0010);
    nt35510_mcu_send_data(1, 0xD524, 0x0003);
    nt35510_mcu_send_data(1, 0xD525, 0x0031);
    nt35510_mcu_send_data(1, 0xD526, 0x0003);
    nt35510_mcu_send_data(1, 0xD527, 0x005A);
    nt35510_mcu_send_data(1, 0xD528, 0x0003);
    nt35510_mcu_send_data(1, 0xD529, 0x0073);
    nt35510_mcu_send_data(1, 0xD52A, 0x0003);
    nt35510_mcu_send_data(1, 0xD52B, 0x0094);
    nt35510_mcu_send_data(1, 0xD52C, 0x0003);
    nt35510_mcu_send_data(1, 0xD52D, 0x009F);
    nt35510_mcu_send_data(1, 0xD52E, 0x0003);
    nt35510_mcu_send_data(1, 0xD52F, 0x00B3);
    nt35510_mcu_send_data(1, 0xD530, 0x0003);
    nt35510_mcu_send_data(1, 0xD531, 0x00B9);
    nt35510_mcu_send_data(1, 0xD532, 0x0003);
    nt35510_mcu_send_data(1, 0xD533, 0x00C1);
    //Gamma (B-)
    nt35510_mcu_send_data(1, 0xD600, 0x0000);
    nt35510_mcu_send_data(1, 0xD601, 0x0037);
    nt35510_mcu_send_data(1, 0xD602, 0x0000);
    nt35510_mcu_send_data(1, 0xD603, 0x0052);
    nt35510_mcu_send_data(1, 0xD604, 0x0000);
    nt35510_mcu_send_data(1, 0xD605, 0x007B);
    nt35510_mcu_send_data(1, 0xD606, 0x0000);
    nt35510_mcu_send_data(1, 0xD607, 0x0099);
    nt35510_mcu_send_data(1, 0xD608, 0x0000);
    nt35510_mcu_send_data(1, 0xD609, 0x00B1);
    nt35510_mcu_send_data(1, 0xD60A, 0x0000);
    nt35510_mcu_send_data(1, 0xD60B, 0x00D2);
    nt35510_mcu_send_data(1, 0xD60C, 0x0000);
    nt35510_mcu_send_data(1, 0xD60D, 0x00F6);
    nt35510_mcu_send_data(1, 0xD60E, 0x0001);
    nt35510_mcu_send_data(1, 0xD60F, 0x0027);
    nt35510_mcu_send_data(1, 0xD610, 0x0001);
    nt35510_mcu_send_data(1, 0xD611, 0x004E);
    nt35510_mcu_send_data(1, 0xD612, 0x0001);
    nt35510_mcu_send_data(1, 0xD613, 0x008C);
    nt35510_mcu_send_data(1, 0xD614, 0x0001);
    nt35510_mcu_send_data(1, 0xD615, 0x00BE);
    nt35510_mcu_send_data(1, 0xD616, 0x0002);
    nt35510_mcu_send_data(1, 0xD617, 0x000B);
    nt35510_mcu_send_data(1, 0xD618, 0x0002);
    nt35510_mcu_send_data(1, 0xD619, 0x0048);
    nt35510_mcu_send_data(1, 0xD61A, 0x0002);
    nt35510_mcu_send_data(1, 0xD61B, 0x004A);
    nt35510_mcu_send_data(1, 0xD61C, 0x0002);
    nt35510_mcu_send_data(1, 0xD61D, 0x007E);
    nt35510_mcu_send_data(1, 0xD61E, 0x0002);
    nt35510_mcu_send_data(1, 0xD61F, 0x00BC);
    nt35510_mcu_send_data(1, 0xD620, 0x0002);
    nt35510_mcu_send_data(1, 0xD621, 0x00E1);
    nt35510_mcu_send_data(1, 0xD622, 0x0003);
    nt35510_mcu_send_data(1, 0xD623, 0x0010);
    nt35510_mcu_send_data(1, 0xD624, 0x0003);
    nt35510_mcu_send_data(1, 0xD625, 0x0031);
    nt35510_mcu_send_data(1, 0xD626, 0x0003);
    nt35510_mcu_send_data(1, 0xD627, 0x005A);
    nt35510_mcu_send_data(1, 0xD628, 0x0003);
    nt35510_mcu_send_data(1, 0xD629, 0x0073);
    nt35510_mcu_send_data(1, 0xD62A, 0x0003);
    nt35510_mcu_send_data(1, 0xD62B, 0x0094);
    nt35510_mcu_send_data(1, 0xD62C, 0x0003);
    nt35510_mcu_send_data(1, 0xD62D, 0x009F);
    nt35510_mcu_send_data(1, 0xD62E, 0x0003);
    nt35510_mcu_send_data(1, 0xD62F, 0x00B3);
    nt35510_mcu_send_data(1, 0xD630, 0x0003);
    nt35510_mcu_send_data(1, 0xD631, 0x00B9);
    nt35510_mcu_send_data(1, 0xD632, 0x0003);
    nt35510_mcu_send_data(1, 0xD633, 0x00C1);

    //PAGE0
    nt35510_mcu_send_data(1, 0xF000, 0x0055);
    nt35510_mcu_send_data(1, 0xF001, 0x00AA);
    nt35510_mcu_send_data(1, 0xF002, 0x0052);
    nt35510_mcu_send_data(1, 0xF003, 0x0008); 
    nt35510_mcu_send_data(1, 0xF004, 0x0000); 

    //480x800
     //B0

    //Dispay control
    nt35510_mcu_send_data(1, 0xB100, 0x00FC); 
    nt35510_mcu_send_data(1, 0xB101, 0x0000); // S1->S1440:00;S1440->S1:02

    //Source hold time (Nova non-used)
    nt35510_mcu_send_data(1, 0xB600, 0x000A);

    //Gate EQ control     (Nova non-used)
    nt35510_mcu_send_data(1, 0xB700, 0x0000);  //HSD:70;Nova:77     
    nt35510_mcu_send_data(1, 0xB701, 0x0070); //HSD:70;Nova:77

    nt35510_mcu_send_data(1, 0xB800, 0x0001);  
    nt35510_mcu_send_data(1, 0xB801, 0x0005); //HSD:05;Nova:07
    nt35510_mcu_send_data(1, 0xB802, 0x0005); //HSD:05;Nova:07
    nt35510_mcu_send_data(1, 0xB803, 0x0005); //HSD:05;Nova:07

            
    //Inversion mode: column
    nt35510_mcu_send_data(1, 0xBC00, 0x0000); //00: column
    nt35510_mcu_send_data(1, 0xBC01, 0x0000); //01:1dot
    nt35510_mcu_send_data(1, 0xBC02, 0x0000); 

    //LGD timing control(4H/4-Delay)
    nt35510_mcu_send_data(1, 0xCC00, 0x0003); //3H:0x50;4H:0xD0     //D
    nt35510_mcu_send_data(1, 0xCC01, 0x0000);  //HSD:05;Nova:02
    nt35510_mcu_send_data(1, 0xCC02, 0x0000); //HSD:05;Nova:50
    //Frame rate    (Nova non-used)
    nt35510_mcu_send_data(1, 0xBD00, 0x0001);
    nt35510_mcu_send_data(1, 0xBD01, 0x0084);
    nt35510_mcu_send_data(1, 0xBD02, 0x0007); //HSD:06;Nova:1C
    nt35510_mcu_send_data(1, 0xBD03, 0x0031); //HSD:04;Nova:1C
    nt35510_mcu_send_data(1, 0xBD04, 0x0000);

                
    nt35510_mcu_send_data(1, 0xBA00, 0x0001);
            
    nt35510_mcu_send_data(1, 0xBF00, 0x00AA);  
    nt35510_mcu_send_data(1, 0xBF01, 0x0055); //HSD:05;Nova:07
    nt35510_mcu_send_data(1, 0xBF02, 0x0025); //HSD:05;Nova:07
    nt35510_mcu_send_data(1, 0xBF03, 0x0001); //HSD:05;Nova:07

    //nt35510_mcu_send_data(1, 0x3600,0x0000);
    nt35510_mcu_send_data(1, 0x3600,0x00D0);

    nt35510_mcu_send_data(1, 0x3500,0x0000);
    nt35510_mcu_send_data(1, 0x4400,0x0000);
    nt35510_mcu_send_data(1, 0x4401,0x0010);

    nt35510_mcu_send_data(1, 0x3A00,0x0055);
    //Sleep out
    nt35510_mcu_send_data(0, 0x1100, 0);
    rtos_delay_milliseconds(160);

    //Display on
    nt35510_mcu_send_data(0, 0x2900, 0);
}



static void nt35510_mcu_set_display_mem_area(uint16 xs, uint16 xe, uint16 ys, uint16 ye)
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

    nt35510_mcu_send_data(1, 0x2a00, xs_h);
    nt35510_mcu_send_data(1, 0x2a01, xs_l);

    nt35510_mcu_send_data(1, 0x2a02, xe_h);
    nt35510_mcu_send_data(1, 0x2a03, xe_l);

    nt35510_mcu_send_data(1, 0x2b00, ys_h);
    nt35510_mcu_send_data(1, 0x2b01, ys_l);

    nt35510_mcu_send_data(1, 0x2b02, ye_h);
    nt35510_mcu_send_data(1, 0x2b03, ye_l);
}

static void nt35510_mcu_start_transform(void)
{
    nt35510_mcu_send_data(0, 0x2c00, 0);
}

static void nt35510_mcu_continue_transform(void)
{
    nt35510_mcu_send_data(0, 0x3c00, 0);
}


static const lcd_mcu_t lcd_mcu =
{
    .clk = LCD_40M,
    .set_xy_swap = lcd_nt35510_mcu_swap_xy,
    .set_mirror = lcd_nt35510_mcu_mirror,
    .set_display_area = nt35510_mcu_set_display_mem_area,
    .start_transform = nt35510_mcu_start_transform,
    .continue_transform = nt35510_mcu_continue_transform,
};

const lcd_device_t lcd_device_nt35510_mcu =
{
    .id = LCD_DEVICE_NT35510_MCU,
    .name = "nt35510_mcu",
    .type = LCD_TYPE_MCU8080,
    .ppi = PPI_480X800,
    .mcu = &lcd_mcu,
    .backlight_open = NULL,
    .backlight_set = NULL,
    .init = lcd_nt35510_mcu_init,
    .backlight_close = NULL,
    .lcd_off = nt35510_mcu_lcd_off,
};

//#else
//const lcd_device_t lcd_device_nt35510_mcu = 
//{};
//
//#endif

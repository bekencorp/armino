#include "vnd_cal.h"
#include <driver/hal/hal_gpio_types.h>

#define CMTAG_FROM_FLASH                1
#define CMTAG_FROM_CALI                 0

#define DEFAULT_TXID_XTAL               (0x1F)

#define EPA_ENABLE_FLAG                 0
#if EPA_ENABLE_FLAG
#define PWR_GAIN_BASE_B                 (0x5494B400)
#define PWR_GAIN_BASE_G                 (0x60B09800)
#define PWR_GAIN_BASE_BLE               (0x63E89800)
#else
#define PWR_GAIN_BASE_G                 (0xE0AB1800)
#define PWR_GAIN_BASE_B                 (0xD4A2B400)
#if CONFIG_BLE_USE_HIGH_POWER_LEVEL
#define PWR_GAIN_BASE_BLE               (0xE0A91800)
#else
#define PWR_GAIN_BASE_BLE               (0x40695800)
#endif
#endif

#define TXPWR_ELEM_INUSED               (0)
#define TXPWR_ELEM_UNUSED               (1)
#define WLAN_2_4_G_CHANNEL_NUM          (14)
#define BLE_2_4_G_CHANNEL_NUM           (40)

#define TMP_PWR_TAB_LEN                 39

#define FLAG_MASK                       (0x1u)
#define FLAG_POSI                       (7)
#define GAIN_MASK                       (0x7fu)
#define GET_TXPWR_GAIN(p)               ((p)->value & GAIN_MASK)
#define SET_TXPWR_GAIN(p, gain)         {(p)->value &= (~GAIN_MASK); \
                                         (p)->value |= ((gain)&GAIN_MASK);}


#define GET_TXPWR_FLAG(p)               (((p)->value>>FLAG_POSI)&FLAG_MASK)
#define SET_TXPWR_FLAG(p, flag)         {(p)->value &= (~(FLAG_MASK<<FLAG_POSI)); \
                                         (p)->value |= ((flag&FLAG_MASK)<<FLAG_POSI);}
#define INIT_TXPWR_VALUE(gain, flag)    {(((flag&FLAG_MASK)<<FLAG_POSI)|(gain&GAIN_MASK))}


#define TCP_PAMAP_TAB_B_LEN             (48)
const PWR_REGS cfg_tab_b[TCP_PAMAP_TAB_B_LEN] = {
    // pregain REGB<31:28> REGC<10:8> REGC<6:4> REGC<2:0> REGA<13:8> REGA<7:4> REGA<1:0>
PWRI(  0x3C ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 0   7.5  dBm
PWRI(  0x3F ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 1   8  dBm
PWRI(  0x43 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 2   8.5  dBm
PWRI(  0x47 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 3   9  dBm
PWRI(  0x4B ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 4   9.5  dBm
PWRI(  0x4F ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 5   10  dBm
PWRI(  0x54 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 6   10.5  dBm
PWRI(  0x59 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 7   11  dBm
PWRI(  0x5E ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 8   11.5  dBm
PWRI(  0x64 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 9   12  dBm
PWRI(  0x6A ,      0xA,       7,        4,         3,      0x20,     0x0,     0x2   ),   // 10  12.5  dBm
PWRI(  0x70 ,      0xA,       7,        4,         3,      0x20,     0x0,     0x2   ),   // 11  13  dBm
PWRI(  0x77 ,      0xA,       7,        4,         3,      0x20,     0x0,     0x2   ),   // 12  13.5  dBm
PWRI(  0x7E ,      0xA,       7,        4,         3,      0x20,     0x0,     0x2   ),   // 13  14  dBm
PWRI(  0x85 ,      0xA,       7,        4,         3,      0x20,     0x0,     0x2   ),   // 14  14.5  dBm
PWRI(  0x8D ,      0xA,       7,        4,         3,      0x20,     0x0,     0x2   ),   // 15  15  dBm
PWRI(  0x96 ,      0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 16  15.5  dBm
PWRI(  0x9F ,      0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 17  16  dBm
PWRI(  0xA8 ,      0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 18  16.5  dBm
PWRI(  0xB2 ,      0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 19  17  dBm
PWRI(  0xBC ,      0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 20  17.5  dBm
PWRI(  0xC8 ,      0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 21  18  dBm
PWRI(  0xD3 ,      0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 22  18.5  dBm
PWRI(  0xE0 ,      0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 23  19  dBm
PWRI(  0xED ,      0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 24  19.5  dBm
PWRI(  0xFB ,      0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 25  20  dBm
PWRI(  0x10A ,     0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 26  20.5  dBm
PWRI(  0x11A ,     0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 27  21  dBm
PWRI(  0x12B ,     0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 28  21.5  dBm
PWRI(  0x13C ,     0xA,       7,        5,         5,      0x20,     0x0,     0x2   ),   // 29  22  dBm
PWRI(  0x14F ,     0xA,       7,        6,         5,      0x20,     0x0,     0x2   ),   // 30  22.5  dBm
PWRI(  0x163 ,     0xA,       7,        6,         6,      0x20,     0x0,     0x2   ),   // 31  23  dBm
PWRI(  0x178 ,     0xA,       7,        4,         4,      0x20,     0x1,     0x1   ),   // 32  23.5  dBm
PWRI(  0x18E ,     0xA,       7,        4,         4,      0x20,     0x1,     0x1   ),   // 33  24  dBm
PWRI(  0x1A6 ,     0xA,       7,        5,         4,      0x20,     0x1,     0x1   ),   // 34  24.5  dBm
PWRI(  0x1BF ,     0xA,       7,        5,         4,      0x20,     0x1,     0x1   ),   // 35  25  dBm
PWRI(  0x1D9 ,     0xA,       7,        5,         4,      0x20,     0x1,     0x1   ),   // 36  25.5  dBm
PWRI(  0x1F5 ,     0xA,       7,        5,         4,      0x20,     0x1,     0x1   ),   // 37  26  dBm
PWRI(  0x213 ,     0xA,       7,        5,         5,      0x20,     0x1,     0x1   ),   // 38  26.5  dBm
PWRI(  0x233 ,     0xA,       7,        5,         5,      0x20,     0x1,     0x1   ),   // 39  27  dBm
PWRI(  0x254 ,     0xA,       7,        5,         5,      0x20,     0x1,     0x1   ),   // 40  27.5  dBm
PWRI(  0x277 ,     0xA,       7,        5,         5,      0x20,     0x1,     0x1   ),   // 41  28  dBm
PWRI(  0x29D ,     0xA,       7,        5,         5,      0x20,     0x1,     0x1   ),   // 42  28.5  dBm
PWRI(  0x2C4 ,     0xA,       7,        5,         5,      0x20,     0x1,     0x1   ),   // 43  29  dBm
PWRI(  0x2EE ,     0xA,       7,        5,         5,      0x20,     0x1,     0x1   ),   // 44  29.5  dBm
PWRI(  0x31B ,     0xA,       7,        5,         5,      0x20,     0x1,     0x1   ),   // 45  30  dBm
PWRI(  0x34A ,     0xA,       7,        5,         5,      0x20,     0x1,     0x1   ),   // 46  30.5  dBm
PWRI(  0x37C ,     0xA,       7,        5,         5,      0x20,     0x1,     0x1   ),   // 47  31  dBm
};

#define TCP_PAMAP_TAB_G_LEN             (80)
const PWR_REGS cfg_tab_g[TCP_PAMAP_TAB_G_LEN] = {
    // pregain REGB<31:28> REGC<10:8> REGC<6:4> REGC<2:0> REGA<13:8> REGA<7:4> REGA<1:0>
PWRI(  0x5E ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 0    2.5  dBm
PWRI(  0x60 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 1    2.75  dBm
PWRI(  0x63 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 2    3  dBm
PWRI(  0x66 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 3    3.25  dBm
PWRI(  0x69 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 4    3.5  dBm
PWRI(  0x6C ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 5    3.75  dBm
PWRI(  0x6F ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 6    4  dBm
PWRI(  0x73 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 7    4.25  dBm
PWRI(  0x76 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 8    4.5  dBm
PWRI(  0x79 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 9    4.75  dBm
PWRI(  0x7D ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 10   5  dBm
PWRI(  0x81 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 11   5.25  dBm
PWRI(  0x84 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 12   5.5  dBm
PWRI(  0x88 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 13   5.75  dBm
PWRI(  0x8C ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 14   6  dBm
PWRI(  0x90 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 15   6.25  dBm
PWRI(  0x95 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 16   6.5  dBm
PWRI(  0x99 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 17   6.75  dBm
PWRI(  0x9D ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 18   7  dBm
PWRI(  0xA2 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 19   7.25  dBm
PWRI(  0xA7 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 20   7.5  dBm
PWRI(  0xAC ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 21   7.75  dBm
PWRI(  0xB1 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 22   8  dBm
PWRI(  0xB6 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 23   8.25  dBm
PWRI(  0xBB ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 24   8.5  dBm
PWRI(  0xC0 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 25   8.75  dBm
PWRI(  0xC6 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 26   9  dBm
PWRI(  0xCC ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 27   9.25  dBm
PWRI(  0xD2 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 28   9.5  dBm
PWRI(  0xD8 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 29   9.75  dBm
PWRI(  0xDE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 30   10  dBm
PWRI(  0xE5 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 31   10.25  dBm
PWRI(  0xEB ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 32   10.5  dBm
PWRI(  0xF2 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 33   10.75  dBm
PWRI(  0xF9 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 34   11  dBm
PWRI(  0x101 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 35   11.25  dBm
PWRI(  0x108 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 36   11.5  dBm
PWRI(  0x110 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 37   11.75  dBm
PWRI(  0x118 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 38   12  dBm
PWRI(  0x120 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 39   12.25  dBm
PWRI(  0x128 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 40   12.5  dBm
PWRI(  0x131 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 41   12.75  dBm
PWRI(  0x13A ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 42   13  dBm
PWRI(  0x143 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 43   13.25  dBm
PWRI(  0x14D ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 44   13.5  dBm
PWRI(  0x156 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 45   13.75  dBm
PWRI(  0x160 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 46   14  dBm
PWRI(  0x16B ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 47   14.25  dBm
PWRI(  0x175 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 48   14.5  dBm
PWRI(  0x180 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 49   14.75  dBm
PWRI(  0x18B ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 50   15  dBm
PWRI(  0x197 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 51   15.25  dBm
PWRI(  0x1A3 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 52   15.5  dBm
PWRI(  0x1AF ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 53   15.75  dBm
PWRI(  0x1BB ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 54   16  dBm
PWRI(  0x1C8 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 55   16.25  dBm
PWRI(  0x1D6 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 56   16.5  dBm
PWRI(  0x1E0 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 57   16.75  dBm
PWRI(  0x1F2 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 58   17  dBm
PWRI(  0x200 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 59   17.25  dBm
PWRI(  0x20F ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 60   17.5  dBm
PWRI(  0x21E ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 61   17.75  dBm
PWRI(  0x22E ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 62   18  dBm
PWRI(  0x23F ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 63   18.25  dBm
PWRI(  0x24F ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 64   18.5  dBm
PWRI(  0x261 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 65   18.75  dBm
PWRI(  0x272 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 66   19  dBm
PWRI(  0x285 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 67   19.25  dBm
PWRI(  0x298 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 68   19.5  dBm
PWRI(  0x2AB ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 69   19.75  dBm
PWRI(  0x2BF ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 70   20  dBm
PWRI(  0x2D3 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 71   20.25  dBm
PWRI(  0x2E9 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 72   20.5  dBm
PWRI(  0x2FE ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 73   20.75  dBm
PWRI(  0x315 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 74   21  dBm
PWRI(  0x32C ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 75   21.25  dBm
PWRI(  0x343 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 76   21.5  dBm
PWRI(  0x35C ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 77   21.75  dBm
PWRI(  0x375 ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 78   22  dBm
PWRI(  0x38F ,     0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 79   22.25  dBm
};

#define TCP_PAMAP_TAB_BLE_LEN           (80)
#if !CONFIG_BLE_USE_HIGH_POWER_LEVEL
const PWR_REGS cfg_tab_ble[TCP_PAMAP_TAB_BLE_LEN] =
{
    // pregain REGB<31:28> REGC<10:8> REGC<6:4> REGC<2:0> REGA<13:8> REGA<7:4> REGA<1:0>
PWRI(  0x0C6 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 0    3.5  dBm
PWRI(  0x0CC ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 1    3.75  dBm
PWRI(  0x0D2 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 2    4  dBm
PWRI(  0x0D8 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 3    4.25  dBm
PWRI(  0x0DE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 4    4.5  dBm
PWRI(  0x0E5 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 5    4.75  dBm
PWRI(  0x0EB ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 6    5  dBm
PWRI(  0x0F2 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 7    5.25  dBm
PWRI(  0x0F9 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 8    5.5  dBm
PWRI(  0x101 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 9    5.75  dBm
PWRI(  0x108 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 10   6  dBm
PWRI(  0x110 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 11   6.25  dBm
PWRI(  0x118 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 12   6.5  dBm
PWRI(  0x120 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 13   6.75  dBm
PWRI(  0x128 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 14   7  dBm
PWRI(  0x131 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 15   7.25  dBm
PWRI(  0x13A ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 16   7.5  dBm
PWRI(  0x143 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 17   7.75  dBm
PWRI(  0x14C ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 18   8  dBm
PWRI(  0x156 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 19   8.25  dBm
PWRI(  0x160 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 20   8.5  dBm
PWRI(  0x16A ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 21   8.75  dBm
PWRI(  0x175 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 22   9  dBm
PWRI(  0x180 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 23   9.25  dBm
PWRI(  0x18B ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 24   9.5  dBm
PWRI(  0x197 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 25   9.75  dBm
PWRI(  0x1A3 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 26   10  dBm
PWRI(  0x1AF ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 27   10.25  dBm
PWRI(  0x1BB ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 28   10.5  dBm
PWRI(  0x1C8 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 29   10.75  dBm
PWRI(  0x1D6 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 30   11  dBm
PWRI(  0x1E3 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 31   11.25  dBm
PWRI(  0x1F1 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 32   11.5  dBm
PWRI(  0x200 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 33   11.75  dBm
PWRI(  0x20F ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 34   12  dBm
PWRI(  0x21E ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 35   12.25  dBm
PWRI(  0x22E ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 36   12.5  dBm
PWRI(  0x23E ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 37   12.75  dBm
PWRI(  0x24F ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 38   13  dBm
PWRI(  0x261 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 39   13.25  dBm
PWRI(  0x272 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 40   13.5  dBm
PWRI(  0x285 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 41   13.75  dBm
PWRI(  0x297 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 42   14  dBm
PWRI(  0x2AB ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 43   14.25  dBm
PWRI(  0x2BF ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 44   14.5  dBm
PWRI(  0x2D3 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 45   14.75  dBm
PWRI(  0x2E8 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 46   15  dBm
PWRI(  0x2FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 47   15.25  dBm
PWRI(  0x314 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 48   15.5  dBm
PWRI(  0x32B ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 49   15.75  dBm
PWRI(  0x343 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 50   16  dBm
PWRI(  0x35C ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 51   16.25  dBm
PWRI(  0x375 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 52   16.5  dBm
PWRI(  0x38E ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 53   16.75  dBm
PWRI(  0x3A9 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 54   17  dBm
PWRI(  0x3C4 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 55   17.25  dBm
PWRI(  0x3E1 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 56   17.5  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 57   17.75  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 58   18  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 59   18.25  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 60   18.5  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 61   18.75  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 62   19  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 63   19.25  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 64   19.5  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 65   19.75  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 66   20  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 67   20.25  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 68   20.5  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 69   20.75  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 70   21  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 71   21.25  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 72   21.5  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 73   21.75  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 74   22  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 75   22.25  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 76   21.5  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 77   21.75  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 78   22  dBm
PWRI(  0x3FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 79   22.25  dBm
};
#else
const PWR_REGS cfg_tab_ble[TCP_PAMAP_TAB_BLE_LEN] =
{
    // pregain REGB<31:28> REGC<10:8> REGC<6:4> REGC<2:0> REGA<13:8> REGA<7:4> REGA<1:0>
PWRI(  0x056 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 0    3.5  dBm
PWRI(  0x058 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 1    3.75  dBm
PWRI(  0x05B ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 2    4  dBm
PWRI(  0x05E ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 3    4.25  dBm
PWRI(  0x060 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 4    4.5  dBm
PWRI(  0x063 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 5    4.75  dBm
PWRI(  0x066 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 6    5  dBm
PWRI(  0x069 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 7    5.25  dBm
PWRI(  0x06C ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 8    5.5  dBm
PWRI(  0x06F ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 9    5.75  dBm
PWRI(  0x073 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 10   6  dBm
PWRI(  0x076 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 11   6.25  dBm
PWRI(  0x079 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 12   6.5  dBm
PWRI(  0x07D ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 13   6.75  dBm
PWRI(  0x081 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 14   7  dBm
PWRI(  0x084 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 15   7.25  dBm
PWRI(  0x088 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 16   7.5  dBm
PWRI(  0x08C ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 17   7.75  dBm
PWRI(  0x090 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 18   8  dBm
PWRI(  0x095 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 19   8.25  dBm
PWRI(  0x099 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 20   8.5  dBm
PWRI(  0x09D ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 21   8.75  dBm
PWRI(  0x0A2 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 22   9  dBm
PWRI(  0x0A7 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 23   9.25  dBm
PWRI(  0x0AC ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 24   9.5  dBm
PWRI(  0x0B1 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 25   9.75  dBm
PWRI(  0x0B6 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 26   10  dBm
PWRI(  0x0BB ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 27   10.25  dBm
PWRI(  0x0C0 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 28   10.5  dBm
PWRI(  0x0C6 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 29   10.75  dBm
PWRI(  0x0CC ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 30   11  dBm
PWRI(  0x0D2 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 31   11.25  dBm
PWRI(  0x0D8 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 32   11.5  dBm
PWRI(  0x0DE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 33   11.75  dBm
PWRI(  0x0E5 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 34   12  dBm
PWRI(  0x0EB ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 35   12.25  dBm
PWRI(  0x0F2 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 36   12.5  dBm
PWRI(  0x0F9 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 37   12.75  dBm
PWRI(  0x101 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 38   13  dBm
PWRI(  0x108 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 39   13.25  dBm
PWRI(  0x110 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 40   13.5  dBm
PWRI(  0x118 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 41   13.75  dBm
PWRI(  0x120 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 42   14  dBm
PWRI(  0x128 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 43   14.25  dBm
PWRI(  0x131 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 44   14.5  dBm
PWRI(  0x13A ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 45   14.75  dBm
PWRI(  0x143 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 46   15  dBm
PWRI(  0x14C ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 47   15.25  dBm
PWRI(  0x156 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 48   15.5  dBm
PWRI(  0x160 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 49   15.75  dBm
PWRI(  0x16A ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 50   16  dBm
PWRI(  0x175 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 51   16.25  dBm
PWRI(  0x180 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 52   16.5  dBm
PWRI(  0x18B ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 53   16.75  dBm
PWRI(  0x197 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 54   17  dBm
PWRI(  0x1A3 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 55   17.25  dBm
PWRI(  0x1AF ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 56   17.5  dBm
PWRI(  0x1BB ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 57   17.75  dBm
PWRI(  0x1C8 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 58   18  dBm
PWRI(  0x1D6 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 59   18.25  dBm
PWRI(  0x1E3 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 60   18.5  dBm
PWRI(  0x1F1 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 61   18.75  dBm
PWRI(  0x200 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 62   19  dBm
PWRI(  0x20F ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 63   19.25  dBm
PWRI(  0x21E ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 64   19.5  dBm
PWRI(  0x22E ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 65   19.75  dBm
PWRI(  0x23E ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 66   20  dBm
PWRI(  0x24F ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 67   20.25  dBm
PWRI(  0x261 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 68   20.5  dBm
PWRI(  0x272 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 69   20.75  dBm
PWRI(  0x285 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 70   21  dBm
PWRI(  0x297 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 71   21.25  dBm
PWRI(  0x2AB ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 72   21.5  dBm
PWRI(  0x2BF ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 73   21.75  dBm
PWRI(  0x2D3 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 74   22  dBm
PWRI(  0x2E8 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 75   22.25  dBm
PWRI(  0x2FE ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 76   21.5  dBm
PWRI(  0x314 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 77   21.75  dBm
PWRI(  0x32B ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 78   22  dBm
PWRI(  0x343 ,      0xA,       7,        3,         2,      0x20,     0x0,     0x2   ),   // 79   22.25  dBm
};
#endif

const TXPWR_ST gtxpwr_tab_def_b[WLAN_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(10, TXPWR_ELEM_INUSED),  // ch1  inused
    INIT_TXPWR_VALUE(10, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(10, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(10, TXPWR_ELEM_UNUSED),  // ch4
    INIT_TXPWR_VALUE(10, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(10, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(10, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(11, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(11, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(11, TXPWR_ELEM_UNUSED),  // ch10
    INIT_TXPWR_VALUE(11, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(11, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(11, TXPWR_ELEM_INUSED),  // ch13  inused
    INIT_TXPWR_VALUE(11, TXPWR_ELEM_UNUSED),
};

const TXPWR_ST gtxpwr_tab_def_g[WLAN_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(27, TXPWR_ELEM_INUSED),  // ch1  inused
    INIT_TXPWR_VALUE(27, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(27, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(27, TXPWR_ELEM_UNUSED),  // ch4
    INIT_TXPWR_VALUE(27, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(27, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(27, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(27, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(26, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(27, TXPWR_ELEM_UNUSED),  // ch10
    INIT_TXPWR_VALUE(27, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(28, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(29, TXPWR_ELEM_INUSED),  // ch13  inused
    INIT_TXPWR_VALUE(29, TXPWR_ELEM_UNUSED),
};

const TXPWR_ST gtxpwr_tab_def_n_40[WLAN_2_4_G_CHANNEL_NUM] ={
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_INUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),  // ch3
    INIT_TXPWR_VALUE(23, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(23, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(23, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(23, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(23, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(23, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(23, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(23, TXPWR_ELEM_UNUSED),  // ch11
    INIT_TXPWR_VALUE(23, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(23, TXPWR_ELEM_INUSED),
    INIT_TXPWR_VALUE(23, TXPWR_ELEM_UNUSED),
};

#if CONFIG_BLE_USE_HIGH_POWER_LEVEL
const TXPWR_ST gtxpwr_tab_def_ble[BLE_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),  // ch0 2402  inused
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),  // ch1 2404
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),  // ch4 2410
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),  // ch9 2420
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),  // ch14 2430
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_INUSED),  // ch19 2440 inused
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),  // ch24 2450
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),  // ch29 2460
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),  // ch34 2470
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),  // ch39 2480 inused
};
#else
const TXPWR_ST gtxpwr_tab_def_ble[BLE_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),  // ch0 2402  inused
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),  // ch1 2404
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),  // ch4 2410
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),  // ch9 2420
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),  // ch14 2430
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_INUSED),  // ch19 2440 inused
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),  // ch24 2450
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),  // ch29 2460
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(21, TXPWR_ELEM_UNUSED),  // ch34 2470
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(22, TXPWR_ELEM_UNUSED),  // ch39 2480 inused
};
#endif

const TMP_PWR_ST tmp_pwr_tab[TMP_PWR_TAB_LEN] = {
//trx0x0c[12:15], shift_b, shift_g, shift_ble, xtal_c_delta
    {  0x00,        -5,      -12,      0,         0},   // 0     ,-40    -35
    {  0x00,        -5,      -12,      0,         2},    // 1     ,-35    -30
    {  0x00,        -4,      -10,      0,         4},    // 2     ,-30    -25
    {  0x00,        -4,      -10,      0,         6},    // 3     ,-25    -20
    {  0x00,        -3,      -8,       0,         7},    // 4     ,-20    -15
    {  0x00,        -3,      -8,       0,         8},    // 5     ,-15    -10
    {  0x00,        -2,      -6,       0,         8},    // 6     ,-10    -5
    {  0x00,        -2,      -6,       0,         8},    // 7     ,-5     0
    {  0x00,        -2,      -4,       0,         7},    // 8     ,0      5
    {  0x00,        -2,      -4,       0,         6},    // 9     ,5      10
    {  0x00,        -1,      -2,       0,         5},    // 10    ,10     15
    {  0x00,        -1,      -2,       0,         4},    // 11    ,15     20
    {  0x00,        0,       0,        0,         3},    // 12    ,20     25
    {  0x00,        0,       0,        0,         0},    // 13    ,25     30
    {  0x00,        0,       0,        0,         -1},    // 14    ,30     35
    {  0x00,        0,       0,        0,         -2},   // 15    ,35     40
    {  0x00,        0,       0,        0,         -4},   // 16    ,40     45
    {  0x00,        0,       0,        0,         -6},   // 17    ,45     50
    {  0x00,        1,       2,        0,         -7},   // 18    ,50     55
    {  0x00,        1,       3,        0,         -8},   // 19    ,55     60
    {  0x00,        1,       3,        0,         -9},   // 20    ,60     65
    {  0x00,        2,       4,        0,         -8},   // 21    ,65     70
    {  0x00,        2,       5,        0,         -8},   // 22    ,70     75
    {  0x00,        2,       5,        0,         -8},   // 23    ,75     80
    {  0x00,        3,       7,        0,         -5},   // 24    ,80     85
    {  0x00,        3,       7,        0,         -3},   // 25    ,85     90
    {  0x00,        3,       7,        0,         -1},    // 26    ,90     95
    {  0x00,        3,       8,        1,         4},    // 27    ,95     100
    {  0x00,        4,       10,       1,         9},   // 28    ,100    105
    {  0x00,        4,       10,       2,         16},   // 29    ,105    110
    {  0x00,        5,       10,       2,         25},   // 30    ,110    115
    {  0x00,        5,       11,       3,         36},   // 31    ,115
    {  0x00,        5,       11,       3,         48},   // 32    ,120
    {  0x00,        6,       11,       3,         64},   // 33    ,125
    {  0x00,        6,       11,       3,         83},  // 34    ,130
    {  0x00,        7,       11,       3,         105},  // 35    ,135
    {  0x00,        7,       11,       3,         127},  // 36    ,140
    {  0x00,        7,       11,       3,         127},  // 37    ,145
    {  0x00,        7,       11,       3,         127},  // 38    ,150
};


const static AUTO_PWR_CALI_CONTEXT auto_pwr =
{
    0x1,      /*0: CALI_MODE_AUTO  1: CALI_MODE_MANUAL */
    0x21C,   /* gtx_tssi_thred_chan1_b */
    0x208,   /* gtx_tssi_thred_chan7_b */
    0x1F4,   /* gtx_tssi_thred_chan13_b */
    0x230,   /* gtx_tssi_thred_chan1_g */
    0x21C,   /* gtx_tssi_thred_chan7_g */
    0x1FE,   /* gtx_tssi_thred_chan13_g */
};


const UINT32 g_default_xtal       = DEFAULT_TXID_XTAL;
const UINT32 g_cmtag_flag         = CMTAG_FROM_FLASH;
const UINT32 g_pwr_gain_base_ble  = PWR_GAIN_BASE_BLE;
const UINT32 pwr_gain_base_gain_b = PWR_GAIN_BASE_B;
const UINT32 pwr_gain_base_gain_g = PWR_GAIN_BASE_G;

const INT16 shift_tab_b[4]        = {0, 0, 0, 0}; // 11M base,5.5M,2M,1M
// 54M base -54M,48M,36M,24M,18M,12M,9M,6M
const INT16 shift_tab_g[8]        = {0,  2,  2, 2,  3, 3,  4, 4/*4*/}; // 54M base -  12M,9M,6M//do
const INT16 shift_tab_n20[10]     = {-4,  -4,  0, 2,  2, 2,  3, 3,  4, 4/*4*/}; // n20 mcs8 mcs9 mcs7(base) -  mcs0,
const INT16 shift_tab_n40[10]     = {-4,  -4,  0, 2,  2, 2,  3, 3,  4, 4/*4*/}; // n40 mcs8 mcs9 mcs7(base) -  mcs0,

void vnd_cal_overlay(void)
{
    vnd_cal_set_auto_pwr_thred(auto_pwr);
    //EPA
#if EPA_ENABLE_FLAG
    vnd_cal_set_epa_config(1, GPIO_28, GPIO_26, pwr_gain_base_gain_b, pwr_gain_base_gain_g);
#else
    vnd_cal_set_epa_config(0, GPIO_28, GPIO_26, pwr_gain_base_gain_b, pwr_gain_base_gain_g);
#endif
}



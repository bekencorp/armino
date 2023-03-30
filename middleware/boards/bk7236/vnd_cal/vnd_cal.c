#include "vnd_cal.h"

#define DEFAULT_TXID_XTAL               (0x3B)
#define PWR_GAIN_BASE_B                 (0x6094A800)
#define PWR_GAIN_BASE_G                 (0x60B09800)
#define PWR_GAIN_BASE_BLE               (0x63E89800)

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

const UINT32 g_default_xtal   = DEFAULT_TXID_XTAL;

/********************************  power table ********************************/
const TXPWR_ST gtxpwr_tab_def_b[WLAN_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_INUSED),  // ch1  inused
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),  // ch4
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),  // ch10
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_INUSED),  // ch13  inused
    INIT_TXPWR_VALUE(24, TXPWR_ELEM_UNUSED),
};

const TXPWR_ST gtxpwr_tab_def_g[WLAN_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_INUSED),  // ch1  inused
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),  // ch4
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),  // ch10
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_INUSED),  // ch13  inused
    INIT_TXPWR_VALUE(39, TXPWR_ELEM_UNUSED),
};

const TXPWR_ST gtxpwr_tab_def_n_40[WLAN_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),  // ch3
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),  // ch11
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
};

const TXPWR_ST gtxpwr_tab_def_ble[BLE_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),  // ch0 2402  inused
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),  // ch1 2404
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),  // ch4 2410
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),  // ch9 2420
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),  // ch14 2430
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_INUSED),  // ch19 2440 inused
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),  // ch24 2450
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),  // ch29 2460
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),  // ch34 2470
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),  // ch39 2480 inused
};

/****************************** temperature table  ****************************/
const TMP_PWR_ST tmp_pwr_tab[TMP_PWR_TAB_LEN] = {
//trx0x0c[12:15], shift_b, shift_g, shift_ble, xtal_c_delta
    {  0x00,        -2,      -4,       0,         -1},   // 0     ,-40    -35
    {  0x00,        -2,      -4,       0,         1},   // 1     ,-35    -30
    {  0x00,        -2,      -4,       0,         2},   // 2     ,-30    -25
    {  0x00,        -1,      -3,       0,         2},   // 3     ,-25    -20
    {  0x00,        -1,      -3,       0,         4},   // 4     ,-20     -15
    {  0x00,        -1,      -2,       0,         4},   // 5     ,-15  -10
    {  0x00,        0,      -1,       0,         4},   // 6     ,-10   -5
    {  0x00,        0,      -1,       0,         4},   // 7     ,-5     0
    {  0x00,        0,      -1,       0,         4},   // 8     ,0       5
    {  0x00,        0,      -1,       0,         3},   // 9     ,5      10
    {  0x00,        0,      -1,       0,         4},   // 10    ,10     15
    {  0x00,        0,      -1,       0,         2},   // 11    ,15    20
    {  0x00,        0,      0,       0,         1},   // 12    ,20    25
    {  0x00,        0,      0,       0,         0},   // 13    ,25    30
    {  0x00,        0,       0,        0,         -2},   // 14    ,30   35
    {  0x00,        0,       0,        0,         -2},   // 15    ,35   40
    {  0x00,        0,       0,        0,         -3},   // 16    ,40  45
    {  0x00,        0,       0,        0,         -3},   // 17    ,45    50
    {  0x00,        0,       1,        0,         -4},   // 18    ,50    55
    {  0x00,        0,       1,        0,         -4},   // 19    ,55   60
    {  0x00,        1,       2,        0,         -4},   // 20    ,60   65
    {  0x00,        1,       2,        0,         -4},   // 21    ,65   70
    {  0x00,        1,       3,        0,         -3},   // 22    ,70   75
    {  0x00,        2,       4,        0,         -2},   // 23    ,75   80
    {  0x00,        2,       4,        0,         -1},   // 24    ,80   85
    {  0x00,        2,       4,        0,         1},   // 25    ,85  90
    {  0x00,        2,       4,        0,         4},   // 26    ,90  95
    {  0x00,        2,       4,        1,        7},   // 27    ,95  100
    {  0x00,        2,       4,        1,        11},   // 28    ,100  105
    {  0x00,        2,       4,        2,        19},   // 29    ,105  110
    {  0x00,        2,       4,        2,        26},   // 30    ,110  115
    {  0x00,        2,       4,        3,        34},   // 31    ,115
    {  0x00,        2,       4,        3,        77},   // 32    ,120
    {  0x00,        2,       4,        3,        99},   // 33    ,125
    {  0x00,        2,       4,        3,        117},   // 34    ,130
    {  0x00,        2,       4,        3,        117},   // 35    ,135
    {  0x00,        2,       4,        3,        117},   // 36    ,140
    {  0x00,        2,       4,        3,        117},   // 37    ,145
    {  0x00,        2,       4,        3,        117},   // 38    ,150
};

/****************************** power shift table  ****************************/
#if CFG_SUPPORT_LOW_MAX_CURRENT//Max default work current<250mA@3.3V
const INT16 shift_tab_b[4] = {0, 0, 0, 0}; // 11M base,5.5M,2M,1M
const INT16 shift_tab_g[8] = {0, 2, 4, 4, 4, 4, 4, 4}; // 54M base -  6M
const INT16 shift_tab_n[1] = {0}; // for MCS7
#else
const INT16 shift_tab_b[4] = {0, 0, 0, 0}; // 11M base,5.5M,2M,1M
// 54M base -                 54M,48M,36M,24M,18M,12M,9M,6M
const INT16 shift_tab_g[8] = {0,  2,  2,  2,  3,  3,  4, 4/*4*/}; // 54M base -  12M,9M,6M//do
const INT16 shift_tab_n20[10] = {-4,  -4,  0,  2,  2,  2,  3,  3,  4, 4/*4*/}; // n20 mcs9 mcs8 mcs7(base) -  mcs0,
const INT16 shift_tab_n40[10] = {-4,  -4,  0,  2,  2,  2,  3,  3,  4, 4/*4*/}; // n40 mcs9 mcs8 mcs7(base) -  mcs0,
#endif

void vnd_cal_overlay(void)
{
	//replace overlay with link to hardening code size
}

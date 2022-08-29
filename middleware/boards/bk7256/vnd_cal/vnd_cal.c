#include "vnd_cal.h"


#define DEFAULT_TXID_XTAL               (0xA)
#define CMTAG_FROM_FLASH                1
#define CMTAG_FROM_CALI                 0
#if CONFIG_BLE_USE_HIGH_POWER_LEVEL
#define PWR_GAIN_BASE_BLE               (0xE0A91800)
#else
#define PWR_GAIN_BASE_BLE               (0x40695800)
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



TXPWR_ST gtxpwr_tab_def_b_overlay[WLAN_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_INUSED),  // ch1  inused
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),  // ch4
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(17, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(17, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(17, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(17, TXPWR_ELEM_UNUSED),  // ch10
    INIT_TXPWR_VALUE(18, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(18, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(18, TXPWR_ELEM_INUSED),  // ch13  inused
    INIT_TXPWR_VALUE(18, TXPWR_ELEM_UNUSED),
};

TXPWR_ST gtxpwr_tab_def_g_overlay[WLAN_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(45, TXPWR_ELEM_INUSED),  // ch1  inused
    INIT_TXPWR_VALUE(45, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(45, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(45, TXPWR_ELEM_UNUSED),  // ch4
    INIT_TXPWR_VALUE(45, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),  // ch10
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(48, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(50, TXPWR_ELEM_INUSED),  // ch13  inused
    INIT_TXPWR_VALUE(50, TXPWR_ELEM_UNUSED),
};

TXPWR_ST gtxpwr_tab_def_n_40_overlay[WLAN_2_4_G_CHANNEL_NUM] ={
    INIT_TXPWR_VALUE(41, TXPWR_ELEM_INUSED),
    INIT_TXPWR_VALUE(41, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(41, TXPWR_ELEM_UNUSED),  // ch3
    INIT_TXPWR_VALUE(41, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(41, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(41, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(44, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(44, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(44, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(44, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(44, TXPWR_ELEM_UNUSED),  // ch11
    INIT_TXPWR_VALUE(44, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(44, TXPWR_ELEM_INUSED),
    INIT_TXPWR_VALUE(44, TXPWR_ELEM_UNUSED),
};

#if CONFIG_BLE_USE_HIGH_POWER_LEVEL
TXPWR_ST gtxpwr_tab_def_ble_overlay[BLE_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),  // ch0 2402  inused
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),  // ch1 2404
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),  // ch4 2410
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),  // ch9 2420
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),  // ch14 2430
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_INUSED),  // ch19 2440 inused
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),  // ch24 2450
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),  // ch29 2460
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),  // ch34 2470
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),  // ch39 2480 inused
};
#else
TXPWR_ST gtxpwr_tab_def_ble_overlay[BLE_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),  // ch0 2402  inused
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),  // ch1 2404
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),  // ch4 2410
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),  // ch9 2420
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),  // ch14 2430
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_INUSED),  // ch19 2440 inused
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),  // ch24 2450
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),  // ch29 2460
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),  // ch34 2470
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(40, TXPWR_ELEM_UNUSED),  // ch39 2480 inused
};
#endif

TMP_PWR_ST tmp_pwr_tab_overlay[TMP_PWR_TAB_LEN] = {
//trx0x0c[12:15], shift_b, shift_g, shift_ble, xtal_c_delta
    {  0x00,        -4,      -10,      0,         -1},   // 0     ,-40    -35
    {  0x00,        -4,      -10,      0,         0},    // 1     ,-35    -30
    {  0x00,        -3,      -9,       0,         3},    // 2     ,-30    -25
    {  0x00,        -3,      -8,       0,         6},    // 3     ,-25    -20
    {  0x00,        -3,      -7,       0,         8},    // 4     ,-20    -15
    {  0x00,        -2,      -6,       0,         10},    // 5     ,-15    -10
    {  0x00,        -2,      -5,       0,         8},    // 6     ,-10    -5
    {  0x00,        -2,      -4,       0,         7},    // 7     ,-5     0
    {  0x00,        -1,      -4,       0,         6},    // 8     ,0      5
    {  0x00,        -1,      -4,       0,         5},    // 9     ,5      10
    {  0x00,        -1,      -3,       0,         4},    // 10    ,10     15
    {  0x00,        0,       -2,       0,         3},    // 11    ,15     20
    {  0x00,        0,       0,        0,         2},    // 12    ,20     25
    {  0x00,        0,       0,        0,         0},    // 13    ,25     30
    {  0x00,        0,       0,        0,         -3},    // 14    ,30     35
    {  0x00,        0,       0,        0,         -5},   // 15    ,35     40
    {  0x00,        0,       0,        0,         -7},   // 16    ,40     45
    {  0x00,        1,       2,        0,         -9},   // 17    ,45     50
    {  0x00,        1,       2,        0,         -11},   // 18    ,50     55
    {  0x00,        1,       2,        0,         -11},   // 19    ,55     60
    {  0x00,        2,       3,        0,         -12},   // 20    ,60     65
    {  0x00,        2,       3,        0,         -12},   // 21    ,65     70
    {  0x00,        2,       3,        0,         -12},   // 22    ,70     75
    {  0x00,        2,       4,        0,         -11},   // 23    ,75     80
    {  0x00,        2,       5,        0,         -8},   // 24    ,80     85
    {  0x00,        3,       5,        0,         -5},   // 25    ,85     90
    {  0x00,        3,       5,        0,         -1},    // 26    ,90     95
    {  0x00,        3,       5,        1,         4},    // 27    ,95     100
    {  0x00,        3,       5,        1,         6},   // 28    ,100    105
    {  0x00,        3,       6,        2,         15},   // 29    ,105    110
    {  0x00,        4,       6,        2,         27},   // 30    ,110    115
    {  0x00,        4,       6,        3,         40},   // 31    ,115
    {  0x00,        4,       6,        3,         57},   // 32    ,120
    {  0x00,        4,       6,        3,         79},   // 33    ,125
    {  0x00,        4,       6,        3,         117},  // 34    ,130
    {  0x00,        4,       6,        3,         117},  // 35    ,135
    {  0x00,        4,       6,        3,         117},  // 36    ,140
    {  0x00,        4,       6,        3,         117},  // 37    ,145
    {  0x00,        4,       6,        3,         117},  // 38    ,150
};

UINT32 g_xtal_overlay = DEFAULT_TXID_XTAL;
UINT32 g_cmtag_overlay = CMTAG_FROM_FLASH;
uint32 g_pwr_gain_base_ble_overlay = PWR_GAIN_BASE_BLE;

INT16 shift_tab_b_overlay[4] = {0, 0, 0, 0}; // 11M base,5.5M,2M,1M
// 54M base -54M,48M,36M,24M,18M,12M,9M,6M
INT16 shift_tab_g_overlay[8] = {0,  2,  2, 2,  3, 3,  4, 4/*4*/}; // 54M base -  12M,9M,6M//do
INT16 shift_tab_n20_overlay[10] = {-4,  -4,  0, 2,  2, 2,  3, 3,  4, 4/*4*/}; // n20 mcs8 mcs9 mcs7(base) -  mcs0,
INT16 shift_tab_n40_overlay[10] = {-4,  -4,  0, 2,  2, 2,  3, 3,  4, 4/*4*/}; // n40 mcs8 mcs9 mcs7(base) -  mcs0,

void vnd_cal_overlay(void)
{
    //set 11b default tx power table
    vnd_cal_txpwr_tab_def_b_overlay(gtxpwr_tab_def_b_overlay);
    //set 11b power shift
    vnd_cal_pwr_shift_b_tab_overlay(shift_tab_b_overlay);

    //set 11g default tx power table
    vnd_cal_txpwr_tab_def_g_overlay(gtxpwr_tab_def_g_overlay);
    //set 11g power shift
    vnd_cal_pwr_shift_g_tab_overlay(shift_tab_g_overlay);

    //set 11n 20M power shift
    vnd_cal_pwr_shift_n_tab_overlay(shift_tab_n20_overlay);

    //set 11n 40M default tx power table
    vnd_cal_txpwr_tab_def_n40_overlay(gtxpwr_tab_def_n_40_overlay);

    vnd_cal_pwr_shift_n40_tab_overlay(shift_tab_n40_overlay);

    //set ble default tx power table
    vnd_cal_txpwr_tab_def_ble_overlay(gtxpwr_tab_def_ble_overlay);
#if CONFIG_TEMP_DETECT
    //set temp power table
    vnd_cal_tmp_pwr_tab_overlay(tmp_pwr_tab_overlay);
#endif
    vnd_cal_gxtal_overlay(g_xtal_overlay);
	vnd_cal_gcmtag_overlay(g_cmtag_overlay);
    vnd_cal_pwr_gain_base_ble_overlay(g_pwr_gain_base_ble_overlay);

#if CONFIG_BLE_USE_HIGH_POWER_LEVEL
    vnd_cal_set_ble_pwr_level(1);
#else
    vnd_cal_set_ble_pwr_level(0);
#endif
}



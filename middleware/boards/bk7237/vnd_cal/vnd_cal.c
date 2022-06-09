#include "vnd_cal.h"


#define DEFAULT_TXID_XTAL           (0x10)
#define PWR_GAIN_BASE_BLE           (0x40691000)

#define TXPWR_ELEM_INUSED                 (0)
#define TXPWR_ELEM_UNUSED                 (1)
#define WLAN_2_4_G_CHANNEL_NUM            (14)
#define BLE_2_4_G_CHANNEL_NUM             (40)

#define TMP_PWR_TAB_LEN                   39

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
    INIT_TXPWR_VALUE(13, TXPWR_ELEM_INUSED),  // ch1  inused
    INIT_TXPWR_VALUE(13, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(13, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(13, TXPWR_ELEM_UNUSED),  // ch4
    INIT_TXPWR_VALUE(13, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(13, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(13, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(14, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(14, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(14, TXPWR_ELEM_UNUSED),  // ch10
    INIT_TXPWR_VALUE(15, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(15, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(15, TXPWR_ELEM_INUSED),  // ch13  inused
    INIT_TXPWR_VALUE(15, TXPWR_ELEM_UNUSED),
};

TXPWR_ST gtxpwr_tab_def_g_overlay[WLAN_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_INUSED),  // ch1  inused
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),  // ch4
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(34, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(35, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(36, TXPWR_ELEM_UNUSED),  // ch10
    INIT_TXPWR_VALUE(37, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(37, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(37, TXPWR_ELEM_INUSED),  // ch13  inused
    INIT_TXPWR_VALUE(37, TXPWR_ELEM_UNUSED),
};

TXPWR_ST gtxpwr_tab_def_n_40_overlay[WLAN_2_4_G_CHANNEL_NUM] ={
    INIT_TXPWR_VALUE(31, TXPWR_ELEM_INUSED),
    INIT_TXPWR_VALUE(31, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(31, TXPWR_ELEM_UNUSED),  // ch3
    INIT_TXPWR_VALUE(31, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(31, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(31, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(30, TXPWR_ELEM_UNUSED),  // ch7
    INIT_TXPWR_VALUE(31, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(32, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(32, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(33, TXPWR_ELEM_UNUSED),  // ch11
    INIT_TXPWR_VALUE(33, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(33, TXPWR_ELEM_INUSED),
    INIT_TXPWR_VALUE(33, TXPWR_ELEM_UNUSED),
};

TXPWR_ST gtxpwr_tab_def_ble_overlay[BLE_2_4_G_CHANNEL_NUM] = {
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),  // ch0 2402  inused
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),  // ch1 2404
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(20, TXPWR_ELEM_UNUSED),  // ch4 2410
    INIT_TXPWR_VALUE(19, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(19, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(19, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(19, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(19, TXPWR_ELEM_UNUSED),  // ch9 2420
    INIT_TXPWR_VALUE(18, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(18, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(18, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(18, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(18, TXPWR_ELEM_UNUSED),  // ch14 2430
    INIT_TXPWR_VALUE(17, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(17, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(17, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(17, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_INUSED),  // ch19 2440 inused
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),  // ch24 2450
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),  // ch29 2460
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),  // ch34 2470
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),
    INIT_TXPWR_VALUE(16, TXPWR_ELEM_UNUSED),  // ch39 2480 inused
};

TMP_PWR_ST tmp_pwr_tab_overlay[TMP_PWR_TAB_LEN] = {
//trx0x0c[12:15], shift_b, shift_g, shift_ble, xtal_c_delta
    {  0x00,        -2,      -4,       0,         -1},   // 0     ,-40    -35
    {  0x00,        -2,      -4,       0,         1},    // 1     ,-35    -30
    {  0x00,        -2,      -4,       0,         3},    // 2     ,-30    -25
    {  0x00,        -1,      -3,       0,         4},    // 3     ,-25    -20
    {  0x00,        -1,      -3,       0,         5},    // 4     ,-20    -15
    {  0x00,        -1,      -2,       0,         6},    // 5     ,-15    -10
    {  0x00,        0,       -1,       0,         7},    // 6     ,-10    -5
    {  0x00,        0,       -1,       0,         7},    // 7     ,-5     0
    {  0x00,        0,       -1,       0,         7},    // 8     ,0      5
    {  0x00,        0,       -1,       0,         6},    // 9     ,5      10
    {  0x00,        0,       -1,       0,         5},    // 10    ,10     15
    {  0x00,        0,       -1,       0,         4},    // 11    ,15     20
    {  0x00,        0,       0,        0,         3},    // 12    ,20     25
    {  0x00,        0,       0,        0,         0},    // 13    ,25     30
    {  0x00,        0,       0,        0,         0},    // 14    ,30     35
    {  0x00,        0,       0,        0,         -2},   // 15    ,35     40
    {  0x00,        0,       0,        0,         -3},   // 16    ,40     45
    {  0x00,        0,       0,        0,         -5},   // 17    ,45     50
    {  0x00,        0,       1,        0,         -5},   // 18    ,50     55
    {  0x00,        0,       1,        0,         -6},   // 19    ,55     60
    {  0x00,        1,       2,        0,         -6},   // 20    ,60     65
    {  0x00,        1,       2,        0,         -6},   // 21    ,65     70
    {  0x00,        1,       3,        0,         -6},   // 22    ,70     75
    {  0x00,        2,       4,        0,         -4},   // 23    ,75     80
    {  0x00,        2,       4,        0,         -4},   // 24    ,80     85
    {  0x00,        2,       4,        0,         -2},   // 25    ,85     90
    {  0x00,        2,       4,        0,         1},    // 26    ,90     95
    {  0x00,        2,       4,        1,         6},    // 27    ,95     100
    {  0x00,        2,       4,        1,         10},   // 28    ,100    105
    {  0x00,        2,       4,        2,         20},   // 29    ,105    110
    {  0x00,        2,       4,        2,         28},   // 30    ,110    115
    {  0x00,        2,       4,        3,         34},   // 31    ,115
    {  0x00,        2,       4,        3,         49},   // 32    ,120
    {  0x00,        2,       4,        3,         99},   // 33    ,125
    {  0x00,        2,       4,        3,         117},  // 34    ,130
    {  0x00,        2,       4,        3,         117},  // 35    ,135
    {  0x00,        2,       4,        3,         117},  // 36    ,140
    {  0x00,        2,       4,        3,         117},  // 37    ,145
    {  0x00,        2,       4,        3,         117},  // 38    ,150
};

UINT32 g_xtal_overlay = DEFAULT_TXID_XTAL;
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
    vnd_cal_pwr_gain_base_ble_overlay(g_pwr_gain_base_ble_overlay);
}



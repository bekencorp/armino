#include "vnd_cal.h"
#include <driver/hal/hal_gpio_types.h>

#define DEFAULT_TXID_XTAL               (0x3B)

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

/**
 * pwr_gain<09:00> pregain in cfg_power_table.pregain
 * pwr_gain<13:10> pactrl in TRX_C<11:8>, F at present
 * pwr_gain<17:14> padctrl in TRX_C<7:4>, C at present
 * pwr_gain<21:18> Rgm in TRX_C<15:12>, 8 at present
 * pwr_gain<31:31> 1:11g/b 0:11b
 */
const UINT32 pwr_gain_base_gain_b = 0x19637c00;//0x00233C00;
#define TPC_PAMAP_TAB_B_LEN			 (48)
const PWR_REGS cfg_tab_b[TPC_PAMAP_TAB_B_LEN] = {
	// pregain
#if 0
	PWRI(0x01D),   // 0   1.5  dBm
	PWRI(0x01F),   // 1   2  dBm
	PWRI(0x021),   // 2   2.5  dBm
	PWRI(0x023),   // 3   3  dBm
	PWRI(0x025),   // 4   3.5  dBm
	PWRI(0x028),   // 5   4  dBm
#endif
#if 1
	PWRI(0x02A),   // 6   4.5  dBm
	PWRI(0x02C),   // 7   5  dBm
	PWRI(0x02F),   // 8   5.5  dBm
	PWRI(0x032),   // 9   6  dBm
	PWRI(0x035),   // 10  6.5  dBm
	PWRI(0x039),   // 11  7  dBm
#endif
	PWRI(0x03C),   // 0   7.5  dBm
	PWRI(0x03F),   // 1   8  dBm
	PWRI(0x043),   // 2   8.5  dBm
	PWRI(0x047),   // 3   9  dBm
	PWRI(0x04B),   // 4   9.5  dBm
	PWRI(0x04F),   // 5   10  dBm
	PWRI(0x054),   // 6   10.5  dBm
	PWRI(0x059),   // 7   11  dBm
	PWRI(0x05E),   // 8   11.5  dBm
	PWRI(0x064),   // 9   12  dBm
	PWRI(0x06A),   // 10  12.5  dBm
	PWRI(0x070),   // 11  13  dBm
	PWRI(0x077),   // 12  13.5  dBm
	PWRI(0x07E),   // 13  14  dBm
	PWRI(0x085),   // 14  14.5  dBm
	PWRI(0x08D),   // 15  15  dBm
	PWRI(0x096),   // 16  15.5  dBm
	PWRI(0x09F),   // 17  16  dBm
	PWRI(0x0A8),   // 18  16.5  dBm
	PWRI(0x0B2),   // 19  17  dBm
	PWRI(0x0BC),   // 20  17.5  dBm
	PWRI(0x0C8),   // 21  18  dBm
	PWRI(0x0D3),   // 22  18.5  dBm
	PWRI(0x0E0),   // 23  19  dBm
	PWRI(0x0ED),   // 24  19.5  dBm
	PWRI(0x0FB),   // 25  20  dBm
	PWRI(0x10A),   // 26  20.5  dBm
	PWRI(0x11A),   // 27  21  dBm
	PWRI(0x12B),   // 28  21.5  dBm
	PWRI(0x13C),   // 29  22  dBm
	PWRI(0x14F),   // 30  22.5  dBm
	PWRI(0x163),   // 31  23  dBm
	PWRI(0x178),   // 32  23.5  dBm
	PWRI(0x18E),   // 33  24  dBm
	PWRI(0x1A6),   // 34  24.5  dBm
	PWRI(0x1BF),   // 35  25  dBm
	PWRI(0x1D9),   // 36  25.5  dBm
	PWRI(0x1F5),   // 37  26  dBm
	PWRI(0x213),   // 38  26.5  dBm
	PWRI(0x233),   // 39  27  dBm
	PWRI(0x254),   // 40  27.5  dBm
	PWRI(0x277),   // 41  28  dBm
#if 0
	PWRI(0x29D),   // 42  28.5  dBm
	PWRI(0x2C4),   // 43  29  dBm
	PWRI(0x2EE),   // 44  29.5  dBm
	PWRI(0x31B),   // 45  30  dBm
	PWRI(0x34A),   // 46  30.5  dBm
	PWRI(0x37C),   // 47  31  dBm
#endif
};

const UINT32 pwr_gain_base_gain_g = 0x99637c00;//0x80233C00;
#define TPC_PAMAP_TAB_G_LEN			 (80)
const PWR_REGS cfg_tab_g[TPC_PAMAP_TAB_G_LEN] = {
	// pregain
	PWRI(0x05E),   // 0	2.5  dBm
	PWRI(0x060),   // 1	2.75  dBm
	PWRI(0x063),   // 2	3  dBm
	PWRI(0x066),   // 3	3.25  dBm
	PWRI(0x069),   // 4	3.5  dBm
	PWRI(0x06C),   // 5	3.75  dBm
	PWRI(0x06F),   // 6	4  dBm
	PWRI(0x073),   // 7	4.25  dBm
	PWRI(0x076),   // 8	4.5  dBm
	PWRI(0x079),   // 9	4.75  dBm
	PWRI(0x07D),   // 10   5  dBm
	PWRI(0x081),   // 11   5.25  dBm
	PWRI(0x084),   // 12   5.5  dBm
	PWRI(0x088),   // 13   5.75  dBm
	PWRI(0x08C),   // 14   6  dBm
	PWRI(0x090),   // 15   6.25  dBm
	PWRI(0x095),   // 16   6.5  dBm
	PWRI(0x099),   // 17   6.75  dBm
	PWRI(0x09D),   // 18   7  dBm
	PWRI(0x0A2),   // 19   7.25  dBm
	PWRI(0x0A7),   // 20   7.5  dBm
	PWRI(0x0AC),   // 21   7.75  dBm
	PWRI(0x0B1),   // 22   8  dBm
	PWRI(0x0B6),   // 23   8.25  dBm
	PWRI(0x0BB),   // 24   8.5  dBm
	PWRI(0x0C0),   // 25   8.75  dBm
	PWRI(0x0C6),   // 26   9  dBm
	PWRI(0x0CC),   // 27   9.25  dBm
	PWRI(0x0D2),   // 28   9.5  dBm
	PWRI(0x0D8),   // 29   9.75  dBm
	PWRI(0x0DE),   // 30   10  dBm
	PWRI(0x0E5),   // 31   10.25  dBm
	PWRI(0x0EB),   // 32   10.5  dBm
	PWRI(0x0F2),   // 33   10.75  dBm
	PWRI(0x0F9),   // 34   11  dBm

	PWRI(0x101),   // 35   11.25  dBm
	PWRI(0x108),   // 36   11.5  dBm
	PWRI(0x110),   // 37   11.75  dBm
	PWRI(0x118),   // 38   12  dBm
	PWRI(0x120),   // 39   12.25  dBm
	PWRI(0x128),   // 40   12.5  dBm
	PWRI(0x131),   // 41   12.75  dBm
	PWRI(0x13A),   // 42   13  dBm
	PWRI(0x143),   // 43   13.25  dBm
	PWRI(0x14D),   // 44   13.5  dBm
	PWRI(0x156),   // 45   13.75  dBm
	PWRI(0x160),   // 46   14  dBm
	PWRI(0x16B),   // 47   14.25  dBm
	PWRI(0x175),   // 48   14.5  dBm
	PWRI(0x180),   // 49   14.75  dBm
	PWRI(0x18B),   // 50   15  dBm
	PWRI(0x197),   // 51   15.25  dBm
	PWRI(0x1A3),   // 52   15.5  dBm
	PWRI(0x1AF),   // 53   15.75  dBm
	PWRI(0x1BB),   // 54   16  dBm
	PWRI(0x1C8),   // 55   16.25  dBm
	PWRI(0x1D6),   // 56   16.5  dBm
	PWRI(0x1E0),   // 57   16.75  dBm
	PWRI(0x1F2),   // 58   17  dBm
	PWRI(0x200),   // 59   17.25  dBm
	PWRI(0x20F),   // 60   17.5  dBm
	PWRI(0x21E),   // 61   17.75  dBm
	PWRI(0x22E),   // 62   18  dBm
	PWRI(0x23F),   // 63   18.25  dBm
	PWRI(0x24F),   // 64   18.5  dBm
	PWRI(0x261),   // 65   18.75  dBm
	PWRI(0x272),   // 66   19  dBm
	PWRI(0x285),   // 67   19.25  dBm
	PWRI(0x298),   // 68   19.5  dBm
	PWRI(0x2AB),   // 69   19.75  dBm
	PWRI(0x2BF),   // 70   20  dBm
	PWRI(0x2D3),   // 71   20.25  dBm
	PWRI(0x2E9),   // 72   20.5  dBm
	PWRI(0x2FE),   // 73   20.75  dBm
	PWRI(0x315),   // 74   21  dBm
	PWRI(0x32C),   // 75   21.25  dBm
	PWRI(0x343),   // 76   21.5  dBm
	PWRI(0x35C),   // 77   21.75  dBm
	PWRI(0x375),   // 78   22  dBm
	PWRI(0x38F),   // 79   22.25  dBm
};

const UINT32 pwr_gain_base_gain_ble = 0x99628c00;//0x00220C00;
#define TPC_PAMAP_TAB_BT_LEN                        (65)
const PWR_REGS cfg_tab_bt[TPC_PAMAP_TAB_BT_LEN] = {
	// pregain
	PWRI(0xB),   // 0
	PWRI(0xB),   // 1
	PWRI(0xB),   // 2
	PWRI(0xC),   // 3
	PWRI(0xC),   // 4
	PWRI(0xD),   // 5
	PWRI(0xD),   // 6
	PWRI(0xE),   // 7
	PWRI(0xE),   // 8
	PWRI(0xE),   // 9
	PWRI(0xF),   // 10
	PWRI(0xF),   // 11
	PWRI(0x10),  // 12
	PWRI(0x10),  // 13
	PWRI(0x11),  // 14
	PWRI(0x12),  // 15
	PWRI(0x13),  // 16
	PWRI(0x14),  // 17
	PWRI(0x15),  // 18
	PWRI(0x16),  // 19
	PWRI(0x17),  // 20
	PWRI(0x18),  // 21
	PWRI(0x18),  // 22
	PWRI(0x19),  // 23
	PWRI(0x1A),  // 24
	PWRI(0x1B),  // 25
	PWRI(0x1C),  // 26
	PWRI(0x1D),  // 27
	PWRI(0x1E),  // 28
	PWRI(0x1F),  // 29
	PWRI(0x20),  // 30
	PWRI(0x22),  // 31
	PWRI(0x23),  // 32
	PWRI(0x26),  // 33
	PWRI(0x29),  // 34
	PWRI(0x2C),   // 35
	PWRI(0x2D),   // 36
	PWRI(0x30),   // 37
	PWRI(0x32),   // 38
	PWRI(0x34),   // 39
	PWRI(0x36),   // 40
	PWRI(0x37),   // 41
	PWRI(0x38),   // 42
	PWRI(0x39),   // 43
	PWRI(0x3A),   // 44
	PWRI(0x3B),   // 45
	PWRI(0x3D),   // 46
	PWRI(0x3F),   // 47
	PWRI(0x40),   // 48
	PWRI(0x4F),   // 49
	PWRI(0x51),   // 50
	PWRI(0x54),   // 51
	PWRI(0x56),   // 52
	PWRI(0x59),   // 53
	PWRI(0x5B),   // 54
	PWRI(0X5E),   // 55
	PWRI(0x61),   // 56
	PWRI(0x63),   // 57
	PWRI(0x66),   // 58
	PWRI(0x69),   // 59
	PWRI(0x6C),   // 60
	PWRI(0x70),   // 61
	PWRI(0x73),   // 62
	PWRI(0x76),   // 63
	PWRI(0x7A),   // 64
};

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

const AUTO_PWR_CALI_CONTEXT auto_pwr =
{
    0x1,      /*0: CALI_MODE_AUTO  1: CALI_MODE_MANUAL */
    0x253,   /* gtx_tssi_thred_chan1_b */
    0x253,   /* gtx_tssi_thred_chan7_b */
    0x257,   /* gtx_tssi_thred_chan13_b */
    0x23F,   /* gtx_tssi_thred_chan1_g */
    0x22B,   /* gtx_tssi_thred_chan7_g */
    0x22B,   /* gtx_tssi_thred_chan13_g */
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
    vnd_cal_set_auto_pwr_thred(auto_pwr);
    //EPA
#if EPA_ENABLE_FLAG
    vnd_cal_set_epa_config(1, GPIO_28, GPIO_26, pwr_gain_base_gain_b, pwr_gain_base_gain_g);
#else
    vnd_cal_set_epa_config(0, GPIO_28, GPIO_26, pwr_gain_base_gain_b, pwr_gain_base_gain_g);
#endif
}

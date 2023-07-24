#ifndef VND_CAL_H
#define VND_CAL_H


#include "bk_arm_arch.h"
#include "bk_misc.h"

#define PWRI(gain, rb_28_31, pactrl, padctrl, rc_0_2, ra_8_13, ra_4_7, ra_0_1)      \
{                                     \
     .unuse      = 0,            \
     .pregain    = gain,         \
     .regc_8_10  = pactrl,      \
     .regc_4_6   = padctrl,       \
}

typedef struct tmp_pwr_st {//do
    unsigned trx0x0c_12_15 : 4; //not used on BK7231N actually
    signed p_index_delta : 6;
    signed p_index_delta_g : 6;
    signed p_index_delta_ble : 6;
    signed xtal_c_dlta : 8;
} TMP_PWR_ST;

typedef struct txpwr_st {
    UINT8 value;
} TXPWR_ST, *TXPWR_PTR;

typedef struct
{
    UINT32 cali_mode;
    INT32 gtx_tssi_thred_chan1_b;
    INT32 gtx_tssi_thred_chan7_b;
    INT32 gtx_tssi_thred_chan13_b;
    INT32 gtx_tssi_thred_chan1_g;
    INT32 gtx_tssi_thred_chan7_g;
    INT32 gtx_tssi_thred_chan13_g;

} AUTO_PWR_CALI_CONTEXT;

/// POWER
typedef struct
{
    unsigned int regc_4_6   : 4;
    unsigned int regc_8_10  : 4;
    unsigned int pregain    : 10;
    unsigned int unuse      : 14;
} PWR_REGS;

void vnd_cal_overlay(void);

//typedef void (*hook_function_t)(void);

extern void vnd_cal_txpwr_tab_def_b_overlay(TXPWR_ST *txpwr_tab_def_ptr);
extern void vnd_cal_txpwr_tab_def_g_overlay(TXPWR_ST *txpwr_tab_def_ptr);
extern void vnd_cal_txpwr_tab_def_n40_overlay(TXPWR_ST *txpwr_tab_def_ptr);
extern void vnd_cal_txpwr_tab_def_ble_overlay(TXPWR_ST *txpwr_tab_def_ptr);
extern void vnd_cal_tmp_pwr_tab_overlay(TMP_PWR_ST *tmp_pwr_tab_def_ptr);
extern void vnd_cal_gxtal_overlay(uint32 overlay_xtal);
extern void vnd_cal_gcmtag_overlay(uint32 overlay_cmtag);
extern void vnd_cal_pwr_shift_b_tab_overlay(INT16 *pwr_shift_b_def_ptr);
extern void vnd_cal_pwr_shift_g_tab_overlay(INT16 *pwr_shift_g_def_ptr);
extern void vnd_cal_pwr_shift_n_tab_overlay(INT16 *pwr_shift_n_def_ptr);
extern void vnd_cal_pwr_shift_n40_tab_overlay(INT16 *pwr_shift_n40_def_ptr);
extern void vnd_cal_pwr_gain_base_ble_overlay(uint32 pwr_gain_base_ble_value);
extern void vnd_cal_set_ble_pwr_level(uint8 level);
extern void vnd_cal_set_auto_pwr_flag(uint8 flag);
extern void vnd_cal_set_auto_pwr_thred(AUTO_PWR_CALI_CONTEXT auto_pwr);
extern void vnd_cal_set_epa_config(UINT8 epa_flag, UINT16 rx_gpio, UINT16 tx_gpio, UINT32 gainbase_b, UINT32 gainbase_g);
#endif
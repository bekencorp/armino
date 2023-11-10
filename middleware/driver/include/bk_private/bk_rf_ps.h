
#define RF_DEV_NAME       "rf_ctrl"


/* CMD_RF_HOLD_BIT_SET/_CLR*/
#define RF_HOLD_RF_SLEEP_BIT                         (1 << 15)
#define RF_HOLD_BY_USER_BIT                          (1 << 14)
#define RF_HOLD_BY_BKREG_BIT                         (1 << 13)
#define RF_HOLD_BY_ATE_BIT                           (1 << 12)
#define RF_HOLD_BY_PHY_BIT                           (1 << 11)
#define RF_HOLD_BY_MAC_USE_BIT                       (1 << 10)
#define RF_HOLD_BY_CONNECT_BIT                       (1 << 9)
#define RF_HOLD_BY_KEY_BIT                           (1 << 8)
#define RF_HOLD_BY_SCAN_BIT                          (1 << 7)
#define RF_HOLD_BY_IBSS_BIT                          (1 << 6)
#define RF_HOLD_BY_MESH_BIT                          (1 << 5)
#define RF_HOLD_BY_MONITOR_BIT                       (1 << 4)
#define RF_HOLD_BY_AP_BIT                            (1 << 3)
#define RF_HOLD_BY_TEMP_BIT                          (1 << 2)
#define RF_HOLD_BY_BLE_BIT                           (1 << 1)
#define RF_HOLD_BY_STA_BIT                           (1 << 0)


enum {
    CMD_RF_INIT,
    CMD_RF_UNCONDITIONAL_RF_DOWN,
    CMD_RF_UNCONDITIONAL_RF_UP,
    CMD_RF_HOLD_BIT_SET,
    CMD_RF_HOLD_BIT_CLR,
};

extern UINT8 rf_sleeped ( void );

extern void power_save_wake_rf_if_in_sleep ( void );
extern void power_save_check_clr_rf_prevent_flag ( void );
extern void power_save_set_temp_use_rf_flag ( void );
extern void power_save_clr_temp_use_rf_flag ( void );




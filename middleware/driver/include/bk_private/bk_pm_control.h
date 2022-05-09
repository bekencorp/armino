

enum {
    PS_ALLOW,
    PS_UNALLOW,
};

//Because what to do control
enum {
    PS_EVENT_BKREG,
    PS_EVENT_ATE,
    PS_EVENT_PHY,
    PS_EVENT_MAC,
    PS_EVENT_CONNECT,
    PS_EVENT_KEY,
    PS_EVENT_SCAN,
    PS_EVENT_IBSS,
    PS_EVENT_MESH,
    PS_EVENT_MONITOR,
    PS_EVENT_AP,
    PS_EVENT_BLE,
    PS_EVENT_TEMP,
    PS_EVENT_STA,
    PS_EVENT_RESET,
    PS_EVENT_NOPS,

};

//be Controlled sleep type
enum {
    PM_MAC,
    PM_RF,
    PM_MCU,
};
#define PM_MAC_BIT            (1UL<<(PM_MAC))
#define PM_RF_BIT            (1UL<<(PM_RF))
#define PM_MCU_BIT            (1UL<<(PM_MCU))


extern UINT32 ps_switch ( UINT32 if_allow, UINT32 event , UINT32 modules );



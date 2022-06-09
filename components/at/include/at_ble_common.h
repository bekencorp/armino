#ifndef AT_BLE_COMMON_H_
#define AT_BLE_COMMON_H_
//#if (CONFIG_BLE_5_X || CONFIG_BTDM_5_2)
#include "at_common.h"


/// reference common_bt_defines.h
#define ADV_INTERVAL_MIN    0x0020 //(20 ms)
#define ADV_INTERVAL_MAX    0x4000 //(10.24 sec)
#define CON_INTERVAL_MIN    0x0006  //(7.5 msec)
#define CON_INTERVAL_MAX    0x0C80  //(4 sec)
/// Supervision TO (N*10ms) (chapter 2.E.7.8.12)
#define CON_SUP_TO_MIN      0x000A  //(100 msec)
#define CON_SUP_TO_MAX      0x0C80  //(32 sec)
/// Scanning interval (in 625us slot) (chapter 2.E.7.8.10)
#define SCAN_INTERVAL_MIN   0x0004 //(2.5 ms)
#define SCAN_INTERVAL_MAX   0x4000 //(10.24 sec)
#define SCAN_INTERVAL_DFT   0x0010 //(10 ms)
/// Scanning window (in 625us slot) (chapter 2.E.7.8.10)
#define SCAN_WINDOW_MIN     0x0004 //(2.5 ms)
#define SCAN_WINDOW_MAX     0x4000 //(10.24 sec)
#define SCAN_WINDOW_DFT     0x0010 //(10 ms)
/// Connection latency (N*cnx evt) (chapter 2.E.7.8.12)
#define CON_LATENCY_MIN     0x0000
#define CON_LATENCY_MAX     0x01F3  // (499)


#define UNKNOW_ACT_IDX         0xFFU

#define AT_BLE_MAX_ACTV                  bk_ble_get_max_actv_idx_count()
#define AT_BLE_MAX_CONN                  bk_ble_get_max_conn_idx_count()

// sync actv state machine, reference actv_state_t
typedef enum {
    AT_ACTV_IDLE,
    /////adv
    AT_ACTV_ADV_CREATED,
    AT_ACTV_ADV_STARTED,
    ////////scan
    AT_ACTV_SCAN_CREATED,
    AT_ACTV_SCAN_STARTED,

    AT_ACTV_INIT_CREATED,
    AT_ACTV_PER_SYNC_CREATED,
    AT_ACTV_PER_SYNC_STARTED,
} at_actv_state;

/// Initing state machine, reference app_init_state
enum at_init_state
{
	/// Iint activity does not exists
	AT_INIT_STATE_IDLE = 0,
	/// Creating Iint activity
	AT_INIT_STATE_CREATING,
	/// Iint activity created
	AT_INIT_STATE_CREATED,

	/// WAIT Start Iint activity
	AT_INIT_STATE_WAIT_CONECTTING = 3,
	/// Starting Iint activity
	AT_INIT_STATE_CONECTTING = 4,
	/// Iint activity conected
	AT_INIT_STATE_CONECTED = 5,
	/// Stopping Iint activity
	AT_INIT_STATE_STOPPING = 6,
};

/// Own BD address source of the device
enum gapm_own_addr
{
   /// Public or Private Static Address according to device address configuration
   BLE_STATIC_ADDR,
   /// Generated resolvable private random address
   BLE_GEN_RSLV_ADDR,
   /// Generated non-resolvable private random address
   BLE_GEN_NON_RSLV_ADDR,
};

enum le_phy_mask
{
    /// The Host prefers to use the LE 1M transmitter/receiver PHY (possibly among others)
    PHY_1MBPS_BIT      = (1<<0),
    PHY_1MBPS_POS      = (0),
    /// The Host prefers to use the LE 2M transmitter/receiver PHY (possibly among others)
    PHY_2MBPS_BIT      = (1<<1),
    PHY_2MBPS_POS      = (1),
    /// The Host prefers to use the LE Coded transmitter/receiver PHY (possibly among others)
    PHY_CODED_BIT      = (1<<2),
    PHY_CODED_POS      = (2),
    /// The Host prefers to use the LE Coded transmitter/receiver PHY (possibly among others)
    PHY_ALL            = (PHY_1MBPS_BIT | PHY_2MBPS_BIT | PHY_CODED_BIT),
};

enum le_coded_phy_option {
	/// The host no preferred
	CODED_NO_PREFEER = 0,
	CODED_S2_PREFEER,
	CODED_S8_PREFEER,
};

enum
{
	PRF_TASK_ID_BOARDING = 10,
	PRF_TASK_ID_MAX,
};

#endif

//#endif

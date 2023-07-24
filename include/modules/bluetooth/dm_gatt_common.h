#pragma once

#include <stdint.h>
#include <stdbool.h>


#include "modules/bluetooth/dm_bt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup dm_gatt_common_v1 GATT COMMON API
 * @{
 */


/**
 * @defgroup dm_gatt_common_types_v1 Defines
 * @{
 */



/// @}




/**
 * @defgroup dm_gatt_common_functions_v1 Functions
 * @{
 */

/**
 * @brief           This function is called to set local MTU,
 *                  the function is called before BLE connection.
 *
 * @param[in]       mtu: the size of MTU. Max is 517, min is 23
 *
 * @return
 *                  - BK_ERR_BLE_SUCCESS: success
 *                  - other: failed
 *
 */
ble_err_t bk_ble_gatt_set_local_mtu (uint16_t mtu);



/// @}

/// @}

#ifdef __cplusplus
}
#endif


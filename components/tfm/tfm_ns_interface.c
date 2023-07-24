/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*
 * An example to implement tfm_ns_interface_dispatch() in NS RTOS to integrate
 * TF-M interface on Armv8-M TrustZone based platforms.
 *
 * In this example, NS OS calls mutex in tfm_ns_interface_dispatch() to
 * synchronize multiple NS client calls.
 * NS OS pseudo code in this example is not based on any specific RTOS.
 *
 * Please note that this example cannot be built directly.
 */

#include <stdint.h>

/* Include NS RTOS specific mutex declarations */
#include <os/os.h>
#include "tfm_ns_interface.h"
#include "components/log.h"

/* Static ns lock handle */
static beken_mutex_t ns_lock_handle = NULL;

void ns_interface_delay(INT32 num)
{
	volatile INT32 i, j;

	for (i = 0; i < num; i ++) {
		for (j = 0; j < 100; j ++)
			;
	}
}

/* Initialize the ns lock */
int32_t ns_interface_lock_init(void)
{
    /* NS RTOS specific mutex creation/initialization */
    rtos_init_mutex(&ns_lock_handle);
    if (ns_lock_handle) {
        return BK_OK;
    }

    return BK_FAIL;
}

int32_t tfm_ns_interface_dispatch(veneer_fn fn,
                                  uint32_t arg0, uint32_t arg1,
                                  uint32_t arg2, uint32_t arg3)
{
    int32_t result;

    /* TF-M request protected by NS lock. */
    while (rtos_lock_mutex(&ns_lock_handle) != BK_OK);
    result = fn(arg0, arg1, arg2, arg3);

    /*
     * Whether to check/handle lock release return code depends on NS RTOS
     * specific implementation and usage scenario.
     */
    rtos_unlock_mutex(&ns_lock_handle);

    return result;
}

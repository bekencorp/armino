/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "config_impl.h"
#include "security_defs.h"
#include "tfm_psa_call_pack.h"

#include "psa/client.h"

/*
 * This is the veneers of FF-M Client APIs, except for Armv8.0-m.
 * The interfaces are written in C unlike Armv8.0-m because reentrant detection
 * is done by the architecture.
 *
 * As NS Agent is also a Secure Partition, it can call the client APIs directly.
 *
 */

__tz_c_veneer
uint32_t tfm_psa_framework_version_veneer(void)
{
    return psa_framework_version();
}

__tz_c_veneer
uint32_t tfm_psa_version_veneer(uint32_t sid)
{
    return psa_version(sid);
}

__tz_c_veneer
psa_status_t tfm_psa_call_veneer(psa_handle_t handle,
                                 uint32_t ctrl_param,
                                 const psa_invec *in_vec,
                                 psa_outvec *out_vec)
{
    return tfm_psa_call_pack(handle, ctrl_param, in_vec, out_vec);
}

/* Following veneers are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1
__tz_c_veneer
psa_handle_t tfm_psa_connect_veneer(uint32_t sid, uint32_t version)
{
    return psa_connect(sid, version);
}

__tz_c_veneer
void tfm_psa_close_veneer(psa_handle_t handle)
{
    psa_close(handle);
}
#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */

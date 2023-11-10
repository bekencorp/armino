/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/client.h"
#include "tfm_ns_interface.h"
#include "tfm_api.h"
#include "tfm_psa_call_pack.h"

/* API functions for connection based services */

psa_handle_t psa_connect(uint32_t sid, uint32_t version)
{
    return tfm_ns_interface_dispatch((veneer_fn)tfm_psa_connect_veneer, sid, version, 0, 0);
}

void psa_close(psa_handle_t handle)
{
    (void)tfm_ns_interface_dispatch((veneer_fn)tfm_psa_close_veneer, (uint32_t)handle, 0, 0, 0);
}

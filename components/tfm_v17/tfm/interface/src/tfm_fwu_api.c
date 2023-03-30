/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/client.h"
#include "psa/update.h"
#include "psa_manifest/sid.h"
#include "tfm_api.h"

psa_status_t psa_fwu_start(psa_fwu_component_t component,
                           const void *manifest,
                           size_t manifest_size)
{
    psa_invec in_vec[] = {
        { .base = &component, .len = sizeof(component) },
        { .base = manifest, .len = manifest_size }
    };

    return psa_call(TFM_FIRMWARE_UPDATE_SERVICE_HANDLE, TFM_FWU_START,
                    in_vec, IOVEC_LEN(in_vec), NULL, 0);
}

psa_status_t psa_fwu_write(psa_fwu_component_t component,
                           size_t image_offset,
                           const void *block,
                           size_t block_size)
{
    psa_invec in_vec[] = {
        { .base = &component, .len = sizeof(component) },
        { .base = &image_offset, .len = sizeof(image_offset) },
        { .base = block, .len = block_size }
    };

    return psa_call(TFM_FIRMWARE_UPDATE_SERVICE_HANDLE, TFM_FWU_WRITE,
                      in_vec, IOVEC_LEN(in_vec), NULL, 0);
}

psa_status_t psa_fwu_finish(psa_fwu_component_t component)
{
    psa_invec in_vec[] = {
        { .base = &component, .len = sizeof(component) },
    };

    return psa_call(TFM_FIRMWARE_UPDATE_SERVICE_HANDLE, TFM_FWU_FINISH,
                    in_vec, IOVEC_LEN(in_vec), NULL, 0);
}

psa_status_t psa_fwu_install(void)
{
    return psa_call(TFM_FIRMWARE_UPDATE_SERVICE_HANDLE, TFM_FWU_INSTALL,
                    NULL, 0, NULL, 0);
}

psa_status_t psa_fwu_cancel(psa_fwu_component_t component)
{
    psa_invec in_vec[] = {
        { .base = &component, .len = sizeof(component) },
    };

    return psa_call(TFM_FIRMWARE_UPDATE_SERVICE_HANDLE, TFM_FWU_CANCEL,
                    in_vec, IOVEC_LEN(in_vec), NULL, 0);
}

psa_status_t psa_fwu_clean(psa_fwu_component_t component)
{
    psa_invec in_vec[] = {
        { .base = &component, .len = sizeof(component) },
    };

    return psa_call(TFM_FIRMWARE_UPDATE_SERVICE_HANDLE, TFM_FWU_CLEAN,
                    in_vec, IOVEC_LEN(in_vec), NULL, 0);
}

psa_status_t psa_fwu_query(psa_fwu_component_t component,
                           psa_fwu_component_info_t *info)
{
    psa_invec in_vec[] = {
        { .base = &component, .len = sizeof(component) }
    };
    psa_outvec out_vec[] = {
        { .base = info, .len = sizeof(*info)}
    };

    return psa_call(TFM_FIRMWARE_UPDATE_SERVICE_HANDLE, TFM_FWU_QUERY,
                    in_vec, IOVEC_LEN(in_vec), out_vec, IOVEC_LEN(out_vec));
}

psa_status_t psa_fwu_request_reboot(void)
{
    return psa_call(TFM_FIRMWARE_UPDATE_SERVICE_HANDLE, TFM_FWU_REQUEST_REBOOT,
                    NULL, 0, NULL, 0);
}

psa_status_t psa_fwu_accept(void)
{
    return psa_call(TFM_FIRMWARE_UPDATE_SERVICE_HANDLE, TFM_FWU_ACCEPT,
                    NULL, 0, NULL, 0);
}

psa_status_t psa_fwu_reject(psa_status_t error)
{
    psa_invec in_vec[] = {
        { .base = &error, .len = sizeof(error) }
    };

    return psa_call(TFM_FIRMWARE_UPDATE_SERVICE_HANDLE, TFM_FWU_REJECT,
                    in_vec, IOVEC_LEN(in_vec), NULL, 0);
}

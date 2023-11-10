/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "cmsis_compiler.h"
#include "config_its.h"
#include "psa/storage_common.h"
#include "tfm_internal_trusted_storage.h"
#include "its_utils.h"

#include "psa/framework_feature.h"
#include "psa/service.h"
#include "psa_manifest/tfm_internal_trusted_storage.h"
#include "tfm_its_defs.h"
#if PSA_FRAMEWORK_HAS_MM_IOVEC != 1
#include "flash/its_flash.h"
#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC != 1 */

#if PSA_FRAMEWORK_HAS_MM_IOVEC != 1
/* Buffer to store asset data from the caller.
 * Note: size must be aligned to the max flash program unit to meet the
 * alignment requirement of the filesystem.
 */
static uint8_t __ALIGNED(4) asset_data[ITS_UTILS_ALIGN(ITS_BUF_SIZE,
                                          ITS_FLASH_MAX_ALIGNMENT)];
#endif

static psa_status_t tfm_its_set_req(const psa_msg_t *msg)
{
    psa_status_t status;
    psa_storage_uid_t uid;
    uint8_t *data_buf;
    size_t size_remaining;
    size_t offset;
    psa_storage_create_flags_t create_flags;
    struct its_asset_info asset_info;
    size_t num;

    if (msg->in_size[0] != sizeof(uid) ||
        msg->in_size[2] != sizeof(create_flags)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 0, &uid, sizeof(uid));
    if (num != sizeof(uid)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 2, &create_flags, sizeof(create_flags));
    if (num != sizeof(create_flags)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    asset_info.uid = uid;
    asset_info.client_id = msg->client_id;
    asset_info.create_flags = create_flags;

    size_remaining = msg->in_size[1];
    offset = 0;

#if PSA_FRAMEWORK_HAS_MM_IOVEC == 1
    if (size_remaining != 0) {
        data_buf = (uint8_t *)psa_map_invec(msg->handle, 1);
    } else {
        /* zero-size asset is supported */
        data_buf = NULL;
    }

    status = tfm_its_set(&asset_info, data_buf, size_remaining,
                         size_remaining, offset);
#else
    data_buf = asset_data;
    do {
        num = psa_read(msg->handle, 1, asset_data,
                       ITS_UTILS_MIN(size_remaining, sizeof(asset_data)));

        status = tfm_its_set(&asset_info, data_buf, size_remaining,
                             num, offset);
        if (status != PSA_SUCCESS) {
            return status;
        }

        size_remaining -= num;
        offset += num;
    } while (size_remaining);
#endif

    return status;
}

static psa_status_t tfm_its_get_req(const psa_msg_t *msg)
{
    psa_status_t status;
    psa_storage_uid_t uid;
    uint8_t *data_buf;
    size_t size_to_read;
    size_t data_offset;
    size_t out_size;
    size_t size_read;
    size_t num;
    struct its_asset_info asset_info;
    bool first_get;

    if (msg->in_size[0] != sizeof(uid) ||
        msg->in_size[1] != sizeof(data_offset)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 0, &uid, sizeof(uid));
    if (num != sizeof(uid)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 1, &data_offset, sizeof(data_offset));
    if (num != sizeof(data_offset)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    asset_info.uid = uid;
    asset_info.client_id = msg->client_id;
    out_size = msg->out_size[0];
    first_get = true;

#if PSA_FRAMEWORK_HAS_MM_IOVEC == 1
    size_to_read = msg->out_size[0];
    if (size_to_read != 0) {
        data_buf = (uint8_t *)psa_map_outvec(msg->handle, 0);
    } else {
        data_buf = NULL;
    }

    status = tfm_its_get(&asset_info, data_buf, size_to_read,
                         data_offset, &size_read, first_get);
    if (status == PSA_SUCCESS && size_to_read != 0) {
        /* Unmap to update caller’s outvec with the number of bytes written  */
        psa_unmap_outvec(msg->handle, 0, size_read);
    }
#else
    /* Fill in the outvec unless no data left */
    data_buf = asset_data;
    do {
        size_to_read = ITS_UTILS_MIN(out_size, sizeof(asset_data));
        status = tfm_its_get(&asset_info, data_buf, size_to_read,
                             data_offset, &size_read, first_get);
        if (status != PSA_SUCCESS) {
            return status;
        }
        if (size_read == 0) {
            /* No more data */
            return PSA_SUCCESS;
        }

        psa_write(msg->handle, 0, data_buf, size_read);

        first_get = false;
        out_size -= size_read;
        data_offset += size_read;
    } while (out_size > 0);
#endif

    return status;
}

static psa_status_t tfm_its_get_info_req(const psa_msg_t *msg)
{
    psa_status_t status;
    psa_storage_uid_t uid;
    struct psa_storage_info_t info;
    size_t num;

    if (msg->in_size[0] != sizeof(uid) ||
        msg->out_size[0] != sizeof(info)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 0, &uid, sizeof(uid));
    if (num != sizeof(uid)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    status = tfm_its_get_info(msg->client_id, uid, &info);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, &info, sizeof(info));
    }

    return status;
}

static psa_status_t tfm_its_remove_req(const psa_msg_t *msg)
{
    psa_storage_uid_t uid;
    size_t num;

    if (msg->in_size[0] != sizeof(uid)) {
        /* The input argument size is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 0, &uid, sizeof(uid));
    if (num != sizeof(uid)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return tfm_its_remove(msg->client_id, uid);
}

psa_status_t tfm_its_entry(void)
{
    return tfm_its_init();
}

psa_status_t tfm_internal_trusted_storage_service_sfn(const psa_msg_t *msg)
{
    switch (msg->type) {
    case TFM_ITS_SET:
        return tfm_its_set_req(msg);
    case TFM_ITS_GET:
        return tfm_its_get_req(msg);
    case TFM_ITS_GET_INFO:
        return tfm_its_get_info_req(msg);
    case TFM_ITS_REMOVE:
        return tfm_its_remove_req(msg);
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return PSA_ERROR_GENERIC_ERROR;
}

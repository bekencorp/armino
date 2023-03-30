/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_ps_req_mngr.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "psa/protected_storage.h"

#include "tfm_api.h"
#include "tfm_protected_storage.h"
#include "psa/service.h"
#include "psa_manifest/tfm_protected_storage.h"
#include "tfm_ps_defs.h"

static const psa_msg_t *p_msg;

static psa_status_t tfm_ps_set_req(const psa_msg_t *msg)
{
    psa_storage_uid_t uid;
    int32_t client_id;
    psa_storage_create_flags_t create_flags;
    size_t num = 0;

    client_id = msg->client_id;

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

    return tfm_ps_set(client_id, uid, msg->in_size[1], create_flags);
}

static psa_status_t tfm_ps_get_req(const psa_msg_t *msg)
{
    psa_storage_uid_t uid;
    uint32_t data_offset;
    size_t num = 0;
    size_t p_data_length;

    if (msg->in_size[0] != sizeof(uid) ||
        msg->in_size[1] != sizeof(data_offset)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 0, &uid,  sizeof(uid));
    if (num != sizeof(psa_storage_uid_t)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 1, &data_offset, sizeof(data_offset));
    if (num != sizeof(data_offset)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return tfm_ps_get(msg->client_id, uid, data_offset,  msg->out_size[0],
                      &p_data_length);
}

static psa_status_t tfm_ps_get_info_req(const psa_msg_t *msg)
{
    psa_storage_uid_t uid;

    struct psa_storage_info_t info;
    size_t num = 0;
    psa_status_t status;

    if (msg->in_size[0] != sizeof(uid) ||
        msg->out_size[0] != sizeof(info)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 0, &uid, sizeof(uid));
    if (num != sizeof(uid)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    status = tfm_ps_get_info(msg->client_id, uid, &info);

    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, &info, sizeof(info));
    }
    return status;
}

static psa_status_t tfm_ps_remove_req(const psa_msg_t *msg)
{
    psa_storage_uid_t uid;
    size_t num = 0;

    if (msg->in_size[0] != sizeof(uid)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 0, &uid, sizeof(uid));
    if (num != sizeof(uid)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return tfm_ps_remove(msg->client_id, uid);
}

static psa_status_t tfm_ps_get_support_req(const psa_msg_t *msg)
{
    size_t out_size;
    uint32_t support_flags;

    out_size = msg->out_size[0];
    if (out_size != sizeof(support_flags)) {
        /* The output argument size is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    support_flags = tfm_ps_get_support();
    psa_write(msg->handle, 0, &support_flags, out_size);
    return PSA_SUCCESS;
}

psa_status_t tfm_protected_storage_service_sfn(const psa_msg_t *msg)
{
    p_msg = msg;

    switch (msg->type) {
    case TFM_PS_SET:
        return tfm_ps_set_req(msg);
    case TFM_PS_GET:
        return tfm_ps_get_req(msg);
    case TFM_PS_GET_INFO:
        return tfm_ps_get_info_req(msg);
    case TFM_PS_REMOVE:
        return tfm_ps_remove_req(msg);
    case TFM_PS_GET_SUPPORT:
        return tfm_ps_get_support_req(msg);
    default:
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return PSA_ERROR_GENERIC_ERROR;
}

psa_status_t tfm_ps_entry(void)
{
    return tfm_ps_init();
}

psa_status_t ps_req_mngr_read_asset_data(uint8_t *out_data, uint32_t size)
{
    size_t num = 0;

    num = psa_read(p_msg->handle, 1, out_data, size);
    if (num != size) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    return PSA_SUCCESS;
}

void ps_req_mngr_write_asset_data(const uint8_t *in_data, uint32_t size)
{
    psa_write(p_msg->handle, 0, in_data, size);
}

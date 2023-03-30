/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>

#include "psa/error.h"
#include "psa/client.h"
#include "psa/initial_attestation.h"
#include "psa/crypto.h"
#include "attest.h"

#include "array.h"
#include "psa/framework_feature.h"
#include "psa/service.h"
#include "psa_manifest/tfm_initial_attestation.h"
#include "region_defs.h"
#include "tfm_attest_defs.h"

#define ECC_P256_PUBLIC_KEY_SIZE PSA_KEY_EXPORT_ECC_PUBLIC_KEY_MAX_SIZE(256)

typedef psa_status_t (*attest_func_t)(const psa_msg_t *msg);

int32_t g_attest_caller_id;

#if PSA_FRAMEWORK_HAS_MM_IOVEC == 1
static psa_status_t psa_attest_get_token(const psa_msg_t *msg)
{
    psa_status_t status;
    const void *challenge_buff;
    void *token_buff;
    size_t challenge_size;
    size_t token_buff_size;
    size_t token_size;

    token_buff_size = msg->out_size[0];
    challenge_size = msg->in_size[0];

    if (challenge_size > PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64
        || challenge_size == 0 || token_buff_size == 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* store the client ID here for later use in service */
    g_attest_caller_id = msg->client_id;

    challenge_buff = psa_map_invec(msg->handle, 0);
    token_buff = psa_map_outvec(msg->handle, 0);

    status = initial_attest_get_token(challenge_buff, challenge_size,
                                      token_buff, token_buff_size, &token_size);
    if (status == PSA_SUCCESS) {
        psa_unmap_outvec(msg->handle, 0, token_size);
    }

    return status;
}
#else /* PSA_FRAMEWORK_HAS_MM_IOVEC == 1 */
/* Buffer to store the created attestation token. */
static uint8_t token_buff[PSA_INITIAL_ATTEST_TOKEN_MAX_SIZE];

static psa_status_t psa_attest_get_token(const psa_msg_t *msg)
{
    psa_status_t status = PSA_SUCCESS;
    uint8_t challenge_buff[PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64];
    uint32_t bytes_read = 0;
    size_t challenge_size;
    size_t token_buff_size;
    size_t token_size;

    challenge_size = msg->in_size[0];
    token_buff_size = msg->out_size[0] < sizeof(token_buff) ?
                                          msg->out_size[0] : sizeof(token_buff);

    if (challenge_size > PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64
        || challenge_size == 0 || token_buff_size == 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* store the client ID here for later use in service */
    g_attest_caller_id = msg->client_id;

    bytes_read = psa_read(msg->handle, 0, challenge_buff, challenge_size);
    if (bytes_read != challenge_size) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    status = initial_attest_get_token(challenge_buff, challenge_size,
                                      token_buff, token_buff_size, &token_size);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, token_buff, token_size);
    }

    return status;
}
#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC == 1 */

static psa_status_t psa_attest_get_token_size(const psa_msg_t *msg)
{
    psa_status_t status = PSA_SUCCESS;
    size_t challenge_size;
    size_t token_size;
    size_t bytes_read = 0;

    if (msg->in_size[0] != sizeof(challenge_size)
        || msg->out_size[0] != sizeof(token_size)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* store the client ID here for later use in service */
    g_attest_caller_id = msg->client_id;

    bytes_read = psa_read(msg->handle, 0,
                          &challenge_size, msg->in_size[0]);
    if (bytes_read != sizeof(challenge_size)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    status = initial_attest_get_token_size(challenge_size, &token_size);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, &token_size, sizeof(token_size));
    }

    return status;
}

psa_status_t tfm_attestation_service_sfn(const psa_msg_t *msg)
{
    switch (msg->type) {
    case TFM_ATTEST_GET_TOKEN:
        return psa_attest_get_token(msg);
    case TFM_ATTEST_GET_TOKEN_SIZE:
        return psa_attest_get_token_size(msg);
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return PSA_ERROR_GENERIC_ERROR;
}

psa_status_t attest_partition_init(void)
{
    return attest_init();
}

/*
 * Copyright (c) 2020 Arm Limited
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "psa_adac.h"

psa_status_t psa_adac_mac_verify(psa_algorithm_t alg, const uint8_t *inputs[],
        size_t input_sizes[], size_t input_count, const uint8_t key[],
        size_t key_size, uint8_t mac[], size_t mac_size)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_adac_derive_key(uint8_t *crt, size_t crt_size,
        uint8_t key_type, uint8_t *key, size_t key_size)
{
    return PSA_ERROR_NOT_SUPPORTED;
}

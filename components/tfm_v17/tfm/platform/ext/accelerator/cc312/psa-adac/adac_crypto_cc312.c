/*
 * Copyright (c) 2021 Arm Limited
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "psa_adac_debug.h"
#include "adac_crypto_cc312.h"

#include "psa/crypto_types.h"

psa_status_t psa_adac_crypto_init() {
    PSA_ADAC_LOG_INFO("cc312", "CryptoCell Initialization function\n");
    return PSA_SUCCESS;
}


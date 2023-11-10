/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CONFIG_PARTITION_ATTEST_H__
#define __CONFIG_PARTITION_ATTEST_H__

#include "config_tfm.h"

/* Include optional claims in initial attestation token */
#ifndef ATTEST_INCLUDE_OPTIONAL_CLAIMS
#pragma message("ATTEST_INCLUDE_OPTIONAL_CLAIMS is defaulted to 1. Please check and set it explicitly.")
#define ATTEST_INCLUDE_OPTIONAL_CLAIMS 1
#endif

/* Include COSE key-id in initial attestation token */
#ifndef ATTEST_INCLUDE_COSE_KEY_ID
#pragma message("ATTEST_INCLUDE_COSE_KEY_ID is defaulted to 0. Please check and set it explicitly.")
#define ATTEST_INCLUDE_COSE_KEY_ID     0
#endif

/* The stack size of the Initial Attestation Secure Partition */
#ifndef ATTEST_STACK_SIZE
#pragma message("ATTEST_STACK_SIZE is defaulted to 0x700. Please check and set it explicitly.")
#define ATTEST_STACK_SIZE              0x700
#endif

/* Set the initial attestation token profile */
#if (!ATTEST_TOKEN_PROFILE_PSA_IOT_1) && \
    (!ATTEST_TOKEN_PROFILE_PSA_2_0_0) && \
    (!ATTEST_TOKEN_PROFILE_ARM_CCA)
#pragma message("ATTEST_TOKEN_PROFILE_PSA_IOT_1 is chosen by default. Please check and set it explicitly.")
#define ATTEST_TOKEN_PROFILE_PSA_IOT_1 1
#endif

#endif /* __CONFIG_PARTITION_ATTEST_H__ */

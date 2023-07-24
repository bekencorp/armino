// Copyright 2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <driver/securityip.h>

#if CONFIG_SECURITYIP
/* MBEDTLS_ERR_SHA512_HW_ACCEL_FAILED is deprecated and should not be used. */
#define MBEDTLS_ERR_SHA512_HW_ACCEL_FAILED                -0x0039  /**< SHA-512 hardware accelerator failed */
#define MBEDTLS_ERR_SHA512_BAD_INPUT_DATA                 -0x0075  /**< SHA-512 input data was malformed. */

#define BK_ALT_MAX_BLOCK_SIZE         (4096)
#define BK_ALT_DIGEST_SHA512_SIZE     (64)

typedef struct
{
    void * Context_p;
    unsigned char digest[BK_ALT_DIGEST_SHA512_SIZE];
    uint8_t * DataLeft;
    uint32_t LenLeft;
#if !defined(MBEDTLS_SHA512_NO_SHA384)
    int is384;                  /*!< Determines which function to use:
                                     0: Use SHA-512, or 1: Use SHA-384. */
#endif
} mbedtls_sha512_context;

#endif //end of CONFIG_SECURITYIP

#ifdef __cplusplus
}
#endif

// #endif
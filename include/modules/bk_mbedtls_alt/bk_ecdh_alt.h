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
#include <mbedtls/bignum.h>
#include <mbedtls/ecdsa.h>
#include <mbedtls/platform.h>
#include <mbedtls/platform_util.h>




#if CONFIG_SECURITYIP

#define MBEDTLS_ERR_ECP_HW_ACCEL_FAILED                   -0x4B80

/* Structures for the ECC curves. */
typedef struct
{
    uint32_t CurveBits;
    const uint8_t * P_p;
    uint32_t PLen;
    const uint8_t * A_p;
    uint32_t ALen;
    const uint8_t * B_p;
    uint32_t BLen;
    const uint8_t * ECPointX_p;
    uint32_t ECPointXLen;
    const uint8_t * ECPointY_p;
    uint32_t ECPointYLen;
    const uint8_t * Order_p;
    uint32_t OrderLen;
    const uint8_t Cofactor;
} TestVector_ECC_Curve_Rec_t;

typedef const TestVector_ECC_Curve_Rec_t * const TestVector_ECC_Curve_t;

/* Discard const qualifier from pointer */
static inline void *
sfzutf_discard_const(
        const void * Ptr_p)
{
    union
    {
        const void * c_p;
        void * n_p;
    } Conversion;

    Conversion.c_p = Ptr_p;
    return Conversion.n_p;
}

#endif //end of CONFIG_SECURITYIP

#ifdef __cplusplus
}
#endif

// #endif
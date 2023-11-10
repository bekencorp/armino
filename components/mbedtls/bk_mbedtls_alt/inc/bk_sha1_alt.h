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

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA1_ALT)
#ifdef __cplusplus
extern "C" {
#endif

#define BK_ALT_DIGEST_SHA1_SIZE            (20)
#define BK_SHA1_ALT_MAX_BLOCK_SIZE         (4096)

typedef struct
{
    void *Context_p;
    unsigned char digest[BK_ALT_DIGEST_SHA1_SIZE];
    uint8_t *DataLeft;
    uint32_t LenLeft;
} mbedtls_sha1_context;

#ifdef __cplusplus
}
#endif  
#endif

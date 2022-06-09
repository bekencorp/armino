/*
 * Copyright (c) 2019-2021 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _FIP_PARSER_H_
#define _FIP_PARSER_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "external/uuid.h"

/* Return code of fip parser APIs */
#define FIP_PARSER_SUCCESS                     (0)
#define FIP_PARSER_ERROR                       (INT32_MIN + 1)

/* This is used as a signature to validate the blob header */
#define TOC_HEADER_NAME 0xAA640001

/* ToC Entry UUIDs */
#define UUID_TRUSTED_BOOT_FIRMWARE_BL2 \
    {{0x5f, 0xf9, 0xec, 0x0b}, {0x4d, 0x22}, \
    {0x3e, 0x4d}, 0xa5, 0x44, \
    {0xc3, 0x9d, 0x81, 0xc7, 0x3f, 0x0a} }

typedef struct _FIP_TOC_HEADER {
    uint32_t    name;
    uint32_t    serial_number;
    uint64_t    flags;
} FIP_TOC_HEADER;

/*fip tool creates fip table using 64 bit integers because of its
  basic support for 64 bit Cortex A.Hence need to add 32 bit padding
  for 32-bit Cortex-M CPUs
*/

typedef struct _FIP_TOC_ENTRY {
    uuid_t      uuid;
    uint32_t    address;
    uint32_t    pad1;
    uint32_t    size;
    uint32_t    pad2;
    uint64_t    flags;
} FIP_TOC_ENTRY;

int parse_fip_and_extract_tfa_info(uint32_t address, uint32_t fip_size,
        uint32_t *tfa_offset, uint32_t *tfa_size);

#ifdef  __cplusplus
}
#endif

#endif /* _FIP_PARSER_H_ */

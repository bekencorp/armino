/*
 * Copyright (c) 2021 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "fip_parser.h"
#include "bootutil/bootutil_log.h"

#include <region_defs.h>
#include <string.h>

int parse_fip_and_extract_tfa_info(uint32_t address, uint32_t fip_size,
        uint32_t *tfa_offset, uint32_t *tfa_size)
{
    FIP_TOC_HEADER *toc_header = NULL;
    FIP_TOC_ENTRY *toc_entry = NULL;
    uuid_t uuid_null = {0};
    uuid_t tfa_uuid = UUID_TRUSTED_BOOT_FIRMWARE_BL2;
    char *iter;

    toc_header = (FIP_TOC_HEADER *) address;

    if (toc_header->name != TOC_HEADER_NAME) {
        return FIP_PARSER_ERROR;
    }

    toc_entry = (FIP_TOC_ENTRY *)(toc_header + 1);

    for (iter = (char *)toc_entry;
        iter <= (char *)toc_header + fip_size;
        iter = iter + sizeof(FIP_TOC_ENTRY), toc_entry++) {

        if (!memcmp(iter, &uuid_null, sizeof(uuid_t))) {
            return FIP_PARSER_ERROR;
        }

        if (!memcmp(iter, &tfa_uuid, sizeof(uuid_t))) {
            BOOT_LOG_INF("TF-A FIP at : address = 0x%x : size = 0x%x \n\r",
                                toc_entry->address,
                                toc_entry->size);
            *tfa_offset = toc_entry->address;
            *tfa_size = toc_entry->size;
            return FIP_PARSER_SUCCESS;
        }
    }

    return FIP_PARSER_ERROR;
}

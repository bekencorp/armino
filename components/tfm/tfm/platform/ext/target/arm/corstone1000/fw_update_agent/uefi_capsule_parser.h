/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef UEFI_CAPSULE_PARSER_H
#define UEFI_CAPSULE_PARSER_H

#include <stdint.h>
#include "fip_parser/external/uuid.h"
#include "flash_layout.h"

enum uefi_capsule_error_t {
    UEFI_CAPSULE_PARSER_SUCCESS = 0,
    UEFI_CAPSULE_PARSER_ERROR = (-1)
};

typedef struct capsule_image_info {
    uint32_t nr_image;
    void *image[NR_OF_IMAGES_IN_FW_BANK];
    struct efi_guid guid[NR_OF_IMAGES_IN_FW_BANK];
    uint32_t size[NR_OF_IMAGES_IN_FW_BANK];
    uint32_t version[NR_OF_IMAGES_IN_FW_BANK];
} capsule_image_info_t;

enum uefi_capsule_error_t uefi_capsule_retrieve_images(void* capsule_ptr,
        capsule_image_info_t* images_info);

#endif /* UEFI_CAPSULE_PARSER_H */

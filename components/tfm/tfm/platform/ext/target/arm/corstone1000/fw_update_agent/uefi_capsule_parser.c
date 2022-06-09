/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "uefi_capsule_parser.h"
#include "fwu_agent.h"
#include <string.h>

/*
Update Capsule Structure (UEFI spec 2.9 1004)
    EFI_CAPSULE_HEADER
    ...
    ...
    ...
    CAPSULE_BODY
        efi_firmware_management_capsule_header
        Optional Driver 1 (item_offset[0])
        Optional Driver 2 (item_offset[1])
            Payload 1 (item_offset[2])
                efi_firmware_management_capsule_iamge_header
                Binary Update image (Image_length == update_image_size)
                Vendor Code bytes  (Data lenght == update_vendorcode_size)
            Payload 2 (item_offset[3])
            ...
            ...
            Payload n (item_offset[embedded_driver_count + payload_item_count -1])
*/

typedef struct {
    struct efi_guid         capsule_guid;
    uint32_t                header_size;
    uint32_t                flags;
    uint32_t                capsule_image_size;
} efi_capsule_header_t;

typedef struct {
    uint32_t                version;
    uint16_t                embedded_driver_count;
    uint16_t                payload_item_count;
    uint64_t                item_offset_list[];
} efi_firmware_management_capsule_header_t;

typedef struct {
    uint32_t                version;
    struct efi_guid         update_image_type_id;
    uint8_t                 update_image_index;
    uint8_t                 reserved_bytes[3];
    uint32_t                update_image_size;
    uint32_t                update_vendorcode_size;
    uint64_t                update_hardware_instance; //introduced in v2
    uint64_t                image_capsule_support; //introduced in v3
} efi_firmware_management_capsule_image_header_t;

typedef struct {
    uint32_t                signature;
    uint32_t                header_size;
    uint32_t                fw_version;
    uint32_t                lowest_supported_version;
} fmp_payload_header_t;

#define ANYSIZE_ARRAY 0

typedef struct {
    uint32_t                dwLength;
    uint16_t                wRevision;
    uint16_t                wCertificateType;
    uint8_t                 bCertificate[ANYSIZE_ARRAY];
} WIN_CERTIFICATE;

typedef struct {
    WIN_CERTIFICATE         hdr;
    struct efi_guid         cert_type;
    uint8_t                 cert_data[ANYSIZE_ARRAY];
} win_certificate_uefi_guid_t;

typedef struct {
    uint64_t                    monotonic_count;
    win_certificate_uefi_guid_t   auth_info;
} efi_firmware_image_authentication_t;


enum uefi_capsule_error_t uefi_capsule_retrieve_images(void* capsule_ptr,
        capsule_image_info_t* images_info)
{
    char *ptr = (char*)capsule_ptr;
    efi_capsule_header_t* capsule_header;
    efi_firmware_management_capsule_header_t* fmp_capsule_header;
    efi_firmware_management_capsule_image_header_t* image_header;
    efi_firmware_image_authentication_t* image_auth;
    fmp_payload_header_t *fmp_payload_header;
    uint32_t total_size;
    uint32_t image_count;
    uint32_t auth_size;

    FWU_LOG_MSG("%s: enter, capsule ptr = 0x%p\n\r", __func__, capsule_ptr);

    if (!capsule_ptr) {
        return UEFI_CAPSULE_PARSER_ERROR;
    }

    capsule_header = (efi_capsule_header_t*)ptr;
    ptr += sizeof(efi_capsule_header_t) + sizeof(uint32_t);
    fmp_capsule_header = (efi_firmware_management_capsule_header_t*)ptr;

    fmp_payload_header = fmp_capsule_header + sizeof(*fmp_capsule_header);

    total_size = capsule_header->capsule_image_size;
    image_count = fmp_capsule_header->payload_item_count;
    images_info->nr_image = image_count;

    FWU_LOG_MSG("%s: capsule size = %u, image count = %u\n\r", __func__,
                        total_size, image_count);

    if ((image_count == 0) || (image_count > NR_OF_IMAGES_IN_FW_BANK)) {
        return UEFI_CAPSULE_PARSER_ERROR;
    }

    for (int i = 0; i < image_count; i++) {

        image_header = (efi_firmware_management_capsule_image_header_t*)(ptr +
                                fmp_capsule_header->item_offset_list[i]);

        images_info->size[i] = image_header->update_image_size;
        images_info->version[i] = fmp_payload_header->fw_version;
        FWU_LOG_MSG("%s: image %i version = %u\n\r", __func__, i,
                                images_info->version[i]);
#ifdef AUTHENTICATED_CAPSULE
        image_auth = (efi_firmware_image_authentication_t*)(
                        (char*)image_header +
                        sizeof (efi_firmware_management_capsule_image_header_t)
                     );
        auth_size = sizeof(uint64_t) /* monotonic_count */  +
                    image_auth->auth_info.hdr.dwLength /* WIN_CERTIFICATE + cert_data */ +
                    sizeof(struct efi_guid) /* cert_type */;

        FWU_LOG_MSG("%s: auth size = %u\n\r", __func__, auth_size);

        images_info->size[i] -= auth_size;

        images_info->image[i] = (
                (char*)image_header +
                sizeof(efi_firmware_management_capsule_image_header_t) +
                auth_size);
#else
        images_info->image[i] = (
                (char*)image_header +
                sizeof(efi_firmware_management_capsule_image_header_t) +
                sizeof(*fmp_payload_header));
#endif
        memcpy(&images_info->guid[i], &(image_header->update_image_type_id),
                                                        sizeof(struct efi_guid));

        FWU_LOG_MSG("%s: image %d at %p, size=%u\n\r", __func__, i,
                        images_info->image[i], images_info->size[i]);

        if ((fmp_capsule_header->item_offset_list[i] +
             sizeof(efi_firmware_management_capsule_image_header_t) +
             image_header->update_image_size) > total_size)
        {
            return UEFI_CAPSULE_PARSER_ERROR;
        }

    }

    FWU_LOG_MSG("%s: exit\n\r", __func__);
    return UEFI_CAPSULE_PARSER_SUCCESS;
}

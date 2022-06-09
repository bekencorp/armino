/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <string.h>
#include "fwu_agent.h"
#include "Driver_Flash.h"
#include "flash_layout.h"
#include "fip_parser/external/uuid.h"
#include "region_defs.h"
#include "uefi_capsule_parser.h"
#include "flash_common.h"
#include "platform_base_address.h"
#include "platform_description.h"
#include "tfm_plat_nv_counters.h"
#include "tfm_plat_defs.h"

/* Properties of image in a bank */
struct fwu_image_properties {

        /* UUID of the image in this bank */
        uuid_t img_uuid;

        /* [0]: bit describing the image acceptance status –
         *      1 means the image is accepted
         * [31:1]: MBZ
         */
        uint32_t accepted;

        /* NOTE: using the reserved field */
        /* image version */
        uint32_t version;

} __packed;

/* Image entry information */
struct fwu_image_entry {

        /* UUID identifying the image type */
        uuid_t img_type_uuid;

        /* UUID of the storage volume where the image is located */
        uuid_t location_uuid;

        /* Properties of images with img_type_uuid in the different FW banks */
        struct fwu_image_properties img_props[NR_OF_FW_BANKS];

} __packed;

struct fwu_metadata {

        /* Metadata CRC value */
        uint32_t crc_32;

        /* Metadata version */
        uint32_t version;

        /* Bank index with which device boots */
        uint32_t active_index;

        /* Previous bank index with which device booted successfully */
        uint32_t previous_active_index;

        /* Image entry information */
        struct fwu_image_entry img_entry[NR_OF_IMAGES_IN_FW_BANK];

} __packed;

/* This is Corstone1000 speific metadata for OTA.
 * Private metadata is written at next sector following
 * FWU METADATA location */
struct fwu_private_metadata {

       /* boot_index: the bank from which system is booted from */
       uint32_t boot_index;

       /* counter: tracking number of boot attempted so far */
       uint32_t boot_attempted;

       /* staged nv_counter: temprary location before written to the otp */
       uint32_t nv_counter[NR_OF_IMAGES_IN_FW_BANK];

} __packed;

#define MAX_BOOT_ATTEMPTS_PER_BANK 3

struct fwu_metadata _metadata;

int is_initialized = 0;

capsule_image_info_t capsule_info;

enum fwu_agent_state_t {
    FWU_AGENT_STATE_UNKNOWN = -1,
    FWU_AGENT_STATE_REGULAR = 0,
    FWU_AGENT_STATE_TRIAL,
};

struct efi_guid full_capsule_image_guid = {
    .time_low = 0xe2bb9c06,
    .time_mid = 0x70e9,
    .time_hi_and_version = 0x4b14,
    .clock_seq_and_node = {0x97, 0xa3, 0x5a, 0x79, 0x13, 0x17, 0x6e, 0x3f}
};


#define IMAGE_ACCEPTED          (1)
#define IMAGE_NOT_ACCEPTED      (0)
#define BANK_0                  (0)
#define BANK_1                  (1)
#define IMAGE_0                 (0)
#define IMAGE_1                 (1)
#define IMAGE_2                 (2)
#define IMAGE_3                 (3)
#define IMAGE_END               (IMAGE_3)
#define IMAGE_ALL               (IMAGE_END + 1)
#define IMAGE_NOT_RECOGNIZED    (-1)
#define INVALID_VERSION         (0xffffffff)


#ifndef FWU_METADATA_FLASH_DEV
    #ifndef FLASH_DEV_NAME
    #error "FWU_METADATA_FLASH_DEV or FLASH_DEV_NAME must be defined in flash_layout.h"
    #else
    #define FWU_METADATA_FLASH_DEV FLASH_DEV_NAME
    #endif
#endif

/* Import the CMSIS flash device driver */
extern ARM_DRIVER_FLASH FWU_METADATA_FLASH_DEV;

#define HOST_ACK_TIMEOUT_SEC    (6 * 60) /* ~seconds, not exact */

static enum fwu_agent_error_t private_metadata_read(
        struct fwu_private_metadata* p_metadata)
{
    int ret;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    if (!p_metadata) {
        return FWU_AGENT_ERROR;
    }

    ret = FWU_METADATA_FLASH_DEV.ReadData(FWU_PRIVATE_AREA_OFFSET, p_metadata,
                                          sizeof(struct fwu_private_metadata));
    if (ret < 0 || ret != sizeof(struct fwu_private_metadata)) {
        return FWU_AGENT_ERROR;
    }

    FWU_LOG_MSG("%s: success: boot_index = %u\n\r", __func__,
                        p_metadata->boot_index);

    return FWU_AGENT_SUCCESS;
}

static enum fwu_agent_error_t private_metadata_write(
        struct fwu_private_metadata* p_metadata)
{
    int ret;

    FWU_LOG_MSG("%s: enter: boot_index = %u\n\r", __func__,
                        p_metadata->boot_index);

    if (!p_metadata) {
        return FWU_AGENT_ERROR;
    }

    ret = FWU_METADATA_FLASH_DEV.EraseSector(FWU_PRIVATE_AREA_OFFSET);
    if (ret != ARM_DRIVER_OK) {
        return FWU_AGENT_ERROR;
    }

    ret = FWU_METADATA_FLASH_DEV.ProgramData(FWU_PRIVATE_AREA_OFFSET,
                                p_metadata, sizeof(struct fwu_private_metadata));
    if (ret < 0 || ret != sizeof(struct fwu_private_metadata)) {
        return FWU_AGENT_ERROR;
    }

    FWU_LOG_MSG("%s: success\n\r", __func__);
    return FWU_AGENT_SUCCESS;
}

static enum fwu_agent_error_t metadata_read(struct fwu_metadata *p_metadata)
{
    int ret;

    FWU_LOG_MSG("%s: enter: flash addr = %u, size = %d\n\r", __func__,
                  FWU_METADATA_REPLICA_1_OFFSET, sizeof(struct fwu_metadata));

    if (!p_metadata) {
        return FWU_AGENT_ERROR;
    }

    ret = FWU_METADATA_FLASH_DEV.ReadData(FWU_METADATA_REPLICA_1_OFFSET,
                                p_metadata, sizeof(struct fwu_metadata));
    if (ret < 0 || ret != sizeof(struct fwu_metadata)) {
        return FWU_AGENT_ERROR;
    }

    FWU_LOG_MSG("%s: success: active = %u, previous = %d\n\r", __func__,
                  p_metadata->active_index, p_metadata->previous_active_index);

    return FWU_AGENT_SUCCESS;
}

static enum fwu_agent_error_t metadata_write(
                        struct fwu_metadata *p_metadata)
{
    int ret;

    FWU_LOG_MSG("%s: enter: flash addr = %u, size = %d\n\r", __func__,
                  FWU_METADATA_REPLICA_1_OFFSET, sizeof(struct fwu_metadata));

    if (!p_metadata) {
        return FWU_AGENT_ERROR;
    }

    ret = FWU_METADATA_FLASH_DEV.EraseSector(FWU_METADATA_REPLICA_1_OFFSET);
    if (ret != ARM_DRIVER_OK) {
        return FWU_AGENT_ERROR;
    }

    ret = FWU_METADATA_FLASH_DEV.ProgramData(FWU_METADATA_REPLICA_1_OFFSET,
                                p_metadata, sizeof(struct fwu_metadata));
    if (ret < 0 || ret != sizeof(struct fwu_metadata)) {
        return FWU_AGENT_ERROR;
    }

    FWU_LOG_MSG("%s: success: active = %u, previous = %d\n\r", __func__,
                  p_metadata->active_index, p_metadata->previous_active_index);
    return FWU_AGENT_SUCCESS;
}

enum fwu_agent_error_t fwu_metadata_init(void)
{
    enum fwu_agent_error_t ret;
    ARM_FLASH_INFO* flash_info;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    if (is_initialized) {
        return FWU_AGENT_SUCCESS;
    }

    /* Code assumes everything fits into a sector */
    if (sizeof(struct fwu_metadata) > FWU_METADATA_FLASH_SECTOR_SIZE) {
        return FWU_AGENT_ERROR;
    }

    if (sizeof(struct fwu_private_metadata) > FWU_METADATA_FLASH_SECTOR_SIZE) {
        return FWU_AGENT_ERROR;
    }

    ret = FWU_METADATA_FLASH_DEV.Initialize(NULL);
    if (ret != ARM_DRIVER_OK) {
        return FWU_AGENT_ERROR;
    }

    flash_info = FWU_METADATA_FLASH_DEV.GetInfo();
    if (flash_info->program_unit != 1) {
        FWU_METADATA_FLASH_DEV.Uninitialize();
        return FWU_AGENT_ERROR;
    }

    is_initialized = 1;

    FWU_LOG_MSG("%s: is_initialized = %d\n\r", __func__, is_initialized);

    return FWU_AGENT_SUCCESS;
}

enum fwu_agent_error_t fwu_metadata_provision(void)
{
    enum fwu_agent_error_t ret;
    struct fwu_private_metadata priv_metadata;
    uint32_t image_version = 0;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    ret = fwu_metadata_init();
    if (ret) {
        return ret;
    }

    /*
     * check by chance if the previous reboot
     * had a firmware data?. If yes, then don't initialize
     * metadata
     */
    metadata_read(&_metadata);
    if(_metadata.active_index < 2 || _metadata.previous_active_index <2){
    	if(_metadata.active_index ^ _metadata.previous_active_index)
    		return FWU_AGENT_SUCCESS;
    }
    /* Provision FWU Agent Metadata */

    memset(&_metadata, 0, sizeof(struct fwu_metadata));

    _metadata.version = 1;
    _metadata.active_index = 0;
    _metadata.previous_active_index = 1;

    /* bank 0 is the place where images are located at the
     * start of device lifecycle */

    for (int i = 0; i < NR_OF_IMAGES_IN_FW_BANK; i++) {

        _metadata.img_entry[i].img_props[BANK_0].accepted = IMAGE_ACCEPTED;
        _metadata.img_entry[i].img_props[BANK_0].version = image_version;

        _metadata.img_entry[i].img_props[BANK_1].accepted = IMAGE_NOT_ACCEPTED;
        _metadata.img_entry[i].img_props[BANK_1].version = INVALID_VERSION;
    }

    ret = metadata_write(&_metadata);
    if (ret) {
        return ret;
    }

    memset(&_metadata, 0, sizeof(struct fwu_metadata));
    ret = metadata_read(&_metadata);
    if (ret) {
        return ret;
    }
    FWU_LOG_MSG("%s: provisioned values: active = %u, previous = %d\n\r",
             __func__, _metadata.active_index, _metadata.previous_active_index);


    /* Provision Private metadata for update agent which is shared
       beween BL1 and tf-m of secure enclave */

    memset(&priv_metadata, 0, sizeof(struct fwu_private_metadata));

    priv_metadata.boot_index = BANK_0;
    priv_metadata.boot_attempted = 0;

    ret = private_metadata_write(&priv_metadata);
    if (ret) {
        return ret;
    }

    memset(&priv_metadata, 0, sizeof(struct fwu_private_metadata));
    ret = private_metadata_read(&priv_metadata);
    if (ret) {
        return ret;
    }
    FWU_LOG_MSG("%s: provisioned values: boot_index = %u\n\r", __func__,
                        priv_metadata.boot_index);

    FWU_LOG_MSG("%s: FWU METADATA PROVISIONED.\n\r", __func__);
    return FWU_AGENT_SUCCESS;
}

static enum fwu_agent_state_t get_fwu_agent_state(
        struct fwu_metadata *metadata_ptr,
        struct fwu_private_metadata *priv_metadata_ptr)
{
    uint32_t boot_index;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    boot_index = priv_metadata_ptr->boot_index;

    if (boot_index != metadata_ptr->active_index) {
        return FWU_AGENT_STATE_TRIAL;
    }

    for (int i = 0; i < NR_OF_IMAGES_IN_FW_BANK; i++) {
        if ((metadata_ptr->img_entry[i].img_props[boot_index].accepted)
                == (IMAGE_NOT_ACCEPTED)) {
            return FWU_AGENT_STATE_TRIAL;
        }
    }

    FWU_LOG_MSG("%s: exit: FWU_AGENT_STATE_REGULAR\n\r", __func__);
    return FWU_AGENT_STATE_REGULAR;
}

static int get_image_info_in_bank(struct efi_guid* guid, uint32_t* image_bank_offset)
{
    if ((memcmp(guid, &full_capsule_image_guid, sizeof(struct efi_guid))) == 0) {
        *image_bank_offset = 0;
        return IMAGE_ALL;
    }

    return IMAGE_NOT_RECOGNIZED;
}

static enum fwu_agent_error_t erase_bank(uint32_t bank_offset)
{
    int ret;
    uint32_t sectors;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    if ((bank_offset % FWU_METADATA_FLASH_SECTOR_SIZE) != 0) {
        return FWU_AGENT_ERROR;
    }

    if ((BANK_PARTITION_SIZE % FWU_METADATA_FLASH_SECTOR_SIZE) != 0) {
        return FWU_AGENT_ERROR;
    }

    sectors = BANK_PARTITION_SIZE / FWU_METADATA_FLASH_SECTOR_SIZE;

    FWU_LOG_MSG("%s: erasing sectors = %u, from offset = %u\n\r", __func__,
                     sectors, bank_offset);

    for (int i = 0; i < sectors; i++) {
        ret = FWU_METADATA_FLASH_DEV.EraseSector(
                bank_offset + (i * FWU_METADATA_FLASH_SECTOR_SIZE));
        if (ret != ARM_DRIVER_OK) {
            return FWU_AGENT_ERROR;
        }
    }

    FWU_LOG_MSG("%s: exit\n\r", __func__);
    return FWU_AGENT_SUCCESS;
}


static enum fwu_agent_error_t flash_full_capsule(
        struct fwu_metadata* metadata, void* images, uint32_t size,
        uint32_t version)
{
    int ret;
    uint32_t active_index = metadata->active_index;
    uint32_t bank_offset;
    uint32_t previous_active_index;

    FWU_LOG_MSG("%s: enter: image = 0x%p, size = %u, version = %u\n\r"
                , __func__, images, size, version);

    if (!metadata || !images) {
        return FWU_AGENT_ERROR;
    }

    if (size > BANK_PARTITION_SIZE) {
        FWU_LOG_MSG("ERROR: %s: size error\n\r",__func__);
        return FWU_AGENT_ERROR;
    }

    if (version <=
            (metadata->img_entry[IMAGE_0].img_props[active_index].version)) {
        FWU_LOG_MSG("ERROR: %s: version error\n\r",__func__);
        return FWU_AGENT_ERROR;
    }

    if (active_index == BANK_0) {
        previous_active_index = BANK_1;
        bank_offset = BANK_1_PARTITION_OFFSET;
    } else if (active_index == BANK_1) {
        previous_active_index = BANK_0;
        bank_offset = BANK_0_PARTITION_OFFSET;
    } else {
        FWU_LOG_MSG("ERROR: %s: active_index %d\n\r",__func__,active_index);
        return FWU_AGENT_ERROR;
    }

    if (erase_bank(bank_offset)) {
        return FWU_AGENT_ERROR;
    }

    FWU_LOG_MSG("%s: writing capsule to the flash at offset = %u...\n\r",
                      __func__, bank_offset);
    ret = FWU_METADATA_FLASH_DEV.ProgramData(bank_offset, images, size);
    if (ret < 0 || ret != size) {
        return FWU_AGENT_ERROR;
    }
    FWU_LOG_MSG("%s: images are written to bank offset = %u\n\r", __func__,
                     bank_offset);

    /* Change system state to trial bank state */
    for (int i = 0; i < NR_OF_IMAGES_IN_FW_BANK; i++) {
        metadata->img_entry[i].img_props[previous_active_index].accepted =
                                                        IMAGE_NOT_ACCEPTED;
        metadata->img_entry[i].img_props[previous_active_index].version = version;
    }
    metadata->active_index = previous_active_index;
    metadata->previous_active_index = active_index;

    ret = metadata_write(metadata);
    if (ret) {
        return ret;
    }

    FWU_LOG_MSG("%s: exit\n\r", __func__);
    return FWU_AGENT_SUCCESS;
}

enum fwu_agent_error_t corstone1000_fwu_flash_image(void)
{
    enum fwu_agent_error_t ret;
    struct fwu_private_metadata priv_metadata;
    enum fwu_agent_state_t current_state;
    void *capsule_ptr = (char*)CORSTONE1000_HOST_DRAM_UEFI_CAPSULE;
    int image_index;
    uint32_t image_bank_offset;
    uint32_t nr_images;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    if (!is_initialized) {
        return FWU_AGENT_ERROR;
    }

    Select_Write_Mode_For_Shared_Flash();

    if (metadata_read(&_metadata)) {
        ret =  FWU_AGENT_ERROR;
        goto out;
    }

    if (private_metadata_read(&priv_metadata)) {
        ret =  FWU_AGENT_ERROR;
        goto out;
    }

    /* Firmware update process can only start in regular state. */
    current_state = get_fwu_agent_state(&_metadata, &priv_metadata);
    if (current_state != FWU_AGENT_STATE_REGULAR) {
        ret =  FWU_AGENT_ERROR;
        goto out;
    }

    memset(&capsule_info, 0, sizeof(capsule_image_info_t));
    if (uefi_capsule_retrieve_images(capsule_ptr, &capsule_info)) {
        ret =  FWU_AGENT_ERROR;
        goto out;
    }
    nr_images = capsule_info.nr_image;

    for (int i = 0; i < nr_images; i++) {
        image_index = get_image_info_in_bank(&capsule_info.guid[i],
                                &image_bank_offset);
        switch(image_index) {
            case IMAGE_ALL:
                ret = flash_full_capsule(&_metadata, capsule_info.image[i],
                                         capsule_info.size[i],
                                         capsule_info.version[i]);
                break;
            default:
                FWU_LOG_MSG("%s: sent image not recognized\n\r", __func__);
                ret = FWU_AGENT_ERROR;
                break;
        }
    }

out:
    Select_XIP_Mode_For_Shared_Flash();

    FWU_LOG_MSG("%s: exit: ret = %d\n\r", __func__, ret);
    return ret;
}

static enum fwu_agent_error_t accept_full_capsule(
          struct fwu_metadata* metadata,
          struct fwu_private_metadata* priv_metadata)
{
    uint32_t active_index = metadata->active_index;
    enum fwu_agent_error_t ret;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    for (int i = 0; i < NR_OF_IMAGES_IN_FW_BANK; i++) {
        metadata->img_entry[i].img_props[active_index].accepted =
                                                            IMAGE_ACCEPTED;
    }

    priv_metadata->boot_attempted = 0;

    ret = private_metadata_write(priv_metadata);
    if (ret) {
        return ret;
    }

    ret = metadata_write(metadata);
    if (ret) {
        return ret;
    }

    FWU_LOG_MSG("%s: exit: fwu state is changed to regular\n\r", __func__);
    return FWU_AGENT_SUCCESS;
}

static enum fwu_agent_error_t fwu_accept_image(struct efi_guid* guid,
        struct fwu_metadata *metadata,
        struct fwu_private_metadata *priv_metadata)
{
    enum fwu_agent_state_t current_state;
    int image_index;
    uint32_t image_bank_offset;
    enum fwu_agent_error_t ret;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    /* it is expected to receive this call only when
       in trial state */
    current_state = get_fwu_agent_state(metadata, priv_metadata);
    if (current_state != FWU_AGENT_STATE_TRIAL) {
        return FWU_AGENT_ERROR;
    }

    /* booted from previous_active_bank, not expected
     * to receive this call in this state, rather host should
     * call corstone1000_fwu_select_previous */
    if (metadata->active_index != priv_metadata->boot_index) {
        return FWU_AGENT_ERROR;
    }

    image_index = get_image_info_in_bank(guid, &image_bank_offset);
    switch(image_index) {
        case IMAGE_ALL:
            ret = accept_full_capsule(metadata, priv_metadata);
            break;
        default:
            FWU_LOG_MSG("%s: sent image not recognized\n\r", __func__);
            ret = FWU_AGENT_ERROR;
            break;
    }

    FWU_LOG_MSG("%s: exit: ret = %d\n\r", __func__, ret);
    return ret;
}

static enum fwu_agent_error_t fwu_select_previous(
        struct fwu_metadata *metadata,
        struct fwu_private_metadata *priv_metadata)
{
    enum fwu_agent_error_t ret;
    enum fwu_agent_state_t current_state;
    uint32_t index;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    /* it is expected to receive this call only when
       in trial state */
    current_state = get_fwu_agent_state(metadata, priv_metadata);
    if (current_state != FWU_AGENT_STATE_TRIAL) {
        return FWU_AGENT_ERROR;
    }

    /* not expected to receive this call in this state, system
     * did not boot from previous active index */
    if (metadata->previous_active_index != priv_metadata->boot_index) {
        return FWU_AGENT_ERROR;
    }

    FWU_LOG_MSG("%s: trial state: active index = %u, previous active = %u\n\r",
            __func__, metadata->active_index, metadata->previous_active_index);

    index = metadata->previous_active_index;
    for (int i = 0; i < NR_OF_IMAGES_IN_FW_BANK; i++) {
        if (metadata->img_entry[i].img_props[index].accepted != IMAGE_ACCEPTED)
        {
            FWU_ASSERT(0);
        }
    }

    index = metadata->active_index;
    metadata->active_index = metadata->previous_active_index;
    metadata->previous_active_index = index;

    priv_metadata->boot_attempted = 0;

    ret = private_metadata_write(priv_metadata);
    if (ret) {
        return ret;
    }

    ret = metadata_write(metadata);
    if (ret) {
        return ret;
    }

    FWU_LOG_MSG("%s: in regular state by choosing previous active bank\n\r",
                 __func__);

    FWU_LOG_MSG("%s: exit: ret = %d\n\r", __func__, ret);
    return ret;

}

void bl1_get_boot_bank(uint32_t *bank_offset)
{
    struct fwu_private_metadata priv_metadata;
    enum fwu_agent_state_t current_state;
    uint32_t boot_attempted;
    uint32_t boot_index;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    if (fwu_metadata_init()) {
        FWU_ASSERT(0);
    }

    if (private_metadata_read(&priv_metadata)) {
        FWU_ASSERT(0);
    }

    if (metadata_read(&_metadata)) {
        FWU_ASSERT(0);
    }

    current_state = get_fwu_agent_state(&_metadata, &priv_metadata);

    if (current_state == FWU_AGENT_STATE_REGULAR) {
        boot_index = _metadata.active_index;
        FWU_ASSERT(boot_index == priv_metadata.boot_index);
        boot_attempted = 0;
    } else if (current_state == FWU_AGENT_STATE_TRIAL) {
        boot_attempted = (++priv_metadata.boot_attempted);
        FWU_LOG_MSG("%s: attempting boot number = %u\n\r",
                                        __func__, boot_attempted);
        if (boot_attempted <= MAX_BOOT_ATTEMPTS_PER_BANK) {
            boot_index = _metadata.active_index;
            FWU_LOG_MSG("%s: booting from trial bank: %u\n\r",
                                        __func__, boot_index);
        } else if (boot_attempted <= (2 * MAX_BOOT_ATTEMPTS_PER_BANK)) {
            boot_index = _metadata.previous_active_index;
            FWU_LOG_MSG("%s: gave up booting from trial bank\n\r", __func__);
            FWU_LOG_MSG("%s: booting from previous active bank: %u\n\r",
                                        __func__, boot_index);
        } else {
            FWU_LOG_MSG("%s: cannot boot system from any bank, halting...\n\r", __func__);
            FWU_ASSERT(0);
        }
    } else {
        FWU_ASSERT(0);
    }

    priv_metadata.boot_index = boot_index;
    if (private_metadata_write(&priv_metadata) < 0) {
        FWU_ASSERT(0);
    }

    if (boot_index == BANK_0) {
        *bank_offset = BANK_0_PARTITION_OFFSET;
    } else if (boot_index == BANK_1) {
        *bank_offset = BANK_1_PARTITION_OFFSET;
    } else {
        FWU_ASSERT(0);
    }

    FWU_LOG_MSG("%s: exit: booting from bank = %u, offset = %x\n\r", __func__,
                        boot_index, *bank_offset);

    return;
}

void bl2_get_boot_bank(uint32_t *bank_offset)
{
    uint32_t boot_index;
    struct fwu_private_metadata priv_metadata;
    FWU_LOG_MSG("%s: enter\n\r", __func__);

    if (fwu_metadata_init()) {
        FWU_ASSERT(0);
    }

    if (private_metadata_read(&priv_metadata)) {
        FWU_ASSERT(0);
    }

    boot_index = priv_metadata.boot_index;

    if (boot_index == BANK_0) {
        *bank_offset = BANK_0_PARTITION_OFFSET;
    } else if (boot_index == BANK_1) {
        *bank_offset = BANK_1_PARTITION_OFFSET;
    } else {
        FWU_ASSERT(0);
    }

    FWU_LOG_MSG("%s: exit: booting from bank = %u, offset = %x\n\r", __func__,
                        boot_index, *bank_offset);

    return;
}

static void disable_host_ack_timer(void)
{
    FWU_LOG_MSG("%s: timer to reset is disabled\n\r", __func__);
    SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
}

static enum fwu_agent_error_t update_nv_counters(
                        struct fwu_private_metadata* priv_metadata)
{
    enum tfm_plat_err_t err;
    uint32_t security_cnt;
    enum tfm_nv_counter_t tfm_nv_counter_i;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    for (int i = 0; i <= FWU_MAX_NV_COUNTER_INDEX; i++) {

        switch (i) {
            case FWU_BL2_NV_COUNTER:
                tfm_nv_counter_i = PLAT_NV_COUNTER_BL1_0;
                break;
            case FWU_TFM_NV_COUNTER:
                tfm_nv_counter_i = PLAT_NV_COUNTER_BL2_0;
                break;
            case FWU_TFA_NV_COUNTER:
                tfm_nv_counter_i = PLAT_NV_COUNTER_BL2_1;
                break;
            default:
                FWU_ASSERT(0);
                break;
        }

        err = tfm_plat_read_nv_counter(tfm_nv_counter_i,
                        sizeof(security_cnt), (uint8_t *)&security_cnt);
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return FWU_AGENT_ERROR;
        }

        if (priv_metadata->nv_counter[i] < security_cnt) {
            return FWU_AGENT_ERROR;
        } else if (priv_metadata->nv_counter[i] > security_cnt) {
            FWU_LOG_MSG("%s: updaing index = %u nv counter = %u->%u\n\r",
                        __func__, i, security_cnt,
                        priv_metadata->nv_counter[FWU_BL2_NV_COUNTER]);
            err = tfm_plat_set_nv_counter(tfm_nv_counter_i,
                                    priv_metadata->nv_counter[FWU_BL2_NV_COUNTER]);
            if (err != TFM_PLAT_ERR_SUCCESS) {
                return FWU_AGENT_ERROR;
            }
        }

    }

    FWU_LOG_MSG("%s: exit\n\r", __func__);
    return FWU_AGENT_SUCCESS;
}

enum fwu_agent_error_t corstone1000_fwu_host_ack(void)
{
    enum fwu_agent_error_t ret;
    struct fwu_private_metadata priv_metadata;
    enum fwu_agent_state_t current_state;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    if (!is_initialized) {
        return FWU_AGENT_ERROR;
    }

    Select_Write_Mode_For_Shared_Flash();

    if (metadata_read(&_metadata)) {
        ret = FWU_AGENT_ERROR;
        goto out;
    }

    if (private_metadata_read(&priv_metadata)) {
        ret = FWU_AGENT_ERROR;
        goto out;
    }

    current_state = get_fwu_agent_state(&_metadata, &priv_metadata);
    if (current_state == FWU_AGENT_STATE_REGULAR) {
        ret = FWU_AGENT_SUCCESS; /* nothing to be done */
        goto out;
    } else if (current_state != FWU_AGENT_STATE_TRIAL) {
        FWU_ASSERT(0);
    }

    if (_metadata.active_index != priv_metadata.boot_index) {
        /* firmware update failed, revert back to previous bank */
        ret = fwu_select_previous(&_metadata, &priv_metadata);
    } else {
        /* firmware update successful */
        ret = fwu_accept_image(&full_capsule_image_guid, &_metadata,
                                &priv_metadata);
        if (!ret) {
            ret = update_nv_counters(&priv_metadata);
        }
    }

    if (ret == FWU_AGENT_SUCCESS) {
        disable_host_ack_timer();
    }

out:
    Select_XIP_Mode_For_Shared_Flash();

    FWU_LOG_MSG("%s: exit: ret = %d\n\r", __func__, ret);
    return ret;
}

static int systic_counter = 0;

void SysTick_Handler(void)
{
    systic_counter++;
    if ((systic_counter % 10) == 0) {
        FWU_LOG_MSG("%s: counted = %d, expiring on = %u\n\r", __func__,
                                systic_counter, HOST_ACK_TIMEOUT_SEC);
    }
    if (systic_counter == HOST_ACK_TIMEOUT_SEC) {
        FWU_LOG_MSG("%s, timer expired, reseting the system\n\r", __func__);
        NVIC_SystemReset();
    }
}

/* When in trial state, start the timer for host to respond.
 * Diable timer when host responds back either by calling
 * corstone1000_fwu_accept_image or corstone1000_fwu_select_previous.
 * Otherwise, resets the system.
 */
void host_acknowledgement_timer_to_reset(void)
{
    struct fwu_private_metadata priv_metadata;
    enum fwu_agent_state_t current_state;

    FWU_LOG_MSG("%s: enter\n\r", __func__);

    Select_Write_Mode_For_Shared_Flash();

    if (!is_initialized) {
        FWU_ASSERT(0);
    }

    if (private_metadata_read(&priv_metadata)) {
        FWU_ASSERT(0);
    }

    if (metadata_read(&_metadata)) {
        FWU_ASSERT(0);
    }

    Select_XIP_Mode_For_Shared_Flash();

    current_state = get_fwu_agent_state(&_metadata, &priv_metadata);

    if (current_state == FWU_AGENT_STATE_TRIAL) {
        FWU_LOG_MSG("%s: in trial state, starting host ack timer\n\r",
                        __func__);
        systic_counter = 0;
        if (SysTick_Config(SysTick_LOAD_RELOAD_Msk)) {
            FWU_LOG_MSG("%s: timer init failed\n\r", __func__);
            FWU_ASSERT(0);
        } else {
            FWU_LOG_MSG("%s: timer started: seconds to expire : %u\n\r",
                        __func__, HOST_ACK_TIMEOUT_SEC);
        }
    }

    FWU_LOG_MSG("%s: exit\n\r", __func__);
    return;
}

/* stage nv counter into private metadata section of the flash.
 * staged nv counters are written to the otp when firmware update
 * is successful
 * the function assumes that the api is called in the boot loading
 * stage
 */
enum fwu_agent_error_t fwu_stage_nv_counter(enum fwu_nv_counter_index_t index,
        uint32_t img_security_cnt)
{
    struct fwu_private_metadata priv_metadata;

    FWU_LOG_MSG("%s: enter: index = %u, val = %u\n\r", __func__,
                                index, img_security_cnt);

    if (!is_initialized) {
        FWU_ASSERT(0);
    }

    if (index > FWU_MAX_NV_COUNTER_INDEX) {
        return FWU_AGENT_ERROR;
    }

    if (private_metadata_read(&priv_metadata)) {
        FWU_ASSERT(0);
    }

    if (priv_metadata.nv_counter[index] != img_security_cnt) {
        priv_metadata.nv_counter[index] = img_security_cnt;
        if (private_metadata_write(&priv_metadata)) {
            FWU_ASSERT(0);
        }
    }

    FWU_LOG_MSG("%s: exit\n\r", __func__);
    return FWU_AGENT_SUCCESS;
}

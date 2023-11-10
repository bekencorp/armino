/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <string.h>
#include "psa/crypto.h"
#include "tfm_sp_log.h"
#include "bootutil_priv.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"
#include "flash_map_backend/flash_map_backend.h"
#include "sysflash/sysflash.h"
#include "tfm_api.h"
#include "tfm_bootloader_fwu_abstraction.h"
#include "tfm_boot_status.h"
#include "service_api.h"

#if (FWU_COMPONENT_NUMBER != MCUBOOT_IMAGE_NUMBER)
    #error "FWU_COMPONENT_NUMBER mismatch with MCUBOOT_IMAGE_NUMBER"
#endif

#if (MCUBOOT_IMAGE_NUMBER == 1)
#define MAX_IMAGE_INFO_LENGTH    (sizeof(struct image_version) + \
                                  SHARED_DATA_ENTRY_HEADER_SIZE)
#else
#define MAX_IMAGE_INFO_LENGTH    2 * (sizeof(struct image_version) + \
                                      SHARED_DATA_ENTRY_HEADER_SIZE)
#endif

/*
 * \struct fwu_image_info_data
 *
 * \brief Contains the received boot status information from bootloader
 *
 * \details This is a redefinition of \ref tfm_boot_data to allocate the
 *          appropriate, service dependent size of \ref boot_data.
 */
typedef struct fwu_image_info_data_s {
    struct shared_data_tlv_header header;
    uint8_t data[MAX_IMAGE_INFO_LENGTH];
} fwu_image_info_data_t;

typedef struct tfm_fwu_mcuboot_ctx_s {
    /* The flash area corresponding to component. */
    const struct flash_area *fap;

    /* The size of the downloaded data in the FWU process. */
    size_t loaded_size;
} tfm_fwu_mcuboot_ctx_t;

static tfm_fwu_mcuboot_ctx_t mcuboot_ctx[FWU_COMPONENT_NUMBER];
static fwu_image_info_data_t __attribute__((aligned(4))) boot_shared_data;

static int fwu_bootloader_get_shared_data(void)
{
    return tfm_core_get_boot_data(TLV_MAJOR_FWU,
                                  (struct tfm_boot_data *)&boot_shared_data,
                                  sizeof(boot_shared_data));
}

static psa_status_t get_active_image_version(psa_fwu_component_t component,
                                             struct image_version *image_ver)
{
    struct shared_data_tlv_entry tlv_entry;
    uint8_t *tlv_end;
    uint8_t *tlv_curr;

    /* The bootloader writes the image version information into the memory which
     * is shared between MCUboot and TF-M. Read the shared memory.
     */
    if (boot_shared_data.header.tlv_magic != SHARED_DATA_TLV_INFO_MAGIC) {
        return PSA_ERROR_DATA_CORRUPT;
    }

    tlv_end = (uint8_t *)&boot_shared_data +
                boot_shared_data.header.tlv_tot_len;
    tlv_curr = boot_shared_data.data;

    while (tlv_curr < tlv_end) {
        (void)memcpy(&tlv_entry, tlv_curr, SHARED_DATA_ENTRY_HEADER_SIZE);
        if ((GET_FWU_CLAIM(tlv_entry.tlv_type) == SW_VERSION) &&
            (GET_FWU_MODULE(tlv_entry.tlv_type) == component)) {
            if (tlv_entry.tlv_len != sizeof(struct image_version)) {
                return PSA_ERROR_DATA_CORRUPT;
            }
            memcpy(image_ver,
                tlv_curr + SHARED_DATA_ENTRY_HEADER_SIZE,
                tlv_entry.tlv_len);
            return PSA_SUCCESS;
        }
        tlv_curr += SHARED_DATA_ENTRY_HEADER_SIZE + tlv_entry.tlv_len;
    }

    return PSA_ERROR_DATA_CORRUPT;
}

psa_status_t fwu_bootloader_init(void)
{
    if (fwu_bootloader_get_shared_data() != TFM_SUCCESS) {
        return PSA_ERROR_STORAGE_FAILURE;
    }
    /* add Init of specific flash driver */
    flash_area_driver_init();
    return PSA_SUCCESS;
}

psa_status_t fwu_bootloader_staging_area_init(psa_fwu_component_t component,
                                              const void *manifest,
                                              size_t manifest_size)
{
    const struct flash_area *fap;

    /* MCUboot uses bundled manifest. */
    if ((manifest_size != 0) || (component >= FWU_COMPONENT_NUMBER)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (flash_area_open(FLASH_AREA_IMAGE_SECONDARY(component),
                        &fap) != 0) {
        LOG_ERRFMT("TFM FWU: opening flash failed.\r\n");
        return PSA_ERROR_STORAGE_FAILURE;
    }

    if (flash_area_erase(fap, 0, fap->fa_size) != 0) {
        LOG_ERRFMT("TFM FWU: erasing flash failed.\r\n");
        return PSA_ERROR_GENERIC_ERROR;
    }

    mcuboot_ctx[component].fap = fap;

    /* Reset the loaded_size. */
    mcuboot_ctx[component].loaded_size = 0;

    return PSA_SUCCESS;
}

psa_status_t fwu_bootloader_load_image(psa_fwu_component_t component,
                                       size_t block_offset,
                                       const void *block,
                                       size_t block_size)
{
    const struct flash_area *fap;

    if (block == NULL || component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* The component should already be added into the mcuboot_ctx. */
    if (mcuboot_ctx[component].fap != NULL) {
        fap = mcuboot_ctx[component].fap;
    } else {
        return PSA_ERROR_BAD_STATE;
    }

    if (flash_area_write(fap, block_offset, block, block_size) != 0) {
        LOG_ERRFMT("TFM FWU: write flash failed.\r\n");
        return PSA_ERROR_STORAGE_FAILURE;
    }

    /* The overflow check has been done in flash_area_write. */
    mcuboot_ctx[component].loaded_size += block_size;
    return PSA_SUCCESS;
}

#if (MCUBOOT_IMAGE_NUMBER > 1)
/**
 * \brief Compare image version numbers not including the build number.
 *
 * \param[in] image_ver_1 The first image version to compare.
 *
 * \param[in] image_ver_2 The second image version to compare.
 *
 * \return true         image_ver_1 is greater or equal than image_ver_2.
 * \return false        image_ver_1 is less than image_ver_2.
 */
static bool is_version_greater_or_equal(const struct image_version *image_ver_1,
                                        const struct image_version *image_ver_2)
{
    if (image_ver_1->iv_major > image_ver_2->iv_major) {
        return true;
    }
    if (image_ver_1->iv_major < image_ver_2->iv_major) {
        return false;
    }
    /* The major version numbers are equal, continue comparison. */
    if (image_ver_1->iv_minor > image_ver_2->iv_minor) {
        return true;
    }
    if (image_ver_1->iv_minor < image_ver_2->iv_minor) {
        return false;
    }
    /* The minor version numbers are equal, continue comparison. */
    if (image_ver_1->iv_revision >= image_ver_2->iv_revision) {
        return true;
    }
    return false;
}
#endif

psa_status_t fwu_bootloader_install_image(const psa_fwu_component_t *candidates, uint8_t number)
{
    uint8_t index_i, cand_index;
#if (MCUBOOT_IMAGE_NUMBER > 1)
    psa_fwu_component_t component;
    const struct flash_area *fap;
    struct image_tlv_iter it;
    struct image_header hdr;
    int rc;
    uint32_t off;
    uint16_t len;
    struct image_dependency dep;
    struct image_version image_ver = { 0 };
    const struct flash_area *fap_secondary;
    struct image_header hdr_secondary;
    bool check_pass = true;
#endif

    if (candidates == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

#if (MCUBOOT_IMAGE_NUMBER > 1)
    for (cand_index = 0; cand_index < number; cand_index++) {
        component = candidates[cand_index];
        /* The image should already be added into the mcuboot_ctx. */
        if ((component >= FWU_COMPONENT_NUMBER) ||
           (mcuboot_ctx[component].fap == NULL)) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
        fap = mcuboot_ctx[component].fap;
        /* Read the image header. */
        if (flash_area_read(fap, 0, &hdr, sizeof(hdr)) != 0) {
            return PSA_ERROR_STORAGE_FAILURE;
        }

        /* Return PSA_ERROR_DATA_CORRUPT if the image header is invalid. */
        if (hdr.ih_magic != IMAGE_MAGIC) {
            return PSA_ERROR_DATA_CORRUPT;
        }

        /* Initialize the iterator. */
        if (bootutil_tlv_iter_begin(&it, &hdr, fap, IMAGE_TLV_DEPENDENCY, true)) {
            return PSA_ERROR_STORAGE_FAILURE;
        }
        /* Check dependencies. */
        while (true) {
            rc = bootutil_tlv_iter_next(&it, &off, &len, NULL);
            if (rc < 0) {
                return PSA_ERROR_STORAGE_FAILURE;
            } else if (rc > 0) {
                /* No more dependency found. */
                rc = 0;
                break;
            }

            /* A dependency requirement is found. Set check_pass to false. */
            check_pass = false;
            if (flash_area_read(fap, off, &dep, len) != 0) {
                return PSA_ERROR_STORAGE_FAILURE;
            }

            if (dep.image_id > MCUBOOT_IMAGE_NUMBER) {
                return PSA_ERROR_DATA_CORRUPT;
            }

            /* As this partition does not validate the image in the secondary slot,
             * so it has no information of which image will be chosen to run after
             * reboot. So if the dependency image in the primary slot or that in the
             * secondary slot can meet the dependency requirement, then the
             * dependency check pass.
             */
            /* Check the dependency image in the primary slot. */
            if (get_active_image_version(dep.image_id,
                                        &image_ver) != PSA_SUCCESS) {
                return PSA_ERROR_STORAGE_FAILURE;
            }

            /* Check whether the version of the running image can meet the
             * dependency requirement.
             */
            if (is_version_greater_or_equal(&image_ver,
                                            &dep.image_min_version)) {
                check_pass = true;
            } else {
                /* Check whether the CANDIDATE image can meet this image's dependency
                 * requirement.
                 */
                for (index_i = 0; index_i < number; index_i++) {
                    if (candidates[index_i] == dep.image_id)
                        break;
                }
                if ((index_i < number) && (mcuboot_ctx[dep.image_id].fap != NULL)) {
                    /* The running image cannot meet the dependency requirement. Check
                     * the dependency image in the secondary slot.
                     */
                    fap_secondary = mcuboot_ctx[dep.image_id].fap;
                    if (flash_area_read(fap_secondary,
                                        0,
                                        &hdr_secondary,
                                        sizeof(hdr_secondary)) != 0) {
                        return PSA_ERROR_STORAGE_FAILURE;
                    }

                    /* Check the version of the dependency image in the secondary slot
                     * only if the image header is good.
                     */
                    if (hdr_secondary.ih_magic == IMAGE_MAGIC &&
                        (is_version_greater_or_equal(&hdr_secondary.ih_ver,
                                                     &dep.image_min_version))) {
                        /* The dependency image in the secondary slot meet the
                         * dependency requirement.
                         */
                        check_pass = true;
                    }
                }
            }

            /* Return directly if dependency check fails. */
            if (!check_pass) {
                return PSA_ERROR_DEPENDENCY_NEEDED;
            }
        }
    }
#endif

    /* Write the boot magic in image trailer so that these images will be
     * taken as candidates.
     */
    for (cand_index = 0; cand_index < number; cand_index++) {
        if (boot_set_pending_multi(candidates[cand_index], false) != 0) {
            /* If failure happens, reject candidates have been installed successfully. */
            for (index_i = 0; index_i < cand_index; index_i++) {
                if (fwu_bootloader_reject_staged_image(candidates[index_i]) != PSA_SUCCESS) {
                    break;
                }
            }
            return PSA_ERROR_STORAGE_FAILURE;
        }
    }
    return PSA_SUCCESS_REBOOT;
}

static inline uint32_t
boot_magic_off(const struct flash_area *fap)
{
    return flash_area_get_size(fap) - BOOT_MAGIC_SZ;
}

#if (defined(MCUBOOT_DIRECT_XIP) && defined(MCUBOOT_DIRECT_XIP_REVERT)) || \
    defined(MCUBOOT_SWAP_USING_SCRATCH) || defined(MCUBOOT_SWAP_USING_MOVE)
static inline uint32_t
boot_image_ok_off(const struct flash_area *fap)
{
    return ALIGN_DOWN(boot_magic_off(fap) - BOOT_MAX_ALIGN, BOOT_MAX_ALIGN);
}

static psa_status_t erase_image_ok(const struct flash_area *fap)
{
    uint32_t off;
    uint8_t buf[BOOT_MAX_ALIGN];
    uint8_t erased_val;
    uint32_t align;

    /* off of image OK flag is already BOOT_MAX_ALIGN aligned. */
    off = boot_image_ok_off(fap);

    /* Clear the image ok trailer. */
    align = flash_area_align(fap);
    align = ALIGN_UP(BOOT_MAX_ALIGN, align);
    if (align > BOOT_MAX_ALIGN) {
        return PSA_ERROR_STORAGE_FAILURE;
    }
    erased_val = flash_area_erased_val(fap);
    memset(buf, erased_val, align);
    if (flash_area_write(fap, off, buf, align) != 0) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}
#endif

psa_status_t fwu_bootloader_mark_image_accepted(const psa_fwu_component_t *trials,
                                                uint8_t number)
{
    /* As RAM_LOAD and OVERWRITE_ONLY do not support image revert, the image
     * does not need to be confirmed explicitly in these two upgrade strategies.
     * Image revert is supported in SWAP upgrade strategy and DIRECT_XIP upgrade
     * strategy when MCUBOOT_DIRECT_XIP_REVERT is true. In these cases, the
     * image needs to be set as a permanent image explicitly. Then the accpeted
     * image can still be selected as the running image during next time reboot
     * up. Otherwise, the image will be reverted and the previous one will be
     * chosen as the running image.
     */
#if (defined(MCUBOOT_DIRECT_XIP) && defined(MCUBOOT_DIRECT_XIP_REVERT)) || \
    defined(MCUBOOT_SWAP_USING_SCRATCH) || defined(MCUBOOT_SWAP_USING_MOVE)
    uint8_t trial_index, i;
    psa_fwu_component_t component;
    const struct flash_area *fap;

    if (trials == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    for (trial_index = 0; trial_index < number; trial_index++) {
        component = trials[trial_index];
        if (component >= FWU_COMPONENT_NUMBER) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
        if (flash_area_open(FLASH_AREA_IMAGE_SECONDARY(component),
                            &fap) != 0) {
            return PSA_ERROR_STORAGE_FAILURE;
        }
        if (fap == NULL) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }
        mcuboot_ctx[component].fap = fap;
    }
    for (trial_index = 0; trial_index < number; trial_index++) {
        component = trials[trial_index];
        if (boot_set_confirmed_multi(component) != 0) {
            for (i = 0; i < trial_index; i++) {
                if (erase_image_ok(mcuboot_ctx[component].fap) != 0) {
                    break;
                }
            }
            return PSA_ERROR_STORAGE_FAILURE;
        }
    }
#else
    (void)trials;
    (void)number;
#endif
    return PSA_SUCCESS;
}

static psa_status_t erase_boot_magic(const struct flash_area *fap)
{
    uint32_t off, pad_off;
    uint8_t magic[BOOT_MAGIC_ALIGN_SIZE];
    uint8_t erased_val;

    off = boot_magic_off(fap);
    pad_off = ALIGN_DOWN(off, BOOT_MAX_ALIGN);
    erased_val = flash_area_erased_val(fap);
    memset(&magic[0], erased_val, sizeof(magic));

    /* Clear the boot magic trailer. */
    if (flash_area_write(fap, pad_off, &magic[0], BOOT_MAGIC_ALIGN_SIZE)) {
        return PSA_ERROR_STORAGE_FAILURE;
    }
    return PSA_SUCCESS;
}

/* Reject the staged image. */
psa_status_t fwu_bootloader_reject_staged_image(psa_fwu_component_t component)
{
    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* The image should already be added into the mcuboot_ctx. */
    if (mcuboot_ctx[component].fap != NULL) {
        return erase_boot_magic(mcuboot_ctx[component].fap);
    }

    /* The component is not in FWU process. */
    return PSA_ERROR_DOES_NOT_EXIST;
}

/* Reject the running image in trial state. */
psa_status_t fwu_bootloader_reject_trial_image(psa_fwu_component_t component)
{
    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* The image will be reverted if it is not accepted explicitly. */
    return PSA_SUCCESS_REBOOT;
}

psa_status_t fwu_bootloader_abort(psa_fwu_component_t component)
{
    const struct flash_area *fap;

    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* The image should already be added into the mcuboot_ctx. */
    if (mcuboot_ctx[component].fap != NULL) {
        fap = mcuboot_ctx[component].fap;
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    flash_area_erase(fap, 0, fap->fa_size);
    flash_area_close(fap);
    mcuboot_ctx[component].fap = NULL;
    mcuboot_ctx[component].loaded_size = 0;
    return PSA_SUCCESS;
}


static psa_status_t util_img_hash(const struct flash_area *fap,
                                 size_t data_size,
                                 uint8_t *hash_result,
                                 size_t buf_size,
                                 size_t *hash_size)
{
    psa_hash_operation_t handle = psa_hash_operation_init();
    psa_status_t status;
    uint8_t tmpbuf[BOOT_TMPBUF_SZ];
    uint32_t tmp_buf_sz = BOOT_TMPBUF_SZ;
    uint32_t blk_sz;
    uint32_t off;

    /* Setup the hash object for the desired hash. */
    status = psa_hash_setup(&handle, PSA_ALG_SHA_256);
    if (status != PSA_SUCCESS) {
        return status;
    }

    for (off = 0; off < data_size; off += blk_sz) {
        blk_sz = data_size - off;
        if (blk_sz > tmp_buf_sz) {
            blk_sz = tmp_buf_sz;
        }

        if (flash_area_read(fap, off, tmpbuf, blk_sz)) {
            return PSA_ERROR_STORAGE_FAILURE;
        }
        status = psa_hash_update(&handle, tmpbuf, blk_sz);
        if (status != PSA_SUCCESS) {
            return status;
        }
    }

    status = psa_hash_finish(&handle, hash_result, buf_size, hash_size);

    return status;
}

static psa_status_t get_second_image_digest(psa_fwu_component_t component,
                                            psa_fwu_component_info_t *info)
{
    const struct flash_area *fap = NULL;
    uint8_t hash[TFM_FWU_MAX_DIGEST_SIZE] = {0};
    size_t hash_size = 0;
    psa_status_t ret = PSA_SUCCESS;
    size_t data_size;

    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check if the image is in a FWU process. */
    if (mcuboot_ctx[component].fap != NULL) {
        /* Calculate hash on the downloaded data. */
        data_size = mcuboot_ctx[component].loaded_size;
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    if ((flash_area_open(FLASH_AREA_IMAGE_SECONDARY(component),
                            &fap)) != 0) {
        LOG_ERRFMT("TFM FWU: opening flash failed.\r\n");
        return PSA_ERROR_STORAGE_FAILURE;
    }

    if (util_img_hash(fap, data_size, hash, (size_t)TFM_FWU_MAX_DIGEST_SIZE,
                      &hash_size) == PSA_SUCCESS) {
        memcpy(info->impl.candidate_digest, hash, hash_size);
    } else {
        ret = PSA_ERROR_STORAGE_FAILURE;
    }

    flash_area_close(fap);
    return ret;
}

psa_status_t fwu_bootloader_get_image_info(psa_fwu_component_t component,
                                           bool query_state,
                                           bool query_impl_info,
                                           psa_fwu_component_info_t *info)
{
#if !defined(MCUBOOT_DIRECT_XIP) && !defined(MCUBOOT_RAM_LOAD) && \
    !defined(MCUBOOT_OVERWRITE_ONLY)
    uint8_t image_ok = BOOT_FLAG_UNSET;
#endif
    const struct flash_area *fap = NULL;
    struct image_version image_version;
    psa_status_t ret = PSA_SUCCESS;

    if (info == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if ((flash_area_open(FLASH_AREA_IMAGE_PRIMARY(component),
                        &fap)) != 0) {
        LOG_ERRFMT("TFM FWU: opening flash failed.\r\n");
        return PSA_ERROR_STORAGE_FAILURE;
    }
    info->max_size = fap->fa_size;
    info->location = fap->fa_id;
    info->flags = PSA_FWU_FLAG_VOLATILE_STAGING;

    if (query_state) {
        /* As DIRECT_XIP, RAM_LOAD and OVERWRITE_ONLY do not support image revert.
         * So the running image is in INSTALLED state in these three upgrade
         * strategies. In the SWAP case, the image_ok flag should be read to check
         * whether the running image has been confirmed as a pernament image.
         */
    #if !defined(MCUBOOT_DIRECT_XIP) && !defined(MCUBOOT_RAM_LOAD) && \
        !defined(MCUBOOT_OVERWRITE_ONLY)
        /* Get value of image-ok flag of the image to check whether application
         * itself is already confirmed.
         */
        if (boot_read_image_ok(fap, &image_ok) != 0) {
            ret = PSA_ERROR_STORAGE_FAILURE;
            goto close_return;
        }
        if (image_ok == BOOT_FLAG_SET) {
            info->state = PSA_FWU_READY;
        } else {
            info->state = PSA_FWU_TRIAL;
        }
    #else
        info->state = PSA_FWU_READY;
    #endif
    }
    if (get_active_image_version(component,
                                    &image_version) == PSA_SUCCESS) {
        info->version.major = image_version.iv_major;
        info->version.minor = image_version.iv_minor;
        info->version.patch = image_version.iv_revision;
        info->version.build = image_version.iv_build_num;
    } else {
        ret = PSA_ERROR_STORAGE_FAILURE;
        goto close_return;
    }
    if (query_impl_info) {
        ret = get_second_image_digest(component, info);
    }

close_return:
    flash_area_close(fap);
    return ret;
}

psa_status_t fwu_bootloader_clean_component(psa_fwu_component_t component)
{
    const struct flash_area *fap = NULL;

    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check if the image is in a FWU process. */
    if (mcuboot_ctx[component].fap != NULL) {
        fap = mcuboot_ctx[component].fap;
        if (flash_area_erase(fap, 0, fap->fa_size) != 0) {
            return PSA_ERROR_STORAGE_FAILURE;
        }
        mcuboot_ctx[component].fap = NULL;
    } else {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    return PSA_SUCCESS;
}

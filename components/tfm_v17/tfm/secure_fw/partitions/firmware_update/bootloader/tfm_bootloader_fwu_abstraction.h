/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_BOOTLOADER_FWU_ABSTRACTION_H__
#define __TFM_BOOTLOADER_FWU_ABSTRACTION_H__

#include "stdbool.h"
#include "psa/update.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Bootloader related initialization for firmware update, such as reading
 * some necessary shared data from the memory if needed.
 *
 * \return PSA_SUCCESS              On success
 *         PSA_ERROR_GENERIC_ERROR  On failure
 */
psa_status_t fwu_bootloader_init(void);

/**
 * \brief Initialize the staging area of the component.
 *
 * The component is in READY state. Prepare the staging area of the component
 * for image download.
 * For example, initialize the staging area, open the flash area, and so on.
 * The image will be written into the staging area later.
 *
 * \param[in] component The identifier of the target component in bootloader.
 * \param[in] manifest  A pointer to a buffer containing a detached manifest for
 *                      the update. If the manifest is bundled with the firmware
 *                      image, manifest must be NULL.
 * \param[in] manifest_size  Size of the manifest buffer in bytes.
 *
 * \return PSA_SUCCESS                  On success
 *         PSA_ERROR_INVALID_SIGNATURE  A signature or integrity check on the
 *                                      manifest has failed.
 *         PSA_ERROR_INVALID_ARGUMENT   Invalid input parameter
 *         PSA_ERROR_INSUFFICIENT_MEMORY
 *         PSA_ERROR_INSUFFICIENT_STORAGE
 *         PSA_ERROR_COMMUNICATION_FAILURE
 *         PSA_ERROR_STORAGE_FAILURE
 *
 */
psa_status_t fwu_bootloader_staging_area_init(psa_fwu_component_t component,
                                              const void *manifest,
                                              size_t manifest_size);

/**
 * \brief Load the image into the target component.
 *
 * The component is in WRITING state. Write the image data into the target
 * component.
 *
 * \param[in] component The identifier of the target component in bootloader.
 * \param[in] image_offset  The offset of the image being passed into block, in
 *                          bytes
 * \param[in] block         A buffer containing a block of image data. This
 *                          might be a complete image or a subset.
 * \param[in] block_size    Size of block.
 *
 * \return PSA_SUCCESS                     On success
 *         PSA_ERROR_INVALID_ARGUMENT      Invalid input parameter
 *         PSA_ERROR_INSUFFICIENT_MEMORY   There is no enough memory to
 *                                         process the update
 *         PSA_ERROR_INSUFFICIENT_STORAGE  There is no enough storage to
 *                                         process the update
 *         PSA_ERROR_GENERIC_ERROR         A fatal error occurred
 *
 */
psa_status_t fwu_bootloader_load_image(psa_fwu_component_t component,
                                       size_t image_offset,
                                       const void *block,
                                       size_t block_size);

/**
 * \brief Starts the installation of an image.
 *
 * The components are in CANDIDATE state. Check the authenticity and integrity of
 * the staged image in the components. If a reboot is required to complete the
 * check, then mark this image as a candidate so that the next time bootloader
 * runs it will take this image as a candidate one to bootup. Return the error
 * code PSA_SUCCESS_REBOOT.
 *
 * \param[in] candidates A list of components in CANDIDATE state.
 * \param[in] number Number of components in CANDIDATE state.
 *
 * \return PSA_SUCCESS         On success
 *         PSA_SUCCESS_REBOOT  A system reboot is needed to finish installation
 *         TFM_SUCCESS_RESTART A restart of the updated component is required
 *                             to complete the update
 *         PSA_ERROR_DEPENDENCY_NEEDED Another image needs to be installed to
 *                                       finish installation
 *         PSA_ERROR_INVALID_ARGUMENT    Invalid input parameter
 *         PSA_ERROR_INVALID_SIGNATURE   The signature is incorrect
 *         PSA_ERROR_GENERIC_ERROR       A fatal error occurred
 *         TFM_ERROR_ROLLBACK_DETECTED   The image is too old to be installed.
 *                                       If the image metadata contains a
 *                                       timestamp and it has expired, then
 *                                       this error is also returned.
 */
psa_status_t fwu_bootloader_install_image(const psa_fwu_component_t *candidates,
                                          uint8_t number);

/**
 * \brief Mark the TRIAL(running) image in component as confirmed.
 *
 * Call this API to mark the running images as permanent/accepted to avoid
 * revert when next time bootup. Usually, this API is called after the running
 * images have been verified as valid.
 *
 * \param[in] candidates A list of components in TRIAL state.
 * \param[in] number Number of components in TRIAL state.
 *
 * \return PSA_SUCCESS         On success
 *         PSA_ERROR_INSUFFICIENT_MEMORY
 *         PSA_ERROR_INSUFFICIENT_STORAGE
 *         PSA_ERROR_COMMUNICATION_FAILURE
 *         PSA_ERROR_STORAGE_FAILURE
 */
psa_status_t fwu_bootloader_mark_image_accepted(const psa_fwu_component_t *trials,
                                                uint8_t number);

/**
 * \brief Uninstall the staged image in the component.
 *
 * The component is in STAGED state. Uninstall the staged image in the component
 * so that this image will not be treated as a candidate next time bootup.
 *
 * \return PSA_SUCCESS         On success
 *         PSA_ERROR_INSUFFICIENT_MEMORY
 *         PSA_ERROR_INSUFFICIENT_STORAGE
 *         PSA_ERROR_COMMUNICATION_FAILURE
 *         PSA_ERROR_STORAGE_FAILURE
 */
psa_status_t fwu_bootloader_reject_staged_image(psa_fwu_component_t component);

/**
 * \brief Reject the trial image in the component.
 *
 * The component is in TRIAL state. Mark the running image in the component as
 * rejected.
 *
 * \return PSA_SUCCESS         On success
 *         PSA_ERROR_INSUFFICIENT_MEMORY
 *         PSA_ERROR_INSUFFICIENT_STORAGE
 *         PSA_ERROR_COMMUNICATION_FAILURE
 *         PSA_ERROR_STORAGE_FAILURE
 */
psa_status_t fwu_bootloader_reject_trial_image(psa_fwu_component_t component);

/**
 * \brief The component is in FAILED or UPDATED state. Clean the staging area of the component.
 *
 * \return PSA_SUCCESS         On success
 *         PSA_ERROR_INVALID_ARGUMENT    Invalid input parameter
 *         PSA_ERROR_STORAGE_FAILURE
 */
psa_status_t fwu_bootloader_clean_component(psa_fwu_component_t component);

/**
 * \brief Get the image information.
 *
 * Get the image information of the given component in staging area
 * or the running area.
 *
 * \param[in] component  The identifier of the target component in bootloader.
 * \param[in] query_state  Query 'state' field.
 * \param[in] query_impl_info  Query 'impl' field.
 * \param[out] info      Buffer containing return the component information.

 * \return PSA_SUCCESS         On success
 *         PSA_ERROR_INVALID_ARGUMENT    Invalid input parameter
 *         PSA_ERROR_GENERIC_ERROR       A fatal error occurred
 */
psa_status_t fwu_bootloader_get_image_info(psa_fwu_component_t component,
                                           bool query_state,
                                           bool query_impl_info,
                                           psa_fwu_component_info_t *info);
#ifdef __cplusplus
}
#endif
#endif /* __TFM_BOOTLOADER_FWU_ABSTRACTION_H__ */

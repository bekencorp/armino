/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* PSA Firmware Update API v1.0 Beta */

#ifndef PSA_UPDATE_H
#define PSA_UPDATE_H

#include <stddef.h>
#include <stdint.h>

#include "psa/error.h"
#ifdef FWU_DEVICE_CONFIG_FILE
#include FWU_DEVICE_CONFIG_FILE
#else
#include "psa/fwu_config.h"
#endif
#include "tfm_fwu_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The major version of this implementation of the Firmware Update API.
 */
#define PSA_FWU_API_VERSION_MAJOR 1

/**
 * @brief The minor version of this implementation of the Firmware Update API.
 */
#define PSA_FWU_API_VERSION_MINOR 0

/**
 * @brief A status code that indicates that the firmware of another component
 *        requires updating.
 */
#define PSA_ERROR_DEPENDENCY_NEEDED ((psa_status_t)-156)

/**
 * @brief A status code that indicates that the system is limiting i/o
 *        operations to avoid rapid flash exhaustion.
 */
#define PSA_ERROR_FLASH_ABUSE ((psa_status_t)-160)

/**
 * @brief A status code that indicates that the system does not have enough
 *        power to carry out the request.
 */
#define PSA_ERROR_INSUFFICIENT_POWER ((psa_status_t)-161)

/**
 * @brief The action was completed successfully and requires a system reboot to
 *        complete installation.
 */
#define PSA_SUCCESS_REBOOT ((psa_status_t)+1)

/**
 * @brief The action was completed successfully and requires a restart of the
 *        component to complete installation.
 */
#define PSA_SUCCESS_RESTART ((psa_status_t)+2)

/**
 * @brief Firmware component type identifier.
 */
typedef uint8_t psa_fwu_component_t;

/**
 * @brief Version information about a firmware image.
 */
typedef struct psa_fwu_image_version_t {
    /* The major version of an image. */
    uint8_t major;
    /* The minor version of an image. */
    uint8_t minor;
    /* The revision or patch version of an image. */
    uint16_t patch;
    /* The build number of an image. */
    uint32_t build;
} psa_fwu_image_version_t;

/**
 * @brief The READY state: the component is ready to start another update.
 */
#define PSA_FWU_READY 0u

/**
 * @brief The WRITING state: a new firmware image is being written to the
 *        firmware store.
 */
#define PSA_FWU_WRITING 1u

/**
 * @brief The CANDIDATE state: a new firmware image is ready for installation.
 */
#define PSA_FWU_CANDIDATE 2u

/**
 * @brief The STAGED state: a new firmware image is queued for installation.
 */
#define PSA_FWU_STAGED 3u

/**
 * @brief The FAILED state: a firmware update has been cancelled or has failed.
 */
#define PSA_FWU_FAILED 4u

/**
 * @brief The TRIAL state: a new firmware image requires testing prior to
 *        acceptance of the update.
 */
#define PSA_FWU_TRIAL 5u

/**
 * @brief The REJECTED state: a new firmware image has been rejected after
 *        testing.
 */
#define PSA_FWU_REJECTED 6u

/**
 * @brief The UPDATED state: a firmware update has been successful, and the new
 *        image is now active.
 */
#define PSA_FWU_UPDATED 7u

/**
 * @brief Flag to indicate whether the image data in the component staging area
 *        is discarded at system reset.
 */
#define PSA_FWU_FLAG_VOLATILE_STAGING 0x00000001u

/**
 * @brief Flag to indicate whether a firmware component expects encrypted images
 *        during an update.
 */
#define PSA_FWU_FLAG_ENCRYPTION 0x00000002u

/**
 * @brief The implementation-specific data in the component information
 *        structure.
 */
typedef struct {
    /* The digest of second image when store state is CANDIDATE. */
    uint8_t candidate_digest[TFM_FWU_MAX_DIGEST_SIZE];
 } psa_fwu_impl_info_t;

/**
 * @brief Information about the firmware store for a firmware component.
 */
typedef struct psa_fwu_component_info_t {
    /* State of the component. */
    uint8_t state;
    /* Error for second image when store state is REJECTED or FAILED. */
    psa_status_t error;
    /* Version of active image. */
    psa_fwu_image_version_t version;
    /* Maximum image size in bytes. */
    uint32_t max_size;
    /* Flags that describe extra information about the firmware component. */
    uint32_t flags;
    /* Implementation-defined image location. */
    uint32_t location;
    /* Reserved for implementation-specific usage. */
    psa_fwu_impl_info_t impl;
} psa_fwu_component_info_t;

/**
 * @brief Retrieve the firmware store information for a specific firmware
 *        component.
 *
 * @param component Firmware component for which information is requested.
 * @param info      Output parameter for component information.
 *
 * @return Result status.
 */
psa_status_t psa_fwu_query(psa_fwu_component_t component,
                           psa_fwu_component_info_t *info);

/**
 * @brief Begin a firmware update operation for a specific firmware component.
 *
 * @param component     Identifier of the firmware component to be updated.
 * @param manifest      A pointer to a buffer containing a detached manifest for
 *                      the update.
 * @param manifest_size The size of the detached manifest.
 *
 * @return Result status.
 */
psa_status_t psa_fwu_start(psa_fwu_component_t component,
                           const void *manifest,
                           size_t manifest_size);

/**
 * @brief The maximum permitted size for block in psa_fwu_write(), in bytes.
 */
#define PSA_FWU_MAX_WRITE_SIZE TFM_CONFIG_FWU_MAX_WRITE_SIZE

/**
 * @brief Write a firmware image, or part of a firmware image, to its staging
 *        area.
 *
 * @param component    Identifier of the firmware component being updated.
 * @param image_offset The offset of the data block in the whole image.
 * @param block        A buffer containing a block of image data.
 * @param block_size   Size of block, in bytes.
 *
 * @return Result status.
 */
psa_status_t psa_fwu_write(psa_fwu_component_t component,
                           size_t image_offset,
                           const void *block,
                           size_t block_size);

/**
 * @brief Mark a firmware image in the staging area as ready for installation.
 *
 * @param component Identifier of the firmware component to install.
 *
 * @return Result status.
 */
psa_status_t psa_fwu_finish(psa_fwu_component_t component);

/**
 * @brief Abandon an update that is in WRITING or CANDIDATE state.
 *
 * @param component Identifier of the firmware component to be cancelled.
 *
 * @return Result status.
 */
psa_status_t psa_fwu_cancel(psa_fwu_component_t component);

/**
 * @brief Prepare the component for another update.
 *
 * @param component Identifier of the firmware component to tidy up.
 *
 * @return Result status.
 */
psa_status_t psa_fwu_clean(psa_fwu_component_t component);

/**
 * @brief Start the installation of all firmware images that have been prepared
 *        for update.
 *
 * @return Result status.
 */
psa_status_t psa_fwu_install(void);

/**
 * @brief Requests the platform to reboot.
 *
 * @return Result status.
 */
psa_status_t psa_fwu_request_reboot(void);

/**
 * @brief Abandon an installation that is in STAGED or TRIAL state.
 *
 * @param error An application-specific error code chosen by the application.
 *
 * @return Result status.
 */
psa_status_t psa_fwu_reject(psa_status_t error);

/**
 * @brief Accept a firmware update that is currently in TRIAL state.
 *
 * @return Result status.
 */
psa_status_t psa_fwu_accept(void);

#ifdef __cplusplus
}
#endif

#endif /* PSA_UPDATE_H */

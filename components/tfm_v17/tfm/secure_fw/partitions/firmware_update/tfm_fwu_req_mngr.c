/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "config_fwu.h"
#include "tfm_platform_api.h"
#include "tfm_bootloader_fwu_abstraction.h"
#include "psa/update.h"
#include "service_api.h"
#include "tfm_api.h"
#include "psa/service.h"
#include "psa_manifest/tfm_firmware_update.h"
#include "compiler_ext_defs.h"

#define COMPONENTS_ITER(x)  \
    for ((x) = 0; (x) < (FWU_COMPONENT_NUMBER); (x)++)

typedef struct tfm_fwu_ctx_s {
    psa_status_t error;
    uint8_t component_state;
    bool in_use;
} tfm_fwu_ctx_t;

/**
 * \brief The context of FWU service.
 */
static tfm_fwu_ctx_t fwu_ctx[FWU_COMPONENT_NUMBER];

#if PSA_FRAMEWORK_HAS_MM_IOVEC != 1
static uint8_t block[TFM_FWU_BUF_SIZE] __aligned(4);
#endif

static psa_status_t tfm_fwu_start(const psa_msg_t *msg)
{
    psa_fwu_component_t component;
#if PSA_FRAMEWORK_HAS_MM_IOVEC == 1 || TFM_CONFIG_FWU_MAX_MANIFEST_SIZE == 0
    uint8_t *manifest = NULL;
#else
    uint8_t manifest_data[TFM_CONFIG_FWU_MAX_MANIFEST_SIZE];
    uint8_t *manifest = manifest_data;
#endif
    size_t manifest_size;
    psa_status_t status;
    psa_fwu_component_info_t info;

    /* Check input parameters. */
    if (msg->in_size[0] != sizeof(component)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    if (msg->in_size[1] > TFM_CONFIG_FWU_MAX_MANIFEST_SIZE) {
        return PSA_ERROR_NOT_SUPPORTED;
    }
    psa_read(msg->handle, 0, &component, sizeof(component));

    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }
    manifest_size = msg->in_size[1];
    if (manifest_size > 0) {
#if PSA_FRAMEWORK_HAS_MM_IOVEC == 1
        manifest = (uint8_t *)psa_map_invec(msg->handle, 1);
#else
        psa_read(msg->handle, 1, manifest, manifest_size);
#endif
    }

    /* The component is in FWU process. */
    if (fwu_ctx[component].in_use) {
        if (fwu_ctx[component].component_state != PSA_FWU_READY) {
            return PSA_ERROR_BAD_STATE;
        }
    } else {
        /* Query the state of the component. */
        status = fwu_bootloader_get_image_info(component, true, false, &info);
        if (status != PSA_SUCCESS) {
            return status;
        }
        if (info.state != PSA_FWU_READY) {
            return PSA_ERROR_BAD_STATE;
        }

        /* The component is not in FWU process. Initialize the ctx for this component. */
        status = fwu_bootloader_staging_area_init(component,
                                                  (const void *)manifest,
                                                  manifest_size);
        if (status != PSA_SUCCESS) {
            return status;
        }
        fwu_ctx[component].in_use = true;
        fwu_ctx[component].component_state = PSA_FWU_WRITING;
    }
    return PSA_SUCCESS;
}

static psa_status_t tfm_fwu_write(const psa_msg_t *msg)
{
    psa_fwu_component_t component;
    size_t image_offset;
    size_t block_size;
    psa_status_t status = PSA_SUCCESS;
#if PSA_FRAMEWORK_HAS_MM_IOVEC == 1
    uint8_t *block;
#else
    size_t write_size, num;
#endif

    /* Check input parameters. */
    if (msg->in_size[2] > PSA_FWU_MAX_WRITE_SIZE) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    block_size = msg->in_size[2];

    if (msg->in_size[0] != sizeof(component) ||
        msg->in_size[1] != sizeof(image_offset)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    psa_read(msg->handle, 0, &component, sizeof(component));
    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    psa_read(msg->handle, 1, &image_offset, sizeof(image_offset));

    /* Check the component state. */
    if (!fwu_ctx[component].in_use ||
        fwu_ctx[component].component_state != PSA_FWU_WRITING) {
        return PSA_ERROR_BAD_STATE;
    }
#if PSA_FRAMEWORK_HAS_MM_IOVEC == 1
    if (block_size > 0) {
        block = (uint8_t *)psa_map_invec(msg->handle, 2);
        status = fwu_bootloader_load_image(component,
                                           image_offset,
                                           block,
                                           block_size);
    }
#else
    while (block_size > 0) {
        write_size = sizeof(block) <= block_size ?
                     sizeof(block) : block_size;
        num = psa_read(msg->handle, 2, block, write_size);
        if (num != write_size) {
            return PSA_ERROR_PROGRAMMER_ERROR;
        }

        status = fwu_bootloader_load_image(component,
                                           image_offset,
                                           block,
                                           write_size);
        if (status != PSA_SUCCESS) {
            return status;
        }
        block_size -= write_size;
        image_offset += write_size;
    }
#endif
    return status;
}

static psa_status_t tfm_fwu_finish(const psa_msg_t *msg)
{
    psa_fwu_component_t component;

    /* Check input parameters. */
    if (msg->in_size[0] != sizeof(component)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    psa_read(msg->handle, 0, &component, sizeof(component));
    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    /* Check the component state. */
    if (!fwu_ctx[component].in_use ||
        fwu_ctx[component].component_state != PSA_FWU_WRITING) {
        return PSA_ERROR_BAD_STATE;
    }

    /* Validity, authenticity and integrity of the image is deferred to system
     * reboot.
     */
    fwu_ctx[component].component_state = PSA_FWU_CANDIDATE;
    return PSA_SUCCESS;
}

static psa_status_t tfm_fwu_install(void)
{
    psa_fwu_component_t component = 0;
    psa_status_t status;
    uint8_t candidate_number = 0, index;
    psa_fwu_component_info_t info;
    psa_fwu_component_t candidates[FWU_COMPONENT_NUMBER];

    /* If at least one component is in STAGED, TRIAL or REJECTED state results
     * PSA_ERROR_BAD_STATE error.
     */
    COMPONENTS_ITER(component) {
        if (fwu_ctx[component].in_use) {
            if (fwu_ctx[component].component_state == PSA_FWU_STAGED ||
                fwu_ctx[component].component_state == PSA_FWU_REJECTED) {
                return PSA_ERROR_BAD_STATE;
            } else if (fwu_ctx[component].component_state == PSA_FWU_CANDIDATE) {
                candidates[candidate_number++] = component;
            }
        } else {
            status = fwu_bootloader_get_image_info(component, true, false, &info);
            if (status != PSA_SUCCESS) {
                return status;
            }
            if (info.state == PSA_FWU_TRIAL) {
                return PSA_ERROR_BAD_STATE;
            }
        }
    }

    /* No components in CANDIDATE state. */
    if (candidate_number == 0) {
        return PSA_ERROR_BAD_STATE;
    }

    status = fwu_bootloader_install_image(candidates,
                                          candidate_number);

    if (status == PSA_ERROR_DEPENDENCY_NEEDED) {
        /* No need to update the CANDIDATE components' state in this case. */
        return status;
    }

    /* Update the CANDIDATE components' state and error context accordingly. */
    for (index = 0; index < candidate_number; index++) {
        component = candidates[index];
        if (status == PSA_SUCCESS) {
            /* Check TRIAL state is supported or not. */
#ifdef FWU_SUPPORT_TRIAL_STATE
            fwu_ctx[component].component_state = PSA_FWU_TRIAL;
#else
            fwu_ctx[component].component_state = PSA_FWU_UPDATED;
#endif
        } else if (status != PSA_SUCCESS_REBOOT && status != PSA_SUCCESS_RESTART) {
            /* Switch to FAILED state on other errors. */
            fwu_ctx[component].component_state = PSA_FWU_FAILED;
            fwu_ctx[component].error = status;
        } else {
            fwu_ctx[component].component_state = PSA_FWU_STAGED;
        }
    }
    return status;
}

static psa_status_t tfm_fwu_query(const psa_msg_t *msg)
{
    psa_fwu_component_t component = { 0 };
    psa_fwu_component_info_t info;
    psa_status_t result;
    bool query_impl_info = false, query_state = true;

    /* Check input parameters. */
    if (msg->in_size[0] != sizeof(component) ||
        msg->out_size[0] != sizeof(psa_fwu_component_info_t)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    psa_read(msg->handle, 0, &component, sizeof(component));
    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    if (fwu_ctx[component].in_use) {
        info.state = fwu_ctx[component].component_state;
        query_state = false;
        /* The psa_fwu_impl_info_t contains the digest of second image when
         * store state is CANDIDATE. Calculate the digest when store state is
         * CANDIDATE.
         */
        if (fwu_ctx[component].component_state == PSA_FWU_CANDIDATE) {
            query_impl_info = true;
        } else if (fwu_ctx[component].component_state == PSA_FWU_REJECTED ||
                   fwu_ctx[component].component_state == PSA_FWU_FAILED) {
            info.error = fwu_ctx[component].error;
        }
    }

    /* If there is no active ctx, the possible component states are READY and
     * TRIAL.
     */
    result = fwu_bootloader_get_image_info(component, query_state,
                                           query_impl_info, &info);
    if (result == PSA_SUCCESS) {
        psa_write(msg->handle, 0, &info, sizeof(info));
    }

    return result;
}

static psa_status_t tfm_fwu_request_reboot(void)
{
    tfm_platform_system_reset();

    return PSA_SUCCESS;
}

static psa_status_t tfm_fwu_accept(void)
{
#ifdef FWU_SUPPORT_TRIAL_STATE
    psa_fwu_component_t component = 0;
    psa_status_t status = PSA_ERROR_BAD_STATE;
    psa_fwu_component_info_t info;
    psa_fwu_component_t trials[FWU_COMPONENT_NUMBER];
    uint8_t trials_number = 0, index;

    COMPONENTS_ITER(component) {
        if (fwu_ctx[component].in_use) {
            if (fwu_ctx[component].component_state == PSA_FWU_TRIAL) {
                trials[trials_number++] = component;
            }
        } else {
            status = fwu_bootloader_get_image_info(component, true, false, &info);
            if (status != PSA_SUCCESS) {
                return status;
            }
            if (info.state == PSA_FWU_TRIAL) {
                trials[trials_number++] = component;
                fwu_ctx[component].in_use = true;
                fwu_ctx[component].component_state = PSA_FWU_TRIAL;
            }
        }
    }

    if (trials_number == 0) {
        return PSA_ERROR_BAD_STATE;
    }
    status = fwu_bootloader_mark_image_accepted(trials, trials_number);
    if (status != PSA_SUCCESS) {
        /* Rebooting the system results in the image being automatically rejected.
         * Keep the component in TRIAL state.
         */
        return status;
    }

    for (index = 0; index < trials_number; index++) {
        component = trials[index];
        fwu_ctx[component].component_state = PSA_FWU_UPDATED;
    }
    return status;
#else
    return PSA_ERROR_NOT_SUPPORTED;
#endif
}

psa_status_t tfm_fwu_reject(const psa_msg_t *msg)
{
    psa_fwu_component_t component = 0;
    psa_status_t status = PSA_SUCCESS, error;
    psa_fwu_component_info_t info;
    bool staged_trial_component_found = false, in_trial_state = false;
    size_t num;

    /* Check input parameters. */
    if (msg->in_size[0] != sizeof(error)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }
    num = psa_read(msg->handle, 0, &error, sizeof(error));
    if (num != sizeof(error)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    COMPONENTS_ITER(component) {
        in_trial_state = false;
        if (fwu_ctx[component].in_use) {
            if (fwu_ctx[component].component_state == PSA_FWU_STAGED) {
                /* If the installation state is STAGED, then the state of
                 * affected components changes to FAILED.
                 */
                fwu_ctx[component].component_state = PSA_FWU_FAILED;
                fwu_ctx[component].error = error;
                staged_trial_component_found = true;
                /* Reject the staged image. */
                status = fwu_bootloader_reject_staged_image(component);
                if (status != PSA_SUCCESS) {
                    return status;
                }
            } else if (fwu_ctx[component].component_state == PSA_FWU_TRIAL) {
                staged_trial_component_found = true;
                in_trial_state = true;
            }
        } else {
            status = fwu_bootloader_get_image_info(component, true, false, &info);
            if (status != PSA_SUCCESS) {
                return status;
            }

            /* Reject the component if it is in TRIAL state. Reserve a ctx
             * for the component to record its state.
             */
            if (info.state == PSA_FWU_TRIAL) {
                fwu_ctx[component].in_use = true;
                in_trial_state = true;
                fwu_ctx[component].component_state = PSA_FWU_TRIAL;
                staged_trial_component_found = true;
            }
        }
        if (in_trial_state) {
            /* Reject the running(trial) image. */
            status = fwu_bootloader_reject_trial_image(component);
            /* If a reboot is required, the state of affected components
             * changes to REJECTED and PSA_SUCCESS_REBOOT is returned.
             * If a component restart is required, the state of affected
             * components changes to REJECTED and PSA_SUCCESS_RESTART is
             * returned.
             * If no reboot or component restart is required, the state
             * of affected components changes to FAILED and PSA_SUCCESS
             * is returned.
             */
            if (status == PSA_SUCCESS_REBOOT || status == PSA_SUCCESS_RESTART) {
                fwu_ctx[component].component_state = PSA_FWU_REJECTED;
                fwu_ctx[component].error = error;
            } else if (status == PSA_SUCCESS) {
                fwu_ctx[component].component_state = PSA_FWU_FAILED;
                fwu_ctx[component].error = error;
            } else {
                return status;
            }
        }
    }
    if (!staged_trial_component_found) {
        /* There are no components in the STAGED or TRIAL state. */
        return PSA_ERROR_BAD_STATE;
    }
    return status;
}

static psa_status_t tfm_fwu_cancel(const psa_msg_t *msg)
{
    psa_fwu_component_t component;

    if (msg->in_size[0] != sizeof(component)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    psa_read(msg->handle, 0, &component, sizeof(component));
    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    if (fwu_ctx[component].in_use) {
        /* The component is in FWU process. */
        if ((fwu_ctx[component].component_state == PSA_FWU_WRITING) ||
           (fwu_ctx[component].component_state == PSA_FWU_CANDIDATE)) {
            fwu_ctx[component].component_state = PSA_FWU_FAILED;
            fwu_ctx[component].error = PSA_SUCCESS;
            return PSA_SUCCESS;
        } else {
            /* If the image is in INSTALLED state or UNDEFINED, it should not in
             * a FWU process.
             */
            return PSA_ERROR_BAD_STATE;
        }
    } else {
        /* The component is not in WRITING or CANDIDATE state. */
        return PSA_ERROR_BAD_STATE;
    }
}

static psa_status_t tfm_fwu_clean(const psa_msg_t *msg)
{
    psa_fwu_component_t component;
    psa_status_t status;

    if (msg->in_size[0] != sizeof(component)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    psa_read(msg->handle, 0, &component, sizeof(component));
    if (component >= FWU_COMPONENT_NUMBER) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    if (fwu_ctx[component].in_use) {
        /* The component is in FWU process. */
        if ((fwu_ctx[component].component_state == PSA_FWU_FAILED) ||
            (fwu_ctx[component].component_state == PSA_FWU_UPDATED)) {
            status = fwu_bootloader_clean_component(component);
            if (status != PSA_SUCCESS) {
                return status;
            }
            fwu_ctx[component].component_state = PSA_FWU_READY;
            fwu_ctx[component].in_use = false;
            return PSA_SUCCESS;
        } else {
            /* If the image is in INSTALLED state or UNDEFINED, it should not in
             * a FWU process.
             */
            return PSA_ERROR_BAD_STATE;
        }
    } else {
        /* The component is not in FAILED or UPDATED state. */
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t tfm_firmware_update_service_sfn(const psa_msg_t *msg)
{
    switch (msg->type) {
    case TFM_FWU_START:
        return tfm_fwu_start(msg);
    case TFM_FWU_WRITE:
        return tfm_fwu_write(msg);
    case TFM_FWU_FINISH:
        return tfm_fwu_finish(msg);
    case TFM_FWU_INSTALL:
        return tfm_fwu_install();
    case TFM_FWU_CANCEL:
        return tfm_fwu_cancel(msg);
    case TFM_FWU_CLEAN:
        return tfm_fwu_clean(msg);
    case TFM_FWU_QUERY:
        return tfm_fwu_query(msg);
    case TFM_FWU_REQUEST_REBOOT:
        return tfm_fwu_request_reboot();
    case TFM_FWU_ACCEPT:
        return tfm_fwu_accept();
    case TFM_FWU_REJECT:
        return tfm_fwu_reject(msg);
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }
}

psa_status_t tfm_fwu_entry(void)
{
    if (fwu_bootloader_init() != 0) {
        return PSA_ERROR_GENERIC_ERROR;
    }
    return PSA_SUCCESS;
}

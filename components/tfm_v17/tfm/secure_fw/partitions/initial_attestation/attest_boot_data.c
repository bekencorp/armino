/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "attest_boot_data.h"
#include "tfm_boot_status.h"
#include "tfm_attest_iat_defs.h"
#include "q_useful_buf.h"
#ifdef TFM_PARTITION_MEASURED_BOOT
#include "measured_boot_api.h"
#include "boot_measurement.h"
#include "psa/crypto.h"
#endif /* TFM_PARTITION_MEASURED_BOOT */

#define MAX_BOOT_STATUS 512

/*!
 * \struct attest_boot_data
 *
 * \brief Contains the received boot status information from bootloader
 *
 * \details This is a redefinition of \ref tfm_boot_data to allocate the
 *          appropriate, service dependent size of \ref boot_data.
 */
struct attest_boot_data {
    struct shared_data_tlv_header header;
    uint8_t data[MAX_BOOT_STATUS];
};

/*!
 * \var boot_data
 *
 * \brief Store the boot status in service's memory.
 *
 * \details Boot status comes from the secure bootloader and primarily stored
 *          on a memory area which is shared between bootloader and SPM.
 *          SPM provides the \ref tfm_core_get_boot_data() API to retrieve
 *          the service related data from shared area.
 */
__attribute__ ((aligned(4)))
static struct attest_boot_data boot_data;

/*!
 * \brief Static function to look up all entries in the shared data area
 *       (boot status) which belong to a specific module.
 *
 * \param[in]     module  The identifier of SW module to look up based on this
 * \param[out]    claim   The type of SW module's attribute
 * \param[out]    tlv_len Length of the shared data entry
 * \param[in/out] tlv_ptr Pointer to the shared data entry. If its value NULL as
 *                        input then it will starts the look up from the
 *                        beginning of the shared data section. If not NULL then
 *                        it continue look up from the next entry. It returns
 *                        the address of next found entry which belongs to
 *                        module.
 *
 * \retval    -1          Error, boot status is malformed
 * \retval     0          Entry not found
 * \retval     1          Entry found
 */
static int32_t attest_get_tlv_by_module(uint8_t    module,
                                        uint8_t   *claim,
                                        uint16_t  *tlv_len,
                                        uint8_t  **tlv_ptr)
{
    struct shared_data_tlv_entry tlv_entry;
    uint8_t *tlv_end;
    uint8_t *tlv_curr;

    if (boot_data.header.tlv_magic != SHARED_DATA_TLV_INFO_MAGIC) {
        return -1;
    }

    /* Get the boundaries of TLV section where to lookup*/
    tlv_end = (uint8_t *)&boot_data + boot_data.header.tlv_tot_len;
    if (*tlv_ptr == NULL) {
        /* At first call set to the beginning of the TLV section */
        tlv_curr = boot_data.data;
    } else {
        /* Any subsequent call set to the next TLV entry */
        (void)memcpy(&tlv_entry, *tlv_ptr, SHARED_DATA_ENTRY_HEADER_SIZE);

        tlv_curr  = (*tlv_ptr) + SHARED_DATA_ENTRY_HEADER_SIZE
                    + tlv_entry.tlv_len;
    }

    /* Iterates over the TLV section and returns the address and size of TLVs
     * with requested module identifier
     */
    while (tlv_curr < tlv_end) {
        /* Create local copy to avoid unaligned access */
        (void)memcpy(&tlv_entry, tlv_curr, SHARED_DATA_ENTRY_HEADER_SIZE);
        if (GET_IAS_MODULE(tlv_entry.tlv_type) == module) {
            *claim   = GET_IAS_CLAIM(tlv_entry.tlv_type);
            *tlv_ptr = tlv_curr;
            *tlv_len = tlv_entry.tlv_len;
            return 1;
        }

        tlv_curr += (SHARED_DATA_ENTRY_HEADER_SIZE + tlv_entry.tlv_len);
    }

    return 0;
}

int32_t attest_get_tlv_by_id(uint8_t    claim,
                             uint16_t  *tlv_len,
                             uint8_t  **tlv_ptr)
{
    uint8_t tlv_id;
    uint8_t module = SW_GENERAL;
    int32_t found;

    /* Ensure that look up starting from the beginning of the boot status */
    *tlv_ptr = NULL;

    /* Look up specific TLV entry which belongs to SW_GENERAL module */
    do {
        /* Look up next entry */
        found = attest_get_tlv_by_module(module, &tlv_id,
                                         tlv_len, tlv_ptr);
        if (found != 1) {
            break;
        }
        /* At least one entry was found which belongs to SW_GENERAL,
         * check whether this one is looked for
         */
        if (claim == tlv_id) {
            break;
        }
    } while (found == 1);

    return found;
}

#ifdef TFM_PARTITION_MEASURED_BOOT
/*!
 * \brief Static function to get a string representing the measurement algorithm
 *
 * \param[in]   algorithm        Algorithm identifier
 * \param[out]  measurement_desc Structure to carry the output string:
 *                               pointer + string length, not including
 *                               the null-terminator
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static inline enum psa_attest_err_t
get_measurement_description(psa_algorithm_t algorithm,
                            struct q_useful_buf_c *measurement_desc)
{
    switch (algorithm) {
    case PSA_ALG_SHA_256:
        measurement_desc->ptr = "sha-256";
        measurement_desc->len = 7; /* Not including the null-terminator. */
        break;
    case PSA_ALG_SHA_384:
        measurement_desc->ptr = "sha-384";
        measurement_desc->len = 7; /* Not including the null-terminator. */
    case PSA_ALG_SHA_512:
        measurement_desc->ptr = "sha-512";
        measurement_desc->len = 7; /* Not including the null-terminator. */
        break;
    default:
        /* Algorithm not supported. */
        return PSA_ATTEST_ERR_INVALID_INPUT;
    }

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to encode the properties of a SW component as a map
 *
 * \param[in]  encode_ctx        The encoding context to open the map in
 * \param[in]  signer_id         Pointer to buffer which stores the signer ID
 * \param[in]  sw_version        Pointer to buffer which stores the SW version
 * \param[in]  sw_type           Pointer to buffer which stores the SW type
 * \param[in]  measurement_desc  Pointer to buffer which stores the
 *                               measurement description
 * \param[in]  measurement       Pointer to buffer which stores the
 *                               measurement value
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_encode_sw_component(QCBOREncodeContext *encode_ctx,
                           const struct q_useful_buf_c *signer_id,
                           const struct q_useful_buf_c *sw_version,
                           const struct q_useful_buf_c *sw_type,
                           const struct q_useful_buf_c *measurement_desc,
                           const struct q_useful_buf_c *measurement)
{
    if (encode_ctx == NULL || signer_id == NULL || sw_version == NULL ||
        sw_type == NULL || measurement_desc == NULL || measurement == NULL) {
        return PSA_ATTEST_ERR_INVALID_INPUT;
    }

    QCBOREncode_OpenMap(encode_ctx);

    /* Encode signer ID as byte string. */
    QCBOREncode_AddBytesToMapN(encode_ctx,
                               IAT_SW_COMPONENT_SIGNER_ID,
                               *signer_id);

    /* Encode component version as text string. */
    QCBOREncode_AddTextToMapN(encode_ctx,
                              IAT_SW_COMPONENT_VERSION,
                              *sw_version);

    /* Encode software component type as text string. */
    QCBOREncode_AddTextToMapN(encode_ctx,
                              IAT_SW_COMPONENT_MEASUREMENT_TYPE,
                              *sw_type);

    /* Encode measurement description as text string. */
    QCBOREncode_AddTextToMapN(encode_ctx,
                              IAT_SW_COMPONENT_MEASUREMENT_DESC,
                              *measurement_desc);

    /* Encode measurement value as byte string. */
    QCBOREncode_AddBytesToMapN(encode_ctx,
                               IAT_SW_COMPONENT_MEASUREMENT_VALUE,
                               *measurement);

    QCBOREncode_CloseMap(encode_ctx);

    return PSA_ATTEST_ERR_SUCCESS;
}
#endif /* TFM_PARTITION_MEASURED_BOOT */

enum psa_attest_err_t
attest_encode_sw_components_array(QCBOREncodeContext *encode_ctx,
                                  const int32_t *map_label,
                                  uint32_t *cnt)
{
#ifdef TFM_PARTITION_MEASURED_BOOT
    uint8_t slot_index;

    Q_USEFUL_BUF_MAKE_STACK_UB(measurement_buf, MEASUREMENT_VALUE_MAX_SIZE);
    Q_USEFUL_BUF_MAKE_STACK_UB(signer_id_buf, SIGNER_ID_MAX_SIZE);
    Q_USEFUL_BUF_MAKE_STACK_UB(sw_version_buf, VERSION_MAX_SIZE);
    Q_USEFUL_BUF_MAKE_STACK_UB(sw_type_buf, SW_TYPE_MAX_SIZE);
    struct q_useful_buf_c measurement_desc = NULL_Q_USEFUL_BUF_C;
    uint32_t measurement_algo;
    bool is_locked;
    enum psa_attest_err_t err;
    psa_status_t status;

    if ((encode_ctx == NULL) || (cnt == NULL)) {
        return PSA_ATTEST_ERR_INVALID_INPUT;
    }

    *cnt = 0;

    /* Retrieve all the measurements from the Measured Boot partition
     * which are accessible to the Attestation partition.
     */
    for (slot_index = 0; slot_index < NUM_OF_MEASUREMENT_SLOTS; slot_index++) {
        measurement_buf.len = MEASUREMENT_VALUE_MAX_SIZE;
        signer_id_buf.len   = SIGNER_ID_MAX_SIZE;
        sw_version_buf.len  = VERSION_MAX_SIZE;
        sw_type_buf.len     = SW_TYPE_MAX_SIZE;

        status = tfm_measured_boot_read_measurement(slot_index,
                                                    signer_id_buf.ptr,
                                                    signer_id_buf.len,
                                                    &signer_id_buf.len,
                                                    sw_version_buf.ptr,
                                                    sw_version_buf.len,
                                                    &sw_version_buf.len,
                                                    &measurement_algo,
                                                    sw_type_buf.ptr,
                                                    sw_type_buf.len,
                                                    &sw_type_buf.len,
                                                    measurement_buf.ptr,
                                                    measurement_buf.len,
                                                    &measurement_buf.len,
                                                    &is_locked);
        if (status != PSA_SUCCESS) {
            continue;
        }

        (*cnt)++;
        if (*cnt == 1) {
            /* Open array which stores SW components claims. */
            if (map_label != NULL) {
                QCBOREncode_OpenArrayInMapN(encode_ctx, *map_label);
            } else {
                QCBOREncode_OpenArray(encode_ctx);
            }
        }

        err = get_measurement_description(measurement_algo, &measurement_desc);
        if (err != PSA_ATTEST_ERR_SUCCESS) {
            return PSA_ATTEST_ERR_GENERAL;
        }

        err = attest_encode_sw_component(
                            encode_ctx,
                            (struct q_useful_buf_c *)&signer_id_buf,
                            (struct q_useful_buf_c *)&sw_version_buf,
                            (struct q_useful_buf_c *)&sw_type_buf,
                            &measurement_desc,
                            (struct q_useful_buf_c *)&measurement_buf);
        if (err != PSA_ATTEST_ERR_SUCCESS) {
            return PSA_ATTEST_ERR_GENERAL;
        }
    }

#else /* TFM_PARTITION_MEASURED_BOOT */
    struct q_useful_buf_c encoded_const = NULL_Q_USEFUL_BUF_C;
    uint16_t tlv_len;
    uint8_t *tlv_ptr;
    uint8_t  tlv_id;
    uint8_t module = 0;
    int32_t found;

    if ((encode_ctx == NULL) || (cnt == NULL)) {
        return PSA_ATTEST_ERR_INVALID_INPUT;
    }

    *cnt = 0;

    /* Extract all boot records (measurements) from the boot status information
     * that was received from the secure bootloader.
     */
    for (module = 0; module < SW_MAX; ++module) {
        /* Indicates to restart the look up from the beginning of the shared
         * data section.
         */
        tlv_ptr = NULL;

        /* Look up the first TLV entry which belongs to the SW module */
        found = attest_get_tlv_by_module(module, &tlv_id,
                                         &tlv_len, &tlv_ptr);
        if (found == -1) {
            /* Boot status area is malformed. */
            return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
        } else if ((found == 1) && (tlv_id == SW_BOOT_RECORD)) {
            (*cnt)++;
            if (*cnt == 1) {
                /* Open array which stores SW components claims. */
                if (map_label != NULL) {
                    QCBOREncode_OpenArrayInMapN(encode_ctx, *map_label);
                } else {
                    QCBOREncode_OpenArray(encode_ctx);
                }
            }

            encoded_const.ptr = tlv_ptr + SHARED_DATA_ENTRY_HEADER_SIZE;
            encoded_const.len = tlv_len;
            QCBOREncode_AddEncoded(encode_ctx, encoded_const);
        }
    }
#endif /* TFM_PARTITION_MEASURED_BOOT */

    if (*cnt != 0) {
        /* Close array which stores SW components claims. */
        QCBOREncode_CloseArray(encode_ctx);
    }

    return PSA_ATTEST_ERR_SUCCESS;
}

enum psa_attest_err_t attest_boot_data_init(void)
{
    return attest_get_boot_data(TLV_MAJOR_IAS,
                                (struct tfm_boot_data *)&boot_data,
                                MAX_BOOT_STATUS);
}

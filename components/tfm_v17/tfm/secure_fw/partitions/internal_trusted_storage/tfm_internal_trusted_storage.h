/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_INTERNAL_TRUSTED_STORAGE_H__
#define __TFM_INTERNAL_TRUSTED_STORAGE_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "psa/error.h"
#include "psa/storage_common.h"

#include "flash_fs/its_flash_fs.h"
#include "its_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

struct its_asset_info {
    psa_storage_uid_t uid;
    int32_t client_id;
    psa_storage_create_flags_t create_flags;
};

/**
 * \brief Initializes the internal trusted storage system.
 *
 * \return A status indicating the success/failure of the operation as specified
 *         in \ref psa_status_t
 *
 * \retval PSA_SUCCESS                The operation completed successfully
 * \retval PSA_ERROR_STORAGE_FAILURE  The operation failed because the storage
 *                                    system initialization has failed (fatal
 *                                    error)
 * \retval PSA_ERROR_GENERIC_ERROR    The operation failed because of an
 *                                    unspecified internal failure
 */
psa_status_t tfm_its_init(void);

/**
 * \brief Create a new, or modify an existing, uid/value pair
 *
 * Stores data in the internal storage.
 *
 * \param[in] asset_info    The asset info include UID, client, etc...
 * \param[in] data_buf      The asset data buffer to be stored
 * \param[in] max_size      Size of the asset file to be set. Ignored if
 *                          the file is not being created.
 * \param[in] size_to_write Size in bytes of data to write in a single call
 * \param[in] offset        Offset in the file to write. Must be less than or
 *                          equal to the current file size.
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                     The operation completed successfully
 * \retval PSA_ERROR_NOT_SUPPORTED         The operation failed because one or
 *                                         more of the flags provided in
 *                                         `create_flags` is not supported or is
 *                                         not valid
 * \retval PSA_ERROR_INSUFFICIENT_STORAGE  The operation failed because there
 *                                         was insufficient space on the
 *                                         storage medium
 * \retval PSA_ERROR_STORAGE_FAILURE       The operation failed because the
 *                                         physical storage has failed (Fatal
 *                                         error)
 * \retval PSA_ERROR_INVALID_ARGUMENT      The operation failed because one
 *                                         of the provided pointers (`p_data`)
 *                                         is invalid, for example is `NULL` or
 *                                         references memory the caller cannot
 *                                         access
 */
psa_status_t tfm_its_set(struct its_asset_info *asset_info,
                         uint8_t *data_buf,
                         size_t max_size,
                         size_t size_to_write,
                         size_t offset);

/**
 * \brief Retrieve data associated with a provided UID
 *
 * Retrieves up to `data_size` bytes of the data associated with `uid`, starting
 * at `data_offset` bytes from the beginning of the data. Upon successful
 * completion, the data will be placed in the `p_data` buffer, which must be at
 * least `data_size` bytes in size. The length of the data returned will be in
 * `p_data_length`. If `data_size` is 0, the contents of `p_data_length` will
 * be set to zero.
 *
 * \param[in]  asset_info     The asset info include UID, client, etc...
 * \param[out] data_buf       The buffer to stored the asset data
 * \param[in]  size_to_read   The amount of data requested
 * \param[in]  offset         The starting offset of the data requested
 * \param[out] size_read      On success, this will contain size of the data
 *                            placed in `data_buf`.
 * \param[in] first_get       Indicator of if it is the first call of a series
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                 The operation completed successfully
 * \retval PSA_ERROR_DOES_NOT_EXIST    The operation failed because the
 *                                     provided `uid` value was not found in
 *                                     the storage
 * \retval PSA_ERROR_STORAGE_FAILURE   The operation failed because the
 *                                     physical storage has failed (Fatal
 *                                     error)
 * \retval PSA_ERROR_INVALID_ARGUMENT  The operation failed because one of the
 *                                     provided arguments (`p_data`,
 *                                     `p_data_length`) is invalid, for example
 *                                     is `NULL` or references memory the
 *                                     caller cannot access. In addition, this
 *                                     can also happen if `data_offset` is
 *                                     larger than the size of the data
 *                                     associated with `uid`.
 */
psa_status_t tfm_its_get(struct its_asset_info *asset_info,
                         uint8_t *data_buf,
                         size_t size_to_read,
                         size_t offset,
                         size_t *size_read,
                         bool first_get);

/**
 * \brief Retrieve the metadata about the provided uid
 *
 * Retrieves the metadata stored for a given `uid` as a `psa_storage_info_t`
 * structure.
 *
 * \param[in]  client_id  Identifier of the asset's owner (client)
 * \param[in]  uid        The `uid` value
 * \param[out] p_info     A pointer to the `psa_storage_info_t` struct that will
 *                        be populated with the metadata
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                 The operation completed successfully
 * \retval PSA_ERROR_DOES_NOT_EXIST    The operation failed because the provided
 *                                     uid value was not found in the storage
 * \retval PSA_ERROR_STORAGE_FAILURE   The operation failed because the physical
 *                                     storage has failed (Fatal error)
 * \retval PSA_ERROR_INVALID_ARGUMENT  The operation failed because one of the
 *                                     provided pointers(`p_info`)
 *                                     is invalid, for example is `NULL` or
 *                                     references memory the caller cannot
 *                                     access
 */
psa_status_t tfm_its_get_info(int32_t client_id, psa_storage_uid_t uid,
                              struct psa_storage_info_t *p_info);

/**
 * \brief Remove the provided uid and its associated data from the storage
 *
 * Deletes the data from internal storage.
 *
 * \param[in] client_id  Identifier of the asset's owner (client)
 * \param[in] uid        The `uid` value
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                 The operation completed successfully
 * \retval PSA_ERROR_INVALID_ARGUMENT  The operation failed because one or more
 *                                     of the given arguments were invalid (null
 *                                     pointer, wrong flags and so on)
 * \retval PSA_ERROR_DOES_NOT_EXIST    The operation failed because the provided
 *                                     uid value was not found in the storage
 * \retval PSA_ERROR_NOT_PERMITTED     The operation failed because the provided
 *                                     uid value was created with
 *                                     PSA_STORAGE_FLAG_WRITE_ONCE
 * \retval PSA_ERROR_STORAGE_FAILURE   The operation failed because the physical
 *                                     storage has failed (Fatal error)
 */
psa_status_t tfm_its_remove(int32_t client_id, psa_storage_uid_t uid);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_INTERNAL_TRUSTED_STORAGE_H__ */

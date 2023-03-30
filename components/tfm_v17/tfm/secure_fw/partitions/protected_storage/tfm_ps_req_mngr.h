/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PS_REQ_MNGR_H__
#define __TFM_PS_REQ_MNGR_H__

#include <stddef.h>

#include "psa/client.h"
#include "psa/protected_storage.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Takes an input buffer containing asset data and writes
 *        its contents to the client iovec
 *
 * \param[in]  in_data Pointer to the buffer data will read from.
 * \param[in]  size    The amount of data to read.
 *
 *
 */
void ps_req_mngr_write_asset_data(const uint8_t *in_data,
                                  uint32_t size);

/**
 * \brief Writes the asset data of a client iovec onto an output buffer
 *
 * \param[out] out_data  Pointer to the buffer data will be written to.
 * \param[in]  size      The amount of data to write.
 *
 * \return A status indicating the success/failure of the operation as specified
 *         in \ref psa_status_t
 *
 */
psa_status_t ps_req_mngr_read_asset_data(uint8_t *out_data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PS_REQ_MNGR_H__ */

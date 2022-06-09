/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SPE_DUAL_CORE_PSA_CLIENT_SECURE_LIB_H__
#define __TFM_SPE_DUAL_CORE_PSA_CLIENT_SECURE_LIB_H__

#include "tfm_openamp_lib.h"

/**
 * \brief Initializes the library.
 */
void init_dual_core_psa_client_secure_lib(void);

/**
 * \brief Decodes the messages received from the NSPE before sending
 * to SPE.
 */
void deliver_msg_to_tfm_spe(void *private);

/**
 * \brief Encodes the reply of service before sending it to NSPE.
 */
void send_service_reply_to_non_secure(int32_t reply, void *private);

/**
 * \brief Validate and register the message. The message details are
 * copied inside the unordered_map.
 *
 * \retval OPENAMP_SUCCESS      Successfully registered the message.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t register_msg_to_spe_and_verify(void **private,
                        const void *data, size_t len);

#endif /* __TFM_SPE_DUAL_CORE_PSA_CLIENT_SECURE_LIB_H__ */

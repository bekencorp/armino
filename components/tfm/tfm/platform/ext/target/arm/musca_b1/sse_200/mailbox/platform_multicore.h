/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PLATFORM_MULTICORE_H_
#define _PLATFORM_MULTICORE_H_

#include <stdint.h>

/* Arbitrary predefined values to sync between Host and Secure Enclave */
#define IPC_SYNC_MAGIC                   (0x7DADE011)

#define NS_MAILBOX_INIT_ENABLE           (0xAE)
#define S_MAILBOX_READY                  (0xC3)

#define PSA_CLIENT_CALL_REQ_MAGIC        (0xA5CF50C6)

#define PLATFORM_MAILBOX_SUCCESS         (0x0)

/**
 * \brief Fetch a data value from mailbox message
 *
 * \param[out] data_ptr    The address to write the pointer value to.
 *
 * \retval 0               The operation succeeds.
 * \retval else            The operation fails.
 */
int platform_mailbox_fetch_msg_data(uint32_t *data_ptr);

/**
 * \brief Send a pointer via mailbox message
 *
 * \param[in] msg_ptr      The pointer value to be sent.
 *
 * \retval 0               The operation succeeds.
 * \retval else            The operation fails.
 */
int platform_mailbox_send_msg_ptr(const void *msg_ptr);

/**
 * \brief Send a data value via mailbox message
 *
 * \param[in] data         The data value to be sent
 *
 * \retval 0               The operation succeeds.
 * \retval else            The operation fails.
 */
int platform_mailbox_send_msg_data(uint32_t data);

/**
 * \brief Wait for a mailbox notify event.
 */
void platform_mailbox_wait_for_notify(void);

#endif /* _PLATFORM_MULTICORE_H_ */

/*
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __NV_COUNTERS_H__
#define __NV_COUNTERS_H__

#include <stdint.h>

/**
 * \brief Retrieve the base address and size of the non-volatile counters
 *        flash region.
 *
 * Note that this function should ensure that the values returned do
 * not result in a security compromise.
 *
 * \param[out] base            Base address of NVC flash region
 * \param[out] size            Size of NVC flash region
 *
 * \return void
 */
void nvc_flash_block(uint32_t *base, size_t *size);

#endif /* __NV_COUNTERS_H__ */

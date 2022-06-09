/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_DMA_H
#define CC3XX_DMA_H

#include "cc3xx_error.h"

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

cc3xx_err_t cc3xx_dma_copy_data(void* dest, const void* src, size_t length);
cc3xx_err_t cc3xx_dma_input_data(const void* buf, size_t length);
cc3xx_err_t cc3xx_dma_set_output(void* buf, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_DMA_H */

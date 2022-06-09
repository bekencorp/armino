/*
 *
 * Copyright (c) 2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef SPI_STRATAFLASHJ3_FLASH_LIB_H
#define SPI_STRATAFLASHJ3_FLASH_LIB_H

#include "cfi_drv.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief SPI Flash error enumeration types
 */
enum strataflashj3_error_t {
    STRATAFLASHJ3_ERR_NONE = CFI_ERR_NONE,
    STRATAFLASHJ3_ERR_WRONG_ARGUMENT = CFI_ERR_WRONG_ARGUMENT,
    STRATAFLASHJ3_ERR_NOT_INITIALIZED = CFI_ERR_NOT_INITIALIZED,
    STRATAFLASHJ3_ERR_WRONG_MEMORY,
    STRATAFLASHJ3_ERR_FLASH_CMD_FAILED,
    STRATAFLASHJ3_ERR_READ_IN_PROGRESS,
    STRATAFLASHJ3_ERR_WRITE_IN_PROGRESS
};

struct cfi_strataflashj3_dev_t {
    struct cfi_dev_t *controller; /* QSPI Flash Controller */
    uint32_t total_sector_cnt;
    uint32_t page_size;
    uint32_t sector_size;
    uint32_t program_unit;
    bool is_initialized;
};

enum strataflashj3_error_t cfi_strataflashj3_initialize(struct cfi_strataflashj3_dev_t* dev);

void cfi_strataflashj3_uninitialize(struct cfi_strataflashj3_dev_t* dev);

enum strataflashj3_error_t cfi_strataflashj3_erase(struct cfi_strataflashj3_dev_t* dev,
                                         uint32_t addr);

enum strataflashj3_error_t cfi_strataflashj3_erase_chip(struct cfi_strataflashj3_dev_t* dev);

enum strataflashj3_error_t cfi_strataflashj3_program(struct cfi_strataflashj3_dev_t* dev,
                                           uint32_t addr,
                                           const uint8_t *data, uint32_t cnt);

enum strataflashj3_error_t cfi_strataflashj3_read(struct cfi_strataflashj3_dev_t* dev,
                                        uint32_t addr,
                                        uint8_t *data, uint32_t cnt);
#ifdef __cplusplus
}
#endif

#endif /* SPI_STRATAFLASHJ3_FLASH_LIB_H */

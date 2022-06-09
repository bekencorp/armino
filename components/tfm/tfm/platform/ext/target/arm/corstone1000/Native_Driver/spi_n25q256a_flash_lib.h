/*
 *
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef SPI_N25Q256A_FLASH_LIB_H
#define SPI_N25Q256A_FLASH_LIB_H

#include "xilinx_pg153_axi_qspi_controller_drv.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief SPI Flash error enumeration types
 */
enum n25q256a_error_t {
    N25Q256A_ERR_NONE = AXI_QSPI_ERR_NONE,
    N25Q256A_ERR_WRONG_ARGUMENT = AXI_QSPI_ERR_WRONG_ARGUMENT,
    N25Q256A_ERR_NOT_INITIALIZED = AXI_QSPI_ERR_NOT_INITIALIZED,
    N25Q256A_ERR_WRONG_MEMORY,
    N25Q256A_ERR_FLASH_CMD_FAILED,
    N25Q256A_ERR_READ_IN_PROGRESS,
    N25Q256A_ERR_WRITE_IN_PROGRESS
};

struct spi_n25q256a_dev_t {
    struct axi_qspi_dev_t *controller; /* QSPI Flash Controller */
    uint32_t total_sector_cnt;
    uint32_t page_size;
    uint32_t sector_size;
    uint32_t program_unit;
    bool is_initialized;
};

enum n25q256a_error_t spi_n25q256a_initialize(struct spi_n25q256a_dev_t* dev);

void spi_n25q256a_uninitialize(struct spi_n25q256a_dev_t* dev);

enum n25q256a_error_t spi_n25q256a_erase(struct spi_n25q256a_dev_t* dev,
                                         uint32_t addr);

enum n25q256a_error_t spi_n25q256a_erase_chip(struct spi_n25q256a_dev_t* dev);

enum n25q256a_error_t spi_n25q256a_program(struct spi_n25q256a_dev_t* dev,
                                           uint32_t addr,
                                           const uint8_t *data, uint32_t cnt);

enum n25q256a_error_t spi_n25q256a_read(struct spi_n25q256a_dev_t* dev,
                                        uint32_t addr,
                                        uint8_t *data, uint32_t cnt);
#ifdef __cplusplus
}
#endif

#endif /* SPI_N25Q256A_FLASH_LIB_H */

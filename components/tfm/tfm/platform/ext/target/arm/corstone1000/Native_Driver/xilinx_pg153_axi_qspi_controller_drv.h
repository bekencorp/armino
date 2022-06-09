/*
 *
 * Copyright (c) 2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef AXI_QSPI_CONTROLLER_DRV_H
#define AXI_QSPI_CONTROLLER_DRV_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Set 1 to enable debug messages */
#define DEBUG_SPI_FLASH         0

#include <stdio.h>
#if (DEBUG_SPI_FLASH == 1)
    #define SPI_FLASH_LOG_MSG(f_, ...) printf((f_), ##__VA_ARGS__)
#else
    #define SPI_FLASH_LOG_MSG(f_, ...)
#endif

/**
 * \brief AXI QSPI error enumeration types
 */
enum axi_qspi_error_t {
    AXI_QSPI_ERR_NONE,
    AXI_QSPI_ERR_WRONG_ARGUMENT,
    AXI_QSPI_ERR_NOT_INITIALIZED
};

/**
 * \brief AXI QSPI device configuration structure
 */
struct axi_qspi_dev_cfg_t {
    const uint32_t base; /*!< AXI QSPI base address */
    const uint32_t scc_base; /*!< SCC base address */
};

/**
 * \brief AXI QSPI controller device structure
 */
struct axi_qspi_dev_t {
    const struct axi_qspi_dev_cfg_t* const cfg;
    bool is_initialized;
};

/**
 * Reset the QSPI controller.
 */
enum axi_qspi_error_t axi_qspi_initialize(struct axi_qspi_dev_t* dev);

/**
 * Unitialize to XIP controller.
 */
void axi_qspi_uninitialize(struct axi_qspi_dev_t* dev);

/**
 * Main function to send data and rcv data through QSPI controller.
 */
enum axi_qspi_error_t spi_transfer_and_receive(struct axi_qspi_dev_t* dev,
        uint8_t *send_buffer, uint8_t *rcv_buffer, int bytes);

/**
 * Selects the XiP controller by programming the MUX bit.
 */
enum axi_qspi_error_t select_xip_mode(struct axi_qspi_dev_t* dev);

/**
 * Selects the QSPI controller by programming the MUX bit.
 */
enum axi_qspi_error_t select_qspi_mode(struct axi_qspi_dev_t* dev);

#ifdef __cplusplus
}
#endif

#endif /* AXI_QSPI_CONTROLLER_DRV_H */

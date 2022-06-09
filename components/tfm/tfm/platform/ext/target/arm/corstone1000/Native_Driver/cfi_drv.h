/*
 *
 * Copyright (c) 2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CFI_DRV_H
#define CFI_DRV_H

#include <stdint.h>
#include <stdbool.h>

/* First bus cycle */
#define NOR_CMD_READ_ARRAY		0xFF
#define NOR_CMD_READ_ID_CODE		0x90
#define NOR_CMD_READ_QUERY		0x98
#define NOR_CMD_READ_STATUS_REG		0x70
#define NOR_CMD_CLEAR_STATUS_REG	0x50
#define NOR_CMD_WRITE_TO_BUFFER		0xE8
#define NOR_CMD_WORD_PROGRAM		0x40
#define NOR_CMD_BLOCK_ERASE		0x20
#define NOR_CMD_LOCK_UNLOCK		0x60
#define NOR_CMD_BLOCK_ERASE_ACK		0xD0

/* Second bus cycle */
#define NOR_LOCK_BLOCK			0x01
#define NOR_UNLOCK_BLOCK		0xD0

/* Status register bits */
#define NOR_DWS				(1 << 7)
#define NOR_ESS				(1 << 6)
#define NOR_ES				(1 << 5)
#define NOR_PS				(1 << 4)
#define NOR_VPPS			(1 << 3)
#define NOR_PSS				(1 << 2)
#define NOR_BLS				(1 << 1)
#define NOR_BWS				(1 << 0)

/* Set 1 to enable debug messages */
#define DEBUG_CFI_FLASH         0

#include <stdio.h>
#if (DEBUG_CFI_FLASH == 1)
    #define CFI_FLASH_LOG_MSG(f_, ...) printf((f_), ##__VA_ARGS__)
#else
    #define CFI_FLASH_LOG_MSG(f_, ...)
#endif

enum cfi_error_t {
    CFI_ERR_NONE,
    CFI_ERR_WRONG_ARGUMENT,
    CFI_ERR_NOT_INITIALIZED,
    CFI_ERR_DEV_BUSY,
    CFI_ERR_GENERAL_IO,
    CFI_ERR_DEV_PROTECTED
};

/**
 * \brief CFI device configuration structure
 */
struct cfi_dev_cfg_t {
    const uint32_t base; /*!< CFI base address */
};

/**
 * \brief CFI controller device structure
 */
struct cfi_dev_t {
    const struct cfi_dev_cfg_t* const cfg;
};

/* Public API */
void nor_send_cmd(uintptr_t base_addr, unsigned long cmd);
void nor_send_cmd_byte(uintptr_t base_addr, unsigned long cmd);
enum cfi_error_t nor_word_program(uintptr_t base_addr, unsigned long data);
enum cfi_error_t nor_byte_program(uintptr_t base_addr, uint8_t data);
enum cfi_error_t nor_lock(uintptr_t base_addr);
enum cfi_error_t nor_unlock(uintptr_t base_addr);
enum cfi_error_t nor_erase(uintptr_t base_addr);
unsigned int nor_id_check(uintptr_t base_addr);
uint8_t nor_cfi_reg_read(uintptr_t addr);
void nor_cfi_reg_write(uintptr_t addr, uint32_t value);
#endif

/*
 *
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef SPI_FLASH_COMMANDS_H
#define SPI_FLASH_COMMANDS_H


#define CMD_WRITE_ENABLE                0x06
#define CMD_RANDOM_READ                 0x03
#define CMD_FAST_READ                   0x0B
#define CMD_PAGE_PROGRAM                0x02
#define CMD_SECTOR_ERASE                0xD8
#define CMD_SUB_SECTOR_ERASE            0x20
#define CMD_BULK_ERASE                  0xC7
#define CMD_READ_STATUSREG              0x05
#define CMD_READ_ID                     0x9E
#define CMD_WRITE_STATUSREG             0x01
#define CMD_READ_FLAG_STATUS            0x70
#define CMD_CLEAR_FLAG_STATUS           0x50
#define CMD_READ_VOLATILE_CONFIG        0x85


#endif /* SPI_FLASH_COMMANDS_H */

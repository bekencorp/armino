/**
 ******************************************************************************
 * @file    vnd_flash.c
 * @brief   This file provides all the headers of Flash operation functions.
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2017 BEKEN Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 */
#include <common/bk_include.h>
#include <os/os.h>
//#include "BkDriverFlash.h"
#include <driver/flash_partition.h>
#include <common/bk_kernel_err.h>
#include <os/mem.h>
/* Logic partition on flash devices */

const bk_logic_partition_t bk7256_partitions[BK_PARTITION_MAX] = {
	[BK_PARTITION_BOOTLOADER] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "Bootloader",
		.partition_start_addr      = 0x00000000,
		.partition_length          = 0x11000, //68KB
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_APPLICATION] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "Application",
		.partition_start_addr      = 0x11000,
		.partition_length          = 0x1DC000,      // 1904KB
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_APPLICATION1] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "Application",
		.partition_start_addr      = 0x1ED000,
		.partition_length          = 0x1DC000,      // 1904KB
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_USR_CONFIG] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "user_data",
		.partition_start_addr	   = 0x3C9000,
		.partition_length          = 0x34000, //208KB
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_OTA_FINA_EXECUTIVE] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "ota_exec_flag",
		.partition_start_addr	   = 0x3FD000,
		.partition_length          = 0x1000, 
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_RF_FIRMWARE] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "RF Firmware",
		.partition_start_addr      = 0x3FE000,
		.partition_length          = 0x1000,
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_NET_PARAM] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "NET info",
		.partition_start_addr      = 0x3FF000,
		.partition_length          = 0x1000,
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
};



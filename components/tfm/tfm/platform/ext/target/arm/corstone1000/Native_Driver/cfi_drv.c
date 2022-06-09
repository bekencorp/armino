/*
 * Copyright (c) 2015-2021, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>

#include "cfi_drv.h"



/*
 * This memory is organized as an interleaved memory of two chips
 * with a 16 bit word. It means that every 32 bit access is going
 * to access to two different chips. This is very important when
 * we send commands or read status of the chips.
 */

/*
 * DWS ready poll retries. The number of retries in this driver have been
 * obtained empirically from Juno. FVP implements a zero wait state NOR flash
 * model
 */
#define DWS_WORD_PROGRAM_RETRIES	1000
#define DWS_WORD_ERASE_RETRIES		3000000
#define DWS_WORD_LOCK_RETRIES		1000

/* Helper macro to detect end of command */
#define NOR_CMD_END (NOR_DWS | (NOR_DWS << 16l))
#define NOR_CMD_END_8BIT (NOR_DWS)


/* Helper macros to access two flash banks in parallel */
#define NOR_2X16(d)			((d << 16) | (d & 0xffff))

static inline void mmio_write_32(uintptr_t addr, uint32_t value)
{
	*(volatile uint32_t*)addr = value;
}

static inline void mmio_write_8(uintptr_t addr, uint8_t value)
{
	*(volatile uint8_t*)addr = value;
}

static inline uint32_t mmio_read_32(uintptr_t addr)
{
	return *(volatile uint32_t*)addr;
}

uint8_t nor_cfi_reg_read(uintptr_t addr){
	return *(volatile uint8_t*)addr;
}

void nor_cfi_reg_write(uintptr_t addr, uint32_t value){
	*(volatile uint32_t*)addr = value;
}

static unsigned int nor_status(uintptr_t base_addr)
{
	unsigned long status;

	nor_send_cmd(base_addr, NOR_CMD_READ_STATUS_REG);
	status = mmio_read_32(base_addr);
	status |= status >> 16; /* merge status from both flash banks */

	return status & 0xFFFF;
}

void nor_send_cmd(uintptr_t base_addr, unsigned long cmd)
{
	mmio_write_32(base_addr, NOR_2X16(cmd));
}

void nor_send_cmd_byte(uintptr_t base_addr, unsigned long cmd)
{
	mmio_write_8(base_addr, cmd);
}

/*
 * Poll Write State Machine.
 * Return values:
 *    0      = WSM ready
 *    -EBUSY = WSM busy after the number of retries
 */
static enum cfi_error_t nor_poll_dws(uintptr_t base_addr, unsigned long int retries)
{
	unsigned long status;

	do {
		nor_send_cmd(base_addr, NOR_CMD_READ_STATUS_REG);
		status = mmio_read_32(base_addr);
		if ((status & NOR_CMD_END) == NOR_CMD_END)
			return 0;
	} while (retries-- > 0);

	return -CFI_ERR_DEV_BUSY;
}

/*
 * Poll Write State Machine.
 * Return values:
 *    0      = WSM ready
 *    -EBUSY = WSM busy after the number of retries
 */
static enum cfi_error_t nor_poll_dws_byte(uintptr_t base_addr, unsigned long int retries)
{
	unsigned long status;

	do {
		nor_send_cmd_byte(base_addr, NOR_CMD_READ_STATUS_REG);
		status = nor_cfi_reg_read(base_addr);
		if ((status & NOR_CMD_END_8BIT) == NOR_CMD_END_8BIT)
			return 0;
	} while (retries-- > 0);

	return -CFI_ERR_DEV_BUSY;
}

/*
 * Return values:
 *    0      = success
 *    -EPERM = Device protected or Block locked
 *    -EIO   = General I/O error
 */
static enum cfi_error_t nor_full_status_check(uintptr_t base_addr)
{
	unsigned long status;

	/* Full status check */
	status = nor_status(base_addr);

	if (status & (NOR_PS | NOR_BLS | NOR_ESS | NOR_PSS))
		return -CFI_ERR_DEV_PROTECTED;
	if (status & (NOR_VPPS | NOR_ES))
		return -CFI_ERR_GENERAL_IO;
	return 0;
}


/*
 * This function programs a word in the flash. Be aware that it only
 * can reset bits that were previously set. It cannot set bits that
 * were previously reset. The resulting bits = old_bits & new bits.
 * Return values:
 *  0 = success
 *  otherwise it returns a negative value
 */
enum cfi_error_t nor_word_program(uintptr_t base_addr, unsigned long data)
{
	uint32_t status;
	int ret;

	nor_send_cmd(base_addr, NOR_CMD_CLEAR_STATUS_REG);

	/* Set the device in write word mode */
	nor_send_cmd(base_addr, NOR_CMD_WORD_PROGRAM);
	mmio_write_32(base_addr, data);

	ret = nor_poll_dws(base_addr, DWS_WORD_PROGRAM_RETRIES);
	if (ret == 0) {
		/* Full status check */
		nor_send_cmd(base_addr, NOR_CMD_READ_STATUS_REG);
		status = mmio_read_32(base_addr);

		if (status & (NOR_PS | NOR_BLS)) {
			nor_send_cmd(base_addr, NOR_CMD_CLEAR_STATUS_REG);
			ret = -CFI_ERR_DEV_PROTECTED;
		}
	}

	if (ret == 0)
		ret = nor_full_status_check(base_addr);
	nor_send_cmd(base_addr, NOR_CMD_READ_ARRAY);

	return ret;
}


enum cfi_error_t nor_byte_program(uintptr_t base_addr, uint8_t data)
{
	uint32_t status;
	int ret;

	nor_send_cmd_byte(base_addr, NOR_CMD_CLEAR_STATUS_REG);

	/* Set the device in write byte mode */
	nor_send_cmd_byte(base_addr, NOR_CMD_WORD_PROGRAM);
	mmio_write_8(base_addr, data);

	ret = nor_poll_dws_byte(base_addr, DWS_WORD_PROGRAM_RETRIES);
	if (ret == 0) {
		/* Full status check */
		nor_send_cmd_byte(base_addr, NOR_CMD_READ_STATUS_REG);
		status = nor_cfi_reg_read(base_addr);

		if (status & (NOR_PS | NOR_BLS)) {
			nor_send_cmd_byte(base_addr, NOR_CMD_CLEAR_STATUS_REG);
			ret = -CFI_ERR_DEV_PROTECTED;
		}
	}


	nor_send_cmd_byte(base_addr, NOR_CMD_READ_ARRAY);

	return ret;
}

/*
 * Erase a full 256K block
 * Return values:
 *  0 = success
 *  otherwise it returns a negative value
 */
enum cfi_error_t nor_erase(uintptr_t base_addr)
{
	int ret;

	nor_send_cmd(base_addr, NOR_CMD_CLEAR_STATUS_REG);

	nor_send_cmd(base_addr, NOR_CMD_BLOCK_ERASE);
	nor_send_cmd(base_addr, NOR_CMD_BLOCK_ERASE_ACK);

	ret = nor_poll_dws(base_addr, DWS_WORD_ERASE_RETRIES);
	if (ret == 0)
		ret = nor_full_status_check(base_addr);
	nor_send_cmd(base_addr, NOR_CMD_READ_ARRAY);

	return ret;
}

enum cfi_error_t nor_erase_byte(uintptr_t base_addr)
{
	enum cfi_error_t ret;

	nor_send_cmd_byte(base_addr, NOR_CMD_CLEAR_STATUS_REG);

	nor_send_cmd_byte(base_addr, NOR_CMD_BLOCK_ERASE);
	nor_send_cmd_byte(base_addr, NOR_CMD_BLOCK_ERASE_ACK);

	ret = nor_poll_dws_byte(base_addr, DWS_WORD_ERASE_RETRIES);

	nor_send_cmd_byte(base_addr, NOR_CMD_READ_ARRAY);

	return ret;
}

/*
 * Lock a full 256 block
 * Return values:
 *  0 = success
 *  otherwise it returns a negative value
 */
enum cfi_error_t nor_lock(uintptr_t base_addr)
{
	enum cfi_error_t ret;

	nor_send_cmd(base_addr, NOR_CMD_CLEAR_STATUS_REG);

	nor_send_cmd(base_addr, NOR_CMD_LOCK_UNLOCK);
	nor_send_cmd(base_addr, NOR_LOCK_BLOCK);

	ret = nor_poll_dws(base_addr, DWS_WORD_LOCK_RETRIES);
	if (ret == 0)
		ret = nor_full_status_check(base_addr);
	nor_send_cmd(base_addr, NOR_CMD_READ_ARRAY);

	return ret;
}

/*
 * unlock a full 256 block
 * Return values:
 *  0 = success
 *  otherwise it returns a negative value
 */
enum cfi_error_t nor_unlock(uintptr_t base_addr)
{
	enum cfi_error_t ret;

	nor_send_cmd(base_addr, NOR_CMD_CLEAR_STATUS_REG);

	nor_send_cmd(base_addr, NOR_CMD_LOCK_UNLOCK);
	nor_send_cmd(base_addr, NOR_UNLOCK_BLOCK);

	ret = nor_poll_dws(base_addr, DWS_WORD_LOCK_RETRIES);
	if (ret == 0)
		ret = nor_full_status_check(base_addr);
	nor_send_cmd(base_addr, NOR_CMD_READ_ARRAY);

	return ret;
}



unsigned int nor_id_check(uintptr_t base_addr)
{
	unsigned long status,status1,status2;

	nor_send_cmd(base_addr, NOR_CMD_READ_ID_CODE);
	status = mmio_read_32(base_addr);
	status1 = mmio_read_32(base_addr+1*4);
	status2 = mmio_read_32(base_addr+2*4);

  	CFI_FLASH_LOG_MSG("STATUS ID: %X \n",status);
  	CFI_FLASH_LOG_MSG("STATUS ID: %X \n",status1);
  	CFI_FLASH_LOG_MSG("STATUS ID: %X \n",status2);
	status |= status >> 16; /* merge status from both flash banks */

	CFI_FLASH_LOG_MSG("STATUS ID: %X \n",status);

	return status & 0xFFFF;
}

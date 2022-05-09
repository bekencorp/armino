/*
 * This file is part of the EasyFlash Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for stm32f4xx platform.
 * Created on: 2015-01-16
 */
#include <common/bk_include.h>
#include <easyflash.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "bk_flash.h"
#include <os/os.h>
#include "bk_uart.h"
#include "bk_drv_model.h"
#include "flash.h"

/* default ENV set for user */
static const ef_env default_env_set[] = {
	{"user", "user"},
};

static beken_semaphore_t env_cache_lock = NULL;

#ifdef PRINT_DEBUG
static char log_buf[256];
#endif

/**
 * Flash port for hardware initialize.
 *
 * @param default_env default ENV set for user
 * @param default_env_size default ENV size
 *
 * @return result
 */
EfErrCode ef_port_init(ef_env const **default_env, size_t *default_env_size)
{
	bk_err_t ret;
	EfErrCode result = EF_NO_ERR;

	*default_env = default_env_set;
	*default_env_size = sizeof(default_env_set) / sizeof(default_env_set[0]);

	ret = rtos_init_semaphore_adv(&env_cache_lock, 1, 1);
	BK_ASSERT(kNoErr == ret);

	return result;
}

/**
 * Read data from flash.
 * @note This operation's units is word.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 */
EfErrCode ef_port_read(uint32_t addr, uint32_t *buf, size_t size)
{
	EfErrCode result = EF_NO_ERR;

	EF_ASSERT(size % 4 == 0);

	flash_read((char *)buf, (unsigned long)size, addr);

	return result;
}

/*
 * size param: The flash size of you want to erase in bytes.
 * return: Returns the size of the actual erase.
 */
static int bk_erase(uint32_t addr, size_t size)
{
	int param;
	UINT32 status;
	int protect_type;
	DD_HANDLE flash_handle;
	unsigned int _size = size;

	flash_handle = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
	ddev_control(flash_handle, CMD_FLASH_GET_PROTECT, (void *)&protect_type);

	if (FLASH_PROTECT_NONE != protect_type) {
		param = FLASH_PROTECT_NONE;
		ddev_control(flash_handle, CMD_FLASH_SET_PROTECT, (void *)&param);
	}

	/* Calculate the start address of the flash sector(4kbytes) */
	addr = addr & 0x00FFF000;

	do {
		flash_ctrl(CMD_FLASH_ERASE_SECTOR, &addr);
		addr += 4096;

		if (_size < 4096)
			_size = 0;
		else
			_size -= 4096;

	} while (_size);

	if (FLASH_PROTECT_NONE != protect_type) {
		param = protect_type;
		ddev_control(flash_handle, CMD_FLASH_SET_PROTECT, (void *)&param);
	}

	return size; // return true erase size
}

/**
 * Erase data on flash.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
EfErrCode ef_port_erase(uint32_t addr, size_t size)
{
	EfErrCode result = EF_NO_ERR;

	/* make sure the start address is a multiple of FLASH_ERASE_MIN_SIZE */
	EF_ASSERT(addr % EF_ERASE_MIN_SIZE == 0);

	bk_erase(addr, size);

	return result;
}

/**
 * Write data to flash.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result
 */
EfErrCode ef_port_write(uint32_t addr, const uint32_t *buf, size_t size)
{
	int param;
	UINT32 status;
	int protect_type;
	DD_HANDLE flash_handle;
	EfErrCode result = EF_NO_ERR;

	EF_ASSERT(size % 4 == 0);

	flash_handle = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
	ddev_control(flash_handle, CMD_FLASH_GET_PROTECT, (void *)&protect_type);

	if (FLASH_PROTECT_NONE != protect_type) {
		param = FLASH_PROTECT_NONE;
		ddev_control(flash_handle, CMD_FLASH_SET_PROTECT, (void *)&param);
	}

	flash_write((char *)buf, (unsigned long)size, addr);
	if (FLASH_PROTECT_NONE != protect_type) {
		param = protect_type;
		ddev_control(flash_handle, CMD_FLASH_SET_PROTECT, (void *)&param);
	}

	return result;
}

/**
 * lock the ENV ram cache
 */
void ef_port_env_lock(void)
{
	rtos_get_semaphore(&env_cache_lock, BEKEN_WAIT_FOREVER);
}

/**
 * unlock the ENV ram cache
 */
void ef_port_env_unlock(void)
{
	rtos_set_semaphore(&env_cache_lock);
}

/**
 * This function is print flash debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 *
 */
void ef_log_debug(const char *file, const long line, const char *format, ...)
{
#ifdef PRINT_DEBUG
	va_list args;

	/* args point to the first variable parameter */
	va_start(args, format);
	ef_print("[Flash](%s:%ld) ", file, line);
	/* must use vprintf to print */
	vsprintf(log_buf, format, args);
	ef_print("%s", log_buf);
	va_end(args);
#endif
}

/**
 * This function is print flash routine info.
 *
 * @param format output format
 * @param ... args
 */
void ef_log_info(const char *format, ...)
{
#ifdef PRINT_DEBUG
	va_list args;

	/* args point to the first variable parameter */
	va_start(args, format);
	ef_print("[Flash]");
	/* must use vprintf to print */
	vsprintf(log_buf, format, args);
	ef_print("%s", log_buf);
	va_end(args);
#endif
}
/**
 * This function is print flash non-package info.
 *
 * @param format output format
 * @param ... args
 */
void ef_print(const char *format, ...)
{
#ifdef PRINT_DEBUG
	va_list args;

	/* args point to the first variable parameter */
	va_start(args, format);
	/* must use vprintf to print */
	vsprintf(log_buf, format, args);
	os_printf("%s", log_buf);
	va_end(args);
#endif
}
// eof


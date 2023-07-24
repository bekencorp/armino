/*
 * Copyright (c) 2020, Armink, <armink.ztl@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "driver/flash.h"
#include <flashdb.h>

#define FLASH_ERASE_MIN_SIZE    (4 * 1024)

static beken_mutex_t s_lock = NULL;


#define LOCK()                                  \
    do {                                        \
        rtos_lock_mutex(&s_lock);  \
    } while(0)

#define UNLOCK()                                \
    do {                                        \
        rtos_unlock_mutex(&s_lock);                 \
    } while(0)

static int init(void)
{
    if(NULL == s_lock)
    {
        rtos_init_mutex(&s_lock);
    }

    g_flashdb0.len = bk_flash_get_current_total_size();
    
    return 1;
}

static int read(long offset, uint8_t *buf, size_t size)
{
    int32_t ret;

    LOCK();
    ret = bk_flash_read_bytes(offset, buf, (unsigned long)size);
    UNLOCK();

    return ret;
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    int32_t ret;
    
    LOCK();
    ret = bk_flash_write_bytes(offset, buf, size);
    UNLOCK();
    
    return ret;
}

static int _erase(uint32_t addr, size_t size)
{
    flash_protect_type_t protect_type;
    unsigned int _size = size;

    protect_type = bk_flash_get_protect_type();
    if (FLASH_PROTECT_NONE != protect_type) {
        bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    }

    /* Calculate the start address of the flash sector(4kbytes) */
    addr = addr & 0x00FFF000;

    do {
        bk_flash_erase_sector(addr);
        addr += 4096;

        if (_size < 4096)
            _size = 0;
        else
            _size -= 4096;

    } while (_size);

    if (FLASH_PROTECT_NONE != protect_type) {
        bk_flash_set_protect_type(protect_type);
    }

    return size; // return true erase size
}

static int erase(long offset, size_t size)
{
    int32_t ret;
    uint32_t addr = offset;

    int32_t erase_size = ((size - 1) / FLASH_ERASE_MIN_SIZE) + 1;

    LOCK();
    ret = _erase(addr, erase_size);
    UNLOCK();
    
    return ret;
}

struct fal_flash_dev g_flashdb0 =
{
    .name       = FLASHDB_DEV_NAME,
    .addr       = 0,
    .len        = 0,
    .blk_size   = FLASH_ERASE_MIN_SIZE,
    .ops        = {init, read, write, erase},
    .write_gran = 8
};


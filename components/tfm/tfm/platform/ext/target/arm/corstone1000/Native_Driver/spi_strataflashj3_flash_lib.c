/*
 *
 * Copyright (c) 2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "spi_strataflashj3_flash_lib.h"
#include "cfi_drv.h"


#define MAX_PROGRAM_SIZE                128 /* should be less than page size */
#define MAX_READ_SIZE                   128 /* should be less than page size */
#define MANUFACTURER_ID                 0x89
#define DEVICE_CODE                     0x18
#define ERASE_BLOCK_SIZE                256
#define FLASH_END_ADDRESS_8MB           0x00800000
#define FLASH_SECTOR_ERASE_SIZE         0x4000
uint8_t send_buf_strata[256];
uint8_t rcv_buf_strata[256];


enum strataflashj3_error_t cfi_strataflashj3_erase_chip(struct cfi_strataflashj3_dev_t* dev)
{
    enum strataflashj3_error_t ret;
    uint32_t base_addr = dev->controller->cfg->base;
    if (!dev->is_initialized) {
        CFI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return STRATAFLASHJ3_ERR_NOT_INITIALIZED;
    }
    CFI_FLASH_LOG_MSG("%s\n\r", __func__);
    for(uint32_t counter = 0;counter<FLASH_END_ADDRESS_8MB;counter+=FLASH_SECTOR_ERASE_SIZE){
      ret = nor_erase(base_addr+counter);
      if (ret != STRATAFLASHJ3_ERR_NONE) {
          return ret;
      }
    }

    return STRATAFLASHJ3_ERR_NONE;
}

enum strataflashj3_error_t erase_block(struct cfi_strataflashj3_dev_t* dev,uint32_t addr){
  uint32_t base_addr = dev->controller->cfg->base;
  enum strataflashj3_error_t ret;

  for(int counter = 0;counter<ERASE_BLOCK_SIZE;counter++){
    ret = nor_byte_program(base_addr+addr+counter,0xFF);
    if (ret != STRATAFLASHJ3_ERR_NONE) {
        return ret;
    }
  }
  return STRATAFLASHJ3_ERR_NONE;
}

enum strataflashj3_error_t cfi_strataflashj3_erase(struct cfi_strataflashj3_dev_t* dev,
        uint32_t addr)
{
    enum strataflashj3_error_t ret;

    if (!dev->is_initialized) {
        CFI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return STRATAFLASHJ3_ERR_NOT_INITIALIZED;
    }
    CFI_FLASH_LOG_MSG("%s: addr=0x%x\n\r", __func__, addr);
    ret = erase_block(dev,addr);
    if (ret != STRATAFLASHJ3_ERR_NONE) {
        return ret;
    }

    return STRATAFLASHJ3_ERR_NONE;
}

static enum strataflashj3_error_t cfi_strataflashj3_program_data_byte(
                                struct cfi_strataflashj3_dev_t* dev, uint32_t addr,
                                const uint8_t *data, uint32_t cnt)
{
    uint32_t remaining_bytes = cnt;
    uint32_t current_data_index = 0;
    uint32_t current_addr = addr;
    uint32_t write_size;
    uint32_t base_addr = dev->controller->cfg->base;

    while (remaining_bytes) {

        if (remaining_bytes < MAX_PROGRAM_SIZE) {
            write_size = remaining_bytes;
        } else {
            write_size = MAX_PROGRAM_SIZE;
        }
        for (int i = 0; i < write_size; i++) {
            send_buf_strata[i] = data[current_data_index++];
        }
        for(int counter = 0;counter<write_size;counter++){
          nor_byte_program(base_addr+current_addr+(counter),send_buf_strata[counter]);
        }

        remaining_bytes -= write_size;
        current_addr += write_size;
    }

    return STRATAFLASHJ3_ERR_NONE;
}

enum strataflashj3_error_t cfi_strataflashj3_program(
                struct cfi_strataflashj3_dev_t* dev, uint32_t addr,
                const uint8_t *data, uint32_t cnt)
{
    enum strataflashj3_error_t ret;
    uint32_t remaining_space;
    uint32_t current_addr = addr;
    uint8_t *current_data_ptr = (uint8_t*)data;
    uint32_t current_cnt = cnt;

    if (!dev->is_initialized) {
        CFI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return STRATAFLASHJ3_ERR_NOT_INITIALIZED;
    }
    CFI_FLASH_LOG_MSG("%s: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);

    /* Is write alligned? */
    if ((addr % dev->program_unit) != 0) {
        return STRATAFLASHJ3_ERR_WRONG_ARGUMENT;
    }

    /* Check the limits */
    if ((addr + cnt) > (dev->total_sector_cnt * dev->sector_size)) {
        return STRATAFLASHJ3_ERR_WRONG_ARGUMENT;
    }

    if ((addr % MAX_PROGRAM_SIZE) != 0) {
        /* unaligned addr */
        remaining_space = MAX_PROGRAM_SIZE - (addr % MAX_PROGRAM_SIZE);
        if (cnt > remaining_space) {
            /* crossing the page boundary */
            /* first write the unaligned data to make addr aligned*/
            ret = cfi_strataflashj3_program_data_byte(dev, current_addr, current_data_ptr,
                    remaining_space);
            if (ret != STRATAFLASHJ3_ERR_NONE) {
                return ret;
            }
            current_addr += remaining_space;
            current_data_ptr += remaining_space;
            current_cnt -= remaining_space;
        }
    }

    ret = cfi_strataflashj3_program_data_byte(dev, current_addr, current_data_ptr,
            current_cnt);

    return ret;
}

enum strataflashj3_error_t cfi_strataflashj3_read(struct cfi_strataflashj3_dev_t* dev,
                                        uint32_t addr,
                                        uint8_t *data, uint32_t cnt)
{
    uint32_t remaining_bytes = cnt;
    uint32_t current_data_index = 0;
    uint32_t current_addr = addr;
    uint32_t read_size;
    uint32_t base_addr = dev->controller->cfg->base ;

    if (!dev->is_initialized) {
        CFI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return STRATAFLASHJ3_ERR_NOT_INITIALIZED;
    }
    CFI_FLASH_LOG_MSG("%s: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);

    /* Check the limits */
    if(addr == 0xFFFFFFFF){
	return STRATAFLASHJ3_ERR_WRONG_ARGUMENT;
    }
    if ((addr + cnt) > (dev->total_sector_cnt * dev->sector_size)) {
        return STRATAFLASHJ3_ERR_WRONG_ARGUMENT;
    }

    while (remaining_bytes) {

        /* prepare send buffer */
        if (remaining_bytes < MAX_READ_SIZE) {
            read_size = remaining_bytes;
        } else {
            read_size = MAX_READ_SIZE;
        }

        /* copy data to user location */
        for (int counter = 0; counter < read_size; counter++) {
            data[current_data_index] = nor_cfi_reg_read(base_addr + current_addr + (counter));
            current_data_index++;
        }

        remaining_bytes -= read_size;
        current_addr += read_size;
    }

    return STRATAFLASHJ3_ERR_NONE;
}

static enum strataflashj3_error_t cfi_strataflashj3_verify_id(
                                struct cfi_strataflashj3_dev_t* dev)
{
    uint32_t base_addr;
    uint32_t vendor_id, device_code;

    base_addr = dev->controller->cfg->base;

    nor_send_cmd(base_addr, NOR_CMD_READ_ID_CODE);
    vendor_id = nor_cfi_reg_read(base_addr);
    device_code = nor_cfi_reg_read(base_addr+4);

    if(vendor_id != MANUFACTURER_ID || device_code != DEVICE_CODE){
      CFI_FLASH_LOG_MSG("ID miss-match %X-%X\n\r",
                              vendor_id, device_code);
      return STRATAFLASHJ3_ERR_WRONG_MEMORY;
    }

    dev->is_initialized = true;

    CFI_FLASH_LOG_MSG("VENDOR ID: %X \n\r",vendor_id);
    CFI_FLASH_LOG_MSG("STATUS ID: %X \n\r",device_code);

    cfi_strataflashj3_erase(dev,0);

    return STRATAFLASHJ3_ERR_NONE;
}

enum strataflashj3_error_t cfi_strataflashj3_initialize(struct cfi_strataflashj3_dev_t* dev)
{
    enum strataflashj3_error_t flash_ret;

    if (dev->is_initialized) {
        CFI_FLASH_LOG_MSG("%s: already initialized\n\r", __func__);
        return STRATAFLASHJ3_ERR_NONE;
    }

    dev->is_initialized = true;

    CFI_FLASH_LOG_MSG("%s: CFI Flash Lib is initialized.\n\r", __func__);

    return STRATAFLASHJ3_ERR_NONE;
}

void cfi_strataflashj3_uninitialize(struct cfi_strataflashj3_dev_t* dev)
{
    dev->is_initialized = false;
}

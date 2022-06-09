/*
 *
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "spi_n25q256a_flash_lib.h"
#include "spi_flash_commands.h"

#define DUMMY_VAL                       0xFF
#define MAX_PROGRAM_SIZE                128 /* should be less than page size */
#define MAX_READ_SIZE                   128 /* should be less than page size */

#define SR_IS_READY_MASK                0x01
#define FSR_IS_ERASE_ERR_MASK           0x20
#define FSR_IS_PROGRAM_ERR_MASK         0x10

#define MANUFACTURER_ID                 0x20
#define MEMORY_TYPE                     0xBA
#define MEMORY_CAPACITY                 0x19
#define DATA_WIDTH                      0x10

#define CONFIG_REG_VALUE                0xFB

uint8_t send_buf[256];
uint8_t rcv_buf[256];

static enum n25q256a_error_t spi_flash_read_reg(struct spi_n25q256a_dev_t* dev,
        uint32_t *reg, uint32_t cmd)
{
    enum axi_qspi_error_t ret;

    if (!reg) {
        return N25Q256A_ERR_WRONG_ARGUMENT;
    }

    /* Read Flash Status Register */
    send_buf[0] = cmd;
    send_buf[1] = DUMMY_VAL;
    ret = spi_transfer_and_receive(dev->controller, send_buf, rcv_buf, 2);
    if (ret != AXI_QSPI_ERR_NONE) {
        return ret;
    }

    *reg = rcv_buf[1] & 0xFF;

    return N25Q256A_ERR_NONE;
}

static enum n25q256a_error_t spi_flash_wait_for_ready(
        struct spi_n25q256a_dev_t* dev)
{
    uint32_t status_reg;
    enum n25q256a_error_t ret;
    uint32_t counter = 0;

    while (1) {
        /* Get the status register */
        ret = spi_flash_read_reg(dev, &status_reg, CMD_READ_STATUSREG);
        if (ret != N25Q256A_ERR_NONE) {
            return ret;
        }

        if ((status_reg & SR_IS_READY_MASK) == 0) {
            break;
        }

        if ((counter % 1000) == 0) {
            SPI_FLASH_LOG_MSG("%s: WAITING status reg 0x%x\n\r",
                    __func__, status_reg);
            counter++;
        }
    }

    return N25Q256A_ERR_NONE;
}

static enum n25q256a_error_t spi_flash_write_enable(
        struct spi_n25q256a_dev_t* dev)
{
    enum n25q256a_error_t ret;
    enum axi_qspi_error_t qspi_ret;

    ret = spi_flash_wait_for_ready(dev);
    if (ret != N25Q256A_ERR_NONE) {
        return ret;
    }

    send_buf[0] = CMD_WRITE_ENABLE;
    qspi_ret = spi_transfer_and_receive(dev->controller, send_buf, NULL, 1);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
        return qspi_ret;
    }

    return N25Q256A_ERR_NONE;
}

static enum n25q256a_error_t spi_flash_identify_and_clear_error(
                        struct spi_n25q256a_dev_t* dev,
                        uint32_t mask)
{
    enum n25q256a_error_t ret;
    enum axi_qspi_error_t qspi_ret;
    uint32_t flag_reg;

    ret = spi_flash_read_reg(dev, &flag_reg, CMD_READ_FLAG_STATUS);
    if (ret != N25Q256A_ERR_NONE) {
        return ret;
    }

    if ((flag_reg & mask) != 0) {
        SPI_FLASH_LOG_MSG("%s:error identified at mask %x\n\r", __func__, mask);
        send_buf[0] = CMD_CLEAR_FLAG_STATUS;
        send_buf[1] = 0xFF;
        qspi_ret = spi_transfer_and_receive(dev->controller, send_buf, NULL, 2);
        if (qspi_ret != AXI_QSPI_ERR_NONE) {
            return qspi_ret;
        }
        return N25Q256A_ERR_FLASH_CMD_FAILED;
    }

    return N25Q256A_ERR_NONE;
}

enum n25q256a_error_t spi_n25q256a_erase_chip(struct spi_n25q256a_dev_t* dev)
{
    enum n25q256a_error_t ret;
    enum axi_qspi_error_t qspi_ret;

    if (!dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return N25Q256A_ERR_NOT_INITIALIZED;
    }
    SPI_FLASH_LOG_MSG("%s\n\r", __func__);

    ret = spi_flash_write_enable(dev);
    if (ret != N25Q256A_ERR_NONE) {
        return ret;
    }

    send_buf[0] = CMD_BULK_ERASE;

    /* start erase */
    qspi_ret = spi_transfer_and_receive(dev->controller, send_buf, NULL, 1);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
        return qspi_ret;
    }

    /* wait for erase completion */
    ret = spi_flash_wait_for_ready(dev);
    if (ret != N25Q256A_ERR_NONE) {
        return ret;
    }

    /* check for error */
    ret = spi_flash_identify_and_clear_error(dev, FSR_IS_ERASE_ERR_MASK);
    if (ret != N25Q256A_ERR_NONE) {
        return ret;
    }

    return N25Q256A_ERR_NONE;
}

enum n25q256a_error_t spi_n25q256a_erase(struct spi_n25q256a_dev_t* dev,
        uint32_t addr)
{
    enum n25q256a_error_t ret;
    enum axi_qspi_error_t qspi_ret;

    if (!dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return N25Q256A_ERR_NOT_INITIALIZED;
    }
    SPI_FLASH_LOG_MSG("%s: addr=0x%x\n\r", __func__, addr);

    ret = spi_flash_write_enable(dev);
    if (ret != N25Q256A_ERR_NONE) {
        return ret;
    }

    send_buf[0] = CMD_SUB_SECTOR_ERASE;
    send_buf[1] = ((addr >> 24) & 0xFF);
    send_buf[2] = ((addr >> 16) & 0xFF);
    send_buf[3] = ((addr >>  8) & 0xFF);
    send_buf[4] = ((addr >>  0) & 0xFF);

    /* start erase */
    qspi_ret = spi_transfer_and_receive(dev->controller, send_buf, NULL, 5);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
        return qspi_ret;
    }

    /* wait for erase completion */
    ret = spi_flash_wait_for_ready(dev);
    if (ret != N25Q256A_ERR_NONE) {
        return ret;
    }

    /* check for error */
    ret = spi_flash_identify_and_clear_error(dev, FSR_IS_ERASE_ERR_MASK);
    if (ret != N25Q256A_ERR_NONE) {
        return ret;
    }

    return N25Q256A_ERR_NONE;
}

static enum n25q256a_error_t spi_n25q256a_program_data(
                                struct spi_n25q256a_dev_t* dev, uint32_t addr,
                                const uint8_t *data, uint32_t cnt)
{
    enum n25q256a_error_t ret;
    enum axi_qspi_error_t qspi_ret;
    uint32_t remaining_bytes = cnt;
    uint32_t current_data_index = 0;
    uint32_t current_addr = addr;
    uint32_t write_size;

    while (remaining_bytes) {

        /* write enable */
        ret = spi_flash_write_enable(dev);
        if (ret != N25Q256A_ERR_NONE) {
            return ret;
        }

        /* prepare send buffer */
        send_buf[0] = CMD_PAGE_PROGRAM;
        send_buf[1] = ((current_addr >> 24) & 0xFF);
        send_buf[2] = ((current_addr >> 16) & 0xFF);
        send_buf[3] = ((current_addr >>  8) & 0xFF);
        send_buf[4] = ((current_addr >>  0) & 0xFF);

        if (remaining_bytes < MAX_PROGRAM_SIZE) {
            write_size = remaining_bytes;
        } else {
            write_size = MAX_PROGRAM_SIZE;
        }
        for (int i = 0; i < write_size; i++) {
            send_buf[i+5] = data[current_data_index++];
        }

        /* start program */
        qspi_ret = spi_transfer_and_receive(dev->controller, send_buf, NULL,
                write_size + 5);
        if (qspi_ret != AXI_QSPI_ERR_NONE) {
            return qspi_ret;
        }
        /* wait for write completion */
        ret = spi_flash_wait_for_ready(dev);
        if (ret != N25Q256A_ERR_NONE) {
            return ret;
        }
        /* check for error */
        ret = spi_flash_identify_and_clear_error(dev,
                FSR_IS_PROGRAM_ERR_MASK);
        if (ret != N25Q256A_ERR_NONE) {
            return ret;
        }

        remaining_bytes -= write_size;
        current_addr += write_size;
    }

    return N25Q256A_ERR_NONE;
}

enum n25q256a_error_t spi_n25q256a_program(
                struct spi_n25q256a_dev_t* dev, uint32_t addr,
                const uint8_t *data, uint32_t cnt)
{
    enum n25q256a_error_t ret;
    uint32_t remaining_space;
    uint32_t current_addr = addr;
    uint8_t *current_data_ptr = (uint8_t*)data;
    uint32_t current_cnt = cnt;

    if (!dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return N25Q256A_ERR_NOT_INITIALIZED;
    }
    SPI_FLASH_LOG_MSG("%s: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);

    /* Is write alligned? */
    if ((addr % dev->program_unit) != 0) {
        return N25Q256A_ERR_WRONG_ARGUMENT;
    }

    /* Check the limits */
    if ((addr + cnt) > (dev->total_sector_cnt * dev->sector_size)) {
        return N25Q256A_ERR_WRONG_ARGUMENT;
    }

    if ((addr % MAX_PROGRAM_SIZE) != 0) {
        /* unaligned addr */
        remaining_space = MAX_PROGRAM_SIZE - (addr % MAX_PROGRAM_SIZE);
        if (cnt > remaining_space) {
            /* crossing the page boundary */
            /* first write the unaligned data to make addr aligned*/
            ret = spi_n25q256a_program_data(dev, current_addr, current_data_ptr,
                    remaining_space);
            if (ret != N25Q256A_ERR_NONE) {
                return ret;
            }
            current_addr += remaining_space;
            current_data_ptr += remaining_space;
            current_cnt -= remaining_space;
        }
    }

    ret = spi_n25q256a_program_data(dev, current_addr, current_data_ptr,
            current_cnt);

    return ret;
}

enum n25q256a_error_t spi_n25q256a_read(struct spi_n25q256a_dev_t* dev,
                                        uint32_t addr,
                                        uint8_t *data, uint32_t cnt)
{
    enum axi_qspi_error_t qspi_ret;
    uint32_t remaining_bytes = cnt;
    uint32_t current_data_index = 0;
    uint32_t current_addr = addr;
    uint32_t read_size;

    if (!dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return N25Q256A_ERR_NOT_INITIALIZED;
    }
    SPI_FLASH_LOG_MSG("%s: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);

    /* Check the limits */
    if ((addr + cnt) > (dev->total_sector_cnt * dev->sector_size)) {
        return N25Q256A_ERR_WRONG_ARGUMENT;
    }

    while (remaining_bytes) {

        /* prepare send buffer */
        send_buf[0] = CMD_RANDOM_READ;
        send_buf[1] = ((current_addr >> 24) & 0xFF);
        send_buf[2] = ((current_addr >> 16) & 0xFF);
        send_buf[3] = ((current_addr >>  8) & 0xFF);
        send_buf[4] = ((current_addr >>  0) & 0xFF);

        if (remaining_bytes < MAX_READ_SIZE) {
            read_size = remaining_bytes;
        } else {
            read_size = MAX_READ_SIZE;
        }

        /* start reading */
        qspi_ret = spi_transfer_and_receive(dev->controller, send_buf, rcv_buf,
                read_size + 5);
        if (qspi_ret != AXI_QSPI_ERR_NONE) {
            return qspi_ret;
        }
        /* copy data to user location */
        for (int i = 0; i < read_size; i++) {
            data[current_data_index] = rcv_buf[i+5];
            current_data_index++;
        }

        remaining_bytes -= read_size;
        current_addr += read_size;
    }

    return N25Q256A_ERR_NONE;
}

static enum n25q256a_error_t spi_n25q256a_verify_id(
                                struct spi_n25q256a_dev_t* dev)
{
    enum axi_qspi_error_t qspi_ret;

    /* Read ID */
    send_buf[0] = CMD_READ_ID;
    for (int i = 1; i <= 6; i++) {
        send_buf[i] = DUMMY_VAL;
    }

    qspi_ret = spi_transfer_and_receive(dev->controller, send_buf, rcv_buf, 7);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
        return qspi_ret;
    }

    if ((rcv_buf[1] != MANUFACTURER_ID) || (rcv_buf[2] != MEMORY_TYPE) ||
            (rcv_buf[3] != MEMORY_CAPACITY) || (rcv_buf[4] != DATA_WIDTH)) {
        SPI_FLASH_LOG_MSG("ID miss-match %X-%X-%X-%X\n\r",
                                rcv_buf[1], rcv_buf[2], rcv_buf[3], rcv_buf[4]);
        return N25Q256A_ERR_WRONG_MEMORY;
    }
    SPI_FLASH_LOG_MSG("ID register: %X-%X-%X-%X\n\r", rcv_buf[1], rcv_buf[2],
                                        rcv_buf[3], rcv_buf[4]);

    return N25Q256A_ERR_NONE;
}

enum n25q256a_error_t spi_n25q256a_initialize(struct spi_n25q256a_dev_t* dev)
{
    enum axi_qspi_error_t qspi_ret;
    enum n25q256a_error_t flash_ret;
    uint32_t reg;

    if (dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: already initialized\n\r", __func__);
        return N25Q256A_ERR_NONE;
    }

    /* Initialize the QSPI controller */
    qspi_ret = axi_qspi_initialize(dev->controller);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
        return qspi_ret;
    }

    /* Verify device id */
    flash_ret = spi_n25q256a_verify_id(dev);
    if (flash_ret != N25Q256A_ERR_NONE) {
        return flash_ret;
    }

    /* Read Config Register */
    flash_ret = spi_flash_read_reg(dev, &reg, CMD_READ_VOLATILE_CONFIG);
    if (flash_ret != N25Q256A_ERR_NONE) {
        return flash_ret;
    }
    if (reg != CONFIG_REG_VALUE) {
        SPI_FLASH_LOG_MSG("config register is not as expected: %X\n\r", reg);
        return N25Q256A_ERR_WRONG_MEMORY;
    }
    SPI_FLASH_LOG_MSG("Config register value : 0x%X\n\r", reg);

    dev->is_initialized = true;

    SPI_FLASH_LOG_MSG("%s: SPI Flash Lib is initialized.\n\r", __func__);

    return N25Q256A_ERR_NONE;
}

void spi_n25q256a_uninitialize(struct spi_n25q256a_dev_t* dev)
{
    axi_qspi_uninitialize(dev->controller);
    dev->is_initialized = false;
}

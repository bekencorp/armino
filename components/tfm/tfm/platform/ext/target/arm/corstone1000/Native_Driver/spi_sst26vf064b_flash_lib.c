/*
 *
 * Copyright (c) 2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "spi_sst26vf064b_flash_lib.h"

#define CMD_WRITE_ENABLE                0x06
#define CMD_RANDOM_READ                 0x03
#define CMD_FAST_READ                   0x0B
#define CMD_PAGE_PROGRAM                0x02
#define CMD_SECTOR_ERASE                0xD8
#define CMD_SUB_SECTOR_ERASE            0x20
#define CMD_BULK_ERASE                  0xC7
#define CMD_READ_STATUSREG              0x05
#define CMD_READ_ID                     0x9F
#define CMD_WRITE_STATUSREG             0x01
#define CMD_READ_VOLATILE_CONFIG        0x35

#define DUMMY_VAL                       0xFF
#define MAX_PROGRAM_SIZE                128 /* should be less than page size */
#define MAX_READ_SIZE                   128 /* should be less than page size */
#define SR_IS_READY_MASK                0x01
#define FSR_IS_ERASE_ERR_MASK           0x20
#define FSR_IS_PROGRAM_ERR_MASK         0x10
#define MANUFACTURER_ID                 0xBF
#define DEVICE_TYPE                     0x26
#define DEVICE_ID                       0x43
#define DATA_WIDTH                      0x10
#define CONFIG_REG_VALUE                0xFB

#define BUFFER_SIZE 256
#define SPI_READ_REG_SIZE 2
#define  SPI_WRITE_ONE_REG_SIZE 1
#define NUMBER_OF_BLOCKS_TO_UNLOCK 19
#define  SPI_WRITE_ONE_REG_SIZE 1

uint8_t send_buf_flash1[BUFFER_SIZE];
uint8_t rcv_buf_flash1[BUFFER_SIZE];

static enum sst26vf064b_error_t spi_flash_read_reg(struct spi_sst26vf064b_dev_t* dev,
        uint32_t *reg, uint32_t cmd)
{
    enum axi_qspi_error_t ret;

    if (!reg) {
        return SST26VF064B_ERR_WRONG_ARGUMENT;
    }

    /* Read Flash Status Register */
    send_buf_flash1[0] = cmd;
    send_buf_flash1[1] = DUMMY_VAL;
    ret = spi_transfer_and_receive(dev->controller, send_buf_flash1, rcv_buf_flash1, SPI_READ_REG_SIZE);
    if (ret != AXI_QSPI_ERR_NONE) {
        return ret;
    }

    *reg = rcv_buf_flash1[1] & 0xFF;

    return SST26VF064B_ERR_NONE;
}

static enum sst26vf064b_error_t spi_flash_wait_for_ready(
        struct spi_sst26vf064b_dev_t* dev)
{
    uint32_t status_reg = 0;
    enum sst26vf064b_error_t ret;
    uint32_t counter = 0;

    while (1) {
        /* Get the status register */
        ret = spi_flash_read_reg(dev, &status_reg, CMD_READ_STATUSREG);
        if (ret != SST26VF064B_ERR_NONE) {
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

    return SST26VF064B_ERR_NONE;
}

static enum sst26vf064b_error_t spi_flash_write_enable(
        struct spi_sst26vf064b_dev_t* dev)
{
    enum sst26vf064b_error_t ret;
    enum axi_qspi_error_t qspi_ret;

    ret = spi_flash_wait_for_ready(dev);
    if (ret != SST26VF064B_ERR_NONE) {
        return ret;
    }

    send_buf_flash1[0] = CMD_WRITE_ENABLE;
    qspi_ret = spi_transfer_and_receive(dev->controller, send_buf_flash1, NULL, SPI_WRITE_ONE_REG_SIZE);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
        return qspi_ret;
    }

    return SST26VF064B_ERR_NONE;
}


static enum sst26vf064b_error_t spi_flash_unblock(
        struct spi_sst26vf064b_dev_t* dev)
{
    enum sst26vf064b_error_t ret;
    enum axi_qspi_error_t qspi_ret;

    spi_flash_write_enable(dev);
    send_buf_flash1[0] = 0x42;
    for(int counter=0;counter<18;counter++){
      send_buf_flash1[counter + 1] = 0;
    }
    qspi_ret = spi_transfer_and_receive(dev->controller, send_buf_flash1, rcv_buf_flash1, NUMBER_OF_BLOCKS_TO_UNLOCK);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
        return qspi_ret;
    }
    ret = spi_flash_wait_for_ready(dev);
    if (ret != SST26VF064B_ERR_NONE) {
        return ret;
    }

    return SST26VF064B_ERR_NONE;
}

//Read status register. Not used currently. Debug purposes only
static enum sst26vf064b_error_t spi_sst26vf064b_configure_status_reg(struct spi_sst26vf064b_dev_t* dev){
    enum axi_qspi_error_t qspi_ret;
    enum sst26vf064b_error_t flash_ret;
    uint32_t status_reg = 0;

    spi_flash_read_reg(dev, &status_reg, CMD_READ_STATUSREG);
    send_buf_flash1[0] = 0x01;
    send_buf_flash1[1] = status_reg;
    send_buf_flash1[2] = 0x82;

    qspi_ret = spi_transfer_and_receive(dev->controller, send_buf_flash1, NULL, 3);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
        return qspi_ret;
    }
    flash_ret = spi_flash_wait_for_ready(dev);
    if (flash_ret != SST26VF064B_ERR_NONE) {
        return flash_ret;
    }
    return SST26VF064B_ERR_NONE;
}


enum sst26vf064b_error_t spi_sst26vf064b_erase_chip(struct spi_sst26vf064b_dev_t* dev)
{
    enum sst26vf064b_error_t ret;
    enum axi_qspi_error_t qspi_ret;

    if (!dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return SST26VF064B_ERR_NOT_INITIALIZED;
    }
    SPI_FLASH_LOG_MSG("%s\n\r", __func__);

    ret = spi_flash_write_enable(dev);
    if (ret != SST26VF064B_ERR_NONE) {
        return ret;
    }

    send_buf_flash1[0] = CMD_BULK_ERASE;

    /* start erase */
    qspi_ret = spi_transfer_and_receive(dev->controller, send_buf_flash1, NULL, SPI_WRITE_ONE_REG_SIZE);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
        return qspi_ret;
    }

    /* wait for erase completion */
    ret = spi_flash_wait_for_ready(dev);
    if (ret != SST26VF064B_ERR_NONE) {
        return ret;
    }

    return SST26VF064B_ERR_NONE;
}

enum sst26vf064b_error_t spi_sst26vf064b_erase_block(struct spi_sst26vf064b_dev_t* dev,
        uint32_t addr)
{
    if (!dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return SST26VF064B_ERR_NOT_INITIALIZED;
    }
    uint8_t buffer_erase[256];
    for(uint16_t counter = 0; counter<256;counter++){
      buffer_erase[counter] = DUMMY_VAL;
    }
    /*programming a block with 0xFF same as erasing (just to provide functionality to erase smaller areas) */
    spi_sst26vf064b_program(dev,addr,buffer_erase,256);

    return SST26VF064B_ERR_NONE;
}

enum sst26vf064b_error_t spi_sst26vf064b_erase_sector(struct spi_sst26vf064b_dev_t* dev,
        uint32_t addr)
{
    enum sst26vf064b_error_t ret;
    enum axi_qspi_error_t qspi_ret;

    if (!dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return SST26VF064B_ERR_NOT_INITIALIZED;
    }
    SPI_FLASH_LOG_MSG("%s: addr=0x%x\n\r", __func__, addr);

    ret = spi_flash_write_enable(dev);
    if (ret != SST26VF064B_ERR_NONE) {
        return ret;
    }

    send_buf_flash1[0] = CMD_SECTOR_ERASE;
    send_buf_flash1[1] = ((addr & 0xFFFFFF) >> 16);
    send_buf_flash1[2] = ((addr & 0xFFFF) >> 8);
    send_buf_flash1[3] = addr & 0xFF;

    /* start erase */
    qspi_ret = spi_transfer_and_receive(dev->controller, send_buf_flash1, NULL, 4);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
        return qspi_ret;
    }

    /* wait for erase completion */
    ret = spi_flash_wait_for_ready(dev);
    if (ret != SST26VF064B_ERR_NONE) {
        return ret;
    }

    return SST26VF064B_ERR_NONE;
}

static enum sst26vf064b_error_t spi_sst26vf064b_program_data(
                                struct spi_sst26vf064b_dev_t* dev, uint32_t addr,
                                const uint8_t *data, uint32_t cnt)
{
    enum sst26vf064b_error_t ret;
    enum axi_qspi_error_t qspi_ret;
    int32_t remaining_bytes = cnt;
    uint32_t current_data_index = 0;
    uint32_t current_addr = addr;
    uint32_t write_size;

    while (remaining_bytes > 0) {

        /* write enable */
        ret = spi_flash_write_enable(dev);
        if (ret != SST26VF064B_ERR_NONE) {
            return ret;
        }

        /* prepare send buffer */
        send_buf_flash1[0] = CMD_PAGE_PROGRAM;
        send_buf_flash1[1] = ((current_addr >> 16) & 0xFF);
        send_buf_flash1[2] = ((current_addr >>  8) & 0xFF);
        send_buf_flash1[3] = ((current_addr >>  0) & 0xFF);


        if (remaining_bytes < MAX_PROGRAM_SIZE) {
            write_size = remaining_bytes;
        } else {
            write_size = MAX_PROGRAM_SIZE;
        }
        for (int i = 0; i < write_size; i++) {
            send_buf_flash1[i+4] = data[current_data_index++];
        }
        /* start program */
        qspi_ret = spi_transfer_and_receive(dev->controller, send_buf_flash1, NULL,
                write_size + 4);
        if (qspi_ret != AXI_QSPI_ERR_NONE) {
            return qspi_ret;
        }
        /* wait for write completion */
        ret = spi_flash_wait_for_ready(dev);
        if (ret != SST26VF064B_ERR_NONE) {
            return ret;
        }

        remaining_bytes -= write_size;
        current_addr += write_size;
    }

    return SST26VF064B_ERR_NONE;
}

enum sst26vf064b_error_t spi_sst26vf064b_program(
                struct spi_sst26vf064b_dev_t* dev, uint32_t addr,
                const uint8_t *data, uint32_t cnt)
{
    enum sst26vf064b_error_t ret;
    uint32_t remaining_space;
    uint32_t current_addr = addr;
    uint8_t *current_data_ptr = (uint8_t*)data;
    uint32_t current_cnt = cnt;

    if (!dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return SST26VF064B_ERR_NOT_INITIALIZED;
    }
    SPI_FLASH_LOG_MSG("%s: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);

    /* Is write alligned? */
    if ((addr % dev->program_unit) != 0) {
        return SST26VF064B_ERR_WRONG_ARGUMENT;
    }

    /* Check the limits */
    if ((addr + cnt) > (dev->total_sector_cnt * dev->sector_size)) {
        return SST26VF064B_ERR_WRONG_ARGUMENT;
    }

    if ((addr % MAX_PROGRAM_SIZE) != 0) {
        /* unaligned addr */
        remaining_space = MAX_PROGRAM_SIZE - (addr % MAX_PROGRAM_SIZE);
        if (cnt > remaining_space) {
            /* crossing the page boundary */
            /* first write the unaligned data to make addr aligned*/
            ret = spi_sst26vf064b_program_data(dev, current_addr, current_data_ptr,
                    remaining_space);
            if (ret != SST26VF064B_ERR_NONE) {
                return ret;
            }
            current_addr += remaining_space;
            current_data_ptr += remaining_space;
            current_cnt -= remaining_space;
        }
    }

    ret = spi_sst26vf064b_program_data(dev, current_addr, current_data_ptr,
            current_cnt);

    return ret;
}

enum sst26vf064b_error_t spi_sst26vf064b_read(struct spi_sst26vf064b_dev_t* dev,
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
        return SST26VF064B_ERR_NOT_INITIALIZED;
    }
    SPI_FLASH_LOG_MSG("%s: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);

    /* Check the limits */
    if ((addr + cnt) > (dev->total_sector_cnt * dev->sector_size)) {
        return SST26VF064B_ERR_WRONG_ARGUMENT;
    }

    while (remaining_bytes) {

        /* prepare send buffer */
        send_buf_flash1[0] = CMD_RANDOM_READ;
        send_buf_flash1[1] = ((current_addr >> 16) & 0xFF);
        send_buf_flash1[2] = ((current_addr >>  8) & 0xFF);
        send_buf_flash1[3] = ((current_addr >>  0) & 0xFF);

        if (remaining_bytes < MAX_READ_SIZE) {
            read_size = remaining_bytes;
        } else {
            read_size = MAX_READ_SIZE;
        }

        /* start reading */
        qspi_ret = spi_transfer_and_receive(dev->controller, send_buf_flash1, rcv_buf_flash1,
                read_size + 4);
        if (qspi_ret != AXI_QSPI_ERR_NONE) {
            return qspi_ret;
        }
        /* copy data to user location */
        for (int i = 0; i < read_size; i++) {
            data[current_data_index] = rcv_buf_flash1[i+4];
            current_data_index++;
        }

        remaining_bytes -= read_size;
        current_addr += read_size;
    }

    return SST26VF064B_ERR_NONE;
}

static enum sst26vf064b_error_t spi_sst26vf064b_verify_id(
                                struct spi_sst26vf064b_dev_t* dev)
{
    enum axi_qspi_error_t qspi_ret;

    /* Read ID */
    send_buf_flash1[0] = CMD_READ_ID;
    for (int i = 1; i <=3 ; i++) {
        send_buf_flash1[i] = DUMMY_VAL;
    }
    qspi_ret = spi_transfer_and_receive(dev->controller, send_buf_flash1, rcv_buf_flash1, 4);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
        return qspi_ret;
    }

    if ((rcv_buf_flash1[1] != MANUFACTURER_ID) || (rcv_buf_flash1[2] != DEVICE_TYPE) ||
            (rcv_buf_flash1[3] != DEVICE_ID)) {
        SPI_FLASH_LOG_MSG("ID miss-match %X-%X-%X\n\r",
                                rcv_buf_flash1[1], rcv_buf_flash1[2], rcv_buf_flash1[3], rcv_buf_flash1[4]);
        return SST26VF064B_ERR_WRONG_MEMORY;
    }
    SPI_FLASH_LOG_MSG("ID register: %X-%X-%X\n\r", rcv_buf_flash1[1], rcv_buf_flash1[2],
                                        rcv_buf_flash1[3]);

  return SST26VF064B_ERR_NONE;
}

enum sst26vf064b_error_t spi_sst26vf064b_initialize(struct spi_sst26vf064b_dev_t* dev)
{
    enum axi_qspi_error_t qspi_ret;
    enum sst26vf064b_error_t flash_ret;

    if (dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: already initialized\n\r", __func__);
        return SST26VF064B_ERR_NONE;
    }

    /* Initialize the QSPI controller */

    qspi_ret = axi_qspi_initialize(dev->controller);
    if (qspi_ret != AXI_QSPI_ERR_NONE) {
      SPI_FLASH_LOG_MSG("DEBUG inside spi error init");
        return qspi_ret;
    }

    spi_sst26vf064b_configure_status_reg(dev);

    /* Verify device id */
    flash_ret = spi_sst26vf064b_verify_id(dev);
    if (flash_ret != SST26VF064B_ERR_NONE) {
        return flash_ret;
    }
    spi_flash_unblock(dev);

    dev->is_initialized = true;

    SPI_FLASH_LOG_MSG("%s: SPI Flash Lib is initialized.\n\r", __func__);

    return SST26VF064B_ERR_NONE;
}

void spi_sst26vf064b_uninitialize(struct spi_sst26vf064b_dev_t* dev)
{
    axi_qspi_uninitialize(dev->controller);
    dev->is_initialized = false;
}

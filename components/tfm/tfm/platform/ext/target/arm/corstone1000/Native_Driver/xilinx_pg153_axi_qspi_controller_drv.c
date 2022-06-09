/*
 *
 * Copyright (c) 2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "xilinx_pg153_axi_qspi_controller_drv.h"

static void WRITE_REGISTER(uint32_t base, uint32_t offset, uint32_t val)
{
    *((volatile unsigned int*)(base + offset)) = val;
}

/* QSPI controller registers */
typedef struct qspi_controller_registers {
    uint8_t    space_1[28];
    uint32_t   dgier;                    /* 0x1C: Global interrupt enable */
    uint32_t   ipisr;                    /* 0x20: Interrupt status */
    uint8_t    space_2[4];
    uint32_t   ipier;                    /* 0x28: Interrupt enable */
    uint8_t    space_3[20];
    uint32_t   srr;                      /* 0x40: Software reset register */
    uint8_t    space_4[28];
    uint32_t   spi_cr;                   /* 0x60: SPI control */
    uint32_t   spi_sr;                   /* 0x64: SPI status */
    uint32_t   spi_dtr;                  /* 0x68: SPI TX FIFO data */
    uint32_t   spi_drr;                  /* 0x6C: SPI RX FIFO data */
    uint32_t   spi_ssr;                  /* 0x70: SPI slave select */
    uint32_t   spi_txf;                  /* 0x74: SPI TX FIFO occupancy */
    uint32_t   spi_rxf;                  /* 0x78: SPI RX FIFO occupancy */
} qspi_controller_registers_t;

#define MODE_REG_OFFSET   0x08  /* Change controller mode */

/* SCC register control */
#define XIPMODE  0x00  /* XIP controller mode */
#define QSPIMODE 0x01  /* QSPI controller mode */

/* SPI Software Reset Register value. */
#define SRR_RESET_VALUE              0x0000000A

/* SPI Control Register (CR) Masks */
#define CR_ENABLE_MASK         0x00000002 /* System enable */
#define CR_CTRL_MODE_MASK      0x00000004 /* Enable controlling mode */
#define CR_TXFIFO_RESET_MASK   0x00000020 /* Reset transmit FIFO */
#define CR_RXFIFO_RESET_MASK   0x00000040 /* Reset receive FIFO */
#define CR_TRANS_INHIBIT_MASK  0x00000100 /* Transaction inhibit */

#define ENABLE_IER             0x00000004 /* Enable IER */
#define DISABLE_GLOBAL_ITR     0x00000000 /* Disable Global Interrupt */

#define INTR_TX_EMPTY_MASK     0x00000004 /* DTR/TxFIFO is empty */
#define SR_RX_EMPTY_MASK       0x00000001 /* Receive Reg/FIFO is empty */

#define SLAVE_SELECT_MASK      0x00000000
#define SLAVE_DESELECT_MASK    0xFFFFFFFF
#define PAGE_SIZE              256

enum axi_qspi_error_t spi_transfer_and_receive(struct axi_qspi_dev_t* dev,
        uint8_t *send_buffer, uint8_t *rcv_buffer, int bytes)
{
    uint32_t control_reg;
    uint8_t rcv_data;
    int j = 0;
    volatile qspi_controller_registers_t *ctrl_regs =
                                (qspi_controller_registers_t*)dev->cfg->base;

    if (!dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return AXI_QSPI_ERR_NOT_INITIALIZED;
    }

    /* FIFO depth is assumed to be PAGE_SIZE */
    /* Max PAGE bytes */
    if ((bytes == 0) || (bytes > PAGE_SIZE)) {
        return AXI_QSPI_ERR_WRONG_ARGUMENT;
    }

    control_reg = ctrl_regs->spi_cr;
    /* Reset fifo and set controlling mode */
    control_reg |= CR_TXFIFO_RESET_MASK | CR_RXFIFO_RESET_MASK |
                                 CR_ENABLE_MASK | CR_CTRL_MODE_MASK;
    /* Transaction disable */
    control_reg |= CR_TRANS_INHIBIT_MASK;
    ctrl_regs->spi_cr = control_reg;

    /* Write to Data Transmit Register */
    /* Even in case of read, there is a need to write dummy data to
       DTR for read to take place. */
    for (int i = 0; i < bytes; i++) {
        ctrl_regs->spi_dtr = send_buffer[i];
    }

    /* Slave selected */
    ctrl_regs->spi_ssr = SLAVE_SELECT_MASK;

    /* Start the transaction by no longer inhibiting the controller */
    control_reg = ctrl_regs->spi_cr;
    control_reg &= ~CR_TRANS_INHIBIT_MASK;
    ctrl_regs->spi_cr = control_reg;

    /* Polling starts */
    /* Wait for TX FIFO empty */
    while (!(ctrl_regs->ipisr & INTR_TX_EMPTY_MASK));

    /* Clear TX Empty interrupt */
    ctrl_regs->ipisr = INTR_TX_EMPTY_MASK;

    /* Slave de-select */
    ctrl_regs->spi_ssr = SLAVE_DESELECT_MASK;

    /* Loop to read the received data */
    while ((ctrl_regs->spi_sr & SR_RX_EMPTY_MASK) == 0)
    {
        rcv_data = ctrl_regs->spi_drr & 0xFF;
        if (rcv_buffer) {
            rcv_buffer[j++] = rcv_data;
        }
    }

    return AXI_QSPI_ERR_NONE;
}


/**
 * Enable the QSPI controller.
 */
enum axi_qspi_error_t axi_qspi_initialize(struct axi_qspi_dev_t* dev)
{
    volatile qspi_controller_registers_t *ctrl_regs =
                                (qspi_controller_registers_t*)dev->cfg->base;

    if (dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: already initialized\n\r", __func__);
        return AXI_QSPI_ERR_NONE;
    }

    /* Switch to QSPI Controller */
    WRITE_REGISTER(dev->cfg->scc_base, MODE_REG_OFFSET, QSPIMODE);

    /* Reset controller */
    ctrl_regs->srr = SRR_RESET_VALUE;

    /* Enable IER */
    ctrl_regs->ipier = ENABLE_IER;
    /* Disable Global Interrupts */
    ctrl_regs->dgier = DISABLE_GLOBAL_ITR;

    SPI_FLASH_LOG_MSG("%s %s %s QSPI Controller is initialized.\n\r",
            __func__, __DATE__, __TIME__);

    dev->is_initialized = true;
    return AXI_QSPI_ERR_NONE;
}

void axi_qspi_uninitialize(struct axi_qspi_dev_t* dev)
{
    select_xip_mode(dev);
    dev->is_initialized = false;
}

enum axi_qspi_error_t select_xip_mode(struct axi_qspi_dev_t* dev)
{
    if (!dev->is_initialized) {
        return AXI_QSPI_ERR_NOT_INITIALIZED;
    }

    /* Switch to XIP Controller */
    WRITE_REGISTER(dev->cfg->scc_base, MODE_REG_OFFSET, XIPMODE);

    return AXI_QSPI_ERR_NONE;
}

enum axi_qspi_error_t select_qspi_mode(struct axi_qspi_dev_t* dev)
{
    if (!dev->is_initialized) {
        SPI_FLASH_LOG_MSG("%s: not initialized\n\r", __func__);
        return AXI_QSPI_ERR_NOT_INITIALIZED;
    }

    /* Switch to XIP Controller */
    WRITE_REGISTER(dev->cfg->scc_base, MODE_REG_OFFSET, QSPIMODE);
    SPI_FLASH_LOG_MSG("Selecting QSPI flash controller.\n\r");

    return AXI_QSPI_ERR_NONE;
}


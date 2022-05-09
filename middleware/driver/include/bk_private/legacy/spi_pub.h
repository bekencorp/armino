// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_FAILURE                (1)
#define SPI_SUCCESS                (0)

#define SPI_DEV_NAME                "spi"
#define SPI2_DEV_NAME              "spi2"
#define SPI3_DEV_NAME              "spi3"


#define SPI_CMD_MAGIC              (0xe250000)
enum
{
    CMD_SPI_UNIT_ENABLE = SPI_CMD_MAGIC + 1,
    CMD_SPI_SET_MSTEN,
    CMD_SPI_SET_CKPHA,
    CMD_SPI_SET_CKPOL,
    CMD_SPI_SET_BITWIDTH,
    CMD_SPI_SET_NSSMD,
    CMD_SPI_SET_CKR,
    CMD_SPI_RXINT_EN,
    CMD_SPI_TXINT_EN,
    CMD_SPI_RXOVR_EN,
    CMD_SPI_TXOVR_EN,
    CMD_SPI_RXFIFO_CLR,
    CMD_SPI_RXINT_MODE,
    CMD_SPI_TXINT_MODE,
    CMD_SPI_INIT_MSTEN,
    CMD_SPI_GET_BUSY,
    CMD_SPI_SET_RX_CALLBACK,
    CMD_SPI_SET_TX_NEED_WRITE_CALLBACK,
    CMD_SPI_SET_TX_FINISH_CALLBACK,
    CMD_SPI_DEINIT_MSTEN,

    CMD_SPI_LSB_EN,
    CMD_SPI_TX_EN,
    CMD_SPI_RX_EN,
    CMD_SPI_TXFINISH_INT_EN,
    CMD_SPI_RXFINISH_INT_EN,

    CMD_SPI_TXTRANS_LEN,
    CMD_SPI_RXTRANS_LEN,
    CMD_SPI_CS_EN,

	CMD_SPI_TX_RX_EN,
    CMD_SPI_TX_FIFO_CLR,
    CMD_SPI_RX_FIFO_CLR,
};

#define BK_SPI_DEBUG                0
#include "bk_uart.h"
#if BK_SPI_DEBUG
#define BK_SPI_PRT               warning_prf
#define BK_SPI_WPRT              warning_prf
#define BK_SPI_FATAL             fatal_prf
#else
#define BK_SPI_PRT		null_prf
#define BK_SPI_WPRT		null_prf
#define BK_SPI_FATAL		null_prf
#endif

#define USE_SPI_GPIO_14_17          (0)
#define USE_SPI_GPIO_30_33          (1)
#define USE_SPI_GPIO_NUM            USE_SPI_GPIO_14_17
#define SPI_FLASH_WP_GPIO_NUM       (GPIO18)
#define SPI_FLASH_HOLD_GPIO_NUM     (GPIO19)

#define SPI_DEF_CLK_HZ              (10 * 1000 * 1000)
#define TX_FINISH_FLAG              (1 << 31)

#define BK_SPI_CPOL                 0x01
#define BK_SPI_CPHA                 0x02
#define SPI_DEF_MODE                (~((BK_SPI_CPOL)|(BK_SPI_CPHA)))

#define SPI_CPHA     (1<<0)                             /* bit[0]:CPHA, clock phase */
#define SPI_CPOL     (1<<1)                             /* bit[1]:CPOL, clock polarity */

#define SPI_LSB      (0<<2)                             /* bit[2]: 0-LSB */
#define SPI_MSB      (1<<2)                             /* bit[2]: 1-MSB */

#define SPI_MASTER   (0<<3)								/* SPI master device */
#define SPI_SLAVE    (1<<3)								/* SPI slave device */

#define SPI_USE_4_LINE			0			/* 0-4 line mode*/
#define SPI_USE_3_LINE			1			/* 1-3 line mode: no NSS*/
#define SPI_LINE_MODE			SPI_USE_4_LINE		/* default:4 line mode*/

#define SPI_MODE_0       (0 | 0)                        /* CPOL = 0, CPHA = 0 */
#define SPI_MODE_1       (0 | SPI_CPHA)              /* CPOL = 0, CPHA = 1 */
#define SPI_MODE_2       (SPI_CPOL | 0)              /* CPOL = 1, CPHA = 0 */
#define SPI_MODE_3       (SPI_CPOL | SPI_CPHA)    /* CPOL = 1, CPHA = 1 */

#define ENABLE			1
#define DISABLE			0

struct spi_message
{
#if (!CONFIG_SOC_BK7231N) && (!CONFIG_SOC_BK7236) && (!CONFIG_SOC_BK7256XX)
    UINT8 *send_buf;
    UINT32 send_len;

    UINT8 *recv_buf;
    UINT32 recv_len;
#else
    UINT8*send_buf;
    UINT32 send_len;

    UINT8*recv_buf;
    UINT32 recv_len;

#endif
};

/**
 * SPI configuration structure
 */
struct spi_configuration
{
    UINT8 mode;
    UINT8 data_width;
    UINT16 reserved;
    UINT32 max_hz;
};

typedef void (*spi_callback)(int port, void *param);
struct spi_callback_des
{
    spi_callback callback;
    void  *param;
};

/*******************************************************************************
* Function Declarations
*******************************************************************************/
UINT32 spi_read_rxfifo(UINT8 *data);
UINT32 spi_write_txfifo(UINT8 data);

void spi_init(void);
void spi_exit(void);
void spi_isr(void);

#if (CONFIG_SOC_BK7271)
void spi_channel_set(UINT8 channel );
void spi2_init(void);
void spi2_exit(void);
void spi2_isr(void);
void spi3_init(void);
void spi3_exit(void);
void spi3_isr(void);
#endif

/*slave api*/
int bk_spi_slave_init(UINT32 rate, UINT32 mode);
int bk_spi_slave_xfer(struct spi_message *msg);
int bk_spi_slave_deinit(void);

/*master api*/
int bk_spi_master_init(UINT32 rate,UINT32 mode);
int bk_spi_master_xfer(struct spi_message *msg);
int bk_spi_master_deinit(void);

#if CONFIG_SPI_DMA
int bk_spi_slave_dma_init(UINT32 mode, UINT32 rate, struct spi_message *spi_msg);
int bk_spi_slave_dma_transfer(struct spi_message*spi_msg );
int bk_spi_dma_init(UINT32 mode, UINT32 rate, struct spi_message *spi_msg);
int bk_spi_dma_transfer(UINT32 mode, struct spi_message *spi_msg);
int bk_spi_slave_dma_send(struct spi_message *spi_msg);
int bk_spi_slave_dma_tx_init(UINT32 mode, UINT32 rate, struct spi_message *spi_msg);
int bk_spi_dma_deinit(UINT32 mode);


#endif

#ifdef __cplusplus
}
#endif

/*
 * Copyright (c) 2016-2021, Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "device_definition.h"
#include "platform_base_address.h"

#ifdef UART0_PL011
static const struct uart_pl011_dev_cfg_t UART0_PL011_DEV_CFG = {
    .base = CORSTONE1000_UART_BASE,
    .def_baudrate = DEFAULT_UART_BAUDRATE,
    .def_wlen = UART_PL011_WLEN_8,
    .def_parity = UART_PL011_PARITY_DISABLED,
    .def_stopbit = UART_PL011_STOPBIT_1};
static struct uart_pl011_dev_data_t UART0_PL011_DEV_DATA = {
    .state = UART_PL011_UNINITIALIZED,
    .uart_clk = 0,
    .baudrate = 0};
struct uart_pl011_dev_t UART0_PL011_DEV = {&(UART0_PL011_DEV_CFG),
                                           &(UART0_PL011_DEV_DATA)};
#endif

#ifdef MHU1_SE_TO_HOST
struct mhu_v2_x_dev_t MHU1_SE_TO_HOST_DEV = {(CORSTONE1000_SEH_1_SENDER_BASE),
                                             (MHU_V2_X_SENDER_FRAME)};
#endif

#ifdef MHU1_HOST_TO_SE
struct mhu_v2_x_dev_t MHU1_HOST_TO_SE_DEV = {(CORSTONE1000_HSE_1_RECEIVER_BASE),
                                             (MHU_V2_X_RECEIVER_FRAME)};
#endif

/* QSPI driver structures */
#if (defined(SPI_N25Q256A_S) && defined(AXI_QSPI_S))
static const struct axi_qspi_dev_cfg_t AXI_QSPI_DEV_CFG_S = {
    .base = CORSTONE1000_HOST_AXI_QSPI_CTRL_REG_BASE,
    .scc_base = CORSTONE1000_HOST_FPGA_SCC_REGISTERS
};
struct axi_qspi_dev_t AXI_QSPI_DEV_S = {
    .cfg = &AXI_QSPI_DEV_CFG_S,
    .is_initialized = false
};
#endif

#if (defined (SPI_STRATAFLASHJ3_S) && defined (CFI_S))
static const struct cfi_dev_cfg_t CFI_DEV_CFG_S = {
    .base = CORSTONE1000_HOST_XNVM_BASE
};
struct cfi_dev_t CFI_DEV_S = {
    .cfg = &CFI_DEV_CFG_S,
};
static const struct cfi_dev_cfg_t CFI_DEV_CFG_SE_SECURE_FLASH_S = {
    .base = CORSTONE1000_HOST_SE_SECURE_FLASH_BASE_FVP
};
struct cfi_dev_t CFI_DEV_SE_SECURE_FLASH_S = {
    .cfg = &CFI_DEV_CFG_SE_SECURE_FLASH_S,
};
#endif

#if (defined (SPI_SST26VF064B_S) && defined (AXI_QSPI_S))
static const struct axi_qspi_dev_cfg_t AXI_QSPI_DEV_2_CFG_S = {
    .base = CORSTONE1000_HOST_AXI_QSPI_CTRL_REG_BASE_SE_SECURE_FLASH,
    .scc_base = CORSTONE1000_HOST_FPGA_SCC_REGISTERS
};
struct axi_qspi_dev_t AXI_QSPI_DEV_2_S = {
    .cfg = &AXI_QSPI_DEV_2_CFG_S,
    .is_initialized = false
};
#endif

/* ======= External peripheral configuration structure definitions ======= */

#if (defined(SPI_N25Q256A_S) && defined(AXI_QSPI_S))
struct spi_n25q256a_dev_t SPI_N25Q256A_DEV = {
    .controller = &AXI_QSPI_DEV_S,
    .total_sector_cnt = 0,
    .page_size = 0,
    .sector_size = 0,
    .program_unit = 0,
    .is_initialized = false
};
#endif

#if (defined(SPI_STRATAFLASHJ3_S) && defined(CFI_S))
struct cfi_strataflashj3_dev_t SPI_STRATAFLASHJ3_DEV = {
    .controller = &CFI_DEV_S,
    .total_sector_cnt = 0,
    .page_size = 0,
    .sector_size = 0,
    .program_unit = 0,
    .is_initialized = false
};

struct cfi_strataflashj3_dev_t SPI_STRATAFLASHJ3_DEV_SE_SECURE_FLASH = {
    .controller = &CFI_DEV_SE_SECURE_FLASH_S,
    .total_sector_cnt = 0,
    .page_size = 0,
    .sector_size = 0,
    .program_unit = 0,
    .is_initialized = false
};
#endif

#if (defined (SPI_SST26VF064B_S) && defined (AXI_QSPI_S))
struct spi_sst26vf064b_dev_t SPI_SST26VF064B_DEV = {
    .controller = &AXI_QSPI_DEV_2_S,
    .total_sector_cnt = 0,
    .page_size = 0,
    .sector_size = 0,
    .program_unit = 0,
    .is_initialized = false
};
#endif

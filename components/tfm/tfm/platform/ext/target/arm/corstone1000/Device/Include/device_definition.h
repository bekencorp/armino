/*
 * Copyright (c) 2017-2021 Arm Limited. All rights reserved.
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

/**
 * \file device_definition.h
 * \brief The structure definitions in this file are exported based on the
 * peripheral definitions from device_cfg.h.
 * This file is meant to be used as a helper for baremetal
 * applications and/or as an example of how to configure the generic
 * driver structures.
 */

#ifndef __DEVICE_DEFINITION_H__
#define __DEVICE_DEFINITION_H__

#include "device_cfg.h"

/** ARM UART PL011 driver structures */
#ifdef UART0_PL011
#include "uart_pl011_drv.h"
extern struct uart_pl011_dev_t UART0_PL011_DEV;
#endif

/** ARM MHU1 SE to Host driver structures */
#ifdef MHU1_SE_TO_HOST
#include "mhu_v2_x.h"
extern struct mhu_v2_x_dev_t MHU1_SE_TO_HOST_DEV;
#endif

/** ARM MHU1 Host to SE driver structures */
#ifdef MHU1_HOST_TO_SE
#include "mhu_v2_x.h"
extern struct mhu_v2_x_dev_t MHU1_HOST_TO_SE_DEV;
#endif

/* QSPI Flash Controller driver structures  */
#if (defined(SPI_N25Q256A_S) && defined(AXI_QSPI_S))
#include "xilinx_pg153_axi_qspi_controller_drv.h"
extern struct axi_qspi_dev_t AXI_QSPI_DEV_S;
#endif

/* QSPI Flash Controller driver structures for SE FLASH */
#if (defined(SPI_SST26VF064B_S) && defined(AXI_QSPI_S))
#include "xilinx_pg153_axi_qspi_controller_drv.h"
extern struct axi_qspi_dev_t AXI_QSPI_DEV_2_S;
#endif

/* CFI  Controller driver structures  */
#if (defined(SPI_STRATAFLASHJ3_S) && defined(AXI_QSPI_S))
#include "cfi_drv.h"
extern struct cfi_dev_t CFI_DEV_S;
extern struct cfi_dev_t CFI_DEV_SE_SECURE_FLASH_S;
#endif

/* PMOD SF3 Nor Flash N25Q256A driver structures */
#if (defined(SPI_N25Q256A_S) && defined(AXI_QSPI_S))
#include "spi_n25q256a_flash_lib.h"
extern struct spi_n25q256a_dev_t SPI_N25Q256A_DEV;
#endif

/* SST26VF064B Flash driver structures */
#if (defined(SPI_SST26VF064B_S) && defined(AXI_QSPI_S))
#include "spi_sst26vf064b_flash_lib.h"
extern struct spi_sst26vf064b_dev_t SPI_SST26VF064B_DEV;
#endif

/* Intel Flash StrataJ3 driver structures */
#if (defined(SPI_STRATAFLASHJ3_S) && defined(CFI_S))
#include "spi_strataflashj3_flash_lib.h"
extern struct cfi_strataflashj3_dev_t SPI_STRATAFLASHJ3_DEV;
extern struct cfi_strataflashj3_dev_t SPI_STRATAFLASHJ3_DEV_SE_SECURE_FLASH;
#endif

#endif  /* __DEVICE_DEFINITION_H__ */

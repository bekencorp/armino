/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CMSIS_DRIVER_CONFIG_H__
#define __CMSIS_DRIVER_CONFIG_H__

#include "device_cfg.h"
#include "device_definition.h"
#include "platform_description.h"
#include "RTE_Device.h"

#define UART0_DEV           UART0_PL011_DEV

/* Externally attached PMOD SF3 Nor Flash Device to the MPS3 Board. */
#if PLATFORM_IS_FVP
/* Intel Strata Flash (32MB NVM FLASH). */
#define FLASH0_DEV          SPI_STRATAFLASHJ3_DEV
/* Intel Strata Flash (8MB SE FLASH). */
#define FLASH1_DEV          SPI_STRATAFLASHJ3_DEV_SE_SECURE_FLASH
#else
/* Externally attached PMOD SF3 Nor Flash Device to the MPS3 Board. */
#define FLASH0_DEV          SPI_N25Q256A_DEV
/* SST26VF064B Flash Device (8MB SE FLASH). */
#define FLASH1_DEV          SPI_SST26VF064B_DEV
#endif

#endif  /* __CMSIS_DRIVER_CONFIG_H__ */

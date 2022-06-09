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

#ifndef __DEVICE_CFG_H__
#define __DEVICE_CFG_H__

/*ARM UART Controller PL011*/
#define UART0_PL011

/* Default UART baud rate */
#define DEFAULT_UART_BAUDRATE 115200

/*ARM MHU1 SE to Host*/
#define MHU1_SE_TO_HOST

/*ARM MHU1 Host to SE*/
#define MHU1_HOST_TO_SE

/* External PMOD SF3 Flash Device */
#define SPI_N25Q256A_S

/*External Intel Strata Flash Device*/
#define SPI_STRATAFLASHJ3_S

/* External SST26VF064B Flash Device */
#define SPI_SST26VF064B_S

/* AXI QSPI Controller */
#define AXI_QSPI_S

/* CFI Controller */
#define CFI_S


#endif  /* __DEVICE_CFG_H__ */

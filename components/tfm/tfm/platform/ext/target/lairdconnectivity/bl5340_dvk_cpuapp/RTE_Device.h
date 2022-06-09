/*
 * Copyright (c) 2019 Arm Limited. All rights reserved.
 * Copyright (c) 2020 Nordic Semiconductor ASA. All rights reserved.
 * Copyright (c) 2021 Laird Connectivity. All rights reserved.
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

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

// <e> USART (Universal synchronous - asynchronous receiver transmitter) [Driver_USART0]
// <i> Configuration settings for Driver_USART0 in component ::Drivers:USART
#define   RTE_USART0                    1
//   <h> Pin Selection (0xFFFFFFFF means Disconnected)
//     <o> TXD
#define   RTE_USART0_TXD_PIN            20
//     <o> RXD
#define   RTE_USART0_RXD_PIN            22
//     <o> RTS
#define   RTE_USART0_RTS_PIN            19
//     <o> CTS
#define   RTE_USART0_CTS_PIN            21
//   </h> Pin Configuration
// </e> USART (Universal synchronous - asynchronous receiver transmitter) [Driver_USART0]

// <e> USART (Universal synchronous - asynchronous receiver transmitter) [Driver_USART1]
// <i> Configuration settings for Driver_USART1 in component ::Drivers:USART
#define   RTE_USART1                    1
//   <h> Pin Selection (0xFFFFFFFF means Disconnected)
//     <o> TXD
#define   RTE_USART1_TXD_PIN            40
//     <o> RXD
#define   RTE_USART1_RXD_PIN            42
//     <o> RTS
#define   RTE_USART1_RTS_PIN            39
//     <o> CTS
#define   RTE_USART1_CTS_PIN            41
//   </h> Pin Configuration
// </e> USART (Universal synchronous - asynchronous receiver transmitter) [Driver_USART1]

// <e> USART (Universal synchronous - asynchronous receiver transmitter) [Driver_USART2]
// <i> Configuration settings for Driver_USART2 in component ::Drivers:USART
#define   RTE_USART2                    0
//   <h> Pin Selection (0xFFFFFFFF means Disconnected)
//     <o> TXD
#define   RTE_USART2_TXD_PIN            0xFFFFFFFF
//     <o> RXD
#define   RTE_USART2_RXD_PIN            0xFFFFFFFF
//     <o> RTS
#define   RTE_USART2_RTS_PIN            0xFFFFFFFF
//     <o> CTS
#define   RTE_USART2_CTS_PIN            0xFFFFFFFF
//   </h> Pin Configuration
// </e> USART (Universal synchronous - asynchronous receiver transmitter) [Driver_USART2]

// <e> USART (Universal synchronous - asynchronous receiver transmitter) [Driver_USART3]
// <i> Configuration settings for Driver_USART3 in component ::Drivers:USART
#define   RTE_USART3                    0
//   <h> Pin Selection (0xFFFFFFFF means Disconnected)
//     <o> TXD
#define   RTE_USART3_TXD_PIN            0xFFFFFFFF
//     <o> RXD
#define   RTE_USART3_RXD_PIN            0xFFFFFFFF
//     <o> RTS
#define   RTE_USART3_RTS_PIN            0xFFFFFFFF
//     <o> CTS
#define   RTE_USART3_RTS_PIN            0xFFFFFFFF
//   </h> Pin Configuration
// </e> USART (Universal synchronous - asynchronous receiver transmitter) [Driver_USART3]

// <e> TWIM (Two-wire interface master) [Driver_TWIM2]
// <i> Configuration settings for Driver_TWIM2 in component ::Drivers:TWIM
#define RTE_TWIM2                      1
//   <h> Pin Selection (0xFFFFFFFF means Disconnected)
//     <o> SCL
#define   RTE_TWIM2_SCL_PIN            35
//     <o> SDA
#define   RTE_TWIM2_SDA_PIN            34
//   </h> Pin Configuration
// </e> TWIM (Two-wire interface master) [Driver_TWIM2]

// <e> QSPI (Quad serial peripheral interface) [Driver_QSPI0]
// <i> Configuration settings for Driver_QSPI0 in component ::Drivers:QSPI
#define RTE_QSPI0                      1
//   <h> Pin Selection (0xFFFFFFFF means Disconnected)
//     <o> IO0
#define   RTE_QSPI0_IO0_PIN            13
//     <o> IO1
#define   RTE_QSPI0_IO1_PIN            14
//     <o> IO2
#define   RTE_QSPI0_IO2_PIN            15
//     <o> IO3
#define   RTE_QSPI0_IO3_PIN            16
//     <o> SCL
#define   RTE_QSPI0_SCL_PIN            17
//     <o> CSN
#define   RTE_QSPI0_CSN_PIN            18
//   </h> Pin Configuration
// </e> QSPI (Quad serial peripheral interface) [Driver_QSPI0]

// <e> FLASH (Flash Memory) [Driver_FLASH0]
// <i> Configuration settings for Driver_FLASH0 in component ::Drivers:FLASH
#define   RTE_FLASH0                    1
// </e> FLASH (Flash Memory) [Driver_FLASH0]

#endif  /* __RTE_DEVICE_H */

/*
 * Copyright (c) 2019 Arm Limited. All rights reserved.
 * Copyright (c) 2020 Nordic Semiconductor ASA. All rights reserved.
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
#define   RTE_USART0_TXD_PIN            29
//     <o> RXD
#define   RTE_USART0_RXD_PIN            28
//     <o> RTS
#define   RTE_USART0_RTS_PIN            27
//     <o> CTS
#define   RTE_USART0_CTS_PIN            26
//   </h> Pin Configuration
// </e> USART (Universal synchronous - asynchronous receiver transmitter) [Driver_USART0]

// <e> USART (Universal synchronous - asynchronous receiver transmitter) [Driver_USART1]
// <i> Configuration settings for Driver_USART1 in component ::Drivers:USART
#define   RTE_USART1                    1
//   <h> Pin Selection (0xFFFFFFFF means Disconnected)
//     <o> TXD
#define   RTE_USART1_TXD_PIN            1
//     <o> RXD
#define   RTE_USART1_RXD_PIN            0
//     <o> RTS
#define   RTE_USART1_RTS_PIN            0xFFFFFFFF
//     <o> CTS
#define   RTE_USART1_CTS_PIN            0xFFFFFFFF
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

// <e> FLASH (Flash Memory) [Driver_FLASH0]
// <i> Configuration settings for Driver_FLASH0 in component ::Drivers:FLASH
#define   RTE_FLASH0                    1
// </e> FLASH (Flash Memory) [Driver_FLASH0]

#endif  /* __RTE_DEVICE_H */

/*
 * Copyright (c) 2020 Arm Limited. All rights reserved.
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

#ifndef __MUSCA_B1_SECURE_ENCLAVE_BASE_ADDRESS_H__
#define __MUSCA_B1_SECURE_ENCLAVE_BASE_ADDRESS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Peripheries from Secure Enclave subsystem */
#define MHU0_SENDER_FRAME_BASE                    (0x50003000UL)
#define MHU0_RECEIVER_FRAME_BASE                  (0x50004000UL)

/* Peripheries remapped from SSE-200 subsystem with SYS remap
 * SYS remap offset should be set to 0x5200_0000
 * SYS remap mask   should be set to 0x00FF_FFFF */
#define REMAPPED_MUSCA_B1_EFLASH0_REG_MAP_S_BASE  (0xA0400000UL)
#define REMAPPED_MUSCA_B1_EFLASH1_REG_MAP_S_BASE  (0xA0500000UL)
#define REMAPPED_MUSCA_B1_QSPI_REG_S_BASE         (0xA0800000UL)

#ifdef __cplusplus
}
#endif

#endif  /* __MUSCA_B1_SECURE_ENCLAVE_BASE_ADDRESS_H__ */

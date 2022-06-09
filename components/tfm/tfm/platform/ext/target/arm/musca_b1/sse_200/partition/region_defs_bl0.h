/*
 * Copyright (c) 2017-2020 Arm Limited. All rights reserved.
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

#ifndef __REGION_DEFS_BL0_H__
#define __REGION_DEFS_BL0_H__

#define BL0_HEAP_SIZE           (0x0000100)
#define BL0_MSP_STACK_SIZE      (0x0000800)

/* Code SRAM area */
#define S_CODE_SRAM_ALIAS_BASE   (0x1A400000)

/* SRAM area */
#define S_RAM_ALIAS_BASE         (0x30000000)

/* FLASH area */
#define FLASH_BASE_ADDRESS       (0x1A000000)

/* BL0 preloader regions */
#define BL0_FLASH_BASE           FLASH_BASE_ADDRESS
#define BL0_FLASH_SIZE           (0x00002000) /* 8 KB */
#define BL0_CODE_SRAM_BASE       S_CODE_SRAM_ALIAS_BASE
#define BL0_CODE_SRAM_SIZE       (0x00002000) /* 8 KB */
#define BL0_DATA_BASE            S_RAM_ALIAS_BASE
#define BL0_DATA_SIZE            (0x00004000) /* 16 KB */

#endif /* __REGION_DEFS_BL0_H__ */

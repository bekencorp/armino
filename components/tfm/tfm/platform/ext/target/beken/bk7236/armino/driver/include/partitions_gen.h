// Copyright 2022-2023 Beken
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

//This is a generated file, don't modify it!

#pragma once


#define KB(size)                                      ((size) << 10)
#define MB(size)                                      ((size) << 20)

#define CONFIG_CODE_ENCRYPTED                         0
#define CONFIG_CODE_HAS_CRC                           1
#define FLASH_VIRTUAL2PHY(virtual_addr)               ((((virtual_addr) >> 5) * 34) + ((virtual_addr) & 31))
#define FLASH_PHY2VIRTUAL(phy_addr)                   ((((phy_addr) / 34) << 5) + ((phy_addr) % 34))
#define CPU_VECTOR_ALIGN_SZ                           128

#define FLASH_CEIL_ALIGN(v, align) ((((v) + ((align) - 1)) / (align)) * (align))
#define FLASH_PHY2VIRTUAL_CODE_START(phy_addr) FLASH_CEIL_ALIGN(FLASH_PHY2VIRTUAL(FLASH_CEIL_ALIGN((phy_addr), 34)), CPU_VECTOR_ALIGN_SZ)

#define CONFIG_PRIMARY_S_PHY_PARTITION_OFFSET         0x0
#define CONFIG_PRIMARY_S_PHY_PARTITION_SIZE           0x77000
#define CONFIG_PRIMARY_S_PHY_CODE_START               0x0
#define CONFIG_PRIMARY_S_VIRTUAL_PARTITION_SIZE       0x70000
#define CONFIG_PRIMARY_S_VIRTUAL_CODE_START           0x0
#define CONFIG_PRIMARY_S_VIRTUAL_CODE_SIZE            0x6ffe0

#define CONFIG_PRIMARY_NS_PHY_PARTITION_OFFSET        0x77000
#define CONFIG_PRIMARY_NS_PHY_PARTITION_SIZE          0x40000
#define CONFIG_PRIMARY_NS_PHY_CODE_START              0x77000
#define CONFIG_PRIMARY_NS_VIRTUAL_PARTITION_SIZE      0x3c3c0
#define CONFIG_PRIMARY_NS_VIRTUAL_CODE_START          0x70000
#define CONFIG_PRIMARY_NS_VIRTUAL_CODE_SIZE           0x3c3a0

#define CONFIG_PRIMARY_APP_PHY_PARTITION_OFFSET       0xb7000
#define CONFIG_PRIMARY_APP_PHY_PARTITION_SIZE         0xff000
#define CONFIG_PRIMARY_APP_PHY_CODE_START             0xb7040
#define CONFIG_PRIMARY_APP_VIRTUAL_PARTITION_SIZE     0xf0000
#define CONFIG_PRIMARY_APP_VIRTUAL_CODE_START         0xac400
#define CONFIG_PRIMARY_APP_VIRTUAL_CODE_SIZE          0xeffa0


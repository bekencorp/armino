// Copyright 2023-2028 Beken
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


#include "bk7236xx.h"
#include "os/os.h"
#include "common/bk_assert.h"

#include "sdkconfig.h"

#define MPU_MAX_NUM_REGIONS     (8UL)                   /* 支持的最大区域数量 */
#define MPU_MAX_NUM_ATTRS       (8UL)                   /* 支持的最大区域数量 */


/*
* #define ARM_MPU_RBAR(BASE, SH, RO, NP, XN)
*
* \brief Region Base Address Register value
* \param BASE The base address bits [31:5] of a memory region. The value is zero extended. Effective address gets 32 byte aligned.
* \param SH Defines the Shareability domain for this memory region.
* \param RO Read-Only: Set to 1 for a read-only memory region.
* \param NP Non-Privileged: Set to 1 for a non-privileged memory region.
* \oaram XN eXecute Never: Set to 1 for a non-executable memory region.
*/
ARM_MPU_Region_t mpu_regions[] = {
    /* Config MPU regions */

    /* MPU region 0，itcm/flash code */
    { ARM_MPU_RBAR(0x00000000UL, ARM_MPU_SH_NON, 1, 1, 0),
      ARM_MPU_RLAR(0x07FFFFE0UL, 0) },

    /* MPU region 1，sram cache data */
    { ARM_MPU_RBAR(0x08000000UL, ARM_MPU_SH_NON, 0, 1, 0),
      ARM_MPU_RLAR(0x1FFFFFE0UL, 0) },

    /* MPU region 2，dtcm data */
    { ARM_MPU_RBAR(0x20000000UL, ARM_MPU_SH_NON, 0, 1, 1),
      ARM_MPU_RLAR(0x27FFFFE0UL, 2) },

    // /* MPU region 3，sram nocache data */
    // { ARM_MPU_RBAR(0x28000000UL, ARM_MPU_SH_NON, 0, 1, 0),
    //   ARM_MPU_RLAR(0x2803FFE0UL, 1) },   //0x2803FFE0UL

    /* MPU region 3，sram nocache data */
    { ARM_MPU_RBAR(0x28000000UL, ARM_MPU_SH_NON, 0, 1, 0),
      ARM_MPU_RLAR(0x3FFFFFE0UL, 2) },

    /* MPU region 4，Peripheral data */
    { ARM_MPU_RBAR(0x40000000UL, ARM_MPU_SH_NON, 0, 1, 1),
      ARM_MPU_RLAR(0x5FFFFFE0UL, 4) },

     /* MPU region 5，external ram data */
     { ARM_MPU_RBAR(0x60000000UL, ARM_MPU_SH_NON, 0, 1, 0),
       ARM_MPU_RLAR(0x606FFE0UL, 2) },

     /* MPU region 6，external ram data */
     { ARM_MPU_RBAR(0x60700000UL, ARM_MPU_SH_NON, 0, 1, 0),
       ARM_MPU_RLAR(0x9FFFFFE0UL, 6) },

     /* MPU region 7，external device memory */
     { ARM_MPU_RBAR(0xA0000000UL, ARM_MPU_SH_OUTER, 0, 1, 0),
       ARM_MPU_RLAR(0xFFFFFFE0UL, 2) }
};

uint8_t mpu_attrs[] = {
    ARM_MPU_ATTR(0xb, 0xb), // Normal memory, cacheable write through, read allocate, write allocate
    ARM_MPU_ATTR(0xb, 0xb), // Normal memory, cacheable write through, read allocate, write allocate
    ARM_MPU_ATTR(0x4, 0x4), // Normal memory, non-cacheable
    ARM_MPU_ATTR(0x4, 0x4), // Normal memory, non-cacheable
    ARM_MPU_ATTR(0x0, 0x1), // ARM_MPU_ATTR_DEVICE
    ARM_MPU_ATTR(0x0, 0x1), // ARM_MPU_ATTR_DEVICE
    ARM_MPU_ATTR(0xf, 0xf), // Normal memory, cacheable write back, read allocate, write allocate
    ARM_MPU_ATTR(0xf, 0xf)  // Normal memory, cacheable write back, read allocate, write allocate
};


void mpu_clear(uint32_t rnr) {
    ARM_MPU_Disable();
    ARM_MPU_ClrRegion(rnr);
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);
}

void mpu_cfg(uint32_t rnr, uint32_t rbar, uint32_t rlar) {
    ARM_MPU_Disable();
    ARM_MPU_SetRegion(rnr, rbar, rlar);
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);
    SCB_CleanInvalidateDCache();
}

void mpu_enable(void)
{
    uint32_t region_num = sizeof(mpu_regions)/sizeof(mpu_regions[0]);
    uint32_t attr_num = sizeof(mpu_attrs)/sizeof(mpu_attrs[0]);

    ARM_MPU_Disable();

    if (region_num > MPU_MAX_NUM_REGIONS){
        region_num = MPU_MAX_NUM_REGIONS;
    }

    for (int i = 0; i < region_num; i++) {
        ARM_MPU_SetRegion(i, mpu_regions[i].RBAR, mpu_regions[i].RLAR);
    }

    for (int j = 0; j < attr_num; j++) {
        ARM_MPU_SetMemAttr(j, mpu_attrs[j]);
    }

    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);

}

void mpu_disable(void) {
    ARM_MPU_Disable();
    for (uint32_t i = 0; i < MPU_MAX_NUM_REGIONS; i++) {
      ARM_MPU_ClrRegion(i);
    }
}

void mpu_dump(void) {

    BK_DUMP_OUT("******************** Dump mpu config begin ********************\r\n");
    BK_DUMP_OUT("MPU->TYPE: 0x%08X.\r\n", MPU->TYPE);
    BK_DUMP_OUT("MPU->CTRL: 0x%08X.\r\n", MPU->CTRL);
    for (uint32_t i = 0; i < MPU_MAX_NUM_REGIONS; i++) {
        MPU->RNR = i;
        BK_DUMP_OUT("MPU->RNR: %d.\r\n", MPU->RNR);
        BK_DUMP_OUT("MPU->RBAR: 0x%08X.\r\n", MPU->RBAR);
        BK_DUMP_OUT("MPU->RLAR: 0x%08X.\r\n", MPU->RLAR);
        BK_DUMP_OUT("MPU->RBAR_A1: 0x%08X.\r\n", MPU->RBAR_A1);
        BK_DUMP_OUT("MPU->RLAR_A1: 0x%08X.\r\n", MPU->RLAR_A1);
        BK_DUMP_OUT("MPU->RBAR_A2: 0x%08X.\r\n", MPU->RBAR_A2);
        BK_DUMP_OUT("MPU->RLAR_A2: 0x%08X.\r\n", MPU->RLAR_A2);
        BK_DUMP_OUT("MPU->RBAR_A3: 0x%08X.\r\n", MPU->RBAR_A3);
        BK_DUMP_OUT("MPU->RLAR_A3: 0x%08X.\r\n", MPU->RLAR_A3);
        BK_DUMP_OUT("MPU->MAIR0: 0x%08X.\r\n", MPU->MAIR0);
        BK_DUMP_OUT("MPU->MAIR1: 0x%08X.\r\n", MPU->MAIR1);
    }
    BK_DUMP_OUT("******************** Dump mpu config begin ********************\r\n");

}


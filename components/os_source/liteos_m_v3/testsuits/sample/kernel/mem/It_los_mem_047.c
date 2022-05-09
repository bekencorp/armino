/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "osTest.h"
#include "It_los_mem.h"

#if (LOSCFG_MEM_MUL_REGIONS == 1)

// simulate two non-continuous memory regions
STATIC UINT8 g_memGap_TC47_Gap1[0x10];
STATIC UINT8 g_memPool_TC47_01[0x1000];
STATIC UINT8 g_memGap_TC47_Gap2[0x10];
STATIC UINT8 g_memPool_TC47_02[0x400];

static UINT32 TestCase(VOID)
{
    UINT32 ret;
    UINT32 size;
    void *p = NULL;
    LosMemRegion memRegions[] =
        {
            {g_memPool_TC47_01, 0x1000},
            {g_memPool_TC47_02, 0x400}
        };

    // Initialize the LOS_MemRegionsAdd
    ret = LOS_MemRegionsAdd(NULL, memRegions, sizeof(memRegions) / sizeof(memRegions[0]));
    if (ret != LOS_OK) {
        return ret;
    }

    // p points to the start address of the gap node between g_memPool_TC47_01 and g_memPool_TC47_02
    p = g_memPool_TC47_01 + 0x1000;
    ret = LOS_MemFree((void *)&g_memPool_TC47_01, p);
    ICUNIT_ASSERT_EQUAL(ret, LOS_NOK, ret);

    size = 0x100;
    p = LOS_MemAlloc((void *)&g_memPool_TC47_01, size);
    ICUNIT_ASSERT_NOT_EQUAL(p, NULL, 0);

    (void)memset_s(p, size, 1, size);
    (void)memset_s(g_memGap_TC47_Gap1, 0x10, 1, 0x10);
    (void)memset_s(g_memGap_TC47_Gap2, 0x10, 1, 0x10);

    ret = LOS_MemFree((void *)&g_memPool_TC47_01, p);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, 0);
    return LOS_OK;
}
#else
static UINT32 TestCase(VOID)
{
    return LOS_OK;
}
#endif

VOID ItLosMem047(void)
{
    TEST_ADD_CASE("ItLosMem047", TestCase, TEST_LOS, TEST_MEM, TEST_LEVEL1, TEST_FUNCTION);
}




/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
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

#include "stdlib.h"
#include "shcmd.h"
#include "shell.h"

#define MEM_SIZE_1K 0x400
#define MEM_SIZE_1M 0x100000

#define MEM_SIZE_TO_KB(size) (((size) + (MEM_SIZE_1K >> 1)) / MEM_SIZE_1K)
#define MEM_SIZE_TO_MB(size) (((size) + (MEM_SIZE_1M >> 1)) / MEM_SIZE_1M)

LITE_OS_SEC_TEXT_MINOR STATIC UINT32 OsShellCmdFreeInfo(INT32 argc, const CHAR *argv[])
{
    UINT32 memUsed = LOS_MemTotalUsedGet(m_aucSysMem0);
    UINT32 totalMem = LOS_MemPoolSizeGet(m_aucSysMem0);
    UINT32 freeMem = totalMem - memUsed;

    if ((argc == 0) ||
        ((argc == 1) && (strcmp(argv[0], "-k") == 0)) ||
        ((argc == 1) && (strcmp(argv[0], "-m") == 0))) {
        PRINTK("\r\n        total        used          free\n");
    }

    if ((argc == 1) && (strcmp(argv[0], "-k") == 0)) {
        PRINTK("Mem:    %-9u    %-10u    %-10u\n", MEM_SIZE_TO_KB(totalMem), MEM_SIZE_TO_KB(memUsed),
               MEM_SIZE_TO_KB(freeMem));
    } else if ((argc == 1) && (strcmp(argv[0], "-m") == 0)) {
        PRINTK("Mem:    %-9u    %-10u    %-10u\n", MEM_SIZE_TO_MB(totalMem), MEM_SIZE_TO_MB(memUsed),
               MEM_SIZE_TO_MB(freeMem));
    } else if (argc == 0) {
        PRINTK("Mem:    %-9u    %-10u    %-10u\n", totalMem, memUsed, freeMem);
    } else {
        PRINTK("\nUsage: free or free [-k/-m]\n");
        return OS_ERROR;
    }
    return 0;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdFree(INT32 argc, const CHAR *argv[])
{
    if (argc > 1) {
        PRINTK("\nUsage: free or free [-k/-m]\n");
        return OS_ERROR;
    }
    if (OsShellCmdFreeInfo(argc, argv) != 0) {
        return OS_ERROR;
    }
    return 0;
}


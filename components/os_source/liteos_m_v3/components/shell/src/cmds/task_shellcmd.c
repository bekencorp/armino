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
#include "los_config.h"
#include "los_task.h"
#include "los_sem.h"
#include "shcmd.h"
#include "shell.h"

#define OS_INVALID_SEM_ID  0xFFFFFFFF
#define OS_ALL_TASK_MASK   0xFFFFFFFF

LITE_OS_SEC_TEXT_MINOR STATIC VOID OsShellCmdTskInfoTitle(VOID)
{
    PRINTK("Name                   TaskEntryAddr       TID    ");

    PRINTK("Priority   Status       "
#if (LOSCFG_TASK_MEM_USED == 1)
           "AllocSize       "
#endif
           "StackSize       StackPoint   TopOfStack");

    PRINTK("\n");
    PRINTK("----                   -------------       ---    ");
    PRINTK("--------   --------     "
#if (LOSCFG_TASK_MEM_USED == 1)
           "---------       "
#endif
           "---------       ----------   ----------");
    PRINTK("\n");
}

LITE_OS_SEC_TEXT_MINOR STATIC INLINE UINT32 OsGetSemID(const LosTaskCB *taskCB)
{
    UINT32 semId = OS_INVALID_SEM_ID;

    if (taskCB->taskSem != NULL) {
        semId = ((LosSemCB *)taskCB->taskSem)->semID;
    }

    return semId;
}

LITE_OS_SEC_TEXT_MINOR STATIC VOID OsShellCmdTskInfoData(const LosTaskCB *allTaskArray)
{
    const LosTaskCB *taskCB = NULL;
    UINT32 loop;

#if (LOSCFG_TASK_MEM_USED == 1)
    UINT32 arraySize = sizeof(UINT32) * (LOSCFG_BASE_CORE_TSK_LIMIT + 1);
    UINT32 *getUsedSizeArray = (UINT32 *)LOS_MemAlloc(m_aucSysMem0, arraySize);
    if (getUsedSizeArray == NULL) {
        PRINTK("Memory is not enough to save task info!\n");
        return;
    }
    (VOID)memset_s(getUsedSizeArray, arraySize, 0, arraySize);
    OsTaskMemUsed(m_aucSysMem0, getUsedSizeArray, (LOSCFG_BASE_CORE_TSK_LIMIT + 1));
#endif

    for (loop = 0; loop < g_taskMaxNum; ++loop) {
        taskCB = allTaskArray + loop;
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }

        PRINTK("%-23s%-20p0x%-5x", taskCB->taskName, taskCB->taskEntry, taskCB->taskID);
#if (LOSCFG_TASK_MEM_USED == 1)
        PRINTK("%-11u%-13s0x%-11x   0x%-11x   0x%-8x   0x%-10x   ", taskCB->priority,
               OsConvertTskStatus(taskCB->taskStatus), getUsedSizeArray[loop], taskCB->stackSize,
               taskCB->stackPointer, taskCB->topOfStack);
#else
        PRINTK("%-11u%-13s0x%-11x   0x%-8x   0x%-10x   ", taskCB->priority,
               OsConvertTskStatus(taskCB->taskStatus), taskCB->stackSize,
               taskCB->stackPointer, taskCB->topOfStack);
#endif
        PRINTK("\n");
    }
#if (LOSCFG_TASK_MEM_USED == 1)
    (VOID)LOS_MemFree(m_aucSysMem0, getUsedSizeArray);
#endif
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdTskInfoGet(UINT32 taskId)
{
    BOOL backupFlag = TRUE;
    UINT32 size;
    LosTaskCB *tcbArray = NULL;
    INT32 ret;

    if (taskId == OS_ALL_TASK_MASK) {
        size = g_taskMaxNum * sizeof(LosTaskCB);
        tcbArray = (LosTaskCB *)LOS_MemAlloc(m_aucSysMem0, size);
        if (tcbArray == NULL) {
            PRINTK("Memory is not enough to save task info!\n");
            tcbArray = g_taskCBArray;
            backupFlag = FALSE;
        }
        if (backupFlag == TRUE) {
            ret = memcpy_s(tcbArray, size, g_taskCBArray, size);
            if (ret != 0) {
                return LOS_NOK;
            }
        }

        OsShellCmdTskInfoTitle();
        OsShellCmdTskInfoData(tcbArray);

        if (backupFlag == TRUE) {
            (VOID)LOS_MemFree(m_aucSysMem0, tcbArray);
        }
    }

    return LOS_OK;
}

INT32 OsShellCmdDumpTask(INT32 argc, const CHAR **argv)
{
    size_t taskId = OS_ALL_TASK_MASK;

    if (argc < 1) {
        return OsShellCmdTskInfoGet((UINT32)taskId);
    } else {
        PRINTK("\nUsage: task\n");
        return OS_ERROR;
    }
}


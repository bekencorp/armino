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

#include "It_los_queue.h"
#include "los_membox.h"

#define LOS_MEMBOX_MAGIC_SIZE 4
static UINT32 g_uwQueueID122 = 0;
static unsigned char g_aucMailBoxPool[40 + LOS_MEMBOX_MAGIC_SIZE] = {0};
static BOOL  g_bTaskFinish1 = FALSE;
static BOOL  g_bTaskFinish2 = FALSE;


static VOID StTaskAllocWait(VOID)
{
    UINT32 ret;
    VOID *memBox = NULL;

    memBox = OsQueueMailAlloc(g_uwQueueID122, (VOID *)g_aucMailBoxPool, 20); // 20, set timeout
    ICUNIT_GOTO_EQUAL(memBox, NULL, memBox, EXIT);

    memBox = OsQueueMailAlloc(g_uwQueueID122, (VOID *)g_aucMailBoxPool, 50); // 50, set timeout
    ICUNIT_GOTO_NOT_EQUAL(memBox, NULL, memBox, EXIT);

    ret = OsQueueMailFree(g_uwQueueID122, (VOID *)g_aucMailBoxPool, memBox);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

EXIT:
    g_bTaskFinish1 = TRUE;
    return;
}


static VOID StTaskAllocNoWait(VOID)
{
    VOID *memBox = NULL;

    memBox = OsQueueMailAlloc(g_uwQueueID122, (VOID *)g_aucMailBoxPool, LOS_NO_WAIT);
    ICUNIT_GOTO_EQUAL(memBox, NULL, memBox, EXIT);

EXIT:
    g_bTaskFinish2 = TRUE;
    return;
}


static UINT32 Testcase(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S task1221 = {0};
    TSK_INIT_PARAM_S task1222 = {0};
    UINT32 taskID1221, taskID1222;
    QUEUE_INFO_S queueInfo;
    VOID *memBox = NULL;

    task1221.pfnTaskEntry = (TSK_ENTRY_FUNC)StTaskAllocWait;
    task1221.pcName       = "TskName122_1";
    task1221.uwStackSize  = TASK_STACK_SIZE_TEST;
    task1221.usTaskPrio   = 23; // 23, Set the priority according to the task purpose,a smaller number means a higher priority.
    task1221.uwResved     = LOS_TASK_STATUS_DETACHED;

    task1222.pfnTaskEntry = (TSK_ENTRY_FUNC)StTaskAllocNoWait;
    task1222.pcName       = "TskName122_2";
    task1222.uwStackSize  = TASK_STACK_SIZE_TEST;
    task1222.usTaskPrio   = 23; // 23, Set the priority according to the task purpose,a smaller number means a higher priority.
    task1222.uwResved     = LOS_TASK_STATUS_DETACHED;

    ret = LOS_QueueCreate("Q122", 1, &g_uwQueueID122, 0, QUEUE_BASE_MSGSIZE);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_MemboxInit((VOID *)g_aucMailBoxPool, sizeof(g_aucMailBoxPool) * 2, sizeof(g_aucMailBoxPool));
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    memBox = OsQueueMailAlloc(g_uwQueueID122, (VOID *)g_aucMailBoxPool, LOS_NO_WAIT);
    ICUNIT_GOTO_NOT_EQUAL(memBox, NULL, memBox, EXIT);

    ret = LOS_TaskCreate(&taskID1221, &task1221);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_TaskCreate(&taskID1222, &task1222);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    /* make sure the first OsQueueMailAlloc is failed of task1 and task2. */
    LOS_TaskDelay(10); // 10, set delay time

    ret = LOS_QueueInfoGet(g_uwQueueID122, &queueInfo);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_QueueDelete(g_uwQueueID122);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_QUEUE_IN_TSKUSE, ret);

    LOS_TaskDelay(20); // 20, set delay time

    ret = OsQueueMailFree(g_uwQueueID122, (VOID *)g_aucMailBoxPool, memBox);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    /* keep waitting until task1 and task2 exit. */
    while (!(g_bTaskFinish1 && g_bTaskFinish2)) {
        LOS_TaskDelay(5); // 5, set delay time
    }

EXIT:
    ret = LOS_QueueDelete(g_uwQueueID122);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

VOID LltLosQueue003(VOID)
{
    TEST_ADD_CASE("LltLosQueue003", Testcase, TEST_LOS, TEST_QUE, TEST_LEVEL1, TEST_FUNCTION);
}

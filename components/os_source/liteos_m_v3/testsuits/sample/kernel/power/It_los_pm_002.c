/*
 * Copyright (c) 2021-2021 Huawei Device Co., Ltd. All rights reserved.
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
#include "It_los_pm.h"
#include "los_timer.h"
#include "los_sched.h"

#define myprintf // printf
#define TEST_LOOP 5
static EVENT_CB_S g_pmTestEvent;
static UINT32 g_taskID1, g_taskID2;
static UINT32 g_deviceCount = 0;
static UINT32 g_sysCount = 0;
static volatile UINT32 g_pmTestCount = 0;

static UINT32 DeviceSuspend(UINT32 mode)
{
    g_deviceCount++;
    g_testCount++;
    return LOS_OK;
}

static VOID DeviceResume(UINT32 mode)
{
    g_deviceCount--;
    return;
}

static LosPmDevice g_device = {
    .suspend = DeviceSuspend,
    .resume = DeviceResume,
};

static VOID SysResume(VOID)
{
    if (g_sysCount != (UINT32)-1) {
        g_sysCount--;
    }
}

static UINT32 SysSuspend(VOID)
{
    g_testCount++;
    g_sysCount++;

    if ((g_deviceCount != 1) || (g_sysCount != 1)) { /* 1: sys count */
        g_sysCount = (UINT32)-1;
    }

    UINT64 timeout = LOS_SchedTickTimeoutNsGet();
    printf("pm timeout : %u ns -> %u ticks\n", (UINT32)timeout, (UINT32)(timeout / OS_NS_PER_TICK));
    return HalEnterSleep();
}

static UINT32 SystemPmEarly(UINT32 mode)
{
    UINT32 ret;

    ret = LOS_TaskSuspend(g_taskID2);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_TaskSuspend(g_taskID1);
    if (ret != LOS_OK) {
        (VOID)LOS_TaskResume(g_taskID2);
    }
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

static VOID SystemPmLate(UINT32 mode)
{
    UINT32 ret;

    ICUNIT_ASSERT_EQUAL_VOID(mode, LOS_SYS_LIGHT_SLEEP, mode);

    ret = LOS_TaskResume(g_taskID2);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    ret = LOS_TaskResume(g_taskID1);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);
}

static LosPmSysctrl g_sysctrl = {
    .early = SystemPmEarly,
    .late = SystemPmLate,
    .normalSuspend = HalEnterSleep,
    .normalResume = NULL,
    .lightSuspend = SysSuspend,
    .lightResume = SysResume,
};

#define TEST_FLAGS 100
static VOID PmTestTask(VOID)
{
    UINT32 ret;
    UINT32 wakeCount;

    while (1) {
        wakeCount = LOS_PmReadLock();

        ret = LOS_PmSuspend(wakeCount);
        ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);
        ICUNIT_GOTO_NOT_EQUAL(g_testCount, 0, g_sysCount, EXIT);
        ICUNIT_GOTO_EQUAL(g_sysCount, 0, g_sysCount, EXIT);

        g_pmTestCount++;
        if (g_pmTestCount > TEST_LOOP) {
            break;
        }
        printf("PmTestTask loop: %u\n", g_pmTestCount);
    }

EXIT:
    g_pmTestCount = TEST_FLAGS;
    (VOID)LOS_EventWrite(&g_pmTestEvent, 0x1); /* 0x1: test exit evnet */
    return;
}

#define TEST_TASK1_LOOP 10
static volatile UINT32 g_testSample1Count, g_testSample2Count;
static void TaskSampleEntry2(void)
{
    UINT32 g_testSample2Count = TEST_FLAGS;

    while (1) {
        if (g_testSample2Count == TEST_FLAGS) {
            g_testSample2Count = 0;
            (VOID)LOS_PmLockRequest("TaskSampleEntry2");
            myprintf("%s request pm lock\n", __FUNCTION__);
        }

        myprintf("TaskSampleEntry2 running...count: %u\n\r", g_testSample2Count);
        (VOID)LOS_TaskDelay(20); /* sleep 20 ticks */

        if (g_testSample2Count <= TEST_TASK1_LOOP) { /* */
            g_testSample2Count++;
        }
        if (g_testSample2Count == TEST_TASK1_LOOP) {
            LOS_PmLockRelease("TaskSampleEntry2");
            myprintf("%s release pm lock\n", __FUNCTION__);
        }

        if (g_pmTestCount > TEST_LOOP) {
            break;
        }
    }

    (VOID)LOS_PmLockRelease("TaskSampleEntry2");
    myprintf("TaskSampleEntry2 exit\n");
}

static void TaskSampleEntry1(void)
{
    UINT32 g_testSample1Count = 0;

    while (1) {
        if (g_testSample1Count == 0) {
            (VOID)LOS_PmLockRequest("TaskSampleEntry1");
            myprintf("%s request pm lock\n", __FUNCTION__);
        }

        myprintf("TaskSampleEntry1 running...%u\n\r", g_testSample1Count);
        LOS_TaskDelay(50); /* sleep 50 ticks */

        g_testSample1Count++;
        if (g_testSample1Count == TEST_TASK1_LOOP) {
            LOS_PmLockRelease("TaskSampleEntry1");
            myprintf("%s release pm lock\n", __FUNCTION__);
        } else if (g_testSample1Count == (TEST_TASK1_LOOP + 1)) { /* 1:  incremental */
            g_testSample1Count = 0;
            g_testSample2Count = TEST_FLAGS;
        }

        if (g_pmTestCount > TEST_LOOP) {
            break;
        }
    }

    (VOID)LOS_PmLockRelease("TaskSampleEntry1");
    myprintf("TaskSampleEntry1 exit\n");
}

static UINT32 TestCase(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S task1 = { 0 };
    g_sysCount = 0;
    g_deviceCount = 0;
    g_testCount = 0;

    ret = LOS_EventInit(&g_pmTestEvent);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_PmRegister(LOS_PM_TYPE_DEVICE, &g_device);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_PmRegister(LOS_PM_TYPE_SYSCTRL, &g_sysctrl);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_PmModeSet(LOS_SYS_LIGHT_SLEEP);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)PmTestTask;
    task1.uwStackSize = 0x2000; /* 0x2000 pm task stack size */
    task1.pcName = "pmTask";
    task1.usTaskPrio = 5; /* 5: pm task prio */
    ret = LOS_TaskCreate(&g_testTaskID01, &task1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskSampleEntry1;
    task1.uwStackSize  = 0x1000; /* 0x1000 task stack size */
    task1.pcName       = "TaskSampleEntry1";
    task1.usTaskPrio   = 10; /* 10: task prio */
    ret = LOS_TaskCreate(&g_taskID1, &task1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskSampleEntry2;
    task1.uwStackSize  = 0x1000; /* 0x1000 task stack size */
    task1.pcName       = "TaskSampleEntry2";
    task1.usTaskPrio   = 12; /* 12: task prio */
    ret = LOS_TaskCreate(&g_taskID2, &task1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    (VOID)LOS_EventRead(&g_pmTestEvent, 0xff, LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);

    ret = LOS_PmUnregister(LOS_PM_TYPE_DEVICE, &g_device);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_PmModeSet(LOS_SYS_NORMAL_SLEEP);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    (VOID)LOS_TaskDelete(g_taskID1);
    (VOID)LOS_TaskDelete(g_taskID2);
    (VOID)LOS_TaskDelete(g_testTaskID01);
    return LOS_OK;
}

VOID ItLosPm002(VOID) // IT_Layer_ModuleORFeature_No
{
    TEST_ADD_CASE("ItLosPm002", TestCase, TEST_LOS, TEST_TASK, TEST_LEVEL0, TEST_FUNCTION);
}

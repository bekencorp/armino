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

static UINT32 DeviceSuspend(UINT32 mode)
{
    return LOS_OK;
}

static VOID DeviceResume(UINT32 mode)
{
    return;
}

static LosPmDevice g_device1 = {
    .suspend = NULL,
    .resume = NULL,
};

static VOID SysResume(VOID)
{
}

static LosPmSysctrl g_sysctrl = {
    .normalSuspend = NULL,
    .normalResume = SysResume,
};

static VOID TimerStart(UINT64 timer)
{

}

static UINT64 GetTimerCycle(VOID)
{
    return 0;
}

static VOID TickLock(VOID)
{
    return;
}

static VOID TickUnlock(VOID)
{
    return;
}

static LosPmTickTimer g_tickTimer = {
    .tickLock = NULL,
    .tickUnlock = NULL,
    .timerStart = NULL,
    .timerStop = NULL,
    .timerCycleGet = NULL,
    .freq = 0,
};

static UINT32 TestCase(VOID)
{
    UINT32 ret;

    ret = LOS_PmRegister(LOS_PM_TYPE_DEVICE, NULL);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_PM_INVALID_PARAM, ret);

    ret = LOS_PmRegister(LOS_PM_TYPE_DEVICE, &g_device1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_PM_INVALID_PARAM, ret);

    g_device1.suspend = DeviceSuspend;
    ret = LOS_PmRegister(LOS_PM_TYPE_DEVICE, &g_device1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_PM_INVALID_PARAM, ret);

    g_device1.suspend = NULL;
    g_device1.resume = DeviceResume;
    ret = LOS_PmRegister(LOS_PM_TYPE_DEVICE, &g_device1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_PM_INVALID_PARAM, ret);

    g_device1.suspend = DeviceSuspend;
    g_device1.resume = DeviceResume;
    ret = LOS_PmRegister(LOS_PM_TYPE_DEVICE, &g_device1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_PmRegister(LOS_PM_TYPE_TICK_TIMER, NULL);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_PM_INVALID_PARAM, ret);

    ret = LOS_PmRegister(LOS_PM_TYPE_TICK_TIMER, &g_tickTimer);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_PM_INVALID_PARAM, ret);

    g_tickTimer.tickLock = TickLock;
    ret = LOS_PmRegister(LOS_PM_TYPE_TICK_TIMER, &g_tickTimer);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_PM_INVALID_PARAM, ret);

    g_tickTimer.tickLock = NULL;
    g_tickTimer.tickUnlock = TickUnlock;
    ret = LOS_PmRegister(LOS_PM_TYPE_TICK_TIMER, &g_tickTimer);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_PM_INVALID_PARAM, ret);

    g_tickTimer.tickLock = TickLock;
    g_tickTimer.tickUnlock = TickUnlock;
    ret = LOS_PmRegister(LOS_PM_TYPE_TICK_TIMER, &g_tickTimer);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_PmUnregister(LOS_PM_TYPE_TICK_TIMER, &g_tickTimer);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    g_tickTimer.tickLock = TickLock;
    g_tickTimer.tickUnlock = TickUnlock;
    g_tickTimer.timerStart = TimerStart;
    ret = LOS_PmRegister(LOS_PM_TYPE_TICK_TIMER, &g_tickTimer);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_PM_INVALID_PARAM, ret);

    g_tickTimer.timerStop = TickLock;
    ret = LOS_PmRegister(LOS_PM_TYPE_TICK_TIMER, &g_tickTimer);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_PM_INVALID_PARAM, ret);

    g_tickTimer.timerCycleGet = GetTimerCycle;
    ret = LOS_PmRegister(LOS_PM_TYPE_TICK_TIMER, &g_tickTimer);
    ICUNIT_ASSERT_EQUAL(ret, LOS_ERRNO_PM_INVALID_PARAM, ret);

    g_tickTimer.freq = 32000; /* 32000HZ */
    ret = LOS_PmRegister(LOS_PM_TYPE_TICK_TIMER, &g_tickTimer);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_PmUnregister(LOS_PM_TYPE_TICK_TIMER, &g_tickTimer);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_PmUnregister(LOS_PM_TYPE_SYSCTRL, &g_sysctrl);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_PmUnregister(LOS_PM_TYPE_DEVICE, &g_device1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);
    return LOS_OK;
}

VOID ItLosPm001(VOID) // IT_Layer_ModuleORFeature_No
{
    TEST_ADD_CASE("ItLosPm001", TestCase, TEST_LOS, TEST_TASK, TEST_LEVEL0, TEST_FUNCTION);
}

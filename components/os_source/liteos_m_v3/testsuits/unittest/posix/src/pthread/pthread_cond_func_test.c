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

#include "ohos_types.h"
#include <securec.h>
#include "hctest.h"
#include "los_config.h"
#include "cmsis_os2.h"
#include "common_test.h"

static UINT32 g_testCount;

/**
 * @tc.desc      : register a test suite, this suite is used to test basic flow and interface dependency
 * @param        : subsystem name is pthread
 * @param        : module name is pthread
 * @param        : test suit name is PthreadFuncTestSuite
 */
LITE_TEST_SUIT(Posix, Pthread, PthreadFuncTestSuite);

/**
 * @tc.setup     : setup for all testcases
 * @return       : setup result, TRUE is success, FALSE is fail
 */
static BOOL PthreadFuncTestSuiteSetUp(void)
{
    return TRUE;
}

/**
 * @tc.teardown  : teardown for all testcases
 * @return       : teardown result, TRUE is success, FALSE is fail
 */
static BOOL PthreadFuncTestSuiteTearDown(void)
{
    printf("+-------------------------------------------+\n");
    return TRUE;
}

static VOID *pthread_join_f01(void *argument)
{
    g_testCount++;

    pthread_exit((void *)8); /* 8: pthread exit code */
    return (void *)9; /* 9: return val */
}

/**
 * @tc.number    : SUB_KERNEL_PTHREAD_OPERATION_001
 * @tc.name      : event operation for join
 * @tc.desc      : [C- SOFTWARE -0200]
 */
LITE_TEST_CASE(PthreadFuncTestSuite, testPthread001, Function | MediumTest | Level1)
{
    pthread_attr_t attr;
    pthread_t newTh;
    struct sched_param schedParam = { 0 };
    UINT32 ret;
    UINTPTR uwtemp = 1;

    g_testCount = 0;

    ret = pthread_attr_init(&attr);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_attr_setstacksize(&attr, OS_TSK_TEST_STACK_SIZE);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    schedParam.sched_priority = TASK_PRIO_TEST;
    ret = pthread_attr_setschedparam(&attr, &schedParam);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_create(&newTh, &attr, pthread_join_f01, NULL);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_join(newTh, (void **)&uwtemp);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);
    ICUNIT_ASSERT_EQUAL(uwtemp, 8, uwtemp); /* 8: pthread exit code */

    return LOS_OK;
};

static VOID *pthread_join_f02(void *argument)
{
    g_testCount++;

    return (void *)9; /* 9: return val */
}

/**
 * @tc.number    : SUB_KERNEL_PTHREAD_OPERATION_002
 * @tc.name      : event operation for join
 * @tc.desc      : [C- SOFTWARE -0200]
 */
LITE_TEST_CASE(PthreadFuncTestSuite, testPthread002, Function | MediumTest | Level1)
{
    pthread_attr_t attr;
    pthread_t newTh;
    struct sched_param schedParam = { 0 };
    UINT32 ret;
    UINTPTR uwtemp = 1;

    g_testCount = 0;

    ret = pthread_attr_init(&attr);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_attr_setstacksize(&attr, OS_TSK_TEST_STACK_SIZE);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    schedParam.sched_priority = TASK_PRIO_TEST;
    ret = pthread_attr_setschedparam(&attr, &schedParam);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_create(&newTh, &attr, pthread_join_f02, NULL);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_join(newTh, (void **)&uwtemp);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);
    ICUNIT_ASSERT_EQUAL(uwtemp, 9, uwtemp); /* 9: pthread exit code */

    return LOS_OK;
};

static VOID *pthread_join_f03(void *argument)
{
    int ret = pthread_detach(pthread_self());
    ICUNIT_ASSERT_EQUAL(ret, ESRCH, ret);

    g_testCount++;
    return NULL;
}

/**
 * @tc.number    : SUB_KERNEL_PTHREAD_OPERATION_003
 * @tc.name      : event operation for deatch
 * @tc.desc      : [C- SOFTWARE -0200]
 */
LITE_TEST_CASE(PthreadFuncTestSuite, testPthread003, Function | MediumTest | Level1)
{
    pthread_attr_t attr;
    pthread_t newTh;
    struct sched_param schedParam = { 0 };
    UINT32 ret;

    g_testCount = 0;

    ret = pthread_attr_init(&attr);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_attr_setstacksize(&attr, OS_TSK_TEST_STACK_SIZE);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    schedParam.sched_priority = TASK_PRIO_TEST + 1;
    ret = pthread_attr_setschedparam(&attr, &schedParam);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_create(&newTh, &attr, pthread_join_f03, NULL);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_join(newTh, NULL);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ICUNIT_ASSERT_EQUAL(g_testCount, 1, g_testCount);
    return LOS_OK;
};

static VOID *pthread_join_f04(void *argument)
{
    int ret = pthread_detach(pthread_self());
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    g_testCount++;
    return NULL;
}

/**
 * @tc.number    : SUB_KERNEL_PTHREAD_OPERATION_004
 * @tc.name      : event operation for deatch
 * @tc.desc      : [C- SOFTWARE -0200]
 */
LITE_TEST_CASE(PthreadFuncTestSuite, testPthread004, Function | MediumTest | Level1)
{
    pthread_attr_t attr;
    pthread_t newTh;
    struct sched_param schedParam = { 0 };
    UINT32 ret;

    g_testCount = 0;

    ret = pthread_attr_init(&attr);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_attr_setstacksize(&attr, OS_TSK_TEST_STACK_SIZE);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    schedParam.sched_priority = TASK_PRIO_TEST - 1;
    ret = pthread_attr_setschedparam(&attr, &schedParam);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_create(&newTh, &attr, pthread_join_f04, NULL);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ICUNIT_ASSERT_EQUAL(g_testCount, 1, g_testCount);

    ret = pthread_join(newTh, NULL);
    ICUNIT_ASSERT_EQUAL(ret, EINVAL, ret);

    return LOS_OK;
};

static VOID *pthread_join_f05(void *argument)
{
    int ret = pthread_detach(pthread_self());
    ICUNIT_ASSERT_EQUAL(ret, EINVAL, ret);

    usleep(100000); /* 100000: sleep 100 ms */
    return NULL;
}

/**
 * @tc.number    : SUB_KERNEL_PTHREAD_OPERATION_005
 * @tc.name      : event operation for deatch
 * @tc.desc      : [C- SOFTWARE -0200]
 */
LITE_TEST_CASE(PthreadFuncTestSuite, testPthread005, Function | MediumTest | Level1)
{
    pthread_attr_t attr;
    pthread_t newTh;
    struct sched_param schedParam = { 0 };
    UINT32 ret;

    ret = pthread_attr_init(&attr);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_attr_setstacksize(&attr, OS_TSK_TEST_STACK_SIZE);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    schedParam.sched_priority = TASK_PRIO_TEST - 1;
    ret = pthread_attr_setschedparam(&attr, &schedParam);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_create(&newTh, &attr, pthread_join_f05, NULL);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    ret = pthread_join(newTh, NULL);
    ICUNIT_ASSERT_EQUAL(ret, EINVAL, ret);

    return LOS_OK;
};

RUN_TEST_SUITE(PthreadFuncTestSuite);

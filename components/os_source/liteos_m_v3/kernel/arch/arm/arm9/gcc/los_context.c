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

#include "los_context.h"
#include "securec.h"
#include "los_arch_context.h"
#include "los_arch_interrupt.h"
#include "los_task.h"
#include "los_sched.h"
#include "los_interrupt.h"
#include "los_arch_timer.h"
#include "los_debug.h"

/* ****************************************************************************
 Function    : HalArchInit
 Description : arch init function
 Input       : None
 Output      : None
 Return      : None
 **************************************************************************** */
LITE_OS_SEC_TEXT_INIT VOID HalArchInit(VOID)
{
    UINT32 ret;
    HalHwiInit();

    ret = HalTickStart(OsTickHandler);
    if (ret != LOS_OK) {
        PRINT_ERR("Tick start failed!\n");
    }
}

/* ****************************************************************************
 Function    : HalSysExit
 Description : Task exit function
 Input       : None
 Output      : None
 Return      : None
 **************************************************************************** */
LITE_OS_SEC_TEXT_MINOR VOID HalSysExit(VOID)
{
    LOS_IntLock();
    while (1) {
    }
}

/* ****************************************************************************
 Function    : HalTskStackInit
 Description : Task stack initialization function
 Input       : taskID     --- TaskID
               stackSize  --- Total size of the stack
               topStack    --- Top of task's stack
 Output      : None
 Return      : Context pointer
 **************************************************************************** */
LITE_OS_SEC_TEXT_INIT VOID *HalTskStackInit(UINT32 taskID, UINT32 stackSize, VOID *topStack)
{
    TaskContext *context = NULL;
    LosTaskCB *taskCB = OS_TCB_FROM_TID(taskID);
    errno_t result;

    /* initialize the task stack, write magic num to stack top */
    result = memset_s(topStack, stackSize, (INT32)(OS_TASK_STACK_INIT & 0xFF), stackSize);
    if (result != EOK) {
        PRINT_ERR("memset_s is failed:%s[%d]\r\n", __FUNCTION__, __LINE__);
    }
    *((UINT32 *)(topStack)) = OS_TASK_MAGIC_WORD;

    context = (TaskContext *)(((UINTPTR)topStack + stackSize) - sizeof(TaskContext));

    context->r0 = taskID;
    context->r1 = 0x01010101L;
    context->r2 = 0x02020202L;
    context->r3 = 0x03030303L;
    context->r4 = 0x04040404L;
    context->r5 = 0x05050505L;
    context->r6 = 0x06060606L;
    context->r7 = 0x07070707L;
    context->r8 = 0x08080808L;
    context->r9 = 0x09090909L;
    context->r10 = 0x10101010L;
    context->r11 = 0x11111111L;
    context->r12 = 0x12121212L;
    context->sp = (UINTPTR)topStack + stackSize;
    context->lr = (UINTPTR)HalSysExit;

    if ((UINTPTR)taskCB->taskEntry & 0x01) {
        context->pc = (UINTPTR)OsTaskEntryThumb;
        context->spsr = PSR_MODE_SYS_THUMB; /* thumb mode */
    } else {
        context->pc = (UINTPTR)OsTaskEntryArm;
        context->spsr = PSR_MODE_SYS_ARM;   /* arm mode */
    }

    return (VOID *)context;
}

LITE_OS_SEC_TEXT_INIT UINT32 HalStartSchedule(VOID)
{
    (VOID)LOS_IntLock();
    OsSchedStart();
    HalStartToRun();

    return LOS_OK; /* never return */
}

LITE_OS_SEC_TEXT_INIT VOID HalTaskSchedule(VOID)
{
    __asm__ __volatile__("swi 0");
}

LITE_OS_SEC_TEXT_INIT VOID dmb(VOID)
{
    __asm__ __volatile__("" : : : "memory");
}

LITE_OS_SEC_TEXT_INIT VOID dsb(VOID)
{
    __asm__ __volatile__("mcr p15, 0, %0, c7, c10, 4" : : "r"(0) : "memory");
}

LITE_OS_SEC_TEXT_INIT VOID isb(VOID)
{
    __asm__ __volatile__("" : : : "memory");
}

LITE_OS_SEC_TEXT_INIT VOID wfi(VOID)
{
    __asm__ __volatile__("mcr p15, 0, %0, c7, c0, 4" : : "r"(0) : "memory");
}


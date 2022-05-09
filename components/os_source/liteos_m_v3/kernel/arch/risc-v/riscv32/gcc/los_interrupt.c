/*
 * Copyright (c) 2013-2020, Huawei Technologies Co., Ltd. All rights reserved.
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

#include <stdio.h>
#include <stdarg.h>
#include "los_arch.h"
#include "los_arch_interrupt.h"
#include "los_arch_context.h"
#include "los_task.h"
#include "los_sched.h"
#include "los_debug.h"
#include "los_hook.h"
#include "riscv_hal.h"


LosExcInfo g_excInfo;
#define RISCV_EXC_TYPE_NUM 16
#define RISCV_EXC_LOAD_MISALIGNED 4
#define RISCV_EXC_STORE_MISALIGNED 6
const CHAR g_excInformation[RISCV_EXC_TYPE_NUM][50] = {
    { "Instruction address misaligned!" },
    { "Instruction access fault!" },
    { "Illegal instruction" },
    { "Breakpoint!" },
    { "Load address misaligned!" },
    { "Load access fault!" },
    { "Store/AMO address misaligned!" },
    { "Store/AMO access fault!" },
    { "Environment call form U-mode!" },
    { "Environment call form S-mode!" },
    { "Reserved!" },
    { "Environment call form M-mode!" },
    { "Instruction page fault!" },
    { "Load page fault!" },
    { "Reserved!" },
    { "Store/AMO page fault!" },
};

LITE_OS_SEC_BSS UINT32  g_intCount = 0;
LITE_OS_SEC_BSS UINT32 g_hwiFormCnt[OS_HWI_MAX_NUM];
LITE_OS_SEC_DATA_INIT HWI_HANDLE_FORM_S g_hwiForm[OS_HWI_MAX_NUM] = {
    { .pfnHook = NULL, .uwParam = 0 }, // 0 User software interrupt handler
    { .pfnHook = NULL, .uwParam = 0 }, // 1 Supervisor software interrupt handler
    { .pfnHook = NULL, .uwParam = 0 }, // 2 Reserved
    { .pfnHook = HalHwiDefaultHandler, .uwParam = 0 }, // 3 Machine software interrupt handler
    { .pfnHook = NULL, .uwParam = 0 }, // 4 User timer interrupt handler
    { .pfnHook = NULL, .uwParam = 0 }, // 5 Supervisor timer interrupt handler
    { .pfnHook = NULL, .uwParam = 0 }, // 6  Reserved
    { .pfnHook = HalHwiDefaultHandler, .uwParam = 0 }, // 7 Machine timer interrupt handler
    { .pfnHook = NULL, .uwParam = 0 }, // 8  User external interrupt handler
    { .pfnHook = NULL, .uwParam = 0 }, // 9 Supervisor external interrupt handler
    { .pfnHook = NULL, .uwParam = 0 }, // 10 Reserved
    { .pfnHook = HalHwiDefaultHandler, .uwParam = 0 }, // 11 Machine external interrupt handler
    { .pfnHook = HalHwiDefaultHandler, .uwParam = 0 }, // 12 NMI handler
    { .pfnHook = NULL, .uwParam = 0 }, // 13 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 14 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 15 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 16 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 17 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 18 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 19 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 20 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 21 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 22 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 23 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 24 Reserved
    { .pfnHook = NULL, .uwParam = 0 }, // 25 Reserved
};

LITE_OS_SEC_TEXT_INIT VOID HalHwiDefaultHandler(VOID *arg)
{
    (VOID)arg;
    PRINT_ERR("default handler\n");
    while (1) {
    }
}

LITE_OS_SEC_TEXT_INIT VOID HalHwiInit(VOID)
{
    UINT32 index;
    for (index = OS_RISCV_SYS_VECTOR_CNT; index < OS_HWI_MAX_NUM; index++) {
        g_hwiForm[index].pfnHook = HalHwiDefaultHandler;
        g_hwiForm[index].uwParam = 0;
    }
}

typedef VOID (*HwiProcFunc)(VOID *arg);
__attribute__((section(".interrupt.text"))) VOID HalHwiInterruptDone(HWI_HANDLE_T hwiNum)
{
    g_intCount++;

    OsHookCall(LOS_HOOK_TYPE_ISR_ENTER, hwiNum);

    HWI_HANDLE_FORM_S *hwiForm = &g_hwiForm[hwiNum];
    HwiProcFunc func = (HwiProcFunc)(hwiForm->pfnHook);
    func(hwiForm->uwParam);

    ++g_hwiFormCnt[hwiNum];

    OsHookCall(LOS_HOOK_TYPE_ISR_EXIT, hwiNum);

    g_intCount--;
}

LITE_OS_SEC_TEXT UINT32 HalGetHwiFormCnt(HWI_HANDLE_T hwiNum)
{
    if (hwiNum < OS_HWI_MAX_NUM) {
        return g_hwiFormCnt[hwiNum];
    }

    return LOS_NOK;
}

LITE_OS_SEC_TEXT HWI_HANDLE_FORM_S *HalGetHwiForm(VOID)
{
    return g_hwiForm;
}


inline UINT32 HalIsIntActive(VOID)
{
    return (g_intCount > 0);
}

/*****************************************************************************
 Function    : HalHwiCreate
 Description : create hardware interrupt
 Input       : hwiNum     --- hwi num to create
               hwiPrio    --- priority of the hwi
               hwiMode    --- hwi interrupt mode
               hwiHandler --- hwi handler
               irqParam   --- param of the hwi handler
 Output      : None
 Return      : LOS_OK on success or error code on failure
 *****************************************************************************/
LITE_OS_SEC_TEXT UINT32 HalHwiCreate(HWI_HANDLE_T hwiNum,
                                      HWI_PRIOR_T hwiPrio,
                                      HWI_MODE_T hwiMode,
                                      HWI_PROC_FUNC hwiHandler,
                                      HWI_ARG_T irqParam)
{
    UINT32 intSave;

    if (hwiHandler == NULL) {
        return OS_ERRNO_HWI_PROC_FUNC_NULL;
    }
    if (hwiNum >= OS_HWI_MAX_NUM) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    if (g_hwiForm[hwiNum].pfnHook == NULL) {
        return OS_ERRNO_HWI_NUM_INVALID;
    } else if (g_hwiForm[hwiNum].pfnHook != HalHwiDefaultHandler) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    if ((hwiPrio < OS_HWI_PRIO_LOWEST) || (hwiPrio > OS_HWI_PRIO_HIGHEST)) {
        return OS_ERRNO_HWI_PRIO_INVALID;
    }

    intSave = LOS_IntLock();
    g_hwiForm[hwiNum].pfnHook = hwiHandler;
    g_hwiForm[hwiNum].uwParam = (VOID *)irqParam;

    if (hwiNum >= OS_RISCV_SYS_VECTOR_CNT) {
        HalSetLocalInterPri(hwiNum, hwiPrio);
    }

    LOS_IntRestore(intSave);

    return LOS_OK;
}

/*****************************************************************************
 Function    : HalHwiDelete
 Description : Delete hardware interrupt
 Input       : hwiNum   --- hwi num to delete
 Return      : LOS_OK on success or error code on failure
 *****************************************************************************/
LITE_OS_SEC_TEXT UINT32 HalHwiDelete(HWI_HANDLE_T hwiNum)
{
    UINT32 intSave;

    if (hwiNum >= OS_HWI_MAX_NUM) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

    intSave = LOS_IntLock();
    g_hwiForm[hwiNum].pfnHook = HalHwiDefaultHandler;
    g_hwiForm[hwiNum].uwParam = 0;
    LOS_IntRestore(intSave);
    return LOS_OK;
}

STATIC VOID DisplayTaskInfo(VOID)
{
    TSK_INFO_S taskInfo;
    UINT32 index;
    UINT32 ret;

    PRINTK("ID  Pri    Status     name \n\r");
    PRINTK("--  ---    ---------  ----\n\r");

    for (index = 0; index < LOSCFG_BASE_CORE_TSK_LIMIT; index++) {
        ret = LOS_TaskInfoGet(index, &taskInfo);
        if (ret != LOS_OK) {
            continue;
        }
        PRINTK("%d    %d     %s      %s \n\r",
               taskInfo.uwTaskID, taskInfo.usTaskPrio, OsConvertTskStatus(taskInfo.usTaskStatus), taskInfo.acName);
    }
    return;
}

STATIC VOID ExcBackTrace(VOID)
{
    UINTPTR LR[LOSCFG_BACKTRACE_DEPTH] = {0};
    UINT32 index;

    OsBackTraceHookCall(LR, LOSCFG_BACKTRACE_DEPTH, 1, 0); /* 1: Ignore the one layer call relationship within the function. */

    PRINTK("----- traceback start -----\r\n");
    for (index = 0; index < LOSCFG_BACKTRACE_DEPTH; index++) {
        if (LR[index] == 0) {
            break;
        }
        PRINTK("traceback %d -- lr = 0x%x\r\n", index, LR[index]);
    }
    PRINTK("----- traceback end -----\r\n");
}

STATIC VOID ExcInfoDisplayContext(const LosExcInfo *exc)
{
    const TaskContext *taskContext = &(exc->context->taskContext);

    PRINTK("mepc       = 0x%x\n", taskContext->mepc);
    PRINTK("mstatus    = 0x%x\n", taskContext->mstatus);
    PRINTK("mtval      = 0x%x\n", exc->context->mtval);
    PRINTK("mcause     = 0x%x\n", exc->context->mcause);
    PRINTK("ra         = 0x%x\n", taskContext->ra);
    PRINTK("sp         = 0x%x\n", taskContext->sp);
    PRINTK("gp         = 0x%x\n", exc->context->gp);
    PRINTK("tp         = 0x%x\n", taskContext->tp);
    PRINTK("t0         = 0x%x\n", taskContext->t0);
    PRINTK("t1         = 0x%x\n", taskContext->t1);
    PRINTK("t2         = 0x%x\n", taskContext->t2);
    PRINTK("s0         = 0x%x\n", taskContext->s0);
    PRINTK("s1         = 0x%x\n", taskContext->s1);
    PRINTK("a0         = 0x%x\n", taskContext->a0);
    PRINTK("a1         = 0x%x\n", taskContext->a1);
    PRINTK("a2         = 0x%x\n", taskContext->a2);
    PRINTK("a3         = 0x%x\n", taskContext->a3);
    PRINTK("a4         = 0x%x\n", taskContext->a4);
    PRINTK("a5         = 0x%x\n", taskContext->a5);
    PRINTK("a6         = 0x%x\n", taskContext->a6);
    PRINTK("a7         = 0x%x\n", taskContext->a7);
    PRINTK("s2         = 0x%x\n", taskContext->s2);
    PRINTK("s3         = 0x%x\n", taskContext->s3);
    PRINTK("s4         = 0x%x\n", taskContext->s4);
    PRINTK("s5         = 0x%x\n", taskContext->s5);
    PRINTK("s6         = 0x%x\n", taskContext->s6);
    PRINTK("s7         = 0x%x\n", taskContext->s7);
    PRINTK("s8         = 0x%x\n", taskContext->s8);
    PRINTK("s9         = 0x%x\n", taskContext->s9);
    PRINTK("s10        = 0x%x\n", taskContext->s10);
    PRINTK("s11        = 0x%x\n", taskContext->s11);
    PRINTK("t3         = 0x%x\n", taskContext->t3);
    PRINTK("t4         = 0x%x\n", taskContext->t4);
    PRINTK("t5         = 0x%x\n", taskContext->t5);
    PRINTK("t6         = 0x%x\n", taskContext->t6);

    ExcBackTrace();
}

STATIC VOID ExcInfoDisplay(const LosExcContext *excBufAddr)
{
    PRINTK("\r\nException Information     \n\r");

    if (g_excInfo.type < RISCV_EXC_TYPE_NUM) {
        PRINTK("Exc  type : Oops  - %s\n\r", g_excInformation[g_excInfo.type]);
    } else {
        PRINTK("Exc  type : Oops  - Invalid\n\r");
    }

    PRINTK("taskName = %s\n\r", g_losTask.runTask->taskName);
    PRINTK("taskID = %u\n\r", g_losTask.runTask->taskID);
    PRINTK("system mem addr:0x%x\n\r", (UINTPTR)LOSCFG_SYS_HEAP_ADDR);
    ExcInfoDisplayContext(&g_excInfo);
}

WEAK UINT32 HalUnalignedAccessFix(UINTPTR mcause, UINTPTR mepc, UINTPTR mtval, VOID *sp)
{
    /* Unaligned access fixes are not supported by default */
    PRINTK("Unaligned access fixes are not supported by default!\n\r");
    return LOS_NOK;
}

VOID HalExcEntry(const LosExcContext *excBufAddr)
{
    UINT32 ret;
    g_excInfo.type = excBufAddr->mcause & 0x1FF;
    g_excInfo.context = (LosExcContext *)excBufAddr;
    if (g_excInfo.nestCnt > 2) { /* 2: Number of layers of exception nesting */
        PRINTK("hard fault!\n\r");
        goto SYSTEM_DEATH;
    }

    if ((g_excInfo.type == RISCV_EXC_LOAD_MISALIGNED) ||
        (g_excInfo.type == RISCV_EXC_STORE_MISALIGNED)) {
        ret = HalUnalignedAccessFix(excBufAddr->mcause, excBufAddr->taskContext.mepc, excBufAddr->mtval,
                                    (VOID *)excBufAddr);
        if (!ret) {
            return;
        }
    }

    ExcInfoDisplay(excBufAddr);

    PRINTK("----------------All Task infomation ------------\n\r");
    DisplayTaskInfo();

SYSTEM_DEATH:
    OsDoExcHook(EXC_INTERRUPT);
    while (1) {
    }
}


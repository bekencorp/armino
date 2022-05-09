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

#include "los_pm.h"
#include "securec.h"
#include "los_sched.h"
#include "los_timer.h"
#include "los_memory.h"
#include "los_swtmr.h"

#if (LOSCFG_KERNEL_PM == 1)
#define OS_PM_NODE_FREE 0x80000000U
#define OS_PM_LOCK_MAX  0xFFFFU
#define OS_PM_SYS_EARLY        1
#define OS_PM_SYS_DEVICE_EARLY 2

typedef UINT32 (*Suspend)(UINT32 mode);

typedef struct {
    CHAR         *name;
    UINT32       count;
    UINT32       swtmrID;
    LOS_DL_LIST  list;
} OsPmLockCB;

typedef struct {
    LOS_SysSleepEnum  pmMode;
    LOS_SysSleepEnum  sysMode;
    UINT16            lock;
    BOOL              isWake;
    LosPmDevice       *device;
    LosPmSysctrl      *sysctrl;
    LosPmTickTimer    *tickTimer;
#if (LOSCFG_BASE_CORE_TICK_WTIMER == 0)
    UINT64            enterSleepTime;
#endif
    LOS_DL_LIST       lockList;
} LosPmCB;

#define PM_EVENT_LOCK_MASK    0xF
#define PM_EVENT_LOCK_RELEASE 0x1
STATIC EVENT_CB_S g_pmEvent;
STATIC LosPmCB g_pmCB;
STATIC LosPmSysctrl g_sysctrl;
STATIC UINT64 g_pmSleepTime;

STATIC VOID OsPmSysctrlInit(VOID)
{
    /* Default handler functions, which are implemented by the product */
    g_sysctrl.early = NULL;
    g_sysctrl.late = NULL;
    g_sysctrl.normalSuspend = HalEnterSleep;
    g_sysctrl.normalResume = NULL;
    g_sysctrl.lightSuspend = HalEnterSleep;
    g_sysctrl.lightResume = NULL;
    g_sysctrl.deepSuspend = HalEnterSleep;
    g_sysctrl.deepResume = NULL;
    g_sysctrl.shutdownSuspend = NULL;
    g_sysctrl.shutdownResume = NULL;
}

STATIC VOID OsPmTickTimerStart(LosPmCB *pm)
{
#if (LOSCFG_BASE_CORE_TICK_WTIMER == 0)
    UINT64 currTime, sleepTime, realSleepTime;
#endif
    LosPmTickTimer *tickTimer = pm->tickTimer;

    if ((tickTimer == NULL) || (tickTimer->tickUnlock == NULL)) {
        return;
    }

#if (LOSCFG_BASE_CORE_TICK_WTIMER == 0)
    if (tickTimer->timerStop != NULL) {
        /* Restore the main CPU frequency */
        sleepTime = tickTimer->timerCycleGet();
        tickTimer->timerStop();

        realSleepTime = OS_SYS_CYCLE_TO_NS(sleepTime, tickTimer->freq);
        realSleepTime = OS_SYS_NS_TO_CYCLE(realSleepTime, OS_SYS_CLOCK);
        currTime = pm->enterSleepTime + realSleepTime;
        pm->enterSleepTime = 0;

        OsSchedTimerBaseReset(currTime);
    }
#endif

    tickTimer->tickUnlock();
    return;
}

STATIC BOOL OsPmTickTimerStop(LosPmCB *pm)
{
#if (LOSCFG_BASE_CORE_TICK_WTIMER == 0)
    UINT64 sleepCycle;
    UINT64 realSleepTime = g_pmSleepTime;
#endif
    LosPmTickTimer *tickTimer = pm->tickTimer;

    if ((tickTimer == NULL) || (tickTimer->tickLock == NULL)) {
        return FALSE;
    }

#if (LOSCFG_BASE_CORE_TICK_WTIMER == 0)
    if (tickTimer->timerStart != NULL) {
        if (realSleepTime == 0) {
            return FALSE;
        }

        sleepCycle = OS_SYS_CYCLE_TO_NS(realSleepTime, OS_SYS_CLOCK);
        sleepCycle = OS_SYS_NS_TO_CYCLE(sleepCycle, tickTimer->freq);

        /* The main CPU reduces the frequency */
        pm->enterSleepTime = OsGetCurrSysTimeCycle();
        tickTimer->tickLock();
        tickTimer->timerStart(sleepCycle);
        return TRUE;
    }
#endif

    tickTimer->tickLock();
    return TRUE;
}

STATIC VOID OsPmCpuResume(LosPmCB *pm)
{
    if ((pm->sysMode == LOS_SYS_NORMAL_SLEEP) && (pm->sysctrl->normalResume != NULL)) {
        pm->sysctrl->normalResume();
    } else if ((pm->sysMode == LOS_SYS_LIGHT_SLEEP) && (pm->sysctrl->lightResume != NULL)) {
        pm->sysctrl->lightResume();
    } else if ((pm->sysMode == LOS_SYS_DEEP_SLEEP) && (pm->sysctrl->deepResume != NULL)) {
        pm->sysctrl->deepResume();
    }
}

STATIC VOID OsPmCpuSuspend(LosPmCB *pm)
{
    /* cpu enter low power mode */
    LOS_ASSERT(pm->sysctrl != NULL);

    if (pm->sysMode == LOS_SYS_NORMAL_SLEEP) {
        pm->sysctrl->normalSuspend();
    } else if (pm->sysMode == LOS_SYS_LIGHT_SLEEP) {
        pm->sysctrl->lightSuspend();
    } else if (pm->sysMode == LOS_SYS_DEEP_SLEEP) {
        pm->sysctrl->deepSuspend();
    } else {
        pm->sysctrl->shutdownSuspend();
    }
}

STATIC VOID OsPmResumePrepare(LosPmCB *pm, UINT32 mode, UINT32 prepare)
{
    if ((prepare == 0) && (pm->device->resume != NULL)) {
        pm->device->resume(mode);
    }

    if (((prepare == 0) || (prepare == OS_PM_SYS_DEVICE_EARLY)) && (pm->sysctrl->late != NULL)) {
        pm->sysctrl->late(mode);
    }
}

STATIC UINT32 OsPmSuspendPrepare(Suspend sysSuspendEarly, Suspend deviceSuspend, UINT32 mode, UINT32 *prepare)
{
    UINT32 ret;

    if (sysSuspendEarly != NULL) {
        ret = sysSuspendEarly(mode);
        if (ret != LOS_OK) {
            *prepare = OS_PM_SYS_EARLY;
            return ret;
        }
    }

    if (deviceSuspend != NULL) {
        ret = deviceSuspend(mode);
        if (ret != LOS_OK) {
            *prepare = OS_PM_SYS_DEVICE_EARLY;
            return ret;
        }
    }

    return LOS_OK;
}

STATIC UINT32 OsPmSuspendCheck(LosPmCB *pm, Suspend *sysSuspendEarly, Suspend *deviceSuspend, LOS_SysSleepEnum *mode)
{
    UINT32 intSave;

    intSave = LOS_IntLock();
    pm->sysMode = pm->pmMode;
    if (pm->lock > 0) {
        pm->sysMode = LOS_SYS_NORMAL_SLEEP;
        LOS_IntRestore(intSave);
        return LOS_NOK;
    }

    pm->isWake = FALSE;
    *mode = pm->sysMode;
    *sysSuspendEarly = pm->sysctrl->early;
    *deviceSuspend = pm->device->suspend;
    LOS_IntRestore(intSave);
    return LOS_OK;
}

STATIC UINT32 OsPmSuspendSleep(LosPmCB *pm)
{
    UINT32 ret, intSave;
    Suspend sysSuspendEarly, deviceSuspend;
    LOS_SysSleepEnum mode;
    UINT32 prepare = 0;
    BOOL tickTimerStop = FALSE;
    UINT64 currTime;

    ret = OsPmSuspendCheck(pm, &sysSuspendEarly, &deviceSuspend, &mode);
    if (ret != LOS_OK) {
        return ret;
    }

    ret = OsPmSuspendPrepare(sysSuspendEarly, deviceSuspend, (UINT32)mode, &prepare);
    if (ret != LOS_OK) {
        intSave = LOS_IntLock();
        LOS_TaskLock();
        goto EXIT;
    }

    intSave = LOS_IntLock();
    LOS_TaskLock();
    if (pm->isWake || (pm->lock > 0)) {
        goto EXIT;
    }

    tickTimerStop = OsPmTickTimerStop(pm);
    if (!tickTimerStop) {
        currTime = OsGetCurrSchedTimeCycle();
        OsSchedResetSchedResponseTime(0);
        OsSchedUpdateExpireTime(currTime, TRUE);
    }

    OsPmCpuSuspend(pm);

    OsPmCpuResume(pm);

    OsPmTickTimerStart(pm);

EXIT:
    pm->sysMode = LOS_SYS_NORMAL_SLEEP;
    OsPmResumePrepare(pm, (UINT32)mode, prepare);
    LOS_IntRestore(intSave);

    LOS_TaskUnlock();
    return ret;
}

STATIC VOID OsPmNormalSleep(VOID)
{
    UINT32 intSave;
    LosPmCB *pm = &g_pmCB;

    intSave = LOS_IntLock();

    OsPmCpuSuspend(pm);

    OsPmCpuResume(pm);

    LOS_IntRestore(intSave);
}

STATIC UINT32 OsPmDeviceRegister(LosPmCB *pm, LosPmDevice *device)
{
    UINT32 intSave;

    if ((device->suspend == NULL) || (device->resume == NULL)) {
        return LOS_ERRNO_PM_INVALID_PARAM;
    }

    intSave = LOS_IntLock();
    pm->device = device;
    LOS_IntRestore(intSave);

    return LOS_OK;
}

STATIC UINT32 OsPmTickTimerRegister(LosPmCB *pm, LosPmTickTimer *tickTimer)
{
    UINT32 intSave;

    if ((tickTimer->tickLock == NULL) || (tickTimer->tickUnlock == NULL)) {
        return LOS_ERRNO_PM_INVALID_PARAM;
    }

    if (((tickTimer->timerStart == NULL) && (tickTimer->timerStop == NULL) &&
         (tickTimer->timerCycleGet == NULL) && (tickTimer->freq == 0)) ||
        ((tickTimer->timerStart != NULL) && (tickTimer->timerStop != NULL) &&
         (tickTimer->timerCycleGet != NULL) && (tickTimer->freq != 0))) {
        intSave = LOS_IntLock();
#if (LOSCFG_BASE_CORE_TICK_WTIMER == 0)
        pm->enterSleepTime = 0;
#endif
        pm->tickTimer = tickTimer;
        LOS_IntRestore(intSave);
        return LOS_OK;
    }

    return LOS_ERRNO_PM_INVALID_PARAM;
}

STATIC UINT32 OsPmSysctrlRegister(LosPmCB *pm, LosPmSysctrl *sysctrl)
{
    UINT32 intSave = LOS_IntLock();
    if (sysctrl->early != NULL) {
        pm->sysctrl->early = sysctrl->early;
    }
    if (sysctrl->late != NULL) {
        pm->sysctrl->late = sysctrl->late;
    }
    if (sysctrl->normalSuspend != NULL) {
        pm->sysctrl->normalSuspend = sysctrl->normalSuspend;
    }
    if (sysctrl->normalResume != NULL) {
        pm->sysctrl->normalResume = sysctrl->normalResume;
    }
    if (sysctrl->lightSuspend != NULL) {
        pm->sysctrl->lightSuspend = sysctrl->lightSuspend;
    }
    if (sysctrl->lightResume != NULL) {
        pm->sysctrl->lightResume = sysctrl->lightResume;
    }
    if (sysctrl->deepSuspend != NULL) {
        pm->sysctrl->deepSuspend = sysctrl->deepSuspend;
    }
    if (sysctrl->deepResume != NULL) {
        pm->sysctrl->deepResume = sysctrl->deepResume;
    }
    if (sysctrl->shutdownSuspend != NULL) {
        pm->sysctrl->shutdownSuspend = sysctrl->shutdownSuspend;
    }
    if (sysctrl->shutdownResume != NULL) {
        pm->sysctrl->shutdownResume = sysctrl->shutdownResume;
    }
    LOS_IntRestore(intSave);
    return LOS_OK;
}

UINT32 LOS_PmRegister(LOS_PmNodeType type, VOID *node)
{
    LosPmCB *pm = &g_pmCB;

    if (node == NULL) {
        return LOS_ERRNO_PM_INVALID_PARAM;
    }

    switch (type) {
        case LOS_PM_TYPE_DEVICE:
            return OsPmDeviceRegister(pm, (LosPmDevice *)node);
        case LOS_PM_TYPE_TICK_TIMER:
            return OsPmTickTimerRegister(pm, (LosPmTickTimer *)node);
        case LOS_PM_TYPE_SYSCTRL:
            return OsPmSysctrlRegister(pm, (LosPmSysctrl *)node);
        default:
            break;
    }

    return LOS_ERRNO_PM_INVALID_TYPE;
}

STATIC UINT32 OsPmDeviceUnregister(LosPmCB *pm, LosPmDevice *device)
{
    UINT32 intSave;

    intSave = LOS_IntLock();
    if (pm->device == device) {
        pm->device = NULL;
        pm->pmMode = LOS_SYS_NORMAL_SLEEP;
        LOS_IntRestore(intSave);
        return LOS_OK;
    }

    LOS_IntRestore(intSave);
    return LOS_ERRNO_PM_INVALID_NODE;
}

STATIC UINT32 OsPmTickTimerUnregister(LosPmCB *pm, LosPmTickTimer *tickTimer)
{
    UINT32 intSave;

    intSave = LOS_IntLock();
    if (pm->tickTimer == tickTimer) {
        pm->tickTimer = NULL;
        if ((pm->pmMode != LOS_SYS_NORMAL_SLEEP) && (pm->pmMode != LOS_SYS_LIGHT_SLEEP)) {
            pm->pmMode = LOS_SYS_NORMAL_SLEEP;
        }
        LOS_IntRestore(intSave);
        return LOS_OK;
    }

    LOS_IntRestore(intSave);
    return LOS_ERRNO_PM_INVALID_NODE;
}

STATIC UINT32 OsPmSysctrlUnregister(LosPmCB *pm, LosPmSysctrl *sysctrl)
{
    (VOID)sysctrl;

    UINT32 intSave = LOS_IntLock();
    OsPmSysctrlInit();
    pm->pmMode = LOS_SYS_NORMAL_SLEEP;
    LOS_IntRestore(intSave);
    return LOS_OK;
}

UINT32 LOS_PmUnregister(LOS_PmNodeType type, VOID *node)
{
    LosPmCB *pm = &g_pmCB;

    if (node == NULL) {
        return LOS_ERRNO_PM_INVALID_PARAM;
    }

    switch (type) {
        case LOS_PM_TYPE_DEVICE:
            return OsPmDeviceUnregister(pm, (LosPmDevice *)node);
        case LOS_PM_TYPE_TICK_TIMER:
            return OsPmTickTimerUnregister(pm, (LosPmTickTimer *)node);
        case LOS_PM_TYPE_SYSCTRL:
            return OsPmSysctrlUnregister(pm, (LosPmSysctrl *)node);
        default:
            break;
    }

    return LOS_ERRNO_PM_INVALID_TYPE;
}

VOID LOS_PmWakeSet(VOID)
{
    UINT32 intSave;
    LosPmCB *pm = &g_pmCB;

    intSave = LOS_IntLock();
    pm->isWake = TRUE;
    LOS_IntRestore(intSave);
    return;
}

LOS_SysSleepEnum LOS_PmModeGet(VOID)
{
    LOS_SysSleepEnum mode;
    LosPmCB *pm = &g_pmCB;

    UINT32 intSave = LOS_IntLock();
    mode = pm->pmMode;
    LOS_IntRestore(intSave);

    return mode;
}

UINT32 LOS_PmModeSet(LOS_SysSleepEnum mode)
{
    UINT32 intSave;
    LosPmCB *pm = &g_pmCB;
    INT32 sleepMode = (INT32)mode;

    if ((sleepMode < 0) || (sleepMode > LOS_SYS_SHUTDOWN)) {
        return LOS_ERRNO_PM_INVALID_MODE;
    }

    intSave = LOS_IntLock();
    if ((mode != LOS_SYS_NORMAL_SLEEP) && (pm->device == NULL)) {
        LOS_IntRestore(intSave);
        return LOS_ERRNO_PM_DEVICE_NULL;
    }

    if ((mode == LOS_SYS_LIGHT_SLEEP) && (pm->sysctrl->lightSuspend == NULL)) {
        LOS_IntRestore(intSave);
        return LOS_ERRNO_PM_HANDLER_NULL;
    }

    if ((mode == LOS_SYS_DEEP_SLEEP) && (pm->sysctrl->deepSuspend == NULL)) {
        LOS_IntRestore(intSave);
        return LOS_ERRNO_PM_HANDLER_NULL;
    }

    if ((mode == LOS_SYS_SHUTDOWN) && (pm->sysctrl->shutdownSuspend == NULL)) {
        LOS_IntRestore(intSave);
        return LOS_ERRNO_PM_HANDLER_NULL;
    }

    pm->pmMode = mode;
    LOS_IntRestore(intSave);

    return LOS_OK;
}

#if (LOSCFG_KERNEL_PM_DEBUG == 1)
VOID LOS_PmLockInfoShow(VOID)
{
    UINT32 intSave;
    LosPmCB *pm = &g_pmCB;
    OsPmLockCB *lock = NULL;
    LOS_DL_LIST *head = &pm->lockList;
    LOS_DL_LIST *list = head->pstNext;

    PRINTK("Name                          Count\n\r");

    intSave = LOS_IntLock();
    while (list != head) {
        lock = LOS_DL_LIST_ENTRY(list, OsPmLockCB, list);
        PRINTK("%-30s%5u\n\r", lock->name, lock->count);
        list = list->pstNext;
    }
    LOS_IntRestore(intSave);

    return;
}
#endif

UINT32 OsPmLockRequest(const CHAR *name, UINT32 swtmrID)
{
    UINT32 intSave;
    UINT32 ret = LOS_ERRNO_PM_NOT_LOCK;
    LosPmCB *pm = &g_pmCB;
    OsPmLockCB *listNode = NULL;
    OsPmLockCB *lock = NULL;
    LOS_DL_LIST *head = &pm->lockList;
    LOS_DL_LIST *list = head->pstNext;

    intSave = LOS_IntLock();
    while (list != head) {
        listNode = LOS_DL_LIST_ENTRY(list, OsPmLockCB, list);
        if (strcmp(name, listNode->name) == 0) {
            lock = listNode;
            break;
        }

        list = list->pstNext;
    }

    if (lock == NULL) {
        lock = LOS_MemAlloc((VOID *)OS_SYS_MEM_ADDR, sizeof(OsPmLockCB));
        if (lock == NULL) {
            LOS_IntRestore(intSave);
            return LOS_NOK;
        }
        lock->name = (CHAR *)name;
        lock->count = 1;
        lock->swtmrID = swtmrID;
        LOS_ListTailInsert(head, &lock->list);
    } else if (lock->count < OS_PM_LOCK_MAX) {
        lock->count++;
    }

    if ((lock->swtmrID != OS_INVALID) && (lock->count > 1)) {
        lock->count--;
        LOS_IntRestore(intSave);
        return LOS_ERRNO_PM_ALREADY_LOCK;
    }

    if (pm->lock < OS_PM_LOCK_MAX) {
        pm->lock++;
        ret = LOS_OK;
    }

    LOS_IntRestore(intSave);
    return ret;
}

UINT32 LOS_PmLockRequest(const CHAR *name)
{
    if (name == NULL) {
        return LOS_ERRNO_PM_INVALID_PARAM;
    }

    return OsPmLockRequest(name, OS_INVALID);
}

UINT32 LOS_PmLockRelease(const CHAR *name)
{
    UINT32 intSave;
    UINT32 ret = LOS_ERRNO_PM_NOT_LOCK;
    LosPmCB *pm = &g_pmCB;
    OsPmLockCB *lock = NULL;
    OsPmLockCB *listNode = NULL;
    LOS_DL_LIST *head = &pm->lockList;
    LOS_DL_LIST *list = head->pstNext;
    OsPmLockCB *lockFree = NULL;
    BOOL isRelease = FALSE;
    UINT32 mode;

    if (name == NULL) {
        return LOS_ERRNO_PM_INVALID_PARAM;
    }

    intSave = LOS_IntLock();
    mode = (UINT32)pm->pmMode;
    while (list != head) {
        listNode = LOS_DL_LIST_ENTRY(list, OsPmLockCB, list);
        if (strcmp(name, listNode->name) == 0) {
            lock = listNode;
            break;
        }

        list = list->pstNext;
    }

    if (lock == NULL) {
        LOS_IntRestore(intSave);
        return LOS_ERRNO_PM_NOT_LOCK;
    } else if (lock->count > 0) {
        lock->count--;
        if (lock->count == 0) {
            LOS_ListDelete(&lock->list);
            lockFree = lock;
        }
    }

    if (pm->lock > 0) {
        pm->lock--;
        if (pm->lock == 0) {
            isRelease = TRUE;
        }
        ret = LOS_OK;
    }
    LOS_IntRestore(intSave);

    if (lockFree != NULL) {
        (VOID)LOS_SwtmrDelete(lockFree->swtmrID);
        (VOID)LOS_MemFree((VOID *)OS_SYS_MEM_ADDR, lockFree);
    }

    if (isRelease && (mode > LOS_SYS_NORMAL_SLEEP)) {
        (VOID)LOS_EventWrite(&g_pmEvent, PM_EVENT_LOCK_RELEASE);
    }

    return ret;
}

VOID OsPmFreezeTaskUnsafe(UINT32 taskID)
{
    UINT64 responseTime;
    LosTaskCB *taskCB = OS_TCB_FROM_TID(taskID);

    responseTime = GET_SORTLIST_VALUE(&taskCB->sortList);
    OsDeleteSortLink(&taskCB->sortList, OS_SORT_LINK_TASK);
    SET_SORTLIST_VALUE(&taskCB->sortList, responseTime);
    taskCB->taskStatus |= OS_TASK_FALG_FREEZE;
    return;
}

VOID OsPmUnfreezeTaskUnsafe(UINT32 taskID)
{
    LosTaskCB *taskCB = OS_TCB_FROM_TID(taskID);
    UINT64 currTime, responseTime;
    UINT32 remainTick;

    taskCB->taskStatus &= ~OS_TASK_FALG_FREEZE;
    currTime = OsGetCurrSchedTimeCycle();
    responseTime = GET_SORTLIST_VALUE(&taskCB->sortList);
    if (responseTime > currTime) {
        remainTick = ((responseTime - currTime) + OS_CYCLE_PER_TICK - 1) / OS_CYCLE_PER_TICK;
        OsAdd2SortLink(&taskCB->sortList, currTime, remainTick, OS_SORT_LINK_TASK);
        return;
    }

    SET_SORTLIST_VALUE(&taskCB->sortList, OS_SORT_LINK_INVALID_TIME);
    if (taskCB->taskStatus & OS_TASK_STATUS_PEND) {
        LOS_ListDelete(&taskCB->pendList);
    }
    taskCB->taskStatus &= ~(OS_TASK_STATUS_DELAY | OS_TASK_STATUS_PEND_TIME | OS_TASK_STATUS_PEND);
    return;
}

STATIC VOID OsPmSwtmrHandler(UINT32 arg)
{
    const CHAR *name = (const CHAR *)arg;
    UINT32 ret = LOS_PmLockRelease(name);
    if (ret != LOS_OK) {
        PRINT_ERR("Pm delay lock %s release faled! : 0x%x\n", name, ret);
    }
}

UINT32 LOS_PmTimeLockRequest(const CHAR *name, UINT64 millisecond)
{
    UINT32 ticks;
    UINT32 swtmrID;
    UINT32 ret;

    if ((name == NULL) || !millisecond) {
        return LOS_ERRNO_PM_INVALID_PARAM;
    }

    ticks = (UINT32)((millisecond + OS_MS_PER_TICK - 1) / OS_MS_PER_TICK);
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == 1)
    ret = LOS_SwtmrCreate(ticks, LOS_SWTMR_MODE_ONCE, OsPmSwtmrHandler, &swtmrID, (UINT32)(UINTPTR)name,
                          OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_INSENSITIVE);
#else
    ret = LOS_SwtmrCreate(ticks, LOS_SWTMR_MODE_ONCE, OsPmSwtmrHandler, &swtmrID, (UINT32)(UINTPTR)name);
#endif
    if (ret != LOS_OK) {
        return ret;
    }

    ret = OsPmLockRequest(name, swtmrID);
    if (ret != LOS_OK) {
        (VOID)LOS_SwtmrDelete(swtmrID);
        return ret;
    }

    ret = LOS_SwtmrStart(swtmrID);
    if (ret != LOS_OK) {
        (VOID)LOS_PmLockRelease(name);
    }

    return ret;
}

UINT32 LOS_PmReadLock(VOID)
{
    UINT32 ret = LOS_EventRead(&g_pmEvent, PM_EVENT_LOCK_MASK, LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);
    if (ret > PM_EVENT_LOCK_MASK) {
        PRINT_ERR("%s event read failed! ERROR: 0x%x\n", __FUNCTION__, ret);
    }

    return LOS_OK;
}

UINT32 LOS_PmSuspend(UINT32 wakeCount)
{
    (VOID)wakeCount;
    return OsPmSuspendSleep(&g_pmCB);
}

STATIC VOID OsPmSleepTimeSet(UINT64 sleepTime)
{
    g_pmSleepTime = sleepTime;
}

BOOL OsIsPmMode(VOID)
{
    LosPmCB *pm = &g_pmCB;

    UINT32 intSave = LOS_IntLock();
    if ((pm->sysMode != LOS_SYS_NORMAL_SLEEP) && (pm->lock == 0)) {
        LOS_IntRestore(intSave);
        return TRUE;
    }
    LOS_IntRestore(intSave);
    return FALSE;
}

UINT32 OsPmInit(VOID)
{
    UINT32 ret;
    LosPmCB *pm = &g_pmCB;

    (VOID)memset_s(pm, sizeof(LosPmCB), 0, sizeof(LosPmCB));

    pm->pmMode = LOS_SYS_NORMAL_SLEEP;
    LOS_ListInit(&pm->lockList);
    (VOID)LOS_EventInit(&g_pmEvent);

    ret = OsSchedRealSleepTimeSet(OsPmSleepTimeSet);
    if (ret != LOS_OK) {
        return ret;
    }

    ret = OsPmEnterHandlerSet(OsPmNormalSleep);
    if (ret != LOS_OK) {
        return ret;
    }

    OsPmSysctrlInit();
    pm->sysctrl = &g_sysctrl;
    return ret;
}
#endif

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

#ifndef _LOS_PM_H
#define _LOS_PM_H

#include "los_config.h"
#include "los_compiler.h"
#include "los_list.h"
#include "los_error.h"

/**
 * @ingroup los_pm
 * Pm error code: Invalid low power mode.
 *
 * Value: 0x02002001
 *
 */
#define LOS_ERRNO_PM_INVALID_MODE        LOS_ERRNO_OS_ERROR(LOS_MOD_PM, 0x01)

/**
 * @ingroup los_pm
 * Pm error code: Invalid input parameter.
 *
 * Value: 0x02002002
 *
 */
#define LOS_ERRNO_PM_INVALID_PARAM       LOS_ERRNO_OS_ERROR(LOS_MOD_PM, 0x02)

/**
 * @ingroup los_pm
 * Pm error code: The current mode is unlocked.
 *
 * Value: 0x02002003
 *
 */
#define LOS_ERRNO_PM_NOT_LOCK            LOS_ERRNO_OS_ERROR(LOS_MOD_PM, 0x03)

/**
 * @ingroup los_pm
 * Pm error code: The lock limit has been exceeded.
 *
 * Value: 0x02002004
 *
 */
#define LOS_ERRNO_PM_LOCK_LIMIT          LOS_ERRNO_OS_ERROR(LOS_MOD_PM, 0x04)

/**
 * @ingroup los_pm
 * Pm error code: Invalid device node.
 *
 * Value: 0x02002005
 *
 */
#define LOS_ERRNO_PM_INVALID_NODE        LOS_ERRNO_OS_ERROR(LOS_MOD_PM, 0x05)

/**
 * @ingroup los_pm
 * Pm error code: Invalid type.
 *
 * Value: 0x02002006
 *
 */
#define LOS_ERRNO_PM_INVALID_TYPE        LOS_ERRNO_OS_ERROR(LOS_MOD_PM, 0x06)

/**
 * @ingroup los_pm
 * Pm error code: The hook for mode is not implemented.
 *
 * Value: 0x02002007
 *
 */
#define LOS_ERRNO_PM_HANDLER_NULL        LOS_ERRNO_OS_ERROR(LOS_MOD_PM, 0x07)

/**
 * @ingroup los_pm
 * Pm error code: Deep and shutdown must implement the Tick Timer related interface.
 *
 * Value: 0x02002008
 *
 */
#define LOS_ERRNO_PM_TICK_TIMER_NULL     LOS_ERRNO_OS_ERROR(LOS_MOD_PM, 0x08)

/**
 * @ingroup los_pm
 * Pm error code: Device is not registered.
 *
 * Value: 0x02002009
 *
 */
#define LOS_ERRNO_PM_DEVICE_NULL         LOS_ERRNO_OS_ERROR(LOS_MOD_PM, 0x09)

/**
 * @ingroup los_pm
 * Pm error code: The delay lock has already been activated.
 *
 * Value: 0x0200200a
 *
 */
#define LOS_ERRNO_PM_ALREADY_LOCK        LOS_ERRNO_OS_ERROR(LOS_MOD_PM, 0x0a)

typedef enum {
    LOS_SYS_NORMAL_SLEEP = 0,
    LOS_SYS_LIGHT_SLEEP,
    LOS_SYS_DEEP_SLEEP,
    LOS_SYS_SHUTDOWN,
} LOS_SysSleepEnum;

typedef enum {
    LOS_PM_TYPE_DEVICE = 0,
    LOS_PM_TYPE_TICK_TIMER,
    LOS_PM_TYPE_SYSCTRL,
} LOS_PmNodeType;

typedef struct {
    UINT32 (*suspend)(UINT32 mode); /* The device enters low power consumption, Unlocked task scheduling. */
    VOID   (*resume)(UINT32 mode);  /* The device exits from low power consumption, Unlocked task scheduling. */
} LosPmDevice;

typedef struct {
    UINT32 freq;                   /* The frequency of the low power timer */
    VOID   (*timerStart)(UINT64);  /* Start the low power timer */
    VOID   (*timerStop)(VOID);     /* Stop the low power timer */
    UINT64 (*timerCycleGet)(VOID); /* Gets the running time of the low power timer in unit cycle */
    VOID   (*tickLock)(VOID);      /* Pause the system tick timer */
    VOID   (*tickUnlock)(VOID);    /* Restore the system tick timer */
} LosPmTickTimer;

typedef struct {
    /* Preparations before the CPU enters low power consumption.
     * All modes except normal mode are invoked.
     * Unlocked task scheduling.
     */
    UINT32 (*early)(UINT32 mode);
    /* The system performs low-power recovery.
     * All modes except normal mode are invoked.
     * Unlocked task scheduling.
     */
    VOID   (*late)(UINT32 mode);
    /* The system enters the Normal sleep mode.
     * In normal mode, the value cannot be NULL.
     */
    UINT32 (*normalSuspend)(VOID);
    /* The system recovers from normal sleep.
     * The value can be NULL.
     */
    VOID   (*normalResume)(VOID);
    /* The system enters the light sleep mode.
     * In light sleep mode, the value cannot be NULL.
     */
    UINT32 (*lightSuspend)(VOID);
    /* The system recovers from light sleep.
     * The value can be NULL.
     */
    VOID   (*lightResume)(VOID);
    /* The system enters the deep sleep mode.
     * In deep sleep mode, the value cannot be NULL.
     */
    UINT32 (*deepSuspend)(VOID);
    /* The system recovers from deep sleep.
     * The value can be NULL.
     */
    VOID   (*deepResume)(VOID);
    /* The system enters the shutdown mode.
     * In shutdown mode, the value cannot be NULL.
     */
    UINT32 (*shutdownSuspend)(VOID);
    /* The system recovers from shutdown.
     * In shutdown mode, the value cannot be NULL.
     */
    VOID   (*shutdownResume)(VOID);
} LosPmSysctrl;

/**
 * @ingroup los_pm
 * @brief Initialize system low power frame.
 *
 * @par Description:
 * This API is used to initialize the system low power frame.
 *
 * @attention None.
 *
 * @param  None.
 *
 * @retval error code, LOS_OK means success.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see
 */
UINT32 OsPmInit(VOID);

/**
 * @ingroup los_pm
 * @brief Whether the low power consumption condition is met.
 *
 * @par Description:
 * This API is used to check whether low power consumption is met.
 *
 * @attention None.
 *
 * @param  None.
 *
 * @retval TRUE or FALSE.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see
 */
BOOL OsIsPmMode(VOID);

/**
 * @ingroup los_pm
 * @brief Freeze delay tasks, internal interfaces between modules.
 *
 * @par Description:
 * This API is used to freeze delay tasks.
 *
 * @attention None.
 *
 * @param taskID [IN] task ID.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see OsPmUnfreezeTaskUnsafe
 */
VOID OsPmFreezeTaskUnsafe(UINT32 taskID);

/**
 * @ingroup los_pm
 * @brief Unfreeze delayed tasks, internal interface between modules.
 *
 * @par Description:
 * This API is used to unfreeze delayed tasks.
 *
 * @attention None.
 *
 * @param taskID [IN] task ID.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see OsPmFreezeTaskUnsafe
 */
VOID OsPmUnfreezeTaskUnsafe(UINT32 taskID);

/**
 * @ingroup los_pm
 * @brief Register a power management node.
 *
 * @par Description:
 * This API is used to register a power management node.
 *
 * @attention None.
 *
 * @param  type [IN] The types supported by the PM module.
 * @param  node [IN] power management node.
 *
 * @retval error code, LOS_OK means success.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_PmUnregister
 */
UINT32 LOS_PmRegister(LOS_PmNodeType type, VOID *node);

/**
 * @ingroup los_pm
 * @brief Unregister a power management node.
 *
 * @par Description:
 * This API is used to unregister a power management node.
 *
 * @attention None.
 *
 * @param  type [IN] The types supported by the PM module.
 * @param  node [IN] power management node.
 *
 * @retval error code, LOS_OK means success.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_PmRegister
 */
UINT32 LOS_PmUnregister(LOS_PmNodeType type, VOID *node);

/**
 * @ingroup los_pm
 * @brief Set the system wake up flag.
 *
 * @par Description:
 * This API is used to set the system wake-up flag.
 *
 * @attention None.
 *
 * @param None.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see
 */
VOID LOS_PmWakeSet(VOID);

/**
 * @ingroup los_pm
 * @brief Get the low power mode of the current system.
 *
 * @par Description:
 * This API is used to get the low power mode of the current system.
 *
 * @attention None.
 *
 * @param None.
 *
 * @retval error code, LOS_OK means success.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_PmModeSet
 */
LOS_SysSleepEnum LOS_PmModeGet(VOID);

/**
 * @ingroup los_pm
 * @brief Set low power mode.
 *
 * @par Description:
 * This API is used to set low power mode.
 *
 * @attention None.
 *
 * @param  mode [IN] low power mode.
 *
 * @retval error code, LOS_OK means success.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_PmModeGet
 */
UINT32 LOS_PmModeSet(LOS_SysSleepEnum mode);

/**
 * @ingroup los_pm
 * @brief Request to obtain the lock in current mode, so that the system will not enter
 * this mode when it enters the idle task next time.
 *
 * @par Description:
 * This API is used to obtain the lock in current mode.
 *
 * @attention None.
 *
 * @param  name [IN] Who requests the lock.
 *
 * @retval error code, LOS_OK means success.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_PmLockRelease
 */
UINT32 LOS_PmLockRequest(const CHAR *name);

/**
 * @ingroup los_pm
 * @brief Request to obtain the lock in current mode, so that the system will not enter
 * this mode when it enters the idle task next time. After the specified interval, the
 * lock is automatically released.
 *
 * @par Description:
 * This API is used to obtain the delay lock in current mode.
 *
 * @attention None.
 *
 * @param  name        [IN] Who requests the lock.
 * @param  millisecond [IN] Specifies the time to automatically release the lock.
 *
 * @retval error code, LOS_OK means success.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_PmLockRelease
 */
UINT32 LOS_PmTimeLockRequest(const CHAR *name, UINT64 millisecond);

/**
 * @ingroup los_pm
 * @brief Release the lock in current mode so that the next time the system enters
 * the idle task, it will enter this mode.
 *
 * @par Description:
 * This API is used to release the lock in current mode.
 *
 * @attention None.
 *
 * @param  name [IN] Who releases the lock.
 *
 * @retval error code, LOS_OK means success.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_PmLockRequest
 */
UINT32 LOS_PmLockRelease(const CHAR *name);

/**
 * @ingroup los_pm
 * @brief Gets the current PM lock status.
 *
 * @par Description:
 * This API is used to Get the current PM lock status.
 *
 * @attention None.
 *
 * @param NA.
 *
 * @retval Number of awakening sources of the device.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see
 */
UINT32 LOS_PmReadLock(VOID);

/**
 * @ingroup los_pm
 * @brief The system enters the low-power flow.
 *
 * @par Description:
 * This API is used to enter the system into a low-power process.
 *
 * @attention None.
 *
 * @param  wakeCount [IN] Number of wake sources.
 *
 * @retval error code, LOS_OK means success.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see
 */
UINT32 LOS_PmSuspend(UINT32 wakeCount);

/**
 * @ingroup los_pm
 * @brief Output the locking information of the pm lock.
 *
 * @par Description:
 * This API is used to output the locking information of the pm lock.
 *
 * @attention None.
 *
 * @param None.
 *
 * @retval error code, LOS_OK means success.
 * @par Dependency:
 * <ul><li>los_pm.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_PmLockRequest
 */
VOID LOS_PmLockInfoShow(VOID);
#endif

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

/**@defgroup los_config System configuration items
 * @ingroup kernel
 */

#ifndef _TARGET_CONFIG_H_
#define _TARGET_CONFIG_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifndef CONFIG_LITEOS_M_BK
#define CONFIG_LITEOS_M_BK                 1
#endif

#define CMSIS_OS_VER                       2
#define LOSCFG_KERNEL_PRINTF               1
#define LOSCFG_KERNEL_PM                   1

#define LOS_TICK_UNIT_CLOCK                26000000
#define LOS_TICK_MS                        2
#define LOS_TICKS_PER_SECOND              (1000UL/LOS_TICK_MS)

/*=============================================================================
 *System clock module configuration
 *=============================================================================*/
#define OS_SYS_CLOCK                                        (LOS_TICK_UNIT_CLOCK) 
#define LOSCFG_BASE_CORE_TICK_PER_SECOND                    (LOS_TICKS_PER_SECOND)
#define LOSCFG_BASE_CORE_TICK_HW_TIME                       0
#define LOSCFG_BASE_CORE_TICK_WTIMER                         1
#define LOSCFG_BASE_CORE_TICK_RESPONSE_MAX            (0xFFFFFFFFFFFFFFFFULL)

/*=============================================================================
 *Hardware interrupt module configuration
 *=============================================================================*/
#define LOSCFG_PLATFORM_HWI                                 1
#define LOSCFG_PLATFORM_HWI_LIMIT                           128

/*=============================================================================
 *Task module configuration
 *=============================================================================*/
#define LOSCFG_BASE_CORE_TSK_LIMIT                          30
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE                (0x1180U)
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE             (0x1180U)

/*scheduler occupies too much stack for every thread*/
#define LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE                 (0x400U)
#define LOSCFG_BASE_CORE_TIMESLICE                          1
#define LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT                  20000
#define LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO                   15

/*=============================================================================
 *Semaphore module configuration
 *=============================================================================*/
#define LOSCFG_BASE_IPC_SEM                                 1
#define LOSCFG_BASE_IPC_SEM_LIMIT                           32

/*=============================================================================
 *Mutex module configuration
 *=============================================================================*/
#define LOSCFG_BASE_IPC_MUX                                 1
#define LOSCFG_BASE_IPC_MUX_LIMIT                           64

/*=============================================================================
 *Queue module configuration
 *=============================================================================*/
#define LOSCFG_BASE_IPC_QUEUE                               1
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                         32

/*=============================================================================
 *Software timer module configuration
 *=============================================================================*/
#define LOSCFG_BASE_CORE_SWTMR                              1
#define LOSCFG_BASE_CORE_SWTMR_ALIGN                        1
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                        32

/*=============================================================================
 *Memory module configuration
 *=============================================================================*/
extern unsigned char _end;  //the end of bss section in sram
extern unsigned char *m_aucSysMem0;

#define LOSCFG_SYS_EXTERNAL_HEAP                            0

#if (LOSCFG_SYS_EXTERNAL_HEAP == 1)
#define LOSCFG_SYS_HEAP_ADDR                                (&_end)
#else
#define LOSCFG_SYS_HEAP_ADDR                                (&m_aucSysMem0[0])
#define LOSCFG_SYS_HEAP_SIZE                                ( size_t ) ( 140 * 1024 )
#endif


//#define OS_SYS_MEM_SIZE                                     0x0001000
#define LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK                0
#define LOSCFG_BASE_MEM_NODE_SIZE_CHECK                     1
#define LOSCFG_MEM_MUL_POOL                                 1
#define OS_SYS_MEM_NUM                                      20
#define LOSCFG_KERNEL_MEM_SLAB                              0
#define OS_HWI_WITH_ARG               1


/*=============================================================================
 *Exception module configuration
 *=============================================================================*/
#define LOSCFG_PLATFORM_EXC                                 0
#define LOSCFG_BASE_CORE_TSK_MONITOR              1
#define LOSCFG_BACKTRACE_TYPE                     0
#define LOSCFG_MEM_LEAKCHECK                      0

extern void uart_putc(char c);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* _TARGET_CONFIG_H */

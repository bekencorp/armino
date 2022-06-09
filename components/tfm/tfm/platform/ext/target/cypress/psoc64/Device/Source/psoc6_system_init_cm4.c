/***************************************************************************//**
* \file psoc6_system_init_cm4.c
* \version 1.00
*
* The device custom system init source file for CM4 core.
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "cycfg.h"
#include "cy_device.h"

#if defined(CY_IPC_DEFAULT_CFG_DISABLE)
    #include "cy_ipc_sema.h"
    #include "cy_ipc_pipe.h"
    #include "cy_ipc_drv.h"

    #if defined(CY_DEVICE_PSOC6ABLE2)
        #include "cy_flash.h"
    #endif /* defined(CY_DEVICE_PSOC6ABLE2) */
#endif /* defined(CY_IPC_DEFAULT_CFG_DISABLE) */

#if defined(CY_DEVICE_SECURE)
    #include "cy_pra.h"
#endif /* defined(CY_DEVICE_SECURE) */

/* SCB->CPACR */
#define SCB_CPACR_CP10_CP11_ENABLE      (0xFUL << 20u)

/*******************************************************************************
* Function Name: Cy_SystemInit
****************************************************************************//**
*
* The function is called during device startup.
*
*******************************************************************************/
void Cy_SystemInit(void)
{
#if defined(__ARMCC_VERSION)
     /* FP registers are still accessed inside armclang RTX library
      * RTX_CM3.lib even with __FPU_USED not defined.
      * Enable FPU as a temp workaround*/
      uint32_t  interruptState;
      interruptState = Cy_SysLib_EnterCriticalSection();
      SCB->CPACR |= SCB_CPACR_CP10_CP11_ENABLE;
      __DSB();
      __ISB();
      Cy_SysLib_ExitCriticalSection(interruptState);
#endif
}



#if defined(CY_IPC_DEFAULT_CFG_DISABLE)
/*******************************************************************************
* Function Name: Cy_SysIpcPipeIsrCm4
****************************************************************************//**
*
* This is the interrupt service routine for the system pipe.
*
*******************************************************************************/
void Cy_SysIpcPipeIsrCm4(void)
{
    Cy_IPC_Pipe_ExecuteCallback(CY_IPC_EP_CYPIPE_CM4_ADDR);
}
#endif


/*******************************************************************************
* Function Name: Cy_Platform_Init
****************************************************************************//**
*
* CM4 custom HW initialization
*
*******************************************************************************/
void Cy_Platform_Init(void)
{
    Cy_PDL_Init(CY_DEVICE_CFG);

#if defined(CY_IPC_DEFAULT_CFG_DISABLE)
    /* Allocate and initialize semaphores for the system operations. */
    static uint32_t ipcSemaArray[CY_IPC_SEMA_COUNT / CY_IPC_SEMA_PER_WORD];
    (void) Cy_IPC_Sema_Init(CY_IPC_CHAN_SEMA, CY_IPC_SEMA_COUNT, ipcSemaArray);

    /********************************************************************************
    *
    * Initializes the system pipes. The system pipes are used by BLE and Flash.
    *
    * If the default startup file is not used, or SystemInit() is not called in your
    * project, call the following three functions prior to executing any flash or
    * EmEEPROM write or erase operation:
    *  -# Cy_IPC_Sema_Init()
    *  -# Cy_IPC_Pipe_Config()
    *  -# Cy_IPC_Pipe_Init()
    *  -# Cy_Flash_Init()
    *
    *******************************************************************************/
    /* Create an array of endpoint structures */
    static cy_stc_ipc_pipe_ep_t systemIpcPipeEpArray[CY_IPC_MAX_ENDPOINTS];

    Cy_IPC_Pipe_Config(systemIpcPipeEpArray);

    static cy_ipc_pipe_callback_ptr_t systemIpcPipeSysCbArray[CY_SYS_CYPIPE_CLIENT_CNT];

    static const cy_stc_ipc_pipe_config_t systemIpcPipeConfigCm4 =
    {
    /* .ep0ConfigData */
        {
            /* .ipcNotifierNumber    */  CY_IPC_INTR_CYPIPE_EP0,
            /* .ipcNotifierPriority  */  CY_SYS_INTR_CYPIPE_PRIOR_EP0,
            /* .ipcNotifierMuxNumber */  CY_SYS_INTR_CYPIPE_MUX_EP0,
            /* .epAddress            */  CY_IPC_EP_CYPIPE_CM0_ADDR,
            /* .epConfig             */  CY_SYS_CYPIPE_CONFIG_EP0
        },
    /* .ep1ConfigData */
        {
            /* .ipcNotifierNumber    */  CY_IPC_INTR_CYPIPE_EP1,
            /* .ipcNotifierPriority  */  CY_SYS_INTR_CYPIPE_PRIOR_EP1,
            /* .ipcNotifierMuxNumber */  0u,
            /* .epAddress            */  CY_IPC_EP_CYPIPE_CM4_ADDR,
            /* .epConfig             */  CY_SYS_CYPIPE_CONFIG_EP1
        },
    /* .endpointClientsCount     */  CY_SYS_CYPIPE_CLIENT_CNT,
    /* .endpointsCallbacksArray  */  systemIpcPipeSysCbArray,
    /* .userPipeIsrHandler       */  &Cy_SysIpcPipeIsrCm4
    };

    Cy_IPC_Pipe_Init(&systemIpcPipeConfigCm4);

#if defined(CY_DEVICE_PSOC6ABLE2)
    Cy_Flash_Init();
#endif /* defined(CY_DEVICE_PSOC6ABLE2) */

#endif /* defined(CY_IPC_DEFAULT_CFG_DISABLE) */

#if defined(CY_DEVICE_SECURE)
    /* Initialize Protected Register Access driver */
    Cy_PRA_Init();
#endif /* defined(CY_DEVICE_SECURE) */
}


int32_t tfm_ns_platform_init (void)
{
    Cy_Platform_Init();
    return 0;
}

/* [] END OF FILE */

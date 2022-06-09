/*
 * Copyright (c) 2018-2021 Arm Limited
 * Copyright (c) 2020 Nuvoton Technology Corp. All rights reserved.
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
 */

#include "cmsis.h"
#include "target_cfg.h"
#include "Driver_MPC.h"
#include "platform_description.h"
#include "device_definition.h"
#include "region_defs.h"
#include "tfm_plat_defs.h"
#include "region.h"
#include "tfm_secure_api.h"

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

/* The section names come from the scatter file */
REGION_DECLARE(Load$$LR$$, LR_NS_PARTITION, $$Base);
REGION_DECLARE(Image$$, ER_VENEER, $$Base);
REGION_DECLARE(Image$$, VENEER_ALIGN, $$Limit);
#ifdef BL2
REGION_DECLARE(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base);
#endif /* BL2 */

const struct memory_region_limits memory_regions = {
    .non_secure_code_start =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        BL2_HEADER_SIZE,

    .non_secure_partition_base =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base),

    .non_secure_partition_limit =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        NS_PARTITION_SIZE - 1,

    .veneer_base =
        (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),

    .veneer_limit =
        (uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit),

#ifdef BL2
    .secondary_partition_base =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base),

    .secondary_partition_limit =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base) +
        SECONDARY_PARTITION_SIZE - 1,
#endif /* BL2 */
};

/* Allows software, via SAU, to define the code region as a NSC */
#define NSCCFG_CODENSC  1

/* Import MPC driver */
extern ARM_DRIVER_MPC Driver_SRAM1_MPC, Driver_SRAM2_MPC;

/* Define Peripherals NS address range for the platform */
#define PERIPHERALS_BASE_NS_START (0x40000000)
#define PERIPHERALS_BASE_NS_END   (0x4FFFFFFF)

/* Enable system reset request for CPU 0 */
#define ENABLE_CPU0_SYSTEM_RESET_REQUEST (1U << 4U)

/* To write into AIRCR register, 0x5FA value must be write to the VECTKEY field,
 * otherwise the processor ignores the write.
 */
#define SCB_AIRCR_WRITE_MASK ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos))

/* Debug configuration flags */
#define SPNIDEN_SEL_STATUS (0x01u << 7)
#define SPNIDEN_STATUS     (0x01u << 6)
#define SPIDEN_SEL_STATUS  (0x01u << 5)
#define SPIDEN_STATUS      (0x01u << 4)
#define NIDEN_SEL_STATUS   (0x01u << 3)
#define NIDEN_STATUS       (0x01u << 2)
#define DBGEN_SEL_STATUS   (0x01u << 1)
#define DBGEN_STATUS       (0x01u << 0)

#define All_SEL_STATUS (SPNIDEN_SEL_STATUS | SPIDEN_SEL_STATUS | \
                        NIDEN_SEL_STATUS | DBGEN_SEL_STATUS)

struct platform_data_t tfm_peripheral_std_uart = {
        UART0_BASE + NS_OFFSET,
        UART0_BASE + NS_OFFSET + 0xFFF,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};

struct platform_data_t tfm_peripheral_uart1 = {
        UART1_BASE,
        UART1_BASE + 0xFFF,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};

struct platform_data_t tfm_peripheral_timer0 = {
        TMR01_BASE,
        TMR01_BASE + 0xFFF,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};

enum tfm_plat_err_t enable_fault_handlers(void)
{
    /* Secure fault is not present in the Baseline implementation. */
    /* Fault handler enable registers are not present in a Baseline
     * implementation.
     */
    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t system_reset_cfg(void)
{
    uint32_t reg_value = SCB->AIRCR;

    /* Clear SCB_AIRCR_VECTKEY value */
    reg_value &= ~(uint32_t)(SCB_AIRCR_VECTKEY_Msk);

    /* Enable system reset request for the secure world only */
    reg_value |= (uint32_t)(SCB_AIRCR_WRITE_MASK | SCB_AIRCR_SYSRESETREQS_Msk);

    SCB->AIRCR = reg_value;

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t init_debug(void)
{
    /* Set UART0 to NS for debug message */
    SCU_SET_PNSSET(UART0_Attr);

    return TFM_PLAT_ERR_SUCCESS;
}


/*----------------- NVIC interrupt target state to NS configuration ----------*/
enum tfm_plat_err_t nvic_interrupt_target_state_cfg(void)
{
    /* Target every interrupt to NS; unimplemented interrupts will be WI */
    for (uint8_t i=0; i<sizeof(NVIC->ITNS)/sizeof(NVIC->ITNS[0]); i++) {
        NVIC->ITNS[i] = 0xFFFFFFFF;
    }

#ifdef SECURE_UART1
    /* UART1 is a secure peripheral, so its IRQs have to target S state */
    NVIC_ClearTargetState(UARTRX1_IRQn);
    NVIC_ClearTargetState(UARTTX1_IRQn);
    NVIC_ClearTargetState(UART1_IRQn);
#endif

    return TFM_PLAT_ERR_SUCCESS;
}

/*----------------- NVIC interrupt enabling for S peripherals ----------------*/
enum tfm_plat_err_t nvic_interrupt_enable(void)
{

    return TFM_PLAT_ERR_SUCCESS;
}

/*------------------- SAU/IDAU configuration functions -----------------------*/
struct sau_cfg_t {
    uint32_t RBAR;
    uint32_t RLAR;
    bool nsc;
};

const struct sau_cfg_t sau_cfg[] = {
    {
        ((uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base)),
        ((uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        NS_PARTITION_SIZE - 1),
        false,
    },
    {
        NS_DATA_START,
        NS_DATA_LIMIT,
        false,
    },
    {
        (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),
        (uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit),
        true,
    },
    {
        (PERIPH_BASE + NS_OFFSET),
        (PERIPH_BASE + NS_OFFSET + 0x10000000 - 1),
        false,
    }
};

void sau_and_idau_cfg(void)
{
    uint32_t i;

    /* Enables SAU */
    TZ_SAU_Enable();

    for(i = 0; i < ARRAY_SIZE(sau_cfg); i++) {
        SAU->RNR = i;
        SAU->RBAR = sau_cfg[i].RBAR & SAU_RBAR_BADDR_Msk;
        SAU->RLAR = (sau_cfg[i].RLAR & SAU_RLAR_LADDR_Msk) |
            (sau_cfg[i].nsc ? SAU_RLAR_NSC_Msk : 0U) |
            SAU_RLAR_ENABLE_Msk;
    }

}

/*------------------- Memory configuration functions -------------------------*/

enum tfm_plat_err_t mpc_init_cfg(void)
{
    /* Secure COnfiguration Setup */
    SCU_Setup();

    /* Set UART0 for Non-secure */
    SCU_SET_PNSSET(UART0_Attr);

    /* Set TIMER2 for Non-secure */
    SCU_SET_PNSSET(TMR23_Attr);

    return TFM_PLAT_ERR_SUCCESS;
}
/*---------------------- PPC configuration functions -------------------------*/

enum tfm_plat_err_t ppc_init_cfg(void)
{
    return TFM_PLAT_ERR_SUCCESS;
}

void ppc_configure_to_secure(enum ppc_bank_e bank, uint32_t pos)
{
}

void ppc_configure_to_non_secure(enum ppc_bank_e bank, uint32_t pos)
{
}

void ppc_en_secure_unpriv(enum ppc_bank_e bank, uint16_t pos)
{
}

void ppc_clr_secure_unpriv(enum ppc_bank_e bank, uint16_t pos)
{
}

void ppc_clear_irq(void)
{
}

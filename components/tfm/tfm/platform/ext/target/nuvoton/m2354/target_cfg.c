/*
 * Copyright (c) 2017-2021 Arm Limited
 * Copyright (c) 2021 Nuvoton Technology Corp. All rights reserved.
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
#include "region_defs.h"
#include "tfm_plat_defs.h"
#include "region.h"
#include "NuMicro.h"

#ifdef PSA_API_TEST_IPC
#define PSA_FF_TEST_SECURE_UART2
#endif

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

    .veneer_base = (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),
    .veneer_limit = (uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit),

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
        UART0_BASE+NS_OFFSET,
        UART0_BASE+NS_OFFSET+0xFFF,
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

#ifdef PSA_API_TEST_IPC

/* Below data structure are only used for PSA FF tests, and this pattern is
 * definitely not to be followed for real life use cases, as it can break
 * security.
 */

struct platform_data_t
    tfm_peripheral_FF_TEST_UART_REGION = {
        UART2_BASE_S,
        UART2_BASE_S + 0xFFF,
        PPC_SP_APB_PPC_EXP2,
        CMSDK_UART2_APB_PPC_POS
};

struct platform_data_t
    tfm_peripheral_FF_TEST_WATCHDOG_REGION = {
        APB_WATCHDOG_BASE_S,
        APB_WATCHDOG_BASE_S + 0xFFF,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};

#define FF_TEST_NVMEM_REGION_START            0x102FFC00
#define FF_TEST_NVMEM_REGION_END              0x102FFFFF
#define FF_TEST_SERVER_PARTITION_MMIO_START   0x3801FC00
#define FF_TEST_SERVER_PARTITION_MMIO_END     0x3801FD00
#define FF_TEST_DRIVER_PARTITION_MMIO_START   0x3801FE00
#define FF_TEST_DRIVER_PARTITION_MMIO_END     0x3801FF00

struct platform_data_t
    tfm_peripheral_FF_TEST_NVMEM_REGION = {
        FF_TEST_NVMEM_REGION_START,
        FF_TEST_NVMEM_REGION_END,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};

struct platform_data_t
    tfm_peripheral_FF_TEST_SERVER_PARTITION_MMIO = {
        FF_TEST_SERVER_PARTITION_MMIO_START,
        FF_TEST_SERVER_PARTITION_MMIO_END,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};

struct platform_data_t
    tfm_peripheral_FF_TEST_DRIVER_PARTITION_MMIO = {
        FF_TEST_DRIVER_PARTITION_MMIO_START,
        FF_TEST_DRIVER_PARTITION_MMIO_END,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};
#endif

enum tfm_plat_err_t enable_fault_handlers(void)
{
    /* Explicitly set secure fault priority to the highest */
    NVIC_SetPriority(SCU_IRQn, 0);

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t system_reset_cfg(void)
{
    uint32_t reg_value = SCB->AIRCR;

    /* Clear SCB_AIRCR_VECTKEY value */
    reg_value &= ~(uint32_t)(SCB_AIRCR_VECTKEY_Msk);

    /* Enable system reset request only to the secure world */
    reg_value |= (uint32_t)(SCB_AIRCR_WRITE_MASK | SCB_AIRCR_SYSRESETREQS_Msk);

    SCB->AIRCR = reg_value;

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t init_debug(void)
{

#if defined(DAUTH_NONE)

    /* Disable secure and non-secure debug */
    DPM->NSCTL = 0x5a000000 | DPM_NSCTL_DBGDIS_Msk;

#elif defined(DAUTH_NS_ONLY)

    /* Disable secure debug */
    DPM->CTL = 0x5a000000 | DPM_CTL_DBGDIS_Msk;

#elif defined(DAUTH_FULL)
    /* By default, all debug is available */
    /* If secure or all debug is disable, it may need erase whole chip to alow debug again. */
#else

#if !defined(DAUTH_CHIP_DEFAULT)
#error "No debug authentication setting is provided."
#endif

    /* Set all the debug enable selector bits to 0 */


    /* No need to set any enable bits because the value depends on
     * input signals.
     */
#endif

    return TFM_PLAT_ERR_SUCCESS;
}

/*----------------- NVIC interrupt target state to NS configuration ----------*/
enum tfm_plat_err_t nvic_interrupt_target_state_cfg(void)
{
    /* Target every interrupt to NS; unimplemented interrupts will be WI */
    for (uint8_t i=0; i<sizeof(NVIC->ITNS)/sizeof(NVIC->ITNS[0]); i++) {
        NVIC->ITNS[i] = 0xFFFFFFFF;
    }

    /* Make sure that SCU are targeted to S state */
    NVIC_ClearTargetState(SCU_IRQn);

#ifdef SECURE_UART1
    /* UART1 is a secure peripheral, so its IRQs have to target S state */
    NVIC_ClearTargetState(UART1_IRQn);
#endif

    return TFM_PLAT_ERR_SUCCESS;
}

/*----------------- NVIC interrupt enabling for S peripherals ----------------*/
enum tfm_plat_err_t nvic_interrupt_enable(void)
{
    NVIC_EnableIRQ(SCU_IRQn);

#ifdef PSA_FF_TEST_SECURE_UART2
# error "Not support PSA_FF_TEST_SECURE_UART2 in M2354"    
#endif

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
    }
};

#define NR_SAU_INIT_STEP                 3

void sau_and_idau_cfg(void)
{
    uint32_t i;

    /* Enables SAU */
    TZ_SAU_Enable();

    for (i = 0; i < ARRAY_SIZE(sau_cfg); i++) {
        SAU->RNR = i;
        SAU->RBAR = sau_cfg[i].RBAR & SAU_RBAR_BADDR_Msk;
        SAU->RLAR = (sau_cfg[i].RLAR & SAU_RLAR_LADDR_Msk) |
                    (sau_cfg[i].nsc ? SAU_RLAR_NSC_Msk : 0U) |
                    SAU_RLAR_ENABLE_Msk;
    }

}

/*------------------- Memory configuration functions -------------------------*/
#ifdef BL2
#define NR_MPC_INIT_STEP                 7
#else
#define NR_MPC_INIT_STEP                 6
#endif

int32_t mpc_init_cfg(void)
{
    int32_t i;

    SCU->PNSSET[0] = SCU_INIT_PNSSET0_VAL;
    SCU->PNSSET[1] = SCU_INIT_PNSSET1_VAL;
    SCU->PNSSET[2] = SCU_INIT_PNSSET2_VAL;
    SCU->PNSSET[3] = SCU_INIT_PNSSET3_VAL;
    SCU->PNSSET[4] = SCU_INIT_PNSSET4_VAL;
    SCU->PNSSET[5] = SCU_INIT_PNSSET5_VAL;
    SCU->PNSSET[6] = SCU_INIT_PNSSET6_VAL;

    SCU->IONSSET[0] = SCU_INIT_IONSSET0_VAL;
    SCU->IONSSET[1] = SCU_INIT_IONSSET1_VAL;
    SCU->IONSSET[2] = SCU_INIT_IONSSET2_VAL;
    SCU->IONSSET[3] = SCU_INIT_IONSSET3_VAL;
    SCU->IONSSET[4] = SCU_INIT_IONSSET4_VAL;
    SCU->IONSSET[5] = SCU_INIT_IONSSET5_VAL;
    SCU->IONSSET[6] = SCU_INIT_IONSSET6_VAL;
    SCU->IONSSET[7] = SCU_INIT_IONSSET7_VAL;

    /* Set Non-secure SRAM */
    for(i = 15; i >= SCU_SECURE_SRAM_SIZE / 16384; i--)
    {
        SCU->SRAMNSSET |= (1U << i);
    }

    /* Set interrupt to non-secure according to PNNSET settings */
    if(SCU_INIT_PNSSET0_VAL & BIT9) NVIC->ITNS[1] |= BIT22; /* Int of USBH_INT     */
    if(SCU_INIT_PNSSET0_VAL & BIT13) NVIC->ITNS[2] |= BIT0; /* Int of SDHOST0_INT  */
    if(SCU_INIT_PNSSET0_VAL & BIT24) NVIC->ITNS[3] |= BIT2; /* Int of PDMA1_INT    */
    if(SCU_INIT_PNSSET1_VAL & BIT18) NVIC->ITNS[2] |= BIT7; /* Int of CRYPTO       */
    if(SCU_INIT_PNSSET2_VAL & BIT2) NVIC->ITNS[3] |= BIT15; /* Int of EWDT_INT     */
    if(SCU_INIT_PNSSET2_VAL & BIT2) NVIC->ITNS[3] |= BIT16; /* Int of EWWDT_INT    */
    if(SCU_INIT_PNSSET2_VAL & BIT3) NVIC->ITNS[1] |= BIT10; /* Int of EADC0_INT    */
    if(SCU_INIT_PNSSET2_VAL & BIT3) NVIC->ITNS[1] |= BIT11; /* Int of EADC1_INT    */
    if(SCU_INIT_PNSSET2_VAL & BIT3) NVIC->ITNS[1] |= BIT14; /* Int of EADC2_INT    */
    if(SCU_INIT_PNSSET2_VAL & BIT3) NVIC->ITNS[1] |= BIT15; /* Int of EADC3_INT    */
    if(SCU_INIT_PNSSET2_VAL & BIT5) NVIC->ITNS[1] |= BIT12; /* Int of ACMP01_INT   */
    if(SCU_INIT_PNSSET2_VAL & BIT7) NVIC->ITNS[1] |= BIT9; /* Int of DAC_INT      */
    if(SCU_INIT_PNSSET2_VAL & BIT8) NVIC->ITNS[2] |= BIT4; /* Int of I2S0_INT     */
    if(SCU_INIT_PNSSET2_VAL & BIT13) NVIC->ITNS[1] |= BIT23; /* Int of USBOTG_INT   */
    if(SCU_INIT_PNSSET2_VAL & BIT17) NVIC->ITNS[1] |= BIT2; /* Int of TMR2_INT     */
    if(SCU_INIT_PNSSET2_VAL & BIT17) NVIC->ITNS[1] |= BIT3; /* Int of TMR3_INT     */
    if(SCU_INIT_PNSSET2_VAL & BIT18) NVIC->ITNS[3] |= BIT18; /* Int of TMR4_INT     */
    if(SCU_INIT_PNSSET2_VAL & BIT18) NVIC->ITNS[3] |= BIT19; /* Int of TMR5_INT     */
    if(SCU_INIT_PNSSET2_VAL & BIT24) NVIC->ITNS[0] |= BIT25; /* Int of EPWM0_P0_INT */
    if(SCU_INIT_PNSSET2_VAL & BIT24) NVIC->ITNS[0] |= BIT26; /* Int of EPWM0_P1_INT */
    if(SCU_INIT_PNSSET2_VAL & BIT24) NVIC->ITNS[0] |= BIT27; /* Int of EPWM0_P2_INT */
    if(SCU_INIT_PNSSET2_VAL & BIT25) NVIC->ITNS[0] |= BIT29; /* Int of EPWM1_P0_INT */
    if(SCU_INIT_PNSSET2_VAL & BIT25) NVIC->ITNS[0] |= BIT30; /* Int of EPWM1_P1_INT */
    if(SCU_INIT_PNSSET2_VAL & BIT25) NVIC->ITNS[0] |= BIT31; /* Int of EPWM1_P2_INT */
    if(SCU_INIT_PNSSET2_VAL & BIT26) NVIC->ITNS[2] |= BIT14; /* Int of BPWM0_INT    */
    if(SCU_INIT_PNSSET2_VAL & BIT27) NVIC->ITNS[2] |= BIT15; /* Int of BPWM1_INT    */
    if(SCU_INIT_PNSSET3_VAL & BIT0) NVIC->ITNS[0] |= BIT22; /* Int of QSPI0_INT    */
    if(SCU_INIT_PNSSET3_VAL & BIT1) NVIC->ITNS[0] |= BIT23; /* Int of SPI0_INT     */
    if(SCU_INIT_PNSSET3_VAL & BIT2) NVIC->ITNS[1] |= BIT19; /* Int of SPI1_INT     */
    if(SCU_INIT_PNSSET3_VAL & BIT3) NVIC->ITNS[1] |= BIT20; /* Int of SPI2_INT     */
    if(SCU_INIT_PNSSET3_VAL & BIT4) NVIC->ITNS[1] |= BIT30; /* Int of SPI3_INT     */
    if(SCU_INIT_PNSSET3_VAL & BIT16) NVIC->ITNS[1] |= BIT4; /* Int of UART0_INT    */
    if(SCU_INIT_PNSSET3_VAL & BIT17) NVIC->ITNS[1] |= BIT5; /* Int of UART1_INT    */
    if(SCU_INIT_PNSSET3_VAL & BIT18) NVIC->ITNS[1] |= BIT16; /* Int of UART2_INT    */
    if(SCU_INIT_PNSSET3_VAL & BIT19) NVIC->ITNS[1] |= BIT17; /* Int of UART3_INT    */
    if(SCU_INIT_PNSSET3_VAL & BIT20) NVIC->ITNS[2] |= BIT10; /* Int of UART4_INT    */
    if(SCU_INIT_PNSSET3_VAL & BIT21) NVIC->ITNS[2] |= BIT11; /* Int of UART5_INT    */
    if(SCU_INIT_PNSSET4_VAL & BIT0) NVIC->ITNS[1] |= BIT6; /* Int of I2C0_INT     */
    if(SCU_INIT_PNSSET4_VAL & BIT1) NVIC->ITNS[1] |= BIT7; /* Int of I2C1_INT     */
    if(SCU_INIT_PNSSET4_VAL & BIT2) NVIC->ITNS[2] |= BIT18; /* Int of I2C2_INT     */
    if(SCU_INIT_PNSSET4_VAL & BIT16) NVIC->ITNS[1] |= BIT26; /* Int of SC0_INT      */
    if(SCU_INIT_PNSSET4_VAL & BIT17) NVIC->ITNS[1] |= BIT27; /* Int of SC1_INT      */
    if(SCU_INIT_PNSSET4_VAL & BIT18) NVIC->ITNS[1] |= BIT28; /* Int of SC2_INT      */
    if(SCU_INIT_PNSSET5_VAL & BIT0) NVIC->ITNS[1] |= BIT24; /* Int of CAN0_INT     */
    if(SCU_INIT_PNSSET5_VAL & BIT16) NVIC->ITNS[2] |= BIT20; /* Int of QEI0_INT     */
    if(SCU_INIT_PNSSET5_VAL & BIT17) NVIC->ITNS[2] |= BIT21; /* Int of QEI1_INT     */
    if(SCU_INIT_PNSSET5_VAL & BIT20) NVIC->ITNS[2] |= BIT22; /* Int of ECAP0_INT    */
    if(SCU_INIT_PNSSET5_VAL & BIT21) NVIC->ITNS[2] |= BIT23; /* Int of ECAP1_INT    */
    if(SCU_INIT_PNSSET5_VAL & BIT25) NVIC->ITNS[3] |= BIT5; /* Int of TRNG_INT     */
    if(SCU_INIT_PNSSET5_VAL & BIT27) NVIC->ITNS[3] |= BIT4; /* Int of LCD_INT      */
    if(SCU_INIT_PNSSET6_VAL & BIT0) NVIC->ITNS[1] |= BIT21; /* Int of USBD_INT     */
    if(SCU_INIT_PNSSET6_VAL & BIT16) NVIC->ITNS[2] |= BIT12; /* Int of USCI0_INT    */
    if(SCU_INIT_PNSSET6_VAL & BIT17) NVIC->ITNS[2] |= BIT13; /* Int of USCI1_INT    */
    if(SCU_INIT_IONSSET_VAL & BIT0) NVIC->ITNS[0] |= BIT16; /* Int of PA           */
    if(SCU_INIT_IONSSET_VAL & BIT1) NVIC->ITNS[0] |= BIT17; /* Int of PB           */
    if(SCU_INIT_IONSSET_VAL & BIT2) NVIC->ITNS[0] |= BIT18; /* Int of PC           */
    if(SCU_INIT_IONSSET_VAL & BIT3) NVIC->ITNS[0] |= BIT19; /* Int of PD           */
    if(SCU_INIT_IONSSET_VAL & BIT4) NVIC->ITNS[0] |= BIT20; /* Int of PE           */
    if(SCU_INIT_IONSSET_VAL & BIT5) NVIC->ITNS[0] |= BIT21; /* Int of PF           */
    if(SCU_INIT_IONSSET_VAL & BIT6) NVIC->ITNS[2] |= BIT8; /* Int of PG           */
    if(SCU_INIT_IONSSET_VAL & BIT7) NVIC->ITNS[2] |= BIT24; /* Int of PH           */
    if(SCU_INIT_IONSSET_VAL & BIT8) NVIC->ITNS[0] |= BIT10; /* Int of EINT0        */
    if(SCU_INIT_IONSSET_VAL & BIT9) NVIC->ITNS[0] |= BIT11; /* Int of EINT1        */
    if(SCU_INIT_IONSSET_VAL & BIT10) NVIC->ITNS[0] |= BIT12; /* Int of EINT2        */
    if(SCU_INIT_IONSSET_VAL & BIT11) NVIC->ITNS[0] |= BIT13; /* Int of EINT3        */
    if(SCU_INIT_IONSSET_VAL & BIT12) NVIC->ITNS[0] |= BIT14; /* Int of EINT4        */
    if(SCU_INIT_IONSSET_VAL & BIT13) NVIC->ITNS[0] |= BIT15; /* Int of EINT5        */
    if(SCU_INIT_IONSSET_VAL & BIT14) NVIC->ITNS[2] |= BIT9;  /* Int of EINT6        */
    if(SCU_INIT_IONSSET_VAL & BIT15) NVIC->ITNS[2] |= BIT25; /* Int of EINT7        */

    /* Enable SCU Int status */
    SCU->SVIOIEN = (uint32_t)(-1);
    SCU->PVIOIEN = (uint32_t)(-1);
    NVIC_EnableIRQ(SCU_IRQn);

    /* Set UART0 for Non-secure */
    SCU_SET_PNSSET(UART0_Attr);

    /* Set TIMER2 for Non-secure */
    SCU_SET_PNSSET(TMR23_Attr);

    /* Add barriers to assure the MPC configuration is done before continue
     * the execution.
     */
    __DSB();
    __ISB();

    return ARM_DRIVER_OK;
}

/*---------------------- PPC configuration functions -------------------------*/
#define NR_PPC_INIT_STEP                 4

void ppc_init_cfg(void)
{
}

void ppc_configure_to_non_secure(enum ppc_bank_e bank, uint16_t pos)
{
}

void ppc_configure_to_secure(enum ppc_bank_e bank, uint16_t pos)
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

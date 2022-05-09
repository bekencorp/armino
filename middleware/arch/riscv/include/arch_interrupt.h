// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __ARCH_INTERRUPT_H__
#define __ARCH_INTERRUPT_H__


#include "ae350.h"
#include "core_v5.h"
#include <common/bk_include.h>
#include <common/bk_err.h>
#include <driver/hal/hal_int_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define USE_PLIC         (1)

/*
 * Define 'NDS_PLIC_BASE' and 'NDS_PLIC_SW_BASE' before include platform
 * intrinsic header file to active PLIC/PLIC_SW related intrinsic functions.
 */
#define NDS_PLIC_BASE        PLIC_BASE
#define NDS_PLIC_SW_BASE     PLIC_SW_BASE

/*
 * CPU Machine timer control
 */
#define HAL_MTIMER_INITIAL()
#define HAL_MTIME_ENABLE()                              set_csr(NDS_MIE, MIP_MTIP)
#define HAL_MTIME_DISABLE()                             clear_csr(NDS_MIE, MIP_MTIP);

/*
 * CPU Machine SWI control
 *
 * Machine SWI (MSIP) is connected to PLIC_SW source 1.
 */
#define HAL_MSWI_INITIAL()                              \
{                                                       \
        __nds__plic_sw_set_priority(1, 1);              \
        __nds__plic_sw_enable_interrupt(1);             \
}
#define HAL_MSWI_ENABLE()                               set_csr(NDS_MIE, MIP_MSIP)
#define HAL_MSWI_DISABLE()                              clear_csr(NDS_MIE, MIP_MTIP)
#define HAL_MSWI_PENDING()                              __nds__plic_sw_set_pending(1)
#define HAL_MSWI_CLEAR()                                __nds__plic_sw_claim_interrupt()

/*
 * Platform defined interrupt controller access
 *
 * This uses the vectored PLIC scheme.
 */
#define HAL_MEIP_ENABLE()                               set_csr(NDS_MIE, MIP_MEIP)
#define HAL_MEIP_DISABLE()                              clear_csr(NDS_MIE, MIP_MEIP)
#define HAL_INTERRUPT_ENABLE(vector)                    __nds__plic_enable_interrupt(vector)
#define HAL_INTERRUPT_DISABLE(vector)                   __nds__plic_disable_interrupt(vector)
#define HAL_INTERRUPT_THRESHOLD(threshold)              __nds__plic_set_threshold(threshold)
#define HAL_INTERRUPT_SET_LEVEL(vector, level)          __nds__plic_set_priority(vector, level)

/*
 * Vectored based inline interrupt attach and detach control
 */
extern int __vectors[];
extern void default_irq_entry(void);

#define HAL_INLINE_INTERRUPT_ATTACH(vector, isr)        { __vectors[vector] = (int)isr; }
#define HAL_INLINE_INTERRUPT_DETACH(vector, isr)        { if ( __vectors[vector] == (int)isr ) __vectors[vector] = (int)default_irq_entry; }

/*
 * Inline nested interrupt entry/exit macros
 */
/* Save/Restore macro */
#define SAVE_CSR(r)                                     long __##r = read_csr(r);
#define RESTORE_CSR(r)                                  write_csr(r, __##r);

#if SUPPORT_PFT_ARCH
#define SAVE_MXSTATUS()                                 SAVE_CSR(NDS_MXSTATUS)
#define RESTORE_MXSTATUS()                              RESTORE_CSR(NDS_MXSTATUS)
#else
#define SAVE_MXSTATUS()
#define RESTORE_MXSTATUS()
#endif

#ifdef __riscv_flen
#define SAVE_FCSR()                                     int __fcsr = read_fcsr();
#define RESTORE_FCSR()                                  write_fcsr(__fcsr);
#else
#define SAVE_FCSR()
#define RESTORE_FCSR()
#endif

#ifdef __riscv_dsp
#define SAVE_UCODE()                                    SAVE_CSR(NDS_UCODE)
#define RESTORE_UCODE()                                 RESTORE_CSR(NDS_UCODE)
#else
#define SAVE_UCODE()
#define RESTORE_UCODE()
#endif

/* Nested IRQ entry macro : Save CSRs and enable global interrupt. */
#define NESTED_IRQ_ENTER()                              \
        SAVE_CSR(NDS_MEPC)                              \
        SAVE_CSR(NDS_MSTATUS)                           \
        SAVE_MXSTATUS()                                 \
        SAVE_FCSR()                                     \
        SAVE_UCODE()                                    \
        set_csr(NDS_MSTATUS, MSTATUS_MIE);

/* Nested IRQ exit macro : Restore CSRs */
#define NESTED_IRQ_EXIT()                               \
        clear_csr(NDS_MSTATUS, MSTATUS_MIE);            \
        RESTORE_CSR(NDS_MSTATUS)                        \
        RESTORE_CSR(NDS_MEPC)                           \
        RESTORE_MXSTATUS()                              \
        RESTORE_FCSR()                                  \
        RESTORE_UCODE()


/*Define INT ID */
typedef enum {
	INT_ID_NULL0_INT          = 0,
	INT_ID_EIP130_0_IRQ          ,
	INT_ID_EIP130_0_SEC_IRQ      ,
	INT_ID_TIMER                 ,
	INT_ID_UART0                 ,
	INT_ID_PWM                   ,
	INT_ID_I2C                   ,
	INT_ID_SPI                   ,
	INT_ID_SADC                  ,
	INT_ID_IRDA                  ,
	INT_ID_SDIO                  ,
	INT_ID_GDMA                  ,
	INT_ID_LA                    ,
	INT_ID_TIMER1                ,
	INT_ID_I2C1                  ,
	INT_ID_UART1                 ,
	INT_ID_UART2                 ,
	INT_ID_SPI1                  ,
	INT_ID_CAN                   ,
	INT_ID_USB                   ,
	INT_ID_QSPI                  ,
	INT_ID_FFT                   ,
	INT_ID_SBC                   ,
	INT_ID_AUD                   ,
	INT_ID_I2S                   ,
	INT_ID_JPEGENC               ,
	INT_ID_JPEGDEC               ,
	INT_ID_LCD                   ,
	INT_ID_NULL1_INT             ,
	INT_ID_INT_PHY               ,
	INT_ID_MAC_TX_RX_TIMER_N     ,
	INT_ID_MAC_TX_RX_MISC_N      ,
	INT_ID_MAC_RX_TRIGGER_N      ,
	INT_ID_MAC_TX_TRIGGER_N      ,
	INT_ID_MAC_PORT_TRIGGER_N    ,
	INT_ID_MAC_GEN_N             ,
	INT_ID_HSU_IRQ               ,
	INT_ID_INT_MAC_WAKEUP        ,
	INT_ID_NULL2_INT             ,
	INT_ID_DM                    ,
	INT_ID_BLE                   ,
	INT_ID_BT                    ,
	INT_ID_NULL3_INT             ,
	INT_ID_NULL4_INT             ,
	INT_ID_NULL5_INT             ,
	INT_ID_NULL6_INT             ,
	INT_ID_NULL7_INT             ,
	INT_ID_NULL8_INT             ,
	INT_ID_MBOX0                 ,
	INT_ID_MBOX1                 ,
	INT_ID_BMC64                 ,
	INT_ID_NULL9_INT             ,
	INT_ID_TOUCHED               ,
	INT_ID_USBPLUG               ,
	INT_ID_RTC                   ,
	INT_ID_GPIO                  ,
	INT_ID_MAX
} arch_int_src_t;


typedef void (*interrupt_handle_p)();


bk_err_t arch_isr_entry_init(void);
void arch_interrupt_set_priority(arch_int_src_t int_number, uint32_t int_priority);
void arch_interrupt_register_int(arch_int_src_t int_number, int_group_isr_t isr_callback);
void arch_interrupt_unregister_int(arch_int_src_t int_number);
unsigned int arch_get_interrupt_nest_cnt(void);
unsigned int arch_is_enter_exception(void);
void mext_interrupt(void);

#endif	/* __ARCH_INTERRUPT_H__ */


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


#include "core_v5.h"
#include <common/bk_include.h>
#include <common/bk_err.h>
#include <driver/hal/hal_int_types.h>

#include "rv_interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif

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
typedef void (*hook_func)(void);

bk_err_t arch_isr_entry_init(void);
void arch_interrupt_set_priority(arch_int_src_t int_number, uint32_t int_priority);
void arch_interrupt_register_int(arch_int_src_t int_number, int_group_isr_t isr_callback);
void arch_interrupt_unregister_int(arch_int_src_t int_number);
unsigned int arch_get_interrupt_nest_cnt(void);
unsigned int arch_is_enter_exception(void);
void mext_interrupt(void);
bool mtimer_is_timeout();
uint64_t arch_get_plic_pending_status(void);
void rtos_regist_wifi_dump_hook(hook_func wifi_func);
void rtos_regist_ble_dump_hook(hook_func ble_func);
void rtos_regist_plat_dump_hook(uint32_t reg_base_addr, uint32_t reg_size);
void rtos_dump_plat_sys_regs(void);

#endif	/* __ARCH_INTERRUPT_H__ */


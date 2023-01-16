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
#include <stdio.h>
#include "arch_interrupt.h"
#include "platform.h"
#include "BK7256_RegList.h"

#define INT_NUMBER_MAX              (64)

#define INT_PRIORITY_PLIC            0x01
#define INT_PRIORITY_MASK            0x07

#define REG_PLIC_PENDING_STATUS_GROUP1 (0xe4001000)	//we can optimize this code with PLIC driver.
#define REG_PLIC_PENDING_STATUS_GROUP2 (0xe4001004)


#define CONFIG_NEST_INT_TEST  0

interrupt_handle_p p_intn_func_handle[INT_NUMBER_MAX];

volatile unsigned int g_enter_interrupt_nest = 0;

unsigned int arch_get_interrupt_nest_cnt(void) {
	return g_enter_interrupt_nest;
}

#if (CONFIG_NEST_INT_TEST)
#define INT_SRC_UART0           (4)
#define SYS_DELAY_TIME_1MS	    (17000UL)

volatile unsigned int g_test_nest_count = 1;
extern void sys_delay_sync(uint32_t time_count );
#endif

/* At the time of writing, interrupt nesting is not supported, so do not use
the default mext_interrupt() implementation as that enables interrupts.  A
version that does not enable interrupts is provided below.  THIS INTERRUPT
HANDLER IS SPECIFIC TO FREERTOS WHICH USES PLIC! */
void mext_interrupt_sub(void)
{
	unsigned int irq_source = HAL_IRQ_CLAIM(); /// __nds__plic_claim_interrupt();
	
	g_enter_interrupt_nest++;

#if (CONFIG_NEST_INT_TEST)
	if (g_enter_interrupt_nest > 1 &&  g_test_nest_count > 0)
	{
		os_printf("mext_interrupt: nest count %d, irq_source = %u.\r\n", g_enter_interrupt_nest, irq_source);
		g_test_nest_count--;
	}
#endif //#if (CONFIG_NEST_INT_TEST)
		
#if (CONFIG_NEST_INT_SUPPORT)
	/* Disable the timer interrupts before
	   Enable interrupts in general to allow nested
	*/
	HAL_TI_DISABLE();

	/* Enable interrupts in general to allow nested */
	HAL_INT_ENABLE();
	
#endif // #if (CONFIG_NEST_INT_SUPPORT)

	if (irq_source != 0 && irq_source < INT_NUMBER_MAX
		&& p_intn_func_handle[irq_source] != NULL) {

		/* Do interrupt handler */
		p_intn_func_handle[irq_source]();

#if (CONFIG_NEST_INT_TEST)
		if (irq_source != INT_SRC_UART0 && g_test_nest_count != 0)
		{
			sys_delay_sync(SYS_DELAY_TIME_1MS);
		}
#endif //#if (CONFIG_NEST_INT_TEST)

	} else {
		//print irq status
		os_printf("ERROR: func %s, irq_source = %u.\r\n", __func__, irq_source);
		if (irq_source < INT_NUMBER_MAX) {
			os_printf("ERROR: func %s, p_intn_func_handle = %p.\r\n", __func__, p_intn_func_handle[irq_source]);
		}
	}

#if (CONFIG_NEST_INT_SUPPORT)
	/* Disable interrupt in general to restore context */
	HAL_INT_DISABLE();

	/* Restore the timer interrupts after
	   interrupts complete handle
	*/
	if (g_enter_interrupt_nest == 1) {
		HAL_TI_ENABLE();
	}
#endif // #if (CONFIG_NEST_INT_SUPPORT)

	g_enter_interrupt_nest--;
	
	HAL_IRQ_COMPLETE(irq_source);
}

extern u32 arch_get_int_status(void);
void mext_interrupt(void)
{
	while(arch_get_int_status() & UIP_UEIP)
	{
		mext_interrupt_sub();
	}
}

void arch_interrupt_set_priority(arch_int_src_t int_number, uint32_t int_priority)
{
	if(int_number > 0 && int_number < INT_NUMBER_MAX) {
		//set Priority for int.  Priority level must be set > 0 and <=7
		HAL_IRQ_SET_PRIORITY(int_number, int_priority & INT_PRIORITY_MASK);
	}

	return;
}

void arch_interrupt_register_int(arch_int_src_t int_number, int_group_isr_t isr_callback)
{
    if((int_number > (INT_ID_MAX-1))|| isr_callback == NULL)
    {
	    return;
    }
	
	p_intn_func_handle[int_number] = isr_callback;

	HAL_IRQ_ENABLE(int_number);
}
void arch_interrupt_unregister_int(arch_int_src_t int_number)
{
    if(int_number > (INT_ID_MAX-1))
    {
	    return;
    }
	
	p_intn_func_handle[int_number] = NULL;
	HAL_IRQ_DISABLE(int_number);
	arch_interrupt_set_priority(int_number, 0);
}

extern void riscv_set_mtimercmp(u64 new_time);
extern u64 riscv_get_mtimer(void);
extern u64 riscv_get_mtimercmp(void);

//specify:low voltage process can't be interrupt or the system can't response external interrupt after wakeup.
void mtimer_reset_next_tick(uint32_t minimum_offset)
{
	u64 time_cmp = riscv_get_mtimercmp();
	u64 cur_time = riscv_get_mtimer();
	if(time_cmp < cur_time + minimum_offset)
	{
		riscv_set_mtimercmp(cur_time + minimum_offset);
	}
}

bool mtimer_is_timeout()
{
	u64 time_cmp = riscv_get_mtimercmp();
	u64 cur_time = riscv_get_mtimer();
	return (time_cmp <= cur_time);
}

void arch_interrupt_init(void)
{
	HAL_INT_DISABLE();  /* Disable interrupts in general. */

	HAL_SI_DISABLE();		/* software interrupt. */
	HAL_EI_ENABLE();		/* external interrupt. */
	HAL_TI_ENABLE();		/* timer interrupt. */

#if (CONFIG_NEST_INT_TEST)
//	arch_interrupt_set_priority(INT_SRC_UART0, 2);   // should update the priority in ICU_MP
#endif //#if (CONFIG_NEST_INT_TEST)
}

bk_err_t arch_isr_entry_init(void)
{
	arch_interrupt_init();
	
	return BK_OK;
}

uint64_t arch_get_plic_pending_status(void)
{
	uint64_t plic_pend1 = *(volatile uint32 *)(REG_PLIC_PENDING_STATUS_GROUP1);
	uint64_t plic_pend2 = *(volatile uint32 *)(REG_PLIC_PENDING_STATUS_GROUP2);
	uint64_t plic_pending_status = (plic_pend2 << 32) | plic_pend1;

	return plic_pending_status;
}


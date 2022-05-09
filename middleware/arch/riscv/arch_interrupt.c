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
#define MACHINE_TIMER_PERIOD        ((2 * CPUFREQ)/5000)          // 0.002 seconds

#define INT_PRIORITY_PLIC            0x01
#define INT_PRIORITY_MASK            0x07

#define NESTED_VPLIC_COMPLETE_INTERRUPT(irq)            \
do {                                                    \
	clear_csr(NDS_MIE, MIP_MEIP);                       \
	__nds__plic_complete_interrupt(irq);                \
	__asm volatile("fence io, io");                     \
	set_csr(NDS_MIE, MIP_MEIP);                         \
} while(0)

#define CONFIG_NEST_INT_TEST  0

interrupt_handle_p p_intn_func_handle[INT_NUMBER_MAX];

/* The flag to trigger UART output message */
static volatile char trigger_mswi_uart_msg_flag = 0;

/* The flag to trigger UART output message */
static volatile char trigger_pit_uart_msg_flag = 0;

// extern volatile char trigger_mswi_flag  ;

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
void mext_interrupt(void)
{
	unsigned int irq_source = __nds__plic_claim_interrupt();
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
	clear_csr(NDS_MIE, MIP_MTIP);

	/* Enable interrupts in general to allow nested */
	set_csr(NDS_MSTATUS, MSTATUS_MIE);
#endif // #if (CONFIG_NEST_INT_SUPPORT)

	if (irq_source != 0 && irq_source < INT_NUMBER_MAX
		&& p_intn_func_handle[irq_source] != NULL) {

		#if( configUSE_ANDES_TRACER == 1 )
			traceISR_ENTER(irq_source);
		#endif

		/* Do interrupt handler */
		p_intn_func_handle[irq_source]();

#if (CONFIG_NEST_INT_TEST)
		if (irq_source != INT_SRC_UART0 && g_test_nest_count != 0)
		{
			sys_delay_sync(SYS_DELAY_TIME_1MS);
		}
#endif //#if (CONFIG_NEST_INT_TEST)

		#if( configUSE_ANDES_TRACER == 1 )
			traceISR_EXIT();
		#endif
 
	} else {
		//print irq status
		os_printf("ERROR: func %s, irq_source = %u.\r\n", __func__, irq_source);
		if (irq_source < INT_NUMBER_MAX) {
			os_printf("ERROR: func %s, p_intn_func_handle = %p.\r\n", __func__, p_intn_func_handle[irq_source]);
		}
	}

#if (CONFIG_NEST_INT_SUPPORT)
	/* Disable interrupt in general to restore context */
	clear_csr(NDS_MSTATUS, MSTATUS_MIE);

	/* Restore the timer interrupts after
	   interrupts complete handle
	*/
	if (g_enter_interrupt_nest == 1) {
		set_csr(NDS_MIE, MIP_MTIP);
	}
#endif // #if (CONFIG_NEST_INT_SUPPORT)

	g_enter_interrupt_nest--;
	__nds__plic_complete_interrupt(irq_source);
}


void arch_interrupt_enable(void)
{
	volatile unsigned int i = 0;
	for (i = 1; i < INT_NUMBER_MAX; i++)
	{
		HAL_INTERRUPT_SET_LEVEL(i, INT_PRIORITY_PLIC);   //set default Priority.  Priority level must be set > 0
		HAL_INTERRUPT_ENABLE(i);         // Enable PLIC interrupt  source
	}

#if (CONFIG_NEST_INT_TEST)
	arch_interrupt_set_priority(INT_SRC_UART0, 2);
#endif //#if (CONFIG_NEST_INT_TEST)
}

void arch_interrupt_set_priority(arch_int_src_t int_number, uint32_t int_priority)
{
	if(int_number > 0 && int_number < INT_NUMBER_MAX) {
		//set Priority for int.  Priority level must be set > 0 and <=7
		HAL_INTERRUPT_SET_LEVEL(int_number, int_priority & INT_PRIORITY_MASK);
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
}
void arch_interrupt_unregister_int(arch_int_src_t int_number)
{
    if(int_number > (INT_ID_MAX-1))
    {
	    return;
    }
	
	p_intn_func_handle[int_number] = NULL;
}
void setup_mtimer(void)
{
	HAL_MTIMER_INITIAL();

	/* Active machine timer */
	DEV_PLMT->MTIMECMP = DEV_PLMT->MTIME + MACHINE_TIMER_PERIOD;

	HAL_MTIME_ENABLE();
}

void setup_mswi(void)
{
	HAL_MSWI_INITIAL();
	HAL_MSWI_ENABLE();
}


void trigger_mswi(void)
{
	/* Trigger Machine software interrupt */
	HAL_MSWI_PENDING();
}

/////////////////////////////msi/////////////////////////////////////////////////////////////
void mswi_handler(void)
{
	HAL_MSWI_CLEAR();

	/*
	 * It is triggered by Machine mtime interrupt handler.
	 * Output messgaes to indicate Machine mtime is alive.
	 */
	if(trigger_mswi_uart_msg_flag) {
		/* Clear UART output message trigger flag */
		trigger_mswi_uart_msg_flag = 0;

		//printf("Message triggered from machine time interrupt\n");
	}

	/*
	 * It is triggered by PIT interrupt handler.
	 * Output messgaes to indicate PLIC PIT is alive.
	 */
	if(trigger_pit_uart_msg_flag) {
		/* Clear UART output message trigger flag */
	 	trigger_pit_uart_msg_flag = 0;

		//printf("Message triggered from PLIC PIT interrupt\n");
	}
}

/////////////////////////mti////////////////////////////
void mtime_handler(void)
{
	/* Disable the timer interrupt to prevent reentry */
	HAL_MTIME_DISABLE();

	/* Reset the timer to specified period */
	DEV_PLMT->MTIMECMP = DEV_PLMT->MTIME + MACHINE_TIMER_PERIOD;

	/* Enable interrupts in general to allow nested */
	set_csr(NDS_MSTATUS, MSTATUS_MIE);

	/*
	 * Set the flag to trigger Machine software interrupt.
	 * The Machine SWI interrupt handler will output message to indicate
	 * Machine mtime is alive.
	 */
	// trigger_mswi_flag = 1;
	trigger_mswi_uart_msg_flag = 1;

	/* Re-enable the timer interrupt. */
	HAL_MTIME_ENABLE();
}
void arch_interrupt_init(void)
{

    /* Disable the Machine external, timer and software interrupts until setup is done */
	clear_csr(NDS_MIE, MIP_MEIP | MIP_MTIP | MIP_MSIP);
	clear_csr(NDS_MSTATUS, MSTATUS_MIE);	 /* Disable interrupts in general. */

	/* Setup Machine mtime and software interrupt */
	setup_mtimer();
	setup_mswi();
	arch_interrupt_enable() ; //irq enabler

    /* Disable the Machine external, timer and software interrupts until setup is done */
	clear_csr(NDS_MIE, MIP_MEIP | MIP_MTIP | MIP_MSIP);
	clear_csr(NDS_MSTATUS, MSTATUS_MIE);	 /* Enable interrupts in general. */

	// set_csr(NDS_MIE, MIP_MEIP | MIP_MTIP | MIP_MSIP);  /* Enable the Machine External/Timer/Sofware interrupt bit in MIE. */
	//set_csr(NDS_MSTATUS, MSTATUS_MIE);	 /* Enable interrupts in general. */
}
bk_err_t arch_isr_entry_init(void)
{
    // arch_interrupt_enable();
	arch_interrupt_init();
	return BK_OK;
}

#ifdef INT_VECTOR_EN
void __attribute__ ((interrupt)) entry_irq1(void)
{
	NESTED_IRQ_ENTER();
    if(p_intn_func_handle[1] != NULL)
	    (*(p_intn_func_handle[1]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(1);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq2(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[2] != NULL)
	    (*(p_intn_func_handle[2]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(2);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq3(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[3] != NULL)
	    (*(p_intn_func_handle[3]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(3);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq4(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[4] != NULL)
	    (*(p_intn_func_handle[4]))() ;
	NESTED_VPLIC_COMPLETE_INTERRUPT(4);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq5(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[5] != NULL)
	    (*(p_intn_func_handle[5]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(5);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq6(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[6] != NULL)
	    (*(p_intn_func_handle[6]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(6);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq7(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[7] != NULL)
	    (*(p_intn_func_handle[7]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(7);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq8(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[8] != NULL)
	    (*(p_intn_func_handle[8]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(8);
	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq9(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[9] != NULL)
	    (*(p_intn_func_handle[9]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(9);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq10(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[10] != NULL)
	    (*(p_intn_func_handle[10]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(10);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq11(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[11] != NULL)
	    (*(p_intn_func_handle[11]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(11);

	NESTED_IRQ_EXIT();
}



void __attribute__ ((interrupt)) entry_irq12(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[12] != NULL)
	    (*(p_intn_func_handle[12]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(12);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq13(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[13] != NULL)
	    (*(p_intn_func_handle[13]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(13);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq14(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[14] != NULL)
	    (*(p_intn_func_handle[14]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(14);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq15(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[15] != NULL)
	    (*(p_intn_func_handle[15]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(15);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq16(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[16] != NULL)
	    (*(p_intn_func_handle[16]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(16);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq17(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[17] != NULL)
	    (*(p_intn_func_handle[17]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(17);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq18(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[18] != NULL)
	    (*(p_intn_func_handle[18]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(18);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq19(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[19] != NULL)
	    (*(p_intn_func_handle[19]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(19);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq20(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[20] != NULL)
	    (*(p_intn_func_handle[20]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(20);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq21(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[21] != NULL)
	    (*(p_intn_func_handle[21]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(21);

	NESTED_IRQ_EXIT();
}




void __attribute__ ((interrupt)) entry_irq22(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[22] != NULL)
	    (*(p_intn_func_handle[22]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(22);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq23(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[23] != NULL)
	    (*(p_intn_func_handle[23]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(23);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq24(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[24] != NULL)
	    (*(p_intn_func_handle[24]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(24);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq25(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[25] != NULL)
	    (*(p_intn_func_handle[25]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(25);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq26(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[26] != NULL)
	    (*(p_intn_func_handle[26]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(26);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq27(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[27] != NULL)
	    (*(p_intn_func_handle[27]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(27);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq28(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[28] != NULL)
	    (*(p_intn_func_handle[28]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(28);

	NESTED_IRQ_EXIT();
}



void __attribute__ ((interrupt)) entry_irq29(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[29] != NULL)
	    (*(p_intn_func_handle[29]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(29);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq30(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[30] != NULL)
	    (*(p_intn_func_handle[30]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(30);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq31(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[31] != NULL)
	    (*(p_intn_func_handle[31]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(31);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq32(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[32] != NULL)
	    (*(p_intn_func_handle[32]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(32);

	NESTED_IRQ_EXIT();
}



void __attribute__ ((interrupt)) entry_irq33(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[33] != NULL)
	    (*(p_intn_func_handle[33]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(33);

	NESTED_IRQ_EXIT();
}



void __attribute__ ((interrupt)) entry_irq34(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[34] != NULL)
	    (*(p_intn_func_handle[34]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(34);

	NESTED_IRQ_EXIT();
}



void __attribute__ ((interrupt)) entry_irq35(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[35] != NULL)
	    (*(p_intn_func_handle[35]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(35);

	NESTED_IRQ_EXIT();
}



void __attribute__ ((interrupt)) entry_irq36(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[36] != NULL)
	    (*(p_intn_func_handle[36]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(36);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq37(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[37] != NULL)
	    (*(p_intn_func_handle[37]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(37);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq38(void)
{
	NESTED_IRQ_ENTER();
	
    if(p_intn_func_handle[38] != NULL)
	    (*(p_intn_func_handle[38]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(38);

	NESTED_IRQ_EXIT();
}
void __attribute__ ((interrupt)) entry_irq39(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[39] != NULL)
	    (*(p_intn_func_handle[39]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(39);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq40(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[40] != NULL)
	    (*(p_intn_func_handle[40]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(40);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq41(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[41] != NULL)
	    (*(p_intn_func_handle[41]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(41);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq42(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[42] != NULL)
	    (*(p_intn_func_handle[42]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(42);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq43(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[43] != NULL)
	    (*(p_intn_func_handle[43]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(43);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq44(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[44] != NULL)
	    (*(p_intn_func_handle[44]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(44);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq45(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[45] != NULL)
	    (*(p_intn_func_handle[45]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(45);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq46(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[46] != NULL)
	    (*(p_intn_func_handle[46]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(46);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq47(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[47] != NULL)
	    (*(p_intn_func_handle[47]))() ;
	NESTED_VPLIC_COMPLETE_INTERRUPT(47);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq48(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[48] != NULL)
	    (*(p_intn_func_handle[48]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(48);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq49(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[49] != NULL)
	    (*(p_intn_func_handle[49]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(49);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq50(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[50] != NULL)
	    (*(p_intn_func_handle[50]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(50);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq51(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[51] != NULL)
	    (*(p_intn_func_handle[51]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(51);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq52(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[52] != NULL)
	    (*(p_intn_func_handle[52]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(52);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq53(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[53] != NULL)
	    (*(p_intn_func_handle[53]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(53);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq54(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[54] != NULL)
	    (*(p_intn_func_handle[54]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(54);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq55(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[55] != NULL)
	    (*(p_intn_func_handle[55]))() ;
	NESTED_VPLIC_COMPLETE_INTERRUPT(55);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq56(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[56] != NULL)
	    (*(p_intn_func_handle[56]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(56);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq57(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[57] != NULL)
	    (*(p_intn_func_handle[57]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(57);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq58(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[58] != NULL)
	    (*(p_intn_func_handle[58]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(58);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq59(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[59] != NULL)
	    (*(p_intn_func_handle[59]))() ;
	NESTED_VPLIC_COMPLETE_INTERRUPT(59);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq60(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[60] != NULL)
	    (*(p_intn_func_handle[60]))() ;
	NESTED_VPLIC_COMPLETE_INTERRUPT(60);

	NESTED_IRQ_EXIT();
}


void __attribute__ ((interrupt)) entry_irq61(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[61] != NULL)
	    (*(p_intn_func_handle[61]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(61);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq62(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[62] != NULL)
	    (*(p_intn_func_handle[62]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(62);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) entry_irq63(void)
{
	NESTED_IRQ_ENTER();

    if(p_intn_func_handle[63] != NULL)
	    (*(p_intn_func_handle[63]))() ;

	NESTED_VPLIC_COMPLETE_INTERRUPT(63);

	NESTED_IRQ_EXIT();
}
#endif


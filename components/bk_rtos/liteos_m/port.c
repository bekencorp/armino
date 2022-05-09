/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved
*/

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the ST STR91x ARM9
 * port.
 *----------------------------------------------------------*/

/* Standard includes. */
#include <stdlib.h>
#include <assert.h>
#include "sys_rtos.h"
#include <common/bk_include.h>
#include "osk_revision.h"
#include "portmacro.h"

#if (NX_POWERSAVE)
#include "ps.h"
#endif //(NX_POWERSAVE)

/*-----------------------------------------------------------*/
uint32_t platform_is_irq_enable( void )
{
    uint32_t interrupt;

    __asm volatile(
		"MRS %0,CPSR\n"
		"AND %0,%0,#0xC0\n"
		:"=r" (interrupt)
		:
		:"memory"
	);

    return (!(interrupt & ARM968_IRQ_ENABLE));
}

/*-----------------------------------------------------------*/
uint32_t platform_is_fiq_enable( void )
{
    uint32_t interrupt;

    __asm volatile(
		"MRS %0,CPSR\n"
		"AND %0,%0,#0xC0\n"
		:"=r" (interrupt)
		:
		:"memory"
	);

    return (!(interrupt & ARM968_FIQ_ENABLE));
}

/*-----------------------------------------------------------*/
void printf_lr_register(void)
{
    uint32_t value;

    __asm volatile(
		"MOV %0,lr\n"
		:"=r" (value)
		:
		:"memory"
	);

	os_printf("lr:%x\r\n", value);
}

uint32_t platform_is_in_irq_context( void )
{
    uint32_t mode;

    __asm volatile(
		"MRS %0,CPSR\n"
		"AND %0,%0,#0x1f\n"
		:"=r" (mode)
		:
		:"memory"
	);

    return (ARM968_IRQ_MODE == mode);
}

/*-----------------------------------------------------------*/
uint32_t platform_is_in_fiq_context( void )
{
    uint32_t mode;

    __asm volatile(
		"MRS %0,CPSR\n"
		"AND %0,%0,#0x1f\n"
		:"=r" (mode)
		:
		:"memory"
	);

    return (ARM968_FIQ_MODE == mode);
}

/*-----------------------------------------------------------*/
uint32_t platform_entry_system_mode_zero_ptr(void)
{
	uint32_t system_md_value = 0xdf;
	FUNCPTR boot = 0;

    __asm volatile(
		"MSR CPSR,%0\n"
		:"=r" (system_md_value)
		:
		:"memory"
	);

	(*boot)();

	return 0;
}

uint32_t platform_spsr_content( void )
{
    uint32_t mode;

    __asm volatile(
		"MRS %0,SPSR\n"
		:"=r" (mode)
		:
		:"memory"
	);

    return mode;
}

/*-----------------------------------------------------------*/
uint32_t platform_sp_content( void )
{
    uint32_t val;

    __asm volatile(
		"mov %0,SP\n"
		:"=r" (val)
		:
		:"memory"
	);

    return val;
}

/*-----------------------------------------------------------*/
uint32_t platform_cpsr_content( void )
{
    uint32_t mode;

    __asm volatile(
		"MRS %0,CPSR\n"
		:"=r" (mode)
		:
		:"memory"
	);

    return mode;
}

uint32_t platform_interrupt_compare( void )
{
	uint32_t ret = 0;
	uint32_t status = platform_cpsr_content();
	uint32_t sta_irq = ((status & 0x80) >> 7);
	uint32_t sta_fiq = ((status & 0x40) >> 6);

	if(sta_fiq == sta_irq)
	{
		ret = 1;
	}

	return ret;
}

uint32_t platform_is_fiq_disable( void )
{
	uint32_t value = platform_cpsr_content();

	return (value & 0x40);
}

uint32_t platform_is_irq_disable( void )
{
	uint32_t value = platform_cpsr_content();

	return (value & 0x80);
}

uint32_t platform_is_in_interrupt_context( void )
{
    return ((platform_is_in_fiq_context())
                || (platform_is_in_irq_context()));
}

void platform_enable_intc_at_restore_context(void)
{
	if(!platform_is_in_interrupt_context())
		portENABLE_IRQ();

	if(!platform_is_in_fiq_context())
		portENABLE_FIQ();
}

void rtos_stack_overflow(char *taskname)
{
	os_printf("stack overflow: %s\r\n", taskname);
	while(1);
}

#if CONTROL_IRQ_WITH_NORMAL_FUNCTION
/*
 * Enable Interrupts
 */
void port_enable_interrupts_flag(int val)
{
	unsigned long cpsr_val;
	unsigned long mask;

	mask = val & ARM968_IF_MASK;
	cpsr_val = platform_cpsr_content();
	cpsr_val &= (~ARM968_IF_MASK);
	cpsr_val += mask;

	__asm volatile(
	"msr	cpsr_c, %0"
	::"r" (cpsr_val)
	:);
}

void portENABLE_IRQ(void)
{
	unsigned long temp;
	__asm volatile(
	"mrs	%0, cpsr		@ local_irq_enable\n"
       "bic	%0, %0, #0x80\n"
       "msr	cpsr_c, %0"
	: "=r" (temp)
	:
	: "memory");
}

void portENABLE_FIQ(void)
{
	unsigned long temp;
	__asm volatile(
	"mrs	%0, cpsr		@ local_irq_enable\n"
       "bic	%0, %0, #0x40\n"
       "msr	cpsr_c, %0"
	: "=r" (temp)
	:
	: "memory");
}

/*
 * Disable Interrupts
 */
int portDISABLE_FIQ(void)
{
	unsigned long temp;
	unsigned long mask;

	__asm volatile(
	"mrs	%1, cpsr		@ local_irq_disable\n"
	"orr	%0, %1, #0x40\n"
	"msr	cpsr_c, %0"
	: "=r" (temp),"=r" (mask)
	:
	: "memory");

	return (!!(mask & 0x40));
}

int port_disable_interrupts_flag(void)
{
	unsigned long temp;
	unsigned long mask;

	__asm volatile(
	"mrs	%1, cpsr		@ local_irq_disable\n"
	"orr	%0, %1, #0xC0\n"
	"msr	cpsr_c, %0"
	: "=r" (temp),"=r" (mask)
	:
	: "memory");

	return (mask & 0xC0);
}

int portDISABLE_IRQ(void)
{
	unsigned long temp;
	unsigned long mask;

	__asm volatile(
	"mrs	%1, cpsr		@ local_irq_disable\n"
	"orr	%0, %1, #0x80\n"
	"msr	cpsr_c, %0"
	: "=r" (temp),"=r" (mask)
	:
	: "memory");

	return (!!(mask & 0x80));
}
#endif

//eof


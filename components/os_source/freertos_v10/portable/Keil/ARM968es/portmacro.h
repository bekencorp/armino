/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/
#ifndef PORTMACRO_H
#define PORTMACRO_H

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <os/os.h>

/**
 * for compiling beken378 library with thumb instruction set
 * bellow four code blocks can't be implemented with macro or inline function
 * portENABLE_IRQ/portENABLE_FIQ/portDISABLE_FIQ/portDISABLE_IRQ
 * so declare these lables as extended function and implement them in port.c
 *
 * this feature is controled with CONTROL_IRQ_WITH_NORMAL_FUNCTION,
 * and it is disabled as default
 *
 * steps to compile thumb instruction set library
 * a. replace -marm with -mthumb
 * b. enable macro CONTROL_IRQ_WITH_NORMAL_FUNCTION
 */
#define CONTROL_IRQ_WITH_NORMAL_FUNCTION    1

/* Type definitions. */
#define portCHAR			char
#define portFLOAT			float
#define portDOUBLE			double
#define portLONG			long
#define portSHORT			short
#define portSTACK_TYPE		uint32_t
#define portBASE_TYPE		long

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;
typedef uint32_t TickType_t;
#define portMAX_DELAY ( TickType_t )        0xffffffffUL

/* Constants required to setup the initial stack. */
#define portINITIAL_SPSR 			    ( ( StackType_t ) 0x1f ) /* System mode, ARM mode, interrupts enabled. */
#define portINSTRUCTION_SIZE	        ( ( StackType_t ) 4 )

#define ARM968_SYS_MODE 	 	        0x1f
#define ARM968_MODE_MASK 	 	        0x1f
#define ARM968_IF_MASK       	        0xC0
#define ARM968_IRQ_ENABLE    	        0x80
#define ARM968_IF_MASK       	        0xC0
#define ARM968_FIQ_ENABLE    	        0x40
#define ARM968_IRQ_MODE      	        0x12
#define ARM968_FIQ_MODE     	        0x11

/* Constants required to handle critical sections. */
#define portNO_CRITICAL_NESTING 		((uint32_t ) 0 )

/*-----------------------------------------------------------*/
enum arm_mode {
	ARM_MODE_USR = 16,
	ARM_MODE_FIQ = 17,
	ARM_MODE_IRQ = 18,
	ARM_MODE_SVC = 19,
	ARM_MODE_MON = 22,
	ARM_MODE_ABT = 23,
	ARM_MODE_HYP = 26,
	ARM_MODE_UND = 27,
	ARM_MODE_1176_MON = 28,
	ARM_MODE_SYS = 31,

	ARM_MODE_THREAD = 0,
	ARM_MODE_USER_THREAD = 1,
	ARM_MODE_HANDLER = 2,

	ARMV8_64_EL0T = 0x0,
	ARMV8_64_EL1T = 0x4,
	ARMV8_64_EL1H = 0x5,
	ARMV8_64_EL2T = 0x8,
	ARMV8_64_EL2H = 0x9,
	ARMV8_64_EL3T = 0xC,
	ARMV8_64_EL3H = 0xD,

	ARM_MODE_ANY = -1
};

/* Hardware specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT			8
#define portYIELD()					__asm ( "SWI 0" )
#define portNOP()                   __asm ( "NOP" )

/*-----------------------------------------------------------*/
/* Critical section handling. */
void vPortEnterCritical( void );
void vPortExitCritical( void );

#if 1	/* yaoguang */
#define portENTER_CRITICAL() vPortEnterCritical()
#define portEXIT_CRITICAL() vPortExitCritical()
#else
#define portENTER_CRITICAL()        do{     \
                                                GLOBAL_INT_DECLARATION();\
                                                GLOBAL_INT_DISABLE(); 
#define portEXIT_CRITICAL()                 \
                                                GLOBAL_INT_RESTORE();\
                                      }while(0)
#endif

__inline static unsigned int port_interrupt_mask_from_isr(void)
{
    unsigned int val;

    val = *((volatile unsigned int *)((0x00802000 + 17 * 4)));
    *((volatile unsigned int *)((0x00802000 + 17 * 4))) = 0x0;
    return val;
}

#define portSET_INTERRUPT_MASK_FROM_ISR() port_interrupt_mask_from_isr();
#define portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedStatusValue )   (*((volatile unsigned int *)((0x00802000 + 17 * 4))) = (uxSavedStatusValue))    


/*
 * Enable Interrupts
 */	
#ifdef CONTROL_IRQ_WITH_NORMAL_FUNCTION
extern void portENABLE_IRQ(void);
extern void portENABLE_FIQ(void);
extern int portDISABLE_FIQ(void);
extern int portDISABLE_IRQ(void);

extern int port_disable_interrupts_flag(void);
extern void port_enable_interrupts_flag(int val);
#else
#define portENABLE_IRQ()					\
	({							            \
		unsigned long temp;				    \
		__asm volatile(					    \
		"mrs	%0, cpsr		@ local_irq_enable\n"	\
	       "bic	%0, %0, #0x80\n"					    \
	       "msr	cpsr_c, %0"					            \
		: "=r" (temp)						            \
		:							                    \
		: "memory");						            \
	})
	
#define port_enable_interrupts_flag(val)					\
	({							              \
		unsigned long temp;				\
		unsigned long mask;				\
		mask = val & 0xC0;				\
		__asm volatile(					\
		"mrs	%0, cpsr		@ local_irq_enable\n"	\
	       "bic	%0, %0, %1\n"				        	\
	       "msr	cpsr_c, %0"					            \
		: "=r" (temp)						            \
		: "r" (mask)				                    \
		: );						            \
	})
	
#define portENABLE_FIQ()					\
	({							              \
		unsigned long temp;				\
		__asm volatile(					\
		"mrs	%0, cpsr		@ local_irq_enable\n"	\
	       "bic	%0, %0, #0x40\n"					\
	       "msr	cpsr_c, %0"					       \
		: "=r" (temp)						       \
		:							              \
		: "memory");						       \
	})
	
static inline  int portDISABLE_FIQ(void)
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

static inline  int port_disable_interrupts_flag(void)
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

static inline  int portDISABLE_IRQ(void)
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

extern uint32_t platform_is_in_interrupt_context( void );
extern uint32_t platform_is_in_fiq_context( void );
extern uint32_t platform_is_in_irq_context( void );
extern uint32_t platform_interrupt_compare( void );

#define portENABLE_INTERRUPTS()			do{		\
												if(platform_is_in_fiq_context())\
													break;\
												if(platform_is_in_irq_context()) {\
													portENABLE_FIQ();\
													break;\
												}\
												portENABLE_IRQ();\
												portENABLE_FIQ();\
										    }while(0)
            
										    
/*
 * Disable Interrupts
 */
#define portDISABLE_INTERRUPTS()		do{		\
												if(platform_is_in_fiq_context())\
													break;\
												if(platform_is_in_irq_context()) {\
													portDISABLE_FIQ();\
													break;\
												}\
												portDISABLE_FIQ();\
												portDISABLE_IRQ();\
										    }while(0)

/*-----------------------------------------------------------*/
/* Task utilities. */
#define portEND_SWITCHING_ISR( xSwitchRequired ) 	\
{													\
extern void vTaskSwitchContext( void ); 			\
													\
	if( xSwitchRequired )							\
	{												\
		vTaskSwitchContext();						\
	}												\
}

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void * pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void * pvParameters )

uint32_t platform_is_irq_disable( void );

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */
// eof

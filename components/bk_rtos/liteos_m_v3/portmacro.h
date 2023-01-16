
#ifndef __PORTMACRO_H__
#define __PORTMACRO_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <os/os.h>
#include "riscv_hal.h"

extern unsigned int arch_int_disable(void);
extern void arch_int_restore(uint32_t val);

__inline static int port_disable_interrupts_flag(void)
{
	return arch_int_disable();
}

__inline static void port_enable_interrupts_flag(int val)
{
	arch_int_restore(val);
}

#define portDISABLE_INTERRUPTS()	HAL_INT_DISABLE()
#define portENABLE_INTERRUPTS()		HAL_INT_ENABLE()


extern uint32_t platform_is_in_interrupt_context( void );


#ifdef __cplusplus
}
#endif

#endif /* _PORTMACRO_H_ */

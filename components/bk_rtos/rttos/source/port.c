#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

extern uint32_t platform_is_in_irq_context(void);
extern uint32_t platform_is_in_fiq_context(void);

uint32_t platform_is_in_interrupt_context(void)
{
	return ((platform_is_in_fiq_context())
			|| (platform_is_in_irq_context()));
}
// eof


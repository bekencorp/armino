#include <stddef.h>
#include "mpversion.h"
#include "py/obj.h"
#include "extmod/moduplatform.h"
#include "shared/runtime/interrupt_char.h"
#include "common/bk_err.h"

int mp_hal_init(void);
int mp_hal_exit(void);
int mp_hal_uart_hook(int hook);

#define mp_hal_quiet_timing_enter() MICROPY_BEGIN_ATOMIC_SECTION()
#define mp_hal_quiet_timing_exit(irq_state) MICROPY_END_ATOMIC_SECTION(irq_state)
#define mp_hal_delay_us_fast(us) mp_hal_delay_us(us)
#define mp_hal_pin_obj_t uint32_t
#define mp_hal_pin_name(p) (p)
#define MP_HAL_PIN_FMT "%u"
mp_hal_pin_obj_t mp_hal_get_pin_obj(mp_obj_t pin);
int mp_hal_pin_read(mp_hal_pin_obj_t pin);
void mp_hal_pin_write(mp_hal_pin_obj_t pin, int value);
void mp_hal_pin_input(mp_hal_pin_obj_t pin);
void mp_hal_pin_output(mp_hal_pin_obj_t pin);
void mp_hal_pin_open_drain(mp_hal_pin_obj_t pin);
void mp_hal_pin_od_high(mp_hal_pin_obj_t pin);
void mp_hal_pin_od_low(mp_hal_pin_obj_t pin);

void mp_hal_exceptions(bk_err_t e);

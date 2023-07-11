#include "py/mphal.h"
#include "py/mperrno.h"
#include "py/lexer.h"
#include "py/builtin.h"
#include "py/runtime.h"
#include "modmachine.h"
#include "machine_pin.h"

#if !MICROPY_VFS
mp_import_stat_t mp_import_stat(const char *path) {
    (void)path;
    return MP_IMPORT_STAT_NO_EXIST;
}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

#endif

mp_hal_pin_obj_t mp_hal_get_pin_obj(mp_obj_t pin) {
    if (mp_obj_get_type(pin) != &machine_pin_type) {
        mp_raise_ValueError(MP_ERROR_TEXT("expecting a pin"));
    }
    machine_pin_obj_t *self = pin;
    return self->id;
}

int mp_hal_pin_read(mp_hal_pin_obj_t pin) {
	return bk_gpio_get_input((gpio_id_t)pin);
}

void mp_hal_pin_write(mp_hal_pin_obj_t pin, int value) {
	if (value) {
		bk_gpio_set_output_high((gpio_id_t)pin);
	} else {
		bk_gpio_set_output_low((gpio_id_t)pin);
	}
}

void mp_hal_pin_input(mp_hal_pin_obj_t pin) {
	bk_gpio_enable_input((gpio_id_t)pin);
	bk_gpio_disable_output((gpio_id_t)pin);
}

void mp_hal_pin_output(mp_hal_pin_obj_t pin) {
	bk_gpio_disable_input((gpio_id_t)pin);
	bk_gpio_enable_output((gpio_id_t)pin);
}

void mp_hal_pin_open_drain(mp_hal_pin_obj_t pin) {
	gpio_config_t config;

	config.io_mode = GPIO_OUTPUT_ENABLE;
	config.pull_mode = GPIO_PULL_UP_EN;
	config.func_mode = GPIO_SECOND_FUNC_DISABLE;

	bk_gpio_set_config((gpio_id_t)pin, &config);
	bk_gpio_enable_input((gpio_id_t)pin);
	bk_gpio_enable_output((gpio_id_t)pin);
}

void mp_hal_pin_od_high(mp_hal_pin_obj_t pin) {
	bk_gpio_set_output_high((gpio_id_t)pin);
}

void mp_hal_pin_od_low(mp_hal_pin_obj_t pin) {
	bk_gpio_set_output_low((gpio_id_t)pin);
}

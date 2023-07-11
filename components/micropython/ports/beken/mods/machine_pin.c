/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * Development of the code in this file was sponsored by Microbric Pty Ltd
 *
 * The MIT License (MIT)
 *
 * Copyright 2020-2023 Beken
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/mphal.h"
#include "mphalport.h"

#if MICROPY_PY_MACHINE
#include "modmachine.h"
#include "extmod/virtpin.h"
#include "machine_rtc.h"
#include "machine_pin.h"

typedef struct _machine_pin_irq_obj_t {
    mp_obj_base_t base;
    gpio_id_t id;
} machine_pin_irq_obj_t;

// forward declaration
STATIC machine_pin_obj_t machine_pin_obj[GPIO_NUM];
STATIC const machine_pin_irq_obj_t machine_pin_irq_object[GPIO_NUM];

void machine_pins_init(void) {
    static bool did_install = false;
    if (!did_install) {
        //gpio_install_isr_service(0);
        did_install = true;
    }
}

void machine_pins_deinit(void) {
    for (int i = 0; i < MP_ARRAY_SIZE(machine_pin_obj); ++i) {
        //gpio_isr_handler_remove(machine_pin_obj[i].id);
    }
}

STATIC void machine_pin_isr_handler(void *arg) {
}

gpio_id_t machine_pin_get_id(mp_obj_t pin_in) {
    if (mp_obj_get_type(pin_in) != &machine_pin_type) {
        mp_raise_ValueError(MP_ERROR_TEXT("expecting a pin"));
    }
    machine_pin_obj_t *self = pin_in;
    return self->id;
}

STATIC void machine_pin_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    machine_pin_obj_t *self = self_in;
    mp_printf(print, "Pin(%u)", self->id);
}

// pin.init(io_mode=None, pull_mode=0, func_mode=0)
STATIC mp_obj_t machine_pin_obj_init_helper(machine_pin_obj_t *self, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_io_mode, ARG_pull_mode, ARG_func_mode };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_io_mode, MP_ARG_INT, {.u_int = GPIO_IO_DISABLE}},
        { MP_QSTR_pull_mode, MP_ARG_INT, {.u_int = GPIO_PULL_DISABLE}},
        { MP_QSTR_func_mode, MP_ARG_INT, {.u_int = GPIO_SECOND_FUNC_DISABLE}},
    };

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    self->config.io_mode = (gpio_io_mode_t)args[ARG_io_mode].u_int;
    self->config.pull_mode = (gpio_pull_mode_t)args[ARG_pull_mode].u_int;
    self->config.func_mode = (gpio_func_mode_t)args[ARG_func_mode].u_int;

    bk_err_t ret = bk_gpio_set_config(self->id, &self->config);
	if (ret != BK_OK) {
        mp_hal_exceptions(ret);
	}

    return mp_const_none;
}

// constructor(id, ...)
mp_obj_t mp_pin_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);

    // get the wanted pin object
    int wanted_pin = mp_obj_get_int(args[0]);
    machine_pin_obj_t *self = NULL;
    if (0 <= wanted_pin && wanted_pin < MP_ARRAY_SIZE(machine_pin_obj)) {
        self = (machine_pin_obj_t *)&machine_pin_obj[wanted_pin];
    }
    if (self == NULL) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid pin"));
    }

    self->base.type = &machine_pin_type;
    self->id = (gpio_id_t)wanted_pin;

    // pin mode given, so configure this GPIO
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, args + n_args);
    machine_pin_obj_init_helper(self, n_args - 1, args + 1, &kw_args);

    return MP_OBJ_FROM_PTR(self);
}

// fast method for getting/setting pin value
STATIC mp_obj_t machine_pin_call(mp_obj_t self_in, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 0, 1, false);
    machine_pin_obj_t *self = self_in;
    if (n_args == 0) {
        // get pin, ensure is input mode
#if 0
        if (self->config.io_mode != GPIO_INPUT_ENABLE) {
            bk_gpio_disable_output(self->id);
            bk_gpio_enable_input(self->id);
            self->config.io_mode = GPIO_INPUT_ENABLE;
        }
#endif
        return MP_OBJ_NEW_SMALL_INT(bk_gpio_get_input(self->id));
    } else{
        // set pin, ensure is output mode
#if 0
        if (self->config.io_mode != GPIO_OUTPUT_ENABLE) {
            bk_gpio_disable_input(self->id);
            bk_gpio_enable_output(self->id);
            self->config.io_mode = GPIO_OUTPUT_ENABLE;
        }
#endif
        if (mp_obj_is_true(args[0])) {
            bk_gpio_set_output_high(self->id);
        } else {
            bk_gpio_set_output_low(self->id);
        }
        return mp_const_none;
        }
}

// pin.init(mode, pull)
STATIC mp_obj_t machine_pin_obj_init(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    return machine_pin_obj_init_helper(args[0], n_args - 1, args + 1, kw_args);
}
MP_DEFINE_CONST_FUN_OBJ_KW(machine_pin_init_obj, 1, machine_pin_obj_init);

// pin.value([value])
STATIC mp_obj_t machine_pin_value(size_t n_args, const mp_obj_t *args) {
    return machine_pin_call(args[0], n_args - 1, 0, args + 1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_pin_value_obj, 1, 2, machine_pin_value);

// pin.off()
STATIC mp_obj_t machine_pin_off(mp_obj_t self_in) {
    machine_pin_obj_t *self = MP_OBJ_TO_PTR(self_in);
    // set pin, ensure is output mode
#if 0
    if (self->config.io_mode != GPIO_OUTPUT_ENABLE) {
        bk_gpio_disable_input(self->id);
        bk_gpio_enable_output(self->id);
        self->config.io_mode = GPIO_OUTPUT_ENABLE;
    }
#endif
    bk_gpio_set_output_low(self->id);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_pin_off_obj, machine_pin_off);

// pin.on()
STATIC mp_obj_t machine_pin_on(mp_obj_t self_in) {
    machine_pin_obj_t *self = MP_OBJ_TO_PTR(self_in);
    // set pin, ensure is output mode
#if 0
    if (self->config.io_mode != GPIO_OUTPUT_ENABLE) {
        bk_gpio_disable_input(self->id);
        bk_gpio_enable_output(self->id);
        self->config.io_mode = GPIO_OUTPUT_ENABLE;
    }
#endif
    bk_gpio_set_output_high(self->id);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_pin_on_obj, machine_pin_on);

// pin.irq(handler=None, trigger=IRQ_FALLING|IRQ_RISING)
STATIC mp_obj_t machine_pin_irq(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_handler, ARG_trigger, ARG_wake };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_handler, MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_trigger, MP_ARG_INT, {.u_int = GPIO_INT_TYPE_LOW_LEVEL} },
        { MP_QSTR_wake, MP_ARG_OBJ, {.u_obj = mp_const_none} },
    };
    machine_pin_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    if (n_args > 1 || kw_args->used != 0) {
        // configure irq
        //mp_obj_t handler = args[ARG_handler].u_obj;
        //uint32_t trigger = args[ARG_trigger].u_int;
        //mp_obj_t wake_obj = args[ARG_wake].u_obj;

    }

    // return the irq object
    return MP_OBJ_FROM_PTR(&machine_pin_irq_object[self->id]);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_pin_irq_obj, 1, machine_pin_irq);

STATIC const mp_rom_map_elem_t machine_pin_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&machine_pin_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_value), MP_ROM_PTR(&machine_pin_value_obj) },
    { MP_ROM_QSTR(MP_QSTR_off), MP_ROM_PTR(&machine_pin_off_obj) },
    { MP_ROM_QSTR(MP_QSTR_on), MP_ROM_PTR(&machine_pin_on_obj) },
    { MP_ROM_QSTR(MP_QSTR_irq), MP_ROM_PTR(&machine_pin_irq_obj) },

    // class constants
    { MP_ROM_QSTR(MP_QSTR_IN), MP_ROM_INT(GPIO_INPUT_ENABLE) },
    { MP_ROM_QSTR(MP_QSTR_OUT), MP_ROM_INT(GPIO_OUTPUT_ENABLE) },
    { MP_ROM_QSTR(MP_QSTR_HIGH_IMPENDENCE), MP_ROM_INT(GPIO_IO_DISABLE) },
    { MP_ROM_QSTR(MP_QSTR_PULL_UP), MP_ROM_INT(GPIO_PULL_UP_EN) },
    { MP_ROM_QSTR(MP_QSTR_PULL_DOWN), MP_ROM_INT(GPIO_PULL_DOWN_EN) },
    { MP_ROM_QSTR(MP_QSTR_PULL_DISABLE), MP_ROM_INT(GPIO_PULL_DISABLE) },
    { MP_ROM_QSTR(MP_QSTR_IRQ_RISING), MP_ROM_INT(GPIO_INT_TYPE_RISING_EDGE) },
    { MP_ROM_QSTR(MP_QSTR_IRQ_FALLING), MP_ROM_INT(GPIO_INT_TYPE_FALLING_EDGE) },
    { MP_ROM_QSTR(MP_QSTR_WAKE_LOW), MP_ROM_INT(GPIO_INT_TYPE_LOW_LEVEL) },
    { MP_ROM_QSTR(MP_QSTR_WAKE_HIGH), MP_ROM_INT(GPIO_INT_TYPE_HIGH_LEVEL) },
    { MP_ROM_QSTR(MP_QSTR_DRIVE_0), MP_ROM_INT(GPIO_DRIVER_CAPACITY_0) },
    { MP_ROM_QSTR(MP_QSTR_DRIVE_1), MP_ROM_INT(GPIO_DRIVER_CAPACITY_1) },
    { MP_ROM_QSTR(MP_QSTR_DRIVE_2), MP_ROM_INT(GPIO_DRIVER_CAPACITY_2) },
    { MP_ROM_QSTR(MP_QSTR_DRIVE_3), MP_ROM_INT(GPIO_DRIVER_CAPACITY_3) },
};

STATIC mp_uint_t pin_ioctl(mp_obj_t self_in, mp_uint_t request, uintptr_t arg, int *errcode) {
    (void)errcode;
    machine_pin_obj_t *self = self_in;

    switch (request) {
        case MP_PIN_READ: {
            // get pin, ensure is input mode
#if 0
            if (self->config.io_mode != GPIO_INPUT_ENABLE) {
                bk_gpio_disable_output(self->id);
                bk_gpio_enable_input(self->id);
                self->config.io_mode = GPIO_INPUT_ENABLE;
            }
#endif
            return bk_gpio_get_input(self->id);
        }
        case MP_PIN_WRITE: {
            // set pin, ensure is output mode
#if 0
            if (self->config.io_mode != GPIO_OUTPUT_ENABLE) {
                bk_gpio_disable_input(self->id);
                bk_gpio_enable_output(self->id);
                self->config.io_mode = GPIO_OUTPUT_ENABLE;
            }
#endif
        	if (arg) {
            	bk_gpio_set_output_high(self->id);
            } else {
            	bk_gpio_set_output_low(self->id);
            }
            return 0;
        }
    }
    return -1;
}

STATIC MP_DEFINE_CONST_DICT(machine_pin_locals_dict, machine_pin_locals_dict_table);

STATIC const mp_pin_p_t pin_pin_p = {
    .ioctl = pin_ioctl,
};

MP_DEFINE_CONST_OBJ_TYPE(
    machine_pin_type,
    MP_QSTR_Pin,
    MP_TYPE_FLAG_NONE,
    make_new, mp_pin_make_new,
    print, machine_pin_print,
    call, machine_pin_call,
    protocol, &pin_pin_p,
    locals_dict, &machine_pin_locals_dict
    );

/******************************************************************************/
// Pin IRQ object

STATIC const mp_obj_type_t machine_pin_irq_type;

STATIC const machine_pin_irq_obj_t machine_pin_irq_object[GPIO_NUM] = {
    {{&machine_pin_irq_type}, GPIO_0},
    {{&machine_pin_irq_type}, GPIO_1},
    {{&machine_pin_irq_type}, GPIO_2},
    {{&machine_pin_irq_type}, GPIO_3},
    {{&machine_pin_irq_type}, GPIO_4},
    {{&machine_pin_irq_type}, GPIO_5},
    {{&machine_pin_irq_type}, GPIO_6},
    {{&machine_pin_irq_type}, GPIO_7},
    {{&machine_pin_irq_type}, GPIO_8},
    {{&machine_pin_irq_type}, GPIO_9},
    {{&machine_pin_irq_type}, GPIO_10},
    {{&machine_pin_irq_type}, GPIO_11},
    {{&machine_pin_irq_type}, GPIO_12},
    {{&machine_pin_irq_type}, GPIO_13},
    {{&machine_pin_irq_type}, GPIO_14},
    {{&machine_pin_irq_type}, GPIO_15},
    {{&machine_pin_irq_type}, GPIO_16},
    {{&machine_pin_irq_type}, GPIO_17},
    {{&machine_pin_irq_type}, GPIO_18},
    {{&machine_pin_irq_type}, GPIO_19},
    {{&machine_pin_irq_type}, GPIO_20},
    {{&machine_pin_irq_type}, GPIO_21},
    {{&machine_pin_irq_type}, GPIO_22},
    {{&machine_pin_irq_type}, GPIO_23},
    {{&machine_pin_irq_type}, GPIO_24},
    {{&machine_pin_irq_type}, GPIO_25},
    {{&machine_pin_irq_type}, GPIO_26},
    {{&machine_pin_irq_type}, GPIO_27},
    {{&machine_pin_irq_type}, GPIO_28},
    {{&machine_pin_irq_type}, GPIO_29},
    {{&machine_pin_irq_type}, GPIO_30},
    {{&machine_pin_irq_type}, GPIO_31},
    {{&machine_pin_irq_type}, GPIO_32},
    {{&machine_pin_irq_type}, GPIO_33},
    {{&machine_pin_irq_type}, GPIO_34},
    {{&machine_pin_irq_type}, GPIO_35},
    {{&machine_pin_irq_type}, GPIO_36},
    {{&machine_pin_irq_type}, GPIO_37},
    {{&machine_pin_irq_type}, GPIO_38},
    {{&machine_pin_irq_type}, GPIO_39},
    {{&machine_pin_irq_type}, GPIO_40},
    {{&machine_pin_irq_type}, GPIO_41},
    {{&machine_pin_irq_type}, GPIO_42},
    {{&machine_pin_irq_type}, GPIO_43},
    {{&machine_pin_irq_type}, GPIO_44},
    {{&machine_pin_irq_type}, GPIO_45},
    {{&machine_pin_irq_type}, GPIO_46},
    {{&machine_pin_irq_type}, GPIO_47},
};

STATIC mp_obj_t machine_pin_irq_call(mp_obj_t self_in, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    machine_pin_irq_obj_t *self = self_in;
    mp_arg_check_num(n_args, n_kw, 0, 0, false);
    machine_pin_isr_handler((void *)&machine_pin_obj[self->id]);
    return mp_const_none;
}

STATIC mp_obj_t machine_pin_irq_trigger(size_t n_args, const mp_obj_t *args) {
    machine_pin_irq_obj_t *self = args[0];
    uint32_t orig_trig = 0;//GPIO.pin[self->id].int_type;
    if (n_args == 2) {
        // set trigger
        bk_gpio_set_interrupt_type(self->id, mp_obj_get_int(args[1]));
    }
    // return original trigger value
    return MP_OBJ_NEW_SMALL_INT(orig_trig);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_pin_irq_trigger_obj, 1, 2, machine_pin_irq_trigger);

STATIC const mp_rom_map_elem_t machine_pin_irq_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_trigger), MP_ROM_PTR(&machine_pin_irq_trigger_obj) },
};
STATIC MP_DEFINE_CONST_DICT(machine_pin_irq_locals_dict, machine_pin_irq_locals_dict_table);

STATIC MP_DEFINE_CONST_OBJ_TYPE(
    machine_pin_irq_type,
    MP_QSTR_IRQ,
    MP_TYPE_FLAG_NONE,
    call, machine_pin_irq_call,
    locals_dict, &machine_pin_irq_locals_dict
    );

MP_REGISTER_ROOT_POINTER(mp_obj_t machine_pin_irq_handler[48]); // 48 is the biggest of GPIO_NUM's

#endif

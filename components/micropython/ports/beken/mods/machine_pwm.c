/*
 * This file is part of the Micro Python project, http://micropython.org/
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

#include <math.h>

#include "py/runtime.h"
#include "py/mphal.h"

#include "driver/pwm.h"

#define PWM_DBG(...)
// #define PWM_DBG(...) mp_printf(&mp_plat_print, __VA_ARGS__); mp_printf(&mp_plat_print, "\n");

// Total number of channels
#define PWM_CHANNEL_MAX (PWM_ID_MAX)

typedef struct _chan_t {
    // Which channel has which GPIO pin assigned?
    // (-1 if not assigned)
    pwm_id_t pin;
    // Which channel has which timer assigned?
    // (-1 if not assigned)
    int timer_idx;
} chan_t;

// List of PWM channels
//STATIC chan_t chans[PWM_CHANNEL_MAX];

// Params for PWM operation
// 5khz is default frequency
#define PWM_FREQ_MAX (26000000)

// Config of timer upon which we run all PWM'ed GPIO pins
//STATIC bool pwm_inited = false;

// MicroPython PWM object struct
typedef struct _machine_pwm_obj_t {
    mp_obj_base_t base;
    pwm_id_t id;
    pwm_init_config_t config;
    bool active;
} machine_pwm_obj_t;

/******************************************************************************/

#define SAME_FREQ_ONLY (true)
#define SAME_FREQ_OR_FREE (false)
#define ANY_MODE (-1)

/******************************************************************************/
// MicroPython bindings for PWM

STATIC void mp_machine_pwm_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    machine_pwm_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "PWM(Pin(%u)", self->id);
    mp_printf(print, ", freq=%u", self->config.period_cycle);
    mp_printf(print, ", duty=%d,%d,%d", self->config.duty_cycle, self->config.duty2_cycle, self->config.duty3_cycle);
    mp_printf(print, ")");
}

// This called from pwm.init() method
/**
 * Example4 - 3 duty with initial signal low and period > (D1 + D2 + D3)::
 *
 *     Clock cycle = 1/26M
 *     Duty_cycle = D1 = 1
 *     Duty2_cycle = D2 = 2
 *     Duty3_cycle = D3 = 3
 *     period_cycle = D1 + D2 + D3 + 1 = 7
 *
 *                       |<-C->|
 *                        __    __    __    __    __    __    __    __    __    __    __    __    __
 *     Clock Signal:   __|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|
 *                     __       ___________                   _____       ___________
 *     PWM Signal:       |_____|           |_________________|     |_____|           |______________
 *
 *     Duty1:           |<-D1->|<----D2--->|<------D3------->|
 *
 *     Period:          |<--------------- Period ----------------->|
 *
 * @attention 1. The period should great or equal to (D1 + D2 + D3)
 * @attention 2. If the duty Dx is 0, then Dx+1 to D3 should also be 0. If D1 is 0, it indicates the
 *               duty ratio is 0% and the signal is always low.
 * @attention 3. If D1 equals period, it indicates the duty ratio is 100% and the signal is always high.
 * @attention 4. The signal strengh between two successive duties is opposite, e.g. if signal of D1
 *               is high, then signal of D2 is low.
 * @attention 5. The initial signal is the signal strengh of D1 if D1 is not 0.
 * @attention 6. The duty/period configuration is very flexible, different configurations may have
 *               same result.
 */
STATIC void mp_machine_pwm_init_helper(machine_pwm_obj_t *self,
    size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_period_cycle, ARG_duty_cycle, ARG_duty2_cycle, ARG_duty3_cycle };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_period_cycle, MP_ARG_INT, {.u_int = 2} },
        { MP_QSTR_duty_cycle, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 1} },
        { MP_QSTR_duty2_cycle, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_duty3_cycle, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args,
        MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    if (self->id == -1) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("out of PWM channels:%d"), PWM_CHANNEL_MAX); // in all modes
    }

	self->config.period_cycle = (uint32_t)args[ARG_period_cycle].u_int;
	self->config.duty_cycle = (uint32_t)args[ARG_duty_cycle].u_int;
	self->config.duty2_cycle = (uint32_t)args[ARG_duty2_cycle].u_int;
	self->config.duty3_cycle = (uint32_t)args[ARG_duty3_cycle].u_int;
    if (self->config.period_cycle > PWM_FREQ_MAX) {
        mp_raise_ValueError(MP_ERROR_TEXT("freqency must be from 1Hz to 26MHz"));
    }

    if (self->config.period_cycle < self->config.duty_cycle + self->config.duty2_cycle + self->config.duty3_cycle) {
        mp_raise_ValueError(MP_ERROR_TEXT("'period' should >= 'duty' + 'duty2' + 'duty3'"));
    }

#if CONFIG_PWM
    mp_hal_exceptions(bk_pwm_init(self->id, &self->config));
    mp_hal_exceptions(bk_pwm_start(self->id));
#endif
}

// This called from PWM() constructor
STATIC mp_obj_t mp_machine_pwm_make_new(const mp_obj_type_t *type,
    size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 1, 2, true);
    pwm_id_t pwm_id = mp_obj_get_int(args[0]);

    // create PWM object from the given pin
    machine_pwm_obj_t *self = mp_obj_malloc(machine_pwm_obj_t, &machine_pwm_type);
    self->id = pwm_id;

    // start the PWM running for this channel
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, args + n_args);
    mp_machine_pwm_init_helper(self, n_args - 1, args + 1, &kw_args);

    return MP_OBJ_FROM_PTR(self);
}

// This called from pwm.deinit() method
STATIC void mp_machine_pwm_deinit(machine_pwm_obj_t *self) {
#if CONFIG_PWM
    bk_pwm_stop(self->id);
    bk_pwm_deinit(self->id);
#endif
}

// Set's and get's methods of PWM class

STATIC mp_obj_t mp_machine_pwm_freq_get(machine_pwm_obj_t *self) {
    return MP_OBJ_NEW_SMALL_INT(self->config.period_cycle);
}

STATIC void mp_machine_pwm_freq_set(machine_pwm_obj_t *self, mp_int_t freq) {
    if ((freq <= 0) || (freq > PWM_FREQ_MAX)) {
        mp_raise_ValueError(MP_ERROR_TEXT("freqency must be from 1Hz to 26MHz"));
    }

    self->config.period_cycle = freq;
#if CONFIG_PWM
    mp_hal_exceptions(bk_pwm_init(self->id, &self->config));
#endif
}

STATIC mp_obj_t mp_machine_pwm_duty_get(machine_pwm_obj_t *self) {
    return MP_OBJ_NEW_SMALL_INT(self->config.duty_cycle);
}

STATIC void mp_machine_pwm_duty_set(machine_pwm_obj_t *self, mp_int_t duty) {
    self->config.duty_cycle = duty;
#if CONFIG_PWM
    mp_hal_exceptions(bk_pwm_init(self->id, &self->config));
#endif
}

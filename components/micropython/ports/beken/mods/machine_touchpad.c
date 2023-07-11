/*
 * This file is part of the MicroPython project, http://micropython.org/
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

#include "py/runtime.h"
#include "py/mphal.h"
#include "modmachine.h"
#include <driver/touch.h>

#if MICROPY_PY_MACHINE_TOUCHPAD

typedef struct _mtp_obj_t {
    mp_obj_base_t base;
    touch_channel_t touchpad_id;
	touch_config_t touch_config;
} mtp_obj_t;

STATIC const mtp_obj_t touchpad_obj[] = {
    {{&machine_touchpad_type}, BK_TOUCH_1},
    {{&machine_touchpad_type}, BK_TOUCH_2},
    {{&machine_touchpad_type}, BK_TOUCH_3},
    {{&machine_touchpad_type}, BK_TOUCH_4},
    {{&machine_touchpad_type}, BK_TOUCH_5},
    {{&machine_touchpad_type}, BK_TOUCH_6},
    {{&machine_touchpad_type}, BK_TOUCH_7},
    {{&machine_touchpad_type}, BK_TOUCH_8},
    {{&machine_touchpad_type}, BK_TOUCH_9},
    {{&machine_touchpad_type}, BK_TOUCH_10},
    {{&machine_touchpad_type}, BK_TOUCH_11},
    {{&machine_touchpad_type}, BK_TOUCH_12},
    {{&machine_touchpad_type}, BK_TOUCH_13},
    {{&machine_touchpad_type}, BK_TOUCH_14},
};

STATIC mp_obj_t mtp_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 1, 1, true);
    touch_channel_t touchpad_id = machine_pin_get_id(args[0]);
    const mtp_obj_t *self = NULL;

    if (touchpad_id >= MP_ARRAY_SIZE(touchpad_obj)) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid pin for touchpad"));
    }
    self = &touchpad_obj[MP_ARRAY_SIZE(touchpad_obj)];

    static int initialized = 0;
    if (!initialized) {
		bk_touch_gpio_init(1 << touchpad_id);
		bk_touch_enable(1 << touchpad_id);
        initialized = 1;
    }
	self->touch_config.sensitivity_level = TOUCH_SENSITIVITY_LEVLE_3;
	self->touch_config.detect_threshold = TOUCH_DETECT_THRESHOLD_6;
	self->touch_config.detect_range = TOUCH_DETECT_RANGE_8PF;
	bk_touch_config(&self->touch_config);
    if (err == BK_OK) {
        return MP_OBJ_FROM_PTR(self);
    }
    mp_raise_ValueError(MP_ERROR_TEXT("Touch pad error"));
}

STATIC mp_obj_t mtp_config(mp_obj_t self_in, mp_obj_t value_in) {
    mtp_obj_t *self = self_in;
    #if CONFIG_IDF_TARGET_ESP32
    uint16_t value = mp_obj_get_int(value_in);
    bk_err_t err = touch_pad_config(self->touchpad_id, value);
    #elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
    bk_err_t err = touch_pad_config(self->touchpad_id);
    #endif
	bk_touch_enable(1 << touch_id);

	touch_config.sensitivity_level = g_gain_s;
	touch_config.detect_threshold = TOUCH_DETECT_THRESHOLD_6;
	touch_config.detect_range = TOUCH_DETECT_RANGE_8PF;
	bk_touch_config(&touch_config);
    if (err == BK_OK) {
        return mp_const_none;
    }
    mp_raise_ValueError(MP_ERROR_TEXT("Touch pad error"));
}
MP_DEFINE_CONST_FUN_OBJ_2(mtp_config_obj, mtp_config);

STATIC mp_obj_t mtp_read(mp_obj_t self_in) {
    uint32_t value;
	bk_touch_calibration_start();
	value = bk_touch_get_calib_value();
    return mp_obj_new_int_from_uint(value);
}
MP_DEFINE_CONST_FUN_OBJ_1(mtp_read_obj, mtp_read);

STATIC const mp_rom_map_elem_t mtp_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_config), MP_ROM_PTR(&mtp_config_obj) },
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&mtp_read_obj) },

    // Constants
    { MP_ROM_QSTR(MP_QSTR_DETECT_RANGE_8PF), MP_ROM_INT(TOUCH_DETECT_RANGE_8PF) },
    { MP_ROM_QSTR(MP_QSTR_DETECT_RANGE_12PF), MP_ROM_INT(TOUCH_DETECT_RANGE_12PF) },
    { MP_ROM_QSTR(MP_QSTR_DETECT_RANGE_19PF), MP_ROM_INT(TOUCH_DETECT_RANGE_19PF) },
    { MP_ROM_QSTR(MP_QSTR_DETECT_RANGE_27PF), MP_ROM_INT(TOUCH_DETECT_RANGE_27PF) },
};

STATIC MP_DEFINE_CONST_DICT(mtp_locals_dict, mtp_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    machine_touchpad_type,
    MP_QSTR_TouchPad,
    MP_TYPE_FLAG_NONE,
    make_new, mtp_make_new,
    locals_dict, &mtp_locals_dict
    );

#endif

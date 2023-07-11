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


#include <stdio.h>

#include "py/runtime.h"
#include "py/mphal.h"
#include "modmachine.h"

#if MICROPY_PY_MACHINE_DAC

#include "driver/gpio.h"
#include "driver/aud.h"

typedef struct _mdac_obj_t {
    mp_obj_base_t base;
    aud_dac_chl_enable_t dac_id;
} mdac_obj_t;

STATIC const mdac_obj_t mdac_obj[] = {
	{{&machine_dac_type}, AUD_DAC_CHL_L_ENABLE},
	{{&machine_dac_type}, AUD_DAC_CHL_R_ENABLE},
	{{&machine_dac_type}, AUD_DAC_CHL_LR_ENABLE},
};

STATIC mp_obj_t mdac_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw,
    const mp_obj_t *args) {

    mp_arg_check_num(n_args, n_kw, 1, 1, true);
    aud_dac_chl_enable_t dac_id = (aud_dac_chl_enable_t)mp_obj_get_int(args[0]);
    const mdac_obj_t *self = NULL;
    for (int i = 0; i < MP_ARRAY_SIZE(mdac_obj); i++) {
        if (dac_id == mdac_obj[i].dac_id) {
            self = &mdac_obj[i];
            break;
        }
    }
    if (!self) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid Pin for DAC"));
    }

    bk_err_t err = dac_output_enable(self->dac_id);
    if (err == BK_OK) {
        err = dac_output_voltage(self->dac_id, 0);
    }
    if (err == BK_OK) {
        return MP_OBJ_FROM_PTR(self);
    }
    mp_raise_ValueError(MP_ERROR_TEXT("parameter error"));
}

STATIC void mdac_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    mdac_obj_t *self = self_in;
    mp_printf(print, "DAC(%u)", self->dac_id);
}

STATIC mp_obj_t mdac_write(mp_obj_t self_in, mp_obj_t value_in) {
    mdac_obj_t *self = self_in;
    int value = mp_obj_get_int(value_in);
    if (value < 0 || value > 255) {
        mp_raise_ValueError(MP_ERROR_TEXT("value out of range"));
    }

    bk_err_t err = bk_aud_dac_write(value);
    if (err == BK_OK) {
        return mp_const_none;
    }
    mp_raise_ValueError(MP_ERROR_TEXT("parameter error"));
}
MP_DEFINE_CONST_FUN_OBJ_2(mdac_write_obj, mdac_write);

STATIC const mp_rom_map_elem_t mdac_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&mdac_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_DAC_CHL_L), MP_ROM_INT(AUD_DAC_CHL_L_ENABLE) },
    { MP_ROM_QSTR(MP_QSTR_DAC_CHL_R), MP_ROM_INT(AUD_DAC_CHL_R_ENABLE) },
    { MP_ROM_QSTR(MP_QSTR_DAC_CHL_LR), MP_ROM_INT(AUD_DAC_CHL_LR_ENABLE) },
};

STATIC MP_DEFINE_CONST_DICT(mdac_locals_dict, mdac_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    machine_dac_type,
    MP_QSTR_DAC,
    MP_TYPE_FLAG_NONE,
    make_new, mdac_make_new,
    print, mdac_print,
    locals_dict, &mdac_locals_dict
    );

#endif // MICROPY_PY_MACHINE_DAC

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
#include "machine_adc.h"

void sys_drv_en_tempdet(uint32_t value);

#define ADC_0 0

STATIC const madc_obj_t madc_obj[] = {
    {{&machine_adc_type}, ADC_0},
    {{&machine_adc_type}, ADC_1},
    {{&machine_adc_type}, ADC_2},
    {{&machine_adc_type}, ADC_3},
    {{&machine_adc_type}, ADC_4},
    {{&machine_adc_type}, ADC_5},
    {{&machine_adc_type}, ADC_6},
    {{&machine_adc_type}, ADC_7},
    {{&machine_adc_type}, ADC_8},
    {{&machine_adc_type}, ADC_9},
    {{&machine_adc_type}, ADC_10},
    {{&machine_adc_type}, ADC_11},
    {{&machine_adc_type}, ADC_12},
    {{&machine_adc_type}, ADC_13},
};

const madc_obj_t *madc_search_helper(adc_chan_t channel_id) {
    for (int i = 0; i < MP_ARRAY_SIZE(madc_obj); i++) {
        const madc_obj_t *adc = &madc_obj[i];
        if (channel_id == adc->channel_id) {
            return adc;
        }
    }
    return NULL;
}

STATIC void madc_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    const madc_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "ADC(%u)", self->channel_id);
}

STATIC mp_obj_t madc_make_new(const mp_obj_type_t *type, size_t n_pos_args, size_t n_kw_args, const mp_obj_t *args) {
    mp_arg_check_num(n_pos_args, n_kw_args, 1, MP_OBJ_FUN_ARGS_MAX, true);
    adc_chan_t channel_id = (adc_chan_t)mp_obj_get_int(args[0]);
    const madc_obj_t *self = madc_search_helper(channel_id);
    if (!self) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid adc channel"));
    }

    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t madc_init(size_t n_pos_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(madc_init_obj, 1, madc_init);

STATIC mp_obj_t madc_read(mp_obj_t self_in) {
    const madc_obj_t *self = MP_OBJ_TO_PTR(self_in);
    uint16_t value   = 0;
    adc_config_t config = {0};
	bk_err_t err;

    err = bk_adc_acquire();
    if (err != BK_OK) {
    	mp_hal_exceptions(err);
    }
    if (self->channel_id == 7) {
	    sys_drv_en_tempdet(1);
    }
    err = bk_adc_init(self->channel_id);
    if (err != BK_OK) {
    	mp_hal_exceptions(err);
    }

    config.chan = self->channel_id;
    config.adc_mode = ADC_CONTINUOUS_MODE;
    config.src_clk = 1;
    config.clk = 0x30e035;
    config.saturate_mode = 4;
    config.steady_ctrl= 7;
    config.adc_filter = 0;
    if(config.adc_mode == ADC_CONTINUOUS_MODE) {
        config.sample_rate = 0;
    }

    err = bk_adc_set_config(&config);
    if (err != BK_OK) {
    	mp_hal_exceptions(err);
    }
    err = bk_adc_enable_bypass_clalibration();
    if (err != BK_OK) {
    	mp_hal_exceptions(err);
    }
    err = bk_adc_start();
    if (err != BK_OK) {
    	mp_hal_exceptions(err);
    }
    err = bk_adc_read(&value, ADC_READ_SEMAPHORE_WAIT_TIME);
    if (err != BK_OK) {
    	mp_hal_exceptions(err);
    }

    if (self->channel_id == 7) {
	    sys_drv_en_tempdet(0);
    }
    bk_adc_stop();
    bk_adc_deinit(self->channel_id);
    bk_adc_release();

    return MP_OBJ_NEW_SMALL_INT(value);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(madc_read_obj, madc_read);

STATIC const mp_rom_map_elem_t madc_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&madc_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&madc_read_obj) },

    { MP_ROM_QSTR(MP_QSTR_ADC_SLEEP_MODE), MP_ROM_INT(ADC_SLEEP_MODE) },
    { MP_ROM_QSTR(MP_QSTR_ADC_SINGLE_STEP_MODE), MP_ROM_INT(ADC_SINGLE_STEP_MODE) },
    { MP_ROM_QSTR(MP_QSTR_ADC_SOFTWARE_CONTRL_MODE), MP_ROM_INT(ADC_SOFTWARE_CONTRL_MODE) },
    { MP_ROM_QSTR(MP_QSTR_ADC_CONTINUOUS_MODE), MP_ROM_INT(ADC_CONTINUOUS_MODE) },
};
STATIC MP_DEFINE_CONST_DICT(madc_locals_dict, madc_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    machine_adc_type,
    MP_QSTR_ADC,
    MP_TYPE_FLAG_NONE,
    make_new, madc_make_new,
    print, madc_print,
    locals_dict, &madc_locals_dict
    );

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
#include "py/mperrno.h"
#include "extmod/machine_i2c.h"
#include "modmachine.h"

#if MICROPY_PY_MACHINE_I2C
#include "driver/i2c.h"

#define I2C_WRITE_TIMEOUT_MS    (500)
#define I2C_READ_TIMEOUT_MS     (500)

typedef struct _machine_hw_i2c_obj_t {
    mp_obj_base_t base;
    i2c_id_t i2c_id;
    i2c_config_t config;
} machine_hw_i2c_obj_t;

STATIC machine_hw_i2c_obj_t machine_hw_i2c_obj[I2C_ID_MAX];

int machine_hw_i2c_transfer(mp_obj_base_t *self_in, uint16_t addr, size_t n, mp_machine_i2c_buf_t *bufs, unsigned int flags) {
    machine_hw_i2c_obj_t *self = MP_OBJ_TO_PTR(self_in);
    int data_len = 0;
    bk_err_t ret = BK_OK;

    #if MICROPY_PY_MACHINE_I2C_TRANSFER_WRITE1
    if (flags & MP_MACHINE_I2C_FLAG_WRITE1) {
        bk_i2c_master_write(self->i2c_id, addr, bufs->buf, bufs->len, I2C_WRITE_TIMEOUT_MS);
        data_len += bufs->len;
        --n;
        ++bufs;
    }
    #endif

    for (; n--; ++bufs) {
        if (flags & MP_MACHINE_I2C_FLAG_READ) {
            ret = bk_i2c_master_read(self->i2c_id, addr, bufs->buf, bufs->len, I2C_WRITE_TIMEOUT_MS);
        } else {
	        ret = bk_i2c_master_write(self->i2c_id, addr, bufs->buf, bufs->len, I2C_WRITE_TIMEOUT_MS);
        }
		if (ret != BK_OK) {
	        break;
		}
        data_len += bufs->len;
    }

    if (flags & MP_MACHINE_I2C_FLAG_STOP) {
    }

    return (ret == BK_OK) ? data_len : ret;
}

/******************************************************************************/
// MicroPython bindings for machine API

STATIC void machine_hw_i2c_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    machine_hw_i2c_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "I2C(%u, addr=%u, mode=%u, freq=%u)",
        self->i2c_id, self->config.slave_addr, self->config.addr_mode, self->config.baud_rate);
}

mp_obj_t machine_hw_i2c_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    #if MICROPY_PY_MACHINE_SOFTI2C
    MP_MACHINE_I2C_CHECK_FOR_LEGACY_SOFTI2C_CONSTRUCTION(n_args, n_kw, all_args);
    #endif

    // Parse args
    enum { ARG_id, ARG_addr_mode, ARG_freq, ARG_slaveaddr };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_id, MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_mode, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = I2C_MEM_ADDR_SIZE_8BIT} },
        { MP_QSTR_freq, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 400000} },
        { MP_QSTR_slaveaddr, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Get I2C bus
    if (!(I2C_ID_0 <= args[ARG_id].u_int && args[ARG_id].u_int < I2C_ID_MAX)) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("I2C(%d) doesn't exist"), args[ARG_id].u_int);
    }

    // Get static peripheral object
    machine_hw_i2c_obj_t *self = (machine_hw_i2c_obj_t *)&machine_hw_i2c_obj[args[ARG_id].u_int];

    self->base.type = &machine_i2c_type;
    self->i2c_id = args[ARG_id].u_int;
    self->config.baud_rate = args[ARG_freq].u_int;
    self->config.addr_mode = args[ARG_addr_mode].u_int;
    self->config.slave_addr = args[ARG_slaveaddr].u_int;

	bk_err_t ret = bk_i2c_init(self->i2c_id, &self->config);
	if (ret != BK_OK) {
        mp_hal_exceptions(ret);
	}

    return MP_OBJ_FROM_PTR(self);
}

STATIC const mp_machine_i2c_p_t machine_hw_i2c_p = {
    #if MICROPY_PY_MACHINE_I2C_TRANSFER_WRITE1
    .transfer_supports_write1 = true,
    #endif
    .transfer = machine_hw_i2c_transfer,
};

MP_DEFINE_CONST_OBJ_TYPE(
    machine_i2c_type,
    MP_QSTR_I2C,
    MP_TYPE_FLAG_NONE,
    make_new, machine_hw_i2c_make_new,
    print, machine_hw_i2c_print,
    protocol, &machine_hw_i2c_p,
    locals_dict, &mp_machine_i2c_locals_dict
    );
#endif

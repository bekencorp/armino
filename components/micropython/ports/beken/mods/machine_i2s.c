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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "py/obj.h"
#include "py/runtime.h"
#include "py/misc.h"
#include "py/stream.h"
#include "py/objstr.h"
#include "modmachine.h"
#include "mphalport.h"

#if MICROPY_PY_MACHINE_I2S

#include "driver/i2s.h"
#include "os/os.h"

// The I2S module has 3 modes of operation:
//
// Mode1:  Blocking
// - readinto() and write() methods block until the supplied buffer is filled (read) or emptied (write)
// - this is the default mode of operation
//
// Mode2:  Non-Blocking
// - readinto() and write() methods return immediately.
// - buffer filling and emptying happens asynchronously to the main MicroPython task
// - a callback function is called when the supplied buffer has been filled (read) or emptied (write)
// - non-blocking mode is enabled when a callback is set with the irq() method
// - a FreeRTOS task is created to implement the asynchronous background operations
// - a FreeRTOS queue is used to transfer the supplied buffer to the background task
//
// Mode3: Uasyncio
// - implements the stream protocol
// - uasyncio mode is enabled when the ioctl() function is called
// - the I2S event queue is used to detect that I2S samples can be read or written from/to DMA memory
//
// The samples contained in the app buffer supplied for the readinto() and write() methods have the following convention:
//   Mono:  little endian format
//   Stereo:  little endian format, left channel first
//
// I2S terms:
//   "frame":  consists of two audio samples (Left audio sample + Right audio sample)
//
// Misc:
// - for Mono configuration:
//   - readinto method: samples are gathered from the L channel only
//   - write method: every sample is output to both the L and R channels
// - for readinto method the I2S hardware is read using 8-byte frames
//   (this is standard for almost all I2S hardware, such as MEMS microphones)
// - all sample data transfers use DMA

#define I2S_TASK_PRIORITY        (5)
#define I2S_TASK_STACK_SIZE      (2048)

#define DMA_BUF_LEN_IN_I2S_FRAMES (256)

// The transform buffer is used with the readinto() method to bridge the opaque DMA memory on the ESP devices
// with the app buffer.  It facilitates audio sample transformations.  e.g.  32-bits samples to 16-bit samples.
// The size of 240 bytes is an engineering optimum that balances transfer performance with an acceptable use of heap space
#define SIZEOF_TRANSFORM_BUFFER_IN_BYTES (240)

#define NUM_I2S_USER_FORMATS (4)
#define I2S_RX_FRAME_SIZE_IN_BYTES (8)

typedef enum {
    BLOCKING,
    NON_BLOCKING,
    UASYNCIO
} io_mode_t;

typedef enum {
    I2S_TX_TRANSFER,
    I2S_RX_TRANSFER,
} direction_t;

typedef struct _non_blocking_descriptor_t {
    mp_buffer_info_t appbuf;
    mp_obj_t callback;
    direction_t direction;
} non_blocking_descriptor_t;

typedef struct _machine_i2s_obj_t {
    mp_obj_base_t base;
    i2s_gpio_group_id_t group_id;
    i2s_channel_id_t channel_id;
    i2s_config_t config;
    RingBufferContext *rx_rb;
    RingBufferContext *tx_rb;
    mp_obj_t callback_for_non_blocking;
    io_mode_t io_mode;
    beken_queue_t non_blocking_mode_queue;
    beken_thread_t non_blocking_mode_task;
} machine_i2s_obj_t;

STATIC machine_i2s_obj_t machine_i2s_obj[I2S_GPIO_GROUP_MAX];

STATIC mp_obj_t machine_i2s_deinit(mp_obj_t self_in);

static int ch1_tx_data_handle_cb(uint32_t size)
{
	return size;
}

static int ch1_rx_data_handle_cb(uint32_t size)
{
	return size;
}

void machine_i2s_init() {
    bk_i2s_driver_init();
}

//  The following function takes a sample buffer and swaps L/R channels
//
//  Background:  For 32-bit stereo, the ESP-IDF API has a L/R channel orientation that breaks
//               convention with other ESP32 channel formats
//
//   appbuf[] = [L_0-7, L_8-15, L_16-23, L_24-31, R_0-7, R_8-15, R_16-23, R_24-31] = [Left channel, Right channel]
//   dma[] =    [R_0-7, R_8-15, R_16-23, R_24-31, L_0-7, L_8-15, L_16-23, L_24-31] = [Right channel, Left channel]
//
//   where:
//     L_0-7 is the least significant byte of the 32 bit sample in the Left channel
//     L_24-31 is the most significant byte of the 32 bit sample in the Left channel
//
//  Example:
//
//   appbuf[] = [0x99, 0xBB, 0x11, 0x22, 0x44, 0x55, 0xAB, 0x77] = [Left channel, Right channel]
//   dma[] =    [0x44, 0x55, 0xAB, 0x77, 0x99, 0xBB, 0x11, 0x22] = [Right channel,  Left channel]
//   where:
//      LEFT Channel =  0x99, 0xBB, 0x11, 0x22
//      RIGHT Channel = 0x44, 0x55, 0xAB, 0x77
//
//    samples in appbuf are in little endian format:
//       0x77 is the most significant byte of the 32-bit sample
//       0x44 is the least significant byte of the 32-bit sample
STATIC void swap_32_bit_stereo_channels(mp_buffer_info_t *bufinfo) {
    int32_t swap_sample;
    int32_t *sample = bufinfo->buf;
    uint32_t num_samples = bufinfo->len / 4;
    for (uint32_t i = 0; i < num_samples; i += 2) {
        swap_sample = sample[i + 1];
        sample[i + 1] = sample[i];
        sample[i] = swap_sample;
    }
}

STATIC uint32_t fill_appbuf_from_dma(machine_i2s_obj_t *self, mp_buffer_info_t *appbuf) {

    // copy audio samples from DMA memory to the app buffer
    // audio samples are read from DMA memory in chunks
    // loop, reading and copying chunks until the app buffer is filled
    // For uasyncio mode, the loop will make an early exit if DMA memory becomes empty
    // Example:
    //   a MicroPython I2S object is configured for 16-bit mono (2 bytes per audio sample).
    //   For every frame coming from DMA (8 bytes), 2 bytes are "cherry picked" and
    //   copied to the supplied app buffer.
    //   Thus, for every 1 byte copied to the app buffer, 4 bytes are read from DMA memory.
    //   If a 8kB app buffer is supplied, 32kB of audio samples is read from DMA memory.

    size_t total_read = 0;
    size_t step_read = 0;
    mp_uint_t atomic_state;

    while (total_read < appbuf->len) {
		atomic_state = MICROPY_BEGIN_ATOMIC_SECTION();
		step_read = ring_buffer_read(self->rx_rb, appbuf->buf, appbuf->len);
		MICROPY_END_ATOMIC_SECTION(atomic_state);
		if (step_read <= 0) {
			break;
		}
		total_read += step_read;
    }

    return total_read;
}

STATIC size_t copy_appbuf_to_dma(machine_i2s_obj_t *self, mp_buffer_info_t *appbuf) {
    size_t total_written = 0;
    size_t step_written = 0;
    mp_uint_t atomic_state;

    while (total_written < appbuf->len) {
		atomic_state = MICROPY_BEGIN_ATOMIC_SECTION();
		step_written = ring_buffer_write(self->tx_rb, appbuf->buf, appbuf->len);
		MICROPY_END_ATOMIC_SECTION(atomic_state);
		if (step_written <= 0) {
			break;
		}
		total_written += step_written;
    }

    return total_written;
}

// FreeRTOS task used for non-blocking mode
STATIC void task_for_non_blocking_mode(void *self_in) {
    machine_i2s_obj_t *self = (machine_i2s_obj_t *)self_in;

    non_blocking_descriptor_t descriptor;

    for (;;) {
        if (rtos_pop_from_queue(self->non_blocking_mode_queue, &descriptor, BEKEN_WAIT_FOREVER)) {
            if (descriptor.direction == I2S_TX_TRANSFER) {
                copy_appbuf_to_dma(self, &descriptor.appbuf);
            } else { // RX
                fill_appbuf_from_dma(self, &descriptor.appbuf);
            }
            mp_sched_schedule(descriptor.callback, MP_OBJ_FROM_PTR(self));
        }
    }
}

STATIC void machine_i2s_init_helper(machine_i2s_obj_t *self, size_t n_pos_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    enum {
        ARG_role,
        ARG_work_mode,
        ARG_lrck_invert,
        ARG_sck_invert,
        ARG_lsb_first,
        ARG_sync_length,
        ARG_data_length,
        ARG_pcm_dlength,
        ARG_store_mode,
        ARG_rate,
        ARG_chl_num,
    };

    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_role,     MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = I2S_ROLE_MASTER} },
        { MP_QSTR_work_mode, MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = I2S_WORK_MODE_I2S} },
        { MP_QSTR_lrck_invert, MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = I2S_LRCK_INVERT_DISABLE} },
        { MP_QSTR_sck_invert, MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = I2S_SCK_INVERT_DISABLE} },
        { MP_QSTR_lsb_first, MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = I2S_LSB_FIRST_DISABLE} },
        { MP_QSTR_sync_length, MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = 0} },
        { MP_QSTR_data_length, MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = 16} },
        { MP_QSTR_pcm_dlength, MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = 0} },
        { MP_QSTR_store_mode, MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = I2S_LRCOM_STORE_LRLR} },
        { MP_QSTR_rate,     MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = I2S_SAMP_RATE_8000} },
        { MP_QSTR_chl_num,  MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = 1} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_pos_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    //
    // ---- Check validity of arguments ----
    //

    // is Mode valid?
    i2s_work_mode_t work_mode = args[ARG_work_mode].u_int;
    if ((work_mode < I2S_WORK_MODE_I2S) || (I2S_WORK_MODE_MAX <= work_mode)) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid mode"));
    }

    // is Rate valid?
    i2s_samp_rate_t samp_rate = args[ARG_rate].u_int;
    if ((samp_rate < I2S_SAMP_RATE_8000) || (I2S_SAMP_RATE_MAX <= samp_rate)) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid rate"));
    }

    self->config.role = args[ARG_role].u_int;
    self->config.work_mode = args[ARG_work_mode].u_int;
    self->config.lrck_invert = args[ARG_lrck_invert].u_int;
    self->config.sck_invert = args[ARG_sck_invert].u_int;
    self->config.lsb_first_en = args[ARG_lsb_first].u_int;
    self->config.sync_length = args[ARG_sync_length].u_int;
    self->config.data_length = args[ARG_data_length].u_int;
    self->config.pcm_dlength = args[ARG_pcm_dlength].u_int;
    self->config.store_mode = args[ARG_store_mode].u_int;
    self->config.samp_rate = args[ARG_rate].u_int;
    self->config.pcm_chl_num = args[ARG_chl_num].u_int;

    bk_err_t ret = bk_i2s_init(self->group_id, &self->config);
	if (ret != BK_OK) {
        mp_hal_exceptions(ret);
	}

	ret = bk_i2s_chl_init(self->channel_id, I2S_TXRX_TYPE_TX, 640, ch1_tx_data_handle_cb, &self->tx_rb);
	if (ret != BK_OK) {
        mp_hal_exceptions(ret);
	}

	ret = bk_i2s_chl_init(self->channel_id, I2S_TXRX_TYPE_RX, 640, ch1_rx_data_handle_cb, &self->rx_rb);
	if (ret != BK_OK) {
        mp_hal_exceptions(ret);
	}
}

STATIC void machine_i2s_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    machine_i2s_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "I2S(id=%u,mode=%u,rate=%d)",
        self->group_id,
        self->config.work_mode,
        self->config.samp_rate);
}

STATIC mp_obj_t machine_i2s_make_new(const mp_obj_type_t *type, size_t n_pos_args, size_t n_kw_args, const mp_obj_t *args) {
    mp_arg_check_num(n_pos_args, n_kw_args, 1, MP_OBJ_FUN_ARGS_MAX, true);

    i2s_gpio_group_id_t i2s_id = mp_obj_get_int(args[0]);
    if (i2s_id < 0 || i2s_id >= I2S_GPIO_GROUP_MAX) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid id"));
    }

    machine_i2s_obj_t *self = &machine_i2s_obj[i2s_id];
    self->base.type = &machine_i2s_type;
    self->group_id = i2s_id;

    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw_args, args + n_pos_args);
    machine_i2s_init_helper(self, n_pos_args - 1, args + 1, &kw_args);

    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t machine_i2s_obj_init(size_t n_pos_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    machine_i2s_obj_t *self = pos_args[0];
    machine_i2s_deinit(self);
    machine_i2s_init_helper(self, n_pos_args - 1, pos_args + 1, kw_args);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_i2s_init_obj, 1, machine_i2s_obj_init);

STATIC mp_obj_t machine_i2s_deinit(mp_obj_t self_in) {
    machine_i2s_obj_t *self = MP_OBJ_TO_PTR(self_in);
	bk_i2s_stop();
	bk_i2s_chl_deinit(self->channel_id, I2S_TXRX_TYPE_TX);
	bk_i2s_deinit();
	bk_i2s_driver_deinit();

    if (self->non_blocking_mode_task != NULL) {
        rtos_delete_thread(&self->non_blocking_mode_task);
        self->non_blocking_mode_task = NULL;
    }

    if (self->non_blocking_mode_queue != NULL) {
        rtos_deinit_queue(&self->non_blocking_mode_queue);
        self->non_blocking_mode_queue = NULL;
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_i2s_deinit_obj, machine_i2s_deinit);

STATIC mp_obj_t machine_i2s_irq(mp_obj_t self_in, mp_obj_t handler) {
    machine_i2s_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (handler != mp_const_none && !mp_obj_is_callable(handler)) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid callback"));
    }

    if (handler != mp_const_none) {
        self->io_mode = NON_BLOCKING;

        // create a queue linking the MicroPython task to a FreeRTOS task
        // that manages the non blocking mode of operation
        rtos_init_queue(&self->non_blocking_mode_queue, "i2s_nonblock_queue", sizeof(non_blocking_descriptor_t), 1);

        // non-blocking mode requires a background FreeRTOS task
        if (rtos_create_thread(&self->non_blocking_mode_task,
            I2S_TASK_PRIORITY,
            "i2s_non_blocking",
            task_for_non_blocking_mode,
            I2S_TASK_STACK_SIZE,
            self) != BK_OK) {

            mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("failed to create I2S task"));
        }
    } else {
        if (self->non_blocking_mode_task != NULL) {
            rtos_delete_thread(&self->non_blocking_mode_task);
            self->non_blocking_mode_task = NULL;
        }

        if (self->non_blocking_mode_queue != NULL) {
            rtos_deinit_queue(&self->non_blocking_mode_queue);
            self->non_blocking_mode_queue = NULL;
        }

        self->io_mode = BLOCKING;
    }

    self->callback_for_non_blocking = handler;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(machine_i2s_irq_obj, machine_i2s_irq);

// Shift() is typically used as a volume control.
// shift=1 increases volume by 6dB, shift=-1 decreases volume by 6dB
STATIC mp_obj_t machine_i2s_shift(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_buf, ARG_bits, ARG_shift};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_buf,    MP_ARG_REQUIRED | MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_bits,   MP_ARG_REQUIRED | MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_shift,  MP_ARG_REQUIRED | MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    };

    // parse args
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_buf].u_obj, &bufinfo, MP_BUFFER_RW);

    int16_t *buf_16 = bufinfo.buf;
    int32_t *buf_32 = bufinfo.buf;

    uint8_t bits = args[ARG_bits].u_int;
    int8_t shift = args[ARG_shift].u_int;

    uint32_t num_audio_samples;
    switch (bits) {
        case 16:
            num_audio_samples = bufinfo.len / 2;
            break;

        case 32:
            num_audio_samples = bufinfo.len / 4;
            break;

        default:
            mp_raise_ValueError(MP_ERROR_TEXT("invalid bits"));
            break;
    }

    for (uint32_t i = 0; i < num_audio_samples; i++) {
        switch (bits) {
            case 16:
                if (shift >= 0) {
                    buf_16[i] = buf_16[i] << shift;
                } else {
                    buf_16[i] = buf_16[i] >> abs(shift);
                }
                break;
            case 32:
                if (shift >= 0) {
                    buf_32[i] = buf_32[i] << shift;
                } else {
                    buf_32[i] = buf_32[i] >> abs(shift);
                }
                break;
        }
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_i2s_shift_fun_obj, 0, machine_i2s_shift);
STATIC MP_DEFINE_CONST_STATICMETHOD_OBJ(machine_i2s_shift_obj, MP_ROM_PTR(&machine_i2s_shift_fun_obj));

STATIC const mp_rom_map_elem_t machine_i2s_locals_dict_table[] = {
    // Methods
    { MP_ROM_QSTR(MP_QSTR_init),            MP_ROM_PTR(&machine_i2s_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_readinto),        MP_ROM_PTR(&mp_stream_readinto_obj) },
    { MP_ROM_QSTR(MP_QSTR_write),           MP_ROM_PTR(&mp_stream_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_deinit),          MP_ROM_PTR(&machine_i2s_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR_irq),             MP_ROM_PTR(&machine_i2s_irq_obj) },
    { MP_ROM_QSTR(MP_QSTR___del__),         MP_ROM_PTR(&machine_i2s_deinit_obj) },

    // Static method
    { MP_ROM_QSTR(MP_QSTR_shift),           MP_ROM_PTR(&machine_i2s_shift_obj) },

    // Constants
    { MP_ROM_QSTR(MP_QSTR_RX),              MP_ROM_INT(I2S_TXRX_TYPE_RX) },
    { MP_ROM_QSTR(MP_QSTR_TX),              MP_ROM_INT(I2S_TXRX_TYPE_TX) },
};
MP_DEFINE_CONST_DICT(machine_i2s_locals_dict, machine_i2s_locals_dict_table);

STATIC mp_uint_t machine_i2s_stream_read(mp_obj_t self_in, void *buf_in, mp_uint_t size, int *errcode) {
    machine_i2s_obj_t *self = MP_OBJ_TO_PTR(self_in);

    if (self->config.role != I2S_ROLE_MASTER) {
        *errcode = MP_EPERM;
        return MP_STREAM_ERROR;
    }

    if (size == 0) {
        return 0;
    }
    if (self->io_mode == NON_BLOCKING) {
        non_blocking_descriptor_t descriptor;
        descriptor.appbuf.buf = (void *)buf_in;
        descriptor.appbuf.len = size;
        descriptor.callback = self->callback_for_non_blocking;
        descriptor.direction = I2S_RX_TRANSFER;
        // send the descriptor to the task that handles non-blocking mode
        rtos_push_to_queue(self->non_blocking_mode_queue, &descriptor, 0);
        return size;
    } else { // blocking or uasyncio mode
        mp_buffer_info_t appbuf;
        appbuf.buf = (void *)buf_in;
        appbuf.len = size;
        uint32_t num_bytes_read = fill_appbuf_from_dma(self, &appbuf);
        return num_bytes_read;
    }
    return 0;
}

STATIC mp_uint_t machine_i2s_stream_write(mp_obj_t self_in, const void *buf_in, mp_uint_t size, int *errcode) {
    machine_i2s_obj_t *self = MP_OBJ_TO_PTR(self_in);

    if (self->config.role != I2S_ROLE_MASTER) {
        *errcode = MP_EPERM;
        return MP_STREAM_ERROR;
    }

    if (size == 0) {
        return 0;
    }

    if (self->io_mode == NON_BLOCKING) {
        non_blocking_descriptor_t descriptor;
        descriptor.appbuf.buf = (void *)buf_in;
        descriptor.appbuf.len = size;
        descriptor.callback = self->callback_for_non_blocking;
        descriptor.direction = I2S_TX_TRANSFER;
        // send the descriptor to the task that handles non-blocking mode
        rtos_push_to_queue(self->non_blocking_mode_queue, &descriptor, 0);
        return size;
    } else { // blocking or uasyncio mode
        mp_buffer_info_t appbuf;
        appbuf.buf = (void *)buf_in;
        appbuf.len = size;
        size_t num_bytes_written = copy_appbuf_to_dma(self, &appbuf);
        return num_bytes_written;
    }

    return 0;
}

STATIC mp_uint_t machine_i2s_ioctl(mp_obj_t self_in, mp_uint_t request, uintptr_t arg, int *errcode) {
    machine_i2s_obj_t *self = MP_OBJ_TO_PTR(self_in);
    self->io_mode = UASYNCIO; // a call to ioctl() is an indication that uasyncio is being used

    return 0;
}

STATIC const mp_stream_p_t i2s_stream_p = {
    .read = machine_i2s_stream_read,
    .write = machine_i2s_stream_write,
    .ioctl = machine_i2s_ioctl,
    .is_text = false,
};

MP_DEFINE_CONST_OBJ_TYPE(
    machine_i2s_type,
    MP_QSTR_I2S,
    MP_TYPE_FLAG_ITER_IS_STREAM,
    make_new, machine_i2s_make_new,
    print, machine_i2s_print,
    protocol, &i2s_stream_p,
    locals_dict, &machine_i2s_locals_dict
    );

#endif // MICROPY_PY_MACHINE_I2S

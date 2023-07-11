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

#include <stdint.h>
#include <stdio.h>

#include <modules/pm.h>
#include <modules/wifi.h>
#include <components/system.h>
#include <bk_manual_ps.h>

#include "py/obj.h"
#include "py/runtime.h"
#include "shared/runtime/pyexec.h"
#include "drivers/dht/dht.h"
#include "extmod/machine_bitstream.h"
#include "extmod/machine_mem.h"
#include "extmod/machine_signal.h"
#include "extmod/machine_pulse.h"
#include "extmod/machine_pwm.h"
#include "extmod/machine_i2c.h"
#include "extmod/machine_spi.h"
#include "modmachine.h"
#include "machine_rtc.h"

#if MICROPY_PY_MACHINE

typedef enum {
    MP_PWRON_RESET = 1,
    MP_HARD_RESET,
    MP_WDT_RESET,
    MP_DEEPSLEEP_RESET,
    MP_SOFT_RESET
} reset_reason_t;

STATIC bool is_soft_reset = 0;

STATIC mp_obj_t machine_freq(size_t n_args, const mp_obj_t *args) {
    if (n_args == 0) {
        // get
        return mp_obj_new_int(bk_pm_current_max_cpu_freq_get());
    } else {
        // set
        mp_int_t freq = mp_obj_get_int(args[0]) / 1000000;
        pm_cpu_freq_e cpu_freq;

        if (freq == 320) {
        	cpu_freq = PM_CPU_FRQ_320M;
        } else if (freq == 240) {
        	cpu_freq = PM_CPU_FRQ_240M;
        } else if (freq == 120) {
        	cpu_freq = PM_CPU_FRQ_120M;
        } else if (freq == 26) {
        	cpu_freq = PM_CPU_FRQ_26M;
        } else {
            mp_raise_ValueError(MP_ERROR_TEXT("frequency must be 26MHz, 120MHz, 240Mhz or 320MHz"));
        }
        bk_err_t ret = bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT, cpu_freq);
        if (ret != BK_OK) {
            mp_raise_ValueError(NULL);
        }
        return mp_const_none;
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_freq_obj, 0, 1, machine_freq);

STATIC mp_obj_t machine_lightsleep(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    bk_wifi_sta_pm_enable();
    return mp_const_none;
};
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_lightsleep_obj, 0, machine_lightsleep);

STATIC mp_obj_t machine_deepsleep(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum {
    	ARG_sleep_ms,
    	ARG_gpio_index_map,
    	ARG_gpio_edge_map,
    	ARG_gpio_last_index_map,
    	ARG_gpio_last_edge_map,
    	ARG_gpio_stay_lo_map,
    	ARG_gpio_stay_hi_map,
    };
    const mp_arg_t allowed_args[] = {
        { MP_QSTR_sleep_ms, MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_gpio_index_map, MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_gpio_edge_map, MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_gpio_last_index_map, MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_gpio_last_edge_map, MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_gpio_stay_lo_map, MP_ARG_INT, { .u_int = 0 } },
        { MP_QSTR_gpio_stay_hi_map, MP_ARG_INT, { .u_int = 0 } },
    };
	PS_DEEP_CTRL_PARAM deep_sleep_param;
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];

    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);



	deep_sleep_param.gpio_index_map = args[ARG_gpio_index_map].u_int;
	deep_sleep_param.gpio_edge_map = args[ARG_gpio_edge_map].u_int;
	deep_sleep_param.gpio_last_index_map = args[ARG_gpio_last_index_map].u_int;
	deep_sleep_param.gpio_last_edge_map = args[ARG_gpio_last_edge_map].u_int;
	deep_sleep_param.sleep_time = args[ARG_sleep_ms].u_int;
	deep_sleep_param.gpio_stay_lo_map = args[ARG_gpio_stay_lo_map].u_int;
	deep_sleep_param.gpio_stay_hi_map = args[ARG_gpio_stay_hi_map].u_int;
	deep_sleep_param.wake_up_way = PS_DEEP_WAKEUP_USB;
    if (deep_sleep_param.sleep_time != 0) {
		deep_sleep_param.wake_up_way |= PS_DEEP_WAKEUP_RTC;
    }
    if (deep_sleep_param.gpio_index_map != 0) {
		deep_sleep_param.wake_up_way |= PS_DEEP_WAKEUP_GPIO;
    }

	//bk_enter_deep_sleep_mode(&deep_sleep_param);

    return mp_const_none;
};
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_deepsleep_obj, 0,  machine_deepsleep);

STATIC mp_obj_t machine_reset_cause(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
	uint32_t reset_reason = bk_misc_get_reset_reason();
    return MP_OBJ_NEW_SMALL_INT(reset_reason);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_reset_cause_obj, 0,  machine_reset_cause);

void machine_init(void) {
    is_soft_reset = 0;
}

void machine_deinit(void) {
    // we are doing a soft-reset so change the reset_cause
    is_soft_reset = 1;
}

STATIC mp_obj_t machine_reset(void) {
    bk_reboot();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_reset_obj, machine_reset);

STATIC mp_obj_t machine_soft_reset(void) {
    pyexec_system_exit = PYEXEC_FORCED_EXIT;
    mp_raise_type(&mp_type_SystemExit);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_soft_reset_obj, machine_soft_reset);

STATIC mp_obj_t machine_unique_id(void) {
    uint8_t chipid[6];
    bk_wifi_sta_get_mac(chipid);
    return mp_obj_new_bytes(chipid, 6);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_unique_id_obj, machine_unique_id);

STATIC mp_obj_t machine_idle(void) {
    MP_THREAD_GIL_EXIT();
    __asm("SWI 0");
    MP_THREAD_GIL_ENTER();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(machine_idle_obj, machine_idle);

STATIC mp_obj_t machine_disable_irq(void) {
    uint32_t state = MICROPY_BEGIN_ATOMIC_SECTION();
    return mp_obj_new_int(state);
}
MP_DEFINE_CONST_FUN_OBJ_0(machine_disable_irq_obj, machine_disable_irq);

STATIC mp_obj_t machine_enable_irq(mp_obj_t state_in) {
    uint32_t state = mp_obj_get_int(state_in);
    MICROPY_END_ATOMIC_SECTION(state);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(machine_enable_irq_obj, machine_enable_irq);

STATIC const mp_rom_map_elem_t machine_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_umachine) },

    { MP_ROM_QSTR(MP_QSTR_mem8), MP_ROM_PTR(&machine_mem8_obj) },
    { MP_ROM_QSTR(MP_QSTR_mem16), MP_ROM_PTR(&machine_mem16_obj) },
    { MP_ROM_QSTR(MP_QSTR_mem32), MP_ROM_PTR(&machine_mem32_obj) },

    { MP_ROM_QSTR(MP_QSTR_freq), MP_ROM_PTR(&machine_freq_obj) },
    { MP_ROM_QSTR(MP_QSTR_reset), MP_ROM_PTR(&machine_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_soft_reset), MP_ROM_PTR(&machine_soft_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_unique_id), MP_ROM_PTR(&machine_unique_id_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep), MP_ROM_PTR(&machine_lightsleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_lightsleep), MP_ROM_PTR(&machine_lightsleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_deepsleep), MP_ROM_PTR(&machine_deepsleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_idle), MP_ROM_PTR(&machine_idle_obj) },

    { MP_ROM_QSTR(MP_QSTR_disable_irq), MP_ROM_PTR(&machine_disable_irq_obj) },
    { MP_ROM_QSTR(MP_QSTR_enable_irq), MP_ROM_PTR(&machine_enable_irq_obj) },

    #if MICROPY_PY_MACHINE_BITSTREAM
    { MP_ROM_QSTR(MP_QSTR_bitstream), MP_ROM_PTR(&machine_bitstream_obj) },
    #endif
    #if MICROPY_PY_MACHINE_PULSE
    { MP_ROM_QSTR(MP_QSTR_time_pulse_us), MP_ROM_PTR(&machine_time_pulse_us_obj) },
    #endif

    //{ MP_ROM_QSTR(MP_QSTR_dht_readinto), MP_ROM_PTR(&dht_readinto_obj) },

    #if MICROPY_PY_MACHINE_TIMER
    { MP_ROM_QSTR(MP_QSTR_Timer), MP_ROM_PTR(&machine_timer_type) },
    #endif
    { MP_ROM_QSTR(MP_QSTR_WDT), MP_ROM_PTR(&machine_wdt_type) },
    #if MICROPY_PY_MACHINE_SDCARD
    { MP_ROM_QSTR(MP_QSTR_SDCard), MP_ROM_PTR(&machine_sdcard_type) },
    #endif

    // wake abilities
    { MP_ROM_QSTR(MP_QSTR_SLEEP), MP_ROM_INT(MACHINE_WAKE_SLEEP) },
    { MP_ROM_QSTR(MP_QSTR_DEEPSLEEP), MP_ROM_INT(MACHINE_WAKE_DEEPSLEEP) },
    { MP_ROM_QSTR(MP_QSTR_Pin), MP_ROM_PTR(&machine_pin_type) },
    { MP_ROM_QSTR(MP_QSTR_Signal), MP_ROM_PTR(&machine_signal_type) },
    { MP_ROM_QSTR(MP_QSTR_ADC), MP_ROM_PTR(&machine_adc_type) },
    #if MICROPY_PY_MACHINE_DAC
    { MP_ROM_QSTR(MP_QSTR_DAC), MP_ROM_PTR(&machine_dac_type) },
    #endif
    #if MICROPY_PY_MACHINE_I2C
    { MP_ROM_QSTR(MP_QSTR_I2C), MP_ROM_PTR(&machine_i2c_type) },
    #endif
    #if MICROPY_PY_MACHINE_SOFTI2C
    { MP_ROM_QSTR(MP_QSTR_SoftI2C), MP_ROM_PTR(&mp_machine_soft_i2c_type) },
    #endif
    #if MICROPY_PY_MACHINE_I2S
    { MP_ROM_QSTR(MP_QSTR_I2S), MP_ROM_PTR(&machine_i2s_type) },
    #endif
    { MP_ROM_QSTR(MP_QSTR_PWM), MP_ROM_PTR(&machine_pwm_type) },
    { MP_ROM_QSTR(MP_QSTR_RTC), MP_ROM_PTR(&machine_rtc_type) },
    #if MICROPY_PY_MACHINE_SPI
    { MP_ROM_QSTR(MP_QSTR_SPI), MP_ROM_PTR(&machine_spi_type) },
    #endif
    #if MICROPY_PY_MACHINE_SOFTSPI
    { MP_ROM_QSTR(MP_QSTR_SoftSPI), MP_ROM_PTR(&mp_machine_soft_spi_type) },
    #endif
    { MP_ROM_QSTR(MP_QSTR_UART), MP_ROM_PTR(&machine_uart_type) },

    // Reset/Wake reasons
    { MP_ROM_QSTR(MP_QSTR_reset_cause), MP_ROM_PTR(&machine_reset_cause_obj) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_POWERON), MP_ROM_INT(RESET_SOURCE_POWERON) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_REBOOT), MP_ROM_INT(RESET_SOURCE_REBOOT) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_WATCHDOG), MP_ROM_INT(RESET_SOURCE_WATCHDOG) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_DEEPPS_GPIO), MP_ROM_INT(RESET_SOURCE_DEEPPS_GPIO) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_DEEPPS_RTC), MP_ROM_INT(RESET_SOURCE_DEEPPS_RTC) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_DEEPPS_USB), MP_ROM_INT(RESET_SOURCE_DEEPPS_USB) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_DEEPPS_TOUCH), MP_ROM_INT(RESET_SOURCE_DEEPPS_TOUCH) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_CRASH_ILLEGAL_JUMP), MP_ROM_INT(RESET_SOURCE_CRASH_ILLEGAL_JUMP) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_CRASH_UNDEFINED), MP_ROM_INT(RESET_SOURCE_CRASH_UNDEFINED) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_CRASH_PREFETCH_ABORT), MP_ROM_INT(RESET_SOURCE_CRASH_PREFETCH_ABORT) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_CRASH_DATA_ABORT), MP_ROM_INT(RESET_SOURCE_CRASH_DATA_ABORT) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_CRASH_UNUSED), MP_ROM_INT(RESET_SOURCE_CRASH_UNUSED) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_CRASH_ILLEGAL_INSTRUCTION), MP_ROM_INT(RESET_SOURCE_CRASH_ILLEGAL_INSTRUCTION) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_CRASH_MISALIGNED), MP_ROM_INT(RESET_SOURCE_CRASH_MISALIGNED) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_CRASH_ASSERT), MP_ROM_INT(RESET_SOURCE_CRASH_ASSERT) },
    { MP_ROM_QSTR(MP_QSTR_RESET_SOURCE_UNKNOWN), MP_ROM_INT(RESET_SOURCE_UNKNOWN) },
};

STATIC MP_DEFINE_CONST_DICT(machine_module_globals, machine_module_globals_table);

const mp_obj_module_t mp_module_machine = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&machine_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_umachine, mp_module_machine);

#endif // MICROPY_PY_MACHINE

#ifndef MICROPY_INCLUDED_PORTS_MACHINE_PIN_H
#define MICROPY_INCLUDED_PORTS_MACHINE_PIN_H

#include "driver/gpio.h"

typedef struct _machine_pin_obj_t {
    mp_obj_base_t base;
    gpio_id_t id;
    gpio_config_t config;
} machine_pin_obj_t;

gpio_id_t machine_pin_get_id(mp_obj_t pin_in);

#endif // MICROPY_INCLUDED_PORTS_MACHINE_PIN_H

#ifndef MICROPY_INCLUDED_PORTS_MACHINE_ADC_H
#define MICROPY_INCLUDED_PORTS_MACHINE_ADC_H

#include <driver/adc.h>

typedef struct _madc_obj_t {
    mp_obj_base_t base;
    adc_chan_t channel_id;
} madc_obj_t;

extern void madc_init_helper(const madc_obj_t *self, size_t n_pos_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

#endif // MICROPY_INCLUDED_PORTS_MACHINE_ADC_H

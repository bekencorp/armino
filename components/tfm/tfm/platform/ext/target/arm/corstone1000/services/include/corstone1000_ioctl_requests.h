/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CORSTONE1000_IOCTL_REQUESTS_H
#define CORSTONE1000_IOCTL_REQUESTS_H

#include<stdint.h>


enum corstone1000_ioctl_id_t {
   IOCTL_CORSTONE1000_FWU_FLASH_IMAGES = 0,
   IOCTL_CORSTONE1000_FWU_HOST_ACK,
};


typedef struct corstone1000_ioctl_in_params {

    uint32_t ioctl_id;

} corstone1000_ioctl_in_params_t;

typedef struct corstone1000_ioctl_out_params {

    int32_t result;

} corstone1000_ioctl_out_params_t;

#endif /* CORSTONE1000_IOCTL_REQUESTS_H */

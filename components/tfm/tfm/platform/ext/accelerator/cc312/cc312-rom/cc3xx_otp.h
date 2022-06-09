/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_OTP_H
#define CC3XX_OTP_H

#include "cc3xx_error.h"
#include "cc3xx_otp_defs.h"

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

cc3xx_err_t cc3xx_otp_write(uint8_t *otp_addr, size_t size, const uint8_t *buf);

cc3xx_err_t cc3xx_otp_read(const uint8_t *otp_addr, size_t size, uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_OTP_H */

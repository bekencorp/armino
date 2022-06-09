/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef TFM_READ_RANGES_H__
#define TFM_READ_RANGES_H__

#include <tfm_ioctl_core_api.h>

#include "nrf.h"

#define FICR_BASE               NRF_FICR_S_BASE

#define FICR_INFO_ADDR          (FICR_BASE + offsetof(NRF_FICR_Type, INFO))
#define FICR_INFO_SIZE          (sizeof(FICR_INFO_Type))

/* Used by nrf_erratas.h */
#define FICR_RESTRICTED_ADDR    (FICR_BASE + 0x130)
#define FICR_RESTRICTED_SIZE    0x8

static const struct tfm_read_service_range ranges[] = {
	{ .start = FICR_INFO_ADDR, .size = FICR_INFO_SIZE },
	{ .start = FICR_RESTRICTED_ADDR, .size = FICR_RESTRICTED_SIZE },
};

#endif /* TFM_READ_RANGES_H__ */

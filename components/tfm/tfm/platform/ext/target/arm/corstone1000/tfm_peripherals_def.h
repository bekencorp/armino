/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PERIPHERALS_DEF_H__
#define __TFM_PERIPHERALS_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

struct tfm_spm_partition_platform_data_t;

extern struct tfm_spm_partition_platform_data_t tfm_peripheral_std_uart;

#define TFM_PERIPHERAL_STD_UART     (&tfm_peripheral_std_uart)

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PERIPHERALS_DEF_H__ */

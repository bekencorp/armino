/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CONFIG_PARTITION_FWU_H__
#define __CONFIG_PARTITION_FWU_H__

#include "config_tfm.h"

/* Size of the FWU internal data transfer buffer */
#ifndef TFM_FWU_BUF_SIZE
#pragma message("TFM_FWU_BUF_SIZE is defaulted to PSA_FWU_MAX_WRITE_SIZE. Please check and set it explicitly.")
#define TFM_FWU_BUF_SIZE               PSA_FWU_MAX_WRITE_SIZE
#endif

/* The stack size of the Firmware Update Secure Partition */
#ifndef FWU_STACK_SIZE
#pragma message("FWU_STACK_SIZE is defaulted to 0x600. Please check and set it explicitly.")
#define FWU_STACK_SIZE                 0x600
#endif

#endif /* __CONFIG_PARTITION_FWU_H__ */

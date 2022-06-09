/*
 * Copyright (c) 2012-2017 Andes Technology Corporation
 * All rights reserved.
 *
 */

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "core_v5.h"
#include "mcu_ip.h"

/*
 * Define 'NDS_PLIC_BASE' and 'NDS_PLIC_SW_BASE' before include platform
 * intrinsic header file to active PLIC/PLIC_SW related intrinsic functions.
 */
#define NDS_PLIC_BASE        PLIC_BASE
#define NDS_PLIC_SW_BASE     PLIC_SW_BASE
#include "nds_v5_platform.h"

/*****************************************************************************
 * Peripheral device HAL declaration
 ****************************************************************************/
#define DEV_PLMT             MCUIP_PLMT
#define DEV_SMU              MCUIP_SMU

#define CFG_XIP              (1)

#ifdef __cplusplus
}
#endif

#endif	/* __PLATFORM_H__ */

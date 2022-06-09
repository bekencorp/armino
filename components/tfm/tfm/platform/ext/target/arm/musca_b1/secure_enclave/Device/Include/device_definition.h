/*
 * Copyright (c) 2017-2020 Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file device_definition.h
 * \brief The structure definitions in this file are exported based on the
 * peripheral definitions from device_cfg.h.
 * This file is meant to be used as a helper for baremetal
 * applications and/or as an example of how to configure the generic
 * driver structures.
 */

#ifndef __MUSCA_B1_SECURE_ENCLAVE_DEVICE_DEFINITION_H__
#define __MUSCA_B1_SECURE_ENCLAVE_DEVICE_DEFINITION_H__

#include "device_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef GFC100_EFLASH0_S
#include "gfc100_eflash_drv.h"
extern struct gfc100_eflash_dev_t GFC100_EFLASH0_DEV_S;
#endif
#ifdef GFC100_EFLASH1_S
#include "gfc100_eflash_drv.h"
extern struct gfc100_eflash_dev_t GFC100_EFLASH1_DEV_S;
#endif

/* QSPI Flash Controller driver structures */
#ifdef QSPI_IP6514E_S
#include "qspi_ip6514e_drv.h"
extern struct qspi_ip6514e_dev_t QSPI_DEV_S;
#endif

/* MT25QL Flash memory library structures */
#if (defined(MT25QL_S) && defined(QSPI_IP6514E_S))
#include "mt25ql_flash_lib.h"
extern struct mt25ql_dev_t MT25QL_DEV_S;
#endif

/* MHU driver structures */
#ifdef MHU0_SENDER
#include "mhu_v2_x.h"
extern struct mhu_v2_x_dev_t MHU0_SENDER_DEV;
#endif

#ifdef MHU0_RECEIVER
#include "mhu_v2_x.h"
extern struct mhu_v2_x_dev_t MHU0_RECEIVER_DEV;
#endif

#ifdef __cplusplus
}
#endif

#endif  /* __MUSCA_B1_SECURE_ENCLAVE_DEVICE_DEFINITION_H__ */

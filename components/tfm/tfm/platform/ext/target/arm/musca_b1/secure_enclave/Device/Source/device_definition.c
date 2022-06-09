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
 * \file device_definition.c
 * \brief This file defines exports the structures based on the peripheral
 * definitions from device_cfg.h.
 * This retarget file is meant to be used as a helper for baremetal
 * applications and/or as an example of how to configure the generic
 * driver structures.
 */

#include "device_cfg.h"
#include "device_definition.h"
#include "platform_base_address.h"
#include "tfm_plat_defs.h"

/** GFC-100 eflash driver structures */
#ifdef GFC100_EFLASH0_S
static const struct gfc100_eflash_dev_cfg_t GFC100_EFLASH0_CFG_S = {
    .base = REMAPPED_MUSCA_B1_EFLASH0_REG_MAP_S_BASE};
static struct gfc100_eflash_dev_data_t GFC100_EFLASH0_DATA_S = {
    .is_initialized = false,
    .flash_size = 0};
struct gfc100_eflash_dev_t GFC100_EFLASH0_DEV_S = {&(GFC100_EFLASH0_CFG_S),
                                                   &(GFC100_EFLASH0_DATA_S)};
#endif

#ifdef GFC100_EFLASH1_S
static const struct gfc100_eflash_dev_cfg_t GFC100_EFLASH1_CFG_S = {
    .base = REMAPPED_MUSCA_B1_EFLASH1_REG_MAP_S_BASE};
static struct gfc100_eflash_dev_data_t GFC100_EFLASH1_DATA_S = {
    .is_initialized = false,
    .flash_size = 0};
struct gfc100_eflash_dev_t GFC100_EFLASH1_DEV_S = {&(GFC100_EFLASH1_CFG_S),
                                                   &(GFC100_EFLASH1_DATA_S)};
#endif

/* QSPI IP6514E driver structures */
#ifdef QSPI_IP6514E_S
static const struct qspi_ip6514e_dev_cfg_t QSPI_DEV_CFG_S = {
    .base = REMAPPED_MUSCA_B1_QSPI_REG_S_BASE,
    .addr_mask = (1U << 23) - 1, /* 8MiB minus 1 byte */
};
struct qspi_ip6514e_dev_t QSPI_DEV_S = {
    &QSPI_DEV_CFG_S
};
#endif

/* MT25QL Flash memory library structures */
#if (defined(MT25QL_S) && defined(QSPI_IP6514E_S))
struct mt25ql_dev_t MT25QL_DEV_S = {
    .controller = &QSPI_DEV_S,
    /* Direct access not possible from Secure Enclave, so dummy address set */
    .direct_access_start_addr = 0xFFFFFFFF,
    .baud_rate_div = 4U,
    .size = 0x00800000U, /* 8 MiB */
    .config_state = { 0 },
};
#endif

/* MHU */
#ifdef MHU0_SENDER
struct mhu_v2_x_dev_t MHU0_SENDER_DEV = {
        .base = MHU0_SENDER_FRAME_BASE,
        .frame = MHU_V2_X_SENDER_FRAME
};
#endif

#ifdef MHU0_RECEIVER
struct mhu_v2_x_dev_t MHU0_RECEIVER_DEV = {
        .base = MHU0_RECEIVER_FRAME_BASE,
        .frame = MHU_V2_X_RECEIVER_FRAME
};
#endif

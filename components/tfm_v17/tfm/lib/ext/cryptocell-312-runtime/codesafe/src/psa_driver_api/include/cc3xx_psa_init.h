/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_PSA_INIT_H
#define CC3XX_PSA_INIT_H

/** \file cc3xx_psa_init.h
 *
 * This file contains the declaration of the entry points associated to
 * driver initialisation and de-initialisation procedures.
 *
 */

#include "psa/crypto.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Initialises the CC3XX driver
 *
 * \retval  PSA_SUCCESS on success. Error code from \ref psa_status_t on
 *          failure
 */
psa_status_t cc3xx_init(void);

/*!
 * \brief De-Initialises the CC3XX driver
 *
 * \retval  PSA_SUCCESS on success. Error code from \ref psa_status_t on
 *          failure
 */
psa_status_t cc3xx_free(void);

#ifdef __cplusplus
}
#endif
#endif /* CC3XX_PSA_INIT_H */

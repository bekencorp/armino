/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_CONFIG_H
#define CC3XX_CONFIG_H

/** \file cc3xx_config.h
 *
 * This file includes the configuration items specific to the CC3XX driver which
 * are not covered by the configuration mechanisms available in the PSA config
 * file. A description of the options is available in \ref cc3xx.h
 */

//#define CC3XX_CONFIG_ENABLE_CC_TO_PSA_TYPE_CONVERSION
#define CC3XX_CONFIG_ENABLE_AEAD_ONE_SHOT_USE_MULTIPART
//#define CC3XX_CONFIG_ENABLE_AEAD_AES_CACHED_MODE

#endif /* CC3XX_CONFIG_H */

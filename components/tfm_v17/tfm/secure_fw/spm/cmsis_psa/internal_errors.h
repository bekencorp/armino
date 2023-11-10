/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __INTERNAL_ERRORS_H__
#define __INTERNAL_ERRORS_H__

#include "psa/error.h"

/* SPM implementation error range from -249 to -256. */
#define SPM_ERROR_BAD_PARAMETERS   ((psa_status_t)-249)
#define SPM_ERROR_SHORT_BUFFER     ((psa_status_t)-250)
#define SPM_ERROR_VERSION          ((psa_status_t)-251)
#define SPM_ERROR_MEMORY_CHECK     ((psa_status_t)-252)
#define SPM_ERROR_GENERIC          ((psa_status_t)-253)

#endif /* __INTERNAL_ERRORS_H__ */

/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __BACKEND_IPC_H__
#define __BACKEND_IPC_H__

/* Calculate the service setting. In IPC it is the signal set. */
#define BACKEND_SERVICE_SET(set, p_service) ((set) |= (p_service)->signal)

#endif /* __BACKEND_IPC_H__ */

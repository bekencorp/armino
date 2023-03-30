/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TEST_INTERRUPT_H__
#define __TEST_INTERRUPT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#if defined(TEST_NS_FPU) || defined(TEST_S_FPU)
/* Change FP caller and callee registers. */
void TFM_FPU_S_TEST_Handler(void);
#endif

#ifdef TEST_NS_FPU
/*
 * Change FP caller and callee registers. For interrupt triggerred by secure
 * thread, all FP registers should be checked whether cleared.
 */
void TFM_FPU_NS_TEST_Handler(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __TEST_INTERRUPT_H__ */

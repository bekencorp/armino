/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 * Copyright (c) 2021 STMicroelectronics. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "crypto_hw.h"


/*
 * \brief Initialize the stm crypto accelerator
 */

int crypto_hw_accelerator_init(void)
{
#if defined(GENERATOR_HW_CRYPTO_DPA_SUPPORTED)
    __HAL_RCC_SHSI_ENABLE();
#endif
    return 0;
}

/*
 * \brief Deallocate the stm crypto accelerator
 */
int crypto_hw_accelerator_finish(void)
{
    return 0;
}



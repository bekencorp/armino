/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TARGET_CFG_H__
#define __TARGET_CFG_H__

#include <stdint.h>
#include <stddef.h>

#define TFM_DRIVER_STDIO    Driver_USART0

/**
 * \brief Holds the data necessary to do isolation for a specific peripheral.
 */
struct platform_data_t {
    uint32_t periph_start;
    uint32_t periph_limit;
};

#endif /* __TARGET_CFG_H__ */

/*
 * Copyright (c) 2019 - 2020, Nordic Semiconductor ASA
 * Copyright (c) 2021, Laird Connectivity
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NRFX_CONFIG_H__
#define NRFX_CONFIG_H__

#include <RTE_Device.h>

#if RTE_FLASH0
#define NRFX_NVMC_ENABLED 1
#endif

#if RTE_QSPI0
#define NRFX_QSPI_ENABLED 1
#endif

#if RTE_USART0 || RTE_USART1 || RTE_USART2 || RTE_USART3
#define NRFX_UARTE_ENABLED 1
#endif
#if RTE_USART0
#define NRFX_UARTE0_ENABLED 1
#endif
#if RTE_USART1
#define NRFX_UARTE1_ENABLED 1
#endif
#if RTE_USART2
#define NRFX_UARTE2_ENABLED 1
#endif
#if RTE_USART3
#define NRFX_UARTE3_ENABLED 1
#endif

#if RTE_TWIM2
#define NRFX_TWIM_ENABLED 1
#define NRFX_TWIM2_ENABLED 1
#endif

/*
 * For chips with TrustZone support, MDK provides CMSIS-Core peripheral
 * accessing symbols in two flavors, with secure and non-secure base address
 * mappings. Their names contain the suffix _S or _NS, respectively.
 * Because nrfx HALs and drivers require these peripheral accessing symbols
 * without any suffixes, the following macro is provided that will translate
 * their names according to the kind of the target that is built.
 */
#if defined(NRF_TRUSTZONE_NONSECURE)
#define NRF_PERIPH(P) P##_NS
#else
#define NRF_PERIPH(P) P##_S
#endif

#if defined(NRF5340_XXAA_APPLICATION)
    #include <nrfx_config_nrf5340_application.h>
#else
    #error "Unknown device."
#endif

#endif // NRFX_CONFIG_H__

/*
 * Copyright (c) 2020 Arm Limited
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

#include "cmsis.h"
#include "mhu_v2_x.h"
#include "platform_irq.h"
#include "platform_regs.h"
#include "musca_b1_scc_drv.h"
#include "device_definition.h"
/*
 * Semihosting is a mechanism that enables code running on an ARM target
 * to communicate and use the Input/Output facilities of a host computer
 * that is running a debugger.
 * There is an issue where if you use armclang at -O0 optimisation with
 * no parameters specified in the main function, the initialisation code
 * contains a breakpoint for semihosting by default. This will stop the
 * code from running before main is reached.
 * Semihosting can be disabled by defining __ARM_use_no_argv symbol
 * (or using higher optimization level).
 */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm("  .global __ARM_use_no_argv\n");
#endif

void start_secure_enclave(void);
void jump_to_vtor(uint32_t addr);

#define AZ_SYS_RESET_VTOR     0xA0
#define AZ_CODE_RESET_VTOR    0x38
#define AZ_ROM_RESET_VTOR     0x00

#define AZ_ROM_REMAP_OFFSET   0x1A020000
#define AZ_ROM_REMAP_MASK     0x0001FFFF

#define AZ_CODE_REMAP_OFFSET  0x1A200000
#define AZ_CODE_REMAP_MASK    0x003FFFFF

#define AZ_SYS_REMAP_OFFSET   0x52000000
#define AZ_SYS_REMAP_MASK     0x00FFFFFF

void start_secure_enclave(void)
{
    musca_b1_scc_set_az_cpu_vtor(&MUSCA_B1_SCC_DEV_S, AZ_SYS_RESET_VTOR,
                                 AZ_CODE_RESET_VTOR, AZ_ROM_RESET_VTOR);

    musca_b1_scc_set_az_rom_remap(&MUSCA_B1_SCC_DEV_S, AZ_ROM_REMAP_OFFSET,
                                  AZ_ROM_REMAP_MASK);

    musca_b1_scc_set_az_code_remap(&MUSCA_B1_SCC_DEV_S, AZ_CODE_REMAP_OFFSET,
                                   AZ_CODE_REMAP_MASK);

    musca_b1_scc_set_az_sys_remap(&MUSCA_B1_SCC_DEV_S, AZ_SYS_REMAP_OFFSET,
                                  AZ_SYS_REMAP_MASK);

    musca_b1_scc_enable_az_boot_remap(&MUSCA_B1_SCC_DEV_S);

    musca_b1_scc_az_release_from_reset(&MUSCA_B1_SCC_DEV_S);
}

int main()
{
    uint32_t new_vtor = 0;

    mhu_v2_x_driver_init(&SE_MHU_RECEIVER_DEV, MHU_REV_2_0);

    start_secure_enclave();

    while(!NVIC_GetPendingIRQ(HostMHUR0_IRQ_Reg0_IRQn)) {
        ;
    }

    /* Receive the new SSE-200 Reset Vector Address */
    mhu_v2_x_channel_receive(&SE_MHU_RECEIVER_DEV, 0, &new_vtor);
    mhu_v2_x_channel_clear(&SE_MHU_RECEIVER_DEV, 0);

    jump_to_vtor(new_vtor);
}

__attribute__ ((naked)) void jump_to_vtor(uint32_t addr)
{
    /* Read the Initial Stack Pointer and the address of the Reset Handler from
     * the address given as the input argument. Than update the MSP with the
     * read value and jump to the read Reset Handler.
     */
    __asm volatile("LDRD R1, [R0]\n"
                   "MSR MSP, R1\n"
                   "BX  R2");
}

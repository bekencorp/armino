/*
 * Copyright (c) 2017-2021 Arm Limited. All rights reserved.
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

/*
 * This file is derivative of CMSIS V5.01:
 * Device\_Template_Vendor\Vendor\Device\Include\Device.h
 */

#ifndef __PLATFORM_BASE_ADDRESS_H__
#define __PLATFORM_BASE_ADDRESS_H__

/* Secure Enclave Peripheral base addresses */
#define CORSTONE1000_ROM_BASE                      (0x00000000U) /* SE ROM Region                     */
#define CORSTONE1000_CRYPTO_ACCELERATOR_BASE       (0x2F000000U) /* Crypto Accelerator                */
#define CORSTONE1000_SRAM_BASE                     (0x30000000U) /* SE RAM Region                     */
#define CORSTONE1000_PERIPHERAL_BASE               (0x50000000U) /* SE Peripheral Region              */
#define CORSTONE1000_CMSDK_TIMER_0_BASE            (0x50000000U) /* CMSDK Timer 0                     */
#define CORSTONE1000_CMSDK_TIMER_1_BASE            (0x50001000U) /* CMSDK Timer 1                     */
#define CORSTONE1000_SEH_0_SENDER_BASE             (0x50003000U) /* SEH0 Sender                       */
#define CORSTONE1000_HSE_0_RECEIVER_BASE           (0x50004000U) /* HSE1 Receiver                     */
#define CORSTONE1000_SEH_1_SENDER_BASE             (0x50005000U) /* SEH1 Sender                       */
#define CORSTONE1000_HSE_1_RECEIVER_BASE           (0x50006000U) /* HSE1 Receiver                     */
/* Not all of the SEESx/ESxSE peripherals will be applicable, depending on the
 * number of external systems present         */
#define CORSTONE1000_SEES0_0_SENDER_BASE           (0x50010000U) /* SEES0 0 Sender                    */
#define CORSTONE1000_ES0SE_0_RECEIVER_BASE         (0x50011000U) /* ES0SE 0 Receiver                  */
#define CORSTONE1000_SEES0_1_SENDER_BASE           (0x50012000U) /* SEES0 1 Sender                    */
#define CORSTONE1000_ES0SE_1_RECEIVER_BASE         (0x50013000U) /* ES0SE 1 Receiver                  */
#define CORSTONE1000_SEES1_0_SENDER_BASE           (0x50014000U) /* SEES1 0 Sender                    */
#define CORSTONE1000_ES1SE_0_RECEIVER_BASE         (0x50015000U) /* ES1SE 0 Receiver                  */
#define CORSTONE1000_SEES1_1_SENDER_BASE           (0x50016000U) /* SEES1 1 Sender                    */
#define CORSTONE1000_ES1SE_1_RECEIVER_BASE         (0x50017000U) /* ES1SE 1 Receiver                  */
#define CORSTONE1000_SEES2_0_SENDER_BASE           (0x50018000U) /* SEES2 0 Sender                    */
#define CORSTONE1000_ES2SE_0_RECEIVER_BASE         (0x50019000U) /* ES2SE 0 Receiver                  */
#define CORSTONE1000_SEES2_1_SENDER_BASE           (0x5001A000U) /* SEES2 1 Sender                    */
#define CORSTONE1000_ES2SE_1_RECEIVER_BASE         (0x5001B000U) /* ES2SE 1 Receiver                  */
#define CORSTONE1000_SEES3_0_SENDER_BASE           (0x5001C000U) /* SEES3 0 Sender                    */
#define CORSTONE1000_ES3SE_0_RECEIVER_BASE         (0x5001D000U) /* ES3SE 0 Receiver                  */
#define CORSTONE1000_SEES3_1_SENDER_BASE           (0x5001E000U) /* SEES3 1 Sender                    */
#define CORSTONE1000_ES3SE_1_RECEIVER_BASE         (0x5001F000U) /* ES3SE 1 Receiver                  */
#define CORSTONE1000_SCR_BASE                      (0x50080000U) /* System Control Register           */
#define CORSTONE1000_WATCHDOG_TIMER_BASE           (0x50081000U) /* Watchdog Timer                    */
#define CORSTONE1000_SECENCTOP_PPU_BASE            (0x5008D000U) /* SECENCTOP PPU                     */
#define CORSTONE1000_BASE_SCR_BASE                 (0x5008E000U) /* SE Base System Control Register   */
#define CORSTONE1000_SOC_WATCHDOG_BASE             (0x5008F000U) /* SoC Watchdog                      */
#define CORSTONE1000_UART_BASE                     (0x50090000U) /* UART                              */
#define CORSTONE1000_SE_FIREWALL_BASE              (0x50200000U) /* SE Firewall                       */
#define CORSTONE1000_HOST_ACCESS_REGION_BASE       (0x60000000U) /* Host Access Region                */
#define CORSTONE1000_PPB_BASE                      (0xE0000000U) /* Private Peripheral Bus (PPB)      */
#define CORSTONE1000_CS_ROM_BASE                   (0xF0000000U) /* SE CS ROM                         */
#define CORSTONE1000_CTI_BASE                      (0xF0001000U) /* SE CTI                            */

/* Host base addresses from the SE perspective - partial list, only the ones
 * required by the SE are defined here */
#define CORSTONE1000_HOST_ADDRESS_SPACE_BASE       (0x60000000U) /* Host Address Space                */
#define CORSTONE1000_HOST_BIR_BASE                 (0x60000000U) /* Boot Instruction Register         */
#define CORSTONE1000_HOST_SHARED_RAM_BASE          (0x62000000U) /* Shared RAM                        */
#define CORSTONE1000_HOST_XNVM_BASE                (0x68000000U) /* XNVM                              */
#define CORSTONE1000_HOST_BASE_SYSTEM_CONTROL_BASE (0x7A010000U) /* Host SCB                          */
#define CORSTONE1000_HOST_FIREWALL_BASE            (0x7A800000U) /* Host Firewall                     */
#define CORSTONE1000_HOST_INT_APBCOM_BASE          (0x7B900000U) /* Internal APBCOM                   */
#define CORSTONE1000_HOST_FPGA_SCC_REGISTERS       (0x80000000U) /* FPGA SCC Registers                */
#define CORSTONE1000_HOST_SE_SECURE_FLASH_BASE_FVP (0x80010000U) /* SE Flash                          */
#define CORSTONE1000_HOST_AXI_QSPI_CTRL_REG_BASE   (0x80050000U) /* AXI QSPI Controller               */
#define CORSTONE1000_HOST_AXI_QSPI_CTRL_REG_BASE_SE_SECURE_FLASH (0x90010000U) /* AXI QSPI Controller for SE FLash  */
#define CORSTONE1000_HOST_DRAM_UEFI_CAPSULE        (0xA0000000U) /* 1.5 GB DDR                        */

#endif  /* __PLATFORM_BASE_ADDRESS_H__ */

/*
 * Copyright (c) 2009-2019 Arm Limited.
 * Copyright (c) 2019-2020 ArmChina.
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "components/log.h"
#include "components/system.h"
#include "bk7236xx.h"
#include "aon_pmu_driver.h"
#include "driver/uart.h"

#if CONFIG_CM_BACKTRACE
#include "cm_backtrace.h"
#endif

#define TAG "arch"

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern __NO_RETURN void __PROGRAM_START(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler  (void);
            void Default_Handler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler            (void) __attribute__ ((weak));
void HardFault_Handler      (void) __attribute__ ((weak));
void MemManage_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void SecureFault_Handler    (void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));

/* Interrupt Handler */
void DMA0_NSEC_Handler               (void) __attribute__ ((weak));
void ENCP_SEC_Handler                (void) __attribute__ ((weak));
void ENCP_NSEC_Handler               (void) __attribute__ ((weak));
void TIMER0_Handler                  (void) __attribute__ ((weak));
void UART0_Handler                   (void) __attribute__ ((weak));
void PWM0_Handler                    (void) __attribute__ ((weak));
void I2C0_Handler                    (void) __attribute__ ((weak));
void SPI0_Handler                    (void) __attribute__ ((weak));
void SARADC_Handler                  (void) __attribute__ ((weak));
void IRDA_Handler                    (void) __attribute__ ((weak));
void SDIO_Handler                    (void) __attribute__ ((weak));
void GDMA_Handler                    (void) __attribute__ ((weak));
void LA_Handler                      (void) __attribute__ ((weak));
void TIMER1_Handler                  (void) __attribute__ ((weak));
void I2C1_Handler                    (void) __attribute__ ((weak));
void UART1_Handler                   (void) __attribute__ ((weak));
void UART2_Handler                   (void) __attribute__ ((weak));
void SPI1_Handler                    (void) __attribute__ ((weak));
void CAN_Handler                     (void) __attribute__ ((weak));
void USB_Handler                     (void) __attribute__ ((weak));
void QSPI0_Handler                   (void) __attribute__ ((weak));
void FFT_Handler                     (void) __attribute__ ((weak));
void SBC_Handler                     (void) __attribute__ ((weak));
void AUDIO_Handler                   (void) __attribute__ ((weak));
void I2S0_Handler                    (void) __attribute__ ((weak));
void JPEG_ENC_Handler                (void) __attribute__ ((weak));
void JPEG_DEC_Handler                (void) __attribute__ ((weak));
void DISPLAY_Handler                 (void) __attribute__ ((weak));
void DMA2D_Handler                   (void) __attribute__ ((weak));
void PHY_MBP_Handler                 (void) __attribute__ ((weak));
void PHY_RIU_Handler                 (void) __attribute__ ((weak));
void MAC_INT_TX_RX_TIMER_Handler     (void) __attribute__ ((weak));
void MAC_INT_TX_RX_MISC_Handler      (void) __attribute__ ((weak));
void MAC_INT_RX_TRIGGER_Handler      (void) __attribute__ ((weak));
void MAC_INT_TX_TRIGGER_Handler      (void) __attribute__ ((weak));
void MAC_INT_PORT_TRIGGER_Handler    (void) __attribute__ ((weak));
void MAC_INT_GEN_Handler             (void) __attribute__ ((weak));
void HSU_Handler                     (void) __attribute__ ((weak));
void INT_MAC_WAKEUP_Handler          (void) __attribute__ ((weak));
void DM_Handler                      (void) __attribute__ ((weak));
void BLE_Handler                     (void) __attribute__ ((weak));
void BT_Handler                      (void) __attribute__ ((weak));
void QSPI1_Handler                   (void) __attribute__ ((weak));
void PWM1_Handler                    (void) __attribute__ ((weak));
void I2S1_Handler                    (void) __attribute__ ((weak));
void I2S2_Handler                    (void) __attribute__ ((weak));
void H264_Handler                    (void) __attribute__ ((weak));
void SDMADC_Handler                  (void) __attribute__ ((weak));
void MBOX0_Handler                   (void) __attribute__ ((weak));
void MBOX1_Handler                   (void) __attribute__ ((weak));
void BMC64_Handler                   (void) __attribute__ ((weak));
void DPLL_UNLOCK_Handler             (void) __attribute__ ((weak));
void TOUCH_Handler                   (void) __attribute__ ((weak));
void USB_PLUG_Handler                (void) __attribute__ ((weak));
void RTC_Handler                     (void) __attribute__ ((weak));
void GPIO_Handler                    (void) __attribute__ ((weak));
void DMA1_SEC_Handler                (void) __attribute__ ((weak));
void DMA1_NSEC_Handler               (void) __attribute__ ((weak));
void YUV_BUF_Handler                 (void) __attribute__ ((weak));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#if CONFIG_CPU_WITHOUT_BOOTLOADER

const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),       /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  NMI_Handler,                              /* -14 NMI Handler */
  HardFault_Handler,                        /* -13 Hard Fault Handler */
  MemManage_Handler,                        /* -12 MPU Fault Handler */
  BusFault_Handler,                         /* -11 Bus Fault Handler */
  UsageFault_Handler,                       /* -10 Usage Fault Handler */
  SecureFault_Handler,                      /*  -9 Secure Fault Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  SVC_Handler,                              /*  -5 SVCall Handler */
  DebugMon_Handler,                         /*  -4 Debug Monitor Handler */
  0,                                        /*     Reserved */
  PendSV_Handler,                           /*  -2 PendSV Handler */
  SysTick_Handler,                          /*  -1 SysTick Handler */

  /* Interrupts */
  DMA0_NSEC_Handler,
  ENCP_SEC_Handler,
  ENCP_NSEC_Handler,
  TIMER0_Handler,
  UART0_Handler,
  PWM0_Handler,
  I2C0_Handler,
  SPI0_Handler,
  SARADC_Handler,
  IRDA_Handler,
  SDIO_Handler,
  GDMA_Handler,
  LA_Handler,
  TIMER1_Handler,
  I2C1_Handler,
  UART1_Handler,
  UART2_Handler,
  SPI1_Handler,
  CAN_Handler,
  USB_Handler,
  QSPI0_Handler,
  FFT_Handler,
  SBC_Handler,
  AUDIO_Handler,
  I2S0_Handler,
  JPEG_ENC_Handler,
  JPEG_DEC_Handler,
  DISPLAY_Handler,
  DMA2D_Handler,
  PHY_MBP_Handler,
  PHY_RIU_Handler,
  MAC_INT_TX_RX_TIMER_Handler,
  MAC_INT_TX_RX_MISC_Handler,
  MAC_INT_RX_TRIGGER_Handler,
  MAC_INT_TX_TRIGGER_Handler,
  MAC_INT_PORT_TRIGGER_Handler,
  MAC_INT_GEN_Handler,
  HSU_Handler,
  INT_MAC_WAKEUP_Handler,
  DM_Handler,
  BLE_Handler,
  BT_Handler,
  QSPI1_Handler,
  PWM1_Handler,
  I2S1_Handler,
  I2S2_Handler,
  H264_Handler,
  SDMADC_Handler,
  /* BK7236 legacy download mode requires that the flash offset 0x100 is 'BK7236'.*/
  (void (*)(void))0x32374B42,
  (void (*)(void))0x00003633,
  BMC64_Handler,
  DPLL_UNLOCK_Handler,
  TOUCH_Handler,
  USB_PLUG_Handler,
  RTC_Handler,
  GPIO_Handler,
  DMA1_SEC_Handler,
  DMA1_NSEC_Handler,
  YUV_BUF_Handler,
                                            /* Interrupts 64 .. 480 are left out */
};


__attribute__((used, section(".vectors_cpu0"))) \
const VECTOR_TABLE_Type __VECTOR_TABLE_CPU0[] = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),       /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  NMI_Handler,                              /* -14 NMI Handler */
  HardFault_Handler,                        /* -13 Hard Fault Handler */
  MemManage_Handler,                        /* -12 MPU Fault Handler */
  BusFault_Handler,                         /* -11 Bus Fault Handler */
  UsageFault_Handler,                       /* -10 Usage Fault Handler */
  SecureFault_Handler,                      /*  -9 Secure Fault Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  SVC_Handler,                              /*  -5 SVCall Handler */
  DebugMon_Handler,                         /*  -4 Debug Monitor Handler */
  0,                                        /*     Reserved */
  PendSV_Handler,                           /*  -2 PendSV Handler */
  SysTick_Handler,                          /*  -1 SysTick Handler */

  /* Interrupts */
  DMA0_NSEC_Handler,
  ENCP_SEC_Handler,
  ENCP_NSEC_Handler,
  TIMER0_Handler,
  UART0_Handler,
  PWM0_Handler,
  I2C0_Handler,
  SPI0_Handler,
  SARADC_Handler,
  IRDA_Handler,
  SDIO_Handler,
  GDMA_Handler,
  LA_Handler,
  TIMER1_Handler,
  I2C1_Handler,
  UART1_Handler,
  UART2_Handler,
  SPI1_Handler,
  CAN_Handler,
  USB_Handler,
  QSPI0_Handler,
  FFT_Handler,
  SBC_Handler,
  AUDIO_Handler,
  I2S0_Handler,
  JPEG_ENC_Handler,
  JPEG_DEC_Handler,
  DISPLAY_Handler,
  DMA2D_Handler,
  PHY_MBP_Handler,
  PHY_RIU_Handler,
  MAC_INT_TX_RX_TIMER_Handler,
  MAC_INT_TX_RX_MISC_Handler,
  MAC_INT_RX_TRIGGER_Handler,
  MAC_INT_TX_TRIGGER_Handler,
  MAC_INT_PORT_TRIGGER_Handler,
  MAC_INT_GEN_Handler,
  HSU_Handler,
  INT_MAC_WAKEUP_Handler,
  DM_Handler,
  BLE_Handler,
  BT_Handler,
  QSPI1_Handler,
  PWM1_Handler,
  I2S1_Handler,
  I2S2_Handler,
  H264_Handler,
  SDMADC_Handler,
  MBOX0_Handler,
  MBOX1_Handler,
  BMC64_Handler,
  DPLL_UNLOCK_Handler,
  TOUCH_Handler,
  USB_PLUG_Handler,
  RTC_Handler,
  GPIO_Handler,
  DMA1_SEC_Handler,
  DMA1_NSEC_Handler,
  YUV_BUF_Handler,
                                            /* Interrupts 64 .. 480 are left out */
};

#else //#if CONFIG_CPU_WITHOUT_BOOTLOADER

const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),       /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  NMI_Handler,                              /* -14 NMI Handler */
  HardFault_Handler,                        /* -13 Hard Fault Handler */
  MemManage_Handler,                        /* -12 MPU Fault Handler */
  BusFault_Handler,                         /* -11 Bus Fault Handler */
  UsageFault_Handler,                       /* -10 Usage Fault Handler */
  SecureFault_Handler,                      /*  -9 Secure Fault Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  SVC_Handler,                              /*  -5 SVCall Handler */
  DebugMon_Handler,                         /*  -4 Debug Monitor Handler */
  0,                                        /*     Reserved */
  PendSV_Handler,                           /*  -2 PendSV Handler */
  SysTick_Handler,                          /*  -1 SysTick Handler */

  /* Interrupts */
  DMA0_NSEC_Handler,
  ENCP_SEC_Handler,
  ENCP_NSEC_Handler,
  TIMER0_Handler,
  UART0_Handler,
  PWM0_Handler,
  I2C0_Handler,
  SPI0_Handler,
  SARADC_Handler,
  IRDA_Handler,
  SDIO_Handler,
  GDMA_Handler,
  LA_Handler,
  TIMER1_Handler,
  I2C1_Handler,
  UART1_Handler,
  UART2_Handler,
  SPI1_Handler,
  CAN_Handler,
  USB_Handler,
  QSPI0_Handler,
  FFT_Handler,
  SBC_Handler,
  AUDIO_Handler,
  I2S0_Handler,
  JPEG_ENC_Handler,
  JPEG_DEC_Handler,
  DISPLAY_Handler,
  DMA2D_Handler,
  PHY_MBP_Handler,
  PHY_RIU_Handler,
  MAC_INT_TX_RX_TIMER_Handler,
  MAC_INT_TX_RX_MISC_Handler,
  MAC_INT_RX_TRIGGER_Handler,
  MAC_INT_TX_TRIGGER_Handler,
  MAC_INT_PORT_TRIGGER_Handler,
  MAC_INT_GEN_Handler,
  HSU_Handler,
  INT_MAC_WAKEUP_Handler,
  DM_Handler,
  BLE_Handler,
  BT_Handler,
  QSPI1_Handler,
  PWM1_Handler,
  I2S1_Handler,
  I2S2_Handler,
  H264_Handler,
  SDMADC_Handler,
  MBOX0_Handler,
  MBOX1_Handler,
  BMC64_Handler,
  DPLL_UNLOCK_Handler,
  TOUCH_Handler,
  USB_PLUG_Handler,
  RTC_Handler,
  GPIO_Handler,
  DMA1_SEC_Handler,
  DMA1_NSEC_Handler,
  YUV_BUF_Handler,
                                            /* Interrupts 64 .. 480 are left out */
};

#endif //#if CONFIG_CPU_WITHOUT_BOOTLOADER

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void)
{
  __set_MSPLIM((uint32_t)(&__STACK_LIMIT));

  SystemInit();                             /* CMSIS System Initialization */
  __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

/*----------------------------------------------------------------------------
  Hard Fault Handler
 *----------------------------------------------------------------------------*/
void HardFault_Handler(void)
{
#if CONFIG_CM_BACKTRACE
  uint32_t lr = __get_LR();
  uint32_t sp = __get_MSP();

  bk_set_printf_sync(true);
  cm_backtrace_fault(lr, sp);
#endif
  while(1);
}

void NMI_Handler(void)
{
#if CONFIG_CM_BACKTRACE
  uint32_t lr = __get_LR();
  uint32_t sp = __get_MSP();

  BK_LOGW(TAG, "NMI_Handler\r\n");
  bk_set_printf_sync(true);
  cm_backtrace_fault(lr, sp);
#endif

  /* wait for print over */
  while (!bk_uart_is_tx_over(bk_get_printf_port()));
  /* set reset modules */
  aon_pmu_drv_wdt_rst_dev_enable();

  while(1);
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void)
{
  bk_set_printf_sync(true);
  BK_LOGW(TAG, "Default_Handler\r\n");
  while(1);
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#endif


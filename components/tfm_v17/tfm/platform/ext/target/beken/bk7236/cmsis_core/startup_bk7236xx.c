/*
 * Copyright (c) 2022 Arm Limited. All rights reserved.
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

/*
 * This file is derivative of CMSIS V5.9.0 startup_ARMCM33.c
 * Git SHA: 2b7495b8535bdcb306dac29b9ded4cfb679d7e5c
 */

#include "cmsis.h"
#include "system_STAR.h"

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
extern uint64_t __STACK_SEAL;
#endif

extern void __PROGRAM_START(void) __NO_RETURN;

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void Reset_Handler  (void) __NO_RETURN;

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
#define DEFAULT_IRQ_HANDLER(handler_name)  \
void __WEAK handler_name(void) __NO_RETURN; \
void handler_name(void) { \
    while(1); \
}

DEFAULT_IRQ_HANDLER(NMI_Handler)
DEFAULT_IRQ_HANDLER(HardFault_Handler)
DEFAULT_IRQ_HANDLER(MemManage_Handler)
DEFAULT_IRQ_HANDLER(BusFault_Handler)
DEFAULT_IRQ_HANDLER(UsageFault_Handler)
DEFAULT_IRQ_HANDLER(SecureFault_Handler)
DEFAULT_IRQ_HANDLER(SVC_Handler)
DEFAULT_IRQ_HANDLER(DebugMon_Handler)
DEFAULT_IRQ_HANDLER(PendSV_Handler)
DEFAULT_IRQ_HANDLER(SysTick_Handler)

DEFAULT_IRQ_HANDLER(DMA0_NSEC_Handler)
DEFAULT_IRQ_HANDLER(ENCP_SEC_Handler)
DEFAULT_IRQ_HANDLER(ENCP_NSEC_Handler)
DEFAULT_IRQ_HANDLER(TFM_TIMER0_Handler)
DEFAULT_IRQ_HANDLER(UART0_Handler)
DEFAULT_IRQ_HANDLER(PWM0_Handler)
DEFAULT_IRQ_HANDLER(I2C0_Handler)
DEFAULT_IRQ_HANDLER(SPI0_Handler)
DEFAULT_IRQ_HANDLER(SARADC_Handler)
DEFAULT_IRQ_HANDLER(IRDA_Handler)
DEFAULT_IRQ_HANDLER(SDIO_Handler)
DEFAULT_IRQ_HANDLER(GDMA_Handler)
DEFAULT_IRQ_HANDLER(LA_Handler)
DEFAULT_IRQ_HANDLER(TIMER1_Handler)
DEFAULT_IRQ_HANDLER(I2C1_Handler)
DEFAULT_IRQ_HANDLER(UART1_Handler)
DEFAULT_IRQ_HANDLER(UART2_Handler)
DEFAULT_IRQ_HANDLER(SPI1_Handler)
DEFAULT_IRQ_HANDLER(CAN_Handler)
DEFAULT_IRQ_HANDLER(USB_Handler)
DEFAULT_IRQ_HANDLER(QSPI0_Handler)
DEFAULT_IRQ_HANDLER(FFT_Handler)
DEFAULT_IRQ_HANDLER(SBC_Handler)
DEFAULT_IRQ_HANDLER(AUDIO_Handler)
DEFAULT_IRQ_HANDLER(I2S0_Handler)
DEFAULT_IRQ_HANDLER(JPEG_ENC_Handler)
DEFAULT_IRQ_HANDLER(JPEG_DEC_Handler)
DEFAULT_IRQ_HANDLER(DISPLAY_Handler)
DEFAULT_IRQ_HANDLER(DMA2D_Handler)
DEFAULT_IRQ_HANDLER(PHY_MBP_Handler)
DEFAULT_IRQ_HANDLER(PHY_RIU_Handler)
DEFAULT_IRQ_HANDLER(MAC_INT_TX_RX_TIMER_Handler)
DEFAULT_IRQ_HANDLER(MAC_INT_TX_RX_MISC_Handler)
DEFAULT_IRQ_HANDLER(MAC_INT_RX_TRIGGER_Handler)
DEFAULT_IRQ_HANDLER(MAC_INT_TX_TRIGGER_Handler)
DEFAULT_IRQ_HANDLER(MAC_INT_PORT_TRIGGER_Handler)
DEFAULT_IRQ_HANDLER(MAC_INT_GEN_Handler)
DEFAULT_IRQ_HANDLER(HSU_Handler)
DEFAULT_IRQ_HANDLER(INT_MAC_WAKEUP_Handler)
DEFAULT_IRQ_HANDLER(DM_Handler)
DEFAULT_IRQ_HANDLER(BLE_Handler)
DEFAULT_IRQ_HANDLER(BT_Handler)
DEFAULT_IRQ_HANDLER(QSPI1_Handler)
DEFAULT_IRQ_HANDLER(PWM1_Handler)
DEFAULT_IRQ_HANDLER(I2S1_Handler)
DEFAULT_IRQ_HANDLER(I2S2_Handler)
DEFAULT_IRQ_HANDLER(H264_Handler)
DEFAULT_IRQ_HANDLER(SDMADC_Handler)
DEFAULT_IRQ_HANDLER(MBOX0_Handler)
DEFAULT_IRQ_HANDLER(MBOX1_Handler)
DEFAULT_IRQ_HANDLER(BMC64_Handler)
DEFAULT_IRQ_HANDLER(DPLL_UNLOCK_Handler)
DEFAULT_IRQ_HANDLER(TOUCH_Handler)
DEFAULT_IRQ_HANDLER(USB_PLUG_Handler)
DEFAULT_IRQ_HANDLER(RTC_Handler)
DEFAULT_IRQ_HANDLER(GPIO_Handler)
DEFAULT_IRQ_HANDLER(DMA1_SEC_Handler)
DEFAULT_IRQ_HANDLER(DMA1_NSEC_Handler)
DEFAULT_IRQ_HANDLER(YUV_BUF_Handler)


/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const VECTOR_TABLE_Type __VECTOR_TABLE[];
const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
	(VECTOR_TABLE_Type)(&__INITIAL_SP),            /*      Initial Stack Pointer */
	Reset_Handler,                     /*      Reset Handler */
	NMI_Handler,                       /* -14: NMI Handler */
	HardFault_Handler,                 /* -13: Hard Fault Handler */
	MemManage_Handler,                 /* -12: MPU Fault Handler */
	BusFault_Handler,                  /* -11: Bus Fault Handler */
	UsageFault_Handler,                /* -10: Usage Fault Handler */
	SecureFault_Handler,               /*  -9: Secure Fault Handler */
	0,                                 /*  -8: Reserved */
	0,                                 /*  -7: Reserved */
	0,                                 /*  -6: Reserved */
	SVC_Handler,                       /*  -5: SVCall Handler */
	DebugMon_Handler,                  /*  -4: Debug Monitor Handler */
	0,                                 /*  -3: Reserved */
	PendSV_Handler,                    /*  -2: PendSV Handler */
	SysTick_Handler,                   /*  -1: SysTick Handler */

	DMA0_NSEC_Handler,
	ENCP_SEC_Handler,
	ENCP_NSEC_Handler,
	TFM_TIMER0_Handler,
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

#if  CONFIG_TFM_NON_BOOTLOADER
	/* BK7236 legacy download mode requires that the flash offset 0x100 is 'BK7236'.*/
	(void (*)(void))0x32374B42,
	(void (*)(void))0x00003633,
#else
	MBOX0_Handler,
	MBOX1_Handler,
#endif

	BMC64_Handler,
	DPLL_UNLOCK_Handler,
	TOUCH_Handler,
	USB_PLUG_Handler,
	RTC_Handler,
	GPIO_Handler,
	DMA1_SEC_Handler,
	DMA1_NSEC_Handler,
	YUV_BUF_Handler,
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    __disable_irq();
#endif
    __set_PSP((uint32_t)(&__INITIAL_SP));

    __set_MSPLIM((uint32_t)(&__STACK_LIMIT));
    __set_PSPLIM((uint32_t)(&__STACK_LIMIT));

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    __TZ_set_STACKSEAL_S((uint32_t *)(&__STACK_SEAL));
#endif

    SystemInit();                             /* CMSIS System Initialization */
    __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}
// eof


// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "arch_interrupt.h"
#include "bk7236xx.h"

#define INT_NUMBER_MAX              (64)
#define NESTED_IRQ_ENTER()
#define NESTED_VPLIC_COMPLETE_INTERRUPT(int_num)
#define NESTED_IRQ_EXIT()

static int_group_isr_t s_irq_handler[INT_NUMBER_MAX];

static void arch_interrupt_enable(void)
{
	__enable_irq();
	__enable_fault_irq();

	for (IRQn_Type irq_type = 0; irq_type < InterruptMAX_IRQn; irq_type++) {
		__NVIC_EnableIRQ(irq_type);
	}
}

void arch_interrupt_disable(void)
{
        __disable_irq();
        __disable_fault_irq();

        for (IRQn_Type irq_type = 0; irq_type < InterruptMAX_IRQn; irq_type++) {
                __NVIC_DisableIRQ(irq_type);
        }
}

uint64_t arch_interrupt_dump(void)
{
	uint64_t int_ena_bits = 0;

        for (IRQn_Type irq_type = 0; irq_type < InterruptMAX_IRQn; irq_type++) {
		if (__NVIC_GetEnableIRQ(irq_type)) {
			int_ena_bits |= BIT(irq_type);
		}
	}

	return int_ena_bits;
}

void arch_interrupt_retarget(void)
{
        for (IRQn_Type irq_type = 0; irq_type < InterruptMAX_IRQn; irq_type++) {
		NVIC_SetTargetState(irq_type);
	}
}

bk_err_t arch_isr_entry_init(void)
{
	arch_interrupt_enable();

	//TODO: __NVIC_SetPriorityGrouping

	return BK_OK;
}

void arch_interrupt_set_priority(arch_int_src_t int_number, uint32_t int_priority)
{
	if (int_number > 0 && int_number < INT_NUMBER_MAX) {
		__NVIC_SetPriority((IRQn_Type)int_number, int_priority);
	}

	return;
}

void arch_interrupt_register_int(arch_int_src_t int_number, int_group_isr_t isr_callback)
{
	if ((int_number > (INT_ID_MAX - 1)) || isr_callback == NULL) {
		return;
	}

	s_irq_handler[int_number] = isr_callback;
}

void arch_interrupt_unregister_int(arch_int_src_t int_number)
{
	if (int_number > (INT_ID_MAX - 1)) {
		return;
	}

	s_irq_handler[int_number] = NULL;
}

void __attribute__ ((interrupt)) DMA0_NSEC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[0] != NULL) {
		(*(s_irq_handler[0]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(0);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) ENCP_SEC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[1] != NULL) {
		(*(s_irq_handler[1]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(1);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) ENCP_NSEC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[2] != NULL) {
		(*(s_irq_handler[2]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(2);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) TIMER0_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[3] != NULL) {
		(*(s_irq_handler[3]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(3);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) UART0_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[4] != NULL) {
		(*(s_irq_handler[4]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(4);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) PWM0_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[5] != NULL) {
		(*(s_irq_handler[5]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(5);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) I2C0_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[6] != NULL) {
		(*(s_irq_handler[6]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(6);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) SPI0_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[7] != NULL) {
		(*(s_irq_handler[7]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(7);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) SARADC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[8] != NULL) {
		(*(s_irq_handler[8]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(8);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) IRDA_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[9] != NULL) {
		(*(s_irq_handler[9]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(9);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) SDIO_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[10] != NULL) {
		(*(s_irq_handler[10]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(10);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) GDMA_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[11] != NULL) {
		(*(s_irq_handler[11]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(11);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) LA_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[12] != NULL) {
		(*(s_irq_handler[12]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(12);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) TIMER1_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[13] != NULL) {
		(*(s_irq_handler[13]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(13);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) I2C1_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[14] != NULL) {
		(*(s_irq_handler[14]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(14);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) UART1_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[15] != NULL) {
		(*(s_irq_handler[15]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(15);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) UART2_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[16] != NULL) {
		(*(s_irq_handler[16]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(16);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) SPI1_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[17] != NULL) {
		(*(s_irq_handler[17]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(17);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) CAN_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[18] != NULL) {
		(*(s_irq_handler[18]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(18);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) USB_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[19] != NULL) {
		(*(s_irq_handler[19]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(19);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) QSPI0_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[20] != NULL) {
		(*(s_irq_handler[20]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(20);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) FFT_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[21] != NULL) {
		(*(s_irq_handler[21]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(21);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) SBC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[22] != NULL) {
		(*(s_irq_handler[22]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(22);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) AUDIO_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[23] != NULL) {
		(*(s_irq_handler[23]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(23);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) I2S0_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[24] != NULL) {
		(*(s_irq_handler[24]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(24);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) JPEG_ENC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[25] != NULL) {
		(*(s_irq_handler[25]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(25);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) JPEG_DEC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[26] != NULL) {
		(*(s_irq_handler[26]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(26);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) DISPLAY_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[27] != NULL) {
		(*(s_irq_handler[27]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(27);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) DMA2D_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[28] != NULL) {
		(*(s_irq_handler[28]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(28);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) PHY_MBP_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[29] != NULL) {
		(*(s_irq_handler[29]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(29);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) PHY_RIU_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[30] != NULL) {
		(*(s_irq_handler[30]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(30);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) MAC_INT_TX_RX_TIMER_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[31] != NULL) {
		(*(s_irq_handler[31]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(31);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) MAC_INT_TX_RX_MISC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[32] != NULL) {
		(*(s_irq_handler[32]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(32);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) MAC_INT_RX_TRIGGER_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[33] != NULL) {
		(*(s_irq_handler[33]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(33);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) MAC_INT_TX_TRIGGER_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[34] != NULL) {
		(*(s_irq_handler[34]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(34);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) MAC_INT_PORT_TRIGGER_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[35] != NULL) {
		(*(s_irq_handler[35]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(35);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) MAC_INT_GEN_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[36] != NULL) {
		(*(s_irq_handler[36]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(36);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) HSU_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[37] != NULL) {
		(*(s_irq_handler[37]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(37);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) INT_MAC_WAKEUP_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[38] != NULL) {
		(*(s_irq_handler[38]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(38);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) DM_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[39] != NULL) {
		(*(s_irq_handler[39]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(39);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) BLE_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[40] != NULL) {
		(*(s_irq_handler[40]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(40);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) BT_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[41] != NULL) {
		(*(s_irq_handler[41]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(41);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) QSPI1_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[42] != NULL) {
		(*(s_irq_handler[42]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(42);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) PWM1_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[43] != NULL) {
		(*(s_irq_handler[43]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(43);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) I2S1_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[44] != NULL) {
		(*(s_irq_handler[44]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(44);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) I2S2_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[45] != NULL) {
		(*(s_irq_handler[45]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(45);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) H264_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[46] != NULL) {
		(*(s_irq_handler[46]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(46);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) SDMADC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[47] != NULL) {
		(*(s_irq_handler[47]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(47);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) MBOX0_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[48] != NULL) {
		(*(s_irq_handler[48]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(48);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) MBOX1_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[49] != NULL) {
		(*(s_irq_handler[49]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(49);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) BMC64_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[50] != NULL) {
		(*(s_irq_handler[50]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(50);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) DPLL_UNLOCK_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[51] != NULL) {
		(*(s_irq_handler[51]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(51);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) TOUCH_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[52] != NULL) {
		(*(s_irq_handler[52]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(52);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) USB_PLUG_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[53] != NULL) {
		(*(s_irq_handler[53]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(53);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) RTC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[54] != NULL) {
		(*(s_irq_handler[54]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(54);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) GPIO_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[55] != NULL) {
		(*(s_irq_handler[55]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(55);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) DMA1_SEC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[56] != NULL) {
		(*(s_irq_handler[56]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(56);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) DMA1_NSEC_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[57] != NULL) {
		(*(s_irq_handler[57]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(57);

	NESTED_IRQ_EXIT();
}

void __attribute__ ((interrupt)) YUV_BUF_Handler(void)
{
	NESTED_IRQ_ENTER();

	if (s_irq_handler[58] != NULL) {
		(*(s_irq_handler[58]))();
	}

	NESTED_VPLIC_COMPLETE_INTERRUPT(58);

	NESTED_IRQ_EXIT();
}


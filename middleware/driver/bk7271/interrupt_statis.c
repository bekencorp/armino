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

#include <common/bk_include.h>
#include "interrupt_base.h"
#include "icu_driver.h"
#include <driver/int_types.h>

#if CONFIG_INT_STATIS
extern int_statis_t g_int_statis_num;

int_statis_t * bk_get_int_statis(void)
{
	return &g_int_statis_num;
}

bk_err_t bk_dump_int_statis(int_statis_t *int_statistic)
{
	ICU_LOGI("  irq : int_statis:%x\r\n", int_statistic->irq_int_statis);
	ICU_LOGI("  uart1 int_num=%d\n", int_statistic->irq_int_statis[0]);
	ICU_LOGI("  uart2 int_num=%d\n", int_statistic->irq_int_statis[1]);
	ICU_LOGI("  uart3 int_num=%d\n", int_statistic->irq_int_statis[2]);
	ICU_LOGI("  i2c0 int_num=%d\n", int_statistic->irq_int_statis[3]);
	ICU_LOGI("  i2c1 int_num=%d\n", int_statistic->irq_int_statis[4]);
	ICU_LOGI("  i2c2 int_num=%d\n", int_statistic->irq_int_statis[5]);
	ICU_LOGI("  sdio_host int_num=%d\n", int_statistic->irq_int_statis[6]);
	ICU_LOGI("  usb1 int_num=%d\n", int_statistic->irq_int_statis[7]);
	ICU_LOGI("  spi int_num=%d\n", int_statistic->irq_int_statis[8]);
	ICU_LOGI("  spi2 int_num=%d\n", int_statistic->irq_int_statis[9]);
	ICU_LOGI("  spi3 int_num=%d\n", int_statistic->irq_int_statis[10]);
	ICU_LOGI("  gpio int_num=%d\n", int_statistic->irq_int_statis[11]);
	ICU_LOGI("  irda int_num=%d\n", int_statistic->irq_int_statis[12]);
	ICU_LOGI("  timer int_num=%d\n", int_statistic->irq_int_statis[13]);
	ICU_LOGI("  pwm1 int_num=%d\n", int_statistic->irq_int_statis[14]);
	ICU_LOGI("  pwm2 int_num=%d\n", int_statistic->irq_int_statis[15]);
	ICU_LOGI("  saradc int_num=%d\n", int_statistic->irq_int_statis[16]);
	ICU_LOGI("  qspi int_num=%d\n", int_statistic->irq_int_statis[17]);
	ICU_LOGI("  usb2 int_num=%d\n", int_statistic->irq_int_statis[18]);
	ICU_LOGI("  sdio_dma int_num=%d\n", int_statistic->irq_int_statis[19]);
	ICU_LOGI("  general_dma int_num=%d\n", int_statistic->irq_int_statis[20]);
	ICU_LOGI("  mailbox_dsp int_num=%d\n", int_statistic->irq_int_statis[21]);
	ICU_LOGI("  mailbox_bt int_num=%d\n", int_statistic->irq_int_statis[22]);
	ICU_LOGI("  security int_num=%d\n", int_statistic->irq_int_statis[23]);
	ICU_LOGI("  modem int_num=%d\n", int_statistic->irq_int_statis[24]);
	ICU_LOGI("  mac_txrx_timer int_num=%d\n", int_statistic->irq_int_statis[25]);
	ICU_LOGI("  mac_txrx_misc int_num=%d\n", int_statistic->irq_int_statis[26]);
	ICU_LOGI("  mac_rx_triggerint_num=%d\n", int_statistic->irq_int_statis[27]);
	ICU_LOGI("  mac_tx_trigger int_num=%d\n", int_statistic->irq_int_statis[28]);
	ICU_LOGI("  mac_prot_trigger int_num=%d\n", int_statistic->irq_int_statis[29]);
	ICU_LOGI("  mac_general int_num=%d\n", int_statistic->irq_int_statis[30]);
	ICU_LOGI("  mac_wakeup int_num=%d\n", int_statistic->irq_int_statis[31]);

	ICU_LOGI("  fiq : int_statis:%x\r\n", int_statistic->fiq_int_statis);
	ICU_LOGI("  pll_unlock int_num=%d\n", int_statistic->fiq_int_statis[0]);
	ICU_LOGI("  usb_plug int_num=%d\n", int_statistic->fiq_int_statis[1]);
	ICU_LOGI("  bt wdt int_num=%d\n", int_statistic->fiq_int_statis[2]);
	ICU_LOGI("  dsp wdt int_num=%d\n", int_statistic->fiq_int_statis[3]);
	ICU_LOGI("  rtc int_num=%d\n", int_statistic->fiq_int_statis[4]);
	ICU_LOGI("  touch int_num=%d\n", int_statistic->fiq_int_statis[5]);
	ICU_LOGI("  cec int_num=%d\n", int_statistic->fiq_int_statis[6]);

	return BK_OK;
}
#endif


#include <common/bk_include.h>
#include "bk_arm_arch.h"
//#include "co_list.h"

#include "audio.h"
#include "audio_pub.h"

#include <driver/int.h>
#include "bk_icu.h"
#include "bk_sys_ctrl.h"
#include "sys_driver.h"

#include "bk_drv_model.h"
#include <os/mem.h>
#include "ring_buffer.h"

#if (CONFIG_AUDIO)
#if (CONFIG_SOC_BK7251)
void audio_power_up(void)
{
	UINT32 param;
	param = PWD_AUDIO_CLK_BIT;
	sddev_control(DD_DEV_TYPE_ICU, CMD_CLK_PWR_UP, &param);
}

void audio_power_down(void)
{
	UINT32 param;
	param = PWD_AUDIO_CLK_BIT;
	sddev_control(DD_DEV_TYPE_ICU, CMD_CLK_PWR_DOWN, &param);
}

void audio_enable_interrupt(void)
{
	UINT32 param;
	param = (IRQ_AUDIO_BIT);
	//sddev_control(DD_DEV_TYPE_ICU, CMD_ICU_INT_ENABLE, &param);
	(void)sys_drv_int_enable(param);
}

void audio_disable_interrupt(void)
{
	UINT32 param;
	param = (IRQ_AUDIO_BIT);
	// sddev_control(DD_DEV_TYPE_ICU, CMD_ICU_INT_DISABLE, &param);
	(void)sys_drv_int_disable(param);
}

static void audio_isr(void)
{
	UINT32 status = REG_READ(AUD_AD_FIFO_STATUS);

	if (status & (DAC_R_INT_FLAG | DAC_L_INT_FLAG)) {
#if CONFIG_AUDIO_DAC
		audio_dac_isr();
#endif

		REG_WRITE(AUD_AD_FIFO_STATUS,
				  DAC_R_NEAR_FULL | DAC_L_NEAR_FULL
				  | DAC_R_NEAR_EMPTY | DAC_L_NEAR_EMPTY
				  | DAC_R_FIFO_FULL | DAC_L_FIFO_FULL
				  | DAC_R_FIFO_EMPTY | DAC_L_FIFO_EMPTY
				  | DAC_R_INT_FLAG | DAC_L_INT_FLAG);

	}
	if (status & ADC_INT_FLAG) {
#if CONFIG_AUDIO_ADC
		audio_adc_isr();
#endif

		REG_WRITE(AUD_AD_FIFO_STATUS,
				  ADC_NEAR_FULL | ADC_NEAR_EMPTY | ADC_FIFO_FULL
				  | ADC_FIFO_EMPTY | ADC_INT_FLAG);
	}
	if (status & DTMF_INT_FLAG) {
		//audio_dtmf_isr(status);
		REG_WRITE(AUD_AD_FIFO_STATUS,
				  DTMF_NEAR_FULL | DTMF_NEAR_EMPTY | DTMF_FIFO_FULL
				  | DTMF_FIFO_EMPTY | DTMF_INT_FLAG);
	}
}

void audio_hardware_init(void)
{
	UINT32 val;

	/* register interrupt */
	bk_int_isr_register(INT_SRC_AUDIO, audio_isr, NULL);

	REG_WRITE(AUDIO_CONFIG, 0);

	REG_WRITE(AUD_DTMF_CONFIG_0, 0);
	REG_WRITE(AUD_DTMF_CONFIG_1, 0);
	REG_WRITE(AUD_DTMF_CONFIG_2, 0);

	REG_WRITE(AUD_ADC_CONFIG_0, 0x00e93A22);
	REG_WRITE(AUD_ADC_CONFIG_1, 0x8BBF3A22);
	REG_WRITE(AUD_ADC_CONFIG_2, 0xC9E6751C);
	REG_WRITE(AUD_AGC_CONFIG_0, 0x4A019465);
	REG_WRITE(AUD_AGC_CONFIG_1, 0x02016C01);
	REG_WRITE(AUD_AGC_CONFIG_2, 0x0F020940);

	REG_WRITE(AUD_DAC_CONFIG_0, 0);
	REG_WRITE(AUD_DAC_CONFIG_1, 0);
	REG_WRITE(AUD_DAC_CONFIG_2, 0);

	// it's very import to config dac interrupt thred(not all zero)
	REG_WRITE(AUD_FIFO_CONFIG, 0x210);

	/* reset int status */
	val = REG_READ(AUD_AD_FIFO_STATUS);
	REG_WRITE(AUD_AD_FIFO_STATUS, val);

#if(AUD_DAC_USE_PORT_SET == AUD_DAC_SINGLE_PORT)
	audio_dac_volume_use_single_port();
#else
	audio_dac_volume_diff_port();
#endif
}

void audio_init(void)
{
#if CONFIG_AUDIO_DAC
	audio_dac_software_init();
#endif

#if CONFIG_AUDIO_ADC
	audio_adc_software_init();
#endif

	audio_hardware_init();
}


void audio_exit(void)
{
	UINT32 val;

	REG_WRITE(AUDIO_CONFIG, 0);

	REG_WRITE(AUD_DTMF_CONFIG_0, 0);
	REG_WRITE(AUD_DTMF_CONFIG_1, 0);
	REG_WRITE(AUD_DTMF_CONFIG_2, 0);

	REG_WRITE(AUD_ADC_CONFIG_0, 0);
	REG_WRITE(AUD_ADC_CONFIG_1, 0);
	REG_WRITE(AUD_ADC_CONFIG_2, 0);

	REG_WRITE(AUD_DAC_CONFIG_0, 0);
	REG_WRITE(AUD_DAC_CONFIG_1, 0);
	REG_WRITE(AUD_DAC_CONFIG_2, 0);

	REG_WRITE(AUD_FIFO_CONFIG, 0);

	/* reset int status */
	val = REG_READ(AUD_AD_FIFO_STATUS);
	REG_WRITE(AUD_AD_FIFO_STATUS, val);

#if CONFIG_AUDIO_DAC
	ddev_unregister_dev(DD_DEV_TYPE_AUD_DAC);
#endif
}
#elif (CONFIG_SOC_BK7271)
void audio_init(void)
{
	uint32_t ret;
	uint32_t param;

	param = AUDIO_DAC_VOL_DIFF_MODE;
	ret = sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_SET_VOLUME_PORT, &param);
	if (ret) {
		AUDIO_LOGE("set volume fail.\r\n");
		return;
	}

	ret = sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_OPEN_DAC_ANALOG, NULL);
	if (ret) {
		AUDIO_LOGE("open dac analog fail.\r\n");
		return;
	}

	ret = sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_OPEN_ADC_MIC_ANALOG, NULL);
	if (ret) {
		AUDIO_LOGE("open adc mic analog fail.\r\n");
		return;
	}

	param = 16000;
	ret = sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_AUDIO_PLL, &param);
	if (ret) {
		AUDIO_LOGE("set audio PLL fail.\r\n");
		return;
	}
}

void audio_exit(void)
{
	uint32_t ret;

	ret = sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_CLOSE_DAC_ANALOG, NULL);
	if (ret) {
		AUDIO_LOGE("close dac analog fail.\r\n");
		return;
	}

	ret = sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_CLOSE_ADC_MIC_ANALOG, NULL);
	if (ret) {
		AUDIO_LOGE("open adc mic analog fail.\r\n");
		return;
	}
}
#endif
#endif // CONFIG_AUDIO


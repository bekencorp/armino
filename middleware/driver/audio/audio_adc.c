#include <common/bk_include.h>
#include "bk_arm_arch.h"
//#include "co_list.h"

#include "audio.h"
#include "audio_pub.h"

#include "bk_intc.h"
#include "bk_icu.h"
#include "bk_sys_ctrl.h"

#include <os/mem.h>
#include "ring_buffer.h"
#include "ring_buffer_dma_write.h"
#include "bk_gpio.h"

#if CONFIG_GENERAL_DMA
#include <driver/dma.h>
#include "bk_general_dma.h"
#define AUD_ADC_DEF_DMA_CHANNEL     DMA_ID_2
#endif

#if ((CONFIG_AUDIO_ADC) && (CONFIG_SOC_BK7251))
enum {
	AUD_ADC_STA_CLOSED   = 0,
	AUD_ADC_STA_OPENED,
	AUD_ADC_STA_PAUSE,
	AUD_ADC_STA_PLAYING
};

typedef struct aud_adc_desc {
	UINT8 *buf;
	UINT16 buf_len;
	UINT16 freq;
	UINT16 channels;
	UINT16 mode;
	UINT32 linein_detect_pin;

	UINT32 status;
	union {
		RB_ST rb;
		RB_DMA_WR_ST rb_dma_wr;
	} u;
} AUD_ADC_DESC_ST, *AUD_ADC_DESC_PTR;

static UINT32 audio_adc_open(UINT32 op_flag);
static UINT32 audio_adc_close(void);
static UINT32 audio_adc_read(char *user_buf, UINT32 count, UINT32 op_flag);
static UINT32 audio_adc_ctrl(UINT32 cmd, void *param);

const DD_OPERATIONS adu_adc_op = {
	audio_adc_open,
	audio_adc_close,
	audio_adc_read,
	NULL,
	audio_adc_ctrl
};
static AUD_ADC_DESC_ST aud_adc = {0};

static void audio_adc_set_enable_bit(UINT32 enable)
{
	UINT32 reg_addr = AUDIO_CONFIG;
	UINT32 reg_val = REG_READ(reg_addr);

	// linein and adc enable bits are the two channels for adc
	// open both of them, no mater channel is.
	if (enable)
		reg_val |= (ADC_ENABLE | LINEIN_ENABLE);
	else
		reg_val &= ~(ADC_ENABLE | LINEIN_ENABLE);
	REG_WRITE(reg_addr, reg_val);
}

static void audio_adc_set_int_enable_bit(UINT32 enable)
{
	UINT32 reg_addr = AUD_FIFO_CONFIG;
	UINT32 reg_val = REG_READ(reg_addr);

	if (enable)
		reg_val |= ADC_INT_EN;
	else
		reg_val &= ~ADC_INT_EN;
	REG_WRITE(reg_addr, reg_val);
}

static void audio_adc_get_l_sample(INT16 *left)
{
	UINT32 reg_addr = AUD_ADC_FIFO_PORT;
	UINT32 reg_val = REG_READ(reg_addr);

	*left = (INT16)(reg_val & AD_ADC_L_FIFO_MASK);
}

static void audio_adc_get_l_and_r_samples(INT16 *left, INT16 *right)
{
	UINT32 reg_addr = AUD_ADC_FIFO_PORT;
	UINT32 reg_val = REG_READ(reg_addr);

	reg_val &= AD_ADC_LR_FIFO_MASK;

	*left = (INT16)(reg_val & AD_ADC_L_FIFO_MASK);
	*right = (INT16)((reg_val >> AD_ADC_R_FIFO_POSI) & AD_ADC_R_FIFO_MASK);
}

static void audio_adc_set_hpf2_bypass_bit(UINT32 enable)
{
	UINT32 reg_addr = AUD_ADC_CONFIG_0;
	UINT32 reg_val = REG_READ(reg_addr);

	if (enable)
		reg_val |= ADC_HPF2_BYPASS;
	else
		reg_val &= ~ADC_HPF2_BYPASS;
	REG_WRITE(reg_addr, reg_val);
}

static void audio_adc_set_gain(UINT32 gain)
{
	UINT32 reg_addr = AUD_ADC_CONFIG_0;
	UINT32 reg_val = REG_READ(reg_addr);

	if (gain > ADC_SET_GAIN_MASK)
		gain = ADC_SET_GAIN_MASK;

	reg_val &= ~(ADC_SET_GAIN_MASK << ADC_SET_GAIN_POSI);
	reg_val |= ((gain & ADC_SET_GAIN_MASK)  << ADC_SET_GAIN_POSI);

	REG_WRITE(reg_addr, reg_val);
}

static void audio_adc_set_write_thred_bit(UINT32 thred)
{
	UINT32 reg_addr = AUD_FIFO_CONFIG;
	UINT32 reg_val = REG_READ(reg_addr);

	if (thred > ADC_WR_THRED_MASK)
		thred = ADC_WR_THRED_MASK;

	reg_val &= ~(ADC_WR_THRED_MASK << ADC_WR_THRED_POSI);
	reg_val &= ~(DTMF_WR_THRED_MASK << DTMF_WR_THRED_POSI);
	reg_val |= ((thred & ADC_WR_THRED_MASK) << ADC_WR_THRED_POSI);
	reg_val |= ((thred & DTMF_WR_THRED_MASK) << DTMF_WR_THRED_POSI);

	REG_WRITE(reg_addr, reg_val);
}

static void audio_adc_set_sample_rate(UINT32 sample_rate)
{
	UINT32 reg;

	/* disable adc handset bit again, to make sure this bit unset */
	reg = REG_READ(AUD_EXTEND_CFG);
	reg &= ~(ADC_FRACMOD_MANUAL);
	REG_WRITE(AUD_EXTEND_CFG, reg);

	switch (sample_rate) {
	case 11025:
		reg = REG_READ(AUD_EXTEND_CFG);
		reg |= ADC_FRACMOD_MANUAL;
		REG_WRITE(AUD_EXTEND_CFG, reg);
		reg = (CONST_DIV_441K);
		REG_WRITE(AUD_ADC_FRACMOD, reg);
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_ADC_MASK << SAMPLE_RATE_ADC_POSI);
		reg |= ((SAMPLE_RATE_44_1_K & SAMPLE_RATE_ADC_MASK) << SAMPLE_RATE_ADC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 22050: //
		reg = REG_READ(AUD_EXTEND_CFG);
		reg |= ADC_FRACMOD_MANUAL;
		REG_WRITE(AUD_EXTEND_CFG, reg);
		reg = (CONST_DIV_441K >> 1);
		REG_WRITE(AUD_ADC_FRACMOD, reg);
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_ADC_MASK << SAMPLE_RATE_ADC_POSI);
		reg |= ((SAMPLE_RATE_44_1_K & SAMPLE_RATE_ADC_MASK) << SAMPLE_RATE_ADC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 44100:
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_ADC_MASK << SAMPLE_RATE_ADC_POSI);
		reg |= ((SAMPLE_RATE_44_1_K & SAMPLE_RATE_ADC_MASK) << SAMPLE_RATE_ADC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 12000:
		reg = REG_READ(AUD_EXTEND_CFG);
		reg |= ADC_FRACMOD_MANUAL;
		REG_WRITE(AUD_EXTEND_CFG, reg);
		reg = (CONST_DIV_48K);
		REG_WRITE(AUD_ADC_FRACMOD, reg);
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_ADC_MASK << SAMPLE_RATE_ADC_POSI);
		reg |= ((SAMPLE_RATE_48K & SAMPLE_RATE_ADC_MASK) << SAMPLE_RATE_ADC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 24000:
		reg = REG_READ(AUD_EXTEND_CFG);
		reg |= ADC_FRACMOD_MANUAL;
		REG_WRITE(AUD_EXTEND_CFG, reg);
		reg = (CONST_DIV_48K >> 1);
		REG_WRITE(AUD_ADC_FRACMOD, reg);
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_ADC_MASK << SAMPLE_RATE_ADC_POSI);
		reg |= ((SAMPLE_RATE_48K & SAMPLE_RATE_ADC_MASK) << SAMPLE_RATE_ADC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 48000:
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_ADC_MASK << SAMPLE_RATE_ADC_POSI);
		reg |= ((SAMPLE_RATE_48K & SAMPLE_RATE_ADC_MASK) << SAMPLE_RATE_ADC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 8000:
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_ADC_MASK << SAMPLE_RATE_ADC_POSI);
		reg |= ((SAMPLE_RATE_8K & SAMPLE_RATE_ADC_MASK) << SAMPLE_RATE_ADC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 16000:
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_ADC_MASK << SAMPLE_RATE_ADC_POSI);
		reg |= ((SAMPLE_RATE_16K & SAMPLE_RATE_ADC_MASK) << SAMPLE_RATE_ADC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 32000:
		reg = REG_READ(AUD_EXTEND_CFG);
		reg |= ADC_FRACMOD_MANUAL;
		REG_WRITE(AUD_EXTEND_CFG, reg);
		reg = (CONST_DIV_32K);
		REG_WRITE(AUD_ADC_FRACMOD, reg);
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_ADC_MASK << SAMPLE_RATE_ADC_POSI);
		reg |= ((SAMPLE_RATE_48K & SAMPLE_RATE_ADC_MASK) << SAMPLE_RATE_ADC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	default:
		AUDIO_LOGE("unsupported sample rate:%d\r\n", sample_rate);
		break;
	}
}

#if CONFIG_GENERAL_DMA
void audio_adc_dma_handler(dma_id_t param)
{
	//AUDIO_LOGI("audio_dac_dma_handler:%d\r\n", param);
}

static void audio_adc_config_dma(void)
{
	dma_config_t dma_config;

	os_memset(&dma_config, 0, sizeof(dma_config_t));

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;

	dma_config.src.dev = DMA_DEV_AUDIO;
	if (aud_adc.channels == 1)
		dma_config.src.width = DMA_DATA_WIDTH_16BITS;
	else
		dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = AUD_ADC_FIFO_PORT;

	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.start_addr = (uint32_t)aud_adc.buf;
	dma_config.dst.end_addr = (uint32_t)(aud_adc.buf + aud_adc.buf_len);

	bk_dma_init(AUD_ADC_DEF_DMA_CHANNEL, &dma_config);
	bk_dma_set_transfer_len(AUD_ADC_DEF_DMA_CHANNEL, aud_adc.buf_len);
	dma_set_dst_pause_addr(AUD_ADC_DEF_DMA_CHANNEL, (uint32_t)aud_adc.buf);
	bk_dma_register_isr(AUD_ADC_DEF_DMA_CHANNEL, NULL, audio_adc_dma_handler);
	bk_dma_enable_finish_interrupt(AUD_ADC_DEF_DMA_CHANNEL);
	bk_dma_enable_half_finish_interrupt(AUD_ADC_DEF_DMA_CHANNEL);
}

static void audio_adc_set_dma(UINT32 enable)
{
	if (enable)
		bk_dma_start(AUD_ADC_DEF_DMA_CHANNEL);
	else
		bk_dma_stop(AUD_ADC_DEF_DMA_CHANNEL);
}

static void audio_adc_eixt_dma(void)
{
	bk_dma_stop(AUD_ADC_DEF_DMA_CHANNEL);
}
#endif // CONFIG_GENERAL_DMA

static void audio_adc_open_analog_regs(void)
{
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_OPEN_ADC_MIC_ANALOG, NULL);
}

static void audio_adc_close_analog_regs(void)
{
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_CLOSE_ADC_MIC_ANALOG, NULL);
}

static void audio_adc_enable_linein(void)
{
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_ENALBLE_ADC_LINE_IN, NULL);
}

static void audio_adc_disable_linein(void)
{
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_DISALBLE_ADC_LINE_IN, NULL);
}

static void audio_adc_init_linein_detect_pin(UINT32 pin)
{
	UINT32 param;

	BK_ASSERT(pin < GPIONUM);

	param = GPIO_CFG_PARAM(pin, GMODE_INPUT_PULLUP);
	sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_CFG, &param);
}

static void audio_adc_linein_detect(void)
{
	UINT32 param;

	if (aud_adc.status != AUD_ADC_STA_CLOSED) {
		UINT32 gpio_val;

		param = aud_adc.linein_detect_pin;
		gpio_val = sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_INPUT, &param);
		//AUDIO_LOGI("%d\r\n", gpio_val);
		if ((gpio_val == AUD_ADC_LINEIN_ENABLE_LEVEL)
			&& (!(aud_adc.mode & AUD_ADC_MODE_LINEIN))) {
			audio_adc_enable_linein();
			aud_adc.mode |= AUD_ADC_MODE_LINEIN;
			AUDIO_LOGI("enable line in: %d\r\n", aud_adc.mode);
		} else if ((gpio_val != AUD_ADC_LINEIN_ENABLE_LEVEL)
				   && (aud_adc.mode & AUD_ADC_MODE_LINEIN)) {
			audio_adc_disable_linein();
			aud_adc.mode &= ~AUD_ADC_MODE_LINEIN;
			AUDIO_LOGI("disable line in: %d\r\n", aud_adc.mode);
		}
	}
}

static UINT32 audio_adc_open(UINT32 op_flag)
{
	AUD_ADC_DESC_PTR cfg;

	if (!op_flag) {
		AUDIO_LOGE("audio_adc_open is NULL\r\n");
		return AUD_FAILURE;
	}

	cfg = (AUD_ADC_DESC_PTR)op_flag;

#if(!CONFIG_GENERAL_DMA)
	if (cfg->mode & AUD_ADC_MODE_DMA_BIT) {
		AUDIO_LOGE("audio_dac_open no support dma\r\n");
		return AUD_FAILURE;
	}
#endif // !CONFIG_GENERAL_DMA

	aud_adc.status = AUD_ADC_STA_CLOSED;
	aud_adc.buf = cfg->buf;
	aud_adc.buf_len = cfg->buf_len;
	aud_adc.channels = cfg->channels;
	aud_adc.freq = cfg->freq;
	aud_adc.mode = cfg->mode;
	aud_adc.linein_detect_pin = cfg->linein_detect_pin;

	audio_adc_set_enable_bit(0);
	audio_adc_set_sample_rate(aud_adc.freq);
	audio_adc_set_hpf2_bypass_bit(1);
	audio_adc_set_gain(AUD_ADC_DEF_GAIN);
	audio_adc_init_linein_detect_pin(aud_adc.linein_detect_pin);

#if !AUD_ADC_DAC_HARDWARD_LOOPBACK
	if (!(aud_adc.mode & AUD_ADC_MODE_DMA_BIT)) {
		audio_adc_set_write_thred_bit(AUD_ADC_DEF_WR_THRED);
		audio_adc_set_int_enable_bit(1);
		audio_enable_interrupt();

		rb_init(&aud_adc.u.rb, aud_adc.buf, aud_adc.buf_len);
	} else {
		audio_adc_set_int_enable_bit(0);
		//audio_disable_interrupt();

#if (CONFIG_GENERAL_DMA)
		audio_adc_config_dma();

		rb_init_dma_write(&aud_adc.u.rb_dma_wr, aud_adc.buf, aud_adc.buf_len, AUD_ADC_DEF_DMA_CHANNEL);
#endif
	}
#else
	REG_WRITE(AUD_FIFO_CONFIG, (REG_READ(AUD_FIFO_CONFIG) | LOOP_ADC2DAC));
#endif

	audio_adc_open_analog_regs();
	audio_power_up();
#if AUD_ADC_DAC_HARDWARD_LOOPBACK
	audio_adc_set_enable_bit(1);
	aud_adc.status = AUD_ADC_STA_PLAYING;
#else
	aud_adc.status = AUD_ADC_STA_OPENED;
#endif

	return AUD_SUCCESS;
}

static UINT32 audio_adc_close(void)
{
	audio_adc_set_enable_bit(0);
	audio_adc_set_int_enable_bit(0);

	if (aud_adc.status == AUD_ADC_STA_CLOSED)
		return AUD_SUCCESS;

#if (CONFIG_GENERAL_DMA)
	if (aud_adc.mode & AUD_ADC_MODE_DMA_BIT) {
		audio_adc_eixt_dma();
		//rb_clear_dma_read(&aud_dac.u.rb_dma_rd);
	}
#endif

	//audio_disable_interrupt();
	//audio_power_down();
	audio_adc_close_analog_regs();

	os_memset(&aud_adc, 0, sizeof(AUD_ADC_DESC_ST));
	aud_adc.status = AUD_ADC_STA_CLOSED;

	return AUD_SUCCESS;
}


static UINT32 audio_adc_read(char *user_buf, UINT32 count, UINT32 op_flag)
{
	int fill_size;

	if (aud_adc.status != AUD_ADC_STA_PLAYING)
		return 0;
	if (aud_adc.mode & AUD_ADC_MODE_DMA_BIT) {
#if CONFIG_GENERAL_DMA
		RB_DMA_WR_PTR rb;
		rb = &aud_adc.u.rb_dma_wr;
		fill_size = rb_get_fill_size_dma_write(rb);
		if (fill_size > count)
			fill_size = count;
		rb_read_dma_write(rb, (UINT8 *)user_buf, fill_size, 1);
#endif
	} else {
		RB_PTR rb;

		rb = &aud_adc.u.rb;
		fill_size = rb_get_fill_size(rb);
		if (fill_size > count)
			fill_size = count;

		rb_read(rb, (UINT8 *)user_buf, fill_size, 1);
	}

	return fill_size;
}

static UINT32 audio_adc_get_fill_buf_size(void)
{
	int free_size;

	if (aud_adc.status != AUD_ADC_STA_PLAYING)
		return 0;

	if (aud_adc.mode & AUD_ADC_MODE_DMA_BIT) {
#if CONFIG_GENERAL_DMA
		RB_DMA_WR_PTR rb;
		rb = &aud_adc.u.rb_dma_wr;
		free_size = rb_get_fill_size_dma_write(rb);
#endif
	} else {
		RB_PTR rb;
		rb = &aud_adc.u.rb;
		free_size = rb_get_fill_size(rb);
	}

	return free_size;
}

static void audio_adc_pause(void)
{
	if (aud_adc.status == AUD_ADC_STA_PAUSE)
		return;

	if (aud_adc.mode & AUD_ADC_MODE_DMA_BIT) {
#if CONFIG_GENERAL_DMA
		audio_adc_set_dma(0);
#endif
	}

	audio_adc_set_enable_bit(0);

	aud_adc.status = AUD_ADC_STA_PAUSE;
}

static void audio_adc_play(void)
{
	if (aud_adc.status == AUD_ADC_STA_PLAYING)
		return;

	if (aud_adc.mode & AUD_ADC_MODE_DMA_BIT) {
#if CONFIG_GENERAL_DMA
		audio_adc_set_dma(1);
#endif
	}

	audio_adc_set_enable_bit(1);

	aud_adc.status = AUD_ADC_STA_PLAYING;
}

static void audio_adc_set_volume(UINT32 volume)
{
	UINT32 act_vol;
	UINT8 high, low;
	UINT32 reg_addr = AUD_AGC_CONFIG_2;
	UINT32 reg_val = REG_READ(reg_addr);

	if (volume > AUD_ADC_MAX_VOLUME)
		volume = AUD_ADC_MAX_VOLUME;

	high = volume & 0x70;
	low = volume & 0x0f;

	if (low > 0xc)
		low = 0xc;

	act_vol = high | low;

	reg_val &= ~(MANUAL_PGA_VAL_MASK << MANUAL_PGA_VAL_POSI);
	reg_val |= ((act_vol & MANUAL_PGA_VAL_MASK) << MANUAL_PGA_VAL_POSI);

	REG_WRITE(reg_addr, reg_val);

	AUDIO_LOGI("set adc vol: %d - %d\r\n", volume, act_vol);
}

static UINT32 audio_adc_ctrl(UINT32 cmd, void *param)
{
	UINT32 ret = AUD_SUCCESS;

	switch (cmd) {
	case AUD_ADC_CMD_GET_FILL_BUF_SIZE:
		ret = audio_adc_get_fill_buf_size();
		break;

	case AUD_ADC_CMD_PLAY:
		audio_adc_play();
		break;

	case AUD_ADC_CMD_PAUSE:
		audio_adc_pause();
		break;

	case AUD_ADC_CMD_DO_LINEIN_DETECT:
		audio_adc_linein_detect();
		break;

	case AUD_ADC_CMD_SET_SAMPLE_RATE:
		BK_ASSERT(param);
		audio_adc_set_enable_bit(0);
		audio_adc_set_sample_rate(*((UINT32 *)param));
		audio_adc_set_enable_bit(1);
		break;

	case AUD_ADC_CMD_SET_VOLUME:
		BK_ASSERT(param);
		audio_adc_set_volume(*((UINT32 *)param));
		break;

	default:
		break;
	}
	return ret;
}

void audio_adc_software_init(void)
{
	ddev_register_dev(DD_DEV_TYPE_AUD_DAC, (DD_OPERATIONS *)&adu_adc_op);

	os_memset(&aud_adc, 0, sizeof(AUD_ADC_DESC_ST));
	aud_adc.status = AUD_ADC_STA_CLOSED;
}

void audio_adc_isr(void)
{
	UINT32 status = REG_READ(AUD_AD_FIFO_STATUS);
	RB_PTR rb = NULL;
	UINT32 ret = 0, channel;
	INT16 sample[2] = {0};

	if (aud_adc.status == AUD_ADC_STA_PLAYING && (!(aud_adc.mode & AUD_ADC_MODE_DMA_BIT))) {
		rb = &aud_adc.u.rb;
		channel = aud_adc.channels;
		if (status & ADC_INT_FLAG) {
			if (channel == 2) {
				while (!(status & (ADC_FIFO_EMPTY))) {
					audio_adc_get_l_and_r_samples(&sample[0], &sample[1]);
					ret = rb_write(rb, (UINT8 *)&sample[0], sizeof(int16), 2);
					//AUDIO_LOGI("%d\r\n", ret);
					if (!ret)
						break;
					status = REG_READ(AUD_AD_FIFO_STATUS);
				}
			} else {
				while (!(status & (ADC_FIFO_EMPTY))) {
					audio_adc_get_l_sample(&sample[0]);
					ret = rb_write(rb, (UINT8 *)&sample[0], sizeof(int16), 1);
					//AUDIO_LOGI("%d\r\n", ret);
					if (!ret)
						break;
					status = REG_READ(AUD_AD_FIFO_STATUS);
				}
			}
		}
	}

	if (ret == 0) {
		while (!(status & (ADC_FIFO_EMPTY))) {
			audio_adc_get_l_and_r_samples(&sample[0], &sample[1]);
			status = REG_READ(AUD_AD_FIFO_STATUS);
		}
		//AUDIO_LOGI("set\r\n");
	}

	//audio_adc_linein_detect();
}

#endif // CONFIG_AUDIO_ADC


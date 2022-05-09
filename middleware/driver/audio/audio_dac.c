#include <common/bk_include.h>
#include "bk_arm_arch.h"
//#include "co_list.h"
#include "audio.h"
#include "audio_pub.h"
#include "bk_intc.h"
#include "bk_icu.h"
#include "bk_sys_ctrl.h"
#include "bk_gpio.h"
#include <os/mem.h>
#include "ring_buffer.h"
#include "ring_buffer_dma_read.h"
#include "audio.h"

#if CONFIG_GENERAL_DMA
#include <driver/dma.h>
#define AUD_DAC_DEF_DMA_CHANNEL     DMA_ID_1
#endif

#if ((CONFIG_AUDIO_DAC) && (CONFIG_SOC_BK7251))
enum {
	AUD_DAC_STA_CLOSED   = 0,
	AUD_DAC_STA_OPENED,
	AUD_DAC_STA_PAUSE,
	AUD_DAC_STA_PLAYING
};

#define AUD_DAC_ELEM_NUM            (1)
#define AUD_DAC_VOL_TABLE_LEN       (17)
#define AUD_DAC_DEF_VOL             (AUD_DAC_VOL_TABLE_LEN/2)
#define AUD_DAC_DEF_RD_THRED        (0x10)
#define AUD_DAC_DEF_DIGTAL_GAIN     (0x2D)  // 0dm

typedef struct aud_volome_s {
	UINT8 ana_gain;
	UINT8 dig_gain;
} AUD_VOL_ST, *AUD_VOL_PTR;

typedef struct aud_dac_desc {
	UINT8 *buf;
	UINT16 buf_len;
	UINT16 freq;
	UINT16 channels;
	UINT16 dma_mode;
	UINT32 status;
	UINT16 mute_pin;
	UINT16 volume;
#if (CONFIG_GENERAL_DMA)
	UINT32 need_write_len;
	void (*need_write_callback)(UINT8 *buf, void *usr_data);
	void *usr_data;
#endif
	union {
		RB_ST rb;
		RB_DMA_RD_ST rb_dma_rd;
	} u;
} AUD_DAC_DESC_ST, *AUD_DAC_DESC_PTR;

static UINT32 audio_dac_open(UINT32 op_flag);
static UINT32 audio_dac_close(void);
static UINT32 audio_dac_write(char *user_buf, UINT32 count, UINT32 op_flag);
static UINT32 audio_dac_ctrl(UINT32 cmd, void *param);

const DD_OPERATIONS aud_dac_op = {
	audio_dac_open,
	audio_dac_close,
	NULL,
	audio_dac_write,
	audio_dac_ctrl
};
static AUD_DAC_DESC_ST aud_dac = {0};

#if (AUD_USE_EXT_PA == 1)
static const AUD_VOL_ST aud_vol_table[AUD_DAC_VOL_TABLE_LEN] = {
	{0x00, 44},           // 0
	{0x01, 45},           // 1
	{0x03, 45},           // 1+
	{0x05, 45},           // 2
	{0x07, 45},           // 3
	{0x09, 45},           // 4
	{0x0b, 45},           // 5
	{0x0d, 45},           // 6
	{0x0f, 45},           // 7
	{0x11, 45},           // 8
	{0x13, 45},           // 9
	{0x15, 45},           // A
	{0x17, 45},           // B
	{0x19, 45},           // C
	{0x1B, 45},           // D
	{0x1D, 45},           // E
	{0x1F, 45},           // F
};
#else
static const AUD_VOL_ST aud_vol_table[AUD_DAC_VOL_TABLE_LEN] = {
	{0x1A, 0},           // 0
	{0x1A, 2},           // 1
	{0x1A, 4},           // 1+
	{0x1A, 6},           // 2
	{0x1A, 9},           // 3
	{0x1A, 12},           // 4
	{0x1A, 15},           // 5
	{0x1A, 18},           // 6
	{0x1A, 21},           // 7
	{0x1A, 24},           // 8
	{0x1A, 27},           // 9
	{0x1A, 30},           // A
	{0x1A, 33},           // B
	{0x1A, 36},           // C
	{0x1A, 39},           // D
	{0x1A, 42},           // E
	{0x1A, 45},           // F
};
#endif

static void audio_dac_set_enable_bit(UINT32 enable)
{
	UINT32 reg_addr = AUDIO_CONFIG;
	UINT32 reg_val = REG_READ(reg_addr);

	if (enable)
		reg_val |= DAC_ENABLE;
	else
		reg_val &= ~DAC_ENABLE;
	REG_WRITE(reg_addr, reg_val);
}

#if AUD_USE_EXT_PA
static UINT32 audio_dac_is_enable_bit(void)
{
	UINT32 reg_addr = AUDIO_CONFIG;
	UINT32 reg_val = REG_READ(reg_addr);

	reg_val = (reg_val & DAC_ENABLE);

	return reg_val ? 1 : 0;
}
#endif

static void audio_dac_set_int_enable_bit(UINT32 enable)
{
	UINT32 reg_addr = AUD_FIFO_CONFIG;
	UINT32 reg_val = REG_READ(reg_addr);

	if (enable)
		reg_val |= (DAC_R_INT_EN | DAC_L_INT_EN);
	else
		reg_val &= ~(DAC_R_INT_EN | DAC_L_INT_EN);
	REG_WRITE(reg_addr, reg_val);
}

static void audio_dac_set_read_thred_bit(UINT32 thred)
{
	UINT32 reg_addr = AUD_FIFO_CONFIG;
	UINT32 reg_val = REG_READ(reg_addr);

	reg_val &= ~(DAC_R_RD_THRED_MASK << DAC_R_RD_THRED_POSI);
	reg_val &= ~(DAC_L_RD_THRED_MASK << DAC_L_RD_THRED_POSI);

	reg_val |= ((thred & DAC_R_RD_THRED_MASK) << DAC_R_RD_THRED_POSI);
	reg_val |= ((thred & DAC_L_RD_THRED_MASK) << DAC_L_RD_THRED_POSI);

	REG_WRITE(reg_addr, reg_val);
}

static void audio_dac_set_hpf1_bit(UINT32 enable)
{
	UINT32 reg_addr = AUD_DAC_CONFIG_0;
	UINT32 reg_val = REG_READ(reg_addr);

	if (enable)
		reg_val |= DAC_HPF1_BYPASS;
	else
		reg_val &= ~DAC_HPF1_BYPASS;
	REG_WRITE(reg_addr, reg_val);
}

static void audio_dac_set_hpf2_bit(UINT32 enable)
{
	UINT32 reg_addr = AUD_DAC_CONFIG_0;
	UINT32 reg_val = REG_READ(reg_addr);

	if (enable)
		reg_val |= DAC_HPF2_BYPASS;
	else
		reg_val &= ~DAC_HPF2_BYPASS;
	REG_WRITE(reg_addr, reg_val);
}

static void audio_dac_set_gain(UINT32 gain)
{
	UINT32 reg_addr = AUD_DAC_CONFIG_0;
	UINT32 reg_val = REG_READ(reg_addr);

	if (gain > DAC_SET_GAIN_MASK)
		gain = DAC_SET_GAIN_MASK;

	reg_val &= ~(DAC_SET_GAIN_MASK << DAC_SET_GAIN_POSI);
	reg_val |= ((gain & DAC_SET_GAIN_MASK)  << DAC_SET_GAIN_POSI);

	REG_WRITE(reg_addr, reg_val);
}

static void audio_dac_set_sample(INT16 left, INT16 right)
{
	UINT32 reg_addr = AUD_DAC_FIFO_PORT;
	UINT32 reg_val;

	reg_val = ((left & AD_DAC_L_FIFO_MASK) << AD_DAC_L_FIFO_POSI)
			  | ((right & AD_DAC_R_FIFO_MASK) << AD_DAC_R_FIFO_POSI);

	REG_WRITE(reg_addr, reg_val);
}

static void audio_dac_set_sample_rate(UINT32 sample_rate)
{
	UINT32 reg;

	/* disable dac handset bit again, to make sure this bit unset */
	reg = REG_READ(AUD_EXTEND_CFG);
	reg &= ~(DAC_FRACMOD_MANUAL);
	REG_WRITE(AUD_EXTEND_CFG, reg);

	switch (sample_rate) {
	case 11025:
		reg = REG_READ(AUD_EXTEND_CFG);
		reg |= DAC_FRACMOD_MANUAL;
		REG_WRITE(AUD_EXTEND_CFG, reg);
		reg = (CONST_DIV_441K << 2);
		REG_WRITE(AUD_DAC_FRACMOD, reg);
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_DAC_MASK << SAMPLE_RATE_DAC_POSI);
		reg |= ((SAMPLE_RATE_44_1_K & SAMPLE_RATE_DAC_MASK) << SAMPLE_RATE_DAC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 22050:
		reg = REG_READ(AUD_EXTEND_CFG);
		reg |= DAC_FRACMOD_MANUAL;
		REG_WRITE(AUD_EXTEND_CFG, reg);
		reg = (CONST_DIV_441K << 1);
		REG_WRITE(AUD_DAC_FRACMOD, reg);
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_DAC_MASK << SAMPLE_RATE_DAC_POSI);
		reg |= ((SAMPLE_RATE_44_1_K & SAMPLE_RATE_DAC_MASK) << SAMPLE_RATE_DAC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 44100:
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_DAC_MASK << SAMPLE_RATE_DAC_POSI);
		reg |= ((SAMPLE_RATE_44_1_K & SAMPLE_RATE_DAC_MASK) << SAMPLE_RATE_DAC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 12000:
		reg = REG_READ(AUD_EXTEND_CFG);
		reg |= DAC_FRACMOD_MANUAL;
		REG_WRITE(AUD_EXTEND_CFG, reg);
		reg = (CONST_DIV_48K << 2);
		REG_WRITE(AUD_DAC_FRACMOD, reg);
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_DAC_MASK << SAMPLE_RATE_DAC_POSI);
		reg |= ((SAMPLE_RATE_48K & SAMPLE_RATE_DAC_MASK) << SAMPLE_RATE_DAC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 24000:
		reg = REG_READ(AUD_EXTEND_CFG);
		reg |= DAC_FRACMOD_MANUAL;
		REG_WRITE(AUD_EXTEND_CFG, reg);
		reg = (CONST_DIV_48K << 1);
		REG_WRITE(AUD_DAC_FRACMOD, reg);
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_DAC_MASK << SAMPLE_RATE_DAC_POSI);
		reg |= ((SAMPLE_RATE_48K & SAMPLE_RATE_DAC_MASK) << SAMPLE_RATE_DAC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 48000:
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_DAC_MASK << SAMPLE_RATE_DAC_POSI);
		reg |= ((SAMPLE_RATE_48K & SAMPLE_RATE_DAC_MASK) << SAMPLE_RATE_DAC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 8000:
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_DAC_MASK << SAMPLE_RATE_DAC_POSI);
		reg |= ((SAMPLE_RATE_8K & SAMPLE_RATE_DAC_MASK) << SAMPLE_RATE_DAC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 16000:
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_DAC_MASK << SAMPLE_RATE_DAC_POSI);
		reg |= ((SAMPLE_RATE_16K & SAMPLE_RATE_DAC_MASK) << SAMPLE_RATE_DAC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	case 32000:
		reg = REG_READ(AUD_EXTEND_CFG);
		reg |= DAC_FRACMOD_MANUAL;
		REG_WRITE(AUD_EXTEND_CFG, reg);
		reg = (CONST_DIV_16K >> 1);
		REG_WRITE(AUD_DAC_FRACMOD, reg);
		reg = REG_READ(AUDIO_CONFIG);
		reg &= ~(SAMPLE_RATE_DAC_MASK << SAMPLE_RATE_DAC_POSI);
		reg |= ((SAMPLE_RATE_16K & SAMPLE_RATE_DAC_MASK) << SAMPLE_RATE_DAC_POSI);
		REG_WRITE(AUDIO_CONFIG, reg);
		break;

	default:
		AUDIO_LOGE("unsupported sample rate:%d\r\n", sample_rate);
		break;
	}
}

#if CONFIG_GENERAL_DMA
void audio_dac_dma_handler(dma_id_t param)
{
	//AUDIO_LOGI("audio_dac_dma_handler:%d\r\n", param);
	RB_DMA_RD_PTR rb;
	int free_size;

	rb = &aud_dac.u.rb_dma_rd;
	free_size = rb_get_free_size_dma_read(rb);
	//AUDIO_LOGI("%d-%d\r\n",free_size, aud_dac.need_write_len);
	if (free_size >= aud_dac.need_write_len) {
		if (aud_dac.need_write_callback)
			aud_dac.need_write_callback((void *)free_size, aud_dac.usr_data);
	}
}

static void audio_dac_config_dma(void)
{
	if (!aud_dac.buf)
		return;

	dma_config_t dma_config;

	os_memset(&dma_config, 0, sizeof(dma_config_t));

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;

	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.src.start_addr = (uint32_t)aud_dac.buf;
	dma_config.src.end_addr = (uint32_t)(aud_dac.buf + aud_dac.buf_len);

	dma_config.dst.dev = DMA_DEV_AUDIO;
	if (aud_dac.channels == 1)
		dma_config.dst.width = DMA_DATA_WIDTH_16BITS;
	else
		dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.start_addr = AUD_DAC_FIFO_PORT;

	bk_dma_init(AUD_DAC_DEF_DMA_CHANNEL, &dma_config);

	aud_dac.need_write_len = aud_dac.buf_len / 4; /* dma translen */
	bk_dma_set_transfer_len(AUD_DAC_DEF_DMA_CHANNEL, aud_dac.buf_len / 4);

	bk_dma_register_isr(AUD_DAC_DEF_DMA_CHANNEL, NULL, audio_dac_dma_handler);
	bk_dma_enable_finish_interrupt(AUD_DAC_DEF_DMA_CHANNEL);
	bk_dma_enable_half_finish_interrupt(AUD_DAC_DEF_DMA_CHANNEL);
}

static void audio_dac_set_dma(UINT32 enable)
{
	if (enable)
		bk_dma_start(AUD_DAC_DEF_DMA_CHANNEL);
	else
		bk_dma_stop(AUD_DAC_DEF_DMA_CHANNEL);
}

static void audio_dac_eixt_dma(void)
{
	bk_dma_stop(AUD_DAC_DEF_DMA_CHANNEL);
}
#endif // CONFIG_GENERAL_DMA

void audio_dac_volume_use_single_port(void)
{
	UINT32 param;

	param = AUDIO_DAC_VOL_SINGLE_MODE;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_SET_VOLUME_PORT,
				  &param);
}

void audio_dac_volume_diff_port(void)
{
	// default mode is ready diff port
	// if use single port, call audio_dac_volume_use_single_port when initial dac
	UINT32 param;

	param = AUDIO_DAC_VOL_DIFF_MODE;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_SET_VOLUME_PORT,
				  &param);
}

static void audio_dac_open_analog_regs(void)
{
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_OPEN_DAC_ANALOG, NULL);
}

static void audio_dac_close_analog_regs(void)
{
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_CLOSE_DAC_ANALOG, NULL);
}

#if AUD_USE_EXT_PA
static void audio_dac_init_mute_pin(UINT32 pin)
{
	UINT32 param;

	BK_ASSERT(pin < GPIONUM);

	param = GPIO_CFG_PARAM(pin, GMODE_OUTPUT);
	sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_CFG, &param);
}

static void audio_dac_eable_mute(UINT32 enable)
{
	UINT32 param;
	BK_ASSERT(aud_dac.mute_pin < GPIONUM);

	if (enable)
		param = GPIO_CFG_PARAM(aud_dac.mute_pin, AUD_DAC_MUTE_ENA_LEVEL);
	else
		param = GPIO_CFG_PARAM(aud_dac.mute_pin, ~AUD_DAC_MUTE_ENA_LEVEL);
	sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_OUTPUT, &param);
}
#endif

static void audio_dac_set_volume(UINT32 percent)
{
	UINT32 param = percent, idx;
	AUD_VOL_PTR vol;

	if (percent > 99)
		percent = 99;

	idx = percent * AUD_DAC_VOL_TABLE_LEN / 100;

	vol = (AUD_VOL_PTR)&aud_vol_table[idx];

	audio_dac_set_gain(vol->dig_gain);

	param = vol->ana_gain;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_SET_DAC_VOLUME_ANALOG, &param);

	if (percent == 0) {
		// no volume, so do mute(disable extral PA)
#if AUD_USE_EXT_PA
		audio_dac_eable_mute(1);
#endif
	} else {
#if AUD_USE_EXT_PA
		if (audio_dac_is_enable_bit())
			audio_dac_eable_mute(0);
#endif
	}

	AUDIO_LOGI("set dac vol:%d - indx:%d,dig:%d,ana:%02x\r\n", percent, idx, vol->dig_gain, vol->ana_gain);
}

static UINT32 audio_dac_open(UINT32 op_flag)
{
	AUD_DAC_CFG_PTR cfg;

	if (!op_flag) {
		AUDIO_LOGE("audio_dac_open is NULL\r\n");
		return AUD_FAILURE;
	}

	cfg = (AUD_DAC_CFG_PTR)op_flag;

#if (!CONFIG_GENERAL_DMA)
	if (cfg->dma_mode) {
		AUDIO_LOGE("audio_dac_open no support dma\r\n");
		return AUD_FAILURE;
	}
#endif // !CONFIG_GENERAL_DMA

	aud_dac.status = AUD_DAC_STA_CLOSED;
	aud_dac.buf = cfg->buf;
	aud_dac.buf_len = cfg->buf_len;
	aud_dac.channels = cfg->channels;
	aud_dac.freq = cfg->freq;
	aud_dac.dma_mode = cfg->dma_mode;

	AUDIO_LOGI("%s: sample_rate %u, channels %u, buf_len %u\n",
			   __func__,
			   aud_dac.freq, aud_dac.channels, aud_dac.buf_len);

	audio_dac_set_enable_bit(0);
	audio_dac_set_sample_rate(aud_dac.freq);
	audio_dac_set_hpf1_bit(1);
	audio_dac_set_hpf2_bit(1);

#if AUD_USE_EXT_PA
	aud_dac.mute_pin = cfg->mute_pin;
	audio_dac_init_mute_pin(aud_dac.mute_pin);
	audio_dac_eable_mute(1);
#endif

#if !AUD_ADC_DAC_HARDWARD_LOOPBACK
	// audio should cfg DAC fifo, both use DMA or not use DMA
	audio_dac_set_read_thred_bit(AUD_DAC_DEF_RD_THRED);
	if (!aud_dac.dma_mode) {
		audio_dac_set_int_enable_bit(1);
		audio_enable_interrupt();
		rb_init(&aud_dac.u.rb, aud_dac.buf, aud_dac.buf_len);
	} else {
		audio_dac_set_int_enable_bit(0);

#if (CONFIG_GENERAL_DMA)
		audio_dac_config_dma();
		aud_dac.need_write_callback = cfg->buf_finish_cb;
		aud_dac.usr_data = cfg->usr_data;

		AUDIO_LOGI("%s: init dma\n", __func__);
		rb_init_dma_read(&aud_dac.u.rb_dma_rd, aud_dac.buf, aud_dac.buf_len, AUD_DAC_DEF_DMA_CHANNEL);
#endif
	}
#endif

	audio_dac_open_analog_regs();
	audio_power_up();

#if AUD_ADC_DAC_HARDWARD_LOOPBACK
	audio_dac_set_enable_bit(1);
	aud_dac.status = AUD_DAC_STA_PLAYING;
#else
	aud_dac.status = AUD_DAC_STA_OPENED;
#endif

	AUDIO_LOGI("%s: dac open succ\n", __func__);
	return AUD_SUCCESS;
}

static UINT32 audio_dac_close(void)
{
#if AUD_USE_EXT_PA
	audio_dac_eable_mute(1);
#endif

	audio_dac_set_enable_bit(0);
	audio_dac_set_int_enable_bit(0);

	if (aud_dac.status == AUD_DAC_STA_CLOSED)
		return AUD_SUCCESS;

#if (CONFIG_GENERAL_DMA)
	if (aud_dac.dma_mode) {
		audio_dac_eixt_dma();
		rb_clear_dma_read(&aud_dac.u.rb_dma_rd);
	}
#endif

	audio_dac_close_analog_regs();

	os_memset(&aud_dac, 0, sizeof(AUD_DAC_DESC_ST));
	aud_dac.status = AUD_DAC_STA_CLOSED;

	AUDIO_LOGI("%s: dac close succ\n", __func__);
	return AUD_SUCCESS;
}

static UINT32 audio_dac_write(char *user_buf, UINT32 count, UINT32 op_flag)
{
	int free_size;

	if (aud_dac.status == AUD_DAC_STA_CLOSED)
		return 0;

	if (aud_dac.dma_mode) {
#if CONFIG_GENERAL_DMA
		RB_DMA_RD_PTR rb;

		rb = &aud_dac.u.rb_dma_rd;
		free_size = rb_get_free_size_dma_read(rb);

		AUDIO_LOGD("0:%d-%d-%d\r\n", free_size, aud_dac.need_write_len, count);
		if (free_size >= aud_dac.buf_len - 4)
			AUDIO_LOGI("0:%d-%d-%d\r\n", free_size, aud_dac.need_write_len, count);

		if (free_size < (int)count) {
			if (aud_dac.status == AUD_DAC_STA_OPENED) {
				bk_dma_set_transfer_len(AUD_DAC_DEF_DMA_CHANNEL, aud_dac.need_write_len);
				audio_dac_set_dma(1);
				audio_dac_set_enable_bit(1);
				aud_dac.status = AUD_DAC_STA_PLAYING;

#if AUD_USE_EXT_PA
				audio_dac_eable_mute(0);
#endif
			}
			return 0;
		}

		free_size = count;
		rb_write_dma_read(rb, (UINT8 *)user_buf, free_size, 1);

		if (aud_dac.status == AUD_DAC_STA_OPENED)
			aud_dac.need_write_len = free_size;
#endif
	} else {
		RB_PTR rb;

		rb = &aud_dac.u.rb;
		free_size = rb_get_free_size(rb);
		if (free_size > count)
			free_size = count;
		rb_write(rb, (UINT8 *)user_buf, free_size, 1);
	}

	return free_size;
}

static UINT32 audio_dac_get_free_buf_size(void)
{
	int free_size;

	if (aud_dac.status != AUD_DAC_STA_PLAYING)
		return 0;

	if (aud_dac.dma_mode) {
#if CONFIG_GENERAL_DMA
		RB_DMA_RD_PTR rb;
		rb = &aud_dac.u.rb_dma_rd;
		free_size = rb_get_free_size_dma_read(rb);
#endif
	} else {
		RB_PTR rb;
		rb = &aud_dac.u.rb;
		free_size = rb_get_free_size(rb);
	}

	return free_size;
}

static void audio_dac_pause(void)
{
	if (aud_dac.status == AUD_DAC_STA_PAUSE)
		return;

	if (aud_dac.dma_mode) {
#if CONFIG_GENERAL_DMA
		audio_dac_set_dma(0);
#endif
	}

	audio_dac_set_enable_bit(0);

	aud_dac.status = AUD_DAC_STA_PAUSE;
}

static void audio_dac_play(void)
{
	if (aud_dac.status == AUD_DAC_STA_PLAYING)
		return;

	if (aud_dac.dma_mode) {
#if CONFIG_GENERAL_DMA
		audio_dac_set_dma(1);
#endif
	}

	audio_dac_set_enable_bit(1);

	aud_dac.status = AUD_DAC_STA_PLAYING;
}

static UINT32 audio_dac_ctrl(UINT32 cmd, void *param)
{
	UINT32 ret = AUD_SUCCESS;

	switch (cmd) {
	case AUD_DAC_CMD_GET_FREE_BUF_SIZE:
		ret = audio_dac_get_free_buf_size();
		break;

	case AUD_DAC_CMD_PLAY:
		audio_dac_play();
		break;

	case AUD_DAC_CMD_PAUSE:
		audio_dac_pause();
		break;

	case AUD_DAC_CMD_SET_SAMPLE_RATE:
		BK_ASSERT(param);
		audio_dac_set_enable_bit(0);
		audio_dac_set_sample_rate(*((UINT32 *)param));
		audio_dac_set_enable_bit(1);
		break;

	case AUD_DAC_CMD_SET_VOLUME:
		BK_ASSERT(param);
		audio_dac_set_volume(*((UINT32 *)param));
		break;

	default:
		break;
	}
	return ret;
}

void audio_dac_software_init(void)
{
	ddev_register_dev(DD_DEV_TYPE_AUD_DAC, (DD_OPERATIONS *)&aud_dac_op);

	os_memset(&aud_dac, 0, sizeof(AUD_DAC_DESC_ST));
	aud_dac.status = AUD_DAC_STA_CLOSED;
}

void audio_dac_isr(void)
{
	UINT32 status = REG_READ(AUD_AD_FIFO_STATUS);
	RB_PTR rb = NULL;
	UINT32 ret = 0, channel;
	INT16 sample[2] = {0};

	if (aud_dac.status == AUD_DAC_STA_PLAYING && !aud_dac.dma_mode) {
		rb = &aud_dac.u.rb;
		channel = aud_dac.channels;
		if (status & (DAC_R_INT_FLAG | DAC_L_INT_FLAG)) {
			if (channel == 2) {
				while (!(status & (DAC_R_FIFO_FULL | DAC_L_FIFO_FULL))) {
					ret = rb_read(rb, (UINT8 *)&sample[0], sizeof(int16), 2);
					//AUDIO_LOGI("%d\r\n", ret);
					if (!ret)
						break;

					audio_dac_set_sample(sample[0], sample[1]);
					status = REG_READ(AUD_AD_FIFO_STATUS);
				}
			} else {
				while (!(status & (DAC_R_FIFO_FULL | DAC_L_FIFO_FULL))) {
					ret = rb_read(rb, (UINT8 *)&sample[0], sizeof(int16), 1);
					if (!ret)
						break;

					audio_dac_set_sample(sample[0], sample[0]);
					status = REG_READ(AUD_AD_FIFO_STATUS);
				}
			}
		}
	}

	if (ret == 0) {
		while (!(status & (DAC_R_FIFO_FULL | DAC_L_FIFO_FULL))) {
			audio_dac_set_sample(0, 0);
			status = REG_READ(AUD_AD_FIFO_STATUS);
		}
		//AUDIO_LOGI("set\r\n");
	}
}

#endif // CONFIG_AUDIO_DAC


#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include "co_list_copy.h"

#if ((CONFIG_AUDIO) && (CONFIG_SOC_BK7251))
#include "audio_pub.h"
#include <os/mem.h>
#include "bk_drv_model.h"
#include "sys_rtos.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>
#include "audio.h"

enum {
	AUD_INTF_DAC_FILL_TIMER   = 0,
	AUD_INTF_DAC_PLAY,
	AUD_INTF_DAC_PAUSE,
	AUD_INTF_DAC_SET_VOLUME,

	AUD_INTF_ADC_GET_TIMER,
	AUD_INTF_ADC_PLAY,
	AUD_INTF_ADC_PAUSE,
	AUD_INTF_ADC_LINEIN_TIMER,

	AUD_INTF_SET_SAMPLE_RATE,

	AUD_INTF_EXIT,


};

typedef struct audio_message {
	UINT32 audio_msg;
} AUDIO_MSG_T;
#define AUDIO_QITEM_COUNT          (5)

beken_queue_t audio_msg_que = NULL;
beken_thread_t  audio_handle = NULL;

void audio_intf_send_msg(u32 new_msg);


#if CONFIG_AUDIO_DAC
AUD_DAC_CFG_ST aud_dac_cfg;
DD_HANDLE aud_dac_hdl = DD_HANDLE_UNVALID;

#define AUD_DAC_BUF_LEN            (4 * 1024)
UINT8 audio_dac_buf[AUD_DAC_BUF_LEN];
UINT8 *audio_read;

#define DAC_TIMER_INTVAL           (15)
beken_timer_t audio_dac_fill_timer;

__maybe_unused static void audio_intf_dac_timer_handler(void *data);

static void audio_intf_dac_timer_poll(void)
{
#if 0
	UINT32 free_len;
	UINT32 left = (QQQG + sizeof(QQQG)) - audio_read;

	free_len = ddev_control(aud_dac_hdl, AUD_DAC_CMD_GET_FREE_BUF_SIZE, NULL);

	if (free_len <  left) {
		ddev_write(aud_dac_hdl, (char *)audio_read, free_len, 0);
		audio_read += free_len;
	} else if (left) {
		UINT32 copy_from_start = free_len - left;
		ddev_write(aud_dac_hdl, (char *)audio_read, left, 0);
		ddev_write(aud_dac_hdl, (char *)QQQG, copy_from_start, 0);
		audio_read = (UINT8 *)&QQQG[copy_from_start];
	}

	//AUDIO_LOGI("%d,%d\r\n", free_len, left);

	rtos_reload_timer(&audio_dac_fill_timer);
	//AUDIO_LOGI("audio_intf_dac_timer_poll, restart timer\r\n");
#endif
}

static void audio_intf_dac_timer_handler(void *data)
{
	audio_intf_send_msg(AUD_INTF_DAC_FILL_TIMER);
}

static void audio_intf_dac_pause_poll(void)
{
	ddev_control(aud_dac_hdl, AUD_DAC_CMD_PAUSE, NULL);
}

void audio_intf_dac_pause(void)
{
	audio_intf_send_msg(AUD_INTF_DAC_PAUSE);
}

static void audio_intf_dac_play_poll(void)
{
	ddev_control(aud_dac_hdl, AUD_DAC_CMD_PLAY, NULL);
}

void audio_intf_dac_play(void)
{
	audio_intf_send_msg(AUD_INTF_DAC_PLAY);
}

static void audio_intf_dac_set_volume_poll(void)
{
	static UINT32 dac_vol = 0;

	dac_vol++;
	if (dac_vol >= 20)
		dac_vol = 0;

	AUDIO_LOGI("set dac vol :%d\r\n", dac_vol);

	ddev_control(aud_dac_hdl, AUD_DAC_CMD_SET_VOLUME, &dac_vol);
}

void audio_intf_dac_set_volume(void)
{
	audio_intf_send_msg(AUD_INTF_DAC_SET_VOLUME);
}

#endif // CONFIG_AUDIO_DAC

#if CONFIG_AUDIO_ADC
AUD_ADC_CFG_ST aud_adc_cfg;
DD_HANDLE aud_adc_hdl = DD_HANDLE_UNVALID;

#define AUD_ADC_BUF_LEN            (3 * 1024)
UINT8 audio_adc_buf[AUD_ADC_BUF_LEN];

#define AUD_ADC_DATA_LEN        (2 * 1024)
UINT8 adc_data[AUD_ADC_DATA_LEN];

#define ADC_TIMER_INTVAL           (4)
beken_timer_t audio_adc_get_timer;

#define ADC_LINEIN_DETECT_TIMER_INTVAL           (10)
beken_timer_t audio_adc_linein_timer;

static void audio_intf_adc_timer_poll(void)
{
	UINT32 mic_filled_len, audio_free_len, copy_len;

	audio_free_len = ddev_control(aud_dac_hdl, AUD_DAC_CMD_GET_FREE_BUF_SIZE, NULL);
	mic_filled_len = ddev_control(aud_adc_hdl, AUD_ADC_CMD_GET_FILL_BUF_SIZE, NULL);

	copy_len = (audio_free_len > mic_filled_len) ? mic_filled_len : audio_free_len;

	if (copy_len) {
		if (copy_len >  AUD_ADC_DATA_LEN)
			copy_len = AUD_ADC_DATA_LEN;

		ddev_read(aud_adc_hdl, (char *)adc_data, copy_len, 0);
		ddev_write(aud_dac_hdl, (char *)adc_data, copy_len, 0);
	}

	//AUDIO_LOGI("%d\r\n", copy_len);

	rtos_reload_timer(&audio_adc_get_timer);
}

static void audio_intf_adc_timer_handler(void *data)
{
	audio_intf_send_msg(AUD_INTF_ADC_GET_TIMER);
}

static void audio_intf_adc_pause_poll(void)
{
	ddev_control(aud_adc_hdl, AUD_ADC_CMD_PAUSE, NULL);
}

void audio_intf_adc_pause(void)
{
	audio_intf_send_msg(AUD_INTF_ADC_PAUSE);
}

static void audio_intf_adc_play_poll(void)
{
	ddev_control(aud_adc_hdl, AUD_ADC_CMD_PLAY, NULL);
}

void audio_intf_adc_play(void)
{
	audio_intf_send_msg(AUD_INTF_ADC_PLAY);
}

static void audio_intf_adc_linein_timer_poll(void)
{
	ddev_control(aud_adc_hdl, AUD_ADC_CMD_DO_LINEIN_DETECT, NULL);

	rtos_reload_timer(&audio_adc_linein_timer);
	//AUDIO_LOGI("restart timer\r\n");
}

static void audio_intf_adc_linein_timer_handler(void *data)
{
	audio_intf_send_msg(AUD_INTF_ADC_LINEIN_TIMER);
}
#endif

UINT32 sample_rate_tab[] = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000};
static void audio_intf_set_sample_rate_poll(void)
{
	static UINT32 i = 0;
	UINT32 param = 0;

	i++;
	if (i >= sizeof(sample_rate_tab) / 4)
		i = 0;

	param = sample_rate_tab[i];

#if CONFIG_AUDIO_DAC
	ddev_control(aud_dac_hdl, AUD_DAC_CMD_SET_SAMPLE_RATE, &param);
#endif

#if CONFIG_AUDIO_ADC
	ddev_control(aud_adc_hdl, AUD_ADC_CMD_SET_SAMPLE_RATE, &param);
#endif

	AUDIO_LOGI("set sample rate:%d\r\n", param);
}

void audio_intf_set_sample_rate(void)
{
	audio_intf_send_msg(AUD_INTF_SET_SAMPLE_RATE);
}

static void audio_intf_main(beken_thread_arg_t data)
{
	bk_err_t err;
	UINT32 status;
	GLOBAL_INT_DECLARATION();

#if CONFIG_AUDIO_DAC
	aud_dac_cfg.buf = &audio_dac_buf[0];
	aud_dac_cfg.buf_len = AUD_DAC_BUF_LEN;
	aud_dac_cfg.freq = 44100;
	aud_dac_cfg.channels = 2;
	aud_dac_cfg.dma_mode = 0;
#if AUD_USE_EXT_PA
	aud_dac_cfg.mute_pin = AUD_DAC_MUTE_PIN;
#else
	aud_dac_cfg.mute_pin = 0;
#endif

	//audio_read = (UINT8*)&QQQG[0];

	GLOBAL_INT_DISABLE();
	aud_dac_hdl = ddev_open(DD_DEV_TYPE_AUD_DAC, &status, (UINT32)&aud_dac_cfg);
	status = status;
	if (DD_HANDLE_UNVALID == aud_dac_hdl) {
		GLOBAL_INT_RESTORE();
		goto audio_exit;
	}
	GLOBAL_INT_RESTORE();
#if 0
	err = rtos_init_timer(&audio_dac_fill_timer,
						  DAC_TIMER_INTVAL,
						  audio_intf_dac_timer_handler,
						  (void *)0);
	BK_ASSERT(kNoErr == err);

	err = rtos_start_timer(&audio_dac_fill_timer);
	BK_ASSERT(kNoErr == err);
#endif
#endif

#if CONFIG_AUDIO_ADC
	aud_adc_cfg.buf = &audio_adc_buf[0];
	aud_adc_cfg.buf_len = AUD_ADC_BUF_LEN;
	aud_adc_cfg.freq = 44100;
	aud_adc_cfg.channels = 2;

	aud_adc_cfg.mode = 0;
	//aud_adc_cfg.mode |= AUD_ADC_MODE_DMA_BIT;
	aud_adc_cfg.linein_detect_pin = AUD_ADC_LINEIN_DETECT_PIN;

	GLOBAL_INT_DISABLE();
	aud_adc_hdl = ddev_open(DD_DEV_TYPE_AUD_DAC, &status, (UINT32)&aud_adc_cfg);
	status = status;
	if (DD_HANDLE_UNVALID == aud_adc_hdl) {
		GLOBAL_INT_RESTORE();
		goto audio_exit;
	}
	GLOBAL_INT_RESTORE();

#if !AUD_ADC_DAC_HARDWARD_LOOPBACK
	err = rtos_init_timer(&audio_adc_get_timer,
						  ADC_TIMER_INTVAL,
						  audio_intf_adc_timer_handler,
						  (void *)0);
	BK_ASSERT(kNoErr == err);

	err = rtos_start_timer(&audio_adc_get_timer);
	BK_ASSERT(kNoErr == err);
#endif

	err = rtos_init_timer(&audio_adc_linein_timer,
						  ADC_LINEIN_DETECT_TIMER_INTVAL,
						  audio_intf_adc_linein_timer_handler,
						  (void *)0);
	BK_ASSERT(kNoErr == err);

	err = rtos_start_timer(&audio_adc_linein_timer);
	BK_ASSERT(kNoErr == err);
#endif

	while (1) {
		AUDIO_MSG_T msg;
		err = rtos_pop_from_queue(&audio_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == err) {
			switch (msg.audio_msg) {
#if CONFIG_AUDIO_DAC
			case AUD_INTF_DAC_FILL_TIMER: {
				audio_intf_dac_timer_poll();
			}
			break;

			case AUD_INTF_DAC_PLAY: {
				audio_intf_dac_play_poll();
			}
			break;

			case AUD_INTF_DAC_PAUSE: {
				audio_intf_dac_pause_poll();
			}
			break;

			case AUD_INTF_DAC_SET_VOLUME: {
				audio_intf_dac_set_volume_poll();
			}
			break;

#endif // CONFIG_AUDIO_DAC

#if CONFIG_AUDIO_ADC
			case AUD_INTF_ADC_GET_TIMER: {
				audio_intf_adc_timer_poll();
			}
			break;

			case AUD_INTF_ADC_PLAY: {
				audio_intf_adc_play_poll();
			}
			break;

			case AUD_INTF_ADC_PAUSE: {
				audio_intf_adc_pause_poll();
			}
			break;

			case AUD_INTF_ADC_LINEIN_TIMER: {
				audio_intf_adc_linein_timer_poll();
			}
			break;
#endif // CONFIG_AUDIO_DAC

			case AUD_INTF_SET_SAMPLE_RATE: {
				audio_intf_set_sample_rate_poll();
			}
			break;

			case AUD_INTF_EXIT: {
				AUDIO_LOGE("exit aud intf\r\n");
				goto audio_exit;
			}
			break;

			default:
				break;
			}
		}
	}

audio_exit:

#if CONFIG_AUDIO_DAC
	GLOBAL_INT_DISABLE();
	status = ddev_close(aud_dac_hdl);
	GLOBAL_INT_RESTORE();
	aud_dac_hdl = DD_HANDLE_UNVALID;

	err = rtos_deinit_timer(&audio_dac_fill_timer);
	BK_ASSERT(kNoErr == err);
#endif // CONFIG_AUDIO_DAC

#if CONFIG_AUDIO_ADC
	GLOBAL_INT_DISABLE();
	status = ddev_close(aud_adc_hdl);
	GLOBAL_INT_RESTORE();
	aud_adc_hdl = DD_HANDLE_UNVALID;

	err = rtos_deinit_timer(&audio_adc_get_timer);
	BK_ASSERT(kNoErr == err);

	err = rtos_deinit_timer(&audio_adc_linein_timer);
	BK_ASSERT(kNoErr == err);
#endif // CONFIG_AUDIO_ADC

	rtos_deinit_queue(&audio_msg_que);
	audio_msg_que = NULL;

	audio_handle = NULL;
	rtos_delete_thread(NULL);
}

UINT32 audio_intf_init(void)
{
	int ret;

	if ((!audio_handle) && (!audio_msg_que)) {

		ret = rtos_init_queue(&audio_msg_que,
							  "audio_queue",
							  sizeof(AUDIO_MSG_T),
							  AUDIO_QITEM_COUNT);
		if (kNoErr != ret) {
			AUDIO_LOGE("audio_intf ceate queue failed\r\n");
			return kGeneralErr;
		}

		ret = rtos_create_thread(&audio_handle,
								 BEKEN_DEFAULT_WORKER_PRIORITY,
								 "audio",
								 (beken_thread_function_t)audio_intf_main,
								 1024,
								 NULL);
		if (ret != kNoErr) {
			rtos_deinit_queue(&audio_msg_que);
			audio_msg_que = NULL;
			AUDIO_LOGE("Error: Failed to create audio_intf: %d\r\n", ret);
			return kGeneralErr;
		}
	}

	return kNoErr;
}

void audio_intf_uninit(void)
{
	if ((audio_handle) && (audio_msg_que)) {
		audio_intf_send_msg(AUD_INTF_EXIT);
		// wait untill task exit
		while (audio_handle)
			rtos_delay_milliseconds(100);
	}
}

void audio_intf_send_msg(u32 new_msg)
{
	bk_err_t ret;
	AUDIO_MSG_T msg;

	if (audio_msg_que) {
		msg.audio_msg = new_msg;

		ret = rtos_push_to_queue(&audio_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			os_printf("audio_intf_send_msg failed\r\n");
	}
}


#endif // CONFIG_AUDIO

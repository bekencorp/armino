#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <modules/audio_ring_buff.h>
#include <modules/aec.h>
#include <driver/aud_types.h>
#include <driver/dma.h>


typedef enum {
	AUD_TRAS_DRV_IDLE = 0,
	AUD_TRAS_DRV_AEC,
	AUD_TRAS_DRV_ENCODER,
	AUD_TRAS_DRV_DECODER,
//	AUD_TRAS_DRV_TX_DONE,
	AUD_TRAS_DRV_START,
	AUD_TRAS_DRV_EXIT,
	AUD_TRAS_DRV_CONTROL,
	AUD_TRAS_DRV_MAX,
} audio_tras_drv_op_t;

typedef enum {
	AUD_TRAS_DRV_MODE_CPU0 = 0,
	AUD_TRAS_DRV_MODE_CPU1,
	AUD_TRAS_DRV_MODE_MAX,
} audio_tras_drv_mode_t;

typedef enum {
	AUD_TRAS_SET_EC_DEPTH,
	AUD_TRAS_GET_EC_DEPTH,
	AUD_TRAS_CTRL_MAX,
} audio_tras_ctrl_op_t;

typedef struct {
	audio_tras_ctrl_op_t op;
	uint32_t param;
} audio_tras_ctrl_t;

typedef struct {
	audio_tras_drv_op_t op;
	audio_tras_ctrl_t * param;
} audio_tras_drv_msg_t;

typedef enum {
	AEC_SAMP_RATE_8K = 0,
	AEC_SAMP_RATE_16K = 1,
	AEC_SAMP_RATE_MAX,
} aec_samp_rate_t;

typedef struct {
	/* default value */
	uint16_t init_flags;
	/* aec */
	uint16_t mic_delay;		//set delay points of ref data according to dump data
	uint8_t ec_depth;		//recommended value range: 1~50, the greater the echo, the greater the value setting
	uint8_t ref_scale;		//value range:0,1,2, the greater the signal amplitude, the greater the setting
	uint8_t voice_vol;		//the voice volume level
	uint8_t TxRxThr;		//the max amplitude of rx audio data
	uint8_t TxRxFlr;		//the min amplitude of rx audio data
	/* ns */
	uint8_t ns_level;		//recommended value range: 1~8, the lower the noise, the lower the level
	uint8_t ns_para;		//value range:0,1,2, the lower the noise, the lower the level, the default valude is recommended
	/* drc */
	uint8_t drc;			//recommended value range:0x10~0x1f, the greater the value, the greater the volume
} aec_config_t;

typedef struct {
	AECContext* aec;
	aec_config_t *aec_config;
	aec_samp_rate_t samp_rate;       //the sample rate of AEC
	uint16_t samp_rate_points;       //the number of points in AEC frame
	int16_t* ref_addr;
	int16_t* mic_addr;
	int16_t* out_addr;
	int16_t *aec_ref_ring_buff;      //save ref data of aec
	int16_t *aec_out_ring_buff;      //save audio data processed by AEC
	RingBufferContext ref_rb; //ref data of AEC context
	RingBufferContext aec_rb; //out data of AEC context
} aec_context_t;

typedef struct {
	uint32_t *buff_addr;
	bool busy_status;
} tx_buff_t;

/* tx buffer config of audio transfer */
typedef struct {
	bool tx_buff_status;		//malloc tx buffer complete
	tx_buff_t ping;
	tx_buff_t pang;
	uint16_t buff_length;
} tx_context_t;

/* rx buffer config of audio transfer */
typedef struct {
	bool rx_buff_status;			//rx buffer status, true:rx ring buffer init complete, false:rx ring buffer not init
	//uint32_t ring_buff_addr;		//rx ring buffer address
	int32_t *decoder_ring_buff;		//save need to decode dac data received from apk
	RingBufferContext *decoder_rb;	//decoder ring buffer context, cpu0 malloc and init
	uint32_t frame_size;			//the size (byte) of one frame
	uint8_t frame_num;				//the number of frame in ring buffer
	//uint8_t fill_num;				//fill frame number
	uint16_t rx_buff_seq_tail;		//the rx data sequence in tail of filled frames
	//uint16_t rx_buff_seq_head;	//the rx data sequence in head of filled frames
	uint16_t aud_trs_read_seq;		//save the sequence number of audio dac readed
	uint16_t fifo_frame_num;
} rx_context_t;

typedef enum {
	EVENT_AUD_TRAS_INIT_CMP,
	EVENT_AUD_TRAS_START_CMP,
	EVENT_AUD_TRAS_STOP_CMP,
#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	EVENT_AUD_TRAS_AEC_DUMP,
#endif
	EVENT_AUD_TRAS_MAX,
} audio_tras_event_t;

typedef struct {
	void (*audio_transfer_event_cb)(audio_tras_event_t event);
	int (*audio_send_mic_data)(unsigned char *data, unsigned int len);
} aud_cb_t;

typedef struct {
	int16_t *pcm_data;
	uint8_t *law_data;
} audio_tras_encode_temp_t;

typedef struct {
	int16_t *pcm_data;
	uint8_t *law_data;
} audio_tras_decode_temp_t;

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
typedef struct {
	/* aec dump config */
	int16_t *mic_dump_addr;
	int16_t *ref_dump_addr;
	int16_t *out_dump_addr;
	uint32_t len;
} aec_dump_t;
#endif

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
typedef struct {
	uint32_t lost_size;
	uint32_t complete_size;
} lost_count_debug_t;
#endif

typedef struct {
	//audio_tras_drv_mode_t mode;			//AUD_TRAS_DRV_MODE_CPU0: audio transfer work in cpu0, AUD_TRAS_DRV_MODE_CPU1:audio transfer work in cpu1
	bool aec_enable;
	aec_context_t *aec_context;

	dma_id_t adc_dma_id;				//audio transfer ADC DMA id
	uint16_t mic_samp_rate_points;		//the number of points in mic frame
	uint8_t mic_frame_number;			//the max frame number of mic ring buffer
	int32_t *mic_ring_buff;				//save mic data from audio adc
	RingBufferContext mic_rb;			//mic ring buffer context

	uint16_t speaker_samp_rate_points;	//the number of points in speaker frame
	uint8_t speaker_frame_number;		//the max frame number of speaker ring buffer
	int32_t *speaker_ring_buff;			//save dac data of speaker
	RingBufferContext speaker_rb;		//speaker ring buffer context
	dma_id_t dac_dma_id;				//audio transfer DAC DMA id

	aud_adc_config_t *adc_config;
	aud_dac_config_t *dac_config;
	tx_context_t tx_context;
	rx_context_t rx_context;			//rx_context shared by cpu0 and cpu1, cpu0 malloc

	/* audio transfer callback */
	aud_cb_t aud_cb;

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	aec_dump_t aec_dump;
#endif

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
	lost_count_debug_t lost_count;
#endif

	audio_tras_encode_temp_t encoder_temp;
	audio_tras_decode_temp_t decoder_temp;
} audio_transfer_context_t;

bk_err_t audio_transfer_send_msg(audio_tras_drv_op_t op, audio_tras_ctrl_t * param);

#ifdef __cplusplus
}
#endif


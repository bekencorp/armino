#pragma once

#include <modules/audio_ring_buff.h>
#include <modules/aec.h>
#include <driver/aud_types.h>
#include <driver/dma.h>
#include "aud_intf_private.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	AUD_TRAS_DRV_IDLE = 0,

	/* aud intf op */
	AUD_TRAS_DRV_INIT,
	AUD_TRAS_DRV_DEINIT,		/**< deinit aud tras drv */
	AUD_TRAS_DRV_SET_MODE,		/**< set work mode */

	/* mic op */
	AUD_TRAS_DRV_MIC_INIT,		/**< init mic */
	AUD_TRAS_DRV_MIC_DEINIT,	/**< deinit mic */
	AUD_TRAS_DRV_MIC_START,		/**< start mic */
	AUD_TRAS_DRV_MIC_PAUSE,		/**< pause mic */
	AUD_TRAS_DRV_MIC_STOP,		/**< stop mic */
	AUD_TRAS_DRV_MIC_SET_CHL,	/**< set mic channel */
	AUD_TRAS_DRV_MIC_SET_SAMP_RATE,		/**< set adc sample rate */
	AUD_TRAS_DRV_MIC_SET_GAIN,			/**< set adc gain */
	/* mic int op */
	AUD_TRAS_DRV_MIC_TX_DATA,			/**< call callback to send mic data */

	/* spk op */
	AUD_TRAS_DRV_SPK_INIT,		/**< init spk */
	AUD_TRAS_DRV_SPK_DEINIT,	/**< deinit spk */
	AUD_TRAS_DRV_SPK_START,		/**< start spk */
	AUD_TRAS_DRV_SPK_PAUSE,		/**< pause spk */
	AUD_TRAS_DRV_SPK_STOP,		/**< stop spk */
	AUD_TRAS_DRV_SPK_SET_CHL,	/**< set speaker channel */
	AUD_TRAS_DRV_SPK_SET_SAMP_RATE, 	/**< set dac sample rate */
	AUD_TRAS_DRV_SPK_SET_GAIN,			/**< set dac gain */
	/* spk int op */
	AUD_TRAS_DRV_SPK_REQ_DATA,			/**< set dac gain */

	/* voc op */
	AUD_TRAS_DRV_VOC_INIT,		/**< init voc */
	AUD_TRAS_DRV_VOC_DEINIT,	/**< deinit voc */
	AUD_TRAS_DRV_VOC_START,		/**< start voc */
	AUD_TRAS_DRV_VOC_STOP,		/**< stop voc */
	AUD_TRAS_DRV_VOC_SET_MIC_GAIN,		/**< set audio adc gain */
	AUD_TRAS_DRV_VOC_SET_SPK_GAIN,		/**< set audio dac gain */
	/* voc int op */
	AUD_TRAS_DRV_AEC,			/**< aec process mic data */
	AUD_TRAS_DRV_ENCODER,		/**< encoder mic data processed by aec */
	AUD_TRAS_DRV_DECODER,		/**< decoder speaker data encoded */
//	AUD_TRAS_DRV_TX_DONE,

	AUD_TRAS_DRV_START,
	AUD_TRAS_DRV_EXIT,

	AUD_TRAS_DRV_CONTROL,
	AUD_TRAS_DRV_MAX,
} aud_tras_drv_op_t;

typedef struct {
	aud_tras_drv_op_t op;
	void *param;
} aud_tras_drv_msg_t;

/* aec config */
typedef enum {
	AEC_SAMP_RATE_8K = 0,
	AEC_SAMP_RATE_16K = 1,
	AEC_SAMP_RATE_MAX,
} aec_samp_rate_t;

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
} aec_info_t;

typedef enum {
	EVENT_AUD_TRAS_DRV_INIT_CMP,
	EVENT_AUD_TRAS_DRV_START_CMP,
	EVENT_AUD_TRAS_DRV_STOP_CMP,
#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	EVENT_AUD_TRAS_DRV_AEC_DUMP,
#endif
	EVENT_AUD_TRAS_DRV_MAX,
} aud_tras_drv_event_t;

/*
typedef struct {
	void (*audio_transfer_event_cb)(audio_tras_event_t event);
	int (*audio_send_mic_data)(unsigned char *data, unsigned int len);
} aud_cb_t;
*/

/* temporary data buffer used in voice transfer mode */
typedef struct {
	int16_t *pcm_data;
	uint8_t *law_data;
} aud_tras_drv_encode_temp_t;

typedef struct {
	int16_t *pcm_data;
	uint8_t *law_data;
} aud_tras_drv_decode_temp_t;

/* voice transfer status */
typedef enum {
	AUD_TRAS_DRV_VOC_STA_NULL = 0,		/**< default status: the voice is not init */
	AUD_TRAS_DRV_VOC_STA_IDLE,			/**< idle status: the voice is init */
	AUD_TRAS_DRV_VOC_STA_START, 		/**< start status: the voice is playing */
	AUD_TRAS_DRV_VOC_STA_STOP,			/**< stop status: the voice is stop */
	AUD_TRAS_DRV_VOC_STA_MAX,
} aud_tras_voc_sta_t;

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
typedef struct {
	uint32_t lost_size;
	uint32_t complete_size;
} lost_count_debug_t;
#endif

typedef struct {
	//audio_tras_drv_mode_t mode;			//AUD_TRAS_DRV_MODE_CPU0: audio transfer work in cpu0, AUD_TRAS_DRV_MODE_CPU1:audio transfer work in cpu1
	aud_tras_voc_sta_t status;
	bool aec_enable;
	aec_info_t *aec_info;

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
	tx_info_t tx_info;
	rx_info_t rx_info;			//rx_context shared by cpu0 and cpu1, cpu0 malloc

	/* audio transfer callback */
//	aud_cb_t aud_cb;
	void (*aud_tras_drv_voc_event_cb)(aud_tras_drv_voc_event_t event, void *param);

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	aec_dump_t aec_dump;
#endif

	aud_intf_voc_data_type_t data_type;

#if CONFIG_AUD_TRAS_LOST_COUNT_DEBUG
	lost_count_debug_t lost_count;
#endif

	aud_tras_drv_encode_temp_t encoder_temp;
	aud_tras_drv_decode_temp_t decoder_temp;
} aud_tras_drv_voc_info_t;

/******************************** mic info ****************************************/
/* audio transfer mic status */
typedef enum {
	AUD_TRAS_DRV_MIC_STA_NULL = 0,		/**< default status: the mic is not init */
	AUD_TRAS_DRV_MIC_STA_IDLE,			/**< idle status: the mic is init */
	AUD_TRAS_DRV_MIC_STA_START,			/**< start status: the mic is sampling */
	AUD_TRAS_DRV_MIC_STA_PAUSE,			/**< pause status: the mic is pause */
	//AUD_TRAS_DRV_MIC_STA_STOP,			/**< stop status: the mic is stop */
	AUD_TRAS_DRV_MIC_STA_MAX,
} aud_tras_mic_sta_t;

/* audio transfer mic information in general mode */
typedef struct {
	bool mic_en;						/**< mic enable, true: enable, false: disable */
	aud_tras_mic_sta_t status;
	aud_intf_mic_chl_t mic_chl;
	aud_adc_config_t *adc_config;
	uint32_t frame_size;									/* size of one frame mic data, the size
																when AUD_MIC_CHL_MIC1 mode, the size must bean integer multiple of two bytes
																when AUD_MIC_CHL_DUAL mode, the size must bean integer multiple of four bytes */

	dma_id_t mic_dma_id;									//dma id that cpu carry mic data from fifo to ring buffer
	RingBufferContext mic_rb;
	int32_t *mic_ring_buff;									//save mic data
	int32_t *temp_mic_addr;									//save temporary one frame mic data readed from mic_ring_buff
} aud_tras_drv_mic_info_t;


/******************************** spk info ****************************************/
/* audio transfer speaker status */
typedef enum {
	AUD_TRAS_DRV_SPK_STA_NULL = 0,		/**< default status: the speaker is not init */
	AUD_TRAS_DRV_SPK_STA_IDLE,			/**< idle status: the speaker is init */
	AUD_TRAS_DRV_SPK_STA_START,			/**< start status: the speaker is playing */
	AUD_TRAS_DRV_SPK_STA_PAUSE,			/**< pause status: the speaker is pause */
//	AUD_TRAS_DRV_SPK_STA_STOP,			/**< stop status: the speaker is stop */
	AUD_TRAS_DRV_SPK_STA_MAX,
} aud_tras_spk_sta_t;

/* audio transfer speaker information in general mode */
typedef struct {
	bool spk_en;						/**< mic enable, true: enable, false: disable */
	aud_tras_spk_sta_t status;
	aud_intf_spk_chl_t spk_chl;
	aud_dac_config_t *dac_config;
	uint32_t frame_size;									/* size of one frame speaker data, the size
																when AUD_MIC_CHL_MIC1 mode, the size must bean integer multiple of two bytes
																when AUD_MIC_CHL_DUAL mode, the size must bean integer multiple of four bytes */

	dma_id_t spk_dma_id;									//dma id that cpu carry mic data from fifo to ring buffer
	RingBufferContext spk_rb;
	int32_t *spk_ring_buff;									//save mic data
	int32_t *temp_spk_addr;									//save temporary one frame speaker data readed from spk_rx_ring_buff

	uint32_t *spk_rx_ring_buff;				/**< the ring buffer address of received speaker data */
	uint16_t fifo_frame_num;				/**< audio dac start work when the fram number received is equal to fifo_frame_num */
	RingBufferContext *spk_rx_rb;			/**< speaker received ring buffer context */

	void (*aud_tras_drv_spk_event_cb)(aud_tras_drv_spk_event_t event, void *param);
} aud_tras_drv_spk_info_t;


/******************************** aud tras drv info ****************************************/
/* audio transfer driver status */
typedef enum {
	AUD_TRAS_DRV_STA_NULL = 0,
	AUD_TRAS_DRV_STA_IDLE,
	AUD_TRAS_DRV_STA_WORKING,
	AUD_TRAS_DRV_STA_MAX,
} aud_tras_drv_sta_t;

/* audio transfer driver information */
typedef struct {
	aud_intf_work_mode_t work_mode;
	aud_tras_drv_sta_t status;
	aud_tras_drv_mic_info_t mic_info;
	aud_tras_drv_spk_info_t spk_info;
	aud_tras_drv_voc_info_t voc_info;

	/* callbacks */
	int (*aud_tras_tx_mic_data)(unsigned char *data, unsigned int size);	/**< the api is called when collecting a frame mic packet data is complete */
	bk_err_t (*aud_tras_rx_spk_data)(unsigned int size);					/**< the api is called when playing a frame speaker packet data is complete */
} aud_tras_drv_info_t;

#ifdef __cplusplus
}
#endif


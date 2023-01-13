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

#pragma once

#include <common/bk_include.h>
#include <driver/aud_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_AUD_INTF_BASE              (BK_ERR_AUD_BASE - 0x50)

#define BK_ERR_AUD_INTF_OK                BK_OK                               /**< ok */
#define BK_ERR_AUD_INTF_FAIL              BK_FAIL                             /**< fail */
#define BK_ERR_AUD_INTF_STA               (BK_ERR_AUD_INTF_BASE - 1)          /**< audio interface module status is error */
#define BK_ERR_AUD_INTF_PARAM             (BK_ERR_AUD_INTF_BASE - 2)          /**< parameter is error */
#define BK_ERR_AUD_INTF_MEMY              (BK_ERR_AUD_INTF_BASE - 3)          /**< malloc memory fail */
#define BK_ERR_AUD_INTF_ADC               (BK_ERR_AUD_INTF_BASE - 4)          /**< config audio adc fail */
#define BK_ERR_AUD_INTF_DAC               (BK_ERR_AUD_INTF_BASE - 5)          /**< config audio dac fail */
#define BK_ERR_AUD_INTF_DMA               (BK_ERR_AUD_INTF_BASE - 6)          /**< config dma fail */
#define BK_ERR_AUD_INTF_AEC               (BK_ERR_AUD_INTF_BASE - 7)          /**< config aec fail */
#define BK_ERR_AUD_INTF_RING_BUFF         (BK_ERR_AUD_INTF_BASE - 8)          /**< config ring buffer fail */
#define BK_ERR_AUD_INTF_TIMEOUT           (BK_ERR_AUD_INTF_BASE - 9)          /**< aud_intf api excute timeout */
#define BK_ERR_AUD_INTF_TX_MSG            (BK_ERR_AUD_INTF_BASE - 10)         /**< aud_intf api send message fail */
#define BK_ERR_AUD_INTF_BUSY              (BK_ERR_AUD_INTF_BASE - 11)         /**< aud_intf is busying */
#define BK_ERR_AUD_INTF_FILE              (BK_ERR_AUD_INTF_BASE - 12)         /**< aud_intf file operate fail */
#define BK_ERR_AUD_INTF_UAC_DRV           (BK_ERR_AUD_INTF_BASE - 13)         /**< aud_intf uac driver fail */
#define BK_ERR_AUD_INTF_UAC_MIC           (BK_ERR_AUD_INTF_BASE - 14)         /**< aud_intf uac mic fail */
#define BK_ERR_AUD_INTF_UAC_SPK           (BK_ERR_AUD_INTF_BASE - 15)         /**< aud_intf uac spk operate fail */



/**************** audio interface common ****************/
/* audio interface work mode */
typedef enum {
	AUD_INTF_WORK_MODE_NULL = 0,
	AUD_INTF_WORK_MODE_GENERAL,		/**< General mode: support record mic data and play speaker */
	AUD_INTF_WORK_MODE_VOICE,		/**< Voice mode: support voice transfer  */
	AUD_INTF_WORK_MODE_MAX,
} aud_intf_work_mode_t;

/* audio interface task configuration */
typedef struct {
	uint8_t priority;				/**< the priority of audio interface task */
} aud_intf_task_config_t;

/* audio interface driver setup configuration */
typedef struct {
	aud_intf_work_mode_t work_mode;
	aud_intf_task_config_t task_config;
	int (*aud_intf_tx_mic_data)(unsigned char *data, unsigned int size);		/**< the api is called when collecting a frame mic packet data is complete */
	bk_err_t (*aud_intf_rx_spk_data)(unsigned int size);						/**< the api is called when playing a frame speaker packet data is complete */
} aud_intf_drv_setup_t;

#define DEFAULT_AUD_INTF_DRV_SETUP_CONFIG() {          \
        .work_mode = AUD_INTF_WORK_MODE_NULL,          \
        .task_config = {.priority = 3},                \
        .aud_intf_tx_mic_data = NULL,                  \
        .aud_intf_rx_spk_data = NULL,                  \
    }

typedef bk_err_t (*aud_intf_dump_data_callback)(unsigned char *data, unsigned int size);

/**************** audio interface mic ****************/
/* audio interface mic channel enum */
typedef enum {
	AUD_INTF_MIC_CHL_MIC1 = 0,		/**< MIC1: only enable mic1 channel */
	AUD_INTF_MIC_CHL_DUAL,			/**< DUAL: enable mic1 channel both and MIC2 channel */
	AUD_INTF_MIC_CHL_MAX,
} aud_intf_mic_chl_t;

/* audio interface mic type */
typedef enum {
	AUD_INTF_MIC_TYPE_BOARD = 0,
	AUD_INTF_MIC_TYPE_UAC,
	AUD_INTF_MIC_TYPE_MAX,
} aud_intf_mic_type_t;

/* audio interface mic setup configuration */
typedef struct {
	aud_intf_mic_chl_t mic_chl;
	aud_adc_samp_rate_t samp_rate;		/**< mic sample rate */
	uint16_t frame_size;				/**< size: a frame packet mic data size(byte) */
	uint8_t mic_gain;					/**< audio adc gain: value range:0x0 ~ 0x3f, suggest:0x2d */
	aud_intf_mic_type_t mic_type;		/**< audio mic type: uac or microphone */
} aud_intf_mic_setup_t;

#define DEFAULT_AUD_INTF_MIC_SETUP_CONFIG() {          \
        .mic_chl = AUD_INTF_MIC_CHL_MIC1,              \
        .samp_rate = AUD_ADC_SAMP_RATE_8K,             \
        .frame_size = 320,                             \
        .mic_gain = 0x2d,                              \
        .mic_type = AUD_INTF_MIC_TYPE_BOARD,           \
    }

/**************** audio interface speaker ****************/
/* audio interface speaker channel enum */
typedef enum {
	AUD_INTF_SPK_CHL_LEFT = 0,		/**< LEFT: only enable SPK left channel */
	AUD_INTF_SPK_CHL_DUAL,			/**< DUAL: enable SPK left and right channel */
	AUD_INTF_SPK_CHL_MAX,
} aud_intf_spk_chl_t;

/* audio interface mic type */
typedef enum {
	AUD_INTF_SPK_TYPE_BOARD = 0,
	AUD_INTF_SPK_TYPE_UAC,
	AUD_INTF_SPK_TYPE_MAX,
} aud_intf_spk_type_t;

/* audio interface speaker setup configuration */
typedef struct {
	aud_intf_spk_chl_t spk_chl;
	aud_dac_samp_rate_t samp_rate;		/**< speaker sample rate */
	uint16_t frame_size;						/**< size: a frame packet speaker data size(byte) */
	uint8_t spk_gain;							/**< audio dac gain: value range:0x0 ~ 0x3f, suggest:0x2d */
	aud_dac_work_mode_t work_mode;				/**< audio dac mode: signal_ended/differen */
	aud_intf_spk_type_t spk_type;				/**< audio speaker type: uac or speaker */
} aud_intf_spk_setup_t;

#define DEFAULT_AUD_INTF_SPK_SETUP_CONFIG() {          \
        .spk_chl = AUD_INTF_SPK_CHL_LEFT,              \
        .samp_rate = AUD_DAC_SAMP_RATE_8K,            \
        .frame_size = 320,                             \
        .spk_gain = 0x2d,                              \
        .work_mode = AUD_DAC_WORK_MODE_DIFFEN,         \
        .spk_type = AUD_INTF_SPK_TYPE_BOARD,           \
    }

/**************** audio interface voice ****************/
/* audio interface voice transfer sample rate */
typedef enum {
	AUD_INTF_VOC_SAMP_RATE_8K = 0,
	AUD_INTF_VOC_SAMP_RATE_16K,
	AUD_INTF_VOC_SAMP_RATE_MAX,
} aud_intf_voc_samp_rate_t;

/* audio interface voice transfer data type */
typedef enum {
	AUD_INTF_VOC_DATA_TYPE_G711A = 0,		/**< the data of voice transfer encoded by G711A */
	AUD_INTF_VOC_DATA_TYPE_PCM,				/**< the data of voice transfer is PCM */
	AUD_INTF_VOC_DATA_TYPE_MAX,
} aud_intf_voc_data_type_t;

/*  */
typedef struct {
	/* aec */
	uint32_t ec_depth;			/**< recommended value range: 1~50, the greater the echo, the greater the value setting */
	uint32_t TxRxThr;			/**< the max amplitude of rx audio data */
	uint32_t TxRxFlr;			/**< the min amplitude of rx audio data */
	uint8_t ref_scale;			/**< value range:0,1,2, the greater the signal amplitude, the greater the setting */
	/* ns */
	uint8_t ns_level;			/**< recommended value range: 1~8, the lower the noise, the lower the level */
	uint8_t ns_para;			/**< value range:0,1,2, the lower the noise, the lower the level, the default valude is recommended */
} aud_intf_voc_aec_cfg_t;

/* aec parameters */
typedef enum {
	AUD_INTF_VOC_AEC_NULL = 0,
	AUD_INTF_VOC_AEC_INIT_FLAG,			/**< init flag, aec module feature enable */
	AUD_INTF_VOC_AEC_MIC_DELAY,			/**< mic_delay */
	AUD_INTF_VOC_AEC_EC_DEPTH,			/**< ec_depth */
	AUD_INTF_VOC_AEC_REF_SCALE,			/**< ref_scale: 0/1/2 */
	AUD_INTF_VOC_AEC_VOICE_VOL,			/**< voice volume level */
	AUD_INTF_VOC_AEC_TXRX_THR,			/**< max amplitude of rx audio data */
	AUD_INTF_VOC_AEC_TXRX_FLR,			/**< min amplitude of rx audio data */
	AUD_INTF_VOC_AEC_NS_LEVEL,			/**< noise level: 0/1/2 */
	AUD_INTF_VOC_AEC_NS_PARA,			/**< noise value, range:1~8 */
	AUD_INTF_VOC_AEC_DRC,				/**< voice volume value, range:0x10~0x1f */
	AUD_INTF_VOC_AEC_MAX,
} aud_intf_voc_aec_para_t;

/* mic control */
typedef enum {
	AUD_INTF_VOC_MIC_OPEN = 0,		/**< open mic */
	AUD_INTF_VOC_MIC_CLOSE,			/**< close spk */
	AUD_INTF_VOC_MIC_MAX,
} aud_intf_voc_mic_ctrl_t;

/* speaker control */
typedef enum {
	AUD_INTF_VOC_SPK_OPEN = 0,		/**< open spk */
	AUD_INTF_VOC_SPK_CLOSE, 		/**< close spk */
	AUD_INTF_VOC_SPK_MAX,
} aud_intf_voc_spk_ctrl_t;

/* audio interface voice setup configuration */
typedef struct {
	bool aec_enable;						/**< true: enable aec, false: disable aec */
	aud_intf_voc_samp_rate_t samp_rate;		/**< voice transfer sample rate */
	aud_intf_voc_data_type_t data_type;		/**< voice data type */
	uint8_t mic_gain;						/**< mic gain: value range:0x0 ~ 0x3f, suggest:0x2d */
	uint8_t spk_gain;						/**< spk gain: value range:0x0 ~ 0x3f, suggest:0x2d */
	aud_dac_work_mode_t spk_mode;			/**< audio spk mode: signal_ended/differen */
	aud_intf_voc_aec_cfg_t aec_cfg;

	aud_intf_voc_mic_ctrl_t mic_en;			/**< mic default status */
	aud_intf_voc_spk_ctrl_t spk_en;			/**< spk default status */
	aud_intf_mic_type_t mic_type;			/**< audio mic type: uac or microphone */
	aud_intf_spk_type_t spk_type;			/**< audio speaker type: uac or speaker */
} aud_intf_voc_setup_t;

#define DEFAULT_AUD_INTF_VOC_SETUP_CONFIG() {          \
        .aec_enable = true,                            \
        .samp_rate = AUD_INTF_VOC_SAMP_RATE_8K,        \
        .data_type = AUD_INTF_VOC_DATA_TYPE_G711A,     \
        .mic_gain = 0x2d,                              \
        .spk_gain = 0x2d,                              \
        .spk_mode = AUD_DAC_WORK_MODE_DIFFEN,          \
        .aec_cfg = {                                   \
                       .ec_depth = 20,                 \
                       .TxRxThr = 30,                  \
                       .TxRxFlr = 6,                   \
                       .ref_scale = 0,                 \
                       .ns_level = 2,                  \
                       .ns_para = 1,                   \
                    },                                 \
        .mic_en = AUD_INTF_VOC_MIC_OPEN,               \
        .spk_en = AUD_INTF_VOC_SPK_OPEN,               \
        .mic_type = AUD_INTF_MIC_TYPE_BOARD,           \
        .spk_type = AUD_INTF_SPK_TYPE_BOARD,           \
    }

#ifdef __cplusplus
}
#endif

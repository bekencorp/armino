#pragma once

#include "aud_intf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief AUD INTF API
 * @defgroup bk_aud_intf_api API group
 * @{
 */


/**
 * @brief     Init the AUD INTF driver
 *
 * This API init the resoure common:
 *   - Creat AUD INTF task
 *   - Configure default work mode
 *   - Register read mic data and write speaker data callback
 *
 * This API should be called before any other AUD INTF APIs.
 *
 * @param setup audio interface driver init configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_drv_init(aud_intf_drv_setup_t *setup);

/**
 * @brief     Deinit the AUD INTF driver
 *
 * This API deinit the resoure common:
 *   - Delete AUD INTF task
 *   - Reset default work mode
 *   - Unregister read mic data and write speaker data callback
 *
 * This API should be called when audio is not needed.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_drv_deinit(void);

/**
 * @brief     Get the AUD INTF work mode
 *
 * @param work_mode save audio interface work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_get_work_mode(aud_intf_work_mode_t *work_mode);

/**
 * @brief     Set the AUD INTF work mode
 *
 * @param work_mode audio interface work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_mode(aud_intf_work_mode_t work_mode);

/**
 * @brief     Set the mic gain
 *
 * @param value the gain value range:0x00 ~ 0x3f
 *
 * This API should be called when mic has been initialized
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_mic_gain(uint8_t value);

/**
 * @brief     Set the mic gain
 *
 * @param value the gain value range:0x00 ~ 0x3f
 *
 * This API should be called when speaker has been initialized
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_spk_gain(uint8_t value);

/**
 * @brief     Get the AUD INTF information
 *
 * @param info save audio interface information
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
//bk_err_t bk_aud_intf_get_info(aud_intf_info_t *info);

/*************************************** mic api *********************************************/
/**
 * @brief     Init the AUD INTF mic
 *
 * This API init the resoure common:
 *   - Init AUD adc driver
 *   - Configure AUD adc paramter
 *   - Configure DMA to carry adc data
 *
 * This API should be called after bk_aud_intf_drv_init.
 *
 * @param mic_setup audio adc setup configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_mic_init(aud_intf_mic_setup_t *mic_setup);

/**
 * @brief     Deinit the AUD INTF mic
 *
 * This API deinit the resoure common:
 *   - Deinit AUD adc driver
 *   - Reset AUD adc paramter
 *   - Close DMA
 *
 * This API should be called after bk_aud_intf_mic_stop.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_mic_deinit(void);

/**
 * @brief     Start the AUD INTF mic work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_mic_start(void);

/**
 * @brief     Pause the AUD INTF mic work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_mic_pause(void);

/**
 * @brief     Stop the AUD INTF mic work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_mic_stop(void);

/**
 * @brief     Set the AUD INTF mic channel
 *
 * @param mic_chl audio interface mic channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_mic_chl(aud_intf_mic_chl_t mic_chl);

/**
 * @brief     Get the AUD INTF mic channel
 *
 * @param mic_chl save audio interface mic channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_get_mic_chl(aud_intf_mic_chl_t *mic_chl);

/**
 * @brief     Set the AUD INTF mic sample rate
 *
 * @param samp_rate audio interface mic sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_mic_samp_rate(aud_adc_samp_rate_t samp_rate);

/**
 * @brief     Get the AUD INTF mic sample rate
 *
 * @param samp_rate save audio interface mic sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_get_mic_samp_rate(aud_adc_samp_rate_t *samp_rate);



/*************************************** speaker api *********************************************/
/**
 * @brief     Init the AUD INTF speaker
 *
 * This API init the resoure common:
 *   - Init AUD dac driver
 *   - Configure AUD dac paramter
 *   - Configure DMA to carry dac data
 *
 * This API should be called after bk_aud_intf_drv_init.
 *
 * @param spk_setup audio dac setup configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_spk_init(aud_intf_spk_setup_t *spk_setup);

/**
 * @brief     Deinit the AUD INTF speaker
 *
 * This API deinit the resoure common:
 *   - Deinit AUD dac driver
 *   - Reset AUD dac paramter
 *   - Close DMA
 *
 * This API should be called after bk_aud_intf_spk_stop.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_spk_deinit(void);

/**
 * @brief     Start the AUD INTF speaker work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_spk_start(void);

/**
 * @brief     Pause the AUD INTF speaker work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_spk_pause(void);

/**
 * @brief     Stop the AUD INTF speaker work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_spk_stop(void);

/**
 * @brief     Set the AUD INTF speaker channel
 *
 * @param spk_chl audio interface speaker channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_spk_chl(aud_intf_spk_chl_t spk_chl);

/**
 * @brief     Get the AUD INTF speaker channel
 *
 * @param spk_chl save audio interface speaker channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_get_spk_chl(aud_intf_mic_chl_t *spk_chl);

/**
 * @brief     Set the AUD INTF speaker sample rate
 *
 * @param samp_rate audio interface speaker sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_spk_samp_rate(aud_dac_samp_rate_t samp_rate);

/**
 * @brief     Get the AUD INTF speaker sample rate
 *
 * @param samp_rate save audio interface speaker sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_get_spk_samp_rate(aud_dac_samp_rate_t *samp_rate);


/*************************************** voice api *********************************************/
/**
 * @brief     Init the AUD INTF voice
 *
 * This API init the resoure common:
 *   - Init AUD voice transfer driver
 *   - Configure AUD dac and adc paramter
 *   - Configure DMA to carry adc and dac data
 *
 * This API should be called after bk_aud_intf_drv_init.
 *
 * @param voc_setup audio voice setup configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_init(aud_intf_voc_setup_t setup);

/**
 * @brief     Deinit the AUD INTF voice
 *
 * This API deinit the resoure common:
 *   - Deinit AUD voice transfer driver
 *   - Reset AUD adc and dac paramter
 *   - Close DMA
 *
 * This API should be called after bk_aud_intf_voice_stop.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_deinit(void);

/**
 * @brief     Start the AUD INTF voice work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_start(void);

/**
 * @brief     Stop the AUD INTF voice work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_stop(void);

/**
 * @brief     Write the speaker data to audio dac
 *
 * @param dac_buff the address of speaker data written
 * @param size the data size (byte)
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_write_spk_data(uint8_t *dac_buff, uint32_t size);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#pragma once

#include <components/audio_transfer_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the audio transfer module
 *
 * This API init the resoure common:
 *   - Init audio transfer driver
 *   - Configure audio dac and adc paramter
 *   - Configure DMA to carry adc and dac data
 *   - Configure AEC
 *
 * @param setup audio transfer setup configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t audio_tras_init(audio_tras_setup_t setup);

/**
 * @brief     Deinit the audio transfer module
 *
 * This API init the resoure common:
 *   - Deinit audio transfer driver
 *   - Reset audio dac and adc paramter
 *   - Free DMA
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t audio_tras_deinit(void);

/**
 * @brief     Write speaker data received by wifi to audio dac
 *
 * @param dac_buff speaker data address
 * @param len the size of speaker data(byte)
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t audio_tras_write_spk_data(uint8_t *dac_buff, uint32_t len);

#ifdef __cplusplus
}
#endif


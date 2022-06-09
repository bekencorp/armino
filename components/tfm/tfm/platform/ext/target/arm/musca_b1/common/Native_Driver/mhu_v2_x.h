/*
 * Copyright (c) 2020 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file mhu_v2_x.h
 * \brief Driver for Arm MHU v2.0 and v2.1
 */

#ifndef __MHU_V2_X_H__
#define __MHU_V2_X_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MHU_2_X_INTR_NR2R_OFF             (0x0u)
#define MHU_2_X_INTR_R2NR_OFF             (0x1u)
#define MHU_2_1_INTR_CHCOMB_OFF           (0x2u)

#define MHU_2_X_INTR_NR2R_MASK            (0x1u << MHU_2_X_INTR_NR2R_OFF)
#define MHU_2_X_INTR_R2NR_MASK            (0x1u << MHU_2_X_INTR_R2NR_OFF)
#define MHU_2_1_INTR_CHCOMB_MASK          (0x1u << MHU_2_1_INTR_CHCOMB_OFF)

enum mhu_v2_x_frame_t {
    MHU_V2_X_SENDER_FRAME   = 0x0u,
    MHU_V2_X_RECEIVER_FRAME = 0x1u,
};

enum mhu_v2_x_supported_revisions {
     MHU_REV_READ_FROM_HW = 0,
     MHU_REV_2_0,
     MHU_REV_2_1,
};

struct mhu_v2_x_dev_t {
    const uint32_t base;
    enum mhu_v2_x_frame_t frame;
    uint32_t subversion;    /*!< Hardware subversion: v2.X */
    bool is_initialized;    /*!< Indicates if the MHU driver
                             *   is initialized and enabled
                             */
};

/**
 * \brief MHU v2 error enumeration types.
 */
enum mhu_v2_x_error_t {
    MHU_V_2_X_ERR_NONE                =  0,
    MHU_V_2_X_ERR_NOT_INIT            = -1,
    MHU_V_2_X_ERR_ALREADY_INIT        = -2,
    MHU_V_2_X_ERR_UNSUPPORTED_VERSION = -3,
    MHU_V_2_X_ERR_INVALID_ARG         = -4,
    MHU_V_2_X_ERR_GENERAL             = -5
};

/**
 * \brief Initializes the driver
 *
 * \param[in] dev   MHU device struct \ref mhu_v2_x_dev_t
 * \param[in] rev   MHU revision (if can't be identified from HW)
 *
 * Reads the MHU hardware version
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note MHU revision only has to be specified when versions can't be read
 *       from HW (ARCH_MAJOR_REV reg reads as 0x0).
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mhu_v2_x_error_t mhu_v2_x_driver_init(struct mhu_v2_x_dev_t *dev,
     enum mhu_v2_x_supported_revisions rev);

/**
 * \brief Returns the number of channels implemented.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 *
 * Returns the number of channels implemented.
 *
 * \return Returns the number of channels implemented.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t mhu_v2_x_get_num_channel_implemented(
         const struct mhu_v2_x_dev_t *dev);

/**
 * \brief Sends the value over a channel.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[in] channel     Channel to send the value over.
 * \param[in] val         Value to send.
 *
 * Sends the value over a channel.
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 * \note This function doesn't check if channel is implemented.
 */
enum mhu_v2_x_error_t mhu_v2_x_channel_send(const struct mhu_v2_x_dev_t *dev,
     uint32_t channel, uint32_t val);

/**
 * \brief Clears the channel after the value is send over it.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[in] channel     Channel to clear.
 *
 * Clears the channel after the value is send over it.
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 * \note This function doesn't check if channel is implemented.
 */
enum mhu_v2_x_error_t mhu_v2_x_channel_clear(const struct mhu_v2_x_dev_t *dev,
     uint32_t channel);

/**
 * \brief Receives the value over a channel.
 *
 * \param[in]  dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[in]  channel     Channel to receive the value from.
 * \param[out] value       Pointer to variable that will store the value.
 *
 * Receives the value over a channel.
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 * \note This function doesn't check if channel is implemented.
 */
enum mhu_v2_x_error_t mhu_v2_x_channel_receive(
     const struct mhu_v2_x_dev_t *dev, uint32_t channel, uint32_t *value);

/**
 * \brief Sets bits in the Channel Mask.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[in] channel     Which channel's mask to set.
 * \param[in] mask        Mask to be set over a receiver frame.
 *
 * Sets bits in the Channel Mask.
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 * \note This function doesn't check if channel is implemented.
 */
enum mhu_v2_x_error_t mhu_v2_x_channel_mask_set(
     const struct mhu_v2_x_dev_t *dev, uint32_t channel, uint32_t mask);

/**
 * \brief Clears bits in the Channel Mask.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[in] channel     Which channel's mask to clear.
 * \param[in] mask        Mask to be clear over a receiver frame.
 *
 * Clears bits in the Channel Mask.
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 * \note This function doesn't check if channel is implemented.
 */
enum mhu_v2_x_error_t mhu_v2_x_channel_mask_clear(
     const struct mhu_v2_x_dev_t *dev, uint32_t channel, uint32_t mask);

/**
 * \brief Enables the Channel interrupt.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[in] channel     Which channel's interrupt to enable.
 *
 * Enables the Channel clear interrupt.
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 * \note This function doesn't check if channel is implemented.
 */
enum mhu_v2_x_error_t mhu_v2_x_channel_interrupt_enable(
     const struct mhu_v2_x_dev_t *dev, uint32_t channel);

/**
 * \brief Disables the Channel interrupt.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[in] channel     Which channel's interrupt to disable.
 *
 * Disables the Channel interrupt.
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 * \note This function doesn't check if channel is implemented.
 */
enum mhu_v2_x_error_t mhu_v2_x_channel_interrupt_disable(
     const struct mhu_v2_x_dev_t *dev, uint32_t channel);

/**
 * \brief Cleares the Channel interrupt.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[in] channel     Which channel's interrupt to clear.
 *
 * Cleares the Channel interrupt.
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 * \note This function doesn't check if channel is implemented.
 */
enum mhu_v2_x_error_t mhu_v2_x_channel_interrupt_clear(
     const struct mhu_v2_x_dev_t *dev, uint32_t channel);

/**
 * \brief Initiates a MHU transfer with the handshake signals.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 *
 * Initiates a MHU transfer with the handshake signals in a blocking mode.
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mhu_v2_x_error_t mhu_v2_x_initiate_transfer(
     const struct mhu_v2_x_dev_t *dev);

/**
 * \brief Closes a MHU transfer with the handshake signals.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 *
 * Closes a MHU transfer with the handshake signals in a blocking mode.
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mhu_v2_x_error_t mhu_v2_x_close_transfer(
     const struct mhu_v2_x_dev_t *dev);

/**
 * \brief Returns the value of access request signal.
 *
 * \param[in]  dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[out] val         Pointer to variable that will store the value.
 *
 * For more information please read the MHU v2 user guide
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mhu_v2_x_error_t mhu_v2_x_get_access_request(
     const struct mhu_v2_x_dev_t *dev, uint32_t *val);

/**
 * \brief Sets the value of access request signal to high.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 *
 * For more information please read the MHU v2 user guide
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mhu_v2_x_error_t mhu_v2_x_set_access_request(
     const struct mhu_v2_x_dev_t *dev);

/**
 * \brief Sets the value of access request signal to low.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 *
 * For more information please read the MHU v2 user guide
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mhu_v2_x_error_t mhu_v2_x_reset_access_request(
     const struct mhu_v2_x_dev_t *dev);

/**
 * \brief Returns the value of access ready signal.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[out] val        Pointer to variable that will store the value.
 *
 * For more information please read the MHU v2 user guide
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mhu_v2_x_error_t mhu_v2_x_get_access_ready(
     const struct mhu_v2_x_dev_t *dev, uint32_t *val);

/**
 * \brief Returns the MHU interrupt status.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 *
 * \return Interrupt status register value. Masking is needed for individual
 *         interrupts.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t mhu_v2_x_get_interrupt_status(const struct mhu_v2_x_dev_t *dev);

/**
 * \brief Enables MHU interrupts.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[in] mask        Bit mask for enabling/disabling interrupts
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mhu_v2_x_error_t mhu_v2_x_interrupt_enable(
     const struct mhu_v2_x_dev_t *dev, uint32_t mask);

/**
 * \brief Disables MHU interrupts.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[in] mask        Bit mask for enabling/disabling interrupts
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mhu_v2_x_error_t mhu_v2_x_interrupt_disable(
     const struct mhu_v2_x_dev_t *dev, uint32_t mask);

/**
 * \brief Clears MHU interrupts.
 *
 * \param[in] dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[in] mask        Bit mask for clearing interrupts
 *
 * \return Returns mhu_v2_x_error_t error code
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mhu_v2_x_error_t mhu_v2_x_interrupt_clear(
     const struct mhu_v2_x_dev_t *dev, uint32_t mask);

/**
 * \brief Returns the first channel number whose interrupt bit is high.
 *
 * \param[in]  dev         MHU device struct \ref mhu_v2_x_dev_t
 * \param[out] channel     Pointer to variable that will have the channel value.
 *
 * \return Returns the first channel number whose interrupt bit is high.
 * \return Returns mhu_v2_x_error_t error code.
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mhu_v2_x_error_t mhu_v2_1_get_ch_interrupt_num(
     const struct mhu_v2_x_dev_t *dev, uint32_t *channel);

#ifdef __cplusplus
}
#endif

#endif /* __MHU_V2_X_H__ */

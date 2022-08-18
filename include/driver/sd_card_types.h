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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t card_version;       /**< the card version */
	uint32_t card_type;          /**< the card version(SDSC/SDHC/SDXC) */
	uint32_t class;              /**< the class of card class */
	uint32_t relative_card_addr; /**< the relaive card address */
} sd_card_info_t;

/**
 * @brief current state of card status,  refer to Card Status BIT[12:9]
 */
typedef enum {
	SD_CARD_IDLE = 0,       /**< card state is idle */
	SD_CARD_READY,          /**< card state is ready */
	SD_CARD_IDENTIFICATION, /**< card is in identification state */
	SD_CARD_STANDBY,        /**< card is in standby state */
	SD_CARD_TRANSFER,       /**< card is in transfer state */
	SD_CARD_SENDING,        /**< card is sending an operation */
	SD_CARD_RECEIVING,      /**< card is receiving operation information */
	SD_CARD_PROGRAMMING,    /**< card is in programming state */
	SD_CARD_DISCONNECTED,   /**< card is disconnected */
	SD_CARD_ERROR = 0xff,
} sd_card_state_t;

#ifdef __cplusplus
}
#endif


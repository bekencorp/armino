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

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ble_err_t bk_err_t


/// Status Return Value
typedef enum {
    BK_BT_STATUS_SUCCESS       = 0,
} bk_bt_status_t;


/// UUID type
typedef struct {
#define BK_UUID_LEN_16     2
#define BK_UUID_LEN_32     4
#define BK_UUID_LEN_128    16
    uint16_t len;							/*!< UUID length, 16bit, 32bit or 128bit */
    union {
        uint16_t    uuid16;                 /*!< 16bit UUID */
        uint32_t    uuid32;                 /*!< 32bit UUID */
        uint8_t     uuid128[BK_UUID_LEN_128]; /*!< 128bit UUID */
    } uuid;									/*!< UUID */
} __attribute__((packed)) bk_bt_uuid_t;


/// Bluetooth address length
#define BK_BD_ADDR_LEN     6

/// Bluetooth device address
typedef uint8_t bk_bd_addr_t[BK_BD_ADDR_LEN];


#ifdef __cplusplus
}
#endif


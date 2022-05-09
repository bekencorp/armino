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

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
    //REG_0x00~REG_0x2F
    struct {
        union {
            struct {
                uint32_t enable:             1; /**< bit[0] DMA enable */
                uint32_t finish_int_en:      1; /**< bit[1] transfer finish interrupt enable */
                uint32_t half_finish_int_en: 1; /**< bit[2] half transfer finish interrupt enable */
                uint32_t mode:               1; /**< bit[3] DMA work mode */
                uint32_t src_data_width:     2; /**< bit[4:5] source data width */
                uint32_t dest_data_width:    2; /**< bit[6:7] destination data width */
                uint32_t src_addr_inc_en:    1; /**< bit[8] source address increment enable */
                uint32_t dest_addr_inc_en:   1; /**< bit[9] dest address increment enable */
                uint32_t src_addr_loop_en:   1; /**< bit[10] source address loop enable */
                uint32_t dest_addr_loop_en:  1; /**< bit[11] dest address loop enable */
                uint32_t chan_prio:          3; /**< bit[12:14] channel prioprity */
                uint32_t reserved:           1; /**< bit[15] */
                uint32_t transfer_len:       16; /**< bit[16:31] DMA transfer length,unit is byte */
            };
            uint32_t v;
        } ctrl;

        uint32_t dest_start_addr;
        uint32_t src_start_addr;
        uint32_t dest_loop_end_addr;
        uint32_t dest_loop_start_addr;
        uint32_t src_loop_end_addr;
        uint32_t src_loop_start_addr;

        union {
            struct {
                /* Source Request Mux
                 * 0x0: DTCM Read Req
                 * 0x1: LA Read Req
                 * 0x2: reserved
                 * 0x3: SDIO Host Rx Req
                 * 0x4: Uart1 Rx Req
                 * 0x5: Uart2 Rx Req
                 * 0x6: reserved
                 * 0x7: GSPI Rx Req
                 * 0x8~0xf: reserved
                 */
                uint32_t src_req_mux:         4; /**< bit[0:3] source request mux */

                /* Destination Request Mux:
                 * 0x0: DTCM Write Req
                 * 0x1: reserved
                 * 0x2: reserved
                 * 0x3: SDIO Host Tx Req
                 * 0x4: Uart1 Tx Req
                 * 0x5: Uart2 Tx Req
                 * 0x6: reserved
                 * 0x7: GSPI Tx Req
                 * 0x8~0xf: reserved
                 */
                uint32_t dest_req_mux:        4; /**< bit[4:7] dest request mux */
                uint32_t reserved0:           4; /**< bit[8:11] */
                uint32_t src_read_interval:   4; /**< bit[12:15] source read operate interval,unit is cycle */
                uint32_t dest_write_interval: 4; /**< bit[16:19] destination write operate interval,unit is cycle */
                uint32_t reserved1:           12; /**< bit[20:31] */
            };
            uint32_t v;
        } req_mux;
    } config_group[SOC_DMA_CHAN_NUM_PER_UNIT];

    //REG_0x30~REG_0x35
    struct {
        union {
            struct {
                uint32_t remain_len:              17; /**< bit[0:16] remain length indication */
                uint32_t flush_src_buff:          1; /**< bit[17] flush source buffer */
                uint32_t reserved:                6; /**< bit[18:23] */
                uint32_t finish_int_counter:      4; /**< bit[24:27] finish interrupt counter */
                uint32_t half_finish_int_counter: 4; /**< bit[28:31] half finish interrupt counter*/
            };
            uint32_t v;
        } status;
    } status_group[SOC_DMA_CHAN_NUM_PER_UNIT];

    uint32_t reserved0;

    //REG_0x37
    union {
        struct {
            uint32_t prio_mode: 1; /**< bit[0] prioprity mode */
            uint32_t reserved:  30; /**< bit[1:31] reserved */
        };
        uint32_t v;
    } prio_mode;

    //REG_0x38
    union {
        struct {
            uint32_t finish_int_status:      6; /**< bit[0:5] finish interrupt status */
            uint32_t reserved0:              2; /**< bit[6:7] */
            uint32_t half_finish_int_status: 6; /**< bit[8:13] half finish interrupt status */
            uint32_t reserved1:              18; /**< bit[14:31] */
        };
        uint32_t v;
    } int_status;

    uint32_t reserved1[0x40 - 0x38 - 1];
    //REG_0x40~REG_0x45
    uint32_t src_pause_addr[SOC_DMA_CHAN_NUM_PER_UNIT];
    uint32_t reserved2[2];
    //REG_0x48~REG_0x4D
    uint32_t dest_pause_addr[SOC_DMA_CHAN_NUM_PER_UNIT];
    uint32_t reserved3[2];
    //REG_0x50~REG_0x55
    uint32_t src_rd_addr[SOC_DMA_CHAN_NUM_PER_UNIT];
    uint32_t reserved4[2];
    //REG_0x58~REG_0x5D
    uint32_t dest_wr_addr[SOC_DMA_CHAN_NUM_PER_UNIT];
} dma_hw_t;

#ifdef __cplusplus
}
#endif


// Copyright 2022-2023 Beken
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



#ifndef _SDIO_TEST_H_
#define _SDIO_TEST_H_

#define SDIO_TEST_PRIORITY (27)
#define SDIO_TEST_THREAD_NAME "sdio_test"
#define SDIO_TEST_THREAD_STACK_SIZE (0x1<<10)	//Bytes

#define SDIO_TEST_MSG_QUEUE_NAME "sdio_test_queue"
#define SDIO_TEST_MSG_QUEUE_COUNT (8)

#define SDIO_TEST_BUFFER_CNT (4)
#define SDIO_TEST_BUFFER_SIZE (512)

typedef struct {
	uint8_t main_case;
	uint8_t sub_case;
	uint16_t reserved;
	uint32_t param1;
	uint32_t param2;
	uint32_t param3;
	uint32_t param4;
} SDIO_TEST_MSG_T;

typedef enum
{
	SDIO_TEST_TX,
	SDIO_TEST_RX,
	SDIO_TEST_TX_THROUGH_PUT,
	SDIO_TEST_RX_THROUGH_PUT,
	SDIO_TEST_MAX_MAIN_CASE,
}SDIO_TEST_MAIN_CASE_ID_T;

typedef enum
{
	SDIO_TEST_TX_INIT,
	SDIO_TEST_TX_SINGLE_PACKET,
	SDIO_TEST_TX_MANY_SINGLE_PACKETS,
	SDIO_TEST_TX_MULTI_PACKETS,
	SDIO_TEST_TX_MAX_SUB_CASE,
}SDIO_TEST_TX_CASE_ID_T;

typedef enum
{
	SDIO_TEST_RX_INIT,
	SDIO_TEST_RX_SINGLE_PACKET,
	SDIO_TEST_RX_MANY_SINGLE_PACKETS,
	SDIO_TEST_RX_MULTI_PACKETS,
	SDIO_TEST_RX_MAX_SUB_CASE,
}SDIO_TEST_RX_CASE_ID_T;

/**
 * @brief	Init the sdio driver test program.
 *
 * This API init the sdio driver test program.
 *	 - It will create a task do selected test case.
 *	 - It can select test case by command line.
 *
 * @attention 1. This API should be called after sdio inited finish.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_test_init(void);

/**
 * @brief	Deinit the sdio driver test program.
 *
 * This API init the sdio driver test program.
 *	 - It will delete the test thread and queue.
 *
 * @attention 1. This API should be called after sdio inited finish.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_test_deinit(void);

#endif // _SDIO_SLAVE_TEST_H_

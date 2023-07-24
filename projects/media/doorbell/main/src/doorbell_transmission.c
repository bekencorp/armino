#include <common/bk_include.h>

#include "doorbell_comm.h"
#include "doorbell_transmission.h"

#include "lwip/tcp.h"
#include "bk_uart.h"
#include <os/mem.h>
#include <os/os.h>
#include <common/bk_kernel_err.h>

#include "lwip/sockets.h"

#include <components/video_transfer.h>

#if CONFIG_ARCH_CM33
#include <driver/aon_rtc.h>
#endif

#define TAG "db-tran"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

//#define DEBUG_DUMP
//#define DEBUG_HEAD
//#define DEBUG_CRC


#define CRC8_INIT_VALUE 0xFF

/*******************************************************************************
* crc8
*
* Computes a crc8 over the input data using the polynomial:
*
*       x^8 + x^7 +x^6 + x^4 + x^2 + 1
*
* The caller provides the initial value (either CRC8_INIT_VALUE
* or the previous returned value) to allow for processing of
* discontiguous blocks of data.  When generating the CRC the
* caller is responsible for complementing the final return value
* and inserting it into the byte stream.  When checking, a final
* return value of CRC8_GOOD_VALUE indicates a valid CRC.
*
* Reference: Dallas Semiconductor Application Note 27
*   Williams, Ross N., "A Painless Guide to CRC Error Detection Algorithms",
*     ver 3, Aug 1993, ross@guest.adelaide.edu.au, Rocksoft Pty Ltd.,
*     ftp://ftp.rocksoft.com/clients/rocksoft/papers/crc_v3.txt
*
* ****************************************************************************
*/


static const uint8 crc8_table[256] =
{
	0x00, 0xF7, 0xB9, 0x4E, 0x25, 0xD2, 0x9C, 0x6B,
	0x4A, 0xBD, 0xF3, 0x04, 0x6F, 0x98, 0xD6, 0x21,
	0x94, 0x63, 0x2D, 0xDA, 0xB1, 0x46, 0x08, 0xFF,
	0xDE, 0x29, 0x67, 0x90, 0xFB, 0x0C, 0x42, 0xB5,
	0x7F, 0x88, 0xC6, 0x31, 0x5A, 0xAD, 0xE3, 0x14,
	0x35, 0xC2, 0x8C, 0x7B, 0x10, 0xE7, 0xA9, 0x5E,
	0xEB, 0x1C, 0x52, 0xA5, 0xCE, 0x39, 0x77, 0x80,
	0xA1, 0x56, 0x18, 0xEF, 0x84, 0x73, 0x3D, 0xCA,
	0xFE, 0x09, 0x47, 0xB0, 0xDB, 0x2C, 0x62, 0x95,
	0xB4, 0x43, 0x0D, 0xFA, 0x91, 0x66, 0x28, 0xDF,
	0x6A, 0x9D, 0xD3, 0x24, 0x4F, 0xB8, 0xF6, 0x01,
	0x20, 0xD7, 0x99, 0x6E, 0x05, 0xF2, 0xBC, 0x4B,
	0x81, 0x76, 0x38, 0xCF, 0xA4, 0x53, 0x1D, 0xEA,
	0xCB, 0x3C, 0x72, 0x85, 0xEE, 0x19, 0x57, 0xA0,
	0x15, 0xE2, 0xAC, 0x5B, 0x30, 0xC7, 0x89, 0x7E,
	0x5F, 0xA8, 0xE6, 0x11, 0x7A, 0x8D, 0xC3, 0x34,
	0xAB, 0x5C, 0x12, 0xE5, 0x8E, 0x79, 0x37, 0xC0,
	0xE1, 0x16, 0x58, 0xAF, 0xC4, 0x33, 0x7D, 0x8A,
	0x3F, 0xC8, 0x86, 0x71, 0x1A, 0xED, 0xA3, 0x54,
	0x75, 0x82, 0xCC, 0x3B, 0x50, 0xA7, 0xE9, 0x1E,
	0xD4, 0x23, 0x6D, 0x9A, 0xF1, 0x06, 0x48, 0xBF,
	0x9E, 0x69, 0x27, 0xD0, 0xBB, 0x4C, 0x02, 0xF5,
	0x40, 0xB7, 0xF9, 0x0E, 0x65, 0x92, 0xDC, 0x2B,
	0x0A, 0xFD, 0xB3, 0x44, 0x2F, 0xD8, 0x96, 0x61,
	0x55, 0xA2, 0xEC, 0x1B, 0x70, 0x87, 0xC9, 0x3E,
	0x1F, 0xE8, 0xA6, 0x51, 0x3A, 0xCD, 0x83, 0x74,
	0xC1, 0x36, 0x78, 0x8F, 0xE4, 0x13, 0x5D, 0xAA,
	0x8B, 0x7C, 0x32, 0xC5, 0xAE, 0x59, 0x17, 0xE0,
	0x2A, 0xDD, 0x93, 0x64, 0x0F, 0xF8, 0xB6, 0x41,
	0x60, 0x97, 0xD9, 0x2E, 0x45, 0xB2, 0xFC, 0x0B,
	0xBE, 0x49, 0x07, 0xF0, 0x9B, 0x6C, 0x22, 0xD5,
	0xF4, 0x03, 0x4D, 0xBA, 0xD1, 0x26, 0x68, 0x9F
};

#define CRC_INNER_LOOP(n, c, x) \
	(c) = ((c) >> 8) ^ crc##n##_table[((c) ^ (x)) & 0xff]


uint8 hnd_crc8(
    uint8 *pdata,   /* pointer to array of data to process */
    uint  nbytes,   /* number of input data bytes to process */
    uint8 crc   /* either CRC8_INIT_VALUE or previous return value */
)
{
	/* hard code the crc loop instead of using CRC_INNER_LOOP macro
	 * to avoid the undefined and unnecessary (uint8 >> 8) operation.
	 */
	while (nbytes-- > 0)
	{
		crc = crc8_table[(crc ^ *pdata++) & 0xff];
	}

	return crc;
}

void doorbell_hex_dump(uint8_t *data, uint32_t length)
{
	for (int i = 0; i < length; i++)
	{
		os_printf("%02X ", data[i]);

		if ((i + 1) % 20 == 0)
		{
			os_printf("\n");
		}
	}
	os_printf("\n");
}

db_channel_t *doorbell_transmission_malloc(uint16_t max_rx_size, uint16_t max_tx_size)
{
	db_channel_t *db_channel = (db_channel_t *)os_malloc(sizeof(db_channel_t));

	if (db_channel == NULL)
	{
		LOGE("malloc db_channel failed\n");
		goto error;
	}

	os_memset(db_channel, 0, sizeof(db_channel_t));

	db_channel->cbuf = os_malloc(max_rx_size);

	if (db_channel->cbuf == NULL)
	{
		LOGE("malloc cache buffer failed\n");
		goto error;
	}

	db_channel->csize = max_rx_size;

	db_channel->tbuf = os_malloc(max_tx_size + sizeof(db_trans_head_t));

	if (db_channel->tbuf == NULL)
	{
		LOGE("malloc cache buffer failed\n");
		goto error;
	}

	db_channel->tsize = max_tx_size;

	LOGI("%s, %p, %p %d, %p %d\n", __func__, db_channel, db_channel->cbuf, db_channel->csize, db_channel->tbuf, db_channel->tsize);

	return db_channel;


error:

	if (db_channel->cbuf)
	{
		os_free(db_channel->cbuf);
		db_channel->cbuf = NULL;
	}

	if (db_channel)
	{
		os_free(db_channel);
		db_channel = NULL;
	}

	return db_channel;
}

void doorbell_transmission_unpack(db_channel_t *channel, uint8_t *data, uint32_t length, doorbell_transmission_recive_cb_t cb)
{
	db_trans_head_t head, *ptr;
	uint8_t *p = data;
	uint32_t left = length;

#ifdef DUMP_DEBUG
	static uint32_t count = 0;

	LOGD("DUMP DATA %u, size: %u\n", count++, length);

	doorbell_hex_dump(data, length);
#endif

	while (left != 0)
	{
		if (channel->ccount == 0)
		{
			if (left < HEAD_SIZE_TOTAL)
			{
				os_memcpy(channel->cbuf + channel->ccount, p, left);
				channel->ccount += left;
				break;
			}

			ptr = (db_trans_head_t *)p;

			head.magic = CHECK_ENDIAN_UINT16(ptr->magic);

			if (head.magic == HEAD_MAGIC_CODE)
			{
				/*
				*   Magic code  2 bytes
				*   Flags       2 bytes
				*   Timestamp   4 bytes
				*   Squence     2 bytes
				*   Length      2 bytes
				*   CRC         1 byte
				*   RESERVED    3 byte
				*/

				head.flags = CHECK_ENDIAN_UINT16(ptr->flags);
				head.timestamp = CHECK_ENDIAN_UINT16(ptr->timestamp);
				head.sequence = CHECK_ENDIAN_UINT16(ptr->sequence);
				head.length = CHECK_ENDIAN_UINT16(ptr->length);
				head.crc = ptr->crc;
				head.reserved[0] = ptr->reserved[0];
				head.reserved[1] = ptr->reserved[1];
				head.reserved[2] = ptr->reserved[2];
#ifdef DEBUG_HEAD
				LOGI("head size: %d, %d\n", HEAD_SIZE_TOTAL, sizeof(db_trans_head_t));
				LOGI("magic code: %08X\n", head.magic);
				LOGI("flags: %04X\n", head.flags);
				LOGI("timestamp: %u\n", head.timestamp);
				LOGI("sequence: %04X\n", head.sequence);
				LOGI("crc: %02X\n", head.crc);
#endif
			}
			else
			{
				LOGE("invaild src data\n");
				doorbell_hex_dump(p, left);
				LOGE("dump src data\n");
				//TODO FIXME
				doorbell_hex_dump(data, length);
				break;
			}

			if (left < head.length + HEAD_SIZE_TOTAL)
			{
				os_memcpy(channel->cbuf + channel->ccount, p, left);
				channel->ccount += left;
				break;
			}

#ifdef DEBUG_CRC
			if (HEAD_FLAGS_CRC & head.flags)
			{

				uint8_t ret_crc = hnd_crc8(p + HEAD_SIZE_TOTAL, head.length, CRC8_INIT_VALUE);

				if (ret_crc != head.crc)
				{
					LOGI("check crc failed\n");
				}

				LOGI("CRC SRC: %02X,  CALC: %02X\n", head.crc, ret_crc);
			}
#endif

			if (cb)
			{
				cb(channel, head.sequence, head.flags, head.timestamp, head.sequence, ptr->payload, head.length);
			}

			p += HEAD_SIZE_TOTAL + head.length;
			left -= HEAD_SIZE_TOTAL + head.length;
		}
		else
		{
			if (channel->ccount < HEAD_SIZE_TOTAL)
			{
				if (left < HEAD_SIZE_TOTAL)
				{
					os_memcpy(channel->cbuf + channel->ccount, p, left);
					channel->ccount += left;
					left = 0;
					break;
				}
				else
				{
					os_memcpy(channel->cbuf + channel->ccount, p, HEAD_SIZE_TOTAL - channel->ccount);
					channel->ccount += HEAD_SIZE_TOTAL - channel->ccount;
					p += HEAD_SIZE_TOTAL - channel->ccount;
					left -= HEAD_SIZE_TOTAL - channel->ccount;
				}
			}

			ptr = (db_trans_head_t *)channel->cbuf;

			head.magic = CHECK_ENDIAN_UINT32(ptr->magic);

			if (head.magic == HEAD_MAGIC_CODE)
			{
				/*
				*   Magic code  2 bytes
				*   Flags       2 bytes
				*   Timestamp   4 bytes
				*   Squence     2 bytes
				*   Length      2 bytes
				*   CRC         1 byte
				*   RESERVED    3 byte
				*/

				head.flags = CHECK_ENDIAN_UINT16(ptr->flags);
				head.timestamp = CHECK_ENDIAN_UINT16(ptr->timestamp);
				head.sequence = CHECK_ENDIAN_UINT16(ptr->sequence);
				head.length = CHECK_ENDIAN_UINT16(ptr->length);
				head.crc = ptr->crc;
				head.reserved[0] = ptr->reserved[0];
				head.reserved[1] = ptr->reserved[1];
				head.reserved[2] = ptr->reserved[2];

#ifdef DEBUG_HEAD
				LOGI("head size: %d, %d\n", HEAD_SIZE_TOTAL, sizeof(db_trans_head_t));
				LOGI("magic code: %08X\n", head.magic);
				LOGI("flags: %04X\n", head.flags);
				LOGI("timestamp: %u\n", head.timestamp);
				LOGI("sequence: %04X\n", head.sequence);
				LOGI("crc: %02X\n", head.crc);
#endif
			}
			else
			{
				LOGE("invaild cached data, %04X\n", head.magic);
				doorbell_hex_dump(channel->cbuf, channel->ccount);
				//TODO FIXME
				break;
			}

			if (channel->ccount < HEAD_SIZE_TOTAL + head.length)
			{
				if (left + channel->ccount < head.length + HEAD_SIZE_TOTAL)
				{
					os_memcpy(channel->cbuf + channel->ccount, p, left);
					channel->ccount += left;
					left = 0;
					break;
				}

				os_memcpy(channel->cbuf + channel->ccount, p, head.length + HEAD_SIZE_TOTAL - channel->ccount);
				left -= head.length + HEAD_SIZE_TOTAL - channel->ccount;
				p += head.length + HEAD_SIZE_TOTAL - channel->ccount;
				channel->ccount += head.length + HEAD_SIZE_TOTAL - channel->ccount;

#ifdef DEBUG_CRC
				if (HEAD_FLAGS_CRC & head.flags)
				{

					uint8_t ret_crc = hnd_crc8(channel->cbuf + HEAD_SIZE_TOTAL, head.length, CRC8_INIT_VALUE);

					if (ret_crc != head.crc)
					{
						LOGI("check crc failed\n");
					}

					LOGI("CRC SRC: %02X,  CALC: %02X\n", head.crc, ret_crc);
				}
#endif

				if (cb)
				{
					cb(channel, head.sequence, head.flags, head.timestamp, head.sequence, ptr->payload, head.length);
				}

				LOGD("cached: %d, left: %d\n", channel->ccount, left);

				channel->ccount = 0;
			}
			else
			{
				LOGE("invaild flow data\n");
				doorbell_hex_dump(channel->cbuf, channel->ccount);
				//SHOULD NOT BE HERE
				//TODO FIMXME
				break;
			}
		}
	}
}

uint32_t doorbell_transmission_get_milliseconds(void)
{
	uint32_t time = 0;

#if CONFIG_ARCH_RISCV
	extern u64 riscv_get_mtimer(void);

	time = (riscv_get_mtimer() / 26000) & 0xFFFFFFFF;
#elif CONFIG_ARCH_CM33

	time = (bk_aon_rtc_get_us() / 1000) & 0xFFFFFFFF;
#endif

	return time;
}

void doorbell_transmission_pack(db_channel_t *channel, uint8_t *data, uint32_t length)
{
	db_trans_head_t *head = channel->tbuf;


	/*
	*   Magic code  2 bytes
	*   Flags       2 bytes
	*   Timestamp   4 bytes
	*   Squence     2 bytes
	*   Length      2 bytes
	*   CRC         1 byte
	*   RESERVED    3 byte
	*/
	head->magic = CHECK_ENDIAN_UINT16(HEAD_MAGIC_CODE);
	head->flags = CHECK_ENDIAN_UINT16(HEAD_FLAGS_CRC);
	head->timestamp = CHECK_ENDIAN_UINT32(doorbell_transmission_get_milliseconds());
	head->sequence = CHECK_ENDIAN_UINT16(++channel->sequence);
	head->length = CHECK_ENDIAN_UINT16(length);
	head->crc = hnd_crc8(data, length, CRC8_INIT_VALUE);;
	head->reserved[0] = 0;
	head->reserved[1] = 0;
	head->reserved[2] = 0;

	os_memcpy(head->payload, data, length);

}

int doorbell_transmission_pack_send(db_channel_t *channel, uint8_t *data, uint32_t length, doorbell_transmission_send_t cb)
{
	bk_err_t ret = BK_FAIL;

	doorbell_transmission_pack(channel, data, length);

	if (channel->cb && channel->cb->tsend)
	{
		return channel->cb->tsend((uint8_t *)channel->tbuf, sizeof(db_trans_head_t) + length);
	}

	if (cb)
	{
		ret = cb((uint8_t *)channel->tbuf, sizeof(db_trans_head_t) + length);
	}

	return ret;
}


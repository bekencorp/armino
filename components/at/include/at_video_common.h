#ifndef AT_VIDEO_COMMON_H_
#define AT_VIDEO_COMMON_H_

#include "at_common.h"

typedef struct {
	/// the frame id
	uint8_t id;
	/// the flag of end frame, 1 for end
	uint8_t is_eof;
	/// the packet count of one frame
	uint8_t pkt_cnt;
	/// the packet header's count of one frame
	uint8_t pkt_seq;
}vbuf_header_t;

typedef struct {
	/// the video data receive complete
	beken_semaphore_t aready_semaphore;
	/// the receive video data, malloc by user
	uint8_t *buf_base;  // handler in usr thread
	/// video buff length, malloc by user
	uint32_t buf_len;
	/// frame id
	uint32_t frame_id;
	/// the packet count of one frame
	uint32_t frame_pkt_cnt;
	/// recoder the buff ptr of every time receive video packte
	uint8_t *buf_ptr;
	/// the length of receive one frame
	uint32_t frame_len;
	/// video buff receive state
	uint32_t receive_state;
}video_buff_t;

enum video_buff_state {
	/// video buff init
	VIDEO_BUFF_IDLE = 0,
	/// video buff begin copy
	VIDEO_BUFF_COPY,
	/// video buff full
	VIDEO_BUFF_FULL,
};

#endif


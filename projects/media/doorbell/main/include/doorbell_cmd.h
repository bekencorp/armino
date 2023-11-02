#ifndef __DOORBELL_CMD_H__
#define __DOORBELL_CMD_H__

#include "doorbell_transmission.h"
#include "lwip/inet.h"

#define EVT_STATUS_OK               (0)
#define EVT_STATUS_ERROR            (1)
#define EVT_STATUS_ALREADY          (2)
#define EVT_STATUS_NULL             (11)
#define EVT_STATUS_UNKNOWN          (12)


#define EVT_FLAGS_COMPLETE          (0 << 0)
#define EVT_FLAGS_CONTINUE          (1 << 0)


#define OPCODE_NOTIFICATION         (1 << 31)


/*
*   cmd
*/

typedef struct
{
	uint32_t  opcode;
	uint32_t  param;
	uint16_t  length;
	uint8_t  payload[];
} __attribute__((__packed__)) db_cmd_head_t;

typedef struct
{
	uint32_t  opcode;
	uint8_t  status;
	uint16_t  flags;
	uint16_t  length;
	uint8_t  payload[];
} __attribute__((__packed__)) db_evt_head_t;



void doorbell_command_handle(uint8_t sub_channel, uint8_t *data, uint16_t length);
void doorbell_cmd_server_init(void);
void doorbell_transmission_cmd_recive_callback(db_channel_t *channel, uint16_t sequence, uint16_t flags, uint32_t timestamp, uint8_t sequences, uint8_t *data, uint16_t length);

in_addr_t doorbell_cmd_get_socket_address(void);

#endif

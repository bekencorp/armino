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

/* IP Header */
typedef struct Ipv4Hdr {
    uint8_t   ver_hl;              /* version/header length */
    uint8_t   tos;                 /* type of service */
    uint16_t  length;              /* total length */
    uint16_t  id;                  /* identification */
    uint16_t  offset;              /* fragment offset field */
    uint8_t   ttl;                 /* time to live */
    uint8_t   protocol;            /* protocol */
    uint16_t  sum;                 /* checksum */
    uint32_t  src;                 /* source address */
    uint32_t  dst;                 /* destination address */
} Ipv4Hdr;


/* TCP Header */
typedef struct TcpHdr {
    uint16_t  srcPort;            /* source port */
    uint16_t  dstPort;            /* destination port */
    uint32_t  sequence;           /* sequence number */
    uint32_t  ack;                /* acknoledgment number */
    uint8_t   offset;             /* data offset, reserved */
    uint8_t   flags;              /* option flags */
    uint16_t  window;             /* window */
    uint16_t  sum;                /* checksum */
    uint16_t  urgent;             /* urgent pointer */
} TcpHdr;

/* UDP Header */
typedef struct UdpHdr {
    uint16_t  srcPort;            /* source port */
    uint16_t  dstPort;            /* destination port */
    uint16_t  len;                /* len */
    uint16_t  sum;                /* checksum */
} UdpHdr;

void MQTTPacket_header_parse(uint32_t buf,uint16_t packet_len);

#define TX_PER_PACKET_INFO_OUTPUT (1<<0)
#define TX_PAYLOAD_PARSE (1<<1)
#define TX_MQTT_PACKET_HEADER_PARSE (1<<2)

#define RX_PER_PACKET_INFO_OUTPUT (1<<3)    
#define RX_PAYLOAD_PARSE (1<<4)
#define RX_MQTT_PACKET_HEADER_PARSE (1<<5)

#define PACKET_TYPE_BASE (8)

#define PACKET_TYPE_MGT (1<<PACKET_TYPE_BASE)
#define PACKET_TYPE_CTRL (1<<(PACKET_TYPE_BASE+1))
#define PACKET_TYPE_DATA (1<<(PACKET_TYPE_BASE+2))

#define PACKET_AC_BASE (12)

#define PACKET_AC_BK (1<<PACKET_AC_BASE)
#define PACKET_AC_BE (1<<(PACKET_AC_BASE+1))
#define PACKET_AC_VI (1<<(PACKET_AC_BASE+2))
#define PACKET_AC_VO (1<<(PACKET_AC_BASE+3))  

extern uint32_t g_per_packet_info_output_bitmap;

#ifdef __cplusplus
}
#endif

#include "usbd_core.h"
#include "usbd_video.h"
#include <os/os.h>
#include <os/mem.h>

#define VIDEO_IN_EP 0x81

#ifdef CONFIG_USB_HS
#define MAX_PAYLOAD_SIZE  1024 // for high speed with one transcations every one micro frame
#define VIDEO_PACKET_SIZE (unsigned int)(((MAX_PAYLOAD_SIZE / 1)) | (0x00 << 11))

// #define MAX_PAYLOAD_SIZE  2048 // for high speed with two transcations every one micro frame
// #define VIDEO_PACKET_SIZE (unsigned int)(((MAX_PAYLOAD_SIZE / 2)) | (0x01 << 11))

// #define MAX_PAYLOAD_SIZE  3072 // for high speed with three transcations every one micro frame
// #define VIDEO_PACKET_SIZE (unsigned int)(((MAX_PAYLOAD_SIZE / 3)) | (0x02 << 11))

#else
#define MAX_PAYLOAD_SIZE  1020
#define VIDEO_PACKET_SIZE (unsigned int)(((MAX_PAYLOAD_SIZE / 1)) | (0x00 << 11))
#endif

#define WIDTH  (unsigned int)(640)
#define HEIGHT (unsigned int)(368)

#define CAM_FPS        (25)

#define INTERVAL       (unsigned long)(10000000 / CAM_FPS)
#define MIN_BIT_RATE   (unsigned long)(WIDTH * HEIGHT * 16 * CAM_FPS) //16 bit
#define MAX_BIT_RATE   (unsigned long)(WIDTH * HEIGHT * 16 * CAM_FPS)
#define MAX_FRAME_SIZE (unsigned long)(WIDTH * HEIGHT * 2)

#define VIDEO_VS_FRM_H264_DESP_SIZE  30
#define VIDEO_VS_FRM_MJPEG_DESP_SIZE 30

#define VC_TERMINAL_SIZ (unsigned int)(0xd + 0x12 + 0x0c + 0x09)
#define VS_HEADER_SIZ   (unsigned int)(15 + 0x1c + VIDEO_VS_FRM_H264_DESP_SIZE * 9 + 0x0b + VIDEO_VS_FRM_MJPEG_DESP_SIZE * 9)

#define USB_VIDEO_DESC_SIZ (unsigned long)(9 + 8 + 9 + VC_TERMINAL_SIZ + 7 + 5 + 9 + VS_HEADER_SIZ + 6 + 9 + 7)

#define USBD_VID           0xffff
#define USBD_PID           0xffff
#define USBD_MAX_POWER     500
#define USBD_LANGID_STRING 1033

/* h264 video descriptor macro */
#define VIDEO_VS_FRM_H264_DESP_INIT(sequence, width, height) \
		VIDEO_VS_FRAME_H264_DESCRIPTOR_INIT(sequence, width, height, MIN_BIT_RATE, MAX_BIT_RATE, DBVAL(INTERVAL), 0x01, DBVAL(INTERVAL))

#define VIDEO_VS_FRAME_H264_DESP_SET	\
		VIDEO_VS_FRM_H264_DESP_INIT(0x01, WIDTH, HEIGHT),	\
		VIDEO_VS_FRM_H264_DESP_INIT(0x02, 320, 240),		\
		VIDEO_VS_FRM_H264_DESP_INIT(0x03, 320, 480),		\
		VIDEO_VS_FRM_H264_DESP_INIT(0x04, 480, 272),		\
		VIDEO_VS_FRM_H264_DESP_INIT(0x05, 480, 320),		\
		VIDEO_VS_FRM_H264_DESP_INIT(0x06, 640, 480),		\
		VIDEO_VS_FRM_H264_DESP_INIT(0x07, 800, 480),		\
		VIDEO_VS_FRM_H264_DESP_INIT(0x08, 1280, 720),		\
		VIDEO_VS_FRM_H264_DESP_INIT(0x09, 1600, 1200)

/* MJPEG video descriptor macro */
#define VIDEO_VS_FRM_MJPEG_DESP_INIT(sequence, width, height) \
		VIDEO_VS_FRAME_MJPEG_DESCRIPTOR_INIT(sequence, width, height, MIN_BIT_RATE, MAX_BIT_RATE, MAX_FRAME_SIZE, DBVAL(INTERVAL), 0x01, DBVAL(INTERVAL))

#define VIDEO_VS_FRAME_MJPEG_DESP_SET	\
		VIDEO_VS_FRM_MJPEG_DESP_INIT(0x01, WIDTH, HEIGHT),	\
		VIDEO_VS_FRM_MJPEG_DESP_INIT(0x02, 320, 240),		\
		VIDEO_VS_FRM_MJPEG_DESP_INIT(0x03, 320, 480),		\
		VIDEO_VS_FRM_MJPEG_DESP_INIT(0x04, 480, 272),		\
		VIDEO_VS_FRM_MJPEG_DESP_INIT(0x05, 480, 320),		\
		VIDEO_VS_FRM_MJPEG_DESP_INIT(0x06, 640, 480),		\
		VIDEO_VS_FRM_MJPEG_DESP_INIT(0x07, 800, 480),		\
		VIDEO_VS_FRM_MJPEG_DESP_INIT(0x08, 1280, 720),		\
		VIDEO_VS_FRM_MJPEG_DESP_INIT(0x09, 1600, 1200)

const uint8_t video_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0xef, 0x02, 0x01, USBD_VID, USBD_PID, 0x0100, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_VIDEO_DESC_SIZ, 0x02, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    VIDEO_VC_DESCRIPTOR_INIT(0x00, 0x01, 0x0100, VC_TERMINAL_SIZ, 15000000, 0x02),
    VIDEO_VS_DESCRIPTOR_INIT(0x01, 0x00, 0x00),
    VIDEO_VS_HEADER_DESCRIPTOR_INIT(0x02, VS_HEADER_SIZ, VIDEO_IN_EP, 0x00, 0x00),
    VIDEO_VS_FORMAT_H264_DESCRIPTOR_INIT(0x01, 0x09),
    VIDEO_VS_FRAME_H264_DESP_SET,
    VIDEO_VS_FORMAT_MJPEG_DESCRIPTOR_INIT(0x02, 0x09),
    VIDEO_VS_FRAME_MJPEG_DESP_SET,
    VIDEO_VS_COLOR_MATCHING_DESCRIPTOR_INIT(),
    VIDEO_VS_DESCRIPTOR_INIT(0x01, 0x01, 0x01),
    /* 1.2.2.2 Standard VideoStream Isochronous Video Data Endpoint Descriptor */
    0x07,                         /* bLength */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: ENDPOINT */
    0x81,                         /* bEndpointAddress: IN endpoint 2 */
    0x05,                         /* bmAttributes: Isochronous transfer type. Asynchronous synchronization type. */
    WBVAL(VIDEO_PACKET_SIZE),     /* wMaxPacketSize */
    0x01,                         /* bInterval: One frame interval */

    ///////////////////////////////////////
    /// string0 descriptor
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor
    ///////////////////////////////////////
    0x14,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'B', 0x00,                  /* wcChar0 */
    'E', 0x00,                  /* wcChar1 */
    'K', 0x00,                  /* wcChar2 */
    'E', 0x00,                  /* wcChar3 */
    'N', 0x00,                  /* wcChar4 */
    'C', 0x00,                  /* wcChar5 */
    'O', 0x00,                  /* wcChar6 */
    'R', 0x00,                  /* wcChar7 */
    'P', 0x00,                  /* wcChar8 */
    ///////////////////////////////////////
    /// string2 descriptor
    ///////////////////////////////////////
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'U', 0x00,                  /* wcChar0 */
    'S', 0x00,                  /* wcChar1 */
    'B', 0x00,                  /* wcChar2 */
    ' ', 0x00,                  /* wcChar3 */
    'C', 0x00,                  /* wcChar4 */
    'a', 0x00,                  /* wcChar5 */
    'm', 0x00,                  /* wcChar6 */
    'e', 0x00,                  /* wcChar7 */
    'r', 0x00,                  /* wcChar8 */
    'a', 0x00,                  /* wcChar9 */
    ///////////////////////////////////////
    /// string3 descriptor
    ///////////////////////////////////////
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    '2', 0x00,                  /* wcChar0 */
    '0', 0x00,                  /* wcChar1 */
    '2', 0x00,                  /* wcChar2 */
    '3', 0x00,                  /* wcChar3 */
    '0', 0x00,                  /* wcChar4 */
    '1', 0x00,                  /* wcChar5 */
    '0', 0x00,                  /* wcChar6 */
    '5', 0x00,                  /* wcChar7 */
    '0', 0x00,                  /* wcChar8 */
    '0', 0x00,                  /* wcChar9 */
#ifdef CONFIG_USB_HS
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
#endif
    0x00
};

static void h264_stream_find_start_code_pos(const uint8_t *h264_stream, uint32_t stream_len, uint32_t *out_index_array, uint32_t out_array_len)
{
    uint32_t out_index = 0;
    for (int i = 0; i + 3 < stream_len; i++) {
        if (h264_stream[i] != 0x0 ||
            h264_stream[i + 1] != 0x0 ||
            h264_stream[i + 2] != 0x0 ||
            h264_stream[i + 3] != 0x1) {
            continue;
        } else {
            if (h264_stream[i + 4] == 0x68 ||
                h264_stream[i + 4] == 0x65) {
                continue;
            } else {
                if (out_index < out_array_len) {
                    out_index_array[out_index++] = i;
                }
            }
        }
    }

    if (out_index_array[out_array_len - 1] == 0) {
        out_index_array[out_array_len - 1] = stream_len;
    }
}

void usbd_configure_done_callback(void)
{
    /* no out ep, so do nothing */
}

volatile bool tx_flag = 0;
volatile bool iso_tx_busy = false;

void usbd_video_open(uint8_t intf)
{
    tx_flag = 1;
    USB_LOG_RAW("OPEN\r\n");
    iso_tx_busy = false;
}
void usbd_video_close(uint8_t intf)
{
    USB_LOG_RAW("CLOSE\r\n");
    tx_flag = 0;
    iso_tx_busy = false;
}

void usbd_video_iso_callback(uint8_t ep, uint32_t nbytes)
{
    // USB_LOG_RAW("actual in len:%d\r\n", nbytes);
    iso_tx_busy = false;
}

static struct usbd_endpoint video_in_ep = {
    .ep_cb = usbd_video_iso_callback,
    .ep_addr = VIDEO_IN_EP
};

struct usbd_interface intf0;
struct usbd_interface intf1;
static bool s_usbd_video_h264_is_init = false;

void usbd_video_h264_init()
{
    if (s_usbd_video_h264_is_init) {
        return;
    }

    usbd_desc_register(video_descriptor);
    usbd_add_interface(usbd_video_init_intf(&intf0, INTERVAL, MAX_FRAME_SIZE, MAX_PAYLOAD_SIZE));
    usbd_add_interface(usbd_video_init_intf(&intf1, INTERVAL, MAX_FRAME_SIZE, MAX_PAYLOAD_SIZE));
    usbd_add_endpoint(&video_in_ep);

    usbd_initialize();
    s_usbd_video_h264_is_init = true;
}

void usbd_video_h264_deinit()
{
    s_usbd_video_h264_is_init = false;

    usbd_deinitialize();
}

#define PACKET_BUFFER_SIZE        (2 << 10)
static USB_MEM_ALIGNX uint8_t packet_buffer[PACKET_BUFFER_SIZE];
//__attribute__((section(".video_cache_bss"))) uint8_t packet_buffer[PACKET_BUFFER_SIZE];

void dump_mem(uint8_t *buf, uint32_t buf_len)
{
    os_printf("=======uvc dump send buf start===========\r\n");
    for (int i = 0; i < buf_len; i++) {
        os_printf("%02x ", buf[i]);
    }
    os_printf("\r\n");
    os_printf("\r\n");
}

void usbd_video_send_common_frame(uint8_t *data, uint32_t size)
{
    uint32_t out_len;
    uint32_t input_len = size;
    int ret = 0;
    uint32_t packets = 0;
    uint32_t last_packet_size = 0;
    uint32_t stream_pos = 0;
    const uint32_t payload_header_length = usbd_video_common_get_payload_header_length();
    const uint32_t max_payload_transfer_size = usbd_video_get_max_payload_transfer_size();

    packets = (input_len % (max_payload_transfer_size - payload_header_length)) ?
              (input_len / (max_payload_transfer_size - payload_header_length) + 1) :
              (input_len / (max_payload_transfer_size - payload_header_length));

    last_packet_size = input_len -
                       ((packets - 1) * (max_payload_transfer_size - payload_header_length)) +
                       payload_header_length;

    // os_printf("last_packet_size:%d\r\n", last_packet_size);
    for (size_t i = 0; i < packets; i++) {
        os_memset(packet_buffer, 0, max_payload_transfer_size);
        if (i == (packets - 1)) {
            usbd_video_common_frame_payload_fill(&data[stream_pos],
                                                 last_packet_size - payload_header_length,
                                                 packet_buffer,
                                                 &out_len,
                                                 true);
        } else {
            usbd_video_common_frame_payload_fill(&data[stream_pos],
                                                 max_payload_transfer_size - payload_header_length,
                                                 packet_buffer,
                                                 &out_len,
                                                 false);
            stream_pos += (max_payload_transfer_size - payload_header_length);
        }
        // dump_mem(packet_buffer, 12);
        if (tx_flag) {
            iso_tx_busy = true;
            do {
                ret = usbd_ep_start_write(VIDEO_IN_EP, packet_buffer, out_len);
            } while(ret);

            while (iso_tx_busy) {
                if (tx_flag == 0) {
                    break;
                }
            }
        }
    }
    usbd_video_common_update_header_with_new_frame();
}

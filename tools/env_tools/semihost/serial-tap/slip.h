/**
 * @file SLIP encode/decode functions
 * @author Thanasis Georgiou
 */

#pragma once

#define SLIP_END 0xC0
#define SLIP_ESC 0xDB
#define SLIP_ESC_END 0xDC
#define SLIP_ESC_ESC 0xDD

enum slip_result {
    SLIP_OK = 0,
    SLIP_INVALID_ESCAPE = 1,
    SLIP_BUFFER_OVERFLOW = 2,
};

/**
 * Encode a piece of data according to the SLIP standard
 * @param frame             Data to encode
 * @param frameLength       Data length
 * @param output            Where to store the encoded frame
 * @param maxOutputSize     Max output size
 * @param outputSize        Where to store output length
 * @return OK for success, otherwise error code
 */
enum slip_result slip_encode(
  unsigned char *frame,
  unsigned long frameLength,
  unsigned char *output,
  unsigned long maxOutputSize,
  unsigned long *outputSize
);

/**
 * Decode a SLIP packet
 * @param encodedFrame      Data to decode
 * @param frameLength       Data length
 * @param output            Where to store the decoded data
 * @param maxOutputSize     Max output size
 * @param outputSize        Where to store output length
 * @return OK for success, otherwise error code
 */
enum slip_result slip_decode(
  unsigned char *encodedFrame,
  unsigned long frameLength,
  unsigned char *output,
  unsigned long maxOutputSize,
  unsigned long *outputSize
);

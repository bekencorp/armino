#include <stdio.h>
#include "slip.h"

enum slip_result slip_encode(unsigned char *frame, unsigned long frameLength, unsigned char *output,
		unsigned long maxOutputSize, unsigned long *outputSize)
{
	unsigned long outputIndex = 0;
	for (unsigned long inIndex = 0; inIndex < frameLength; inIndex++) {
		// Check if we ran out of space on the output
		if (maxOutputSize <= outputIndex) {
			return SLIP_BUFFER_OVERFLOW;
		}

		// Grab one byte from the input and check if we need to escape it
		unsigned char c = frame[inIndex];
		switch (c) {
		case SLIP_END:
			output[outputIndex] = SLIP_ESC;
			output[outputIndex + 1] = SLIP_ESC_END;
			outputIndex += 2;
			break;
		case SLIP_ESC:
			output[outputIndex] = SLIP_ESC;
			output[outputIndex + 1] = SLIP_ESC_ESC;
			outputIndex += 2;
			break;
		default:
			// No need to escape, copy as it is
			output[outputIndex] = c;
			outputIndex += 1;
			break;
		}
	}

	// Mark the frame end
	output[outputIndex] = SLIP_END;

	// Return the output size
	*outputSize = outputIndex + 1;
	return SLIP_OK;
}

enum slip_result slip_decode(unsigned char *encodedFrame, unsigned long frameLength,
		unsigned char *output, unsigned long maxOutputSize, unsigned long *outputSize)
{
	int invalidEscape = 0;

	unsigned long outputIndex = 0;
	for (unsigned long inIndex = 0; inIndex < frameLength; inIndex++) {
		// Check if we ran out of space on the output buffer
		if (maxOutputSize <= outputIndex) {
			return SLIP_BUFFER_OVERFLOW;
		}

		unsigned char inByte = encodedFrame[inIndex];
		if (inByte == SLIP_ESC) {
			switch (encodedFrame[inIndex + 1]) {
			case SLIP_ESC_END:
				output[outputIndex] = SLIP_END;
				break;
			case SLIP_ESC_ESC:
				output[outputIndex] = SLIP_ESC;
				break;
			default:
				// Escape sequence invalid, complain on stderr
				output[outputIndex] = SLIP_ESC;
				invalidEscape = 1;
				fprintf(stderr, "SLIP escape error! (Input bytes 0x%02x, 0x%02x)\n", inByte, encodedFrame[inIndex + 1]);
				break;
			}
			inIndex += 1;
		} else if (inByte == SLIP_END) {
			// End of packet, stop the loop
			outputIndex++;
			break;
		} else {
			output[outputIndex] = inByte;
		}

		outputIndex++;
	}

	*outputSize = outputIndex;
	if (invalidEscape)
		return SLIP_INVALID_ESCAPE;
	return SLIP_OK;
}

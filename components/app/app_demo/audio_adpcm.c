#include <stdio.h>
#include <stdlib.h>
#include "audio_adpcm.h"

short g_nEnAudioPreSample[AUDIO_ADPCM_MAX_INDEX];
short g_nEnAudioIndex[AUDIO_ADPCM_MAX_INDEX];
short g_nDeAudioPreSample[AUDIO_ADPCM_MAX_INDEX];
short g_nDeAudioIndex[AUDIO_ADPCM_MAX_INDEX];

const static int gs_index_adjust[8] = { -1, -1, -1, -1, 2, 4, 6, 8};
const static int gs_step_table[89] = {
	7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
	50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371,
	408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
	2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493,
	10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};

void ADPCM_EncoderClear(int sit)
{
	if (sit >= AUDIO_ADPCM_MAX_INDEX) {
		printf("sit is greate to adpcm=%d\n", sit);
		return;
	}
	g_nEnAudioPreSample[sit] = 0;
	g_nEnAudioIndex[sit] = 0;
}

void EncoderGetIndex(int sit, short *sample, short *index)
{
	if (sit >= AUDIO_ADPCM_MAX_INDEX) {
		printf("EncoderGetIndex sit is greate to adpcm=%d\n", sit);
		return;
	}
	*sample = g_nEnAudioPreSample[sit];
	*index = g_nEnAudioIndex[sit];
}

void ADPCM_DecoderClear(int sit)
{
	if (sit >= AUDIO_ADPCM_MAX_INDEX) {
		printf("sit is greate to adpcm\n");
		return;
	}
	g_nDeAudioPreSample[sit] = 0;
	g_nDeAudioIndex[sit] = 0;
}

void DecoderSetIndex(int sit, short sample, short index)
{
	if (sit >= AUDIO_ADPCM_MAX_INDEX) {
		printf("sit is greate to adpcm\n");
		return;
	}
	g_nDeAudioPreSample[sit] = sample;
	g_nDeAudioIndex[sit] = index;
}

void ADPCM_EncodeData(int sit, unsigned char *pRaw, int nLenRaw, unsigned char *pBufEncoded)
{
	short *pcm = (short *)pRaw;
	int cur_sample;
	int i;
	int delta;
	int sb;
	int code;
	nLenRaw >>= 1;

	if (sit >= AUDIO_ADPCM_MAX_INDEX) {
		printf("sit is greate to adpcm\n");
		return;
	}

	for (i = 0; i < nLenRaw; i++) {
		cur_sample = pcm[i];
		delta = cur_sample - g_nEnAudioPreSample[sit];

		if (delta < 0) {
			delta = -delta;
			sb = 8;
		} else
			sb = 0;

		code = 4 * delta / gs_step_table[g_nEnAudioIndex[sit]];

		if (code > 7)
			code = 7;

		delta = (gs_step_table[g_nEnAudioIndex[sit]] * code) / 4 + gs_step_table[g_nEnAudioIndex[sit]] / 8;

		if (sb)
			delta = -delta;

		g_nEnAudioPreSample[sit] += delta;

		if (g_nEnAudioPreSample[sit] > 32767)
			g_nEnAudioPreSample[sit] = 32767;
		else if (g_nEnAudioPreSample[sit] < -32768)
			g_nEnAudioPreSample[sit] = -32768;

		g_nEnAudioIndex[sit] += gs_index_adjust[code];

		if (g_nEnAudioIndex[sit] < 0)
			g_nEnAudioIndex[sit] = 0;
		else if (g_nEnAudioIndex[sit] > 88)
			g_nEnAudioIndex[sit] = 88;

		if (i & 0x01)
			pBufEncoded[i >> 1] |= code | sb;
		else
			pBufEncoded[i >> 1] = (code | sb) << 4;
	}
}

void ADPCM_DecodeData(int sit, char *pDataCompressed, int nLenData, char *pDecoded)
{
	int i;
	int code;
	int sb;
	int delta;
	short *pcm = (short *)pDecoded;
	nLenData <<= 1;

	if (sit >= AUDIO_ADPCM_MAX_INDEX) {
		printf("index is greate to %d\n", AUDIO_ADPCM_MAX_INDEX);
		return;
	}

	for (i = 0; i < nLenData; i++) {
		if (i & 0x01)
			code = pDataCompressed[i >> 1] & 0x0f;
		else
			code = pDataCompressed[i >> 1] >> 4;

		if ((code & 8) != 0)
			sb = 1;
		else
			sb = 0;

		code &= 7;

		delta = (gs_step_table[g_nDeAudioIndex[sit]] * code) / 4 + gs_step_table[g_nDeAudioIndex[sit]] / 8;

		if (sb)
			delta = -delta;

		g_nDeAudioPreSample[sit] += delta;

		if (g_nDeAudioPreSample[sit] > 32767)
			g_nDeAudioPreSample[sit] = 32767;
		else if (g_nDeAudioPreSample[sit] < -32768)
			g_nDeAudioPreSample[sit] = -32768;

		pcm[i] = g_nDeAudioPreSample[sit];
		g_nDeAudioIndex[sit] += gs_index_adjust[code];

		if (g_nDeAudioIndex[sit] < 0)
			g_nDeAudioIndex[sit] = 0;

		if (g_nDeAudioIndex[sit] > 88)
			g_nDeAudioIndex[sit] = 88;
	}
}
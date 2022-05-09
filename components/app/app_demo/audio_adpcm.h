#ifndef __AUDIO_ADPCM_H__
#define __AUDIO_ADPCM_H__

#define AUDIO_ADPCM_MAX_INDEX   1

void ADPCM_DecodeData(int sit, char *pDataCompressed, int nLenData, char *pDecoded);
void ADPCM_EncodeData(int sit, unsigned char *pRaw, int nLenRaw, unsigned char *pBufEncoded);
void ADPCM_EncoderClear(int sit);
void ADPCM_DecoderClear(int sit);



#endif

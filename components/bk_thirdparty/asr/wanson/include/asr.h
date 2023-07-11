#ifndef ASR_FN_ASR_H_
#define ASR_FN_ASR_H_

#ifdef __cplusplus
extern "C" {
#endif

// Return value   : 0 - OK, -1 - Error
int  Wanson_ASR_Init();

void Wanson_ASR_Reset();

/*****************************************
* Input:
*      - buf      : Audio data (16k, 16bit, mono) 
*      - buf_len  : Now must be 480 (30ms) 
*
* Output:
*      - text     : The text of ASR 
*      - score    : The confidence of ASR (Now not used)
*
* Return value    :  0 - No result
*                    1 - Has result 
*                   -1 - Error
******************************************/
int  Wanson_ASR_Recog(short *buf, int buf_len, const char **text, float *score);

void Wanson_ASR_Release();

#ifdef __cplusplus
}
#endif

#endif

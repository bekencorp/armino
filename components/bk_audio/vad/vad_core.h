#ifndef _VAD_CORE_H_
#define _VAD_CORE_H_

#define FRAME_TYPE_SPEECH                  (1)
#define FRAME_TYPE_NOISE                   (0)

/*threshold value of power*/
//#ifdef TEST7251_
#define POWER_THRESHOLD_VAL 89925  //26510
//#endif



extern int vad(short samples[], int len);

#endif // _VAD_CORE_H_
// eof

#ifndef _PCM_RESAMPLER_H_
#define _PCM_RESAMPLER_H_

#include <stdlib.h>
#include <common/sys_config.h>

#if !defined(_MSC_VER)
#include <sys/types.h>
#else
typedef unsigned int               uint32_t;
typedef unsigned short             uint16_t;
typedef int                        int32_t;
typedef short                      int16_t;
typedef unsigned long long         uint64_t;
#endif

#define USE_SMALLFILTER

#define Nhc       8
#define Na        7
#define Np       (Nhc+Na)
#define Npc      (1<<Nhc)
#define Amask    ((1<<Na)-1)
#define Pmask    ((1<<Np)-1)
#define Nh       16
#define Nb       16
#define Nhxn     16
#define Nhg      (Nh-Nhxn)
#define NLpScl   13

#define IBUFF_PAD_SIZE 512 /* reference to resample_Init Xlen = len(X1) = (PCM_INPUT_BUFF_SIZE + IBUFF_PAD_SIZE) >= */
//#define PCM_RESAMPLER_OPTIMIZE

typedef struct {
	uint16_t LpScl;               /* Unity-gain scale factor */
	uint16_t Nwing;               /* Filter table size */
	uint16_t Nmult;               /* Filter length for up-conversions */
	uint16_t Xlen;                /* real length of X1 */
	uint16_t Xoff;
	uint16_t Xread;
	int16_t *Imp;               /* Filter coefficients */
	int16_t *ImpD;              /* ImpD[n] = Imp[n+1]-Imp[n] */
	uint32_t factor;
	uint32_t dhb, dtb;
	uint32_t Time;
	int16_t X1[1];
} ParameterResample;

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef MAX
#define MAX(x,y) ((x)>(y) ?(x):(y))
#endif
#ifndef MIN
#define MIN(x,y) ((x)<(y) ?(x):(y))
#endif

#ifndef ABS
#define ABS(x)   ((x)<0   ?(-(x)):(x))
#endif

#ifndef SGN
#define SGN(x)   ((x)<0   ?(-1):((x)==0?(0):(1)))
#endif

#ifndef MAX_HWORD
#define MAX_HWORD (32767)
#endif

#ifndef MIN_HWORD
#define MIN_HWORD (-32768)
#endif

#define PCM_INPUT_BUFF_SIZE  (0x1000)
#define PCM_OUTPUT_BUFF_SIZE (PCM_INPUT_BUFF_SIZE/2) /* PCM_INPUT_BUFF_SIZE * output_sameple_rate / input_sample_rate */

#define DEFAULT_IN_SAMPLE_RATE      (44100)
#define DEFAULT_OUT_SAMPLE_RATE     (16000)

typedef struct {
	int32_t		nInputSampleRate;
	int32_t     nOutputSampleRate;
	int32_t     nInputbufferszie; /* samples number of input buffer */
	int32_t     nMaxbuffersize; /* max size of input buffer for each process */
	void    *pSRCHandler; /* ParameterResample pointer */
} PCM_Resampler;

PCM_Resampler *pcm_resampler_initialize(int32_t in_length, uint32_t in_sampRate, uint32_t out_sampRate);
int32_t pcm_resampler_updateinsamprate(PCM_Resampler *resampler, int32_t in_length, int32_t in_sampRate);
int32_t pcm_resampler_process(PCM_Resampler *resampler, char *pIn, char *pOut);
int32_t pcm_resampler_finalize(PCM_Resampler *resampler);
#endif
// eof



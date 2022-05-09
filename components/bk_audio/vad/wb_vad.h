#ifndef _WB_VAD_H_
#define _WB_VAD_H_

/******************************************************************************
*                         INCLUDE FILES
******************************************************************************/
#include "vad_typedef.h"
#include "wb_vad_c.h"

/******************************************************************************
*                         DEFINITION OF DATA TYPES
******************************************************************************/

typedef struct {
	float backgroud_estimate[SUB_BAND_CNT];    /* background noise estimate */
	float average_level[SUB_BAND_CNT];   /* averaged input components for stationary */

	/* estimation */
	float old_level[SUB_BAND_CNT];   /* input levels of the previous frame */
	float sub_level[SUB_BAND_CNT];   /* input levels calculated at the end of a frame (lookahead) */
	float a_data5[F_5TH_CNT][2];     /* memory for the filter bank */
	float a_data3[F_3TH_CNT];        /* memory for the filter bank */

	VAD_I16 burst_count;         /* counts length of a speech burst */
	VAD_I16 hang_count;          /* hangover counter */
	VAD_I16 stat_count;          /* stationary counter */

	/* Note that each of the following two variables holds 15 flags. Each flag reserves
	 * 1 bit of the variable. The newest flag is
	 * in the bit 15 (assuming that LSB is bit 1 and MSB is bit 16). */
	VAD_I16 vad_reg;              /* flags for intermediate VAD decisions */
	VAD_I16 pitch_tone;          /* flags for pitch and tone detection */

	VAD_I16 sp_est_cnt;          /* counter for speech level estimation */
	float sp_max;               /* maximum level */
	VAD_I16 sp_max_cnt;          /* counts frames that contains speech */
	float speech_level;         /* estimated speech level */
	double prev_pow_sum;        /* power of previous frame */

	float level[SUB_BAND_CNT];
	float prevLevel[SUB_BAND_CNT];
} VAD_VARS_T;

/*
********************************************************************************
*                         DECLARATION OF PROTOTYPES
********************************************************************************
*/
int wb_vad_init(VAD_VARS_T **st);
int wb_vad_reset(VAD_VARS_T *st);
void wb_vad_exit(VAD_VARS_T **st);
void wb_vad_pitch_tone_detection(VAD_VARS_T *st, float p_gain);
VAD_I16 wb_vad(VAD_VARS_T *st, float in_buf[]);

#endif // _WB_VAD_H_

#ifndef __BPS_FPS_CALCULATOR_H__
#define __BPS_FPS_CALCULATOR_H__

#include <stdio.h>

void *create_bps_fps_calculator(uint32_t stats_wind_in_sec);
void destroy_bps_fps_calculator(void *calculator);
int insert_frame(void *calculator, uint32_t frame_len);
int insert_frame_within_target_bitrate(void *calculator, uint32_t frame_len, uint32_t target_bps);
int get_bps_fps(void *calculator, uint32_t *bps, uint32_t *fps);

#endif
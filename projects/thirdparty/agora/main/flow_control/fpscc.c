#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "FifoBuffer.h"

#include <os/os.h>
#include <os/mem.h>

#define TAG "bps_calc"

typedef struct {
  uint32_t frame_len;
  uint32_t time_stamp;
} frame_t;

FifoBuffer_typedef(frame_t, frame_fifo_t);

typedef struct {
  frame_fifo_t frame_fifo;
  uint32_t total_bytes;
  uint32_t total_frames;
  uint32_t stats_wind_in_sec;
} bps_fps_calc_t;

void *create_bps_fps_calculator(uint32_t stats_wind_in_sec)
{
  bps_fps_calc_t *bps_fps_calc = os_malloc(sizeof(bps_fps_calc_t));
  if (!bps_fps_calc) {
    printf("Failed to alloc memory for bitrate calculator!\n");
    return NULL;
  }

  // create a FIFO (queue) capable of storing 100 items
  frame_fifo_t *fifo = &bps_fps_calc->frame_fifo;
  FifoBuffer_init(fifo, 50, frame_t);
  bps_fps_calc->stats_wind_in_sec = stats_wind_in_sec;
  bps_fps_calc->total_bytes = 0;
  bps_fps_calc->total_frames = 0;

  return bps_fps_calc;
}

void destroy_bps_fps_calculator(void *calculator)
{
  bps_fps_calc_t *bps_fps_calc = (bps_fps_calc_t *)calculator;
  if (bps_fps_calc) {
    frame_fifo_t *fifo = &bps_fps_calc->frame_fifo;
    FifoBuffer_deinit(fifo);
    os_free(bps_fps_calc);
  }
}

static int try_insert_frame(void *calculator, uint32_t frame_len, bool need_check_target_bps, uint32_t target_bps)
{
  assert(calculator);
  bps_fps_calc_t *bps_fps_calc = (bps_fps_calc_t *)calculator;
  frame_fifo_t *fifo = &bps_fps_calc->frame_fifo;

  frame_t frame;
  frame.frame_len = frame_len;
  frame.time_stamp = rtos_get_time();

  if (need_check_target_bps && target_bps != 0) {
    uint32_t total_bytes_budget = (target_bps * bps_fps_calc->stats_wind_in_sec) >> 3;
    if ((bps_fps_calc->total_bytes + frame.frame_len) > total_bytes_budget) {
      return -1;
    }
  }

  assert(!FifoBuffer_is_full(fifo));
  FifoBuffer_write(fifo, frame);

  // update the total bytes within the statistics window
  bps_fps_calc->total_bytes += frame.frame_len;
  bps_fps_calc->total_frames += 1;

  return 0;
}

int insert_frame(void *calculator, uint32_t frame_len)
{
  return try_insert_frame(calculator, frame_len, false, 0);
}

int insert_frame_within_target_bitrate(void *calculator, uint32_t frame_len, uint32_t target_bps)
{
  return try_insert_frame(calculator, frame_len, true, target_bps);
}

int get_bps_fps(bps_fps_calc_t *calculator, uint32_t *bps, uint32_t *fps)
{
  assert(calculator);
  bps_fps_calc_t *bps_fps_calc = (bps_fps_calc_t *)calculator;
  frame_fifo_t *fifo = &bps_fps_calc->frame_fifo;
  uint32_t now = rtos_get_time();
  uint32_t start_time = now - bps_fps_calc->stats_wind_in_sec * 1000;
  frame_t frame;

  while (!FifoBuffer_is_empty(fifo)) {
    FifoBuffer_peek(fifo, frame, 0);
    if (frame.time_stamp > start_time) {
      break;
    }
    // remove the oldest frame which is outside of the statistics window
    FifoBuffer_read(fifo, frame);
    bps_fps_calc->total_bytes -= frame.frame_len;
    bps_fps_calc->total_frames -= 1;
    //printf("discard: total_bytes %d\n", bps_fps_calc->total_bytes);
  };
  *bps = ((bps_fps_calc->total_bytes << 3) / bps_fps_calc->stats_wind_in_sec);
  *fps = (bps_fps_calc->total_frames / bps_fps_calc->stats_wind_in_sec);

  return 0;
}
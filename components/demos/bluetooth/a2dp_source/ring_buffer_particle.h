#pragma once

#include <stdint.h>

//#define INT_CEIL_4 (x) ((x) / 4 * 4 + ((x) % 4 ? 4 : 0))
//#define INT_FLOOR_4 (x) ((x) / 4 * 4)
#define INT_CEIL(x, size) ((x) / (size) * (size) + ((x) % (size) ? (size) : 0))
#define INT_FLOOR(x, size) ((x) / (size) * (size))

#define INT_CEIL_4(x) ((((size_t)(x)) & (~(size_t)3)) + ((((size_t)(x)) & 0b11) ? 0b100: 0))
#define INT_CEIL_4(x) ((((size_t)(x)) & (~(size_t)3)) + ((((size_t)(x)) & 0b11) ? 0b100: 0))
#define INT_FLOOR_4(x) (((size_t)(x)) & (~(size_t)3))

typedef struct
{
    uint8_t *buffer;
    uint32_t len;
    uint32_t rp;
    uint32_t wp;
}ring_buffer_particle_ctx;

int32_t ring_buffer_particle_init(ring_buffer_particle_ctx *ctx, uint32_t len);
int32_t ring_buffer_particle_deinit(ring_buffer_particle_ctx *ctx);
bool ring_buffer_particle_is_init(ring_buffer_particle_ctx *ctx);
int32_t ring_buffer_particle_reset(ring_buffer_particle_ctx *ctx);

int32_t ring_buffer_particle_write(ring_buffer_particle_ctx *ctx, uint8_t *data, uint32_t len);
int32_t ring_buffer_particle_read(ring_buffer_particle_ctx *ctx, uint8_t *buff, uint32_t len, uint32_t *already_len);
uint32_t ring_buffer_particle_len(ring_buffer_particle_ctx *ctx);

void ring_buffer_particle_debug(ring_buffer_particle_ctx *ctx);
int32_t ring_buffer_particle_test(void);

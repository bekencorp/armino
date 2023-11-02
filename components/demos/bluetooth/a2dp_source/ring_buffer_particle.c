#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "os/mem.h"
#include "components/log.h"
#include "driver/psram.h"

#include "ring_buffer_particle.h"

#define USE_PSRAM 1

#define RB_MIN(x, y) (((x) > (y)) ? (y):(x))

#define rb_write_word(addr, val)                 do{*((volatile uint32_t *)(addr)) = val;} while(0)
#define rb_read_word(val, addr)                  do{(val) = *((volatile uint32_t *)(addr));} while(0)
#define rb_get_word(addr)                       *((volatile uint32_t *)(addr))

#if 0//USE_PSRAM

#define bk_psram_word_memcpy rb_psram_memcpy

//__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns")))
static void rb_psram_memcpy(uint8 *out, const uint8_t *in, uint32_t n)
{
    size_t out_i = (size_t)out, in_i = (size_t)in;
    uint32_t tmp_org_32 = 0, tmp_new_32 = 0, tmp_mask = 0;
    uint32_t i = 0;

    if (out_i % 4)
    {
        tmp_org_32 = rb_get_word(INT_FLOOR_4(out_i));

        os_memcpy(&tmp_new_32, (void *)in_i, RB_MIN(n, INT_CEIL_4(out_i) - out_i));

        for (i = 0; i < out_i - INT_FLOOR_4(out_i); ++i)
        {
            tmp_mask <<= 8;
            tmp_mask |= 0xff;
        }

        tmp_new_32 <<= 8 * (out_i - INT_FLOOR_4(out_i));

        tmp_org_32 &= tmp_mask;
        tmp_org_32 |= tmp_new_32;

        rb_write_word(INT_FLOOR_4(out_i), tmp_org_32);

        in_i += RB_MIN(n, INT_CEIL_4(out_i) - out_i);
        n -= RB_MIN(n, INT_CEIL_4(out_i) - out_i);
        out_i = INT_CEIL_4(out_i);
    }

    for (i = 0; i < n / 4; ++i)
    {
        os_memcpy(&tmp_new_32, (void *)(in_i + i * 4), 4);
        rb_write_word(out_i + i * 4, tmp_new_32);
    }

    if (n % 4)
    {
        uint32_t index = i;

        tmp_org_32 = rb_get_word(out_i + index * 4);
        os_memcpy(&tmp_new_32, (void *)(in_i + index * 4), n - INT_FLOOR_4(n));

        tmp_mask = 0;

        for (i = 0; i < INT_CEIL_4(n) - n; ++i)
        {
            tmp_mask >>= 8;
            tmp_mask |= 0xff000000;
        }

        tmp_org_32 &= tmp_mask;
        tmp_org_32 |= tmp_new_32;

        rb_write_word(out_i + index * 4, tmp_org_32);
    }
}

#endif

static inline uint32_t ring_buffer_particle_ptr_plus(ring_buffer_particle_ctx *ctx, uint32_t ptr, uint32_t len)
{
    return (ptr + len) % ctx->len;
}

int32_t ring_buffer_particle_init(ring_buffer_particle_ctx *ctx, uint32_t len)
{
    uint32_t final_len = len;

    if (!ctx || ctx->buffer)
    {
        BK_LOGE("rb", "%s param err\n", __func__);
        return -1;
    }

#if USE_PSRAM
    ctx->buffer = psram_malloc(final_len);
#else
    ctx->buffer = os_malloc(final_len);
#endif

    if (!ctx->buffer)
    {
        BK_LOGE("rb", "%s alloc err\n", __func__);
        return -1;
    }

    ctx->len = final_len;
    ctx->rp = ctx->wp = 0;

    return 0;
}

int32_t ring_buffer_particle_deinit(ring_buffer_particle_ctx *ctx)
{
    if (ctx->buffer)
    {
#if USE_PSRAM
        psram_free(ctx->buffer);
#else
        os_free(ctx->buffer);
#endif
    }

    os_memset(ctx, 0, sizeof(*ctx));

    return 0;
}

int32_t ring_buffer_particle_reset(ring_buffer_particle_ctx *ctx)
{
    uint8_t *tmp_buff = ctx->buffer;

    os_memset(ctx, 0, sizeof(*ctx));
    ctx->buffer = tmp_buff;

    return 0;
}

int32_t ring_buffer_particle_write(ring_buffer_particle_ctx *ctx, uint8_t *data, uint32_t len)
{
    volatile uint32_t crp = ctx->rp, cwp = ctx->wp;
    uint32_t tmp_len = len;

    if (crp > cwp && cwp + tmp_len >= crp)
    {
        return -1;
    }

    if (crp <= cwp)
    {
        if ((cwp + tmp_len) / ctx->len >= 2 || ((cwp + tmp_len) / ctx->len == 1 && (cwp + tmp_len) % ctx->len >= crp))
        {
            return -1;
        }
    }

    if (crp > cwp)
    {

#if USE_PSRAM
        bk_psram_word_memcpy(ctx->buffer + cwp, data, tmp_len);
#else
        os_memcpy(ctx->buffer + cwp, data, tmp_len);
#endif

        cwp += tmp_len;
    }
    else
    {
        if (tmp_len <= ctx->len - cwp)
        {
#if USE_PSRAM
            bk_psram_word_memcpy(ctx->buffer + cwp, data, tmp_len);
#else
            os_memcpy(ctx->buffer + cwp, data, tmp_len);
#endif
            cwp = ring_buffer_particle_ptr_plus(ctx, cwp, tmp_len);
        }
        else
        {
#if USE_PSRAM
            bk_psram_word_memcpy(ctx->buffer + cwp, data, ctx->len - cwp);
            bk_psram_word_memcpy(ctx->buffer, data + (ctx->len - cwp), tmp_len - (ctx->len - cwp));
#else
            os_memcpy(ctx->buffer + cwp, data, ctx->len - cwp);
            os_memcpy(ctx->buffer, data + (ctx->len - cwp), tmp_len - (ctx->len - cwp));
#endif
            cwp = ring_buffer_particle_ptr_plus(ctx, cwp, tmp_len);
        }
    }

    ctx->wp = cwp;

    return 0;
}

int32_t ring_buffer_particle_read(ring_buffer_particle_ctx *ctx, uint8_t *buff, uint32_t len, uint32_t *already_len)
{
    volatile uint32_t crp = ctx->rp, cwp = ctx->wp;

    if (crp == cwp)
    {
        *already_len = 0;
        return 0;
    }

    if (crp < cwp)
    {
        uint32_t final_len = len < cwp - crp ? len : cwp - crp;
        os_memcpy(buff, ctx->buffer + crp, final_len);
        *already_len = final_len;
        crp += final_len;
    }
    else if (crp > cwp)
    {
        uint32_t first = 0, sec = 0;
        first += ctx->len - crp < len ? ctx->len - crp : len;
        sec += cwp - 0 < len - first ? cwp - 0 : len - first;
        os_memcpy(buff, ctx->buffer + crp, first);
        os_memcpy(buff + first, ctx->buffer, sec);

        *already_len = first + sec;
        crp = ring_buffer_particle_ptr_plus(ctx, crp, first + sec);
    }

    ctx->rp = crp;

    return 0;
}

uint32_t ring_buffer_particle_len(ring_buffer_particle_ctx *ctx)
{
    volatile uint32_t crp = ctx->rp, cwp = ctx->wp;

    if (crp == cwp)
    {
        return 0;
    }

    if (crp < cwp)
    {
        return cwp - crp;
    }

    if (crp > cwp)
    {
        return ctx->len - (crp - cwp);
    }

    return 0;
}

void ring_buffer_particle_debug(ring_buffer_particle_ctx *ctx)
{
    BK_LOGI("rb", "%s %d %d %d %d\n", __func__, ctx->rp, ctx->wp, ctx->len, ring_buffer_particle_len(ctx));
}

int32_t ring_buffer_particle_test(void)
{
#define RB_SIZE 128
#define BUFF_SIZE 79

    ring_buffer_particle_ctx ctx;
    uint8_t *write_buff, *read_buff;
    uint32_t i = 0;
    int32_t ret = 0;
    uint32_t read_size = 0;

    os_memset(&ctx, 0, sizeof(ctx));
    ring_buffer_particle_init(&ctx, RB_SIZE + 1);

    write_buff = os_malloc(BUFF_SIZE);
    read_buff = os_malloc(BUFF_SIZE);

    srand(rtos_get_time());

    for (i = 0; i < BUFF_SIZE; i++)
    {
        write_buff[i] = ((uint32_t)rand()) % 255;
    }

    //test 1
    ret = ring_buffer_particle_write(&ctx, write_buff, BUFF_SIZE);

    if (ret)
    {
        BK_LOGE("rb", "%s write 1 err\n", __func__);
        return -1;
    }

    ret = ring_buffer_particle_read(&ctx, read_buff, BUFF_SIZE, &read_size);

    if (ret)
    {
        BK_LOGE("rb", "%s read 1 err\n", __func__);
        return -1;
    }

    if (read_size != BUFF_SIZE)
    {
        BK_LOGE("rb", "%s read 1 len err %d\n", __func__, read_size);
        return -1;
    }
    else if (os_memcmp(read_buff, write_buff, BUFF_SIZE))
    {
        BK_LOGE("rb", "%s read 1 not match write\n", __func__);
        return -1;
    }



    //test 2
    ret = ring_buffer_particle_write(&ctx, write_buff, BUFF_SIZE);

    if (ret)
    {
        BK_LOGE("rb", "%s write 2 err\n", __func__);
        return -1;
    }

    ret = ring_buffer_particle_read(&ctx, read_buff, BUFF_SIZE, &read_size);

    if (ret)
    {
        BK_LOGE("rb", "%s read 2 err\n", __func__);
        return -1;
    }

    if (read_size != BUFF_SIZE)
    {
        BK_LOGE("rb", "%s read 2 len err %d\n", __func__, read_size);
        return -1;
    }
    else if (os_memcmp(read_buff, write_buff, BUFF_SIZE))
    {
        BK_LOGE("rb", "%s read 2 not match write\n", __func__);
        return -1;
    }


    //test 3
    ret = ring_buffer_particle_write(&ctx, write_buff, BUFF_SIZE);

    if (ret)
    {
        BK_LOGE("rb", "%s write 3 step 1 err\n", __func__);
        return -1;
    }

    ret = ring_buffer_particle_write(&ctx, write_buff, RB_SIZE - BUFF_SIZE);

    if (ret)
    {
        BK_LOGE("rb", "%s write 3 step 2 err\n", __func__);
        return -1;
    }

    ret = ring_buffer_particle_write(&ctx, write_buff, 1);

    if (!ret)
    {
        BK_LOGE("rb", "%s write 3 step 3 should ret err but ok !!!\n", __func__);
        return -1;
    }

    ret = ring_buffer_particle_read(&ctx, read_buff, BUFF_SIZE, &read_size);

    if (ret)
    {
        BK_LOGE("rb", "%s read 3 step 1 err\n", __func__);
        return -1;
    }

    if (read_size != BUFF_SIZE)
    {
        BK_LOGE("rb", "%s read 3 len step 1 err %d\n", __func__, read_size);
        return -1;
    }
    else if (os_memcmp(read_buff, write_buff, BUFF_SIZE))
    {
        BK_LOGE("rb", "%s read 3 step 1 not match write\n", __func__);
        return -1;
    }

    ret = ring_buffer_particle_read(&ctx, read_buff, BUFF_SIZE, &read_size);

    if (ret)
    {
        BK_LOGE("rb", "%s read 3 step 2 err\n", __func__);
        return -1;
    }

    if (read_size != RB_SIZE - BUFF_SIZE)
    {
        BK_LOGE("rb", "%s read 3 len step 2 err %d\n", __func__, read_size);
        return -1;
    }
    else if (os_memcmp(read_buff, write_buff, RB_SIZE - BUFF_SIZE))
    {
        BK_LOGE("rb", "%s read 3 step 2 not match write\n", __func__);
        return -1;
    }

    ret = ring_buffer_particle_read(&ctx, read_buff, 1, &read_size);

    if (ret || read_size)
    {
        BK_LOGE("rb", "%s read 3 step 3 err ret %d size %d\n", __func__, ret, read_size);
        return -1;
    }

    os_free(write_buff);
    os_free(read_buff);
    ring_buffer_particle_deinit(&ctx);

    return 0;
}

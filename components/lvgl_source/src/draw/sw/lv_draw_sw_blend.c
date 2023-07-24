/**
 * @file lv_draw_sw_blend.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_draw_sw.h"
#include "../../misc/lv_math.h"
#include "../../hal/lv_hal_disp.h"
#include "../../core/lv_refr.h"
#include "lv_disp.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
#if LVGL_USE_PSRAM
static void _memset_word(uint32_t *b, int32_t c, uint32_t n)
{
    // Note:
    // the word count == sizeof(buf)/sizeof(uint32_t)
    uint32_t word_cnt = n >> 2;

    for(int i = 0; i < word_cnt; i++)
    {
        os_write_word((b + i), c);
    }
}

static void _memset_color(lv_color_t * dest_buf, lv_color_t color, uint32_t point_num)
{
    uint32_t c32 = (uint32_t)color.full + ((uint32_t)color.full << 16);
    lv_color_t *tmp_ptr = dest_buf;
    lv_color_t *pre_ptr = NULL;
    lv_color_t *next_ptr = NULL;
    
    if((((uint32_t)tmp_ptr) & 0x3) != 0)
    {
        pre_ptr = tmp_ptr - 1;
        *((volatile uint32_t *)(tmp_ptr - 1)) = (color.full << 16) | pre_ptr->full;

        tmp_ptr++;
        point_num--;
    }

    if((point_num & 0x1) == 0)
    {
        _memset_word((uint32_t *)tmp_ptr, c32, point_num * 2);
    }
    else
    {
        _memset_word((uint32_t *)tmp_ptr, c32, (point_num - 1) * 2);
        tmp_ptr += point_num - 1;

        next_ptr = tmp_ptr + 1;
        *((volatile unsigned int *)tmp_ptr) = (next_ptr->full << 16) | color.full;
    }
}
#endif

void lv_memcpy_one_line(void *dest_buf, const void *src_buf, uint32_t point_num)
{
#if (LV_COLOR_DEPTH == 16)
    lv_color_t *tmp_dst_ptr = dest_buf;
    const lv_color_t *tmp_src_ptr = src_buf;
    lv_color_t *pre_ptr = NULL;
    lv_color_t *next_ptr = NULL;

    if((((uint32_t)tmp_dst_ptr) & 0x3) != 0)
    {
        pre_ptr = tmp_dst_ptr - 1;
        *((volatile uint32_t *)(tmp_dst_ptr - 1)) = (tmp_src_ptr->full << 16) | pre_ptr->full;

        tmp_dst_ptr++;
        tmp_src_ptr++;
        point_num--;
    }

    if((point_num & 0x1) == 0)
    {
        os_memcpy_word((uint32_t *)tmp_dst_ptr, (const uint32_t *)tmp_src_ptr, point_num * sizeof(lv_color16_t));
    }
    else
    {
        os_memcpy_word((uint32_t *)tmp_dst_ptr, (const uint32_t *)tmp_src_ptr, (point_num - 1) * sizeof(lv_color16_t));

        tmp_dst_ptr += point_num - 1;
        tmp_src_ptr += point_num - 1;

        next_ptr = tmp_dst_ptr + 1;
        *((volatile unsigned int *)tmp_dst_ptr) = (next_ptr->full << 16) | tmp_src_ptr->full;
    }
#elif (LV_COLOR_DEPTH == 32)
    os_memcpy_word((uint32_t *)dest_buf, (const uint32_t *)src_buf, point_num * sizeof(lv_color32_t));
#endif
}

void bk_printf(const char *fmt, ...);
static void fill_set_px(lv_color_t * dest_buf, const lv_area_t * blend_area, lv_coord_t dest_stride,
                        lv_color_t color, lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stide);

LV_ATTRIBUTE_FAST_MEM static void fill_normal(lv_color_t * dest_buf, const lv_area_t * dest_area,
                                              lv_coord_t dest_stride, lv_color_t color, lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stride);


#if LV_COLOR_SCREEN_TRANSP
LV_ATTRIBUTE_FAST_MEM static void fill_argb(lv_color_t * dest_buf, const lv_area_t * dest_area,
                                            lv_coord_t dest_stride, lv_color_t color, lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stride);
#endif /*LV_COLOR_SCREEN_TRANSP*/

#if LV_DRAW_COMPLEX
static void fill_blended(lv_color_t * dest_buf, const lv_area_t * dest_area, lv_coord_t dest_stride, lv_color_t color,
                         lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stride, lv_blend_mode_t blend_mode);
#endif  /*LV_DRAW_COMPLEX*/

static void map_set_px(lv_color_t * dest_buf, const lv_area_t * dest_area, lv_coord_t dest_stride,
                       const lv_color_t * src_buf, lv_coord_t src_stride, lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stride);

LV_ATTRIBUTE_FAST_MEM static void map_normal(lv_color_t * dest_buf, const lv_area_t * dest_area, lv_coord_t dest_stride,
                                             const lv_color_t * src_buf, lv_coord_t src_stride, lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stride);

#if LV_COLOR_SCREEN_TRANSP
LV_ATTRIBUTE_FAST_MEM static void map_argb(lv_color_t * dest_buf, const lv_area_t * dest_area, lv_coord_t dest_stride,
                                           const lv_color_t * src_buf, lv_coord_t src_stride, lv_opa_t opa,
                                           const lv_opa_t * mask, lv_coord_t mask_stride, lv_blend_mode_t blend_mode);

#endif /*LV_COLOR_SCREEN_TRANSP*/

#if LV_DRAW_COMPLEX
static void map_blended(lv_color_t * dest_buf, const lv_area_t * dest_area, lv_coord_t dest_stride,
                        const lv_color_t * src_buf, lv_coord_t src_stride, lv_opa_t opa,
                        const lv_opa_t * mask, lv_coord_t mask_stride, lv_blend_mode_t blend_mode);

static inline lv_color_t color_blend_true_color_additive(lv_color_t fg, lv_color_t bg, lv_opa_t opa);
static inline lv_color_t color_blend_true_color_subtractive(lv_color_t fg, lv_color_t bg, lv_opa_t opa);
static inline lv_color_t color_blend_true_color_multiply(lv_color_t fg, lv_color_t bg, lv_opa_t opa);
#endif /*LV_DRAW_COMPLEX*/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#define FILL_NORMAL_MASK_PX(color)                                                          \
    if(*mask == LV_OPA_COVER) *dest_buf = color;                                 \
    else *dest_buf = lv_color_mix(color, *dest_buf, *mask);            \
    mask++;                                                         \
    dest_buf++;

#if !LVGL_USE_PSRAM
#define MAP_NORMAL_MASK_PX(x)                                                          \
    if(*mask_tmp_x) {          \
        if(*mask_tmp_x == LV_OPA_COVER) dest_buf[x] = src_buf[x];                                 \
        else dest_buf[x] = lv_color_mix(src_buf[x], dest_buf[x], *mask_tmp_x);            \
    }                                                                                               \
    mask_tmp_x++;
#else
#define MAP_NORMAL_MASK_PX(x)                                                          \
    if(*mask_tmp_x) {          \
        if(*mask_tmp_x == LV_OPA_COVER) tmp_color = src_buf[x];                                 \
        else tmp_color = lv_color_mix(src_buf[x], dest_buf[x], *mask_tmp_x);            \
    \
        if(((unsigned int)(dest_buf + x) & 0x3) == 0)    \
             *((volatile unsigned int *)(dest_buf + x)) = (dest_buf[x + 1].full << 16) | tmp_color.full;    \
        else    \
            *((volatile unsigned int *)(dest_buf + x -1)) = (tmp_color.full << 16) | dest_buf[x - 1].full;    \
    }                                                                                               \
    mask_tmp_x++;
#endif


/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_sw_blend(lv_draw_ctx_t * draw_ctx, const lv_draw_sw_blend_dsc_t * dsc)
{
    /*Do not draw transparent things*/
    if(dsc->opa <= LV_OPA_MIN) return;

    lv_area_t blend_area;
    if(!_lv_area_intersect(&blend_area, dsc->blend_area, draw_ctx->clip_area)) return;

    if(draw_ctx->wait_for_finish) draw_ctx->wait_for_finish(draw_ctx);

    ((lv_draw_sw_ctx_t *)draw_ctx)->blend(draw_ctx, dsc);
}

LV_ATTRIBUTE_FAST_MEM void lv_draw_sw_blend_basic(lv_draw_ctx_t * draw_ctx, const lv_draw_sw_blend_dsc_t * dsc)
{
    const lv_opa_t * mask;
    if(dsc->mask_buf == NULL) mask = NULL;
    if(dsc->mask_buf && dsc->mask_res == LV_DRAW_MASK_RES_TRANSP) return;
    else if(dsc->mask_res == LV_DRAW_MASK_RES_FULL_COVER) mask = NULL;
    else mask = dsc->mask_buf;

    lv_coord_t dest_stride = lv_area_get_width(draw_ctx->buf_area);

    lv_area_t blend_area;
    if(!_lv_area_intersect(&blend_area, dsc->blend_area, draw_ctx->clip_area)) return;

    lv_disp_t * disp = _lv_refr_get_disp_refreshing();
    lv_color_t * dest_buf = draw_ctx->buf;
    if(disp->driver->set_px_cb == NULL) {
        if(disp->driver->screen_transp == 0) {
            dest_buf += dest_stride * (blend_area.y1 - draw_ctx->buf_area->y1) + (blend_area.x1 - draw_ctx->buf_area->x1);
        }
        else {
            /*With LV_COLOR_DEPTH 16 it means ARGB8565 (3 bytes format)*/
            uint8_t * dest_buf8 = (uint8_t *) dest_buf;
            dest_buf8 += dest_stride * (blend_area.y1 - draw_ctx->buf_area->y1) * LV_IMG_PX_SIZE_ALPHA_BYTE;
            dest_buf8 += (blend_area.x1 - draw_ctx->buf_area->x1) * LV_IMG_PX_SIZE_ALPHA_BYTE;
            dest_buf = (lv_color_t *)dest_buf8;
        }
    }

    const lv_color_t * src_buf = dsc->src_buf;
    lv_coord_t src_stride;
    if(src_buf) {
        src_stride = lv_area_get_width(dsc->blend_area);
        src_buf += src_stride * (blend_area.y1 - dsc->blend_area->y1) + (blend_area.x1 - dsc->blend_area->x1);
    }
    else {
        src_stride = 0;
    }

    lv_coord_t mask_stride;
    if(mask) {
        mask_stride = lv_area_get_width(dsc->mask_area);
        mask += mask_stride * (blend_area.y1 - dsc->mask_area->y1) + (blend_area.x1 - dsc->mask_area->x1);
    }
    else {
        mask_stride = 0;
    }

    lv_area_move(&blend_area, -draw_ctx->buf_area->x1, -draw_ctx->buf_area->y1);


    if(disp->driver->set_px_cb) {
        if(dsc->src_buf == NULL) {
            fill_set_px(dest_buf, &blend_area, dest_stride, dsc->color, dsc->opa, mask, mask_stride);
        }
        else {
            map_set_px(dest_buf, &blend_area, dest_stride, src_buf, src_stride, dsc->opa, mask, mask_stride);
        }
    }
#if LV_COLOR_SCREEN_TRANSP
    else if(disp->driver->screen_transp) {
        if(dsc->src_buf == NULL) {
            fill_argb(dest_buf, &blend_area, dest_stride, dsc->color, dsc->opa, mask, mask_stride);
        }
        else {
            map_argb(dest_buf, &blend_area, dest_stride, src_buf, src_stride, dsc->opa, mask, mask_stride, dsc->blend_mode);
        }
    }
#endif
    else if(dsc->blend_mode == LV_BLEND_MODE_NORMAL) {
        if(dsc->src_buf == NULL) {
            fill_normal(dest_buf, &blend_area, dest_stride, dsc->color, dsc->opa, mask, mask_stride);
        }
        else {
            map_normal(dest_buf, &blend_area, dest_stride, src_buf, src_stride, dsc->opa, mask, mask_stride);
        }
    }
    else {
#if LV_DRAW_COMPLEX
        if(dsc->src_buf == NULL) {
            fill_blended(dest_buf, &blend_area, dest_stride, dsc->color, dsc->opa, mask, mask_stride, dsc->blend_mode);
        }
        else {
            map_blended(dest_buf, &blend_area, dest_stride, src_buf, src_stride, dsc->opa, mask, mask_stride, dsc->blend_mode);
        }
#endif
    }
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

static void fill_set_px(lv_color_t * dest_buf, const lv_area_t * blend_area, lv_coord_t dest_stride,
                        lv_color_t color, lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stide)
{
    lv_disp_t * disp = _lv_refr_get_disp_refreshing();

    int32_t x;
    int32_t y;

    if(mask == NULL) {
        for(y = blend_area->y1; y <= blend_area->y2; y++) {
            for(x = blend_area->x1; x <= blend_area->x2; x++) {
                disp->driver->set_px_cb(disp->driver, (void *)dest_buf, dest_stride, x, y, color, opa);
            }
        }
    }
    else {
        int32_t w = lv_area_get_width(blend_area);
        int32_t h = lv_area_get_height(blend_area);

        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                if(mask[x]) {


                    disp->driver->set_px_cb(disp->driver, (void *)dest_buf, dest_stride, blend_area->x1 + x, blend_area->y1 + y, color,
                                            (uint32_t)((uint32_t)opa * mask[x]) >> 8);
                }
            }
            mask += mask_stide;
        }
    }
}

LV_ATTRIBUTE_FAST_MEM static void fill_normal(lv_color_t * dest_buf_tmp, const lv_area_t * dest_area,
                                              lv_coord_t dest_stride, lv_color_t color, lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stride)
{
    int32_t w = lv_area_get_width(dest_area);
    int32_t h = lv_area_get_height(dest_area);

    int32_t x;
    int32_t y;
    lv_color_t * dest_buf;
#if LVGL_USE_PSRAM
    lv_color_t * dest_buf_bak;
#endif

    #if !LVGL_USE_PSRAM
    dest_buf = dest_buf_tmp;
    #else
    dest_buf = LV_MEM_CUSTOM_ALLOC(w * sizeof(lv_color_t));
    if(!dest_buf)
    {
        bk_printf("[%s][%d] malloc fail\r\n", __FUNCTION__, __LINE__);
        return ;
    }

    dest_buf_bak = dest_buf;
    #endif
    
    /*No mask*/
    if(mask == NULL) {
        if(opa >= LV_OPA_MAX) {
            #if LVGL_USE_PSRAM
            _memset_color(dest_buf, color, w);
            #endif
            for(y = 0; y < h; y++) {
                #if !LVGL_USE_PSRAM
                lv_color_fill(dest_buf, color, w);
                dest_buf += dest_stride;
                #else
                lv_memcpy_one_line(dest_buf_tmp, dest_buf, w);
                dest_buf_tmp += dest_stride;
                #endif
            }
        }
        /*Has opacity*/
        else {
            lv_color_t last_dest_color = lv_color_black();
            lv_color_t last_res_color = lv_color_mix(color, last_dest_color, opa);

#if LV_COLOR_MIX_ROUND_OFS == 0 && LV_COLOR_DEPTH == 16
            /*lv_color_mix work with an optimized algorithm with 16 bit color depth.
             *However, it introduces some rounded error on opa.
             *Introduce the same error here too to make lv_color_premult produces the same result */
            opa = (uint32_t)((uint32_t)opa + 4) >> 3;
            opa = opa << 3;
#endif

            uint16_t color_premult[3];
            lv_color_premult(color, opa, color_premult);
            lv_opa_t opa_inv = 255 - opa;

            for(y = 0; y < h; y++) {
                #if LVGL_USE_PSRAM
                dest_buf = dest_buf_bak;
                lv_memcpy_one_line(dest_buf, dest_buf_tmp, w);
                #endif
                
                for(x = 0; x < w; x++) {
                    if(last_dest_color.full != dest_buf[x].full) {
                        last_dest_color = dest_buf[x];
                        last_res_color = lv_color_mix_premult(color_premult, dest_buf[x], opa_inv);
                    }

                    dest_buf[x] = last_res_color;
                }

                #if !LVGL_USE_PSRAM
                dest_buf += dest_stride;
                #else
                lv_memcpy_one_line(dest_buf_tmp, dest_buf, w);
                dest_buf_tmp += dest_stride;
                #endif
            }
        }
    }
    /*Masked*/
    else {
#if LV_COLOR_DEPTH == 16
        uint32_t c32 = color.full + ((uint32_t)color.full << 16);
#endif
        /*Only the mask matters*/
        if(opa >= LV_OPA_MAX) {
            int32_t x_end4 = w - 4;
            for(y = 0; y < h; y++) {
                #if LVGL_USE_PSRAM
                dest_buf = dest_buf_bak;
                lv_memcpy_one_line(dest_buf, dest_buf_tmp, w);
                #endif
                
                for(x = 0; x < w && ((lv_uintptr_t)(mask) & 0x3); x++) {
                    FILL_NORMAL_MASK_PX(color)
                }

                for(; x <= x_end4; x += 4) {
                    uint32_t mask32 = *((uint32_t *)mask);
                    if(mask32 == 0xFFFFFFFF) {
#if LV_COLOR_DEPTH == 16
                        if((lv_uintptr_t)dest_buf & 0x3) {
                            *(dest_buf + 0) = color;
                            uint32_t * d = (uint32_t *)(dest_buf + 1);
                            *d = c32;
                            *(dest_buf + 3) = color;
                        }
                        else {
                            uint32_t * d = (uint32_t *)dest_buf;
                            *d = c32;
                            *(d + 1) = c32;
                        }
#else
                        dest_buf[0] = color;
                        dest_buf[1] = color;
                        dest_buf[2] = color;
                        dest_buf[3] = color;
#endif
                        dest_buf += 4;
                        mask += 4;
                    }
                    else if(mask32) {
                        FILL_NORMAL_MASK_PX(color)
                        FILL_NORMAL_MASK_PX(color)
                        FILL_NORMAL_MASK_PX(color)
                        FILL_NORMAL_MASK_PX(color)
                    }
                    else {
                        mask += 4;
                        dest_buf += 4;
                    }
                }

                for(; x < w ; x++) {
                    FILL_NORMAL_MASK_PX(color)
                }
                #if !LVGL_USE_PSRAM
                dest_buf += (dest_stride - w);
                #else
                lv_memcpy_one_line(dest_buf_tmp, dest_buf_bak, w);
                dest_buf_tmp += dest_stride;
                #endif
                mask += (mask_stride - w);
            }
        }
        /*With opacity*/
        else {
            /*Buffer the result color to avoid recalculating the same color*/
            lv_color_t last_dest_color;
            lv_color_t last_res_color;
            lv_opa_t last_mask = LV_OPA_TRANSP;
            last_dest_color.full = dest_buf[0].full;
            last_res_color.full = dest_buf[0].full;
            lv_opa_t opa_tmp = LV_OPA_TRANSP;

            for(y = 0; y < h; y++) {
                #if LVGL_USE_PSRAM
                dest_buf = dest_buf_bak;
                lv_memcpy_one_line(dest_buf, dest_buf_tmp, w);
                #endif
                
                for(x = 0; x < w; x++) {
                    if(*mask) {
                        if(*mask != last_mask) opa_tmp = *mask == LV_OPA_COVER ? opa :
                                                             (uint32_t)((uint32_t)(*mask) * opa) >> 8;
                        if(*mask != last_mask || last_dest_color.full != dest_buf[x].full) {
                            if(opa_tmp == LV_OPA_COVER) last_res_color = color;
                            else last_res_color = lv_color_mix(color, dest_buf[x], opa_tmp);
                            last_mask = *mask;
                            last_dest_color.full = dest_buf[x].full;
                        }
                        dest_buf[x] = last_res_color;
                    }
                    mask++;
                }
                #if !LVGL_USE_PSRAM
                dest_buf += dest_stride;
                #else
                lv_memcpy_one_line(dest_buf_tmp, dest_buf_bak, w);
                dest_buf_tmp += dest_stride;
                #endif
                mask += (mask_stride - w);
            }
        }
    }

#if LVGL_USE_PSRAM
    if(dest_buf_bak)
    {
        LV_MEM_CUSTOM_FREE(dest_buf_bak);
        dest_buf_bak = NULL;
    }
#endif
}

#if LV_COLOR_SCREEN_TRANSP
static inline void set_px_argb(uint8_t * buf, lv_color_t color, lv_opa_t opa)
{
    lv_color_t bg_color;
    lv_color_t res_color;
    lv_opa_t bg_opa = buf[LV_IMG_PX_SIZE_ALPHA_BYTE - 1];
#if LV_COLOR_DEPTH == 8
    bg_color.full = buf[0];
    lv_color_mix_with_alpha(bg_color, bg_opa, color, opa, &res_color, &buf[1]);
    if(buf[1] <= LV_OPA_MIN) return;
    buf[0] = res_color.full;
#elif LV_COLOR_DEPTH == 16
    bg_color.full = buf[0] + (buf[1] << 8);
    lv_color_mix_with_alpha(bg_color, bg_opa, color, opa, &res_color, &buf[2]);
    if(buf[2] <= LV_OPA_MIN) return;
    buf[0] = res_color.full & 0xff;
    buf[1] = res_color.full >> 8;
#elif LV_COLOR_DEPTH == 32
    lv_opa_t opa_tmp;

    bg_color = *((lv_color_t *)buf);
    lv_color_mix_with_alpha(bg_color, bg_opa, color, opa, &res_color, &opa_tmp);
    if(opa_tmp <= LV_OPA_MIN) return;
    #if 0
    buf[0] = res_color.ch.blue;
    buf[1] = res_color.ch.green;
    buf[2] = res_color.ch.red;
    buf[3] = opa;
    #else
    res_color.ch.alpha = opa_tmp;
    *((unsigned int *)buf) = res_color.full;
    #endif
#endif
}

static inline void set_px_argb_blend(uint8_t * buf, lv_color_t color, lv_opa_t opa, lv_color_t (*blend_fp)(lv_color_t,
                                                                                                           lv_color_t, lv_opa_t))
{
    static lv_color_t last_dest_color;
    static lv_color_t last_src_color;
    static lv_color_t last_res_color;
    static uint32_t last_opa = 0xffff; /*Set to an invalid value for first*/

    lv_color_t bg_color;

    /*Get the BG color*/
#if LV_COLOR_DEPTH == 8
    if(buf[1] <= LV_OPA_MIN) return;
    bg_color.full = buf[0];
#elif LV_COLOR_DEPTH == 16
    if(buf[2] <= LV_OPA_MIN) return;
    bg_color.full = buf[0] + (buf[1] << 8);
#elif LV_COLOR_DEPTH == 32
    if(buf[3] <= LV_OPA_MIN) return;
    bg_color = *((lv_color_t *)buf);
#endif

    /*Get the result color*/
    if(last_dest_color.full != bg_color.full || last_src_color.full != color.full || last_opa != opa) {
        last_dest_color = bg_color;
        last_src_color = color;
        last_opa = opa;
        last_res_color = blend_fp(last_src_color, last_dest_color, last_opa);
    }

    /*Set the result color*/
#if LV_COLOR_DEPTH == 8
    buf[0] = res_color.full;
#elif LV_COLOR_DEPTH == 16
    buf[0] = last_res_color.full & 0xff;
    buf[1] = last_res_color.full >> 8;
#elif LV_COLOR_DEPTH == 32
    buf[0] = last_res_color.ch.blue;
    buf[1] = last_res_color.ch.green;
    buf[2] = last_res_color.ch.red;
#endif

}

LV_ATTRIBUTE_FAST_MEM static void fill_argb(lv_color_t * dest_buf, const lv_area_t * dest_area,
                                            lv_coord_t dest_stride, lv_color_t color, lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stride)
{
    uint8_t * dest_buf8 = (uint8_t *) dest_buf;
    int32_t w = lv_area_get_width(dest_area);
    int32_t h = lv_area_get_height(dest_area);

    int32_t x;
    int32_t y;

    uint8_t ctmp[LV_IMG_PX_SIZE_ALPHA_BYTE];
    lv_memcpy(ctmp, &color, sizeof(lv_color_t));
    ctmp[LV_IMG_PX_SIZE_ALPHA_BYTE - 1] = opa;

    /*No mask*/
    if(mask == NULL) {
        if(opa >= LV_OPA_MAX) {
            for(x = 0; x < w; x++) {
                lv_memcpy(dest_buf8, ctmp, LV_IMG_PX_SIZE_ALPHA_BYTE);
                dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
            }

            dest_buf8 += (dest_stride - w) * LV_IMG_PX_SIZE_ALPHA_BYTE;

            for(y = 1; y < h; y++) {
                lv_memcpy(dest_buf8, (uint8_t *) dest_buf, w * LV_IMG_PX_SIZE_ALPHA_BYTE);
                dest_buf8 += dest_stride * LV_IMG_PX_SIZE_ALPHA_BYTE;
            }
        }
        /*Has opacity*/
        else {
            uint8_t * dest_buf8_row = dest_buf8;
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    set_px_argb(dest_buf8, color, opa);
                    dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
                }
                dest_buf8_row += dest_stride * LV_IMG_PX_SIZE_ALPHA_BYTE;
                dest_buf8 = dest_buf8_row;
            }
        }
    }
    /*Masked*/
    else {
        /*Only the mask matters*/
        if(opa >= LV_OPA_MAX) {
            uint8_t * dest_buf8_row = dest_buf8;
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    set_px_argb(dest_buf8, color,  *mask);
                    mask++;
                    dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
                }
                dest_buf8_row += dest_stride * LV_IMG_PX_SIZE_ALPHA_BYTE;
                dest_buf8 = dest_buf8_row;
            }
        }
        /*With opacity*/
        else {
            /*Buffer the result color to avoid recalculating the same color*/
            lv_opa_t last_mask = LV_OPA_TRANSP;
            lv_opa_t opa_tmp = LV_OPA_TRANSP;

            uint8_t * dest_buf8_row = dest_buf8;
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    if(*mask) {
                        if(*mask != last_mask) opa_tmp = *mask == LV_OPA_COVER ? opa :
                                                             (uint32_t)((uint32_t)(*mask) * opa) >> 8;

                        set_px_argb(dest_buf8, color,  opa_tmp);
                    }
                    dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
                    mask++;
                }
                dest_buf8_row += dest_stride * LV_IMG_PX_SIZE_ALPHA_BYTE;
                dest_buf8 = dest_buf8_row;
                mask += (mask_stride - w);
            }
        }
    }
}
#endif

#if LV_DRAW_COMPLEX
static void fill_blended(lv_color_t * dest_buf, const lv_area_t * dest_area,
                         lv_coord_t dest_stride, lv_color_t color, lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stride,
                         lv_blend_mode_t blend_mode)
{

    int32_t w = lv_area_get_width(dest_area);
    int32_t h = lv_area_get_height(dest_area);

    int32_t x;
    int32_t y;

    lv_color_t (*blend_fp)(lv_color_t, lv_color_t, lv_opa_t);
    switch(blend_mode) {
        case LV_BLEND_MODE_ADDITIVE:
            blend_fp = color_blend_true_color_additive;
            break;
        case LV_BLEND_MODE_SUBTRACTIVE:
            blend_fp = color_blend_true_color_subtractive;
            break;
        case LV_BLEND_MODE_MULTIPLY:
            blend_fp = color_blend_true_color_multiply;
            break;
        default:
            LV_LOG_WARN("fill_blended: unsupported blend mode");
            return;
    }

    /*Simple fill (maybe with opacity), no masking*/
    if(mask == NULL) {
        lv_color_t last_dest_color = dest_buf[0];
        lv_color_t last_res_color = blend_fp(color, dest_buf[0], opa);
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                if(last_dest_color.full != dest_buf[x].full) {
                    last_dest_color = dest_buf[x];
                    last_res_color = blend_fp(color, dest_buf[x], opa);
                }
                #if !LVGL_USE_PSRAM
                dest_buf[x] = last_res_color;
                #else
                if(((unsigned int)(dest_buf + x) & 0x3) == 0)
                     *((volatile unsigned int *)(dest_buf + x)) = (dest_buf[x + 1].full << 16) | last_res_color.full;
                else
                    *((volatile unsigned int *)(dest_buf + x - 1)) = (last_res_color.full << 16) | dest_buf[x - 1].full;
                #endif
            }
            dest_buf += dest_stride;
        }
    }
    /*Masked*/
    else {
        /*Buffer the result color to avoid recalculating the same color*/
        lv_color_t last_dest_color;
        lv_color_t last_res_color;
        lv_opa_t last_mask = LV_OPA_TRANSP;
        last_dest_color = dest_buf[0];
        lv_opa_t opa_tmp = mask[0] >= LV_OPA_MAX ? opa : (uint32_t)((uint32_t)mask[0] * opa) >> 8;
        last_res_color = blend_fp(color, last_dest_color, opa_tmp);

        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                if(mask[x] == 0) continue;
                if(mask[x] != last_mask || last_dest_color.full != dest_buf[x].full) {
                    opa_tmp = mask[x] >= LV_OPA_MAX ? opa : (uint32_t)((uint32_t)mask[x] * opa) >> 8;

                    last_res_color = blend_fp(color, dest_buf[x], opa_tmp);
                    last_mask = mask[x];
                    last_dest_color.full = dest_buf[x].full;
                }
                #if !LVGL_USE_PSRAM
                dest_buf[x] = last_res_color;
                #else
                if(((unsigned int)(dest_buf + x) & 0x3) == 0)
                     *((volatile unsigned int *)(dest_buf + x)) = (dest_buf[x + 1].full << 16) | last_res_color.full;
                else
                    *((volatile unsigned int *)(dest_buf + x - 1)) = (last_res_color.full << 16) | dest_buf[x - 1].full;
                #endif
            }
            dest_buf += dest_stride;
            mask += mask_stride;
        }
    }
}
#endif

static void map_set_px(lv_color_t * dest_buf, const lv_area_t * dest_area, lv_coord_t dest_stride,
                       const lv_color_t * src_buf, lv_coord_t src_stride, lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stride)

{
    lv_disp_t * disp = _lv_refr_get_disp_refreshing();

    int32_t w = lv_area_get_width(dest_area);
    int32_t h = lv_area_get_height(dest_area);

    int32_t x;
    int32_t y;

    if(mask == NULL) {
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                disp->driver->set_px_cb(disp->driver, (void *)dest_buf, dest_stride, dest_area->x1 + x, dest_area->y1 + y, src_buf[x],
                                        opa);
            }
            src_buf += src_stride;
        }
    }
    else {
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                if(mask[x]) {
                    disp->driver->set_px_cb(disp->driver, (void *)dest_buf, dest_stride, dest_area->x1 + x, dest_area->y1 + y, src_buf[x],
                                            (uint32_t)((uint32_t)opa * mask[x]) >> 8);
                }
            }
            mask += mask_stride;
            src_buf += src_stride;
        }
    }
}

LV_ATTRIBUTE_FAST_MEM static void map_normal(lv_color_t * dest_buf, const lv_area_t * dest_area, lv_coord_t dest_stride,
                                             const lv_color_t * src_buf, lv_coord_t src_stride, lv_opa_t opa, const lv_opa_t * mask, lv_coord_t mask_stride)

{
    int32_t w = lv_area_get_width(dest_area);
    int32_t h = lv_area_get_height(dest_area);

    int32_t x;
    int32_t y;
#if LVGL_USE_PSRAM
    lv_color_t tmp_color = {0};
#endif

    /*Simple fill (maybe with opacity), no masking*/
    if(mask == NULL) {
        if(opa >= LV_OPA_MAX) {
            for(y = 0; y < h; y++) {
                #if !LVGL_USE_PSRAM
                lv_memcpy(dest_buf, src_buf, w * sizeof(lv_color_t));
                #else
                lv_memcpy_one_line(dest_buf, src_buf, w);
                #endif
                dest_buf += dest_stride;
                src_buf += src_stride;
            }
        }
        else {
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    #if !LVGL_USE_PSRAM
                    dest_buf[x] = lv_color_mix(src_buf[x], dest_buf[x], opa);
                    #else
                    tmp_color = lv_color_mix(src_buf[x], dest_buf[x], opa);
                    if(((unsigned int)(dest_buf + x) & 0x3) == 0)
                         *((volatile unsigned int *)(dest_buf + x)) = (dest_buf[x + 1].full << 16) | tmp_color.full;
                    else
                        *((volatile unsigned int *)(dest_buf + x - 1)) = (tmp_color.full << 16) | dest_buf[x - 1].full;
                    #endif
                }
                dest_buf += dest_stride;
                src_buf += src_stride;
            }
        }
    }
    /*Masked*/
    else {
        /*Only the mask matters*/
        if(opa > LV_OPA_MAX) {
            int32_t x_end4 = w - 4;

            for(y = 0; y < h; y++) {
                const lv_opa_t * mask_tmp_x = mask;
#if 0
                for(x = 0; x < w; x++) {
                    MAP_NORMAL_MASK_PX(x);
                }
#else
                for(x = 0; x < w && ((lv_uintptr_t)mask_tmp_x & 0x3); x++) {
                    MAP_NORMAL_MASK_PX(x)
                }

                uint32_t * mask32 = (uint32_t *)mask_tmp_x;
                for(; x < x_end4; x += 4) {
                    if(*mask32) {
                        if((*mask32) == 0xFFFFFFFF) {
                            #if !LVGL_USE_PSRAM
                            dest_buf[x] = src_buf[x];
                            dest_buf[x + 1] = src_buf[x + 1];
                            dest_buf[x + 2] = src_buf[x + 2];
                            dest_buf[x + 3] = src_buf[x + 3];
                            #else
                            if(((unsigned int)(dest_buf + x) & 0x3) == 0)
                            {
                                 *((volatile unsigned int *)(dest_buf + x)) = (src_buf[x + 1].full << 16) | src_buf[x].full;
                                 *((volatile unsigned int *)(dest_buf + x + 2)) = (src_buf[x + 3].full << 16) | src_buf[x + 2].full;
                            }
                            else
                            {
                                 *((volatile unsigned int *)(dest_buf + x - 1)) = (src_buf[x].full << 16) | dest_buf[x - 1].full;
                                 *((volatile unsigned int *)(dest_buf + x + 1)) = (src_buf[x + 2].full << 16) | src_buf[x + 1].full;
                                 *((volatile unsigned int *)(dest_buf + x + 3)) = (dest_buf[x + 4].full << 16) | src_buf[x + 3].full;
                            }
                            #endif
                        }
                        else {
                            mask_tmp_x = (const lv_opa_t *)mask32;
                            MAP_NORMAL_MASK_PX(x)
                            MAP_NORMAL_MASK_PX(x + 1)
                            MAP_NORMAL_MASK_PX(x + 2)
                            MAP_NORMAL_MASK_PX(x + 3)
                        }
                    }
                    mask32++;
                }

                mask_tmp_x = (const lv_opa_t *)mask32;
                for(; x < w ; x++) {
                    MAP_NORMAL_MASK_PX(x)
                }
#endif
                dest_buf += dest_stride;
                src_buf += src_stride;
                mask += mask_stride;
            }
        }
        /*Handle opa and mask values too*/
        else {
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    if(mask[x]) {
                        lv_opa_t opa_tmp = mask[x] >= LV_OPA_MAX ? opa : ((opa * mask[x]) >> 8);
                        #if !LVGL_USE_PSRAM
                        dest_buf[x] = lv_color_mix(src_buf[x], dest_buf[x], opa_tmp);
                        #else
                        tmp_color = lv_color_mix(src_buf[x], dest_buf[x], opa_tmp);
                        if(((unsigned int)(dest_buf + x) & 0x3) == 0)
                             *((volatile unsigned int *)(dest_buf + x)) = (dest_buf[x + 1].full << 16) | tmp_color.full;
                        else
                            *((volatile unsigned int *)(dest_buf + x - 1)) = (tmp_color.full << 16) | dest_buf[x - 1].full;
                        #endif
                    }
                }
                dest_buf += dest_stride;
                src_buf += src_stride;
                mask += mask_stride;
            }
        }
    }
}



#if LV_COLOR_SCREEN_TRANSP
LV_ATTRIBUTE_FAST_MEM static void map_argb(lv_color_t * dest_buf, const lv_area_t * dest_area, lv_coord_t dest_stride,
                                           const lv_color_t * src_buf, lv_coord_t src_stride, lv_opa_t opa,
                                           const lv_opa_t * mask, lv_coord_t mask_stride, lv_blend_mode_t blend_mode)

{
    uint8_t * dest_buf8 = (uint8_t *) dest_buf;

    int32_t w = lv_area_get_width(dest_area);
    int32_t h = lv_area_get_height(dest_area);

    int32_t x;
    int32_t y;

    lv_color_t (*blend_fp)(lv_color_t, lv_color_t, lv_opa_t);
    switch(blend_mode) {
        case LV_BLEND_MODE_ADDITIVE:
            blend_fp = color_blend_true_color_additive;
            break;
        case LV_BLEND_MODE_SUBTRACTIVE:
            blend_fp = color_blend_true_color_subtractive;
            break;
        case LV_BLEND_MODE_MULTIPLY:
            blend_fp = color_blend_true_color_multiply;
            break;
        default:
            blend_fp = NULL;
    }

    /*Simple fill (maybe with opacity), no masking*/
    if(mask == NULL) {
        if(opa >= LV_OPA_MAX) {
            if(blend_fp == NULL && LV_COLOR_DEPTH == 32) {
                for(y = 0; y < h; y++) {
                    //lv_memcpy(dest_buf, src_buf, w * sizeof(lv_color_t));
                    lv_memcpy_one_line(dest_buf, src_buf, w);
                    dest_buf += dest_stride;
                    src_buf += src_stride;
                }
            }
            else {
                uint8_t * dest_buf8_row = dest_buf8;
                for(y = 0; y < h; y++) {
                    if(blend_fp == NULL) {
                        for(x = 0; x < w; x++) {
                            set_px_argb(dest_buf8, src_buf[x], LV_OPA_COVER);
                            dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
                        }
                    }
                    else {
                        for(x = 0; x < w; x++) {
                            set_px_argb_blend(dest_buf8, src_buf[x], LV_OPA_COVER, blend_fp);
                            dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
                        }
                    }

                    dest_buf8_row += dest_stride * LV_IMG_PX_SIZE_ALPHA_BYTE;
                    dest_buf8 = dest_buf8_row;
                    src_buf += src_stride;
                }
            }
        }
        /*No mask but opacity*/
        else {
            uint8_t * dest_buf8_row = dest_buf8;
            for(y = 0; y < h; y++) {
                if(blend_fp == NULL) {
                    for(x = 0; x < w; x++) {
                        set_px_argb(dest_buf8, src_buf[x], opa);
                        dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
                    }
                }
                else {
                    for(x = 0; x < w; x++) {
                        set_px_argb_blend(dest_buf8, src_buf[x], opa, blend_fp);
                        dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
                    }
                }

                dest_buf8_row += dest_stride * LV_IMG_PX_SIZE_ALPHA_BYTE;
                dest_buf8 = dest_buf8_row;
                src_buf += src_stride;
            }
        }
    }
    /*Masked*/
    else {
        /*Only the mask matters*/
        if(opa > LV_OPA_MAX) {
            uint8_t * dest_buf8_row = dest_buf8;
            for(y = 0; y < h; y++) {
                if(blend_fp == NULL) {
                    for(x = 0; x < w; x++) {
                        set_px_argb(dest_buf8, src_buf[x], mask[x]);
                        dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
                    }
                }
                else {
                    for(x = 0; x < w; x++) {
                        set_px_argb_blend(dest_buf8, src_buf[x], mask[x], blend_fp);
                        dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
                    }
                }
                dest_buf8_row += dest_stride * LV_IMG_PX_SIZE_ALPHA_BYTE;
                dest_buf8 = dest_buf8_row;
                src_buf += src_stride;
                mask += mask_stride;
            }
        }
        /*Handle opa and mask values too*/
        else {
            uint8_t * dest_buf8_row = dest_buf8;
            for(y = 0; y < h; y++) {
                if(blend_fp == NULL) {
                    for(x = 0; x < w; x++) {
                        if(mask[x]) {
                            lv_opa_t opa_tmp = mask[x] >= LV_OPA_MAX ? opa : ((opa * mask[x]) >> 8);
                            set_px_argb(dest_buf8, src_buf[x], opa_tmp);
                        }
                        dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
                    }
                }
                else {
                    for(x = 0; x < w; x++) {
                        if(mask[x]) {
                            lv_opa_t opa_tmp = mask[x] >= LV_OPA_MAX ? opa : ((opa * mask[x]) >> 8);
                            set_px_argb_blend(dest_buf8, src_buf[x], opa_tmp, blend_fp);
                        }
                        dest_buf8 += LV_IMG_PX_SIZE_ALPHA_BYTE;
                    }
                }
                dest_buf8_row += dest_stride * LV_IMG_PX_SIZE_ALPHA_BYTE;
                dest_buf8 = dest_buf8_row;
                src_buf += src_stride;
                mask += mask_stride;
            }
        }
    }
}
#endif


#if LV_DRAW_COMPLEX
static void map_blended(lv_color_t * dest_buf, const lv_area_t * dest_area, lv_coord_t dest_stride,
                        const lv_color_t * src_buf, lv_coord_t src_stride, lv_opa_t opa,
                        const lv_opa_t * mask, lv_coord_t mask_stride, lv_blend_mode_t blend_mode)
{

    int32_t w = lv_area_get_width(dest_area);
    int32_t h = lv_area_get_height(dest_area);

    int32_t x;
    int32_t y;

    lv_color_t (*blend_fp)(lv_color_t, lv_color_t, lv_opa_t);
    switch(blend_mode) {
        case LV_BLEND_MODE_ADDITIVE:
            blend_fp = color_blend_true_color_additive;
            break;
        case LV_BLEND_MODE_SUBTRACTIVE:
            blend_fp = color_blend_true_color_subtractive;
            break;
        case LV_BLEND_MODE_MULTIPLY:
            blend_fp = color_blend_true_color_multiply;
            break;
        default:
            LV_LOG_WARN("fill_blended: unsupported blend mode");
            return;
    }

    lv_color_t last_dest_color;
    lv_color_t last_src_color;
    /*Simple fill (maybe with opacity), no masking*/
    if(mask == NULL) {
        last_dest_color = dest_buf[0];
        last_src_color = src_buf[0];
        lv_color_t last_res_color = blend_fp(last_src_color, last_dest_color, opa);
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                if(last_src_color.full != src_buf[x].full || last_dest_color.full != dest_buf[x].full) {
                    last_dest_color = dest_buf[x];
                    last_src_color = src_buf[x];
                    last_res_color = blend_fp(last_src_color, last_dest_color, opa);
                }
                #if !LVGL_USE_PSRAM
                dest_buf[x] = last_res_color;
                #else
                if(((unsigned int)(dest_buf + x) & 0x3) == 0)
                     *((volatile unsigned int *)(dest_buf + x)) = (dest_buf[x + 1].full << 16) | last_res_color.full;
                else
                    *((volatile unsigned int *)(dest_buf + x - 1)) = (last_res_color.full << 16) | dest_buf[x - 1].full;
                #endif
            }
            dest_buf += dest_stride;
            src_buf += src_stride;
        }
    }
    /*Masked*/
    else {
        last_dest_color = dest_buf[0];
        last_src_color = src_buf[0];
        lv_opa_t last_opa = mask[0] >= LV_OPA_MAX ? opa : ((opa * mask[0]) >> 8);
        lv_color_t last_res_color = blend_fp(last_src_color, last_dest_color, last_opa);
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                if(mask[x] == 0) continue;
                lv_opa_t opa_tmp = mask[x] >= LV_OPA_MAX ? opa : ((opa * mask[x]) >> 8);
                if(last_src_color.full != src_buf[x].full || last_dest_color.full != dest_buf[x].full || last_opa != opa_tmp) {
                    last_dest_color = dest_buf[x];
                    last_src_color = src_buf[x];
                    last_opa = opa_tmp;
                    last_res_color = blend_fp(last_src_color, last_dest_color, last_opa);
                }
                #if !LVGL_USE_PSRAM
                dest_buf[x] = last_res_color;
                #else
                if(((unsigned int)(dest_buf + x) & 0x3) == 0)
                     *((volatile unsigned int *)(dest_buf + x)) = (dest_buf[x + 1].full << 16) | last_res_color.full;
                else
                    *((volatile unsigned int *)(dest_buf + x - 1)) = (last_res_color.full << 16) | dest_buf[x - 1].full;
                #endif
            }
            dest_buf += dest_stride;
            src_buf += src_stride;
            mask += mask_stride;
        }
    }
}

static inline lv_color_t color_blend_true_color_additive(lv_color_t fg, lv_color_t bg, lv_opa_t opa)
{

    if(opa <= LV_OPA_MIN) return bg;

    uint32_t tmp;
#if LV_COLOR_DEPTH == 1
    tmp = bg.full + fg.full;
    fg.full = LV_MIN(tmp, 1);
#else
    tmp = bg.ch.red + fg.ch.red;
#if LV_COLOR_DEPTH == 8
    fg.ch.red = LV_MIN(tmp, 7);
#elif LV_COLOR_DEPTH == 16
    fg.ch.red = LV_MIN(tmp, 31);
#elif LV_COLOR_DEPTH == 32
    fg.ch.red = LV_MIN(tmp, 255);
#endif

#if LV_COLOR_DEPTH == 8
    tmp = bg.ch.green + fg.ch.green;
    fg.ch.green = LV_MIN(tmp, 7);
#elif LV_COLOR_DEPTH == 16
#if LV_COLOR_16_SWAP == 0
    tmp = bg.ch.green + fg.ch.green;
    fg.ch.green = LV_MIN(tmp, 63);
#else
    tmp = (bg.ch.green_h << 3) + bg.ch.green_l + (fg.ch.green_h << 3) + fg.ch.green_l;
    tmp = LV_MIN(tmp, 63);
    fg.ch.green_h = tmp >> 3;
    fg.ch.green_l = tmp & 0x7;
#endif

#elif LV_COLOR_DEPTH == 32
    tmp = bg.ch.green + fg.ch.green;
    fg.ch.green = LV_MIN(tmp, 255);
#endif

    tmp = bg.ch.blue + fg.ch.blue;
#if LV_COLOR_DEPTH == 8
    fg.ch.blue = LV_MIN(tmp, 4);
#elif LV_COLOR_DEPTH == 16
    fg.ch.blue = LV_MIN(tmp, 31);
#elif LV_COLOR_DEPTH == 32
    fg.ch.blue = LV_MIN(tmp, 255);
#endif
#endif

    if(opa == LV_OPA_COVER) return fg;

    return lv_color_mix(fg, bg, opa);
}

static inline lv_color_t color_blend_true_color_subtractive(lv_color_t fg, lv_color_t bg, lv_opa_t opa)
{
    if(opa <= LV_OPA_MIN) return bg;

    int32_t tmp;
    tmp = bg.ch.red - fg.ch.red;
    fg.ch.red = LV_MAX(tmp, 0);

#if LV_COLOR_16_SWAP == 0
    tmp = bg.ch.green - fg.ch.green;
    fg.ch.green = LV_MAX(tmp, 0);
#else
    tmp = (bg.ch.green_h << 3) + bg.ch.green_l + (fg.ch.green_h << 3) + fg.ch.green_l;
    tmp = LV_MAX(tmp, 0);
    fg.ch.green_h = tmp >> 3;
    fg.ch.green_l = tmp & 0x7;
#endif

    tmp = bg.ch.blue - fg.ch.blue;
    fg.ch.blue = LV_MAX(tmp, 0);

    if(opa == LV_OPA_COVER) return fg;

    return lv_color_mix(fg, bg, opa);
}

static inline lv_color_t color_blend_true_color_multiply(lv_color_t fg, lv_color_t bg, lv_opa_t opa)
{
    if(opa <= LV_OPA_MIN) return bg;

#if LV_COLOR_DEPTH == 32
    fg.ch.red = (fg.ch.red * bg.ch.red) >> 8;
    fg.ch.green = (fg.ch.green * bg.ch.green) >> 8;
    fg.ch.blue = (fg.ch.blue * bg.ch.blue) >> 8;
#elif LV_COLOR_DEPTH == 16
    fg.ch.red = (fg.ch.red * bg.ch.red) >> 5;
    fg.ch.blue = (fg.ch.blue * bg.ch.blue) >> 5;
    LV_COLOR_SET_G(fg, (LV_COLOR_GET_G(fg) * LV_COLOR_GET_G(bg)) >> 6);
#elif LV_COLOR_DEPTH == 8
    fg.ch.red = (fg.ch.red * bg.ch.red) >> 3;
    fg.ch.green = (fg.ch.green * bg.ch.green) >> 3;
    fg.ch.blue = (fg.ch.blue * bg.ch.blue) >> 2;
#endif

    if(opa == LV_OPA_COVER) return fg;

    return lv_color_mix(fg, bg, opa);
}

#endif



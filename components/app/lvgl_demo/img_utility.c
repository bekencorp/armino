#include <common/bk_include.h>
#include "lvgl.h"
#include "os/mem.h"
#include "driver/lcd.h"
#include <soc/mapping.h>
#include "../media/include/frame_buffer.h"
#include "../media/include/storage_act.h"
#include "../../demos/media/video/jpeg_dec/tjpgd.h"
#include <components/jpeg_decode.h>

#if (CONFIG_LCD) && (CONFIG_LVGL) && (CONFIG_LVGL_DEMO)
static s32 _dec_img(frame_buffer_t *jpeg_frame, lv_img_dsc_t *img_dst)
{
    s32 ret = BK_FAIL;
#if CONFIG_JPEG_DECODE
    sw_jpeg_dec_res_t result;
    int flag = 0;

    do{
        ret = bk_jpeg_get_img_info(jpeg_frame->length, jpeg_frame->frame, &result);
        if (ret != BK_OK)
        {
            bk_printf("[%s][%d] get img info fail:%d\r\n", __FUNCTION__, __LINE__, ret);
            ret = BK_FAIL;
            break;
        }

        img_dst->header.w = result.pixel_x;
        img_dst->header.h = result.pixel_y;
        img_dst->data_size = img_dst->header.w*img_dst->header.h*2;
        if(img_dst->data == NULL)
        {
            img_dst->data = psram_malloc(img_dst->data_size);
            if(!img_dst->data)
            {
                bk_printf("[%s][%d] malloc psram size %d fail\r\n", __FUNCTION__, __LINE__, img_dst->data_size);
                ret = BK_ERR_NO_MEM;
                break;
            }

            flag = 1;
        }

        ret = bk_jpeg_dec_sw_start(jpeg_frame->length, jpeg_frame->frame, (uint8_t *)img_dst->data, &result);
        if (ret != BK_OK)
        {
            bk_printf("[%s][%d] sw decoder error\r\n", __FUNCTION__, __LINE__);
            if(flag)
            {
                psram_free((void *)img_dst->data);
                img_dst->data = NULL;
            }
            break;
        }

        img_dst->header.always_zero = 0;
        img_dst->header.cf = LV_IMG_CF_TRUE_COLOR;
    }while(0);
#else
    ret = BK_ERR_NOT_SUPPORT;
    bk_printf("[%s][%d] don't support sw dec, CONFIG_JPEG_DECODE should open\r\n", __FUNCTION__, __LINE__);
#endif

    return ret;
}

static frame_buffer_t *_read_file_from_sdcard(char *file_name)
{
    frame_buffer_t *jpeg_frame = NULL;
#if CONFIG_SDCARD_HOST
    int file_len = 0;
    int ret = 0;

    do{
        file_len = sdcard_read_filelen(file_name);
        if(file_len <= 0)
        {
            bk_printf("[%s][%d] %s don't exit in sdcard\r\n", __FUNCTION__, __LINE__, file_name);
            break;
        }

        jpeg_frame = os_malloc(sizeof(frame_buffer_t));
        if(!jpeg_frame)
        {
            bk_printf("[%s][%d] malloc fail\r\n", __FUNCTION__, __LINE__);
            break;
        }

        memset(jpeg_frame, 0, sizeof(frame_buffer_t));
        jpeg_frame->frame = psram_malloc(file_len);
        if(!jpeg_frame->frame)
        {
            os_free(jpeg_frame);
            jpeg_frame = NULL;
            bk_printf("[%s][%d] psram malloc fail\r\n", __FUNCTION__, __LINE__);
            break;
        }

        ret = sdcard_read_to_mem((char *)file_name, (uint32_t *)jpeg_frame->frame, &jpeg_frame->length);
        if(BK_OK != ret)
        {
            bk_printf("%s, sd read file failed\n", __func__);
            os_free(jpeg_frame);
            jpeg_frame = NULL;

            psram_free(jpeg_frame->frame);
            jpeg_frame->frame = NULL;
        }
    }while(0);
#else
    bk_printf("%s, don't support sd, should open CONFIG_SDCARD_HOST\n", __func__);
#endif

    return jpeg_frame;
}

static s32 _dec_img_file_from_sdcard(char *file_name, lv_img_dsc_t *img_dst)
{
    int ret = BK_FAIL;
    frame_buffer_t *jpeg_frame = NULL;

    do{
        jpeg_frame = _read_file_from_sdcard(file_name);
        if (jpeg_frame == NULL)
        {
            ret = BK_FAIL;
            break;
        }
        
        ret = _dec_img(jpeg_frame, img_dst);
        if(BK_OK == ret)
        {
            bk_printf("jpg decode success, width:%d, height:%d, size:%d\r\n", 
                            img_dst->header.w, img_dst->header.h, img_dst->data_size);
        }
    }while(0);

    if (jpeg_frame)
    {
        if(jpeg_frame->frame)
        {
            psram_free(jpeg_frame->frame);
            jpeg_frame->frame = NULL;
        }
        
        os_free(jpeg_frame);
        jpeg_frame = NULL;
    }

    return ret;
}

/**
 * @brief load jpg file from sd card, and store data in img_dst
 * @param[in] filename: only filename
 * @param[in] img_dst: if the data of img_dst is NULL, will use memory of psram_malloc, so when you don't use, should free this ram
 * @retval  BK_OK:success
 * @retval  <0: decode fail or file don't exit in sd
 */
s32 img_jpg_load_from_sd(char *filename, lv_img_dsc_t *img_dst)
{
    int ret = BK_FAIL;
    
    do{
        if(!filename || !img_dst)
        {
            ret = BK_ERR_NULL_PARAM;
            break;
        }

        jd_set_JD_FORMAT(JD_FORMAT_RGB565);
        ret = _dec_img_file_from_sdcard(filename, img_dst);
        jd_set_JD_FORMAT(JD_FORMAT_VYUY);
    }while(0);

    return ret;
}

#endif

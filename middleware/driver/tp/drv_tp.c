#include <driver/int.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>
#include <gpio_map.h>
#include "gpio_driver.h"
#include <driver/i2c.h>
#include <driver/media_types.h>
#include <driver/tp.h>
#include <driver/tp_types.h>
#include "tp_sensor_devices.h"
#include "bk_misc.h"
#include <driver/media_types.h>
#include "driver/drv_tp.h"


// #if (CONFIG_LCD && CONFIG_TP)

#define TAG "drv_tp"
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


static tp_dev_t g_tp_dev = {0};

/**
 * @brief tp open
 * @param[in] hor_size lcd horizontal size
 * @param[in] ver_size lcd vertical size
 * @retval kNoErr success
 * @retval kGeneralErr fail
 */
int drv_tp_open(int hor_size, int ver_size)
{
    LOGI("%s, start.\r\n", __func__);

    int ret = kNoErr;

    do{
        g_tp_dev.m_point_queue = bk_queue_create();
        ret = rtos_init_mutex(&g_tp_dev.m_mutex);
        if( kNoErr != ret )
        {
            LOGE("%s, init mutex fail!\r\n", __func__);
            ret = kGeneralErr;
            break;
        }

        tp_config_t tp_config = {0};
        uint32_t ppi = hor_size;
        ppi <<= 16;
        ppi |= ver_size;
        tp_config.ppi = ppi;
        tp_config.int_type = TP_INT_TYPE_FALLING_EDGE;
        tp_config.refresh_rate = 20;  // unit: ms
        tp_config.tp_num = TP_SUPPORT_MAX_NUM;

        bk_tp_driver_init(&tp_config);
    }while(0);

    return ret;
}

/**
 * @brief tp close
 * @param[in] none
 * @retval kNoErr success
 * @retval kGeneralErr fail
 */
int drv_tp_close(void)
{
    LOGI("%s, start.\r\n", __func__);

    int ret = kNoErr;

    do{
        if (g_tp_dev.m_point_queue)
        {
            bk_queue_destroy(g_tp_dev.m_point_queue);
            g_tp_dev.m_point_queue = NULL;
        }

        if (g_tp_dev.m_mutex)
        {
            ret = rtos_deinit_mutex(&g_tp_dev.m_mutex);
            if (kNoErr != ret)
            {
                LOGE("%s, deinit mutex fail!\r\n", __func__);
            }
            g_tp_dev.m_mutex = NULL;
        }

        bk_tp_driver_deinit();
    }while(0);

    return(ret);
}

/**
 * @brief read tp data from point buffer
 * @param[in] point: buff use to fill tp data
 * @retval kNoErr success
 * @retval kGeneralErr fail
 */
int drv_tp_read(tp_point_infor_t *point)
{
    int ret = kGeneralErr;
    tp_point_infor_t *tp_pstr = NULL;

    do{
        if(!point)
        {
            ret = kGeneralErr;
            break;
        }

        rtos_lock_mutex(&g_tp_dev.m_mutex);
        if(bk_queue_is_empty(g_tp_dev.m_point_queue))
        {
            rtos_unlock_mutex(&g_tp_dev.m_mutex);
            break;
        }

        bk_queue_delete(g_tp_dev.m_point_queue, (void **)&tp_pstr);
        if(NULL != tp_pstr)
        {
            rtos_unlock_mutex(&g_tp_dev.m_mutex);
            os_memcpy(point, tp_pstr, sizeof(tp_point_infor_t));

            if(!bk_queue_is_empty(g_tp_dev.m_point_queue))
            {
                point->m_need_continue = 1;
            }
            else
            {
                point->m_need_continue = 0;
            }
            os_free(tp_pstr);
            tp_pstr = NULL;
            ret = kNoErr;
        }
    }while(0);

    return ret;
}

/**
 * @brief write tp data to queue
 * @param[in] x: x coordinate value
 * @param[in] y: y coordinate value
 * @param[in] state: 1-press 0-release
 * @retval kNoErr success
 * @retval kGeneralErr fail
 */
int drv_tp_write(uint16_t x, uint16_t y, uint16_t state)
{
    int ret = kGeneralErr;
    tp_point_infor_t *point = NULL;

    do{
        point = (tp_point_infor_t *)os_malloc(sizeof(tp_point_infor_t));
        if(!point)
        {
            LOGE("%s, malloc failed!\r\n", __func__);
            break;
        }

        os_memset(point, 0, sizeof(tp_point_infor_t));
        point->m_x = x;
        point->m_y = y;
        point->m_state = state;

        rtos_lock_mutex(&g_tp_dev.m_mutex);
        if(bk_queue_get_size(g_tp_dev.m_point_queue) < TP_DATA_QUEUE_MAX_SIZE)
        {
            bk_queue_entry(g_tp_dev.m_point_queue, point);
        }
        else
        {
            LOGW("%s, tp data queue is full!\r\n", __func__);
        }
        rtos_unlock_mutex(&g_tp_dev.m_mutex);

        ret = kNoErr;
    }while(0);

    return ret;
}

void bk_tp_read_info_callback(tp_data_t *tp_data)
{
	static uint32_t hisTimeStamp = 0;

	// write tp data to queue
	if (tp_data->event == TP_EVENT_TYPE_DOWN)
	{
		hisTimeStamp = tp_data->timestamp;
		drv_tp_write(tp_data->x_coordinate, tp_data->y_coordinate, 1);
	}
	else if (tp_data->event == TP_EVENT_TYPE_MOVE)
	{
		if ( (tp_data->timestamp - hisTimeStamp) >= 5 )
		{
			hisTimeStamp = tp_data->timestamp;
			drv_tp_write(tp_data->x_coordinate, tp_data->y_coordinate, 1);
		}
	}
	else if (tp_data->event == TP_EVENT_TYPE_UP)
	{
		drv_tp_write(tp_data->x_coordinate, tp_data->y_coordinate, 0);
	}
}

// #endif


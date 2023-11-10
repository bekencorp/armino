#include <stdio.h>
#include <string.h>
#include <common/bk_include.h>
#include <os/os.h>
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_vendor.h"

static beken_thread_t  g_disp_thread_handle;
static u32 g_init_stack_size = (1024*4);
static beken_mutex_t g_disp_mutex;
beken_semaphore_t lvgl_sem;
lv_vnd_config_t vendor_config = {0};

#define TAG "lvgl"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

typedef enum{
	STATE_INIT,
	STATE_RUNNING,
	STATE_STOP
} lvgl_task_state_t;


unsigned char lvgl_task_state = STATE_INIT;


void lv_vendor_disp_lock(void)
{
    rtos_lock_mutex(&g_disp_mutex);
}

void lv_vendor_disp_unlock(void)
{
    rtos_unlock_mutex(&g_disp_mutex);
}

void lv_vendor_init(lv_vnd_config_t *config, uint16_t width, uint16_t height)
{
    bk_err_t ret;

	if (lvgl_task_state != STATE_INIT)
	{
		LOGE("%s already init\n", __func__);
		return;
	}

	memcpy(&vendor_config, config, sizeof(lv_vnd_config_t));
		
	lv_init();

	ret = lv_port_disp_init(width, height);

	if(ret != BK_OK)
	{
		LOGE("%s lv_port_disp_init failed\n", __func__);
		return;
	}
	
	lv_port_indev_init();

	rtos_init_mutex(&g_disp_mutex);

	ret = rtos_init_semaphore_ex(&lvgl_sem, 1, 0);

	if (BK_OK != ret)
	{
		LOGE("%s semaphore init failed\n", __func__);
		return;
	}
}

static void lv_tast_entry( void *arg )
{
	lvgl_task_state = STATE_RUNNING;

	rtos_set_semaphore(&lvgl_sem);

    while(lvgl_task_state == STATE_RUNNING)
    {
		lv_vendor_disp_lock();
        lv_task_handler();
        lv_vendor_disp_unlock();
        rtos_delay_milliseconds(5);
    }

	rtos_set_semaphore(&lvgl_sem);

    rtos_delete_thread(NULL);
}


void lv_vendor_start(void)
{
    bk_err_t ret;

    ret = rtos_create_thread(&g_disp_thread_handle,
                             4,
                             "lcd_thr",
                             (beken_thread_function_t)lv_tast_entry,
                             (unsigned short)g_init_stack_size,
                             (beken_thread_arg_t)0);
    if(BK_OK != ret)
	{
        return;
	}

	ret = rtos_get_semaphore(&lvgl_sem, BEKEN_NEVER_TIMEOUT);
	
	if (BK_OK != ret)
	{
		LOGE("%s transfer_sem get failed\n", __func__);
	}


}

void lv_vendor_stop(void)
{
	bk_err_t ret;

	rtos_delay_milliseconds(150);
	lvgl_task_state = STATE_STOP;

	ret = rtos_get_semaphore(&lvgl_sem, BEKEN_NEVER_TIMEOUT);
	
	if (BK_OK != ret)
	{
		LOGE("%s transfer_sem get failed\n", __func__);
	}

}


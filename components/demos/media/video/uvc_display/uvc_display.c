#include <modules/pm.h>
#include <driver/uvc_camera.h>


extern bk_err_t bk_uvc_camera_open(media_ppi_t ppi);
extern int lcd_act_driver_init(uint32_t lcd_ppi);

void uvc_display_test()
{
    rtos_delay_milliseconds(1000);
#if (CONFIG_MEDIA)
    //frame_buffer_enable(true);
#endif
    bk_uvc_camera_open(PPI_640X480);

    rtos_delay_milliseconds(1000);
    bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_320M);
    lcd_act_driver_init(PPI_480X272);
}


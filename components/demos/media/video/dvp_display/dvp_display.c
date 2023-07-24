#include "modules/pm.h"
#include <driver/int.h>
#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>

extern bk_err_t bk_dvp_camera_open(media_ppi_t ppi, media_camera_type_t type);
extern int lcd_act_driver_init(uint32_t lcd_ppi);

void dvp_display_test()
{
    rtos_delay_milliseconds(1000);
#if (CONFIG_MEDIA)
    //frame_buffer_enable(true);
#endif
    bk_dvp_camera_open(PPI_640X480, MEDIA_DVP_MJPEG);

    rtos_delay_milliseconds(1000);
    bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_320M);
    lcd_act_driver_init(PPI_480X272);
}



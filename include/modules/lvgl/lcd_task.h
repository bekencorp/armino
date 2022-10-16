#ifndef __LCD_TASK_H
#define __LCD_TASK_H

#define LV_NOTIFY_TIMER_DURATION       10

typedef enum{
    GUI_INIT,
    GUI_RUNNING,
    GUI_STOP
}GUI_RUN_STATE_E;
    
void bk_gui_disp_task_init(int hor_size, int ver_size);
void bk_gui_disp_task_deinit(void);
void bk_gui_disp_lock(void);
void bk_gui_disp_unlock(void);

#endif
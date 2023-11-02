#ifndef __PAGE_LOAD_CTROL_H__
#define __PAGE_LOAD_CTROL_H__

#define TAG "ui"
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


void ui_event_UiPage6_screen(lv_event_t * e);
void hor_page_load_main(void);
void ui_tabview_set(uint32_t id);

#endif

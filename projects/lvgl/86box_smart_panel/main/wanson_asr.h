#ifndef __WANSON_ASR_H__
#define __WANSON_ASR_H__

#include <components/aud_intf_types.h>


bk_err_t wanson_asr_open(aud_intf_mic_type_t mic_type);
bk_err_t wanson_asr_close(void);

#endif
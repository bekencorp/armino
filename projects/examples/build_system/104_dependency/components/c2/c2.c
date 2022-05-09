#include <stdio.h>
#include <components/log.h>

#include "bk_api_c1.h"

#define TAG "exam"

int c2_public_api(void)
{
	c1_public_api();
	BK_LOGI(TAG, "Call %s\n", __FUNCTION__);
	return 0;
}

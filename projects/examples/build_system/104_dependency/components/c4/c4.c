#include <stdio.h>
#include <components/log.h>

#include "bk_api_c1.h"
#include "bk_api_c2.h"
#include "bk_api_c3.h"

#define TAG "exam"

int c4_public_api(void)
{
	c1_public_api();
	c2_public_api();
	c3_public_api();
	BK_LOGI(TAG, "Call %s\n", __FUNCTION__);
	return 0;
}

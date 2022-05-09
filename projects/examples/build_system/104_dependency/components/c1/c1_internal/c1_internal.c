#include <stdio.h>
#include <components/log.h>

#define TAG "exam"

int c1_internal_only(void)
{
	BK_LOGI(TAG, "call %s\n", __FUNCTION__);
	return 0;
}

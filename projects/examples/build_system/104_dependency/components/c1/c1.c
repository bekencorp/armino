#include <stdio.h>
#include <components/log.h>

//headers specific to component c1
#include "c1.h"
#include "c1_internal.h"

#define TAG "exam"

int c1_public_api(void)
{
	c1_internal_only();
	BK_LOGI(TAG, "Call %s\n", __FUNCTION__);
	return 0;
}

int c1_local(void)
{
	BK_LOGI(TAG, "Call %s\n", __FUNCTION__);
	return 0;
}

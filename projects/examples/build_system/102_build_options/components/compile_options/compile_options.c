#include <stdio.h>
#include <components/log.h>

#define TAG "compile_options"

static int unused_function(void)
{
	BK_LOGI(TAG, "unsed functions\n");
	return BK_OK;
}

int compile_options(void)
{
	int unused_variable = 0;

	BK_LOGI(TAG, "hello cmake compile options\n");
	return 0;
}



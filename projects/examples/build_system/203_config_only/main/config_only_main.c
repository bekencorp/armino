#include <components/log.h>

#define TAG "exam"

int main(void)
{
#if CONFIG_CONFIG_ONLY
	BK_LOGI(TAG, "Hello config-only enabled\n");
#else
	BK_LOGI(TAG, "Hello config-only disabled\n");
#endif
	return 0;
}

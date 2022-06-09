#include "bk_private/legacy_init.h"
#include <stdio.h>
#include <os/os.h>

void __attribute__((weak)) OHOS_SystemInit(void)
{
	os_printf("@NOOS@");
	return;
}

void __attribute__((weak)) OHOS_TEST(void)
{
}

int main(void)
{
	legacy_init();
	OHOS_SystemInit();
	OHOS_TEST();

	return 0;
}

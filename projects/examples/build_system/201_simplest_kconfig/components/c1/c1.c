#include <stdio.h>
#include "sdkconfig.h"

int c1(void)
{
#if CONFIG_C1
	printf("component c1 enabled\n");
#else
	printf("component c1 disabled\n");
#endif
	return 0;
}

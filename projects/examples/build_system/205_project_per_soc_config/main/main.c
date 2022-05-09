#include <stdio.h>
#include "sdkconfig.h"

int main(void)
{
	printf("CONFIG_FOO=%d\n", CONFIG_FOO);
	return 0;
}

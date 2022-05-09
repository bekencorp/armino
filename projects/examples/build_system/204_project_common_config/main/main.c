#include <stdio.h>
#include "sdkconfig.h"

int main(void)
{
	printf("hello armino cmake\n");
	printf("CONFIG_FOO=%d\n", CONFIG_FOO);
	return 0;
}

#include "_at_server_port.h"
#include "_at_server.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"




void __weak__ *at_malloc(unsigned int size)
{
	return NULL;
}

void __weak__ at_free(void *p)
{

}





#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>

void *cs2_malloc(size_t size)
{
	return os_malloc(size);
}

void cs2_free(void *ptr)
{
	os_free(ptr);
}

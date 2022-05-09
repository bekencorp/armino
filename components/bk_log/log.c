#include <components/log.h>

void bk_mem_dump(const char *title, uint32_t start, uint32_t len)
{
	uint32_t val = 0;
	uint32_t mem;

	BK_LOG_RAW("%s - dump(len=%u)\n", title, len);

	for (uint32_t i = 0; i < len; i += 4) {
		mem = start + i;

		if ((i % 32) == 0) {
			if (i)
				BK_LOG_RAW("\n");

			BK_LOG_RAW("%x:", mem);
		}

		val = *((volatile uint32_t *)(mem));
		BK_LOG_RAW(" %08x", val);
	}
	BK_LOG_RAW("\n");
}

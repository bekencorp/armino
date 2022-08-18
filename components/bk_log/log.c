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


void bk_mem_dump_ex(const char *title, unsigned char *data, uint32_t data_len)
{
	uint32_t cnt = 0;

	BK_DUMP_OUT("====== %s - dump(len=%u) begin ======\r\n", title, data_len);

	for (int i = 0;  i < data_len; i++) {
		if ((cnt++ & 0xf) == 0) {
			BK_DUMP_OUT("\r\n");
		}
		BK_DUMP_OUT("%02x ", data[i]);
	}

	BK_DUMP_OUT("\r\n");
	BK_DUMP_OUT("====== %s - dump(len=%u)   end ======\r\n", title, data_len);
	BK_DUMP_OUT("\r\n");
}
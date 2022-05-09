#include "bk_api_ate.h"
#include "bk_api_cli.h"
#include <components/log.h>

#define CMD_SINGLE_WAVE  "txevm -b 0 -r 24 -c 1 -w 1"

#ifdef SINGLE_WAVE_TEST
static void do_single_wave_test(void)
{
	uint32_t cmd_len = os_strlen(CMD_SINGLE_WAVE) + 1;
	uint8 *cmd_buf = os_malloc(cmd_len);
	if (cmd_buf) {
		extern void bk_test_cmd_handle_input(char *inbuf, int len);

		os_memcpy(cmd_buf, CMD_SINGLE_WAVE, cmd_len);
		bk_test_cmd_handle_input((char *)cmd_buf, cmd_len);
		os_free(cmd_buf);
	}
}
#endif

int main(void)
{
	bk_ate_init();
	bk_cli_init();

	//TODO fix it
	//rf_ps_enable_clear();

	//TODO fix it
	//do_single_wave_test();
	return BK_OK;
}

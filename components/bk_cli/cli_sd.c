#include "cli.h"

#if CONFIG_SDCARD_HOST
/*
sdtest I 0 --
sdtest R secnum
sdtest W secnum
*/
extern uint32_t sdcard_intf_test(void);
extern uint32_t test_sdcard_read(uint32_t blk, uint32_t blk_cnt);
extern uint32_t test_sdcard_write(uint32_t blk, uint32_t blk_cnt);
extern void sdcard_intf_close(void);

static void sd_operate(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t cmd;
	uint32_t blknum, blkcnt;
	uint32_t ret;
	if (argc == 3) {
		cmd = argv[1][0];
		blknum = os_strtoul(argv[2], NULL, 16);
		switch (cmd) {
		case 'I':
			ret = sdcard_intf_test();
			os_printf("init :%x\r\n", ret);
			break;
		case 'R':
			blkcnt = blknum % 3;
			ret = test_sdcard_read(blknum, blkcnt);
			os_printf("read :%x\r\n", ret);
			break;
		case 'W':
			blkcnt = blknum % 3;
			ret = test_sdcard_write(blknum, blkcnt);
			os_printf("write :%x\r\n", ret);
			break;
		case 'C':
			sdcard_intf_close();
			os_printf("close \r\n");
			break;
		default:
			break;
		}
	} else
		os_printf("cmd param error\r\n");
}

#endif


#define SD_CMD_CNT (sizeof(s_sd_commands) / sizeof(struct cli_command))
static const struct cli_command s_sd_commands[] = {
#if CONFIG_SDCARD_HOST
	{"sdtest", "sdtest <cmd>", sd_operate},
#endif
};

int cli_sd_init(void)
{
	return cli_register_commands(s_sd_commands, SD_CMD_CNT);
}

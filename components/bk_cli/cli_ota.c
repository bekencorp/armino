#include "cli.h"
#include "modules/ota.h"

#if CONFIG_OTA_TFTP
extern void tftp_start(void);
extern void string_to_ip(char *s);
static void tftp_ota_get_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	short len = 0, i;
	extern char     BootFile[] ;

	if (argc > 3) {
		os_printf("ota server_ip ota_file\r\n");
		return;
	}

	os_printf("%s\r\n", argv[1]);

	os_strcpy(BootFile, argv[2]);
	os_printf("%s\r\n", BootFile);
	string_to_ip(argv[1]);


	tftp_start();

	return;

}
#endif


#if CONFIG_OTA_HTTP
void http_ota_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret;
	if (argc != 2)
		goto HTTP_CMD_ERR;
	ret = bk_http_ota_download(argv[1]);

	if (0 != ret)
		os_printf("http_ota download failed.");

	return;

HTTP_CMD_ERR:
	os_printf("Usage:http_ota [url:]\r\n");
}
#endif


#define OTA_CMD_CNT (sizeof(s_ota_commands) / sizeof(struct cli_command))
static const struct cli_command s_ota_commands[] = {

#if CONFIG_OTA_TFTP
	{"tftpota", "tftpota [ip] [file]", tftp_ota_get_Command},
#endif

#if CONFIG_OTA_HTTP
	{"http_ota", "http_ota url", http_ota_Command},
#endif
};

int cli_ota_init(void)
{
	return cli_register_commands(s_ota_commands, OTA_CMD_CNT);
}

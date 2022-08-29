#include "cli.h"
#include "bk_private/bk_wifi.h"
#include <components/system.h>
#if CONFIG_OTA_HTTP
#include "utils_httpc.h"
#endif

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

#ifdef CONFIG_HTTP_AB_PARTITION
    part_flag update_part_flag;
#endif
#if CONFIG_OTA_HTTP
int http_ota_download(const char *uri)
{
	int ret;
	httpclient_t httpclient;
	httpclient_data_t httpclient_data;
	char http_content[HTTP_RESP_CONTENT_LEN];
    
    CLI_LOGI("http_ota_download :0x%x",http_ota_download);
    
    //__asm volatile ("j .");
#ifdef CONFIG_HTTP_AB_PARTITION
    bk_logic_partition_t *bk_ptr =NULL;
    int exec_part_sta_addr = 0;
    int exec_part_len = 0;
    int this_func_adr = 0;

    this_func_adr = (int)http_ota_download;
    CLI_LOGI("this_func_adr :0x%x",http_ota_download);

    bk_ptr = bk_flash_partition_get_info(BK_PARTITION_APPLICATION);
    CLI_LOGI("bk_ptr :0x%x",bk_ptr);
    //__asm volatile ("j .");
    exec_part_sta_addr = (bk_ptr->partition_start_addr)*INS_NO_CRC_CHUNK/INS_CRC_CHUNK;
    exec_part_len = (bk_ptr->partition_length)*INS_NO_CRC_CHUNK/INS_CRC_CHUNK;
    CLI_LOGI("exec_part_sta_addr :0x%x,exec_part_len :0x%x",exec_part_sta_addr,exec_part_len);
    if((this_func_adr > exec_part_sta_addr)&&(this_func_adr < (exec_part_len + exec_part_len)))
    {
        CLI_LOGI("12345");
        update_part_flag = UPDATE_B_PART;
    }
    else
        update_part_flag = UPDATE_A_PART;
#endif

	os_memset(&httpclient, 0, sizeof(httpclient_t));
	os_memset(&httpclient_data, 0, sizeof(httpclient_data));
	os_memset(&http_content, 0, sizeof(HTTP_RESP_CONTENT_LEN));
	httpclient.header = "Accept: text/xml,text/html,\r\n";
	httpclient_data.response_buf = http_content;
	httpclient_data.response_content_len = HTTP_RESP_CONTENT_LEN;
	ret = httpclient_common(&httpclient,
							uri,
							80,/*port*/
							NULL,
							HTTPCLIENT_GET,
							180000,
							&httpclient_data);

	if (0 != ret)
		CLI_LOGI("request epoch time from remote server failed.");
	else {
		CLI_LOGI("sucess.\r\n");
		bk_reboot();
	}

	return ret;
}

void http_ota_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret;
	if (argc != 2)
		goto HTTP_CMD_ERR;
	ret = http_ota_download(argv[1]);

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

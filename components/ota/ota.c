#include "sdkconfig.h"
#include <string.h>
#include "cli.h"
#include <components/system.h>
#include "driver/flash.h"
#include "modules/ota.h"
#if CONFIG_OTA_HTTP
#include "utils_httpc.h"
#include "modules/wifi.h"
#endif
#if (CONFIG_TFM)
#include "psa/update.h"
#endif

#if (CONFIG_TFM)
psa_image_id_t ota_image = \
                (psa_image_id_t)FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_STAGE,
                                                       FWU_IMAGE_TYPE_FULL,
                                                       0);
#endif

#ifdef CONFIG_HTTP_AB_PARTITION
part_flag update_part_flag;

uint8 bk_ota_get_current_partition(void)
{
    exec_flag ota_exec_flag = 5;
    bk_logic_partition_t *bk_ptr = NULL;

    bk_ptr = bk_flash_partition_get_info(BK_PARTITION_OTA_FINA_EXECUTIVE);
    os_printf("bk_ptr->partition_start_addr  :0x%x\r\n",bk_ptr->partition_start_addr);
    bk_flash_read_bytes(bk_ptr->partition_start_addr ,(uint8_t *)&ota_exec_flag, sizeof(u8));

    return ota_exec_flag ;  // ota_exec_flag :0x0/0xFF represents A 0x1 :represents B 

}

void ota_write_flash(bk_partition_t ota_partition_flag, u8 flag, u8 offset)
{
	bk_logic_partition_t *bk_ptr = NULL;
	u8 ota_final_buff[1],ota_temp_buff[1],ota_cconfirm_buff[1];

    u8 temp1_buff[1],temp2_buff[1],temp3_buff[1];
    
	bk_ptr = bk_flash_partition_get_info(ota_partition_flag);
	
	os_printf("ota_write_flash:partition_start_addr:0x%x\r\n",(bk_ptr->partition_start_addr));
	bk_flash_read_bytes((bk_ptr->partition_start_addr),(uint8_t *)ota_final_buff, sizeof(u8));
	bk_flash_read_bytes((bk_ptr->partition_start_addr + 4),(uint8_t *)ota_temp_buff, sizeof(u8));
	bk_flash_read_bytes((bk_ptr->partition_start_addr + 8),(uint8_t *)ota_cconfirm_buff, sizeof(u8));
	
	os_printf("before:ota_final_buff:0x%x,ota_temp_buff:0x%x,ota_cconfirm_buff:0x%x\r\n",
			ota_final_buff[0],ota_temp_buff[0],ota_cconfirm_buff[0]);
	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
	bk_flash_erase_sector(bk_ptr->partition_start_addr);
    
	if(offset == 0)
	{
		os_printf("offset == 0\r\n");
		bk_flash_write_bytes((bk_ptr->partition_start_addr + offset),(uint8_t *)&flag, sizeof(u8));
		bk_flash_write_bytes((bk_ptr->partition_start_addr + 4),(uint8_t *)ota_temp_buff, sizeof(u8));
		bk_flash_write_bytes((bk_ptr->partition_start_addr + 8),(uint8_t *)ota_cconfirm_buff, sizeof(u8));
		
	}
	else if(offset == 4)
	{
		os_printf("offset == 4\r\n");
		bk_flash_write_bytes((bk_ptr->partition_start_addr),(uint8_t *)ota_final_buff, sizeof(u8));
		bk_flash_write_bytes((bk_ptr->partition_start_addr + offset),(uint8_t *)&flag, sizeof(u8));
		bk_flash_write_bytes((bk_ptr->partition_start_addr + 8),(uint8_t *)ota_cconfirm_buff, sizeof(u8));	
	}
	else if(offset == 8)
	{
		os_printf("offset == 8\r\n");
		bk_flash_write_bytes((bk_ptr->partition_start_addr),(uint8_t *)ota_final_buff, sizeof(u8));
		bk_flash_write_bytes((bk_ptr->partition_start_addr + 4),(uint8_t *)ota_temp_buff,sizeof(u8));
		bk_flash_write_bytes((bk_ptr->partition_start_addr + offset),(uint8_t *)&flag, sizeof(u8));	
	}
	#if 1
    bk_flash_read_bytes((bk_ptr->partition_start_addr + 0),(uint8_t *)temp1_buff, sizeof(u8));
	bk_flash_read_bytes((bk_ptr->partition_start_addr + 4),(uint8_t *)temp2_buff, sizeof(u8));
	bk_flash_read_bytes((bk_ptr->partition_start_addr + 8),(uint8_t *)temp3_buff, sizeof(u8));
	os_printf("ota_final_buff:0x%x,ota_temp_buff:0x%x,ota_cconfirm_buff:0x%x\r\n",
			temp1_buff[0],temp2_buff[0],temp3_buff[0]);
    #endif
    bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
}

void bk_ota_confirm_update_partition(ota_confirm_flag ota_confirm_val)
{
    exec_flag ota_exec_flag ;   
    
    ota_write_flash(BK_PARTITION_OTA_FINA_EXECUTIVE, ota_confirm_val,8); 
    
    if(ota_confirm_val == CONFIRM_EXEC_A)   
    {   
        ota_exec_flag = EXEX_A_PART;    
        ota_write_flash(BK_PARTITION_OTA_FINA_EXECUTIVE, ota_exec_flag, 0);
    }
    else if(ota_confirm_val == CONFIRM_EXEC_B)
    { 
        ota_exec_flag = EXEC_B_PART;
        ota_write_flash(BK_PARTITION_OTA_FINA_EXECUTIVE, ota_exec_flag, 0);
    }
}

#if 0
uint8 custmer_state_cb(uint8 temp_exec_part)
{
    uint8 t_exec_part;

    if(temp_exec_part == 0)
    {
        t_exec_part = 4;
    }
    os_printf("custmer_state_cb :%d\r\n ",t_exec_part);
    return t_exec_part;
}
#endif

static callback_func update_state_cb = NULL;
void bk_ota_register_temp_partition_callback(callback_func cb)
{
    update_state_cb = cb;
}

uint8 ota_temp_execute_partition(int state_val)
{
    uint8 temp_exec_flag = 6;

    if(update_state_cb)
    {
         temp_exec_flag = update_state_cb(state_val);
    }
    
    return temp_exec_flag;
}

#endif
u8  ota_flag =0;
#if CONFIG_OTA_HTTP
int bk_http_ota_download(const char *uri)
{
	int ret;
	httpclient_t httpclient;
	httpclient_data_t httpclient_data;
	char http_content[HTTP_RESP_CONTENT_LEN];
#if (CONFIG_TFM)
	psa_status_t status;
	psa_image_id_t dependency_uuid;
	psa_image_version_t dependency_version;
	psa_image_info_t info;
#endif
	if(!uri)
	{
		ret = BK_FAIL;
		CLI_LOGI( "uri is NULL\r\n");
        
		return ret;
	}
    CLI_LOGI("http_ota_download :0x%x",bk_http_ota_download);

    //__asm volatile ("j .");
#ifdef CONFIG_HTTP_AB_PARTITION
    bk_logic_partition_t *bk_ptr =NULL;
    bk_logic_partition_t *bk_ptr1 =NULL;
    int exec_part_sta_addr, exec_part1_sta_addr= 0;
    int exec_part_len, exec_part1_len = 0; 
    int this_func_adr  = 0;
    ota_temp_exec_flag temp_exec_flag = 6;
    exec_flag exec_temp_part = 6;

    //bk_ota_register_temp_partition_callback(custmer_state_cb);
    
    this_func_adr = (int)bk_http_ota_download;
    CLI_LOGI("this_func_adr :0x%x",bk_http_ota_download);

    bk_ptr = bk_flash_partition_get_info(BK_PARTITION_APPLICATION);
    CLI_LOGI("bk_ptr :0x%x",bk_ptr);
    exec_part_sta_addr = (bk_ptr->partition_start_addr)*INS_NO_CRC_CHUNK/INS_CRC_CHUNK;
    exec_part_len = (bk_ptr->partition_length)*INS_NO_CRC_CHUNK/INS_CRC_CHUNK;
    CLI_LOGI("exec_part_sta_addr :0x%x,exec_part_len :0x%x",exec_part_sta_addr,exec_part_len);

    bk_ptr1 = bk_flash_partition_get_info(BK_PARTITION_APPLICATION1);
    exec_part1_sta_addr = (bk_ptr1->partition_start_addr)*INS_NO_CRC_CHUNK/INS_CRC_CHUNK;
    exec_part1_len = (bk_ptr1->partition_length)*INS_NO_CRC_CHUNK/INS_CRC_CHUNK;
    CLI_LOGI("exec_part_sta_addr :0x%x,exec_part_len :0x%x",exec_part1_sta_addr,exec_part1_len);

    if(((this_func_adr > exec_part_sta_addr)&&(this_func_adr < (exec_part_sta_addr + exec_part_len))))
    {
        CLI_LOGI("123");
        update_part_flag = UPDATE_B_PART;
    }
    else if(((this_func_adr > exec_part1_sta_addr)&&(this_func_adr < (exec_part1_sta_addr + exec_part1_len))))
    {
        CLI_LOGI("456");
        update_part_flag = UPDATE_A_PART;
    }
    else
        return -1;
#endif

#if CONFIG_SYSTEM_CTRL
	bk_wifi_ota_dtim(1);
#endif

	ota_flag = 1;
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


        ota_flag = 0;
	if (0 != ret)
        {
            CLI_LOGI("request epoch time from remote server failed.ret:%d\r\n",ret);
#if CONFIG_SYSTEM_CTRL
	    bk_wifi_ota_dtim(0);
#endif
        }
	else
        {
	    CLI_LOGI("sucess.\r\n");
#ifdef CONFIG_HTTP_AB_PARTITION
        #ifndef CONFIG_OTA_UPDATE_DEFAULT_PARTITION
            temp_exec_flag = ota_temp_execute_partition(ret); //temp_exec_flag :3 :A ,4:B
        #else
            #ifdef CONFIG_OTA_UPDATE_B_PARTITION
                temp_exec_flag = CONFIRM_EXEC_B; //update B Partition;
            #else
                temp_exec_flag = CONFIRM_EXEC_A; //update A Partition;
            #endif
        #endif
        os_printf("from cus temp_exec_flag:0x%x \r\n",temp_exec_flag);

        if(temp_exec_flag == CONFIRM_EXEC_A)
        {
            os_printf("B>>>A \r\n");
            exec_temp_part = EXEX_A_PART;
        }
        else if(temp_exec_flag == CONFIRM_EXEC_B)
        {
            os_printf("A>>B \r\n");
            exec_temp_part = EXEC_B_PART;
        }
        os_printf("temp_exec_flag:0x%x \r\n",exec_temp_part);
        ota_write_flash(BK_PARTITION_OTA_FINA_EXECUTIVE, exec_temp_part, 4); //
		bk_reboot();
#else
#if (CONFIG_TFM)
		status = psa_fwu_query(ota_image, &info);
		if (status != PSA_SUCCESS) {
			bk_printf("query status %d\r\n", status);
			return -1;
		}
		if (info.state != PSA_IMAGE_CANDIDATE) {
			bk_printf("info state %d\r\n", info.state);
			return -1;
		}

		status = psa_fwu_install(ota_image, &dependency_uuid, &dependency_version);
		if (status != PSA_SUCCESS_REBOOT) {
			bk_printf("install fail %d\r\n", status);
			return -1;
		} else {
			bk_printf("install success\r\n");
		}

		status = psa_fwu_query(ota_image, &info);
		if (status != PSA_SUCCESS) {
			bk_printf("query fail %d\r\n", status);
			return -1;
		}
		if (info.state != PSA_IMAGE_REBOOT_NEEDED) {
			bk_printf("info fail %d\r\n", info.state);
			return -1;
		}

		bk_printf("reboot\r\n");
		psa_fwu_request_reboot();
#else
        bk_reboot();
#endif
#endif

	}

	return ret;
}

#endif

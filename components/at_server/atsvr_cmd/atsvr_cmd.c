#include "atsvr_cmd.h"
#include "at_server.h"
#include "atsvr_wlan.h"
#include "atsvr_misc.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"


static void atsvr_reset_system(int argc, char **argv)
{
    if(argc != 1){
		atsvr_cmd_rsp_error();
        return;
    }
	atsvr_cmd_rsp_ok();
	reboot_the_system();
    return;
}


static void atsvr_get_station_mac(int argc, char **argv)
{
    if(argc == 1){
		unsigned char mac[6];
	    int n = 0;
	    char resultbuf[200];

		wlan_get_station_mac_address((char*)mac);

		n = snprintf(resultbuf,sizeof(resultbuf),ATSVR_CMDRSP_HEAD"+WMAC:%02x:%02x:%02x:%02x:%02x:%02x\r\n",mac[0],
				mac[1],mac[2],mac[3],mac[4],mac[5]);
		n += snprintf(resultbuf+n,sizeof(resultbuf) - n,"%s",ATSVR_CMD_RSP_SUCCEED);
		atsvr_output_msg(resultbuf,n);
	    return;
	}else{
		atsvr_cmd_rsp_error();
    }
}

void atsvr_set_station_dhcp(int argc, char **argv)
{
    char *mode;
    unsigned int param;

    if(argc != 2){
		atsvr_cmd_rsp_error();
        return;
    }

    mode = argv[1];
    if(strcmp(mode,"ON DHCP") == 0){
        param = 1;
    }else if(strcmp(mode,"OFF DHCP") == 0){
       param = 0;
    }else if(strcmp(mode,"1") == 0){
        param = 1;
    }else if(strcmp(mode,"0") == 0){
       param = 0;
    }else{
        atsvr_cmd_rsp_error();
        return;
    }

    wlan_set_station_dhcp(param);
    atsvr_cmd_rsp_ok();
    return;
}

static void atsvr_station_ssid_pwd(int argc, char **argv)
{
    if((argc != 2)&&(argc != 3)){
		atsvr_cmd_rsp_error();
        return;
    }
	char *my_ssid;
	char *connect_key;

	if(argc == 2){
		my_ssid = argv[1];
		connect_key = "OPEN";
	}else if(argc == 3){
		my_ssid = argv[1];
		connect_key = argv[2];
	}else{
		atsvr_cmd_rsp_error();
        return;
	}
	if(wlan_start_station_connect(my_ssid, connect_key) == 0){
		atsvr_cmd_rsp_ok();
	}else{
		atsvr_cmd_rsp_error();
	}

    return;
}

static void atsvr_station_stop(int argc, char **argv)
{
    if(argc != 1){
		atsvr_cmd_rsp_error();
        return;
    }

	if(wlan_stop_station() == 0){
		atsvr_cmd_rsp_ok();
	}else{
		atsvr_cmd_rsp_error();
	}

    return;
}

static void atsvr_station_static_ip(int argc, char **argv)
{
    if((argc != 4)&&(argc != 5)){
		atsvr_cmd_rsp_error();
        return;
    }
	char *ip = argv[1];
	char *mask = argv[2];
	char *gate = argv[3];
	char *dns = NULL;
	char *dns2 = NULL;

	if(argc == 5){
		dns = argv[4];
	}
	if(judge_the_string_is_ipv4_string(ip) != 0){
		atsvr_cmd_rsp_error();
		ATSVRLOG("[atsvr]ip:%s error\r\n",ip);
        return;
	}
	if(judge_the_string_is_ipv4_string(mask) != 0){
		atsvr_cmd_rsp_error();
		ATSVRLOG("[atsvr]mask:%s error\r\n",mask);
        return;
	}
	if(judge_the_string_is_ipv4_string(gate) != 0){
		atsvr_cmd_rsp_error();
		ATSVRLOG("[atsvr]gate:%s error\r\n",gate);
        return;
	}
	if(dns != NULL){
		if(judge_the_string_is_ipv4_string(dns) != 0){
			atsvr_cmd_rsp_error();
			ATSVRLOG("[atsvr]dns:%s error\r\n",dns);
	        return;
		}
	}
	if(dns2 != NULL){
		if(judge_the_string_is_ipv4_string(dns2) != 0){
			atsvr_cmd_rsp_error();
			ATSVRLOG("[atsvr]dns2:%s error\r\n",dns2);
			return;
		}
	}

	if(wlan_set_station_static_ip(ip, mask,gate,dns,dns2) == 0){
		atsvr_cmd_rsp_ok();
	}else{
		atsvr_cmd_rsp_error();
	}

    return;
}

void atsvr_get_station_status(int argc, char **argv)
{
    if(argc != 1){
		atsvr_cmd_rsp_error();
        return;
    }
	int statue = wlan_get_station_cur_status();
	if(statue == 1){
		ATSVR_SIZEOF_OUTPUT_STRRING(ATSVR_CMDRSP_HEAD"STATION_UP\r\n");
	}else{
		ATSVR_SIZEOF_OUTPUT_STRRING(ATSVR_CMDRSP_HEAD"STATION_DOWN\r\n");
	}
	set_atsvr_work_state(ATSVR_WK_DONE);
    atsvr_cmd_rsp_ok();
    return;
}

void atsvr_wlan_scan_cmd(int argc, char **argv)
{
    if((argc != 1) && (argc != 2)){
		atsvr_cmd_rsp_error();
        return;
    }
	char *ssid = NULL;

	if(argc == 2){
		ssid = argv[1];
	}
	if(wlan_scan_start(ssid) != 0){
		atsvr_cmd_rsp_error();
        return;
	}
	set_atsvr_work_state(ATSVR_WK_DONE);
    atsvr_cmd_rsp_ok();
    return;
}


static void atsvr_softap_start(int argc, char **argv)
{
    if((argc != 2)&&(argc != 3)){
		atsvr_cmd_rsp_error();
		return;
    }
	char *my_ssid;
	char *connect_key;

	if(argc == 2){
		my_ssid = argv[1];
		connect_key = "OPEN";
	}else if(argc == 3){
		my_ssid = argv[1];
		connect_key = argv[2];
	}else{
		atsvr_cmd_rsp_error();
        return;
	}
	if(wlan_softap_start(my_ssid, connect_key) == 0){
		atsvr_cmd_rsp_ok();
	}else{
		atsvr_cmd_rsp_error();
	}

    return;
}

static void atsvr_softap_stop(int argc, char **argv)
{
    if(argc != 1){
		atsvr_cmd_rsp_error();
		return;
    }

	if(wlan_stop_softap() == 0){
		set_atsvr_work_state(ATSVR_WK_DONE);
		atsvr_cmd_rsp_ok();
	}else{
		set_atsvr_work_state(ATSVR_WK_DONE);
		atsvr_cmd_rsp_error();
	}

    return;
}

static void atsvr_softap_static_ip(int argc, char **argv)
{
    if((argc != 4)&&(argc != 5)){
		atsvr_cmd_rsp_error();
        return;
    }
	char *ip = argv[1];
	char *mask = argv[2];
	char *gate = argv[3];
	char *dns = NULL;

	if(argc == 5){
		dns = argv[4];
	}
	if(judge_the_string_is_ipv4_string(ip) != 0){
		atsvr_cmd_rsp_error();
		ATSVRLOG("[atsvr]ip:%s error\r\n",ip);
        return;
	}
	if(judge_the_string_is_ipv4_string(mask) != 0){
		atsvr_cmd_rsp_error();
		ATSVRLOG("[atsvr]mask:%s error\r\n",mask);
        return;
	}
	if(judge_the_string_is_ipv4_string(gate) != 0){
		atsvr_cmd_rsp_error();
		ATSVRLOG("[atsvr]gate:%s error\r\n",gate);
        return;
	}
	if(dns != NULL){
		if(judge_the_string_is_ipv4_string(dns) != 0){
			atsvr_cmd_rsp_error();
			ATSVRLOG("[atsvr]dns:%s error\r\n",dns);
	        return;
		}
	}
	if(wlan_set_softap_static_ip(ip, mask,gate,dns) == 0){
		atsvr_cmd_rsp_ok();
	}else{
		atsvr_cmd_rsp_error();
	}

    return;
}

void atsvr_get_softap_status(int argc, char **argv)
{
    if(argc != 1){
		atsvr_cmd_rsp_error();
        return;
    }
	int statue = wlan_get_softap_cur_status();
	if(statue == 1){
		ATSVR_SIZEOF_OUTPUT_STRRING(ATSVR_CMDRSP_HEAD"SOFTAP_UP\r\n");
	}else{
		ATSVR_SIZEOF_OUTPUT_STRRING(ATSVR_CMDRSP_HEAD"SOFTAP_DOWN\r\n");
	}
	set_atsvr_work_state(ATSVR_WK_DONE);
    atsvr_cmd_rsp_ok();
    return;
}

const struct _atsvr_command atsvc_cmds_table[] = {
	ATSVR_CMD_HADLER("AT+RST","AT+RST",atsvr_reset_system),

	ATSVR_CMD_HADLER("AT+WLMAC","AT+WLMAC",atsvr_get_station_mac),
	ATSVR_CMD_HADLER("AT+STADHCP","AT+STADHCP=0/1",atsvr_set_station_dhcp),
	ATSVR_CMD_HADLER("AT+STAAPINFO","AT+STAAPINFO=SSID,PWD",atsvr_station_ssid_pwd),
	ATSVR_CMD_HADLER("AT+STASTOP","AT+STASTOP",atsvr_station_stop),
	ATSVR_CMD_HADLER("AT+STASTATIC","AT+STASTATIC=ip,mask,gate[,dns]",atsvr_station_static_ip),
	ATSVR_CMD_HADLER("AT+STAST","AT+STAST",atsvr_get_station_status),
	ATSVR_CMD_HADLER("AT+WSCAN","AT+WSCAN",atsvr_wlan_scan_cmd),

	ATSVR_CMD_HADLER("AT+SOFTAP","AT+SOFTAP=SSID,PWD",atsvr_softap_start),
	ATSVR_CMD_HADLER("AT+SOFTAPSTOP","AT+STASTOP",atsvr_softap_stop),
	ATSVR_CMD_HADLER("AT+APSTATIC","AT+APSTATIC=ip,mask,gate[,dns]",atsvr_softap_static_ip),
	ATSVR_CMD_HADLER("AT+APSTATE","AT+APSTATE",atsvr_get_softap_status),
};




void atsvr_cmd_init(void)
{
	atsvr_register_commands(atsvc_cmds_table, sizeof(atsvc_cmds_table) / sizeof(struct _atsvr_command));
	atsvr_extern_cmd_init();

	atsvr_wlan_init();
}

void __weak__ atsvr_extern_cmd_init(void)
{

}


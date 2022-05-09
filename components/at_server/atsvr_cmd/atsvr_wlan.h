#ifndef _ATSVR_WLAN_H_
#define _ATSVR_WLAN_H_

#define ATSVR_MAX_SSID_LEN             32

#define ATSVR_WLAN_DEFAULT_IP          "192.168.19.1"
#define ATSVR_WLAN_DEFAULT_GW          "192.168.19.1"
#define ATSVR_WLAN_DEFAULT_MASK        "255.255.255.0"
#define ATSVR_WLAN_DEFAULT_DNS         "192.168.19.1"

#define ATSVR_WLAN_STA_DEFAULT_IP          "192.168.0.19"
#define ATSVR_WLAN_STA_DEFAULT_GW          "192.168.0.19"
#define ATSVR_WLAN_STA_DEFAULT_MASK        "255.255.255.0"
#define ATSVR_WLAN_STA_DEFAULT_DNS         "192.168.0.1"

#define ATSVR_SCAN_TIMEOUT_MS              (4000)

#ifndef ATSVR_MACSTR
#define ATSVR_MACSTR "%02x:%02x:%02x:%02x:%02x:%02x "
#endif
#ifndef ATSVR_MAC2STR
#define ATSVR_MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#endif

#define ATSVR_RSSI2APPOWER(rssi)     (((rssi + 100) > 100) ? 100 : (rssi + 100))

typedef enum atsvr_wlan_sec_type_e
{
    ATSVR_SEC_TYPE_NONE = 0,    /**< Open system. */
    ATSVR_SEC_TYPE_WEP,         /**< Wired Equivalent Privacy. WEP security. */
    ATSVR_SEC_TYPE_WPA_TKIP,    /**< WPA /w TKIP */
    ATSVR_SEC_TYPE_WPA_AES,     /**< WPA /w AES */
    ATSVR_SEC_TYPE_WPA2_TKIP,   /**< WPA2 /w TKIP */
    ATSVR_SEC_TYPE_WPA2_AES,    /**< WPA2 /w AES */
    ATSVR_SEC_TYPE_WPA2_MIXED,  /**< WPA2 /w AES or TKIP */
    ATSVR_SEC_TYPE_WPA3_SAE,	  /**< WPA3 SAE */
    ATSVR_SEC_TYPE_WPA3_WPA2_MIXED, /** WPA3 SAE or WPA2 AES */
    ATSVR_SEC_TYPE_OWE,         /**< WPA3 OWE */
    ATSVR_SEC_TYPE_AUTO,        /**< It is used when calling @ref bkWlanStartAdv, It's read security type from scan result. */
}atsvr_wlan_sec_type;

typedef struct{
	char ssid[33];  /**< The SSID of an access point. */
	char ApPower;   /**< Signal strength, min:0, max:100. */
	char channel;	/**< The RF frequency, 1-13*/
	atsvr_wlan_sec_type security;	/**< Security type, @ref atsvr_wlan_sec_type*/
}atsvr_ap_item;

typedef struct _atsvr_scan_result{
	char ApNum;
	atsvr_ap_item *ApList;
}atsvr_scan_result;

extern void wlan_get_station_mac_address(char *mac);
extern void wlan_get_softap_mac_address(char *mac);
extern void wlan_set_station_dhcp(int en);
extern int wlan_get_station_dhcp(void);

extern int wlan_set_station_static_ip(char *ip,char *mask,char *gate,char *dns,char *dns2);
extern int wlan_set_station_dns(char *dns_ip,char *dns2_ip);

extern int wlan_get_station_cur_status(void);

extern int wlan_start_station_connect(char *my_ssid,char* connect_key);
extern int wlan_stop_station(void);


extern int wlan_softap_start(char *ap_ssid, char *ap_key);
extern int wlan_stop_softap(void);

extern int wlan_set_softap_static_ip(char *ip,char *mask,char *gate,char *dns_ip);
extern int wlan_set_softap_dns(char *dns_ip);
extern int wlan_get_softap_cur_status(void);
extern int wlan_scan_start(char *ssid);

extern int wlan_event_handler(int event);

extern atsvr_wlan_sec_type wlan2atsvr_sec_type(int sec_type);
extern int atsvr2wlan_sec_type(atsvr_wlan_sec_type sec_type);
extern int judge_the_string_is_ipv4_string(char *is_ip_string);

extern void atsvr_wlan_init(void);

#endif

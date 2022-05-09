#ifndef _ARCH_CONFIG_H_
#define _ARCH_CONFIG_H_

//TODO
// 1. Rename this file
// 2. Optimize this file

#include <common/bk_include.h>
#include "modules/wifi_types.h"

#define CONFIG_ROLE_NULL        0
#define CONFIG_ROLE_AP          1
#define CONFIG_ROLE_STA         2
#define CONFIG_ROLE_COEXIST     3

#define DEFAULT_CHANNEL_AP      11

struct wifi_ssid {
    /// Actual length of the SSID.
    uint8_t length;
    /// Array containing the SSID name.
    uint8_t array[32];
};

#if CONFIG_MAC_BSSID
#define MAC_ADDR_LEN		6
/// MAC address structure.
struct mac_bssid
{
    /// Array of 16-bit words that make up the MAC address.
    uint16_t array[MAC_ADDR_LEN/2];
} __packed;
#endif

struct wifi_bssid {
	uint8_t bssid[6];
};

typedef struct fast_connect_param {
	uint8_t bssid[6];
	uint8_t chann;
} fast_connect_param_t;

typedef struct general_param {
	uint8_t role;
	uint8_t dhcp_enable;
	uint32_t ip_addr;
	uint32_t ip_mask;
	uint32_t ip_gw;
} general_param_t;

typedef struct ap_param {
#if CONFIG_MAC_BSSID
	struct mac_bssid bssid;
#else
	struct wifi_bssid bssid;
#endif
	struct wifi_ssid ssid;

	uint8_t chann;
	uint8_t cipher_suite;
	uint8_t key[65];
	uint8_t key_len;
#if CONFIG_AP_VSIE
	uint8_t vsie[255];
	uint8_t vsie_len;
#endif
} ap_param_t;

typedef struct sta_param {
	uint8_t own_mac[6];
	struct wifi_ssid ssid;
	uint8_t cipher_suite;
	uint8_t key[65];
	uint8_t key_len;
	uint8_t fast_connect_set;
	fast_connect_param_t fast_connect;

#if CONFIG_COMPONENTS_STA_VSIE
	struct {
		uint8_t len;
		uint8_t buf[255];
	} vsies[NUM_WIFI_VENDOR_ELEM_FRAMES];
#endif

#if CONFIG_COMPONENTS_WPA2_ENTERPRISE
	/* starts of WPA2-Enterprise/WPA3-Enterprise EAP-TLS configuration */
	char eap[16];					   /**< phase1 authType: TLS/TTLS/SIM */
	char identity[32];				   /**< user identity */
	char ca[32];					   /**< CA certificate filename */
	char client_cert[32];			   /**< client's Certification filename in PEM,DER format */
	char private_key[32];			   /**< client's private key filename in PEM,DER format */
	char private_key_passwd[32];	   /**< client's private key password */
	char phase1[32];	               /**< client's phase1 parameters */
#endif
} sta_param_t;

extern general_param_t *g_wlan_general_param;
extern ap_param_t *g_ap_param_ptr;
extern sta_param_t *g_sta_param_ptr;

uint32_t cfg_param_init(void);

//Optimize it
__attribute__((weak)) uint32_t cfg_ap_is_open_system(void);
#endif

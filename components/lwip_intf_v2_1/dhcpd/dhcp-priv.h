#ifndef __DHCP_PRIV_H__
#define __DHCP_PRIV_H__

#include "lwip/sockets.h"

#if 0
#define dhcp_e(...)				\
	LWIP_LOGI("dhcp: %s\r\n", ##__VA_ARGS__)
#define dhcp_w(...)				\
	LWIP_LOGI("dhcp: %s\r\n", ##__VA_ARGS__)

#define dhcp_d(...)				\
	LWIP_LOGI("dhcp: %s\r\n", ##__VA_ARGS__)
#else
#define dhcp_d(...)  //LWIP_LOGI
#define dhcp_e(...)  //LWIP_LOGI
#define dhcp_w(...)  //LWIP_LOGI
#endif

#define SERVER_BUFFER_SIZE		1024
#define MAC_IP_CACHE_SIZE		8

#if CONFIG_MALLOC_STATIS
#define DHCP_SERVER_TASK_STACK_SIZE     2048
#else
#define DHCP_SERVER_TASK_STACK_SIZE     1536
#endif

struct client_mac_cache {
	uint8_t client_mac[6];    /* mac address of the connected device */
	uint32_t client_ip;       /* ip address of the connected device */
};

struct dhcp_server_data {
	int sock;
	int dnssock;
	int ctrlsock;
	int count_clients;       /* to keep count of cached devices */
	char *msg;
	struct sockaddr_in saddr;	/* dhcp server address */
	struct sockaddr_in dnsaddr;	/* dns server address */
	struct sockaddr_in uaddr;	/* unicast address */
	struct sockaddr_in baddr;	/* broadcast address */
	struct sockaddr_in ctrladdr;
	struct client_mac_cache ip_mac_mapping[MAC_IP_CACHE_SIZE];
	uint32_t netmask;		/* network order */
	uint32_t my_ip;		/* network order */
	uint32_t client_ip;	/* last address that was requested, network
				 * order */
	uint32_t current_ip;     /* keep track of assigned IP addresses */
	uint32_t router_ip;     /* router IP addresses */
    void *prv;
};

void dhcp_enable_nack_dns_server(void);
int dhcp_server_init(void *intrfc_handle);
void dhcp_server(void* data);
int dhcp_send_halt(void);
int dhcp_free_allocations(void);

#endif

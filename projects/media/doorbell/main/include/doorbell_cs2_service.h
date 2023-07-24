#ifndef __DOORBELL_CS2_SERVICE_H__
#define __DOORBELL_CS2_SERVICE_H__

#define CS2_P2P_TRANSFER_DELAY 20

#define SIZE_DID            64  // Device ID Size
#define SIZE_APILICENSE     24  // APILicense Size
#define SIZE_INITSTRING     256 // InitString Size
#define SIZE_WAKEUP_KEY     17  // WakeUp Key Size
#define SIZE_ARMINO_KEY     17  // WakeUp Key Size

#define SIZE_DATE      32   //// "[YYYY-MM-DD hh:mm:ss.xxx]"

#define CMD_P2P_CHANNEL     (0)
#define AUD_P2P_CHANNEL     (1)
#define IMG_P2P_CHANNEL     (2)


#define TIME_USE                (int)((end.tick_msec) - (begin.tick_msec))
#define TU_MS(begin, end)     (int)((end.tick_msec) - (begin.tick_msec))



typedef struct
{
	int year;
	int mon;
	int day;
	int week;
	int hour;
	int min;
	int sec;
	int msec;
	unsigned long tick_sec;
	unsigned long long tick_msec;
	char date[SIZE_DATE];
} time_info_t;

typedef struct
{
	int  skt;                       // Sockfd
	// struct sockaddr_in RemoteAddr;  // Remote IP:Port
	// struct sockaddr_in MyLocalAddr; // My Local IP:Port
	// struct sockaddr_in MyWanAddr;   // My Wan IP:Port
	char remote_ip[16];
	int remote_port;
	char local_ip[16];
	int local_port;
	char wan_ip[16];
	int wan_port;
	unsigned int connect_time;       // Connection build in ? Sec Before
	char did[24];                   // Device ID
	char bcord;   // I am Client or Device, 0: Client, 1: Device
	char bmode; // my define mode by PPCS_Check bmode(0:P2P(Including: LAN TCP/UDP), 1:RLY, 2:TCP); Mydefine: 0:LAN, 1:P2P, 2:RLY, 3:TCP, 4:RP2P.
	char mode[12];   // Connection mode: LAN/P2P/RLY/TCP.
	char reserved[2];
} session_info_t;

typedef struct
{
	char did[SIZE_DID];
	char apilicense[SIZE_APILICENSE];
	char initstring[SIZE_INITSTRING];
	char key[SIZE_ARMINO_KEY];
	char pwakeupkey[SIZE_WAKEUP_KEY];
} p2p_cs2_key_t;


const doorbell_service_interface_t *get_doorbell_cs2_service_interface(void);

#endif

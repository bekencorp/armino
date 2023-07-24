#ifndef __DOORBELL_UDP_SERVICE_H__
#define __DOORBELL_UDP_SERVICE_H__

bk_err_t doorbell_udp_service_init(void);
void doorbell_udp_update_remote_address(in_addr_t address);

#endif

#ifndef _APP_DEMO_TCP_H_
#define _APP_DEMO_TCP_H_

UINT32 app_demo_tcp_init(void);
void app_demo_tcp_deinit(void);
int app_demo_tcp_send_packet(uint8_t *data, uint32_t len);

#endif // _APP_DEMO_TCP_H_


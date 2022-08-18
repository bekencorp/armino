#ifndef LWIP_PING_H
#define LWIP_PING_H

extern int ping(char* target_name, uint32_t times, size_t size);
#ifdef CONFIG_IPV6
void ping_cmd(char *buf, int len, int argc, char **argv);
#endif
void ping_stop(void);
void ping_start(char* target_name, uint32_t times, size_t size);
#endif /* LWIP_PING_H */

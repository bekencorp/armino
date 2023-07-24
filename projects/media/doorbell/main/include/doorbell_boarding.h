#ifndef __DOORBELL_BOARDING_H__
#define __DOORBELL_BOARDING_H__

int doorbell_boarding_init(void);
void doorbell_boarding_event_notify(uint16_t opcode, int status);
void doorbell_boarding_ble_disable(void);

#endif

#ifndef SRV_H_
#define SRV_H_

#include <stdint.h>

uint8_t srv_recv(void* pkt, uint8_t max_size);

void srv_send(void* pkt, uint8_t size);

#endif /*  SRV_H_ */


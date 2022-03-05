#ifndef FAILSAFE_H_
#define FAILSAFE_H_

#include <stdint.h>

void failsafe_init(uint8_t sec, void (*callback)(void));
void failsafe_dec(uint8_t sec);
void failsafe_reset(uint8_t counter_idx);

#endif /* FAILSAFE_H_ */

#ifndef FAILSAFE_H_
#define FAILSAFE_H_

#include <stdint.h>

void failsafe_init(uint8_t sec);
void failsafe_dec(uint8_t sec);
void failsafe_reset();

#endif /* FAILSAFE_H_ */

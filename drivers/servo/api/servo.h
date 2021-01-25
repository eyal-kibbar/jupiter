#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>


void servo_init(void);

void servo_attach(uint8_t pin);
void servo_set(uint8_t pin, uint16_t duty);
void servo_set_mircoseconds(uint8_t pin, uint16_t microseconds);







#endif /* SERVO_H_ */

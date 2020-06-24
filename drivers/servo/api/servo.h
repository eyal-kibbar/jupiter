#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>


#define SERVO_50HZ_COMPARE     250
#define SERVO_50HZ_RESOLUTION  160
#define SERVO_50HZ_SCALER      8

// microseconds per 1 resolution value
#define SERVO_50HZ_TICK_US ((SERVO_50HZ_SCALER * SERVO_50HZ_COMPARE) / (F_CPU / 1000000))

#if (SERVO_50HZ_TICK_US * SERVO_50HZ_RESOLUTION) != 20000
  #error "cycle is not 50 Hz"
#endif

void servo_init(void);

void servo_attach(uint8_t pin);
void servo_set(uint8_t pin, uint8_t duty);
void servo_set_mircoseconds(uint8_t pin, uint16_t microseconds);







#endif /* SERVO_H_ */

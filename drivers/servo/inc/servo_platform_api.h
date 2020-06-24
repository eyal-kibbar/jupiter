#ifndef SERVO_PLATFORM_API_H_
#define SERVO_PLATFORM_API_H_

#include <stdint.h>

void servo_platform_init();
void servo_platform_attach(uint8_t pin);

static void servo_tick();


#endif /* SERVO_PLATFORM_API_H_ */

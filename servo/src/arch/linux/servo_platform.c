
#include "servo_platform_api.h"

#include <stdio.h>

void servo_platform_init()
{
}

void servo_attach(uint8_t pin)
{
}



void servo_set_pins(uint16_t status, uint16_t mask)
{
	printf("status: %04x  mask: %04x\n", status, mask);
}

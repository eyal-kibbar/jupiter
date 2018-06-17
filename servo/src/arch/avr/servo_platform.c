
#include "servo_platform_api.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void servo_platform_init()
{
    TCCR0B = _BV(CS00);  // set clock with 1024 prescalar
    TIMSK0 = _BV(TOIE0); // enable overflow interrupt
}

void servo_attach(uint8_t pin)
{
    if (pin < 8) {
        DDRD |= _BV(pin);
    }
    else {
        DDRB |= _BV(pin - 8);
    }
}




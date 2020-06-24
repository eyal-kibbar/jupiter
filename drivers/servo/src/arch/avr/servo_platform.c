
#include "servo_platform_api.h"
#include "servo.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void servo_platform_init()
{
    // initialize 50Hz:
    TCCR0B = _BV(CS01);   // set clock with 8 prescalar
    OCR0A  = SERVO_50HZ_COMPARE-1; // set compare A value
    TIMSK0 = _BV(OCIE0A); // enable compare match A interrupt
}

void servo_platform_attach(uint8_t pin)
{
    if (pin < 8) {
        DDRD |= _BV(pin);
    }
    else {
        DDRB |= _BV(pin - 8);
    }
}

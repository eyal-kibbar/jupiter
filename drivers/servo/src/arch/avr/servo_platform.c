
#include "servo_platform_api.h"
#include "servo.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io_platform.h"

void servo_platform_init()
{
    // initialize 50Hz:
    OCR0A  = 255; // set compare A value
    TCCR0A = _BV(WGM01); // set CTC mode
    TIMSK0 = _BV(OCIE0A); // enable compare match A interrupt
    TCCR0B = _BV(CS01);   // set clock with 8 prescalar (tick every 0.5 microseconds)
}

void servo_platform_attach(uint8_t pin)
{
    io_pin_output(pin);
}

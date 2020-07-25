#include "io_platform.h"
#include <avr/io.h>


void io_pin_output(uint8_t pin)
{
    if (pin < 8) {
        DDRD |= _BV(pin);
    }
    else {
        DDRB |= _BV(pin - 8);
    }
}

void io_pin_input(uint8_t pin)
{
    if (pin < 8) {
        DDRD &= ~(_BV(pin));
    }
    else {
        DDRB &= ~(_BV(pin - 8));
    }
}

void io_pin_set(uint8_t pin)
{
    if (pin < 8) {
        PORTD |= _BV(pin);
    }
    else {
        PORTB |= _BV(pin - 8);
    }
}

void io_pin_clr(uint8_t pin)
{
    if (pin < 8) {
        PORTD &= ~(_BV(pin));
    }
    else {
        PORTB &= ~(_BV(pin - 8));
    }
}

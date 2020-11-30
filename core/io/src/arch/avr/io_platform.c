#include "io_platform.h"
#include <avr/io.h>
#include <stdlib.h>

void io_pin_port(uint8_t pin, uint8_t** port, uint8_t* mask)
{
    if (pin == 0xFF) {
        *port = NULL;
        *mask = 0;
    }
    else if (pin < 8) {
        *port = (uint8_t*)&PIND;
        *mask = _BV(pin);
    }
    else {
        *port = (uint8_t*)&PINB;
        *mask = _BV(pin - 8);
    }
}


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

uint8_t io_pin_get(uint8_t pin)
{
    if (pin < 8) {
        return !!(PIND & _BV(pin));
    }
    else {
        return !!(PINB & _BV(pin - 8));
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

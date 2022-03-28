
#include "servo_platform_api.h"
#include "servo.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io_platform.h"
#include "logging.h"

#define SERVO_CLK_PRESCALER 8

void servo_platform_init()
{
    OCR1A  = ((F_CPU / SERVO_CLK_PRESCALER) / SERVO_CYCLE_FREQ) - 1;
    TCCR1B = _BV(WGM12) | _BV(CS11); // set CTC OCR1A mode, 8 prescaler
    OCR1B  = 0xFFFF; // avoid having an update interrupt
    TIMSK1 = _BV(OCIE1B) | _BV(OCIE1A);

    //LOG_INFO(SERVO, "OCR1A: %u", OCR1A);
}

void servo_platform_attach(uint8_t pin)
{
    io_pin_output(pin);
}

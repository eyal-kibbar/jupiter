#ifndef SERVO_PLATFORM_H_
#define SERVO_PLATFORM_H_

#include <avr/interrupt.h>
#include "servo_platform_api.h"


#define SERVO_CLK_NEXT_TOGGLE(ticks) do { OCR1B = (ticks); } while(0)

void servo_platform_init();

void servo_platform_attach(uint8_t pin);

static void servo_toggle();
static void servo_new_cycle();

static void servo_set_pins(uint16_t status, uint16_t mask)
{
    union port_t {
        struct {
            uint8_t portd;
            uint8_t portb;
        };
        uint16_t raw;
    };

    union port_t port;
    union port_t port_mask;

    port.raw = status;
    port_mask.raw = mask;

    if (port_mask.portb) {
        PORTB = (PORTB & (~port_mask.portb)) | (port.portb & port_mask.portb);
    }

    if (port_mask.portd) {
        PORTD = (PORTD & (~port_mask.portd)) | (port.portd & port_mask.portd);
    }
}

ISR(TIMER1_COMPA_vect)
{
    servo_new_cycle();
}

ISR(TIMER1_COMPB_vect)
{
    servo_toggle();
}



#endif /* SERVO_PLATFORM_H_ */

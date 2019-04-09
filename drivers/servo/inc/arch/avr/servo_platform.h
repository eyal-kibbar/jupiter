#ifndef SERVO_PLATFORM_H_
#define SERVO_PLATFORM_H_

#include <avr/interrupt.h>
#include "servo_platform_api.h"

#define servo_platform_cli() cli()
#define servo_platform_sei() sei()


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


ISR(TIMER0_OVF_vect)
{
    servo_tick();
}


#endif /* SERVO_PLATFORM_H_ */

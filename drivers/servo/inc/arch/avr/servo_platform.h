#ifndef SERVO_PLATFORM_H_
#define SERVO_PLATFORM_H_

#include <avr/interrupt.h>
#include "servo_platform_api.h"

#define SERVO_MAX_SLEEP_TICKS 0xFF

// 8 prescaler
#define SERVO_US2TICKS(us) ((us)*(F_CPU/8000000))
#define SERVO_T_TICKS SERVO_US2TICKS(20000)



#define SERVO_CLK_SET_LIMIT(limit) do { OCR0A = (limit); } while(0)
#define SERVO_CLK_GET(a) do { *(a) = TCNT0; } while (0)
#define SERVO_CLK_CLR() do { TCNT0 = 0; } while (0)

void servo_platform_init();

void servo_platform_attach(uint8_t pin);

static void servo_tick();

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

ISR(TIMER0_COMPA_vect)
{
    servo_tick();
}


#endif /* SERVO_PLATFORM_H_ */

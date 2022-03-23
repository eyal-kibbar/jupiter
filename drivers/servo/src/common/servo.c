#include "servo.h"
#include "utils.h"
#include "io_platform.h"
#include "servo_platform.h"
#include "logging.h"

#define SERVO_MIN_ISR_US 20
#define SERVO_MIN_CYCLE_ISR_US 50

struct servo_pin_s {
    uint8_t  pin;
    uint16_t microseconds;
};

struct servo_update_s {
    uint16_t mask;
    uint16_t tick;
};

struct servo_t {
    struct servo_pin_s pins[SERVO_MAX_PIN];
    struct servo_update_s updates[SERVO_MAX_PIN];

    uint8_t curr;
    uint8_t num_pins;
    uint8_t update_needed;
    uint16_t attached;
    uint16_t non_zero;
};


volatile static struct servo_t servo;

void servo_init(void)
{
    servo_platform_init();
}

// CTC mode, 2 types of interrupts:
// 1. OCR1A - cycle has finished, start a new 20ms cycle
// 2. OCR1B - need to perform a change

void servo_attach(uint8_t pin)
{
    platform_cli();
    servo.pins[servo.num_pins].pin = pin;
    servo.pins[servo.num_pins].microseconds = 0;
    ++servo.num_pins;
    servo.update_needed = 1;
    servo_platform_attach(pin);
    servo.attached |= 1 << pin;
    platform_sei();
}


void servo_set_mircoseconds(uint8_t pin, uint16_t microseconds)
{
    int i, j;

    platform_cli();
    for (i=0; i < servo.num_pins && servo.pins[i].pin != pin; ++i);

    if (microseconds == servo.pins[i].microseconds) {
        // no need to update
        platform_sei();
        return;
    }
    if (0 != microseconds) {
        servo.non_zero |= 1 << i;
    }
    else {
        servo.non_zero &= ~(1 <<i);
    }

    servo.pins[i].microseconds = microseconds;
    servo.update_needed = 1;

    // sort
    for (i=0; i < servo.num_pins-1; ++i) {
        for (j=i+1; j < servo.num_pins; ++j) {
            if (servo.pins[i].microseconds > servo.pins[j].microseconds) {
                struct servo_pin_s temp_pin = servo.pins[i];
                servo.pins[i] = servo.pins[j];
                servo.pins[j] = temp_pin;
            }
        }
    }

    platform_sei();

    //LOG_INFO(SERVO, "setting pin %d (%d): %u", pin, i, microseconds);
}

static void servo_new_cycle()
{
    uint8_t i, j;
    uint8_t idx_first;
    uint16_t mask;

    // set all the attached except the true 0 microseconds pins
    servo_set_pins(~0, servo.attached & servo.non_zero);
    servo.curr = 0;
    
    // skip pins with very short duty cycle
    for (idx_first=0, mask=0; idx_first < servo.num_pins && servo.pins[idx_first].microseconds < SERVO_MIN_CYCLE_ISR_US; ++idx_first) {
        mask |= 1 << servo.pins[idx_first].pin;
    }

    if (0 != servo.update_needed && idx_first < servo.num_pins) {

        // initialize the first mask and tick
        servo.updates[0].mask = mask | (1 << servo.pins[idx_first].pin);
        servo.updates[0].tick = servo.pins[idx_first].microseconds * 2; // 2 ticks per microsecond

        // calculate the next tick for every pin
        for (i=idx_first+1, j=0; i < servo.num_pins; ++i) {
            uint16_t tick = servo.pins[i].microseconds * 2; // 2 ticks per microsecond

            if ((tick - servo.updates[j].tick) < SERVO_MIN_ISR_US) {
                // in this case, the next update is too close, update them together
                tick = servo.updates[j].tick;
            }
            else {
                // next tick is far enough, update it separately
                ++j;
                servo.updates[j].mask = servo.updates[j-1].mask;
            }
            servo.updates[j].mask |= 1 << servo.pins[i].pin;
            servo.updates[j].tick = tick;
        }

        servo.updates[j+1].tick = 0xFFFF; // set last pin timer to be beyond the OCR1A

    }

    servo.update_needed = 0; // mark updated
    // kickstart toggle by setting the first OCR1B
    if (idx_first < servo.num_pins) {
        SERVO_CLK_NEXT_TOGGLE(servo.updates[0].tick);
    }

    // clear state for short duty cycle pins
    servo_set_pins(~mask, servo.attached);
}

static void servo_toggle()
{
    servo_set_pins(~servo.updates[servo.curr].mask, servo.attached);
    ++servo.curr;
    SERVO_CLK_NEXT_TOGGLE(servo.updates[servo.curr].tick);
}




void servo_set(uint8_t pin, uint16_t duty)
{

}

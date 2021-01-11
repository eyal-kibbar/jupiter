#include "servo.h"
#include "utils.h"
#include "io_platform.h"
#include "servo_platform.h"
#include "logging.h"


struct servo_pin_s {
    uint8_t pin;
    uint16_t ticks;
};


struct servo_t {
    struct servo_pin_s pins[SERVO_MAX_PIN];
    uint8_t num_pins;
    uint8_t curr;

    uint16_t curr_ticks;
    uint16_t mask;
    uint16_t attached;
};

static struct servo_t servo;

void servo_init(void)
{
    servo_platform_init();
}

void servo_attach(uint8_t pin)
{
    platform_cli();
    servo.pins[servo.num_pins++].pin = pin;
    servo_platform_attach(pin);
    servo.attached |= 1 << pin;
    platform_sei();
}

void servo_set(uint8_t pin, uint16_t duty)
{

}
void servo_set_mircoseconds(uint8_t pin, uint16_t microseconds)
{
    int i;
    uint16_t ticks = SERVO_US2TICKS(microseconds);

    //LOG_INFO(SERVO, "ticks: %u", ticks);

    for (i=0; i < servo.num_pins; ++i);

    platform_cli();
    // find pin
    for (i=0; pin != servo.pins[i].pin; ++i);
    
    // bubble left
    while (i > 0 && servo.pins[i-1].ticks > ticks) {
        servo.pins[i] = servo.pins[i-1];
        --i;
    }

    // bubble right
    while (i < (servo.num_pins-1) && servo.pins[i+1].ticks < ticks) {
        servo.pins[i] = servo.pins[i+1];
        ++i;
    }

    servo.pins[i].pin = pin;
    servo.pins[i].ticks = ticks;
    platform_sei();
}

static void servo_tick()
{
    uint8_t sleep_ticks;
    uint8_t used_ticks;
    // set countup clock to SERVO_MAX_SLEEP_TICKS
    SERVO_CLK_SET_LIMIT(SERVO_MAX_SLEEP_TICKS); // avoid nested interrupt
    do {
        if (servo.curr == servo.num_pins) {
            // sleep till the end of the cycle
            sleep_ticks = MIN(SERVO_MAX_SLEEP_TICKS, SERVO_T_TICKS - servo.curr_ticks);
            if (0 == sleep_ticks) {
                // start a new cycle
                servo.curr = 0;
                servo.curr_ticks = 0;
                servo.mask = 0;
                servo_set_pins(0xFFFF, servo.attached);
            }
        }
        else {
            sleep_ticks = MIN(SERVO_MAX_SLEEP_TICKS, servo.pins[servo.curr].ticks - servo.curr_ticks);
            if (0 == sleep_ticks) {
                servo.mask |= 1 << servo.pins[servo.curr++].pin;
                servo_set_pins(~servo.mask, servo.attached);
            }
        }


        servo.curr_ticks += sleep_ticks;
        SERVO_CLK_GET(&used_ticks);
        // count the number of ticks already
        if (used_ticks < sleep_ticks) {
            SERVO_CLK_SET_LIMIT(sleep_ticks);
            return;
        }
        else {
            // counter has already passed the number of ticks we need to sleep
            // restart timer
            SERVO_CLK_CLR();
        }
    } while (1);
}

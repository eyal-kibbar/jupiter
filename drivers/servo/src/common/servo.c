
#include "ganymede.h"
#include "utils.h"
#include "logging.h"
#include "servo.h"
#include "servo_platform.h"
#include "servo_platform_api.h"
#include <stdlib.h>


typedef struct servo_pin_s {
    uint8_t pin_idx;
    uint16_t duty;
} servo_pin_t;

typedef struct servo_s {
    servo_pin_t pin[SERVO_MAX_PIN];
    uint8_t num_attached;
    uint8_t curr;

    uint16_t tick;
    uint16_t attached;
    uint16_t mask;
} servo_t;


static servo_t servo;


COMPILER_CHECK(SERVO_MAX_PIN <= (8*sizeof(servo.mask))); // need to add more bits to mask

void servo_init(void)
{
    servo.tick = SERVO_50HZ_RESOLUTION;
    servo_platform_init();
}

void servo_attach(uint8_t pin)
{
    uint8_t n = servo.num_attached;
    servo_platform_attach(pin);

    servo.pin[n].pin_idx = pin;
    servo.pin[n].duty = SERVO_50HZ_RESOLUTION;
    servo.attached |= (1 << pin);
    ++servo.num_attached;
}

static int servo_cmp(const void* a, const void* b)
{
    const servo_pin_t* pa = (const servo_pin_t*)a;
    const servo_pin_t* pb = (const servo_pin_t*)b;

    return (int)pa->duty - (int)pb->duty;
}

void servo_set_mircoseconds(uint8_t pin, uint16_t microseconds)
{
    servo_set(pin, microseconds / SERVO_50HZ_TICK_US);
}

void servo_set(uint8_t pin, uint16_t duty)
{
    uint8_t i;

    LOG_INFO(SERVO, "setting pin %u duty %u", pin, duty);

    duty = SERVO_50HZ_RESOLUTION - duty;


    for (i=0; i < servo.num_attached; ++i ) {
        if (servo.pin[i].pin_idx != pin) {
            continue;
        }

        servo_platform_cli();
        servo.pin[i].duty = duty;
        qsort(servo.pin, servo.num_attached, sizeof(servo_pin_t), servo_cmp);
        servo_platform_sei();
        return;
    }
}

static void servo_tick()
{
    uint8_t need_change = 0;
    const servo_pin_t* c;


    if (servo.tick == SERVO_50HZ_RESOLUTION) {
        servo.curr = 0;
        servo.mask = 0;
        servo.tick = 0;
        need_change = 1;
    }

    ++servo.tick;

    for (c = &servo.pin[servo.curr];
         servo.curr < servo.num_attached && c->duty < servo.tick;
         c = &servo.pin[++servo.curr]) {

        servo.mask |= 1 << c->pin_idx;
        need_change = 1;
    }

    if (need_change) {
        // enable the pins
        servo_set_pins(servo.mask, servo.attached);
    }
}

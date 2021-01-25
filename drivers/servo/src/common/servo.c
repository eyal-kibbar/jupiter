#include "servo.h"
#include "utils.h"
#include "io_platform.h"
#include "servo_platform.h"
#include "logging.h"


struct servo_pin_s {
    uint8_t  pin;
    uint16_t microseconds;
    uint16_t ticks;
};

struct servo_t {
    struct servo_pin_s pins[SERVO_MAX_PIN];
    uint8_t curr;
    uint8_t num_pins;
    uint16_t mask;
    uint16_t attached;
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
    servo.pins[servo.num_pins].microseconds = 20000;
    ++servo.num_pins;
    servo_platform_attach(pin);
    servo.attached |= 1 << pin;
    platform_sei();
}


void servo_set_mircoseconds(uint8_t pin, uint16_t microseconds)
{
    int i;
    platform_cli();
    for (i=0; i < servo.num_pins && servo.pins[i].pin != pin; ++i);

    servo.pins[i].microseconds = 20000 - microseconds;
    platform_sei();

    //LOG_INFO(SERVO, "setting pin %d (%d): %u", pin, i, microseconds);
}

static int servo_pins_ticks_compare(const void* a, const void* b)
{
    const struct servo_pin_s* pa = (const struct servo_pin_s*)a;
    const struct servo_pin_s* pb = (const struct servo_pin_s*)b;

    if (pa->ticks > pb->ticks) return 1;
    if (pa->ticks < pb->ticks) return -1;
    return 0;
}

static void servo_new_cycle()
{
    int i, j;

    // clear all the attached
    servo_set_pins(0, servo.attached);
    servo.mask = 0;
    servo.curr = 0;
    //toggle = 0;

    for (i=0; i < servo.num_pins; ++i) {
        servo.pins[i].ticks = servo.pins[i].microseconds * 2; // 2 ticks per microsecond
    }
    servo.pins[servo.num_pins].ticks = 0xFFFF; // set last pin timer to be beyond the OCR1A
    // TODO: sort pins by ticks
    for (i=0; i < servo.num_pins-1; ++i) {
        for (j=i+1; j < servo.num_pins; ++j) {
            if (servo.pins[i].ticks > servo.pins[j].ticks) {
                struct servo_pin_s temp_pin = servo.pins[i];
                servo.pins[i] = servo.pins[j];
                servo.pins[j] = temp_pin;
            }
        }
    }


    (servo.pins, servo.num_pins, sizeof(*servo.pins), servo_pins_ticks_compare);

    // kickstart toggle by setting the first OCR1B
    SERVO_CLK_NEXT_TOGGLE(servo.pins[0].ticks);
}

static void servo_toggle()
{
    uint16_t prev_ticks, next_ticks;

    do {
        prev_ticks = servo.pins[servo.curr].ticks;
        servo.mask |= 1 << servo.pins[servo.curr].pin;
        servo_set_pins(servo.mask, servo.attached);
        next_ticks = servo.pins[++servo.curr].ticks;
    } while ( (servo.curr < servo.num_pins) && (next_ticks - prev_ticks) < 50); // continue toggling next if we don't have enough time to set the interrupt

    SERVO_CLK_NEXT_TOGGLE(next_ticks);
}




void servo_set(uint8_t pin, uint16_t duty)
{

}

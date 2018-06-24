
#include "ganymede.h"
#include "logging.h"
#include "servo.h"
#include "servo_platform.h"
#include "servo_platform_api.h"


typedef struct servo_pin_s {
    uint8_t next_idx;
    uint8_t duty;
} servo_pin_t;

typedef struct servo_s {
    servo_pin_t pin[SERVO_MAX_PIN];
    uint8_t first_idx;
    uint8_t curr_idx;
    uint8_t tick;
    uint16_t attached;
    uint16_t mask;
} servo_t;


static servo_t servo;


void servo_init(void)
{
    int i;
    servo_platform_init();
    //servo.curr_idx = servo.first_idx = SERVO_MAX_PIN;
}


void servo_set(uint8_t pin, uint8_t duty)
{
    uint16_t mask;
    uint8_t prev;
    
    LOG_INFO(SERVO, "setting pin %u duty %u", pin, duty);
    
    servo_platform_cli();
    
    // remove from sorted list
    if (servo.first_idx == 0) {
        // nothing to remove
    }
    else if (pin == servo.first_idx) {
        servo.first_idx = servo.pin[pin].next_idx;
    }
    else {
        for (prev = servo.first_idx;
             servo.pin[prev].next_idx != 0 &&
             servo.pin[prev].next_idx != pin;
             prev = servo.pin[prev].next_idx);
        
        servo.pin[prev].next_idx = servo.pin[pin].next_idx;
    }
    
    //gmd_uart_write("cc\r\n",4);
    // add to sorted list
    if (servo.first_idx == 0 || duty <= servo.pin[servo.first_idx].duty) {
        servo.pin[pin].next_idx = servo.first_idx;
        servo.first_idx = pin;
    }
    else {
        for (prev = servo.first_idx; 
             servo.pin[prev].next_idx != 0 &&
             servo.pin[prev].duty < duty;
             prev = servo.pin[prev].next_idx);
        
        servo.pin[pin].next_idx = servo.pin[prev].next_idx;
    }
    
    //gmd_uart_write("dd\r\n",4);
    // update duty
    servo.pin[pin].duty = duty;
    
    servo.attached |= 1 << pin;
    servo_platform_sei();
    
}




void servo_tick()
{
    uint8_t need_change = 0;
    
    if (servo.tick == 0 && servo.first_idx != 0) {
        servo.curr_idx = servo.first_idx;
        servo.mask = servo.attached;
        need_change = 1;
    }
    
    ++servo.tick;
    
    // advance curr if duty cycle passed
    while (servo.curr_idx != 0 &&
           servo.pin[servo.curr_idx].duty < servo.tick) {
        
        servo.mask ^= (1 << servo.curr_idx);
        servo.curr_idx = servo.pin[servo.curr_idx].next_idx;
        need_change = 1;
    }
    
    if (need_change) {
        // enable the pins
        servo_set_pins(servo.mask, servo.attached);
    }
}




















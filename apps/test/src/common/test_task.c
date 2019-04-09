#include "ganymede.h"
#include "logging.h"

//#include "mpu.h"

#include <stdio.h>
#include <string.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include <math.h>

#include "servo.h"


void setup()
{
    //DDRD = 0xC;
    //PORTD = _BV(2);
    EICRA = 0;

    //mpu_init();

}

static volatile uint8_t flag;


ISR(INT0_vect)
{
    LOG_INFO(TEST, "im here");
    EIMSK = 0;
    flag = 2;
}



void loop()
{
    flag = 0;
    LOG_INFO(test, "testing 123");
    //gmd_delay(3000);



    EIMSK = _BV(INT0);
    gmd_wfe(&flag, 0x2, 0);
    //flag = 0;
    //if (PIND & 0xC) {
    //    LOG_INFO(TEST, "int: %x", PIND & 0xC);
    //}

}


TASK(STACK_SIZE);

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
    DDRB = 0b00000000;
    //PORTB = 0b00000001;
    //DDRD = 0xC;
    //PORTD = _BV(2);
    //EICRA = 0;

    //mpu_init();

}

static volatile uint8_t flag;

/*
ISR(INT0_vect)
{
    LOG_INFO(TEST, "im here");
    EIMSK = 0;
    flag = 2;
}
*/

#define BAUD 9600
#define CYCLES_PER_BIT (F_CPU / BAUD)

#define XMIT_CYCLES_BIAS 10
#define RECV_CYCLES_BIAS 10

#define delay_cycles __builtin_avr_delay_cycles

#define XMIT_BIT_DELAY() delay_cycles(CYCLES_PER_BIT - XMIT_CYCLES_BIAS)
#define RECV_BIT_DELAY() delay_cycles(CYCLES_PER_BIT - RECV_CYCLES_BIAS)


void swuart_send(const uint8_t* buff, uint8_t len)
{
    uint8_t b;
    uint8_t i;
    uint8_t mask[2];

    //PORTB = 0b00000000;

    for (;len;--len, ++buff) {

        cli();

        mask[0] = PORTB & ~0b00000001;
        mask[1] = mask[0] | 0b00000001;

        // send start bit
        PORTB = mask[0];
        XMIT_BIT_DELAY();

        // send bit after bit
        b = *buff;
        for (i=0; i < 8; ++i, b >>= 1) {
            PORTB = mask[(b & 0x1)];
            XMIT_BIT_DELAY();
        }

        // send stop bit
        PORTB = mask[1];
        XMIT_BIT_DELAY();

        sei();
    }

}

int swuart_recv(uint8_t* buff, uint8_t len)
{
    uint8_t b;
    uint8_t i;

    // wait for start bit
    while (PORTB & 0b00000001);

    RECV_BIT_DELAY();

    for (;len;--len, ++buff) {

        // read bit after bit
        for (i=0; i < 8; ++i, b <<= 1) {
            b |= !!(PORTB & 0b00000001);
            RECV_BIT_DELAY();
        }

        // make sure stop bit is received
        //if (!(PORTB & 0b00000001)) {
        //    return -1;
        //}

        *buff = b;

        RECV_BIT_DELAY();
    }

    return 0;
}



void loop()
{
    //const char* n = "eyal\n\r";
    char n[1];
    int ret;
    flag = 0;
    //LOG_INFO(test, "testing 123");


    //PORTB = 0b0000001;
    //gmd_delay(1000);
    ret = swuart_recv(( uint8_t*)n, sizeof n);
    LOG_INFO(test, "got: %c %d", n[0], ret);
    //PORTB = 0b00000000;
    gmd_delay(5000);



    //EIMSK = _BV(INT0);
    //gmd_wfe(&flag, 0x2, 0);
    //flag = 0;
    //if (PIND & 0xC) {
    //    LOG_INFO(TEST, "int: %x", PIND & 0xC);
    //}

}


TASK(STACK_SIZE);

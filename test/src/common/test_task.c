#include "ganymede.h"
#include "logging.h"

//#include "mpu.h"

#include <stdio.h>
#include <string.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include <math.h>


void setup()
{
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
    flag = 1;
}
*/

int16_t calc_angle(int16_t x, int16_t z)
{
    float xf = (float)x;
    float zf = (float)z;
    float res;
    
    res = atan(xf/zf) * (180 / M_PI);
    
    return (int16_t)roundf(res);
}

void loop()
{
    
    LOG_INFO(test, "testing 123");
    gmd_delay(3000);
#if 0
    
    uint8_t val = 0;
    int16_t x, y, z;
    volatile uint8_t evt = 0;
    uint16_t timeout;
    uint8_t pkt[10];
    
    val = srv_recv(pkt, 10);
    
    
    
    //EIMSK = _BV(INT0);
    //gmd_wfe(&flag);
    //flag = 0;
    //if (PIND & 0xC) {
    //    LOG_INFO(TEST, "int: %x", PIND & 0xC);
    //}
    
//#if 0
    if (0 == (val = mpu_get_status())) {
        return;
    }
    
    
    mpu_get_accel(&x, &y, &z);
    LOG_INFO(TEST, "(%6d %6d %6d) %3d %3d", x, y, z,
             calc_angle(x, z),
             calc_angle(y, z));
    gmd_delay(3000);
#endif

#if 0
    char c[4];
    char* eol = "\n\r";
    gmd_io_tx_t tx[] = {
        { .isw = 0, .buf = (uint8_t*)c, .len = 3,   .off = 0},
        { .isw = 1, .buf = (uint8_t*)c, .len = 3,   .off = 0 },
        { .isw = 1, .buf = (uint8_t*)eol, .len = 2, .off = 0 },
    };
    
    c[3] = '\0';
    LOG_INFO(TEST, "waiting ... ");
    gmd_uart_sg(tx, 3, 3000);
    
    LOG_INFO(TEST, "got '%s'", c);
    
    //timeout = gmd_wfe(&evt, 1000);
    //LOG_INFO(TEST, "waited %d ms", timeout);
#endif
}


TASK(STACK_SIZE);

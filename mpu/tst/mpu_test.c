#include "ganymede.h"
#include "logging.h"
#include "mpu.h"

#include <stdio.h>
#include <string.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include <math.h>


void setup()
{
    mpu_init();

}

int16_t calc_angle(int16_t x, int16_t z)
{
    float xf = (float)x;
    float zf = (float)z;
    float res;
    
    res = atan2(xf, zf) * (180 / M_PI);
    
    return (int16_t)roundf(res);
}

void loop()
{
    int16_t x, y, z;
    uint8_t val;
    
    if (0 == (val = mpu_get_status())) {
        gmd_delay(100);
        return;
    }
    
    
    mpu_get_accel(&x, &y, &z);
    LOG_INFO(TEST, "(%6d %6d %6d) %3d %3d", x, y, z,
             calc_angle(x, z),
             calc_angle(y, z));
}


TASK(STACK_SIZE);

#include "ganymede.h"
#include "io.h"
#include "logging.h"
#include "mpu.h"

#include <stdio.h>
#include <string.h>

#include <avr/interrupt.h>
//#include <avr/io.h>

#include <math.h>


void setup()
{
    io_uart_init(9600);
    io_logging_init();
    io_i2c_master_init();
}

int16_t calc_angle(float x,float z)
{
    float res;

    res = atan2(x, z) * (180 / M_PI);

    return (int16_t)roundf(res);
}

mpu_data_t data;

void loop()
{
    int16_t x, y, z;
    int16_t gx, gy, gz;
    uint8_t val;
    static uint8_t is_initialized = 0;

    if (0 == is_initialized) {
        mpu_init();
        is_initialized = 1;
    }

    if (0 == (val = mpu_get_status())) {
        gmd_delay(100);
        return;
    }

    mpu_read(&data);
    x = (int16_t)roundf(data.accel[0] * 1000);
    y = (int16_t)roundf(data.accel[1] * 1000);
    z = (int16_t)roundf(data.accel[2] * 1000);

    gx = (int16_t)roundf(data.gyro[0] * 1000);
    gy = (int16_t)roundf(data.gyro[1] * 1000);
    gz = (int16_t)roundf(data.gyro[2] * 1000);

    //mpu_get_accel(&x, &y, &z);
    LOG_INFO(TEST, "(%6d %6d %6d) %3d %3d (%6d %6d %6d) %6d", x, y, z,
             calc_angle(x / 1000.0f, z / 1000.0f),
             calc_angle(y / 1000.0f, z / 1000.0f),
            gx, gy, gz,
            (int16_t)roundf(data.temperature));

}


TASK(STACK_SIZE);

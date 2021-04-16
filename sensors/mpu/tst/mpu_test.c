#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include "io_platform.h"
#include "mpu.h"
#include "math.h"
#include <avr/io.h>


void setup()
{
    io_uart_init(0, 57600);
    io_logging_init();
    io_i2c_master_init();
}



void init()
{
    mpu_init();
    // initialize 16bit clock with 64 prescaler
    // this gives us a 4 microseconds per tick: (16Mhz/64)^-1
    TCCR1B = _BV(CS11) | _BV(CS10);
    TCNT1H = 0;
    TCNT1L = 0;

    mpu_calibrate();
}

static float calc_angle(float x,float z)
{
    float res;

    res = atan2(x, z) * (180 / M_PI);

    return res;
}


mpu_data_t data;
float angles[3];
float gyro_integral_angles[3];

void loop()
{
    int16_t x, y, z;
    int16_t gx, gy, gz;
    uint8_t val = 0;
    float accel_angles[3];
    float gyro_angles[3];

    uint16_t curr_ts;
    uint8_t* p_curr_ts = (uint8_t*)&curr_ts;
    float dt;

    if (0 == (0x1 & (val = mpu_get_status()))) {
        LOG_INFO(TEST, "data not ready: %02x", val);
        return;
    }

    // read and clear the clock
    p_curr_ts[0] = TCNT1L;
    p_curr_ts[1] = TCNT1H;
    TCNT1H = 0;
    TCNT1L = 0;

    mpu_read(&data);

    accel_angles[0] = calc_angle(-data.accel[0], sqrt(data.accel[1]*data.accel[1] + data.accel[2]*data.accel[2]));
    accel_angles[1] = calc_angle(data.accel[1],  sqrt(data.accel[1]*data.accel[1] + data.accel[2]*data.accel[2]));

    // convert clock ticks to seconds (gyro output is in degrees per second)
    dt = (curr_ts * 4.0) / 1000000.0;

    gyro_angles[0] = data.gyro[1] * dt;
    gyro_angles[1] = data.gyro[0] * dt;
    gyro_angles[2] = data.gyro[2] * dt;

    gyro_integral_angles[0] += gyro_angles[0];
    gyro_integral_angles[1] += gyro_angles[1];
    gyro_integral_angles[2] += gyro_angles[2];

    // apply complementary filter for pitch and roll
    angles[0] = (0.7 * (angles[0] + gyro_angles[0])) + 0.3 * accel_angles[0];
    angles[1] = (0.7 * (angles[1] + gyro_angles[1])) + 0.3 * accel_angles[1];
    angles[2] = angles[2] + gyro_angles[2]; // use only gyro for yaw

    // print results for arduplot.py script
    LOG_INFO(TEST, "%d,%d,%d,%d,%d,%d,%d,%d,%d",
        (int16_t)roundf(accel_angles[0]),
        (int16_t)roundf(gyro_integral_angles[0]),
        (int16_t)roundf(angles[0]),

        (int16_t)roundf(accel_angles[1]),
        (int16_t)roundf(gyro_integral_angles[1]),
        (int16_t)roundf(angles[1]),

        (int16_t)roundf(gyro_integral_angles[2]),
        (int16_t)roundf(angles[2]),
        curr_ts);


    gmd_delay(100);
}


TASK(STACK_SIZE);

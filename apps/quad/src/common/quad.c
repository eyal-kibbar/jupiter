#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include "io_platform.h"
#include "mpu.h"
#include "math.h"
#include <avr/io.h>


struct quad_s {
    float angles[3];
    uint16_t timer_us;
    uint16_t timer_ms;
};

static struct quad_s quad;

void setup()
{
    io_uart_init(0, 57600);
    io_logging_init();
    io_i2c_master_init();
}



void init()
{
    mpu_init();
    // initialize 8bit clock with 1024 prescaler
    // this gives us a 64 microseconds per tick: (16Mhz/1024)^-1
    TCCR2B = _BV(CS22) | _BV(CS11) | _BV(CS10);

    mpu_calibrate();

    TCNT2 = 0;
}

static float calc_angle(float x,float z)
{
    float res;

    res = atan2(x, z) * (180 / M_PI);

    return res;
}



static void quad_timer_sleep(uint16_t sleep_ms)
{
    uint16_t curr_us;
    uint16_t ms, us, ms_carry;

    // read & reset timer
    curr_us = TCNT2;
    TCNT2 = 0;

    curr_us *= (1024 / (F_CPU / 1000000)); // prescaler / CPU freq in microseconds

    ms = curr_us / 1000;
    us = curr_us - (ms * 1000);

    quad.timer_ms += ms;
    quad.timer_us += us;

    ms_carry = quad.timer_us / 1000;
    quad.timer_ms += ms_carry;
    quad.timer_us -= (ms_carry * 1000);

    if (sleep_ms > 0) {
        uint16_t time_slept_ms;

        // add the few microseconds of this function's calculations
        quad.timer_us += TCNT2 * (1024 / (F_CPU / 1000000));

        // sleep
        time_slept_ms = gmd_delay(sleep_ms);
        TCNT2 = 0; // clock lost its track after sleep due to overflow

        quad.timer_ms += time_slept_ms;

    }
}

static float quad_timer_clear()
{
    float dt;

    // calc elapsed time in seconds
    dt = (quad.timer_ms + (quad.timer_us / 1000.0)) / 1000.0;
    TCNT2 = 0;
    quad.timer_ms = 0;
    quad.timer_us = 0;

    return dt;
}

static void quad_wait_for_data()
{
    uint8_t val = 0;

    while (0 == (0x1 & (val = mpu_get_status()))) {
        LOG_INFO(TEST, "data not ready: %02x", val);
        quad_timer_sleep(10);
    }
}

void loop()
{
    mpu_data_t data;

    float accel_angles[3];
    float gyro_angles[3];

    uint8_t curr_ts;
    float dt;

    quad_wait_for_data();

    mpu_read(&data);
    dt = quad_timer_clear();

    accel_angles[0] = calc_angle(-data.accel[0], sqrt(data.accel[1]*data.accel[1] + data.accel[2]*data.accel[2]));
    accel_angles[1] = calc_angle(data.accel[1],  sqrt(data.accel[1]*data.accel[1] + data.accel[2]*data.accel[2]));

    gyro_angles[0] = data.gyro[1] * dt;
    gyro_angles[1] = data.gyro[0] * dt;
    gyro_angles[2] = data.gyro[2] * dt;


    // apply complementary filter for pitch and roll
    quad.angles[0] = (0.7 * (quad.angles[0] + gyro_angles[0])) + 0.3 * accel_angles[0];
    quad.angles[1] = (0.7 * (quad.angles[1] + gyro_angles[1])) + 0.3 * accel_angles[1];
    quad.angles[2] = quad.angles[2] + gyro_angles[2]; // use only gyro for yaw

    quad_timer_sleep(0); // flush current time to avoid timer overflow

    // print results for arduplot.py script
    LOG_INFO(TEST, "%d,%d,%d,%d,%d,%d",
        (int16_t)roundf(accel_angles[0]),
        (int16_t)roundf(quad.angles[0]),

        (int16_t)roundf(accel_angles[1]),
        (int16_t)roundf(quad.angles[1]),

        (int16_t)roundf(quad.angles[2]),
        (int16_t)roundf(dt * 1000));


    quad_timer_sleep(100);
}


TASK(STACK_SIZE);

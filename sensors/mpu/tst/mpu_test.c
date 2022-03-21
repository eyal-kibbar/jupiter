#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include "io_platform.h"
#include "mpu.h"
#include "math.h"
#include <avr/io.h>
#include "utils.h"

#define MPU_FILTER_COEF 0.98
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

    //mpu_calibrate();
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
int16_t prev_gyro[3];


mpu_rawdata_t raw_data[6];

void loop()
{
    uint8_t i;
    uint8_t sz = ARR_SIZE(raw_data);
    mpu_pipe_read(raw_data, &sz);

    for (i=0; i < sz; ++i) {
        mpu_data_t data;
        mpu_raw_parse(&raw_data[i], &data);
        LOG_INFO(MPU_TEST, "i:%d gz:%f az:%f", i, data.gyro[2], data.accel[2]);
    }


    gmd_delay(1000);
}


#if 0
char s[50];
void loop()
{
    int16_t x, y, z;
    int16_t gx, gy, gz;
    uint8_t val = 0;
    float accel_angles[3];
    float gyro_angles[3];
    uint16_t t1, t2;

    io_tx_t tx[] = {
        { .mode = IO_TX_MODE_W, .off = 0, .len = 2, .buf = "\r"},
        { .mode = IO_TX_MODE_W, .off = 0, .len = sizeof s, .buf = s},
    };


    uint16_t curr_ts;
    float dt, pitch, roll;

    if (0 == (0x1 & (val = mpu_get_status()))) {
        LOG_INFO(TEST, "data not ready: %02x", val);
        return;
    }

    // read and clear the clock
    curr_ts = TCNT1;
    TCNT1 = 0;
    t1 = 0;
    mpu_read(&data);
    t2 = TCNT1;

    accel_angles[0] = calc_angle(data.accel[1], data.accel[2]);
    accel_angles[1] = calc_angle(data.accel[0], data.accel[2]);

    // convert clock ticks to seconds (gyro output is in degrees per second)
    dt = (curr_ts * 4.0) / 1000000.0;

    // calculate the average of the last 2 readings for the integral
    gyro_angles[0] =  data.gyro[0] * dt;
    gyro_angles[1] = -data.gyro[1] * dt;
    gyro_angles[2] =  data.gyro[2] * dt;



    gyro_integral_angles[0] += gyro_angles[0];
    gyro_integral_angles[1] += gyro_angles[1];
    gyro_integral_angles[2] += gyro_angles[2];

    // fix yaw transfer

    pitch = angles[0];
    roll  = angles[1];
    angles[0] = (angles[0] + gyro_angles[0]) - (roll  * sin(gyro_angles[2] * (M_PI/180)));
    angles[1] = (angles[1] + gyro_angles[1]) + (angles[0] * sin(gyro_angles[2] * (M_PI/180)));

    // apply complementary filter for pitch and roll
    angles[0] = (MPU_FILTER_COEF * angles[0]) + (1-MPU_FILTER_COEF) * accel_angles[0];
    angles[1] = (MPU_FILTER_COEF * angles[1]) + (1-MPU_FILTER_COEF) * accel_angles[1];
    angles[2] = angles[2] + gyro_angles[2]; // use only gyro for yaw

    snprintf(s, sizeof s, "%03.03f   %03.03f   %03.03f    %-5d    X", angles[0], angles[1], angles[2], t2 - t1);
    io_uart_sg(0, tx, ARR_SIZE(tx), 0);

    // print results for arduplot.py script
    /*
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
        */

    gmd_delay(10);
}

#endif
TASK(STACK_SIZE);

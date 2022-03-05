#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include "io_platform.h"
#include "mpu.h"
#include "math.h"
#include "pid.h"
#include "failsafe.h"
#include <avr/io.h>
#include "servo.h"
#include "utils.h"
#include "quad.h"
#include <string.h>


#define QUAD_ANGLE_FILTER_ALPHA 0.90





struct quad_s quad;

void setup()
{
    io_uart_init(0, 57600);
    io_logging_init();

    io_i2c_master_init();
    failsafe_init(3, quad_stop);
    servo_init();

    quad.pid_cfg.kp = 0.4;
    quad.pid_cfg.ki = 0.03;
    quad.pid_cfg.kd = 4.0;
    quad.pid_cfg.output_limit = 400;

    quad.pid_yaw_cfg.kp = 1;
    quad.pid_yaw_cfg.ki = 0.00;
    quad.pid_yaw_cfg.kd = 0;
    quad.pid_yaw_cfg.output_limit = 200;
}

void quad_angles_get(float rpy[3], float* dt)
{
    memcpy(rpy,  quad.angles, sizeof(quad.angles));
    *dt = quad.dt;
}

void quad_get_servo_microseconds(int16_t motor[4])
{
    memcpy(motor,  quad.motor, sizeof(quad.motor));
}

void init()
{
    servo_attach(4);
    servo_attach(5);
    servo_attach(6);
    servo_attach(7);

    servo_set_mircoseconds(4, 2000);
    servo_set_mircoseconds(5, 2000);
    servo_set_mircoseconds(6, 2000);
    servo_set_mircoseconds(7, 2000);

    gmd_delay(2000);

    servo_set_mircoseconds(4, 1000);
    servo_set_mircoseconds(5, 1000);
    servo_set_mircoseconds(6, 1000);
    servo_set_mircoseconds(7, 1000);

    gmd_delay(5000);

    mpu_init();

    // initialize 8bit clock with 1024 prescaler
    // this gives us a 64 microseconds per tick: (16Mhz/1024)^-1
    TCCR2B = _BV(CS22) | _BV(CS11) | _BV(CS10);

    LOG_INFO(QUAD, "calibrating MPU");
    mpu_calibrate();

    TCNT2 = 0;
    //quad_start();

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
    //LOG_INFO(TEST, "current time: %d", quad.timer_ms);
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
        LOG_INFO(QUAD, "data not ready: %02x", val);
        quad_timer_sleep(10);
    }
}

static void quad_mixer(float pid_output[3], float pid_throttle)
{
    uint8_t i;
    // quad +
    //         throtte        roll           pitch           yaw
    quad.motor[0] = pid_throttle                 + pid_output[1] - pid_output[2]; //REAR
    quad.motor[1] = pid_throttle - pid_output[0]                 + pid_output[2]; //RIGHT
    quad.motor[2] = pid_throttle + pid_output[0]                 + pid_output[2]; //LEFT
    quad.motor[3] = pid_throttle                 - pid_output[1] - pid_output[2]; //FRONT



    // quad x
    /*
    quad.motor[0] = pid_throttle - pid_output[0] + pid_output[1] - pid_output[2]; //REAR_R
    quad.motor[1] = pid_throttle - pid_output[0] - pid_output[1] + pid_output[2]; //FRONT_R
    quad.motor[2] = pid_throttle + pid_output[0] + pid_output[1] + pid_output[2]; //REAR_L
    quad.motor[3] = pid_throttle + pid_output[0] - pid_output[1] - pid_output[2]; //FRONT_L
    */

    for (i=0; i < 4; ++i) {
        quad.motor[i] = MIN(quad.motor[i], 2000);
        quad.motor[i] = MAX(quad.motor[i], 1100);
    }

    /*
    LOG_INFO(TEST, "%4d %4d %4d %4d",
        quad.motor[0],
        quad.motor[1],
        quad.motor[2],
        quad.motor[3]
    );


    LOG_INFO(TEST, "%4d %4d   %03d",
        quad.motor[0],
        quad.motor[3],
        (int16_t)roundf(quad.angles[1])
    );
    */
    /*
    servo_set_mircoseconds(4, 1000);
    servo_set_mircoseconds(5, 1000);
    servo_set_mircoseconds(6, 1000);
    servo_set_mircoseconds(7, 1000);
    */
    servo_set_mircoseconds(4, quad.motor[0]);
    servo_set_mircoseconds(5, quad.motor[1]);
    servo_set_mircoseconds(6, quad.motor[2]);
    servo_set_mircoseconds(7, quad.motor[3]);


}



void loop()
{
    float accel_angles[3];
    float gyro_angles[3];
    float pid_output[3];
    mpu_data_t data;
    uint8_t curr_ts;
    float dt, yaw_sin;
    float pitch, roll;

    //failsafe_reset(0);


    quad_wait_for_data();

    mpu_read(&data);
    dt = quad_timer_clear();
    quad.dt = dt;

    quad_timer_sleep(0);
    accel_angles[0] = calc_angle(data.accel[1], data.accel[2]);
    accel_angles[1] = calc_angle(data.accel[0], data.accel[2]);

    gyro_angles[0] = data.gyro[0] * dt;
    gyro_angles[1] = -data.gyro[1] * dt;
    gyro_angles[2] = data.gyro[2] * dt;

    // fix yaw transfer
    yaw_sin = sin(gyro_angles[2] * (M_PI/180));

    pitch = quad.angles[0];
    roll = quad.angles[1];
    quad.angles[0] = (quad.angles[0] + gyro_angles[0]) - (roll * yaw_sin);
    quad.angles[1] = (quad.angles[1] + gyro_angles[1]) + (pitch * yaw_sin);

    // apply complementary filter for pitch and roll
    quad.angles[0] = (QUAD_ANGLE_FILTER_ALPHA * quad.angles[0]) + ((1-QUAD_ANGLE_FILTER_ALPHA) * accel_angles[0]);
    quad.angles[1] = (QUAD_ANGLE_FILTER_ALPHA * quad.angles[1]) + ((1-QUAD_ANGLE_FILTER_ALPHA) * accel_angles[1]);
    quad.angles[2] = quad.angles[2] + gyro_angles[2]; // use only gyro for yaw



    quad_timer_sleep(0); // flush current time to avoid timer overflow

    //quad.setpoint[0] = quad.setpoint[1] = quad.setpoint[2] = 0;

    if (quad.state == QUAD_BALANCING) {
        pid_output[0] = pid_step(&quad.pid_cfg,     &quad.pid_roll_state,  quad.angles[0], 1, quad.setpoint[0]);
        pid_output[1] = pid_step(&quad.pid_cfg,     &quad.pid_pitch_state, quad.angles[1], 1, quad.setpoint[1]);
        //pid_output[2] = pid_step(&quad.pid_yaw_cfg, &quad.pid_yaw_state,   quad.angles[2], 1, quad.setpoint[2]);
        pid_output[2]=0;
        /*
        pid_output[0] = pid_step(&quad.pid_cfg, &quad.pid_roll_state,  quad.angles[0], dt, quad.setpoint[0]);
        pid_output[1] = pid_step(&quad.pid_cfg, &quad.pid_pitch_state, 0, dt, quad.setpoint[1]);
        pid_output[2] = pid_step(&quad.pid_cfg, &quad.pid_yaw_state,   0, dt, quad.setpoint[2]);
        */
        quad_timer_sleep(0);
        quad_mixer(pid_output, quad.throtte);
    }

    quad_timer_sleep(10);

    /*
    LOG_INFO(QUAD, "%03d %03d %03d",
        (int16_t)roundf(quad.angles[0]),
        (int16_t)roundf(quad.angles[1]),
        (int16_t)roundf(quad.angles[2]));
    */
}

void quad_start()
{
    quad.state = QUAD_BALANCING;
}

void quad_stop()
{
    quad.state = QUAD_INIT;
    servo_set_mircoseconds(4, 1000);
    servo_set_mircoseconds(5, 1000);
    servo_set_mircoseconds(6, 1000);
    servo_set_mircoseconds(7, 1000);
}

void quad_set_setpoint(float rpy[3], float throtte)
{
    memcpy(quad.setpoint, rpy, sizeof quad.setpoint);
    quad.throtte = (throtte * 800) + 1100;
}

TASK(STACK_SIZE);

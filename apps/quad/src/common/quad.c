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

enum quad_state_e {
    QUAD_INIT,
    QUAD_BALANCING
};

struct quad_s {
    enum quad_state_e state;

    float setpoint[3];
    float throtte;

    float angles[3];
    uint16_t timer_us;
    uint16_t timer_ms;

    pid_config_t pid_cfg;
    pid_config_t pid_yaw_cfg;

    pid_state_t pid_roll_state;
    pid_state_t pid_pitch_state;
    pid_state_t pid_yaw_state;
};

static struct quad_s quad;

void setup()
{
    io_uart_init(0, 57600);
    io_logging_init();
    io_i2c_master_init();
    failsafe_init(3);

    quad.pid_cfg.kp = 1;
    quad.pid_cfg.ki = 0.5;
    quad.pid_cfg.kd = 3;
    quad.pid_cfg.output_limit = 400;

    quad.pid_yaw_cfg.kp = 1;
    quad.pid_yaw_cfg.ki = 0.5;
    quad.pid_yaw_cfg.kd = 0;
    quad.pid_cfg.output_limit = 400;
}



void init()
{
    mpu_init();
    servo_init();

    servo_attach(4);
    servo_attach(5);
    servo_attach(6);
    servo_attach(7);

    // initialize 8bit clock with 1024 prescaler
    // this gives us a 64 microseconds per tick: (16Mhz/1024)^-1
    TCCR2B = _BV(CS22) | _BV(CS11) | _BV(CS10);

    LOG_INFO(QUAD, "calibrating MPU");
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

static uint16_t quad_set_servo(float pid_output[3], float pid_throttle)
{
    uint8_t i;
    int16_t motor[4];

    // quad +
    //         throtte        roll           pitch           yaw
    motor[0] = pid_throttle                 + pid_output[1] - pid_output[2]; //REAR
    motor[1] = pid_throttle - pid_output[0]                 + pid_output[2]; //RIGHT
    motor[2] = pid_throttle + pid_output[0]                 + pid_output[2]; //LEFT
    motor[3] = pid_throttle                 - pid_output[1] - pid_output[2]; //FRONT



    // quad x
    /*
    motor[0] = pid_throttle - pid_output[0] + pid_output[1] - pid_output[2]; //REAR_R
    motor[1] = pid_throttle - pid_output[0] - pid_output[1] + pid_output[2]; //FRONT_R
    motor[2] = pid_throttle + pid_output[0] + pid_output[1] + pid_output[2]; //REAR_L
    motor[3] = pid_throttle + pid_output[0] - pid_output[1] - pid_output[2]; //FRONT_L
    */

    for (i=0; i < 4; ++i) {
        motor[i] = MIN(motor[i], 2000);
        motor[i] = MAX(motor[i], 1100);
    }

    LOG_INFO(TEST, "%4d %4d %4d %4d",
        motor[0],
        motor[1],
        motor[2],
        motor[3]
    );

    servo_set_mircoseconds(4, motor[0]);
    servo_set_mircoseconds(5, motor[1]);
    servo_set_mircoseconds(6, motor[2]);
    servo_set_mircoseconds(7, motor[3]);
}



void loop()
{
    float accel_angles[3];
    float gyro_angles[3];
    float pid_output[3];
    mpu_data_t data;
    uint8_t curr_ts;
    float dt;

    failsafe_reset(0);


    if (quad.state != QUAD_BALANCING) {
        gmd_delay(100);
        quad_timer_clear();
        return;
    }

    quad_wait_for_data();

    mpu_read(&data);
    dt = quad_timer_clear();

    quad_timer_sleep(0);
    accel_angles[0] = calc_angle(-data.accel[0], data.accel[2]);
    accel_angles[1] = calc_angle(data.accel[1],  data.accel[2]);

    gyro_angles[0] = data.gyro[1] * dt;
    gyro_angles[1] = data.gyro[0] * dt;
    gyro_angles[2] = data.gyro[2] * dt;


    // apply complementary filter for pitch and roll
    quad.angles[0] = (0.7 * (quad.angles[0] + gyro_angles[0])) + 0.3 * accel_angles[0];
    quad.angles[1] = (0.7 * (quad.angles[1] + gyro_angles[1])) + 0.3 * accel_angles[1];
    quad.angles[2] = quad.angles[2] + gyro_angles[2]; // use only gyro for yaw

    quad_timer_sleep(0); // flush current time to avoid timer overflow

    pid_output[0] = pid_step(&quad.pid_cfg, &quad.pid_roll_state,  quad.angles[0], dt, quad.setpoint[0]);
    pid_output[1] = pid_step(&quad.pid_cfg, &quad.pid_pitch_state, quad.angles[1], dt, quad.setpoint[1]);
    pid_output[2] = pid_step(&quad.pid_cfg, &quad.pid_yaw_state,   quad.angles[2], dt, quad.setpoint[2]);

    quad_timer_sleep(0);
    quad_set_servo(pid_output, quad.throtte);
    quad_timer_sleep(0);
    //LOG_INFO(TEST, "%d %d", (int16_t)roundf(output_roll), (int16_t)roundf(dt * 1000));

    quad_timer_sleep(100);
}

void quad_start()
{
    quad.state = QUAD_BALANCING;
}

void quad_stop()
{
    quad.state = QUAD_INIT;
    servo_set_mircoseconds(4, 0);
    servo_set_mircoseconds(5, 0);
    servo_set_mircoseconds(6, 0);
    servo_set_mircoseconds(7, 0);
}

void quad_set_setpoint(float rpy[3], float throtte)
{
    memcpy(quad.setpoint, rpy, sizeof quad.setpoint);
    quad.throtte = (throtte * 1000) + 1000;
}

TASK(STACK_SIZE);

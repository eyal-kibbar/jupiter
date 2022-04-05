#include "ganymede.h"
#include "airplane.h"
#include "arrow.h"
#include "io.h"
#include "logging.h"
#include "servo.h"
#include "pid.h"
#include "mpu.h"

#include <stdlib.h>

#define ARROW_PIN_ENGINE 4
#define ARROW_PIN_LEFT 5
#define ARROW_PIN_RIGHT 6

#define SERVO_MIN_US 500
#define SERVO_MAX_US 2000

#define SERVO_RANGE_US 200

struct arrow_s {
    uint16_t roll;
    uint16_t pitch;
    uint16_t throttle;

    uint8_t auto_balance_enabled;

    pid_config_t pid_cfg;
    pid_state_t pid_d_state;
    pid_state_t pid_y_state;
};

struct arrow_calib_s {
    uint16_t left_lvl_us;
    uint16_t right_lvl_us;
};

struct arrow_s arrow;
struct arrow_calib_s arrow_calib = {
    .left_lvl_us = 1560,
    .right_lvl_us = 1400
};

static void arrow_stop();


void setup()
{
    io_uart_init(0, 57600);
    io_logging_init();
    io_i2c_master_init();
    //failsafe_init(3, arrow_stop);
    servo_init();

    arrow.pid_cfg.kp = 20;
    arrow.pid_cfg.ki = 1;
    arrow.pid_cfg.kd = 0;
    arrow.pid_cfg.output_limit = 512;


}

void init()
{
    LOG_INFO(ARROW, "init");
    servo_attach(ARROW_PIN_ENGINE);
    servo_attach(ARROW_PIN_LEFT);
    servo_attach(ARROW_PIN_RIGHT);

    /*
    servo_set_mircoseconds(ARROW_PIN_ENGINE, 2000);
    gmd_delay(2000);
    servo_set_mircoseconds(ARROW_PIN_ENGINE, 1000);
    gmd_delay(5000);
    */
    LOG_INFO(ARROW, "init mpu");
    mpu_init();
    mpu_calibrate();
    LOG_INFO(ARROW, "ready");
}

static void arrow_stop()
{
    servo_set_mircoseconds(ARROW_PIN_ENGINE, 0);
}

static uint16_t calc_servo_microseconds(int16_t offset, uint16_t lvl_us)
{
    int16_t us;
    us = (int16_t)interp(offset, -1024, 1024, -SERVO_RANGE_US, SERVO_RANGE_US);
    us += lvl_us;
    return (uint16_t)us;
}

void loop()
{
    int16_t offset;
    int16_t left_us, right_us;
    int16_t d, y;
    float ypr[3];


    mpu_ypr(ypr);


    if (arrow.auto_balance_enabled) {
        // auto balance
        float setpoint_roll  = interp(arrow.roll, 0, 1024, -30, 30);
        float setpoint_pitch = interp(arrow.pitch, 0, 1024, -30, 30);

        d = (int16_t)pid_step(&arrow.pid_cfg, &arrow.pid_d_state, ypr[2], setpoint_roll);
        //y = (int16_t)pid_step(&arrow.pid_cfg, &arrow.pid_y_state, ypr[1], setpoint_pitch);
        y = (int16_t)arrow.pitch - 512;
    }
    else {
        d = (int16_t)arrow.roll - 512;
        y = (int16_t)arrow.pitch - 512;
    }



    left_us  = calc_servo_microseconds(-d + y, arrow_calib.left_lvl_us);
    right_us = calc_servo_microseconds(-d - y, arrow_calib.right_lvl_us);

    servo_set_mircoseconds(ARROW_PIN_LEFT,  left_us);
    servo_set_mircoseconds(ARROW_PIN_RIGHT, right_us);
    servo_set_mircoseconds(ARROW_PIN_ENGINE, arrow.throttle);

    /*
    LOG_INFO(ARROW, "roll: %04d, pitch: %04d, throttle: %04d l: %04d r: %04d lvl: %d d=%d y=%d",
        arrow.roll, arrow.pitch, arrow.throttle, left_us, right_us, arrow_calib.left_lvl_us,d,y);
    */
    gmd_delay(40);
}

void arrow_autobalance(uint8_t enabled)
{
    arrow.auto_balance_enabled = enabled;
}
void arrow_set(uint16_t roll, uint16_t pitch, uint16_t throttle)
{
    arrow.roll = roll;
    arrow.pitch = pitch;
    arrow.throttle = throttle;
}


TASK(STACK_SIZE);

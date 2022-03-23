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



struct quad_s quad;

void setup()
{
    io_uart_init(0, 57600);
    io_logging_init();


    io_i2c_master_init();
    failsafe_init(3, quad_stop);
    servo_init();

    quad.pid_cfg.kp = 1.0;
    quad.pid_cfg.ki = 0.01;
    quad.pid_cfg.kd = 0.1;
    quad.pid_cfg.output_limit = 400;

    quad.pid_yaw_cfg.kp = 0.1;
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
    LOG_INFO(QUAD, "init");
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
    //TCCR2B = _BV(CS22) | _BV(CS11) | _BV(CS10);

    LOG_INFO(QUAD, "calibrating MPU");
    mpu_calibrate();

    //TCNT2 = 0;
    //quad_start();

    LOG_INFO(QUAD, "ready");
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


    LOG_INFO(TEST, "%4d %4d %4d %4d",
        quad.motor[0],
        quad.motor[1],
        quad.motor[2],
        quad.motor[3]
    );

    /*
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
    float ypr[3];
    float pid_output[3];
    //failsafe_reset(0);

    mpu_ypr(ypr);
    //LOG_INFO(QUAD, "(%03d, %03d, %03d)", (int16_t)ypr[0], (int16_t)ypr[1], (int16_t)ypr[2]);

    if (quad.state == QUAD_BALANCING) {
        pid_output[0] = pid_step(&quad.pid_cfg,     &quad.pid_roll_state,  ypr[2], quad.setpoint[0]);
        pid_output[1] = pid_step(&quad.pid_cfg,     &quad.pid_pitch_state, ypr[1], quad.setpoint[1]);
        pid_output[2] = pid_step(&quad.pid_yaw_cfg, &quad.pid_yaw_state,   ypr[0], quad.setpoint[2]);
        //pid_output[2]=0;
        /*
        pid_output[0] = pid_step(&quad.pid_cfg, &quad.pid_roll_state,  quad.angles[0], dt, quad.setpoint[0]);
        pid_output[1] = pid_step(&quad.pid_cfg, &quad.pid_pitch_state, 0, dt, quad.setpoint[1]);
        pid_output[2] = pid_step(&quad.pid_cfg, &quad.pid_yaw_state,   0, dt, quad.setpoint[2]);
        */
        quad_mixer(pid_output, quad.throtte);
    }

    gmd_delay(20);
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
    LOG_INFO(QUAD, "stopped");
}

void quad_set_setpoint(float rpy[3], float throtte)
{
    memcpy(quad.setpoint, rpy, sizeof quad.setpoint);
    quad.throtte = (throtte * 800) + 1100;
}

TASK(STACK_SIZE);

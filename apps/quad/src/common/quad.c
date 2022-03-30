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
    failsafe_init(2, quad_stop);
    servo_init();

    quad.pid_cfg.kp = 1.5;
    quad.pid_cfg.ki = 0.1;
    quad.pid_cfg.kd = 2.0;
    quad.pid_cfg.output_limit = 400;

    quad.pid_yaw_cfg.kp = 0.1;
    quad.pid_yaw_cfg.ki = 0.0;
    quad.pid_yaw_cfg.kd = 0.0;
    quad.pid_yaw_cfg.output_limit = 200;

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

#ifndef DEBUG
    gmd_delay(2000);
#endif
    servo_set_mircoseconds(4, 1000);
    servo_set_mircoseconds(5, 1000);
    servo_set_mircoseconds(6, 1000);
    servo_set_mircoseconds(7, 1000);

#ifndef DEBUG
    gmd_delay(5000);
#endif

    //LOG_INFO(QUAD, "mpu init");
    mpu_init();


    LOG_INFO(QUAD, "calibrating MPU");
    mpu_calibrate();

    quad.state = QUAD_READY;
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

    /*
    LOG_INFO(TEST, "%4d %4d %4d %4d",
        quad.motor[0],
        quad.motor[1],
        quad.motor[2],
        quad.motor[3]
    );
    */

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
char s[80];

void loop()
{
    float ypr[3];
    float pid_output[3];
    io_tx_t tx[1] = {
        {.mode = IO_TX_MODE_W, .len = sizeof(s), .off = 0, .buf = s}
    };

    //TCNT2 = 0;

    failsafe_reset(0);


    mpu_ypr(ypr);

    //LOG_INFO(QUAD, "(%03d, %03d, %03d)", (int16_t)ypr[0], (int16_t)ypr[1], (int16_t)ypr[2]);

    if (quad.state == QUAD_BALANCING) {
        pid_output[0] = pid_step(&quad.pid_cfg,     &quad.pid_roll_state,  ypr[2], quad.setpoint[0]);
        pid_output[1] = pid_step(&quad.pid_cfg,     &quad.pid_pitch_state, ypr[1], quad.setpoint[1]);
        pid_output[2] = pid_step(&quad.pid_yaw_cfg, &quad.pid_yaw_state,   ypr[0], quad.setpoint[2]);
        //pid_output[2]=0;

        quad_mixer(pid_output, quad.throtte);
    }


#ifdef DEBUG
    {

        snprintf(s, sizeof(s)-1, "(%03.02f, %03.02f, %03.02f) ,%d,%d,%d,%d  \n\r",
            ypr[0], ypr[1], ypr[2],
            quad.motor[0], quad.motor[1], quad.motor[2], quad.motor[3]
        );
        io_uart_sg(0, tx, ARR_SIZE(tx), 0);
    }
#endif


    gmd_delay(40);
}

void quad_start()
{
    quad.state = QUAD_BALANCING;
}

void quad_stop()
{
    if (quad.state != QUAD_READY) {
        quad.state = QUAD_READY;
        LOG_INFO(QUAD, "stopped");
    }
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
